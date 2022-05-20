/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 *               2022 Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger
 *    - initial API and implementation and/or initial documentation
 *    Martin Melik Merkumians - changes for removed implicit constructor for
 *      primitve types
 *******************************************************************************/

#include "ForteBootFileLoader.h"
#include "../../arch/devlog.h"
#include "../../../src/core/datatypes/forte_string.h"
#include "IBootFileCallback.h"
#include <mgmcmd.h>
#include <mgmcmdstruct.h>
#include "../../core/device.h"

char* gCommandLineBootFile = nullptr;

ForteBootFileLoader::ForteBootFileLoader(IBootFileCallback &paCallback) : mBootfile(nullptr), mCallback(paCallback), mNeedsExit(false){
  openBootFile();
}

ForteBootFileLoader::~ForteBootFileLoader() {
  if(nullptr != mBootfile){
    DEVLOG_INFO("Closing bootfile\n");
    fclose(mBootfile);
  }
}

bool ForteBootFileLoader::openBootFile() {
  bool retVal = false;
  CIEC_STRING bootFileName;
  if(gCommandLineBootFile) {
    DEVLOG_INFO("Using provided bootfile location set in the command line: %s\n", gCommandLineBootFile);
    bootFileName = CIEC_STRING(gCommandLineBootFile);
  } else {
    // select provided or default boot file name
    char * envBootFileName = getenv("FORTE_BOOT_FILE");
    if(nullptr != envBootFileName) {
      DEVLOG_INFO("Using provided bootfile location from environment variable: %s\n", envBootFileName);
      bootFileName = CIEC_STRING(envBootFileName);
    } else {
      DEVLOG_INFO("Using provided bootfile location set in CMake: %s\n", FORTE_BOOT_FILE_LOCATION);
      bootFileName = CIEC_STRING(FORTE_BOOT_FILE_LOCATION);
    }
  }

  // check if we finally have a boot file name
  if(bootFileName.empty()){
    DEVLOG_INFO("No bootfile specified and no default bootfile configured during build\n");
  }else{
    mBootfile = fopen(bootFileName.getValue(), "r");
    if(nullptr != mBootfile){
      DEVLOG_INFO("Boot file %s opened\n", bootFileName.getValue());
      retVal = true;
    }
    else{
      if(nullptr != getenv("FORTE_BOOT_FILE_FAIL_MISSING")){
        DEVLOG_ERROR("Boot file %s could not be opened and FORTE_BOOT_FILE_FAIL_MISSING is set. Failing...\n", bootFileName.getValue());
        mNeedsExit = true;
      }
      else{
        DEVLOG_INFO("Boot file %s could not be opened. Skipping...\n", bootFileName.getValue());
      }
    }
  }
  return retVal;
}

LoadBootResult ForteBootFileLoader::loadBootFile(){
  LoadBootResult eResp = FILE_NOT_OPENED;
  if(nullptr != mBootfile){
    //we could open the file try to load it
    int nLineCount = 1;
    eResp = LOAD_RESULT_OK;
    CIEC_STRING line;
    while(readLine(line) && LOAD_RESULT_OK == eResp) {
      char *cmdStart = strchr(line.getValue(), ';');
      if(nullptr == cmdStart){
        eResp = MISSING_COLON;
        DEVLOG_ERROR("Boot file line does not contain separating ';'. Line: %d\n", nLineCount);
      } else {
        *cmdStart = '\0';
        cmdStart++;
        if(!mCallback.executeCommand(line.getValue(), cmdStart)) {
          //command was not successful
          DEVLOG_ERROR("Boot file command could not be executed. Line: %d: %s\n", nLineCount, cmdStart);
          eResp = EXTERNAL_ERROR;
        } else {
          nLineCount++;
        }
      }
    }
  }else{
    DEVLOG_ERROR("Loading cannot proceed because the boot file is no opened\n");
  }
  return eResp;
}

bool ForteBootFileLoader::readLine(CIEC_STRING &line){
  const unsigned int size = 100;
  line.clear();
  char acLineBuf[size];
  do {
    if(nullptr != fgets(acLineBuf, size, mBootfile)) {
      line.append(acLineBuf);
    } else {
      return 0 != line.length();
    }
  }while(!hasCommandEnded(line));
  return true;
}

bool ForteBootFileLoader::hasCommandEnded(const CIEC_STRING &line) const{
  return (0 == strcmp(line.getValue() + line.length() - 11, "</Request>\n") || 0 == strcmp(line.getValue() + line.length() - 3, "/>\n"));
}
