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

#ifndef SRC_CORE_COMINFRA_IPPACKETBUFFER_H_
#define SRC_CORE_COMINFRA_IPPACKETBUFFER_H_

#include <datatype.h>  //needed for size_t?
#include <queue>
#include <cstring>

namespace forte{
    namespace com_infra {
        class CIPComPacketBuffer {

            class CComPacket {
            public:
                CComPacket(char arr[], unsigned int paSize) : mSize(paSize) {
                  char *dest = new char[paSize];
                  memcpy(dest, arr, sizeof(char)*paSize);
                  mData = dest;
                }

                //~CComPacket();
                unsigned int mSize;
                char *mData;
            };


        private:
            const size_t cm_packetBufferQueueSize = 20;
            std::queue<CComPacket> mPacketQueue;

        public:
            CIPComPacketBuffer() = default;
            ~CIPComPacketBuffer() = default;
            bool enqueuePacket(char arr[], unsigned int paSize);
            void dequeuePacket();
            bool empty();
            unsigned int copyQueueHead(char arr[]);
        };
    }
}


#endif /* SRC_CORE_COMINFRA_IPPACKETBUFFER_H_ */
