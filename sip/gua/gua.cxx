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
#include <string>
#include "cpLog.h"
#include "UaFacade.hxx"
#include "UaConfiguration.hxx"
#include "UaCommandLine.hxx"
#include <misc.hxx>
#include "gua.hxx"
#include <sstream>

#include <SipUdp_impl.hxx>
#include <SipUdpConnection.hxx>

#ifdef USE_LANFORGE
#include "LFVoipHelper.hxx"
#endif

using namespace Vocal;
using namespace Vocal::UA;

int
main(const int argc, const char**argv)
{
    INIT_DEBUG_MEM_USAGE;
    DEBUG_MEM_USAGE("beginning of main");

    try
    {
        UaCommandLine::instance( (int)argc, (const char**)argv );
        //Set the default log file size to be 3MB
        DEBUG_MEM_USAGE("after argc");
        cpLogSetFileSize (3000000);
        //Set the num of backup files to be 2
        cpLogSetNumOfBackupFiles (2);

        DEBUG_MEM_USAGE("log has been set up");
        Data applName("gua");
        cpLogSetLabel( applName.logData() );

        UaConfiguration::instance(UaCommandLine::instance()->getStringOpt("cfgfile"));
   
        DEBUG_MEM_USAGE("UaConfiguraiton has been set up");
        string lFileName = UaConfiguration::instance().getValue(LogFileNameTag).c_str();
        if(lFileName.length())
        {
            cpLogOpen(lFileName.c_str());
        }
        cpLogSetPriority(cpLogStrToPriority(UaConfiguration::instance().getValue(LogLevelTag).c_str()));

        int localSipPort = 
          atoi(UaConfiguration::instance().getValue(LocalSipPortTag).c_str());

        cpLog(LOG_INFO, "Trying to use port %d for SIP", localSipPort);


        DEBUG_MEM_USAGE("About to init facade");
        cpLog(LOG_ERR, "About to initialize UaFacade...\n");
        UaFacade::initialize(applName, localSipPort, true, true);

#ifdef USE_LANFORGE
        cpLog(LOG_ERR, "About to initialize LANforge thread...\n");
        LFVoipThread* lf_thread = new LFVoipThread(&(UaFacade::instance()), argv, argc);
        UaFacade::instance().setLFThread(lf_thread);
#endif

        DEBUG_MEM_USAGE("About to run facade");

        cpLog(LOG_ERR, "About to 'run' the UaFacade...\n");
        UaFacade::instance().run();
#ifdef USE_LANFORGE
        UaFacade::instance().setLFThread(NULL);
        delete lf_thread;
#endif
    }
    catch(VException& e)
    {
        cpLog(LOG_ERR, "Caught exception, termination application. Reason:%s",
                        e.getDescription().c_str());
        cerr << "Caught exception, termination application. Reason ";
        cerr << e.getDescription().c_str() << endl;
    }

    cpLog(LOG_ERR, "Exiting gua...\n");
    cerr << "Exiting gua..." << endl;
}

void debugMemUsage(const char* msg, const char* file, int line) {
   ostringstream oss;
   oss << file << ":" << line << "  " << msg << endl
       << " SipUdpImpl::_cnt: " << SipUdp_impl::getInstanceCount() << endl
       << " SipUdpConnection::_cnt: " << SipUdpConnection::getInstanceCount() << endl
       << " SipTransceiver::_cnt: " << SipTransceiver::getInstanceCount() << endl;

   ::debugMemUsage(oss.str().c_str(),  "gua_mem.txt");
}//debugMemUsage


