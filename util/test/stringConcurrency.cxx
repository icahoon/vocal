
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
static const char* const stringConcurrency_cxx_Version =
    "$Id: stringConcurrency.cxx,v 1.1 2004/05/01 04:15:37 greear Exp $";
#include <string>
#include "VThread.hxx"
#include <stdio.h>

class foo
{
    public:
        static foo* instance();
        static VMutex mutex;
        static const char* print();

    protected:
        foo();
        static foo* instance_;
        int x;
        string logLabel;
};

foo* foo::instance_ = 0;
VMutex foo::mutex;

foo::foo()
{
    x = 0;
}

foo*
foo::instance()
{
    if (!instance_)
    {
        instance_ = new foo;
    }
    return instance_;
}

const char*
foo::print()
{
    LockHelper lock(mutex);

    return foo::instance()->logLabel.c_str();
}



void* thread1(void*)
{
    const char* orig = 0;
    for (int i = 0; i < 1000000; i++)
    {
        const char* x = foo::print();
        if (orig != x)
        {
            orig = x;
            printf("%p\n", orig);
        }
        if (*x != '\0')
        {
            printf("not null!\n");
        }
        if (strlen(x) != 0)
        {
            printf("bad!\n");
        }
    }
    return 0;
}

void* thread2(void*)
{
    const char* orig = 0;
    for (int i = 0; i < 1000000; i++)
    {
        const char* x = foo::print();
        if (orig != x)
        {
            orig = x;
            printf("%p\n", orig);
        }
        if (*x != '\0')
        {
            printf("not null!\n");
        }
        if (strlen(x) != 0)
        {
            printf("bad!\n");
        }
    }
    return 0;
}


int main()
{
    //    setupProfilingStuff();

    string junk;

    VThread my_thread1;
    VThread my_thread2;

    my_thread1.spawn(thread1, 0);
    my_thread2.spawn(thread2, 0);

    for (int i = 0; i < 1000000000; i++);

    my_thread1.join();
    my_thread2.join();

    //    DumpAddressMap();
    //    finish();
    return 0;
}
