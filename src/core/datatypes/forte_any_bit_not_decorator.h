/*******************************************************************************
 * Copyright (c) 2023 Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik Merkumians
 *      - initial implementation
 *******************************************************************************/

template<typename T>
class CIEC_ANY_BIT_NOT : public T {
  public:
    CIEC_ANY_BIT_NOT(T& paValue) : T(paValue), wrapped(paValue) {
    }

    ~CIEC_ANY_BIT_NOT() {
      wrapped = func_NOT(*this);
    }

    T &operator=(const T &paValue) {
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

  private:
    T& wrapped;
};