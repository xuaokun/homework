#pragma once
   
#include "MutexLock.h"

#include <pthread.h>

namespace wd{
   class Condition
   {
   public:
            Condition(MutexLock & mutex)
            :_mutex(mutex)
            {
                if(pthread_cond_init(&_cond,NULL)){
                    perror(">> pthread_cond_init");
                }
            }    
            Condition(const Condition & rhs)=delete;
            Condition & operator=(const Condition & rhs)=delete;

            ~Condition(){
                if(pthread_cond_destroy(&_cond)){
                    perror(">> pthread_cond_destory");
                }
            }
			void wait(){
                if(pthread_cond_wait(&_cond,_mutex.getMutexLockPtr())){
                    perror(">> pthread_cond_wait");
                }
            }
			void notify(){
                if(pthread_cond_signal(&_cond)){
                    perror(">> pthread_cond_signal");
                }
            }
			void notifyall(){
                if(pthread_cond_broadcast(&_cond)){
                    perror(">> pthread_cond_boadcast");
                }
            }
	 private:
	    MutexLock &_mutex;
        pthread_cond_t _cond;
   };
}