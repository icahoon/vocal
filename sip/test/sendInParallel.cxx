
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
static const char* const sendInviteVersion =
    "$Id: sendInParallel.cxx,v 1.1 2004/05/01 04:15:26 greear Exp $";

#include <cstdlib>
#include <fstream>
#include <pthread.h>

#include "cpLog.h"
#include "Sptr.hxx"
#include "Data.hxx"
#include "VovidaSipStack.hxx"
#include "InviteMsg.hxx"
#include "AckMsg.hxx"
#include "TimeFunc.hxx"
#include "ByeMsg.hxx"

using namespace Vocal;



int numOfMessages = 1;
int interval = 2000000;     // 2 seconds
InviteMsg invite;



void * sendReq(void * stack);
void * recvResp(void * stack);
///
int
main( int argc, char* argv[] )
{
    if ( argc < 3 )
    {
        cerr << "Usage: "
        << argv[0]
        << " <local_SIP_port>"
        << " <msg_file_name>"
        << " [ <#messages> [ <interval(ms)> [ s|d ] ] ]"
        << endl;
        exit( -1 );
    }

    Data logFile = "/tmp/its";
    // logFile += getpid();
    // logFile += ".log";
    cpLogOpen( logFile.logData() );

    cpLogSetPriority(LOG_DEBUG_STACK);
    cout << argc << endl;
    if ( argc > 5 )
    {
        cpLogSetPriority( (*(argv[5]) == 's' ? LOG_DEBUG_STACK : LOG_DEBUG ) );
    }


    SipTransceiverFilter* stack;
    stack = new SipTransceiverFilter( argv[0], atoi( argv[1] ) );

    stack->setRandomLosePercent(0);

    std::ifstream inputFile( argv[2] );
    if ( !inputFile )
    {
        cerr << "Error: Cannot open input file " << argv[2] << endl;
        exit( -1 );
    }
    string str;
    char ch;
    while ( inputFile.get( ch ) )
    {
        str += ch;
    }

    // Optional command line arguments
    if ( argc >= 4 )
    {
        numOfMessages = atoi( argv[3] );
    }

    if ( argc >= 6 )
    {
        interval = atoi( argv[5] ) * 1000;
    }

    pthread_t sendThread;
    pthread_t recvThread;

    cout << "\t<<< Send Invite Message test >>>\n" << endl;
    cout << "     Local SIP Port : " << argv[1] << endl;
    cout << "         Input File : " << argv[2] << endl;
    cout << " Number of Messages : " << numOfMessages << endl;
    cout << "           Interval : " << interval << endl;
    cout << "           Log File : " << logFile.logData() << endl;
    cout << endl << "<<< Message to send begin >>>" << endl << str << endl;
    cout << "<<< Message to send end >>>" << endl << endl;

    Data msg(str);
    InviteMsg inviteMsg( msg );

    invite = inviteMsg;
    
    pthread_create(&sendThread, 0, sendReq, (void*)stack);
    pthread_create(&recvThread, 0, recvResp, (void*)stack);

    pthread_join(sendThread, 0);
    pthread_join(recvThread, 0);

}

void * sendReq(void * s)
{
    SipTransceiverFilter * stack = (SipTransceiverFilter *)s;

    Sptr < SipMsg > sipMsg;
    InviteMsg inviteMsg = invite;

    for ( int i = 1; i <= numOfMessages; i++ )
    {
        SipCallId callId;
        inviteMsg.setCallId( callId );

        // Note: this is needed as the stack won't code the from user
        // field otherwise
        SipFrom from = inviteMsg.getFrom();
        from.setUser(Data("-"));
        inviteMsg.setFrom(from);

        // Send INVITE
        stack->sendAsync( inviteMsg );
    }
    
    return 0;
}

void * recvResp(void * s)
{
    SipTransceiverFilter * stack = (SipTransceiverFilter *)s;

    Sptr < SipMsgQueue > sipRcv;
    
    Sptr < StatusMsg > statusMsg;
    Sptr <SipMsg> sipMsg;

    if (stack == 0)
    {
        return 0;
    }
       
    // Get response
    while ( 1 )
    {
        sipRcv = stack->receive();
        if ( sipRcv != 0 )
        {
            sipMsg = sipRcv->back();
            if ( sipMsg != 0 )
            {
                statusMsg.dynamicCast( sipMsg );
                if ( statusMsg == 0 )
                {
                    cpLog( LOG_ERR, "Not a status message" );
                }
                else
                {
                    int status = statusMsg->getStatusLine().getStatusCode();
                    Data method = statusMsg->getCSeq().getMethod();
                    if ( status >= 200 )
                    {
                            // Send ACK
                            AckMsg ackMsg( *statusMsg );
                            //get the original request.
                            if (status > 200)
                            {
                                Sptr <SipMsg> origInvite = sipRcv->back();
                                if (origInvite != 0)
                                {
                                    Sptr<InviteMsg> origInviteMsg;
                                    origInviteMsg.dynamicCast(origInvite);
                                    if (origInviteMsg != 0)
                                    {
                                        Sptr<BaseUrl> baseDest = 
                    origInviteMsg->getRequestLine().getUrl();
                                        SipRequestLine reqLine;
                                        reqLine.setUrl(baseDest);
                                        ackMsg.setRequestLine(reqLine);
                                    }

                                }
                            }
                            stack->sendAsync( ackMsg );
                    }
                }
            }
        }
    }

    if (stack)
    {
        delete stack;
    }
        
    return 0;
}
