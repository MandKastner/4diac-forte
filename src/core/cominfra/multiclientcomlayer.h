

#ifndef FORTE_MULTICLIENTCOMLAYER_H
#define FORTE_MULTICLIENTCOMLAYER_H

#include <sockhand.h>
#include <forte_config.h>
#include "comlayer.h"
#include "simpleIpPacketBuffer.h"


namespace forte::com_infra {

        class CMultiClientComLayer : public CComLayer{
        public:
            CMultiClientComLayer(CComLayer* paUpperLayer, CBaseCommFB* paComFB);
            ~CMultiClientComLayer() override;

            EComResponse sendData(void *paData, unsigned int paSize) override;
            EComResponse recvData(const void *paData, unsigned int paSize) override;

            EComResponse processInterrupt() override;

        protected:
            CIPComSocketHandler::TSocketDescriptor mListeningID;
            CIPComSocketHandler::TSocketDescriptor mActiveClientID; //send ID of client (determined in process Intrerrupt)
            std::vector<CIPComSocketHandler::TSocketDescriptor> mClientIDs; //necessary to close all the connections
            const size_t cmMaxClientNum = 5; //todo use reserve in vector to avoid copies and move to cmake config


        private:
            void closeSocket(CIPComSocketHandler::TSocketDescriptor &paSocketID);

            EComResponse openConnection(char *paLayerParameter) override;
            void closeConnection() override;

            void handleConnectedDataRecv(CIPComSocketHandler::TSocketDescriptor clientID);
            void updateConnectionState();

            //client management
            bool registerClient(CIPComSocketHandler::TSocketDescriptor paClientID);
            bool deregisterClient(CIPComSocketHandler::TSocketDescriptor paClientID);
            bool clientIsConnected(CIPComSocketHandler::TSocketDescriptor clientID);


            EComResponse mInterruptResp;
            char mRecvBuffer[cgIPLayerRecvBufferSize]{};
            unsigned int mBufFillSize;
            forte::com_infra::CSimpleIPComPacketBuffer mPacketBuffer;
        };

    }



#endif //FORTE_MULTICLIENTCOMLAYER_H
