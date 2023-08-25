/*******************************************************************************
 * Copyright (c) 2005 - 2015 Profactor GmbH, ACIN, fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Alois Zoitl, Gunnar Grabmaier, Gerhard Ebenhofer,
 *    Ingo Hegny
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#ifndef _CFB_H_
#define _CFB_H_

#include "funcbloc.h"

class CInterface2InternalDataConnection;

struct SCFB_FBConnectionData {
    CStringDictionary::TStringId mSrcId;
    int mSrcFBNum;
    CStringDictionary::TStringId mDstId;
    int mDstFBNum;
};

/*! \ingroup CORE
 * \brief Connection data for a fanned out connection of a composite's FB network
 *
 * As fanned out connections are attached together they need different data than
 * the first connection.
 */
struct SCFB_FBFannedOutConnectionData {
    int mConnectionNum; //!<the connection this fanned out connection should be attached to.
    CStringDictionary::TStringId mDstId;
    int mDstFBNum;
};

struct SCFB_FBParameter {
    unsigned int mFBNum;
    CStringDictionary::TStringId mDINameID; //!< the data input id of the input param to set
    const char * mParamValue;
};

struct SCFB_FBNData {
    unsigned int mNumFBs;
    const SCFB_FBInstanceData * mFBInstances;
    //TODO add FB parameters
    unsigned int mNumEventConnections;
    const SCFB_FBConnectionData * mEventConnections;
    unsigned int mNumFannedOutEventConnections;
    const SCFB_FBFannedOutConnectionData * mFannedOutEventConnections;
    unsigned int mNumDataConnections;
    const SCFB_FBConnectionData * mDataConnections;
    unsigned int mNumFannedOutDataConnections;
    const SCFB_FBFannedOutConnectionData * mFannedOutDataConnections;
    unsigned int mNumParams;
    const SCFB_FBParameter * mParams;
};

//These two defines are here for legacy reasons. They allow that old CFBs can still be complied
/*! Generate a connection port id for a given fb-nameid / port-nameid combination
 *  This template will generate a constant and will therefore be very efficient
 */
#define GENERATE_CONNECTION_PORT_ID_2_ARG(FBNameId, PortNameId) (PortNameId)

/*! Generate a connection port id for a connection with a single source id e.g. interface connections
 *  This template will generate a constant and will therefore be very efficient
 */
#define GENERATE_CONNECTION_PORT_ID_1_ARG(PortNameId)\
GENERATE_CONNECTION_PORT_ID_2_ARG(CStringDictionary::scmInvalidStringId, PortNameId)

/*!\ingroup CORE
 * \brief Class for handling firmware composite function blocks.
 */
class CCompositeFB: public CFunctionBlock {

  public:
    /*! \brief Indicator that the given FB id is an adapter.
     *
     *This will allow for 65535 FBs within an CFB, which should be sufficient.
     */
    static const TForteInt32 scmAdapterMarker = 0x10000;

    /*! \brief constant for retrieving an FB Num from an FBId marked with an Adapter Marker
     *
     * needs to correspond to scmAdapterMarker
     */
    static const TForteInt32 scmAdapterFBRange = 0xFFFF;

    /*!\brief The main constructor for a composite function block.
     *
     * The interface of the constructor has been extended from the basic FB's interface with the following values:
     * \param paSrcRes         pointer to including resource
     * \param paInterfaceSpec pointer to interface specification
     * \param paInstanceNameId  StringId of instance name
     * \param paFBNData       const pointer to description of internal structure of FB (FBs, Connections, ...)
     * \param paFBData         ByteArray for FB-specific data (DI, DO, int. Vars, ...)
     */
    CCompositeFB(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec,
                 CStringDictionary::TStringId paInstanceNameId,
                 const SCFB_FBNData * paFBNData);

    ~CCompositeFB() override;

    bool initialize() override;

    /*!\brief Connects specific data input of a CFB with a specific data connection.
     *
     * \param paDINameId    StringId of input name.
     * \param paDataCon     Pointer to the data connection the CFB should be connected to.
     * \return TRUE on success, FALSE if data output not exists or output is already connected.
     */
    bool connectDI(TPortId paDIPortId, CDataConnection *paDataCon) override;

    bool configureGenericDO(TPortId paDOPortId, const CIEC_ANY &paRefValue) override;
    //! implement improved version of get Var for CFBs, allowing to read and write to internal elements
    CIEC_ANY* getVar(CStringDictionary::TStringId *paNameList,
        unsigned int paNameListSize) override;

    EMGMResponse changeFBExecutionState(EMGMCommandType paCommand) override;

#ifdef FORTE_SUPPORT_MONITORING
    CFunctionBlock *getFB(forte::core::TNameIdentifier::CIterator &paNameListIt) override;
#endif

  private:
    void executeEvent(TEventID paEIID, CEventChainExecutionThread * const paECET) override;

    bool createInternalFBs();
    void createEventConnections();
    void prepareIf2InEventCons();
    void establishConnection(CConnection *paCon, CFunctionBlock *paDstFb, CStringDictionary::TStringId paDstNameId);
    void createDataConnections();
    void prepareIf2InDataCons();
    void setParams();
    void sendInternal2InterfaceOutputEvent(TEventID paEOID, CEventChainExecutionThread *paECET);

    //!Acquire the functionblock for a given function block number this may be a contained fb, an adapter, or the composite itself.
    CFunctionBlock *getFunctionBlock(int paFBNum);

    CInterface2InternalDataConnection *mIf2InDConns;
    CDataConnection **mIn2IfDConns;

    const SCFB_FBNData * const cmFBNData;

    CEventConnection **mEventConnections;
    CDataConnection **mDataConnections;

    //!Array storing the holding the connections to be used in the execute event for triggering the internal FBs
    CEventConnection **mInterface2InternalEventCons;

    TFunctionBlockPtr *mInternalFBs;

#ifdef FORTE_FMU
    friend class fmuInstance;
#endif //FORTE_FMU
};

#define COMPOSITE_FUNCTION_BLOCK_CTOR(fbclass) \
 fbclass(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) : \
 CCompositeFB(paSrcRes, &scmFBInterfaceSpec, paInstanceNameId, &scmFBNData)

#endif /*_CFB_H_*/
