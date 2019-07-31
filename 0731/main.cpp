#include "rss.h"
#include <iostream>

using std::cout;
using std::endl;
int main(){
    RssReader rssReader;
    rssReader.paraseRss("coolshell.xml");
    rssReader.dump("pagelib.dat");
    return 0;
}