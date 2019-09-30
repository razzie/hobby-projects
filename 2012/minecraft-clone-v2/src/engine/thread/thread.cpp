#include "thread.h"

Thread::Thread() {}

int Thread::Start(void * arg)
{
   Arg(arg); // store user data
   int code = thread_create(Thread::EntryPoint, this, & ThreadId_);
   return code;
}

int Thread::Run(void * arg)
{
   Setup();
   Execute( arg );
}

/*static */
void * Thread::EntryPoint(void * pthis)
{
   Thread * pt = (Thread*)pthis;
   pthis->Run( Arg() );
}

virtual void Thread::Setup()
{
        // Do any setup here
}

virtual void Thread::Execute(void* arg)
{
        // Your code goes here
}

void * Thread::Arg() const
{
    return Arg_;
}

void Thread::Arg(void* a)
{
    Arg_ = a;
}
