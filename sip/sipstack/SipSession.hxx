#ifndef SipSession_HXX
#define SipSession_HXX

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
static const char* const SipSessionVersion =
    "$Id: SipSession.hxx,v 1.1 2004/05/01 04:15:26 greear Exp $";



#include "Data.hxx"
#include "VException.hxx"
#include "SipHeader.hxx"

namespace Vocal
{
    
enum SipSessionErrorType
{
    DECODE_SESSION_FAILED
};


/// Exception handling class
class SipSessionParserException : public VException
{
    public:
        SipSessionParserException( const string& msg,
                                       const string& file,
                                       const int line,
                                       const int error = 0 );
        SipSessionParserException( const string& msg,
                                       const string& file,
                                       const int line,
                                       SipSessionErrorType i)
        : VException( msg, file, line, static_cast < int > (i))
        {
            value = i;
        }
        SipSessionErrorType getError() const
        {
            return value;
        }
        string getName() const ;
    private:
        SipSessionErrorType value;

};

/// data container for Session header
class SipSession : public SipHeader
{
    public:
    ///
    SipSession( const SipSession& src);
    ///
    SipSession( const Data& data, const string& local_ip );
    ///
    bool operator ==(const SipSession& other) const;
    ///
    SipSession& operator=( const SipSession& src);
    ///
    Data encode() const;
    ///
    bool isSessionMedia();
    ///
    bool isSessionQos();
    ///
    bool isSessionSecurity();
    ///
    void setSessionMedia(const bool&);
    ///
    void setSessionQos(const bool&);
    ///
    void setSessionSecurity(const bool&);
	/// method for copying sip headers of any type without knowing which type
	SipHeader* duplicate() const;
	/// compare two headers of (possibly) the same class
	virtual bool compareSipHeader(SipHeader* msg) const;
private:
    bool sessionMedia;
    bool sessionQos;
    bool sessionSecurity;
    
    void decode(const Data& data);
    friend class StatusMsg;
        SipSession(); // Not Implemented
};

 
} // namespace Vocal

/* Local Variables: */
/* c-file-style: "stroustrup" */
/* indent-tabs-mode: nil */
/* c-file-offsets: ((access-label . -) (inclass . ++)) */
/* c-basic-offset: 4 */
/* End: */
#endif
