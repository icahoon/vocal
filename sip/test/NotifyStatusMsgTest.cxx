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

static const char* const NotifyStatusMsgTest_cxx_Version =
    "$Id: NotifyStatusMsgTest.cxx,v 1.1 2004/05/01 04:15:26 greear Exp $";
#include "global.h"
#include "symbols.hxx"



#include <cassert>
#include <sys/time.h>
#include "cpLog.h"
//#include "NotifyStatusMsgTest.hxx"
#include "SubscribeMsg.hxx"
#include "SipSubsNotifyEvent.hxx"
#include "StatusMsg.hxx"
#include "NotifyMsg.hxx"
#include "Verify.hxx"


void testEncodeDecode()
{
    Data url;
    url = "sip:destination@192.168.16.10";
    Sptr <BaseUrl> tourl;
    tourl = BaseUrl::decode(url);
    SipTo to(tourl);
    int listenPort = 5060;
    cpLog(LOG_DEBUG, "Completed the Url decoding");

    //create default call-id
    SipCallId callId;

    SubscribeMsg subscribe(callId, to, listenPort);

    //add events
    Data event1 = "Call Back";
    SipSubsNotifyEvent callBackEvent(event1);
    Data event2 = "Message Waiting Indicator";
    SipSubsNotifyEvent messageWaitingEvent(event2);

    subscribe.setSubsNotifyEvent(callBackEvent);
    subscribe.setSubsNotifyEvent(messageWaitingEvent);

    Data data = subscribe.encode();
    cout << " original subscribe msg before decoding " << data.logData() << endl;

    //create a notify message using the last event in the list of SubsNotifyEvent headers.
    NotifyMsg notify(subscribe, subscribe.getSubsNotifyEvent());
    Data notifyData = notify.encode();
    cout << " original notify msg before decoding " << notifyData.logData() << endl;

    //create status response, decode, encode, and compare data objects
    StatusMsg statusMsg(notify, 200);
    Data statusData = statusMsg.encode();
    cout << " status response before decoding " << statusData.logData() << endl;

    StatusMsg newStatusMsg(statusData);

    Data newdata = newStatusMsg.encode();

    cout << "status response after encoding " << newdata.logData() << endl;

    test_verify (statusData ==  newdata);
}

int main()
{
//    cpLogSetPriority(LOG_DEBUG_STACK);

    testEncodeDecode();

    return test_return_code(1);
}
