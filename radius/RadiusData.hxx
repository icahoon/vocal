#ifndef RadiusData_hxx
#define RadiusData_hxx

/* ====================================================================
 * The Vovida Software License, Version 1.0 
 * 
 * Copyright (c) 2001 Vovida Networks, Inc.  All rights reserved.
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

static const char* const RadiusData_hxx_Version =
    "$Id: RadiusData.hxx,v 1.1 2004/05/01 04:15:22 greear Exp $";

#include <cassert>
#include "VRadiusException.hxx"
#include "radius.h"


/** Raw RADIUS data
     This is a simple but limited implementation with a static size
 */
class RadiusData
{
public:
    ///
    RadiusData( ) : myLength( 0 )
        {
            memset( &myData, 0, RadiusMaxPacketSize );
        }

    ///
    RadiusData( const void *val, u_int16_t len ) :
        myLength( len > RadiusMaxPacketSize ? RadiusMaxPacketSize : len )
        {
            assert( val );
            memcpy( &myData, val, myLength );
        }
    
    ///
    RadiusData( const RadiusData& rhs )
        {
            copyRhsToThis( rhs );
        }

    ///
    RadiusData& operator=( const RadiusData& rhs )
        {
            if( this != &rhs )
            {
                copyRhsToThis( rhs );
            }
            return *this;
        }

    ///
    void copyRhsToThis( const RadiusData& rhs )
        {
            myLength = rhs.myLength;
            memcpy( &myData, rhs.myData, myLength);
        }

    ///
    RadiusData& operator+=( const RadiusData& rhs )
        {
            assert( this != &rhs );
            append( &rhs.myData, rhs.myLength );
            return *this;
        }

    ///
    void append( const void *val, const int len )
        throw ( VRadiusException& )
        {
            assert( val );

            if( (myLength + len) > RadiusMaxPacketSize )
            {
                throw VRadiusException( "Buffer overflow", __FILE__, __LINE__ );

            }
            memcpy( myData + myLength, val, len );
            myLength += len;
        }
    
    ///
    ~RadiusData() {}
    
    ///
    const u_int8_t* data() const { return myData; }
    
    ///
    u_int16_t length() const { return myLength; }
    
private:
    ///
    u_int16_t   myLength;
    ///
    u_int8_t    myData[ RadiusMaxPacketSize ];
};

// Local Variables:
// mode:c++
// c-file-style:"bsd"
// c-basic-offset:4
// c-file-offsets:((inclass . ++))
// indent-tabs-mode:nil
// End:

#endif
