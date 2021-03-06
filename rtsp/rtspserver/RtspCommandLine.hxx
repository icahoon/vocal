#ifndef RtspCommandLine_hxx
#define RtspCommandLine_hxx
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
 * IN EXCESS OF 281421,000, NOR FOR ANY INDIRECT, INCIDENTAL, SPECIAL,
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

static const char* const RtspCommandLine_hxx_version =
    "$Id: RtspCommandLine.hxx,v 1.1 2004/05/01 04:15:23 greear Exp $";


#include <map>
#include <string>


static const char* const appUsage = "[-d] [-v[<log-level>]] [-f<config-file>]";


/** Parses and syntax checks the command line arguments and allows other 
    classes to access those values.<p>
    
    Default usage: <code>[-d] [-v[log-level]] [-f[config-file]]</code><p>
    Valid access values and their default values:
    <ul>
    <li>CfgFile (mediaServer.cfg)
    <li>LogLevel* (LOG_WARNING)
    <li>Daemon (false)
    </ul>
    <i>LogLevel</i>: <CODE>LOG_ERR, LOG_DEBUG, LOG_DEBUG_STACK, LOG_DEBUG_HB</CODE>
 */
class RtspCommandLine
{
    public:
        /** constructor */
        RtspCommandLine( const int argc,
                         const char** argv,
                         const char* applicationUsage = appUsage
                       );

        /** destructor */
        ~RtspCommandLine();

        /** get a string argument based on a name */
        const string& getString( const string& cmdLineOption );

        /** get a int argument based on a name */
        const int getInt( const string& cmdLineOption );

        /** get a bool argument based on a name */
        const bool getBool( const string& cmdLineOption );

    private:
        /** load in default argument values */
        void setDefaultValues();

        /** parse command argments
            @param argc number of arguments to parse
            @param argv string of arguments
            @param applicationUsage string to print if parsing fails
         */
        void parseCommandLine( const int argc,
                               const char** argv,
                               const char* applicationUsage );

        /** internal structure of one command line option */
        static const pair<const char*, const char*> cmdLineOptionString[];

        /** Command Line options structure */
        typedef map<string, string> Table;
        /** collection of command line option */
        Table cmdLineOptionTable;

    protected:
        /** suppress copy constructor */
        RtspCommandLine( const RtspCommandLine& );
        /** suppress assignment operator */
        RtspCommandLine& operator=( const RtspCommandLine& );
};

/* Local Variables: */
/* c-file-style: "stroustrup" */
/* indent-tabs-mode: nil */
/* c-file-offsets: ((access-label . -) (inclass . ++)) */
/* c-basic-offset: 4 */
/* End: */
#endif

