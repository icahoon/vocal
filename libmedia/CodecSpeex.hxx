#ifndef CodecSpeex_hxx
#define CodecSpeex_hxx

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


static const char* const CodecSpeex_hxx_Version = 
    "$Id: CodecSpeex.hxx,v 1.1 2004/05/01 04:15:16 greear Exp $";

#include "CodecAdaptor.hxx"
#include <speex.h>


#define VSPEEX_FRAME_SIZE 160

namespace Vocal
{

namespace UA
{

class CodecSpeex  : public CodecAdaptor
{
public:

   CodecSpeex(const CodecSpeex &);

   /**Constructor to create a CodecAdaptor, the priority indicate
    * the pref when multiple codecs can be used, a 0 priority means
    * equal preferrence
    */
   CodecSpeex(int priority=0);

   //From codec type  to raw data (PCMU)
   virtual int decode(char* data, int length, char* decBuf, int decBufLen,
                      int &decodedSamples, int& decodedPerSampleSize);

   //from raw data (PCMU) to codec type 
   virtual int encode(char* data, int num_samples, int per_sample_size,
                      char* encBuf, int& encodedLength);

   /// Virtual destructor
   virtual ~CodecSpeex();

   ///
   virtual string className() { return "CodecSpeex"; }

protected:

   void commonInit();

   SpeexBits enc_bits;
   SpeexBits dec_bits;
   void *enc_state;
   void *dec_state;

   float decode_floats[VSPEEX_FRAME_SIZE];
   int overflow_catcher;

private:

   /** Suppress copying
    */
   const CodecSpeex & operator=(const CodecSpeex &);
};

}
}

#endif
