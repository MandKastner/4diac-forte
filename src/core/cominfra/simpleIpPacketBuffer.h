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

#pragma once

#include <datatype.h>
#include <queue>
#include <cstring>
#include <forte_config.h>
#include <memory>
#include "sockhand.h"


namespace forte :: com_infra {
    class CSimpleIPComPacketBuffer {
        class CComPacket {
        public:
            CComPacket(char paData[], unsigned int paSize, CIPComSocketHandler::TSocketDescriptor paClientID) : mClientID(paClientID), mSize(paSize) {
              mData = std::make_unique<char[]>(paSize);
              std::copy(paData, paData + paSize, mData.get());
            }

            CIPComSocketHandler::TSocketDescriptor mClientID;
            unsigned int mSize; //the unique ptr does not store any information about the size
            std::unique_ptr<char[]> mData;
        };


    private:
        const unsigned int cm_packetBufferQueueSize = cgIpPacketBufferQueueSize;
        std::queue<CComPacket> mPacketQueue;

    public:
        CSimpleIPComPacketBuffer() = default;

        ~CSimpleIPComPacketBuffer() = default;

        bool enqueuePacket(char *paData, unsigned int paSize, CIPComSocketHandler::TSocketDescriptor paClientID);

        CIPComSocketHandler::TSocketDescriptor dequeuePacket(std::unique_ptr<char[]>& paDestinationData, unsigned int& packetSize);

        bool empty();
    };

}
