#include <string.h>
#include <string>
#include <vector>
#include "tinyxml2.h"

using std::string;
using std::vector;
using namespace tinyxml2;

struct RssItem{
    string title;//标题
    string link;//链接
    string description;//描述
    string content;//内容
};

class RssReader{
public:
    RssReader();
    void paraseRss(const char *xmlFile);//解析
    void dump(const string & filename);//输出
private:
    vector<RssItem> _rss;
};