#include <log4cpp/Category.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/RollingFileAppender.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/PatternLayout.hh>

#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::string;
using namespace log4cpp;
//使用单例模式
 
class Mylogger
{
public:
    template<typename... Args>
    void warn(Args... args){
        mycategory.warn(args...);
    }
    template<typename... Args>
    void error(Args... args){
        mycategory.error(args...);
    }
    template<typename... Args>
    void debug(Args... args){
        mycategory.debug(args...);
    }
    template<typename... Args>
    void info(Args... args){
        mycategory.info(args...);
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
#define LogInfo(msg,...) log->info(string("[").append(__FILE__)\
    .append(":").append(__FUNCTION__).append(":").append(std::to_string(__LINE__)).append("]").append(msg).c_str(),##__VA_ARGS__)
#define LogError(msg,...) log->error(string("[").append(__FILE__)\
    .append(":").append(__FUNCTION__).append(":").append(std::to_string(__LINE__)).append("]").append(msg).c_str(),##__VA_ARGS__)
#define LogWarn(msg,...) log->warn(string("[").append(__FILE__)\
    .append(":").append(__FUNCTION__).append(":").append(std::to_string(__LINE__)).append("]").append(msg).c_str(),##__VA_ARGS__)
#define LogDebug(msg,...) log->debug(string("[").append(__FILE__)\
    .append(":").append(__FUNCTION__).append(":").append(std::to_string(__LINE__)).append("]").append(msg).c_str(),##__VA_ARGS__)

void test() {
	int number = 100;
	const char * pstr = "hello, log4cpp";
	LogWarn("this is an warn message. number = %d, str = %s\n", number, pstr);
    LogInfo("this is an info message. number = %d, str = %s\n", number, pstr);
    LogError("this is an error message. number = %d, str = %s\n", number, pstr);
    LogDebug("this is an debug message. number = %d, str = %s\n", number, pstr);
}
	
int main(void)
{   
    test();
    Mylogger::destory();
    return 0;
}