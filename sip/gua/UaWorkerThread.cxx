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

static const char* const UaWorkerThread_cxx_Version =
    "$Id: UaWorkerThread.cxx,v 1.2 2004/06/02 03:38:05 greear Exp $";


#include "SipEvent.hxx"
#include "UaBase.hxx"
#include "CallTimerEvent.hxx"
#include "CallDB.hxx"
#include "UaWorkerThread.hxx"
#include "UaCallControl.hxx"
#include "UaFacade.hxx"


using namespace Vocal::UA;

void
UaWorkerThread::shutdown()
{
    assert( myFifo != 0 );

    ThreadIf::shutdown();

    myFifo->add(0);
}


void
UaWorkerThread::thread()
{
    allDone = 1;
    cpLogSetLabelThread (VThread::selfId(), "bcWorker");

    while ( true )
    {
        assert( myFifo != 0 );

        Sptr < SipProxyEvent > nextEvent = myFifo->getNext();

        cpLog(LOG_DEBUG, "UaWorkerThread received event");

        if ( isShutdown() )
        {
            cpLog(LOG_ERR, "NOTE: Exiting UaWorker thread...");
            break;
        }

        assert( nextEvent != 0 );
        try
        {
            UaCallControl::instance().processEvent(nextEvent);
        }
        catch(VException& e)
        {
            cpLog(LOG_ERR, "Failed to process event, reason %s", e.getDescription().c_str());

            // Tell the GUI about this problem.
            string errm = "LOCAL_EXCEPTION: ";
            errm += e.getDescription().c_str();
            UaFacade::instance().postMsg(errm.c_str());
        }
    }//while
    cpLog(LOG_DEBUG, "Exiting worker thread...");
    allDone = 2;
}

