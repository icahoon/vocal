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


static const char* const StatelessBasicProxy_cxx_Version =
    "$Id: StatelessBasicProxy.cxx,v 1.4 2004/05/27 04:32:18 greear Exp $";


#include "CommandLine.hxx"
#include "ServerContainer.hxx"
#include "SipThread.hxx"
#include "SipTransceiverFilter.hxx"
#include "StatelessBasicProxy.hxx"

using namespace Vocal;


StatelessBasicProxy::StatelessBasicProxy( int hashTableSize,
                                          const string& local_ip,
                                          const string& local_dev_to_bind_to,
                                          unsigned short defaultSipPort,
                                          Data applName,
                                          bool filterOn, 
                                          bool nat,
                                          SipAppContext aContext) {

   //  Filter option controls which transceiver object is created for the
   //  sip stack.

   // NOTE:  SipTransceiverFilter was just a typedef, so the filter flag
   // was making no difference.
   mySipStack = new SipTransceiver(hashTableSize, local_ip,
                                   local_dev_to_bind_to,
                                   applName, defaultSipPort, nat, aContext,
                                   false);
   
   mySipThread = new SipThread(mySipStack, &myCallProcessingQueue, false);    

   if (CommandLine::instance()->getInt("HEARTBEAT")) {
      cpLog(LOG_INFO, "Initializing heartbeat mechanism");
      myHeartbeatThread = new HeartbeatThread(local_ip, local_dev_to_bind_to,
                                              &(ServerContainer::instance()),
                                              defaultSipPort,
                                              HB_RX|HB_TX|HB_HOUSEKEEPING);
   }
}


StatelessBasicProxy::~StatelessBasicProxy()
{
}

int StatelessBasicProxy::setFds(fd_set* input_fds, fd_set* output_fds, fd_set* exc_fds,
                                int& maxdesc, uint64& timeout, uint64 now) {
   mySipThread->setFds(input_fds, output_fds, exc_fds, maxdesc, timeout, now);

   if (myHeartbeatThread != NULL) {
      myHeartbeatThread->setFds(input_fds, output_fds, exc_fds, maxdesc, timeout, now);
   }
}

void StatelessBasicProxy::tick(fd_set* input_fds, fd_set* output_fds, fd_set* exc_fds,
                               uint64 now) {
   mySipThread->tick(input_fds, ouput_fds, exc_fds, now);
   if (myHeartbeatThread != NULL) {
      myHeartbeatThread->tick(input_fds, ouput_fds, exc_fds, now);
   }
}


void
StatelessBasicProxy::process(const Sptr < SipProxyEvent > event) const {
   assert ( event != 0 );

   cpLog( LOG_DEBUG, "StatelessBasicProxy::process event...\n");

   for (   vector<Sptr <Operator> >::const_iterator iter = myOperators.begin(); 
           iter != myOperators.end(); 
           iter++) {
      Sptr < State > newState = (*iter)->process(event);

      if ( newState == PROXY_DONE_WITH_EVENT ) {
         break;
      }
   }
}
