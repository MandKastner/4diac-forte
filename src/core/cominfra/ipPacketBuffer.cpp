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

#include <ipPacketBuffer.h>

using namespace forte::com_infra;


bool CIPComPacketBuffer::enqueuePacket(char arr[], unsigned int paSize) {
  if (mPacketQueue.size() >= cm_packetBufferQueueSize || arr == nullptr || paSize == 0) {
    return false;
  }

  mPacketQueue.push(CIPComPacketBuffer::CComPacket(arr, paSize));
  return true;
}

void CIPComPacketBuffer::dequeuePacket() {
  if (!mPacketQueue.empty()) {
    delete[] mPacketQueue.front().mData;
  }
  mPacketQueue.pop();
}

unsigned int CIPComPacketBuffer::copyQueueHead(char arr[]) {
  memcpy(arr, mPacketQueue.front().mData, mPacketQueue.front().mSize);
  return mPacketQueue.front().mSize;
}


bool CIPComPacketBuffer::empty() {
  return mPacketQueue.empty();
}


