/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef FORTE_INSTANCE_H_
#define FORTE_INSTANCE_H_

/* When moving this file to the outside of the architecture,
 * the corresponding defines for exporting in windows or other platform
 * must be added here
 *
 */
#ifndef FORTE_SHARED_PREFIX
  #define FORTE_SHARED_PREFIX
#endif

#ifndef FORTE_SHARED_CALL
  #define FORTE_SHARED_CALL
#endif


#ifdef __cplusplus
extern "C" {
#endif

  enum FORTE_STATUS{
    FORTE_OK,
    FORTE_DEVICE_ALREADY_STARTED,
    FORTE_WRONG_ENDIANESS,
    FORTE_WRONG_PARAMETERS,
    FORTE_ARCHITECTURE_NOT_READY,
  };

  typedef void* TForteInstance;

  /**
   * \brief Start forte instance
   * @param pa_port The port on which to forte will listen. Use 0 for default (normally 61499)
   * @param pa_resultDevice Address of an instance of forte
   * @return FORTE_OK if no error occurred, other values otherwise
   */
  FORTE_SHARED_PREFIX int FORTE_SHARED_CALL forte_start_instance(unsigned int pa_port, TForteInstance* pa_resultDevice);

  /**
   * \brief Start forte instance with posibilities of more arguments
   * @param argc Number of arguments in arg
   * @param arg Arguments
   * @param pa_resultDevice Address of an instance of forte
   * @return FORTE_OK if no error occurred, other values otherwise
   */
  FORTE_SHARED_PREFIX int FORTE_SHARED_CALL forte_start_instance_generic(int argc, char *arg[], TForteInstance* pa_resultDevice);

  /**
   * \brief Terminates a Forte instance
   * @param signal  Signal value to terminate instance
   * @param pa_resultDevice Instance to terminate
   */
  FORTE_SHARED_PREFIX void FORTE_SHARED_CALL forte_stop_instance(int signal, TForteInstance pa_resultDevice);

  /**
   * \brief Initializes the architecture. Prepare all resources needed by the Forte's instances. Must be called once before the first Forte instance is started
   */
  FORTE_SHARED_PREFIX void FORTE_SHARED_CALL forte_global_initialize(void);

  /**
   * \brief Deinitializes the architecture. Frees all resources used by Forte's instances. Must be called after the last instance is ended
   */
  FORTE_SHARED_PREFIX void FORTE_SHARED_CALL forte_global_deinitialize(void);


#ifdef __cplusplus
}
#endif

#endif /* FORTE_INSTANCE_H_ */
