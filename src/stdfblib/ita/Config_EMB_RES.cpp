/*******************************************************************************
 * Copyright (c) 2024 Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Markus Meingast
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <stringdict.h>
#include "Config_EMB_RES.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "Config_EMB_RES_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(Config_EMB_RES, g_nStringIdConfig_EMB_RES);

const CStringDictionary::TStringId Config_EMB_RES::scmVarInputNameIds[] = {g_nStringIdOPCUA_Namespace};
const CStringDictionary::TStringId Config_EMB_RES::scmDIDataTypeIds[] = {g_nStringIdWSTRING};

const SFBInterfaceSpec Config_EMB_RES::scmFBInterfaceSpec = {
  0, nullptr, nullptr, nullptr,
  0, nullptr, nullptr, nullptr,
  1, scmVarInputNameIds, scmDIDataTypeIds,
  0, nullptr, nullptr,
  0, nullptr,
  0, nullptr
};

Config_EMB_RES::Config_EMB_RES(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paDevice) :
    EMB_RES(paInstanceNameId, paDevice) {
      mInterfaceSpec = &scmFBInterfaceSpec;
}

Config_EMB_RES::~Config_EMB_RES() = default;