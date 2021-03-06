#ifndef RtspTransceiver_hxx
#define RtspTransceiver_hxx
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
 * IN EXCESS OF 281421,000, NOR FOR ANY INDIRECT, INCIDENTAL, SPECIAL,
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



static const char* const RtspTransceiver_hxx_Version =
    "$Id: RtspTransceiver.hxx,v 1.1 2004/05/01 04:15:23 greear Exp $";

#include "RtspMsg.hxx"
#include "RtspResponse.hxx"
#include "RtspTcpConnection.hxx"

#include "Fifo.h"
#include "Sptr.hxx"

/** RtspTransceiver
 *  Accepts new TCP connections on listenPort.
 *  For each new connection a new RtspTcpBuffer object is added to
 *  myTcpConnection map.  All connections are processed by a select ()
 *  loop.  RTSP messages are received from connection and inserted into
 *  a shared recvFifo for RtspServer to process
 */
class RtspTransceiver
{
    public:
        /** constructor
         *  @param recvFifo fifo to insert new RTSP messages
         *  @param listenPort port to accept new connections
         */
        RtspTransceiver( Fifo< Sptr<RtspMsg> >& recvFifo, int listenPort );
        /** deconstructor */
        virtual ~RtspTransceiver();

        /** sends an rtsp request msg */
        void send( Sptr<RtspMsg> msg )  throw (VNetworkException&);
        /** sends an rtsp response msg */
        void send( Sptr<RtspResponse> msg )  throw (VNetworkException&);

        /** writes nchar to socket */
        static void writeData( Connection conn, void* data, size_t len )
            throw( VNetworkException& );

        /** reads nchar from a given sockets, bRead returns the bytes read */
        static int readData( Connection conn, void *dataRead, size_t nchar,
            int &bRead ) throw( VNetworkException& );

    private:
        /** TCP Interface */
        RtspTcpConnection myTcpConnection;

    protected:
        /** suppress copy constructor */
        RtspTransceiver( const RtspTransceiver& );
        /** suppress assignment operator */
        RtspTransceiver& operator=( const RtspTransceiver& );
};

/* Local Variables: */
/* c-file-style: "stroustrup" */
/* indent-tabs-mode: nil */
/* c-file-offsets: ((access-label . -) (inclass . ++)) */
/* c-basic-offset: 4 */
/* End: */
#endif
