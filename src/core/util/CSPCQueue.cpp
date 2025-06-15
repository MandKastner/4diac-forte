/*******************************************************************************
 * Copyright (c) 2025 Mario Kastner
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Mario Kastner
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "CSPCQueue.h"


bool CSPSCQueue::push(CComLayer* paComlayer) {
  size_t tail = mTail.load(std::memory_order_relaxed);
  const size_t nextTail = (tail + 1) % cgCommunicationInterruptQueueSize;

  if (nextTail == mHead.load(std::memory_order_acquire)) {
    return false;
  }

  mBuffer[tail] = paComlayer;
  mTail.store(nextTail, std::memory_order_release);
  mSize.fetch_add(1, std::memory_order_acq_rel);
  return true;
}

CComLayer* CSPSCQueue::pop() {
  size_t head = mHead.load(std::memory_order_relaxed);
  if (head == mTail.load(std::memory_order_acquire)) {
    return nullptr;
  }

  auto comLayer = mBuffer[head];
  mHead.store((head + 1) % cgCommunicationInterruptQueueSize, std::memory_order_release);
  mSize.fetch_sub(1,std::memory_order_acq_rel);
  return comLayer;
}

CComLayer* CSPSCQueue::getHead() {
  size_t head = mHead.load(std::memory_order_relaxed);
  if (head == mTail.load(std::memory_order_acquire)) {
    return nullptr;
  }
  return mBuffer[head];
}


size_t CSPSCQueue::getSize() {
  return mSize.load(std::memory_order_acquire);
}
