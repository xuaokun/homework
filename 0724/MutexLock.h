#pragma once
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
namespace wd
{
   class MutexLock
   {
   public:
            MutexLock(){
                if(pthread_mutex_init(&_mutex,nullptr)){
                    perror(">> pthread_mute_init");
                }
            }
            ~MutexLock(){
                if(pthread_mutex_destroy(&_mutex)){
                    perror(">> pthread_mutex_lock");
                }
            }
			void lock(){
                if(pthread_mutex_lock(&_mutex)){
                    perror(">> pthread_mutex_lock");
                }
            }
			void unlock(){
                if(pthread_mutex_unlock(&_mutex)){
                    perror(">> pthread_mutex_unlock");
                }
            }
            pthread_mutex_t * getMutexLockPtr(){
                return &_mutex;
            }
            MutexLock(const MutexLock & rhs)=delete;
            MutexLock & operator=(const MutexLock & rhs)=delete;
	 private:
        //MutexLock(const MutexLock & rhs);
        //MutexLock & operator=(const MutexLock & rhs);
	 	pthread_mutex_t _mutex;
   };
   //guard类可以在程序异常返回时也能将互斥锁解开，避免死锁
    class MutexLockGuard{
    public:
        MutexLockGuard(MutexLock & mutex)
        :_mutex(mutex)
        {
            _mutex.lock();
        }
        ~MutexLockGuard(){
            _mutex.unlock();
        }
    private:
        MutexLock & _mutex;
    };
}