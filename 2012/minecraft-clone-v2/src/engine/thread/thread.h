#ifndef _THREAD_H_
#define _THREAD_H_

#include <windows.h>

class Thread {
   public:
      Thread();
      int Start(void * arg);

   protected:
      int Run(void * arg);
      static void * EntryPoint(void*);
      virtual void Setup();
      virtual void Execute(void*);
      void * Arg() const;
      void Arg(void* a);

   private:
      THREADID ThreadId_;
      void * Arg_;
};

#endif
