#ifndef UaState_hxx
#define UaState_hxx

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



static const char* const UaState_hxx_Version = 
"$Id: UaState.hxx,v 1.3 2005/08/10 21:58:03 greear Exp $";

#include "CInvalidStateException.hxx"
#include "SipMsg.hxx"
#include "SystemInfo.hxx"
#include "cpLog.h"
#include "SipCommand.hxx"

namespace Vocal {

namespace UA {

class UaBase;
/** Object UaState
<pre>
<br> Usage of this Class </br>

    UaState represent base class for UAS/UAC states.

</pre>
*/
class UaState : public BugCatcher {
    public:
         ///
         virtual const string& className() const { return _className; }

         UaState(const char* class_name);

         ///Destructor
         virtual ~UaState() { };

         ///
         virtual void recvRequest(UaBase& agent, Sptr<SipMsg> msg) 
                 throw (CInvalidStateException&);

         /// Returns < 0 on failure
         virtual int sendRequest(UaBase& agent, Sptr<SipMsg> msg) 
                 throw (CInvalidStateException&);

         ///
         virtual int sendStatus(UaBase& agent, Sptr<SipMsg> msg) 
                 throw (CInvalidStateException&);
         ///
         virtual void recvStatus(UaBase& agent, Sptr<SipMsg> msg) 
                 throw (CInvalidStateException&);

	 void addSelfInVia(UaBase& agent, Sptr<SipCommand> cmd);

         virtual string toString() { return className(); }

    protected:
         ///
         void error(const string& errMsg);

         ///
         void info(const string& errMsg);

         ///
         void changeState(UaBase& agent, Sptr<UaState> newState);

   private:
         string _className;

         UaState();
         UaState(const UaState& src);
         UaState& operator=(const UaState& src);
};

}
}

#endif
