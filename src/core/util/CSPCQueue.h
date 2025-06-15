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

#pragma once

#include "comlayer.h"
#include <atomic>

using namespace forte::com_infra;

class CSPSCQueue {
public:
    CSPSCQueue(): mBuffer(), mHead(0), mTail(0), mSize(0) {}

    bool push(CComLayer* paComlayer);
    CComLayer* pop();
    CComLayer* getHead();
    size_t getSize();

private:
    std::array<CComLayer*, cgCommunicationInterruptQueueSize> mBuffer;
    std::atomic<size_t> mHead;
    std::atomic<size_t> mTail;
    std::atomic<size_t> mSize;
};


