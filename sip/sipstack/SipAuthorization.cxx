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

static const char* const SipAuthorization_cxx_version =
    "$Id: SipAuthorization.cxx,v 1.1 2004/05/01 04:15:26 greear Exp $";

#include "global.h"
#include "SipAuthorization.hxx"
#include "symbols.hxx"
#include "SipParserMode.hxx"
#include "cpLog.h"

using namespace Vocal;

string
SipAuthorizationParserException::getName( void ) const
{
    return "SipAuthorizationParserException";
}


SipAuthorization::SipAuthorization( const Data& srcData, const string& local_ip )
    : SipHeader(local_ip),
      myAuthScheme(),
      myParamList(',')
{
    if (srcData == "") {
        return;
    }

    Data fdata = srcData;
    try
    {
        decode(fdata);
    }
    catch (SipAuthorizationParserException&)
    {
        if (SipParserMode::sipParserMode())
        {

            throw SipAuthorizationParserException(
                "failed to decode the Authorization header",
                __FILE__,
                __LINE__,
                DECODE_FAILED_AUTHORIZE);
        }
    }
}


SipAuthorization::SipAuthorization( const SipAuthorization& src )
        : SipHeader(src),
          myAuthScheme(src.myAuthScheme),
          myBasicCookie(src.myBasicCookie),
          myParamList(src.myParamList)
{
}


SipAuthorization& 
SipAuthorization::operator =( const SipAuthorization& src)
{
    if (this != &src)
    {
        myParamList = src.myParamList;
        myAuthScheme = src.myAuthScheme;
        myBasicCookie = src.myBasicCookie;
    }
    return *this;
}


bool 
SipAuthorization::operator ==( const SipAuthorization& src) const
{
   return ( myParamList == src.myParamList && 
            isEqualNoCase(myAuthScheme, src.myAuthScheme ) &&
            isEqualNoCase(myBasicCookie, src.myBasicCookie )  );
}


void 
SipAuthorization::decode(const Data& data)
{
    Data nData = data;

    try
    {
        scanSipAuthorization(nData);
    }
    catch (SipAuthorizationParserException exception)
    {
        if (SipParserMode::sipParserMode())
        {
            throw SipAuthorizationParserException(
                exception.getDescription(),
                __FILE__,
                __LINE__,
                DECODE_FAILED_AUTHORIZE
            );
        }
    }
}


void
SipAuthorization::scanSipAuthorization(const Data& tmpdata)
{
    Data authdetails = tmpdata ;
    Data authType;
    int ret = authdetails.match(" ", &authType, true);
    if (ret == FIRST)
    {
        if (SipParserMode::sipParserMode())
        {
            throw SipAuthorizationParserException(
                "failed to decode the Authorization header",
                __FILE__,
                __LINE__,
                DECODE_FAILED_AUTHORIZE);
        }
    }

    if (ret == FOUND)
    {
        if (  (isEqualNoCase( authType , AUTH_BASIC )) ||
              (isEqualNoCase( authType , AUTH_DIGEST )) ||
              (isEqualNoCase( authType ,AUTH_PGP ))
           )
        {

            setAuthScheme(authType);

            if (isEqualNoCase(authType , AUTH_BASIC))
            {
                //store cookie.
                setBasicCookie(authdetails);
                return ;
            }

            try
            {
                myParamList.decode(authdetails, ',', true);
            }
            catch (SipAuthorizationParserException&)
            {
                if (SipParserMode::sipParserMode())
                {
                    throw (SipAuthorizationParserException(
                               "failed in parsing auth tokens",
                               __FILE__,
                               __LINE__,
                               DECODE_FAILED_AUTHORIZE));
                }
            }

        }
        else
        {
            if (SipParserMode::sipParserMode())
            {
                throw SipAuthorizationParserException(
                    "auth scheme incorrect.",
                    __FILE__,
                    __LINE__,
                    DECODE_FAILED_AUTHORIZE);
            }
        }

    }
    if (ret == NOT_FOUND)
    {
        throw SipAuthorizationParserException(
            "No AuthScheme",
            __FILE__,
            __LINE__,
            DECODE_FAILED_AUTHORIZE);
    }

}


/*** return the encoded string version of this. This call should only be
     used inside the stack and is not part of the API */
Data 
SipAuthorization::encode() const
{
    Data data;

    if ( myAuthScheme.length() )
    {
        data += AUTHORIZATION;
        data += SP;
        data += myAuthScheme;
        data += SP;

        if (isEqualNoCase(myAuthScheme, AUTH_BASIC))
        {
            data += myBasicCookie;
        }
        else
        {
            data += myParamList.encode();
        }//other than basic.
        data += CRLF;
    }
    return data;
}


void 
SipAuthorization::setAuthScheme(const Data& data)
{
    myAuthScheme = data;
}


Data 
SipAuthorization::getAuthScheme() const
{
    return myAuthScheme;
}


void
SipAuthorization::setBasicCookie(const Data& data)
{
    myBasicCookie = data;
}


Data
SipAuthorization::getBasicCookie() const
{
    return myBasicCookie;
}


void 
SipAuthorization::setTokenDetails(const Data& paramName,
                             const Data& paramValue)
{
    myParamList.setValue(paramName, paramValue);
}


Data 
SipAuthorization::getTokenValue(const Data& token) const
{
    Data ret;
    //Strip off any "" from the value
    string tokenstr = myParamList.getValue(token).convertString();
    int pos;
    pos = tokenstr.find("\"");

    if (pos != static_cast<int>(string::npos))
    {
         tokenstr = tokenstr.substr(pos + 1, tokenstr.length() - 2);
    }

    ret = tokenstr;
    return (ret);
}


SipHeader*
SipAuthorization::duplicate() const
{
    return new SipAuthorization(*this);
}


bool
SipAuthorization::compareSipHeader(SipHeader* msg) const
{
    SipAuthorization* otherMsg = dynamic_cast<SipAuthorization*>(msg);
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
