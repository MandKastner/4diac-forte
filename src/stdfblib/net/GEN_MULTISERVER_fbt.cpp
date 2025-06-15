/*******************************************************************************
 * Copyright (c) 2006 - 2011 ACIN, Profactor GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Rene Smodic, THomas Strasser, Alois Zoitl, Gerhard Ebenhofer,
 *   Ingo Hegny, Martin Melik Merkumians
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "GEN_MULTISERVER_fbt.h"

using namespace forte::com_infra;

DEFINE_GENERIC_FIRMWARE_FB(GEN_MULTISERVER, g_nStringIdGEN_MULTISERVER)

GEN_MULTISERVER::GEN_MULTISERVER(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
            forte::com_infra::CCommFB(paInstanceNameId, paContainer, forte::com_infra::e_Server){
}

void GEN_MULTISERVER::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    EComResponse resp = e_Nothing;

    switch (paEIID) {
      case scmEventINITID:
        if (true == QI()) {
          resp = openConnection();
        }
        else {
          closeConnection();
          resp = e_InitTerminated;
        }
        break;
      case scmSendNotificationEventID:
          resp = CCommFB::sendData();
          if (!mInterruptQueue.isEmpty()) {
            //drop event of queue
            dropTopOfInterruptQueue();
            if (!mInterruptQueue.isEmpty()) {
              //process event but do not remove
              resp = processInterruptQueueEvent();
            }
          }
        break;
      case cgExternalEventID:
        if (mInterruptQueue.getSize() > 1) {
          //do nothing we have to wait for the current data being processed
          resp = e_Nothing;
          break;
        }
        resp = processInterruptQueueEvent();
        break;
      default:
        break;
    }
    processEventResponse(resp, paEIID, paECET);
}

