
#include "string.hh"
#include <string.h>

using std::cout;
using std::endl;


String::String()
:_pstr(nullptr)
{
    cout << "String::String()" <<endl;
}
String::String(const char *str)
:_pstr(new char[strlen(str)+1])
{
    strcpy(_pstr,str);
}
String::String(const String &str)
:_pstr(new char[strlen(str._pstr)+1])
{
    strcpy(_pstr,str._pstr);
}
String::~String(){
    if(_pstr){
        delete [] _pstr;
        _pstr=nullptr;
    }
}
String &String::operator=(const String &rhs){
    if(this!=&rhs){
        delete [] _pstr;
        _pstr =new char[strlen(rhs._pstr)+1]();
        strcpy(_pstr,rhs._pstr);
     }
     return *this;
}
String &String::operator=(const char *str){
    delete [] _pstr;
    _pstr=new char[strlen(str)+1]();
    strcpy(_pstr,str);
}
char &String::operator[](std::size_t index){
    if(index<strlen(_pstr) && index>0){
        return _pstr[index];
    }
    else{
        static char nullchar='\0';
        return nullchar;
    }
}
const char &String::operator[](std::size_t index) const{
    return _pstr[index];
}


std::size_t String::size() const{
    return strlen(_pstr);
}
const char* String::c_str() const{
    return _pstr;
}

bool operator==(const String &s1, const String &s2){
    return strcmp(s1._pstr,s2._pstr)==0;
}
bool operator!=(const String &s1, const String &s2){
    return !(s1==s2);
}  

bool operator<(const String &s1, const String &s2){
    return strcmp(s1._pstr,s2._pstr) < 0;
}
bool operator>(const String &s1, const String &s2){
    return strcmp(s1._pstr,s2._pstr) > 0;
}
bool operator<=(const String &s1, const String &s2){
    return strcmp(s1._pstr,s2._pstr) <= 0;
}
bool operator>=(const String &s1, const String &s2){
    return strcmp(s1._pstr,s2._pstr) >= 0;
}

std::ostream &operator<<(std::ostream &os, const String &s){
    return os << s._pstr;
}
std::istream &operator>>(std::istream &is, String &s){
    return is >> s._pstr;
}

String operator+(const String &s1, const String &s2){
    String tmp=s1;
    tmp+=s2;
    return tmp;
}
String operator+(const String &s1, const char *s2){
    return s1 + String(s2);
}
String operator+(const char *s1, const String &s2){
    return String(s1) + s2;
}
