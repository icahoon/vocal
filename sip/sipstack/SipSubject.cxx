
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

static const char* const SipSubject_cxx_Version =
    "$Id: SipSubject.cxx,v 1.2 2004/06/16 06:51:26 greear Exp $";


#include "global.h"
#include "SipSubject.hxx"
#include "symbols.hxx"
#include "SipParserMode.hxx"
#include "cpLog.h"

using namespace Vocal;


string
SipSubjectParserException::getName( void ) const
{
    return "SipSubjectParserException";
}


SipSubject::SipSubject( const SipSubject & src)
    : SipHeader(src)
{
    data = src.data;
}

const SipSubject& SipSubject::operator=(const SipSubject& src)
{
    if ( &src != this)
    {
        data = src.data;
    }
    return (*this);
}
///
bool SipSubject::operator==(const SipSubject& src) const
{
    return ( data ==src.data );
}

SipSubject::SipSubject( const Data& srcData, const string& local_ip )
    : SipHeader(local_ip)
{
    if (srcData != "") {
        Data wdata = srcData;
        try
        {
            decode(wdata);
        }
        catch (SipSubjectParserException&)
        {
            if (SipParserMode::sipParserMode())
            {
                cpLog(LOG_ERR, "Failed to Decode in Constructor of Subject :( ");
                throw SipSubjectParserException(
                    "failed to decode the Subject string :(",
                    __FILE__,
                    __LINE__, DECODE_SUBJECT_FAILED);
            }
        }
    }
}

void SipSubject::decode(const Data& indata)
{

    Data nData = indata;

    try
    {

        scanSipSubject(nData);
    }
    catch (SipSubjectParserException& exception)
    {
        if (SipParserMode::sipParserMode())
        {
            cpLog(LOG_ERR, "Failed to Decode in Constructor of Subject :( ");
            throw SipSubjectParserException(
                exception.getDescription(),
                __FILE__,
                __LINE__, DECODE_SUBJECT_FAILED
            );
        }
    }
}

void
SipSubject::scanSipSubject(const Data &tmpdata)
{
    set(tmpdata);
}



Data SipSubject::get() const
{
    return data;
}

void SipSubject::set( const Data& newdata)
{
    data = newdata;
}

Data SipSubject::encode() const
{
    Data ret;
    if (data.length())
    {
        ret = SUBJECT;
        ret += SP;
        ret += data;
        ret += CRLF;
    }
    return ret;
}



Sptr<SipHeader>
SipSubject::duplicate() const
{
    return new SipSubject(*this);
}


bool
SipSubject::compareSipHeader(SipHeader* msg) const
{
    SipSubject* otherMsg = dynamic_cast<SipSubject*>(msg);
    if(otherMsg != 0)
    {
	return (*this == *otherMsg);
    }
    else
    {
	return false;
    }
}
/* Local Variables: */
/* c-file-style: "stroustrup" */
/* indent-tabs-mode: nil */
/* c-file-offsets: ((access-label . -) (inclass . ++)) */
/* c-basic-offset: 4 */
/* End: */
