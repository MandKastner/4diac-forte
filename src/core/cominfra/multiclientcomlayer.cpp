#include "multiclientcomlayer.h"

#include "devlog.h"
#include "commfb.h"
#include <forte_thread.h>
#include "parameterParser.h"
#include "simpleIpPacketBuffer.h"


using namespace forte::com_infra;

CMultiClientComLayer::CMultiClientComLayer(CComLayer *paUpperLayer, CBaseCommFB *paComFB) :
  CComLayer(paUpperLayer, paComFB),
  mListeningID(CIPComSocketHandler::scmInvalidSocketDescriptor),
  mActiveClientID(CIPComSocketHandler::scmInvalidSocketDescriptor),
  mInterruptResp(e_Nothing),
  mRecvBuffer{0},
  mBufFillSize(0){
  mClientIDs.reserve(cmMaxClientNum);
}

CMultiClientComLayer::~CMultiClientComLayer() = default;

EComResponse CMultiClientComLayer::sendData(void *paData, unsigned int paSize) {
  EComResponse eRetVal = e_ProcessDataOk;
  if (mActiveClientID != CIPComSocketHandler::scmInvalidSocketDescriptor) {
    if (CIPComSocketHandler::sendDataOnTCP(mActiveClientID, static_cast<char*>(paData), paSize) < 0) {
      deregisterClient(mActiveClientID);
      eRetVal = e_InitTerminated;
    }
  }
  return eRetVal;
}

EComResponse CMultiClientComLayer::recvData(const void *paData, unsigned int) {
  mInterruptResp = e_Nothing;
  const CIPComSocketHandler::TSocketDescriptor requestID = *(static_cast<const CIPComSocketHandler::TSocketDescriptor *>(paData));

  if (mConnectionState == e_Disconnected) {
    return mInterruptResp;
  }

  if (clientIsConnected(requestID)) { //client is already connected
    handleConnectedDataRecv(requestID);
  } else if (requestID == mListeningID) { // new client connection
    const CIPComSocketHandler::TSocketDescriptor clientID = CIPComSocketHandler::acceptTCPConnection(mListeningID);
    registerClient(clientID);
  }
  return mInterruptResp;
}



void CMultiClientComLayer::handleConnectedDataRecv(CIPComSocketHandler::TSocketDescriptor paClientID){

  if(CIPComSocketHandler::scmInvalidSocketDescriptor != paClientID){
    CCriticalRegion criticalRegion(mFb->getFBLock());
    int nRetVal = 0;

    nRetVal = CIPComSocketHandler::receiveDataFromTCP(paClientID, &mRecvBuffer[mBufFillSize], cgIPLayerRecvBufferSize-mBufFillSize);

    switch (nRetVal){
      case 0:
        DEVLOG_INFO("[MULTICLIENTCOMLAYER] Connection closed by peer\n");
        mInterruptResp = e_InitTerminated;
        deregisterClient(paClientID);
        break;
      case -1:
        mInterruptResp = e_ProcessDataRecvFaild;
        //todo deregisterClient(paClientID) here on error?
        break;
      default:
        //we successfully received data
        mBufFillSize += static_cast<unsigned int>(nRetVal);
        mPacketBuffer.enqueuePacket(mRecvBuffer, static_cast<unsigned int>(mBufFillSize), paClientID);
        mInterruptResp = e_ProcessDataOk;
        mBufFillSize = 0;
        break;
    }
    mFb->interruptCommFB(this);
  }
}

EComResponse CMultiClientComLayer::processInterrupt() {
  if(e_ProcessDataOk == mInterruptResp){  //todo peer sided termination does not work using interrupt response here
    //todo --> look if client is still connected and get rid of interrupt response or return interrupt response e_ProcessDataOk if client is still connected..
      if (mConnectionState == e_Connected || mConnectionState == e_ConnectedAndListening) {
        if((nullptr != mTopLayer && !mPacketBuffer.empty())){
          CCriticalRegion criticalRegion(mFb->getFBLock());
          std::unique_ptr<char[]> data;
          unsigned int size;
          mActiveClientID = mPacketBuffer.dequeuePacket(data, size);
          if (size > 0) {
            mInterruptResp = mTopLayer->recvData(data.get(),size);
          }
        }
      }
  }
  return mInterruptResp;
}

void CMultiClientComLayer::closeSocket(CIPComSocketHandler::TSocketDescriptor &paSocketID) {
  if(CIPComSocketHandler::scmInvalidSocketDescriptor != paSocketID){
    getExtEvHandler<CIPComSocketHandler>().removeComCallback(paSocketID);
    CIPComSocketHandler::closeSocket(paSocketID);
    paSocketID = CIPComSocketHandler::scmInvalidSocketDescriptor;
  }
}

EComResponse CMultiClientComLayer::openConnection(char *paLayerParameter) {
  mConnectionState = e_Disconnected;
  DEVLOG_INFO("[MULTICLIENTCOMLAYER] Opening Conenction\r\n");

  if (mFb != nullptr && mFb->getComServiceType() != EComServiceType::e_Server) {
    DEVLOG_INFO("[MULTICLIENTCOMLAYER] This communication layer can only be used with Server FB Types!\r\n");
    return EComResponse::e_InitTerminated;
  }

  CParameterParser parser(paLayerParameter,':',2);
  const size_t numParams = parser.parseParameters();
  const auto port = static_cast<TForteUInt16>(forte::core::util::strtoul(parser[1], nullptr, 10));
  mListeningID = CIPComSocketHandler::openTCPServerConnection(const_cast<char *>(parser[0]), port);

  if (numParams != 2 || mListeningID == CIPComSocketHandler::scmInvalidSocketDescriptor) {
    return e_InitTerminated;
  }

  getExtEvHandler<CIPComSocketHandler>().addComCallback(mListeningID, this);
  mConnectionState = e_Listening;
  return e_InitOk;
}

void CMultiClientComLayer::closeConnection() {
  CCriticalRegion criticalRegion(mFb->getFBLock());
  for (CIPComSocketHandler::TSocketDescriptor fd : mClientIDs) {
    closeSocket(fd);
  }
  closeSocket(mListeningID);
  mConnectionState = e_Disconnected;
  mClientIDs.clear();
}


/*
 * CLIENT MANAGEMENT
 */

bool CMultiClientComLayer::registerClient(CIPComSocketHandler::TSocketDescriptor paClientID) {
  CCriticalRegion criticalRegion(mFb->getFBLock());

  if (mClientIDs.size() >= cmMaxClientNum || paClientID == CIPComSocketHandler::scmInvalidSocketDescriptor) {
    return false;
  }

  auto it = std::lower_bound(mClientIDs.begin(), mClientIDs.end(), paClientID);
  mClientIDs.insert(it, paClientID);
  getExtEvHandler<CIPComSocketHandler>().addComCallback(paClientID, this);
  updateConnectionState();
  DEVLOG_INFO("[MULTICLIENTCOMLAYER] New client registered! Number of clients: %d\r\n", mClientIDs.size());
  return true;
}

bool CMultiClientComLayer::deregisterClient(CIPComSocketHandler::TSocketDescriptor paClientID) {
  CCriticalRegion criticalRegion(mFb->getFBLock());
  auto it = std::lower_bound(mClientIDs.begin(), mClientIDs.end(), paClientID); //first element not less than
  if (it != mClientIDs.end() && *it == paClientID) {
    mClientIDs.erase(it);
    closeSocket (paClientID);
    if (*it == mActiveClientID) {
      mActiveClientID = CIPComSocketHandler::scmInvalidSocketDescriptor;
    }
    updateConnectionState();
    DEVLOG_INFO("Client has been removed. Number of clients: %d\r\n", mClientIDs.size());
    return true;
  }
  return false;
}

void CMultiClientComLayer::updateConnectionState() {
  mConnectionState = mClientIDs.empty() ? e_Listening : e_ConnectedAndListening;
  if (mClientIDs.size() == cmMaxClientNum) {
    mConnectionState = e_Connected;
  }
}

bool CMultiClientComLayer::clientIsConnected(CIPComSocketHandler::TSocketDescriptor clientID) {
  return std::any_of(mClientIDs.begin(), mClientIDs.end(),[clientID](const auto& id) {
    return id == clientID;
  });
}


