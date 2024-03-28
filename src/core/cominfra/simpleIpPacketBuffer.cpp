/*******************************************************************************
 * Copyright (c) 2024 Mario Kastner
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Mario Kastner - initial implementation
 *******************************************************************************/

#include <simpleIpPacketBuffer.h>


using namespace forte::com_infra;

bool CSimpleIPComPacketBuffer::enqueuePacket(char *paData, unsigned int paSize, CIPComSocketHandler::TSocketDescriptor paClientID) {
  if (mPacketQueue.size() >= cm_packetBufferQueueSize || paData == nullptr || paSize == 0) {
    return false;
  }
  mPacketQueue.emplace(paData, paSize, paClientID);
  return true;
}

CIPComSocketHandler::TSocketDescriptor CSimpleIPComPacketBuffer::dequeuePacket(std::unique_ptr<char[]>& paDestinationData, unsigned int& packetSize) {
  CComPacket &targetPacket = mPacketQueue.front();
  const CIPComSocketHandler::TSocketDescriptor targetClientID = targetPacket.mClientID;
  paDestinationData = std::move(targetPacket.mData);
  packetSize = targetPacket.mSize;
  mPacketQueue.pop();
  return targetClientID;
}

bool CSimpleIPComPacketBuffer::empty() {
  return mPacketQueue.empty();
}


