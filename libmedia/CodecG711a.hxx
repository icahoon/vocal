#ifndef CodecG711A_hxx
#define CodecG711A_hxx

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


#include "CodecAdaptor.hxx"

namespace Vocal
{

namespace UA
{

/** CodecG711U - PCM Alaw codec implementation
 */
class CodecG711A  : public CodecAdaptor
{
public:
   /**Constructor to create a CodecAdaptor, the priority indicate
    * the pref when multiple codecs can be used, a 0 priority means
    * equal preferrence
    */
   CodecG711A(int priority=0)
         : CodecAdaptor(G711A, priority) 
      { 
         myEncodingName = "PCMA";
         myClockRate = 8000;
         myMediaType = AUDIO;
         myAttrValueMap["ptime"] = "20";
         myCompressionRatio = 1;
      };

   /**Decode form codec type to raw data (PCMA), caller should supply the buffer of
    *size atleast length * 2. Returns 0 if successfull or -1 on failure
    * @param length is in bytes
    * @param decBufLen is the space available in decBuf, in units of bytes.
    * @param decodedSamples  Upon return, specifies number of samples that were decoded.
    * @param decodedPerSampleSize Upon return, 
    */
   virtual int decode(char* data, int length, char* decBuf, int decBufLen,
                      int &decodedSamples, int& decodedPerSampleSize,
                      bool is_silence);

   /**Encode from raw data (PCMA) to codec type, caller should supply the buffer of
    * size atleast > length/2. Returns 0 if successfull  or -1 on failure.
    * @ num_samples is not necessarily the length of 'data'.  num_samples * per_sample_size
    *     is the length of 'data' in bytes.
    * @ param encodedLength should be max size of encBuf, in bytes, upon calling,
    *     and will be the actuall number of bytes encoded upon return.
    */
   virtual int encode(char* data, int num_samples, int per_sample_size,
                      char* encBuf, int& encodedLength);

   /** Should not free the bytes returned, is internal data
    */
   virtual char* getSilenceFill(int& len);

   /// Virtual destructor
   virtual ~CodecG711A() { };

   ///
   virtual string className() { return "CodecG711A"; }

protected:
   /** Suppress copying
    */
   CodecG711A(const CodecG711A &);
   
   /** Suppress copying
    */
   const CodecG711A & operator=(const CodecG711A &);
};

}
}

#endif
