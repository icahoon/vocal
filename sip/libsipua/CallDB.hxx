#ifndef CallDB_hxx
#define CallDB_hxx

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


#include "global.h"
#include <map>

#include "MultiLegCallData.hxx"
#include "UaBase.hxx"
#include "AccountingData.hxx"

namespace Vocal {

namespace UA {


/** Object CallDB

<pre>
   Database containing the call leg data. it implements the call-info
   database in memory for each call-leg.

   for Management usage please refer to /vocal/sip/b2b/MultiLegCallControler.cxx

</pre>
*/
class CallDB {
public:
   ///
   typedef map<SipCallLeg , Sptr<MultiLegCallData> > MultiLegCallDataMap;
   
   ///
   typedef map<int , Sptr<MultiLegCallData> > AccountingDataMap;

   /// Create one with default values
   static CallDB& instance();

   ///
   string className() { return "CallDB"; }

   ///Destructor
   virtual ~CallDB();

   /**
      Frees the memory associated with singelton instance.
      gets register to atexit() function at the time of creation.
   */
   static void destroy(void);

   ///Add a call-leg served by the userAgent.
   void addCallLeg(Sptr<UaBase> userAgent);

   /**Add a peer to given userAgent
    * Example - If user A is in call with B and C
    * addPeer(A,B);
    * addPeer(A,C);
    * addPeer(B,A);
    * addPeer(C,A);
    */
   void addPeer(Sptr<UaBase> userAgent, Sptr<UaBase> peerAgent);

   ///
   Sptr<MultiLegCallData> getMultiLegCallData(const SipCallLeg& callLeg);

   /**
    * Set the accounting data for a given call identified by the
    * session ID.Parameter cLeg identifies the initiating call-leg
    * In case of libsipua used in B2bUa
    * <pre>
    * UA1 ---- CallLeg1 ---->B2bUa --- CallLeg2 --->UA2
    * Sptr<AccountingData> aData = new AccountingData(sessionId);
    * aData.setUnusedSeconds(1000);
    * CallDB::instance().setAccountingData(CallLeg1, sessionId, aData);
    * </pre>
    */
   void setAccountingData(const SipCallLeg& cLeg, int sessionId, Sptr<AccountingData> aData);
   ///
   void removeAccountingData(int sessionId);

   ///
   Sptr<AccountingData> getAccountingData( int sessionId);

   ///
   void findAllPeers(const UaBase& userAgent, UserAgentPeerList& retVal);

   /**Removes the peerAgent from srcAgent list and returns the current
    * number that srcAgent is peering with after removal
    */
   int removePeer(const UaBase& srcAgent, const UaBase& peerAgent);

   ///Simply remove agent and all its peers from the CallDB
   void removePeer(const UaBase& agent);

   ///Remove call data for user agent 
   void removeCallData(const UaBase& agent);

private:
   ///
   int cleanupTransactionPeer(const UaBase &srcAgent,
                              const UaBase& peerAgent,
                              Sptr<MultiLegCallData> mData);
   ///
   CallDB();
   ///
   static CallDB* myInstance;
   ///
   AccountingDataMap myAccountingDataMap;
   ///
   MultiLegCallDataMap myMultiLegCallDataMap;
};

}

}


#endif
