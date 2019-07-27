#include <func.h>
#include <sched.h>
int main()
{
    int pid=getpid(); 
    printf("the process nice value:%d\n",getpriority(PRIO_PROCESS,pid));//查看当前进程的优先级
    setpriority(PRIO_PROCESS,pid,15);
    printf("the process nice value:%d\n",getpriority(PRIO_PROCESS,0));//查看当前进程的优先级
    printf("the process schduling policy:%d\n",sched_getscheduler(pid));//当前进程的调度策略
    printf("the max rt pri value of the policy:%d\n",sched_get_priority_max(0));//指定policy的最大的rt priority
    printf("the min rt pri value of the policy:%d\n",sched_get_priority_min(0));//指定policy的最小的rt priority
    struct sched_param param;
    bzero(&param,sizeof(param));
    param.sched_priority=0;
    int ret=sched_setparam(pid,&param);
    ERROR_CHECK(ret,-1,"sched_setparam");
    bzero(&param,sizeof(param));
    ret=sched_getparam(pid,&param);
    ERROR_CHECK(ret,-1,"sched_getparam");
    printf("the set priority is:%d\n",param.sched_priority);
    while(1);
    return 0;
}

