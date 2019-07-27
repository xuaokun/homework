#include <log4cpp/Category.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/BasicLayout.hh>
#include <log4cpp/OstreamAppender.hh>

#include <iostream>
using std::cout;
using std::endl;
using namespace log4cpp;

int main(void){
    OstreamAppender * ostreamAppender=new OstreamAppender("ostreamAppender",&cout);
    ostreamAppender->setLayout(new BasicLayout());

    Category &root=Category::getRoot();
    root.setPriority(Priority::CRIT);
    root.addAppender(ostreamAppender);

    root.alert("this is a alert message");
    root.crit("this is a crit message");
    root.error("this is a error message");
    root.warn("this is a warn message");
    root.info("this is a info message");
    root.notice("this is a notice message");


    Category::shutdown();
    return 0;
}