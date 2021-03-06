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
#include "SipTransactionDB.hxx"

using namespace Vocal;


SipTransactionDB::SipTransactionDB(const string& _local_ip, const char* _name) {
   local_ip = _local_ip;
   name = _name;
}


SipTransactionDB::~SipTransactionDB() {
   // Nothing to do at this point.
}


string SipTransactionDB::toString() {
   string rv;
   map <string, Sptr<SipCallContainer> >::iterator i = table.begin();
   while(i != table.end()) {
      rv += i->first.c_str();
      rv += "\n";
      i++;
   }
   return rv;
}


Sptr<SipCallContainer> SipTransactionDB::getCallContainer(const SipTransactionId& id) {
   string k = id.getCallId().c_str();
   map <string, Sptr<SipCallContainer> >::iterator i = table.find(k);
   if (i != table.end()) {
      return i->second;
   }
   cpLog(LOG_INFO, "%s, Could not find call container -:%s:-, table:\n%s",
         name.c_str(), k.c_str(), toString().c_str());
   return NULL;
}

void SipTransactionDB::addCallContainer(Sptr<SipCallContainer> m) {
   string k(m->getCallId().getCallId().c_str());
   cpLog(LOG_DEBUG_STACK, "%s Adding call container -:%s:-", name.c_str(), k.c_str());
   table[k] = m;
}

void SipTransactionDB::setCallPurgeTimer(const SipTransactionId& id, uint64 expires_at) {
   Sptr<SipCallContainer> c = getCallContainer(id);
   string k(id.getCallId().c_str());
   if (c != 0) {
      cpLog(LOG_INFO, "%s Setting purge timer for call -:%s:-", name.c_str(), k.c_str());
      c->setPurgeTimer(expires_at);
   }
   else {
      cpLog(LOG_INFO, "%s Could not find call in setPurgeTimer, call -:%s:-", name.c_str(), k.c_str());
      cpLog(LOG_INFO, "%s all calls:\n%s\n", toString().c_str());
   }
}

void SipTransactionDB::purgeOldCalls(uint64 now) {
   map <string, Sptr<SipCallContainer> >::iterator i = table.begin();
   map <string, Sptr<SipCallContainer> >::iterator tmpi;
   while(i != table.end()) {
      uint64 p = i->second->getPurgeTimer();
      string key = i->second->getCallId().getCallId().c_str();
      if (p && (p < now)) {
         tmpi = i;
         i++;
         //TODO:  Make sure this does not invalidate iterator i.
         table.erase(tmpi);
         cpLog(LOG_DEBUG_STACK, "%s Purged call: %s from transaction DB, size: %i",
               name.c_str(), key.c_str(), table.size());
      }
      else {
         cpLog(LOG_DEBUG_STACK, "%s NOT purging call: %s from transaction DB, size: %i, p: %llu",
               name.c_str(), key.c_str(), table.size(), p);
         i++;
      }
   }
}//purgeOldCalls

