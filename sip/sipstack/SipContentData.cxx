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
#include "SipContentData.hxx"

using namespace Vocal;

SipContentData::SipContentData(const string& _local_ip)
    : contentType(),
      contentLength(),
      contentDisposition(),
      local_ip(_local_ip)
{
}


SipContentData::SipContentData( const SipContentData& rhs )
    : local_ip(rhs.local_ip)
{
    contentType = rhs.contentType;
    contentLength = rhs.contentLength;
    contentDisposition = rhs.contentDisposition;
    
}


SipContentData&
SipContentData::operator=( const SipContentData& rhs )
{
    if ( &rhs != this )
    {
        contentType = rhs.contentType;
	contentLength = rhs.contentLength;
	contentDisposition = rhs.contentDisposition;
	
    }
    return *this;
}


Data
SipContentData::encodeHeaders() const
{
    Data data;
    
    if (contentType != 0)
    {
	data += contentType->encode();
    }
    if (contentLength != 0)
    {
	data += contentLength->encode();
    }
    if (contentDisposition != 0)
    {
	data += contentDisposition->encode();
    }
    return data;
}
 

Sptr <SipContentType> SipContentData::getContentType()
{
    return contentType;
}


void SipContentData::setContentType(Sptr <SipContentType>  contType)
{
    contentType = contType;
}
    

Sptr <SipContentLength> SipContentData::getContentLength()
{
    return contentLength;
}

    
void SipContentData::setContentLength(Sptr <SipContentLength> contLen)
{
    contentLength = contLen;
}

    
Sptr <SipContentDisposition> SipContentData::getContentDisposition()
{
    return contentDisposition;
}

    
void SipContentData::setContentDisposition(Sptr <SipContentDisposition> contDisp)
{
    contentDisposition = contDisp;
}


/* Local Variables: */
/* c-file-style: "stroustrup" */
/* indent-tabs-mode: nil */
/* c-file-offsets: ((access-label . -) (inclass . ++)) */
/* c-basic-offset: 4 */
/* End: */
