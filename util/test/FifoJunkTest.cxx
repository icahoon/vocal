/* ====================================================================
 * The Vovida Software License, Version 1.0 
 * 
 * Copyright (c) 2002 Vovida Networks, Inc.  All rights reserved.
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

static const char* const FifoTimeTest_cxx_Version =
    "$Id: FifoJunkTest.cxx,v 1.1 2004/05/01 04:15:37 greear Exp $";

#include "Fifo.h"
#include "VTime.hxx"
#include "VThread.hxx"

using namespace Vocal;

Fifo < int > shared;

const int iteration_count = 126000;

void* producer(void* my_count)
{
    shared.addDelay(0, 10000);
    shared.add(1);
    return 0;
}

void* consumer(void* my_count)
{
    cout << shared.getNext() << endl;
    cout << shared.getNext() << endl;
}


int runTest(int numProducers, int numConsumers)
{
    VThread producerThreads[1];
    VThread consumerThreads[1];

    for(int i = 0; i < 1; i++)
    {
	consumerThreads[i].spawn(&consumer, (void*)(iteration_count/numConsumers));
    }
    for(int i = 0; i < 1; i++)
    {
	producerThreads[i].spawn(&producer, (void*)(iteration_count/numProducers));
    }

    for(int i = 0; i < 1; i++)
    {
	producerThreads[i].join();
    }

    for(int i = 0; i < 1; i++)
    {
	consumerThreads[i].join();
    }

    return 0;
}


int main()
{
    int delta = runTest(1,1);

    return 0;
}
