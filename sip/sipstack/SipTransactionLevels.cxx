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

static const char* const SipTransactionLevels_cxx_version =
    "$Id: SipTransactionLevels.cxx,v 1.3 2004/06/01 07:23:31 greear Exp $";

#include "global.h"
#include "SipTransactionLevels.hxx"
#include <sstream>

using namespace Vocal;


SipMsgContainer::SipMsgContainer(const SipTransactionId& id) {
   clear();
   trans_id = id;
}

void SipMsgContainer::setMsgIn(Sptr<SipMsg> inm) {
   in = inm;
   in_encode = in->encode();
}

void SipMsgContainer::clear() {
   in = NULL;
   in_encode = "";
   netAddr = NULL;
   transport = "";
   shouldGcAt = 0;
   retransCount = 1;
   collected = false;
   prepareCount = 0;
   trans_id.clear();
}//clear

bool SipMsgContainer::matches(const SipTransactionId& id) {
   return (id == trans_id);
}

void SipMsgContainer::cleanup() {
    msg.in = NULL;
    msg.netAddr = NULL;
}

string SipMsgContainer::toString() const {
    ostringstream oss;
    if (msg.in.getPtr()) {
        oss << " in: " << msg.in->toString() << endl;
    }
    else {
        oss << " in: NULL\n";
    }

    
    oss << "out: " << msg.out.c_str()
        << "\nnetAddr: ";

    if (msg.netAddr != 0) {
        oss << msg.netAddr->toString();
    }
    else {
        oss << "NULL";
    }

    oss << "\ntype: " << msg.type << " transport: " << msg.transport.c_str()
        << "\nretransCount: " << retransCount << " collected: " << collected
        << endl;

    return oss.str();
}


///*******************  Sip Call Container  **********************///

SipCallContainer::SipCallContainer(const SipTransactionId& call_id)
      : id(call_id) {
   seqSet = false;
   curSeqNum = 0;
}


Sptr<SipMsgPair> SipCallContainer::findMsgPair(const SipTransactionId& id) {
    list<Sptr<SipMsgPair> >::iterator i = msgs.begin();
    while (i != msgs.end()) {
        Sptr<SipMsgPair> mp = *i;
        if ((mp->request != 0) && mp->request->matches(id)) {
            return (*i);
        }
        if ((mp->response != 0) && mp->response->matches(id)) {
            return (*i);
        }
        i++;
    }
    return NULL;
}


Sptr<SipMsgPair> SipCallContainer::findMsgPair(Method method) {
    list<Sptr<SipMsgPair> >::iterator i = msgs.begin();
    while (i != msgs.end()) {
        Sptr<SipMsgPair> mp = *i;
        if ((mp->request != 0) && mp->request->getType() == method) {
            return (*i);
        }
        if ((mp->response != 0) && mp->response->getType() == method) {
            return (*i);
        }
        i++;
    }
    return NULL;
}


void SipCallContainer::clear() {
   msgs.clear();
   seqSet = false;
   curSeqNum = 0;
}
