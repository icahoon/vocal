#ifndef SIP_ALSO_HXX_
#define SIP_ALSO_HXX_

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
#include "Data.hxx"
#include "BaseUrl.hxx"
#include "VException.hxx"
#include "cpLog.h"
#include <map>
#include "Sptr.hxx"
#include "SipHeader.hxx"

namespace Vocal
{

enum SipAlsoErrorType
{
    DECODE_ALSO_FAILED,
    PARSE_AURL_FAILED,
    USERINFO_AERR
    //may need to change this to be more specific
};

/// Exception handling class SipAlsoParserException
class SipAlsoParserException : public VException
{
    public:
        SipAlsoParserException( const string& msg,
                                const string& file,
                                const int line,
                                const int error = 0 );
        SipAlsoParserException( const string& msg,
                                const string& file,
                                const int line,
                                SipAlsoErrorType i)
        : VException( msg, file, line, static_cast < int > (i))
        {
            value = i;
        }
        SipAlsoErrorType getError() const
        {
            return value;
        }
        string getName() const ;
    private:
        SipAlsoErrorType value;

};



/// data container for Also header
class SipAlso : public SipHeader
{
    public:

        ///
        SipAlso( Sptr <BaseUrl> url, const string& local_ip);

        ///  If data == "", no decoding will be attempted.
        SipAlso( const Data& data, const string& local_ip, UrlType uType = SIP_URL );
        ///
        SipAlso(const SipAlso&);

        ///
        Data encode() const;
        ///
        SipAlso& operator = (const SipAlso&);
        ///
        void setDisplayName(const Data& name);
        ///
        Data getDisplayName() const;
        ///
        void setHost(const Data& newhost);
        ///
        Data getHost() const;
        ///
        Sptr <BaseUrl> getUrl() const;
        ///
        void setUrl(Sptr <BaseUrl> alsourl);

        ///
        bool operator ==(const SipAlso& srcAlso) const;


	/// method for copying sip headers of any type without knowing which type
	Sptr<SipHeader> duplicate() const;
	/// compare two headers of (possibly) the same class
	virtual bool compareSipHeader(SipHeader* msg) const;
    private:
        ///
        Sptr <BaseUrl> alsoUrl;
        ///
        UrlType urlType;
        ///
        Data displayName;

        void decode(const Data& data);
        void parse(const Data& data);
        void scanSipAlso(const Data& data);
        void parseNameInfo(const Data& data);
        void parseUrl(const Data& data);
        friend class SipMsg;

        SipAlso(); //Not Implemented
};
 
} // namespace Vocal

#endif
