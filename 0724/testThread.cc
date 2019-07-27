#include "MutexLock.h"
#include "Condition.h"

#include <iostream>
using std::cout;
using std::endl;

int ticket=200;
bool flag = true;
wd::MutexLock gmutex;
wd::Condition gcondition(gmutex);

void * windows1(void * arg){//flag为true时进行卖票
    int cnt=20;
    while(cnt--){
        wd::MutexLockGuard autolock(gmutex);
        while(!flag){
            gcondition.wait();
        }
        --ticket;
        cout << ">> windows1 " << pthread_self()
             << ":left ticket = " << ticket <<endl;
        if(flag){
            flag=false;
        }
        gcondition.notify();
    }
    return nullptr;
}
void * windows2(void *arg){//windows2当flag为false时卖票
    int cnt=20;
    while(cnt--){
        wd::MutexLockGuard autolock(gmutex);
        while(flag){
            gcondition.wait();
        }
        --ticket;
        cout << ">> windows2 " << pthread_self()
             << ":left ticket = " << ticket <<endl;
        if(!flag){
            flag=true;
        }
        gcondition.notify();
    }
    return nullptr;
}
int main(){
    pthread_t pthid1,pthid2;
    pthread_create(&pthid1,nullptr,windows1,nullptr);
    pthread_create(&pthid2,nullptr,windows2,nullptr);


    pthread_join(pthid1,nullptr);
    pthread_join(pthid2,nullptr);

    return 0;
}