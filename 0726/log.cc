#include <log4cpp/Category.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/RollingFileAppender.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/PatternLayout.hh>

#include <iostream>
using std::cout;
using std::endl;
using namespace log4cpp;
//使用单例模式
//还没有实现：输出的日志信息中有文件的名字，函数的名字及其所在的行号
 
class Mylogger
{
public:
    void warn(const char * msg){
        mycategory.warn(msg);
    }
    void error(const char * msg){
        mycategory.error(msg);
    }
    void debug(const char * msg){
        mycategory.debug(msg);
    }
    void info(const char * msg){
        mycategory.info(msg);
    }
    static Mylogger *getInstance(){
        if(_pstr==nullptr){
            _pstr=new Mylogger();
        }
        return _pstr;
    }
    static void destory(){
        if(_pstr){
            delete _pstr;
        }
    }
private:
    Mylogger()
    :ptnLayout1(new PatternLayout())
    ,ptnLayout2(new PatternLayout())
    ,ostreamAppender(new OstreamAppender("ostreamAppender",&cout))
    ,rollingFileAppender(new RollingFileAppender("rollingFileAppender","ak.log",1024,5))
    ,mycategory(Category::getRoot().getInstance("mycategory"))
    {
        ptnLayout1->setConversionPattern("%d [%p] %m%n");
        ptnLayout2->setConversionPattern("%d [%p] %m%n");

        ostreamAppender->setLayout(ptnLayout1);
        rollingFileAppender->setLayout(ptnLayout2);

        mycategory.setPriority(Priority::DEBUG);
        mycategory.addAppender(ostreamAppender);
        mycategory.addAppender(rollingFileAppender);
    }
    ~Mylogger(){
        Category::shutdown();
    };
private:
    static Mylogger *_pstr;
    PatternLayout *ptnLayout1;
    PatternLayout *ptnLayout2;
    OstreamAppender *ostreamAppender;
    RollingFileAppender *rollingFileAppender;
    Category &mycategory;
};
Mylogger *Mylogger::_pstr=nullptr;
Mylogger * log = Mylogger::getInstance();
void LogInfo(const char *msg){
    char information[150]={0};
    sprintf(information,"%s %s %s %d",msg,__FILE__,__FUNCTION__,__LINE__);
    log->info(information);
}
void LogError(const char *msg){
    char information[150]={0};
    sprintf(information,"%s %s %s %d",msg,__FILE__,__FUNCTION__,__LINE__);
    log->error(information);
}
void LogWarn(const char *msg){
    char information[150]={0};
    sprintf(information,"%s %s %s %d",msg,__FILE__,__FUNCTION__,__LINE__);
    log->warn(information);
}
void LogDebug(const char *msg){
    char information[150]={0};
    sprintf(information,"%s %s %s %d",msg,__FILE__,__FUNCTION__,__LINE__);
    log->debug(information);
}


int main(void)
{
    LogInfo("this is a info message");	//或者   
    LogError("this is a error message");
    LogWarn("this is a warn message");
    LogDebug("this is a debug message");
    Mylogger::destory();
    return 0;
}