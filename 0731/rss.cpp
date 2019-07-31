#include "rss.h"
#include <iostream>

using std::cout;
using std::endl;

RssReader::RssReader()
:_rss()
{
    _rss.reserve(65535);
}
void RssReader::paraseRss(const char *xmlFile){//解析
    XMLDocument doc;
    if(doc.LoadFile(xmlFile)){
         doc.PrintError();
         exit(1);
    }
    //根元素
    XMLElement *rss = doc.RootElement();
    // <channel>元素
	XMLElement* channel = rss->FirstChildElement( "channel" );
    // 遍历子元素
    XMLElement* channelChild = channel->FirstChildElement();
    while (channelChild && strcmp(channelChild->Name(),"item")!=0)
    {
        channelChild = channelChild->NextSiblingElement();
    }
    while(channelChild){//循环进入每一个item元素，查询并记录title、link、description等信息
        RssItem rssitem;
        XMLElement *itemchild=channelChild->FirstChildElement();
        rssitem.title=itemchild->GetText();//title
        itemchild=itemchild->NextSiblingElement();
        rssitem.link=itemchild->GetText();//link
        while(strcmp(itemchild->Name(),"description")!=0){
            itemchild=itemchild=itemchild->NextSiblingElement();
        }
        rssitem.description=itemchild->GetText();//des
        itemchild=itemchild->NextSiblingElement();
        rssitem.content=itemchild->GetText();//content
        _rss.push_back(rssitem);
        channelChild = channelChild->NextSiblingElement();
    }
}
void RssReader::dump(const string & filename){//输出
    XMLDocument doc;
    for(int i=0;i < _rss.size(); i++){
        // 创建根元素<doc>
        XMLElement* root = doc.NewElement( "doc" );
        doc.InsertEndChild( root );
    
        // 创建子元素<docid>
        XMLElement* docid = doc.NewElement( "docid" );
        char num[10]={0};
        sprintf(num,"%d",i+1);//数字转为字符串
        XMLText* docidText = doc.NewText(num);
        docid->InsertEndChild( docidText ); 
        root->InsertEndChild( docid );
    
        // 创建子元素<title>
        XMLElement* title = doc.NewElement( "title" );
        XMLText* titleText = doc.NewText(_rss[i].title.c_str());
        title->InsertEndChild( titleText ); 
        root->InsertEndChild( title );
        // 创建子元素<link>
        XMLElement* link = doc.NewElement( "link" );
        XMLText* linkText = doc.NewText( _rss[i].link.c_str() );
        link->InsertEndChild( linkText ); 
        root->InsertEndChild( link );	
        // 创建子元素<content>
        XMLElement* content = doc.NewElement( "content" );
        content->SetText( _rss[i].content.c_str() ); 
        root->InsertEndChild( content ); 
     }
    // 输出XML至文件
    cout << "output xml to 'pagelib.dat'" << endl;
    doc.SaveFile( "pagelib.dat" );
}   
