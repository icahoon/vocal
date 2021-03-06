
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
#include <cassert>

#include "MediaDevice.hxx"
#include "MediaSession.hxx"
#include "Sdp2Session.hxx"
#include "Sdp2Attributes.hxx"
#include "MediaController.hxx"
#include "SdpHandler.hxx"
#include "CodecSpeex.hxx"
#include "CodecG726_16.hxx"
#include "CodecG726_24.hxx"
#include "CodecG726_32.hxx"
#include "CodecG726_40.hxx"
#include "CodecG729a.hxx"
#include "MRtpSession.hxx"

using namespace Vocal::UA;
using namespace Vocal::SDP;


int MediaSession::_cnt = 0;

MediaSession::MediaSession(int sessionId, 
                           Sptr<NetworkRes> localRes,
                           uint16 tos, uint32 priority,
                           const string& local_dev_to_bind_to,
                           const char* debug,
                           VADOptions *vadOptions, int _jitter_buffer_sz) 
      : mySessionId(sessionId),
        myRtpSession(NULL),
        localDevToBindTo(local_dev_to_bind_to),
        _tos(tos), _skb_priority(priority),
        vadOptions(vadOptions) {
   _cnt++;
   jitter_buffer_sz = _jitter_buffer_sz;

   cpLog(LOG_DEBUG_STACK, "MediaSession constructor, sessionId: %d  debug: %s  cnt: %d  this: %p\n",
         sessionId, debug, _cnt, this);

   mySSRC = 0;
   while (mySSRC == 0) {
      mySSRC =  rand();
   }
   localRes->setBusy(true, "MediaSession constructor");
   localRes->setOwner(this);
   myLocalRes = localRes;
   cpLog(LOG_DEBUG_STACK, "Created media session, this: %p, count: %d, debug: %s\n",
         this, _cnt, debug);
}


MediaSession::~MediaSession() {
   cpLog(LOG_DEBUG_STACK, "MediaSession::~MediaSession, this: %p, count: %d",
         this, _cnt);
   _cnt--;
   tearDown();
   if (myLocalRes.getPtr() != 0) {
      myLocalRes->setOwner(NULL);
      if (myLocalRes->getBusyFlag()) {
         myLocalRes->setBusy(false, "MediaSession destructor");
      }
   }
}
 
void MediaSession::addToSession( Sptr<MediaDevice> mDevice) {
   assertNotDeleted();
   
   if (mDevice != 0) {
      mDevice->setBusy(true);
      mDevice->assignedTo(mySessionId);
      myMediaDevice = mDevice;
   }
}


void MediaSession::addToSession( SdpSession& localSdp, SdpSession& remoteSdp) {
   assertNotDeleted();
   int fmt = 0;

   //cpLog(LOG_ERR, "MediaSession::addToSession, remoteSdp: %p   %s\n", &remoteSdp, remoteSdp.encode().c_str());

   if (myRtpSession != 0) {
      cpLog(LOG_ERR, "Already has a session, ignoring...");
      return;
   }

   // Set remote host and port
   //   For now only support unicast
   string remAddr;
   string localAddr;
   LocalScopeAllocator lo;
   if (localSdp.getConnection())
      localAddr = localSdp.getConnection()->getUnicast().getData(lo);
   if (remoteSdp.getConnection())
      remAddr = remoteSdp.getConnection()->getUnicast().getData(lo);
   
   //Get the Negotiated Codec for each media type
   //Get each media description from remoteSdp and map it to the
   //localSdp SDP and create an MRtpSession
   list < SdpMedia* > mList = remoteSdp.getMediaList();
   for (list < SdpMedia* >::iterator itr = mList.begin();
       itr != mList.end(); itr++) {
      SdpMedia* rMedia = (*itr);
      string rAddr;
      string lAddr;
      int lPort = -1;
      int rPort;
      int sample_rate = 8000;      

      Sptr<CodecAdaptor> cAdp;
      rPort = rMedia->getPort();
      list < SdpMedia* > lmList = localSdp.getMediaList(); 
      for(list < SdpMedia* >::iterator itr2 = lmList.begin();
          itr2 != lmList.end(); itr2++) {
         SdpMedia* lMedia = (*itr2);
         if (lMedia->getMediaType() != rMedia->getMediaType()) {
            continue;
         }
         //Get the very first codec of the media description
         vector < int > * lfmList = lMedia->getFormatList();
         if (!lfmList || (lfmList->size() == 0)) {
            continue;
         }

         vector < int > * rfmList = rMedia->getFormatList();
         if (!rfmList || (rfmList->size() == 0)) {
            continue;
         }
         
         // So, we put our favorites first.  Assume the remote does
         // the same.  So, find the codec that matches the remote's preference.
         for (unsigned int q = 0; q<rfmList->size(); q++) {
            for (unsigned int r = 0; r<lfmList->size(); r++) {
               if ((*rfmList)[q] == (*lfmList)[r]) {
                  fmt = (*rfmList)[q];
                  goto found_one;
               }
            }
         }

         // The goto will jump over this if we actually find a match.
         continue;
               
        found_one:
         
         cAdp = getCodecAdaptor(fmt, lMedia, sample_rate);
         
         if (lMedia->getConnection()) {
            LocalScopeAllocator lo;
            lAddr = lMedia->getConnection()->getUnicast().getData(lo);
         }
         else {
            lAddr = localAddr;
         }
         lPort = lMedia->getPort();
         break;
      }
      if (rMedia->getConnection()) {
         LocalScopeAllocator lo;
         rAddr = rMedia->getConnection()->getUnicast().getData(lo);
         //cpLog(LOG_ERR, "Got remote addr from rMedia: %s", rAddr.c_str());
      }
      else {
         rAddr = remAddr;
         //cpLog(LOG_ERR, "Got remote addr from remAddr: %s", rAddr.c_str());
      }
      
      //Now if we have lAddr, rAddr, lport, rPort and codec adaptor
      if ((lAddr.size() ==0) || (rAddr.size() == 0) || (cAdp == 0)) {
         cpLog(LOG_ERR, "Media is not setup correctly, lAddr: %s  rAddr: %s  cAdp: %p, fmt: %d\n",
               lAddr.c_str(), rAddr.c_str(), cAdp.getPtr(), fmt);
         continue;
      }
      
      //Create an RTP session
      NetworkRes localRes(lAddr, lPort);
      NetworkRes remoteRes(rAddr, rPort);
      myRtpSession = new MRtpSession("MediaSession::addToSession", mySessionId, localRes,
                                     _tos, _skb_priority,
                                     localDevToBindTo,
                                     remoteRes,
                                     cAdp, fmt, mySSRC,
                                     vadOptions, jitter_buffer_sz);
   }
}//addToSession


Sptr<CodecAdaptor> MediaSession::getCodecAdaptor(int fmt, SdpMedia* lMedia,
                                                 int& sample_rate) {
   char tbuf[50];
   snprintf(tbuf, 49, "%d", fmt);
   string fmt_name = tbuf;
   MediaController& mInstance = MediaController::instance();
         
   if (fmt >= rtpPayloadDynMin) {
      SdpRtpMapAttribute attribute;
      int lm_rv = lMedia->getRtpmapAttributeValue(fmt, attribute);
      assert(lm_rv >= 0);
            
      // Now, we got something like 'speex/8000'
      if (strncasecmp(attribute.getEncodingName().c_str(), "speex", 5) == 0) {
         fmt_name = "SPEEX";
         sample_rate = attribute.getClockRate();
      }
      else {
         cpLog(LOG_ERR, "FATAL:  We do not know about this negotiated protocol: %s",
               attribute.getEncodingName().c_str());
         assert("unhandled encoding name for negotiated protocol");
      }
   }

   Sptr<CodecAdaptor> cAdp = mInstance.getMediaCapability().getCodec(fmt_name);
   cpLog(LOG_DEBUG_STACK, "NOTE:  Negotiated format: %d  name: %s  cAdp->type: %d\n",
         fmt, fmt_name.c_str(), cAdp->getType());
         
   if (cAdp->getType() == G726_16) {
      // Make a copy, we have state in this codec.
      cAdp = new CodecG726_16(*((CodecG726_16*)(cAdp.getPtr())));
      cAdp->setRtpType(fmt);
      cAdp->setClockRate(sample_rate);
   }
#ifdef USE_VOICE_AGE
   else if (cAdp->getType() == G729) {
      // Make a copy, we have state in the g729 codec.
      cAdp = new CodecG729a(*((CodecG729a*)(cAdp.getPtr())));
      cAdp->setRtpType(fmt);
      cAdp->setClockRate(sample_rate);
   }
#endif
#ifdef USE_SPEEX
   else if (cAdp->getType() == SPEEX) {
      // Make a copy, we have state in the Speex codec.
      cAdp = new CodecSpeex(*((CodecSpeex*)(cAdp.getPtr())));
      cAdp->setRtpType(fmt);
      cAdp->setClockRate(sample_rate);
   }
#endif
   else if (cAdp->getType() == G726_24) {
      // Make a copy, we have state in this codec.
      cAdp = new CodecG726_24(*((CodecG726_24*)(cAdp.getPtr())));
      cAdp->setRtpType(fmt);
      cAdp->setClockRate(sample_rate);
   }
   else if (cAdp->getType() == G726_32) {
      // Make a copy, we have state in this codec.
      cAdp = new CodecG726_32(*((CodecG726_32*)(cAdp.getPtr())));
      cAdp->setRtpType(fmt);
      cAdp->setClockRate(sample_rate);
   }
   else if (cAdp->getType() == G726_40) {
      // Make a copy, we have state in this codec.
      cAdp = new CodecG726_40(*((CodecG726_40*)(cAdp.getPtr())));
      cAdp->setRtpType(fmt);
      cAdp->setClockRate(sample_rate);
   }
   return cAdp;
}//getCodecAdaptor


int MediaSession::tearDown() {
   assertNotDeleted();
   cpLog(LOG_DEBUG, "Tearing down the session:%d" , mySessionId);

   // Flush the jitter buffer.
   if (myRtpSession != 0) {
      myRtpSession->flushJitterBuffer();
   }

   // Stop them both...
   if (myMediaDevice != 0) {
      myMediaDevice->stop("MediaSession::teardown");
   }
   
   // Now, clear out the references.
   if (myRtpSession != 0) {
      delete myRtpSession;
      myRtpSession = NULL;
   }
   
   if (myMediaDevice != 0) {
      myMediaDevice->assignedTo(-1);
      myMediaDevice->setBusy(false);
      myMediaDevice = 0;
   }
   
   myLocalRes->setBusy(false, "tear down");
   assertNotDeleted();
   return 1;
}

// This assumes that the data is already right for myCodec.
// Use sinkRaw if that is not the case.
int MediaSession::processCooked(const char* data, int length, int samples,
                                VCodecType type) {
   if (myRtpSession != 0) {
      return myRtpSession->sinkCooked(data, length, samples, type);
   }
   else {
      cpLog(LOG_ERR, "ERROR:  myRtpSession is NULL!\n");
      return -1;
   }
}

void 
MediaSession::processRaw(char *data, int len, VCodecType cType,
                         Sptr<CodecAdaptor> codec, Adaptor* adp,
                         bool silence_pkt, RtpPayloadCache* payload_cache) {
   assertNotDeleted();
   cpLog(LOG_DEBUG_STACK, "processRaw, RTP: %d  adp->deviceType: %d  adp->instanceName: %s  adp->description: %s silence_pkt: %i\n",
         (int)(RTP), (int)(adp->getDeviceType()), adp->getInstanceName().c_str(),
         adp->getDescription().c_str(), silence_pkt);

   if (adp->getDeviceType() != RTP) {
      //Data from hardware, ship it out to the RTP session
      if (myRtpSession != 0) {
         myRtpSession->sinkData(data, len, cType, codec, silence_pkt, payload_cache);
      }
      else {
         cpLog(LOG_ERR, "ERROR:  myRtpSession is NULL!\n");
      }
   }
   else {
      cpLog(LOG_DEBUG_STACK, "processRaw, using device: %s to sink data.\n",
            myMediaDevice->getInstanceName().c_str());
      myMediaDevice->sinkData(data, len, cType, codec, silence_pkt, NULL);
   }
}

void MediaSession::tick(fd_set* input_fds, fd_set* output_fds, fd_set* exc_fds,
                        uint64 now) {
   if (myRtpSession) {
      myRtpSession->tick(input_fds, output_fds, exc_fds, now);
   }
   if (myMediaDevice != 0) {
      myMediaDevice->tick(input_fds, output_fds, exc_fds, now);
   }
}

int MediaSession::setFds(fd_set* input_fds, fd_set* output_fds, fd_set* exc_fds,
                         int& maxdesc, uint64& timeout, uint64 now) {
   if (myRtpSession) {
      myRtpSession->setFds(input_fds, output_fds, exc_fds, maxdesc, timeout, now);
   }
   if (myMediaDevice != 0) {
      myMediaDevice->setFds(input_fds, output_fds, exc_fds, maxdesc, timeout, now);
   }
   return 0;
}


void MediaSession::startSession(VSdpMode mode) {
   assertNotDeleted();
   if (myRtpSession != 0) {
      myMediaDevice->start(myRtpSession->getCodec()->getType());
      myRtpSession->setMode(mode); 
   }
   else {
      cpLog(LOG_ERR, "WARNING:  myRtpSession is NULL in startSession");
      // Default to use G711 U codec.
      myMediaDevice->start(G711U);
   }
}

uint32 MediaSession::getPerPacketTimeMs() {

   uint64 rv = 20;
   if (myMediaDevice.getPtr()) {
      rv = myMediaDevice->getDataRate();
   }
   return rv;
}//getPerPacketTimeMs

void MediaSession::suspend() {
   myMediaDevice->suspend(); 
}

int MediaSession::resumeSession(SdpSession& localSdp, SdpSession& remoteSdp) {
   int rv = -1;
   if (myRtpSession != 0) {
      rv = myRtpSession->adopt(localSdp, remoteSdp);
      myMediaDevice->resume(myRtpSession->getCodec()->getType());
   }
   return rv;
}

SdpSession MediaSession::getSdp(VSdpMode mode) {
   string localAddr;
   int    localPort;
   localAddr = myLocalRes->getIpName().c_str();
   localPort = myLocalRes->getPort();
   SdpSession sdp;
   setStandardSdp(sdp, localAddr, localPort, mode);
   sdp.setSessionId(mySessionId);
   return sdp;
}
 

SdpSession MediaSession::getSdp(SdpSession& remoteSdp) {
   string localAddr;
   int    localPort;
   localAddr = myLocalRes->getIpName().c_str();
   localPort = myLocalRes->getPort();
   SdpSession sdp;
   negotiateSdp(sdp, localAddr, localPort, remoteSdp);
   sdp.setSessionId(mySessionId);
   return sdp;
}
