/*************************************************************************
 *** Copyright (c) 2024 HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: ASSEMBLE_DWORD_FROM_BYTES
 *** Description: this Function combines the 2 BYTES to a DWORD
 *** Version:
 ***     1.0: 2024-02-22/Franz Höpfinger - HR Agrartechnik - initial Implementation
 ***     1.1: 2024-07-28/Moritz Ortmeier - HR Agrartechnik - rename Function and change Input/Output Variables
 *************************************************************************/

#include "ASSEMBLE_DWORD_FROM_BYTES_fct.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "ASSEMBLE_DWORD_FROM_BYTES_fct_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_dword.h"
#include "forte_byte.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"
#include "ASSEMBLE_DWORD_FROM_BYTES_fct.h"

DEFINE_FIRMWARE_FB(FORTE_ASSEMBLE_DWORD_FROM_BYTES, g_nStringIdASSEMBLE_DWORD_FROM_BYTES)

const CStringDictionary::TStringId FORTE_ASSEMBLE_DWORD_FROM_BYTES::scmDataInputNames[] = {g_nStringIdBYTE_00, g_nStringIdBYTE_01, g_nStringIdBYTE_02, g_nStringIdBYTE_03};
const CStringDictionary::TStringId FORTE_ASSEMBLE_DWORD_FROM_BYTES::scmDataInputTypeIds[] = {g_nStringIdBYTE, g_nStringIdBYTE, g_nStringIdBYTE, g_nStringIdBYTE};
const CStringDictionary::TStringId FORTE_ASSEMBLE_DWORD_FROM_BYTES::scmDataOutputNames[] = {g_nStringId};
const CStringDictionary::TStringId FORTE_ASSEMBLE_DWORD_FROM_BYTES::scmDataOutputTypeIds[] = {g_nStringIdDWORD};
const TDataIOID FORTE_ASSEMBLE_DWORD_FROM_BYTES::scmEIWith[] = {0, 1, 2, 3, scmWithListDelimiter};
const TForteInt16 FORTE_ASSEMBLE_DWORD_FROM_BYTES::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_ASSEMBLE_DWORD_FROM_BYTES::scmEventInputNames[] = {g_nStringIdREQ};
const TDataIOID FORTE_ASSEMBLE_DWORD_FROM_BYTES::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_ASSEMBLE_DWORD_FROM_BYTES::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_ASSEMBLE_DWORD_FROM_BYTES::scmEventOutputNames[] = {g_nStringIdCNF};
const SFBInterfaceSpec FORTE_ASSEMBLE_DWORD_FROM_BYTES::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  4, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_ASSEMBLE_DWORD_FROM_BYTES::FORTE_ASSEMBLE_DWORD_FROM_BYTES(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_conn_(var_),
    conn_CNF(this, 0),
    conn_BYTE_00(nullptr),
    conn_BYTE_01(nullptr),
    conn_BYTE_02(nullptr),
    conn_BYTE_03(nullptr),
    conn_(this, 0, &var_conn_) {
}

void FORTE_ASSEMBLE_DWORD_FROM_BYTES::setInitialValues() {
  var_BYTE_00 = 0_BYTE;
  var_BYTE_01 = 0_BYTE;
  var_BYTE_02 = 0_BYTE;
  var_BYTE_03 = 0_BYTE;
  var_ = 0_DWORD;
}

void FORTE_ASSEMBLE_DWORD_FROM_BYTES::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_BYTE_00, conn_BYTE_00);
      readData(1, var_BYTE_01, conn_BYTE_01);
      readData(2, var_BYTE_02, conn_BYTE_02);
      readData(3, var_BYTE_03, conn_BYTE_03);
      break;
    }
    default:
      break;
  }
}

void FORTE_ASSEMBLE_DWORD_FROM_BYTES::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_, conn_);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_ASSEMBLE_DWORD_FROM_BYTES::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_BYTE_00;
    case 1: return &var_BYTE_01;
    case 2: return &var_BYTE_02;
    case 3: return &var_BYTE_03;
  }
  return nullptr;
}

CIEC_ANY *FORTE_ASSEMBLE_DWORD_FROM_BYTES::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_;
  }
  return nullptr;
}

CEventConnection *FORTE_ASSEMBLE_DWORD_FROM_BYTES::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_ASSEMBLE_DWORD_FROM_BYTES::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_BYTE_00;
    case 1: return &conn_BYTE_01;
    case 2: return &conn_BYTE_02;
    case 3: return &conn_BYTE_03;
  }
  return nullptr;
}

CDataConnection *FORTE_ASSEMBLE_DWORD_FROM_BYTES::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_;
  }
  return nullptr;
}

void FORTE_ASSEMBLE_DWORD_FROM_BYTES::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
  var_ = func_ASSEMBLE_DWORD_FROM_BYTES(var_BYTE_00, var_BYTE_01, var_BYTE_02, var_BYTE_03);
  sendOutputEvent(scmEventCNFID, paECET);
}

CIEC_DWORD func_ASSEMBLE_DWORD_FROM_BYTES(CIEC_BYTE st_lv_BYTE_00, CIEC_BYTE st_lv_BYTE_01, CIEC_BYTE st_lv_BYTE_02, CIEC_BYTE st_lv_BYTE_03) {
  CIEC_DWORD st_ret_val = 0_DWORD;

  #line 11 "ASSEMBLE_DWORD_FROM_BYTES.fct"
  st_ret_val.partial<CIEC_BYTE>(0) = st_lv_BYTE_00;
  #line 12 "ASSEMBLE_DWORD_FROM_BYTES.fct"
  st_ret_val.partial<CIEC_BYTE>(1) = st_lv_BYTE_01;
  #line 13 "ASSEMBLE_DWORD_FROM_BYTES.fct"
  st_ret_val.partial<CIEC_BYTE>(2) = st_lv_BYTE_02;
  #line 14 "ASSEMBLE_DWORD_FROM_BYTES.fct"
  st_ret_val.partial<CIEC_BYTE>(3) = st_lv_BYTE_03;

  return st_ret_val;
}
