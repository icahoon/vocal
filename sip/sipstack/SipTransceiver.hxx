#ifndef SipTransceiver_HXX
#define SipTransceiver_HXX

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



static const char* const SipTransceiver_hxx_Version
= "$Id: SipTransceiver.hxx,v 1.3 2004/05/07 17:30:46 greear Exp $";


#include <string>
#include "global.h"
#include "TransceiverSymbols.hxx"

#include "SipTcpConnection.hxx"
#include "SipUdpConnection.hxx"

#include "SipTransactionDB.hxx"
#include "SipSentRequestDB.hxx"
#include "SipSentResponseDB.hxx"
#include "SipTransactionLevels.hxx"
#include "SipTransactionId.hxx"

#include "SipSnmpDetails.hxx"
#include "SipMsgQueue.hxx"
#include "SipMsg.hxx"
#include "SipCommand.hxx"
#include "StatusMsg.hxx"
#include "Sptr.hxx"

namespace Vocal
{
    
///
typedef enum
{
   APP_CONTEXT_GENERIC,
   APP_CONTEXT_PROXY
}SipAppContext;


/**
   SipTransceiver is the main class for users the SIP stack.  It is the object
   which the caller uses to send and receive SIP messages.
*/
class SipTransceiver: public BugCatcher
{
    public:
	/**
         * hashTableSizeHint is what we'll use for the initial number of hash table
         * buckets.
         * @param local_dev_to_bind_to  If not "", we'll bind to this
         *  device with SO_BINDTODEV
         */
 	SipTransceiver(int hashTableSizeHint,
                       const string& local_ip,
                       const string& local_ip_to_bind_to,
                       Data s, /* = 0 */
		       int sipPort,/* = SIP_PORT, */
		       bool nat/* = false*/ , 
		       SipAppContext aContext/*=APP_CONTEXT_GENERIC*/,
                       bool blocking);

	///
	virtual ~SipTransceiver();

	/**
	 * Copy on write is "expensive", can we change this interface,
	 * specially since we know that most of these are in Sptr's
	 * already
	 */
	virtual void sendAsync(SipCommand& sipMessage);

	///Interface so that we do not have to copy message again in stack
	void sendAsync(Sptr<SipCommand> sipMessage, const Data& host="",
                       const Data& port="5060");

	///
	virtual void sendReply(StatusMsg& sipMessage);

	///Interface so that we do not have to copy message again in stack
	virtual void sendReply(Sptr<StatusMsg> sipMessage);

	/**
	 ** Return a deque of SipMsgs, basically containing the msg chain.
	 ** A timeOut out of -1 means infinite timeout.
	 */
	//TODO: virtual Sptr < SipMsgQueue > receive(int timeOutMs = -1);

	///
	static void reTransOff();

	///
	static void reTransOn();


	///
	static void setRetransTime(int initial = retransmitTimeInitial ,
				   int max = retransmitTimeMax 
				   /* Default values in TranscevierSymbols.hxx*/ );

	/// ?????????
	void setRandomLosePercent(int percent);

        const string getLocalIp() const { return localIp; }

	///
	void updateSnmpData(Sptr < SipMsg > sipMsg, SnmpType snmpType);

	///
	SipTransactionDB::CallLegVector getCallLegMsgs(Sptr < SipMsg >
						       sipmsg);
	///
	void printSize();

	///
	static SipAppContext myAppContext;

	///
	Data getLocalNamePort() const;

        static int getInstanceCount() { return atomic_read(&_cnt); }

    private:
	///
        SipTransceiver();
	SipTransceiver(const SipTransceiver&);
	///
	SipTransceiver& operator =(const SipTransceiver& src);
  

	/* ---------------------------------------------------------------
           these will have to be shared iff there has to be thread pooling 
           --------------------------------------------------------------- */

	// we'll do the decoding of sip msgs in the sip thread's context
	// (encoding is in worker's context when it calls send)
	// for the time being this will be in udp, but later move it here
	list < Sptr < SipMsgContainer> > recvdMsgsFifo;

	Sptr < SipUdpConnection > udpConnection;

	Sptr < SipTcpConnection > tcpConnection;

	SipSentRequestDB sentRequestDB;

	SipSentResponseDB sentResponseDB;

	///
	void send(SipMsgContainer *msgPtr, const Data& host="",
                  const Data& port="5060");
  
	/// SNMP details data member vriables
	Sptr<SipAgent> sipAgent;

	Data myLocalNamePort;
  
	Data application;
  
	bool natOn;

        string localIp;

        static atomic_t _cnt;
};
 
} // namespace Vocal

/* Local Variables: */
/* c-file-style: "stroustrup" */
/* indent-tabs-mode: nil */
/* c-file-offsets: ((access-label . -) (inclass . ++)) */
/* c-basic-offset: 4 */
/* End: */

#endif
