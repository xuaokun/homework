#include <log4cpp/Category.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/PatternLayout.hh>

#include  <iostream>
using std::cout;
using std::endl;
using namespace log4cpp;

int main(void){
    PatternLayout * ptnLayout1=new PatternLayout();
    ptnLayout1->setConversionPattern("%d %c [%p] %m%n");

     PatternLayout * ptnLayout2=new PatternLayout();
    ptnLayout2->setConversionPattern("%d %c [%p] %m%n");

    OstreamAppender *ostreamAppender=new OstreamAppender("ostreamAppender",&cout);
    ostreamAppender->setLayout(ptnLayout1);

    FileAppender *fileAppender=new FileAppender("fileAppender","wangdao.log");
    fileAppender->setLayout(ptnLayout2);

    Category & mycategory=Category::getRoot().getInstance("mycategory");
    mycategory.setPriority(Priority::DEBUG);
    mycategory.addAppender(ostreamAppender);
    mycategory.addAppender(fileAppender);

    mycategory.alert("this is a alert message");
    mycategory.crit("this is a crit message");
    mycategory.error("this is a error message");
    mycategory.warn("this is a warn message");
    mycategory.info("this is a info message");
    mycategory.notice("this is a notice message");

    Category::shutdown();

    return 0;
}