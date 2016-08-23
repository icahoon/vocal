#ifndef SipTelSubscriberUser_HXX
#define SipTelSubscriberUser_HXX

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
#include "VException.hxx"
#include "symbols.hxx"
#include "BaseUser.hxx"
    
namespace Vocal
{
    

///
class SipTelSubscriberUserParserException : public VException
{
public:
    ///
    SipTelSubscriberUserParserException( const string& msg,
                               const string& file,
                               const int line, 
			       const int error = 0)
	:VException(msg, file, line, error)
	{
	}
    ///
    string getName() const;  
};

/// pure virtual class for Url types
class SipTelSubscriberUser : public BaseUser
{
public:
    /// Create one with default values
    SipTelSubscriberUser();
    ///
    SipTelSubscriberUser(const Data&) throw(SipTelSubscriberUserParserException&);
    ///
    Sptr<BaseUser> duplicate() const;
    ///parse
    bool  decode(const Data& data);
    ///called by a SipUrl telUser
    Data encode() const;
    ///called by a TelUrl telUser
    Data encodeWithParams() const;
    ///
    bool operator ==(const SipTelSubscriberUser&) const;
    ///
    bool operator < (const SipTelSubscriberUser&) const;
    ///
    bool areEqual(Sptr<BaseUser> baseUser) const;
    ///
    bool isLessThan(Sptr<BaseUser> baseUser) const;
    ///
    UserType getType() const;
    
    ///global phone number sub-component functions.
    ///
    Data getBasePhoneNumber() const;
    ///
    void setBasePhoneNumber(const Data& data);
    ///
    Data getIsdnSubAddress() const;
    ///
    void setIsdnSubAddress(const Data& data);
    ///
    Data getPostDial() const;
    ///
    void setPostDial(const Data& data);
    ///
    Data getPhoneContext() const;
    ///
    void setPhoneContext(const Data& data);
    ///
    Data getServiceProvider() const;
    ///
    void setServiceProvider(const Data& data);
    ///
    Data getFutureExtensionName() const;
    ///
    void setFutureExtensionName(const Data& data);
    ///
    Data getFutureExtensionValue() const;
    ///
    void setFutureExtensionValue(const Data& data);
    ///
    Data getTelParams() const;
    ///call this only if you want along with TelParams.
    Data getGlobalPhoneNumber() const;
    ///
    void setGlobalPhoneNumber(const Data& data);
    ///call this only if you want along with TelParams.
    Data getLocalPhoneNumber() const;
    ///
    void setLocalPhoneNumber(const Data& data);
    ///
    bool getIsGlobalPhoneNumber() const;
private:
    bool isGlobalPhoneNumber;  //if false, this is a localPhoneNumber
    
    mutable bool isGlobalPhoneNumberValid;  //if false, the globalPhoneNumber is not valid.
    //very important to set this flag to false, whenever, the sub-components
    //are being set.
    mutable bool isLocalPhoneNumberValid;
    
    mutable Data globalPhoneNumber;
    mutable Data localPhoneNumber;
    
    //sub-components for globalPhoneNumber/ localPhoneNumber
    Data basePhoneNumber;
    Data isdnSubAddress;
    Data postDial;
    Data phoneContext;
    Data serviceProvider;
    Data futureExtensionName;
    Data futureExtensionValue;
    
    
};

 
} // namespace Vocal

#endif
