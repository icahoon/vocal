#ifndef SessionTimer_HXX
#define SessionTimer_HXX

/* ====================================================================
 * The Vovida Software License, Version 1.0 
 * 
 * Copyright (c) 2000 Vovida Networks, Inc.  All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 
 * 3. The names "VOCAL", "Vovida Open Communication Application Library",
 *    and "Vovida Open Communication Application Library (VOCAL)" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact vocal@vovida.org.
 *
 * 4. Products derived from this software may not be called "VOCAL", nor
 *    may "VOCAL" appear in their name, without prior written
 *    permission of Vovida Networks, Inc.
 * 
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND
 * NON-INFRINGEMENT ARE DISCLAIMED.  IN NO EVENT SHALL VOVIDA
 * NETWORKS, INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT DAMAGES
 * IN EXCESS OF $1,000, NOR FOR ANY INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 * 
 * ====================================================================
 * 
 * This software consists of voluntary contributions made by Vovida
 * Networks, Inc. and many individuals on behalf of Vovida Networks,
 * Inc.  For more information on Vovida Networks, Inc., please see
 * <http://www.vovida.org/>.
 *
 */

static const char* const SessionTimer_hxx_version =
    "$Id: SessionTimer.hxx,v 1.1 2004/05/01 04:15:26 greear Exp $";
    
#include "global.h"
#include <map>
#include "Data.hxx"
#include "Sptr.hxx"
#include "TimerEntry.hxx"
#include "InviteMsg.hxx"
#include "SipCallLeg.hxx"
#include "Fifo.h"
#include "SipTransceiver.hxx"

using Vocal::TimeAndDate::TimerEntryId;

namespace Vocal
{

enum VSessionContext 
{
    VS_NONE=0,
    VS_SEND,
    VS_RECV
};

typedef struct VSessionData_t
{
    long myDelta;
    long myStartTime;
    TimerEntryId          myTimerId;
    VSessionContext       mySessionContext;
    Sptr<InviteMsg> myInviteMsg;

    VSessionData_t(): myDelta(0), myStartTime(0) {};
} VSessionData;

typedef void (*VSessionCallBack)(SipCallLeg arg);


/// Singelton clas to support SessionTimers
class SessionTimer
{
public:
    /// Create one with default values
    static SessionTimer& instance();
    ///
    static SessionTimer& instance(Sptr<SipTransceiver> tranceiver);

    ///
    static  void destroy();

    ///
    void registerCallBack(VSessionCallBack func) { myCallbackFunc = func; };

    ///Chjeck if timer already exists for a given call-leg
    bool timerExists(const InviteMsg& iMsg);

    /**Start a timer for Invite message with given sessionInterval
     * To be called when not a refresher
     */
    void startTimerFor(const InviteMsg& iMsg, long  sessionInterval, VSessionContext ct=VS_RECV);

    ///Interface to start the SessionTimer by refresher when received status msg
    // local_ip cannot be "" here, must be the local IP we are bound to locally
    // or 'hostaddress' if we are not specifically bound.
    void startTimerFor(const StatusMsg& sMsg, const string& local_ip);

    ///
    void processResponse(StatusMsg& sMsg);
private:
    ///
    void startTimerFor(const InviteMsg& iMsg, Sptr<VSessionData> sData);

    ///
    void sendInvite(Sptr<VSessionData> sData);
    ///
    SessionTimer(Sptr<SipTransceiver> tranceiver);
    ///
    static void* processThreadWrapper(void *p);
    ///
    virtual ~SessionTimer() ;

    static SessionTimer* mInstance;
    typedef Fifo<Sptr<VSessionData> > SessionDataFifo;
    SessionDataFifo mSessionDataFifo;
    typedef map<SipCallLeg, Sptr<VSessionData> > SessionDataMap;
    SessionDataMap mSessionDataMap;
    VThread mProcessThread;
    Sptr<SipTransceiver> mTransceiver;
    VSessionCallBack myCallbackFunc;
    bool mShutdown;
};
 
} // namespace Vocal

/* Local Variables: */
/* c-file-style: "stroustrup" */
/* indent-tabs-mode: nil */
/* c-file-offsets: ((access-label . -) (inclass . ++)) */
/* c-basic-offset: 4 */
/* End: */

#endif
