#ifndef PLAYQUEUE_H
#define PLAYQUEUE_H

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
#include <string>
#include <queue>
#include <cstdio>
#include <sndfile.h>
#include <misc.hxx>


/**
  * This Class is used as a queue for media files to play.
  * Wave files get queued and de-queued from this queue.
  */

class PlayQueue {
public:

   ///
   PlayQueue();

   ///
   virtual ~PlayQueue();

   /** Add to the queue */
   virtual void add(const string & fileName);

   /** Return the data in buffer.  Returns true on success, false on failure.
    *  Note that returned data is g711 ulaw, so bytes == samples */
   virtual bool getData(unsigned char *buffer, int bytes_to_read);

   /** Start playing */
   virtual int start();

   /** Stop playing */
   virtual void stop();

   bool isListEmpty() { return m_xFileQueue.empty(); };

   // 0 means loop forever.
   void setLoopWavefileCount(uint32 c) { loop_wavefile_count = c; }

   uint32 getLoopWavefileCount() const { return loop_wavefile_count; }

   //Since we were last started...
   uint32 getLoopWavefileSofar() const { return loop_wavefile_sofar; }

   void incrementWavefileSofar() { loop_wavefile_sofar++; }

private:  // Private attributes

   /** Start playing next song */
   virtual int reStart();

   /**  */
   std::queue < std::string > m_xFileQueue;

   /**  */
   bool m_bActive;

   /**  */
   SNDFILE *m_iFd;
   
   /** Info about the file we are reading. */
   SF_INFO read_info;

   uint32 loop_wavefile_count;
   uint32 loop_wavefile_sofar; //Since we were last started...
};

#endif
