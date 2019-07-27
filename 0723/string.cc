#include <string.h>
#include <iostream>

using std::cout;
using std::endl;


class String
{
public:
	String()//无参数构造函数
    :_pstr(NULL)
    {
        cout << "String()" << endl;
    }
	String(const char *pstr)//字符串参数构造函数
    :_pstr(new char[strlen(pstr)+1]())
    {
        strcpy(_pstr,pstr);
        cout << "String(const char *pstr)" <<endl;
    }
	String(const String & rhs)//拷贝构造函数
    :_pstr(new char[strlen(rhs._pstr)+1]())
    {
        strcpy(_pstr,rhs._pstr);
        cout << "String(const String & rhs)" <<endl;
    }
	String & operator=(const String & rhs){//赋值函数
        if(this!=&rhs){
            delete [] _pstr;
            _pstr=new char[strlen(rhs._pstr)+1]();
            strcpy(_pstr,rhs._pstr);
        }
        cout << "String & operator=(const String & rhs)" <<endl;
        return *this;
    }
	~String(){//析构函数
        if(_pstr){
            delete [] _pstr;
            cout << "~String" <<endl;
        }
    }

	void print(){
        if(_pstr){
            cout << _pstr <<endl;
        }
    }

private:
	char * _pstr;
};
int main(void)
{
	String str1;//无参数的构造函数
	str1.print();//啥都不打印
	
	String str2 = "Hello,world";//等价于str2("Hello,world")，调用带字符串的构造函数
	String str3("wangdao");//带字符串参数的构造函数
    
	str2.print();//打印	Hello,world
	str3.print();//打印 wangdao	
	
	String str4 = str3;//拷贝构造函数
	str4.print();//wangdao
	
	str4 = str2;//
	str4.print();//Hello,world
	
	return 0;
}