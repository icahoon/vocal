#ifndef SIP_HIDE_HXX_
#define SIP_HIDE_HXX_

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

static const char* const SipHideVersion =
    "$Id: SipHide.hxx,v 1.2 2004/06/16 06:51:25 greear Exp $";

#include "Data.hxx"
#include "symbols.hxx"
#include "cpLog.h"
#include "SipHeader.hxx"
#include "VException.hxx"

namespace Vocal
{

enum SipHideErrorType
{
    DECODE_HIDE_FAILED

    //may need to change this to be more specific
};

/// Exception handling class SipHideParserException
class SipHideParserException : public VException
{
    public:
        SipHideParserException( const string& msg,
                                const string& file,
                                const int line,
                                const int error = 0 );
        SipHideParserException( const string& msg,
                                const string& file,
                                const int line,
                                SipHideErrorType i)
        : VException( msg, file, line, static_cast < int > (i))
        {
            value = i;
        }
        SipHideErrorType getError() const
        {
            return value;
        }
        string getName() const ;
    private:
        SipHideErrorType value;
};

/// data container for Hide header
class SipHide : public SipHeader
{
    public:
        /*** Create by decoding the data string passed in. This is the decode
             or parse.  */
        SipHide( const Data& srcData, const string& local_ip );
        SipHide( const SipHide& src );

        const SipHide& operator =(const SipHide& src);
        ///
        bool operator==(const SipHide& src) const;

        ///
        Data get() const;

        ///
        void set( const Data& newData);
        void scanHide(const Data & scandata);
        /*** return the encoded string version of this. This call should only be
             used inside the stack and is not part of the API */
        Data encode() const;

	/// method for copying sip headers of any type without knowing which type
	Sptr<SipHeader> duplicate() const;
	/// compare two headers of (possibly) the same class
	virtual bool compareSipHeader(SipHeader* msg) const;
    private:
        Data data;

        void decode(const Data& data);
        friend class SipCommand;

        SipHide(); // Not implemented
};

 
} // namespace Vocal

#endif
