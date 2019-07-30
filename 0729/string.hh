#include <iostream>
class String {
public:
	String();//无参数构造函数
	String(const char *);//c风格字符串作参数的复制构造函数
	String(const String&);//String类型参数的复制构造函数
	~String();//析构函数
	String &operator=(const String &);//赋值函数，参数为String
	String &operator=(const char *);//赋值函数,参数为c风格字符串

	String &operator+=(const String &);//参数为String的+=运算符重载函数
	String &operator+=(const char *);//参数为C风格字符串的+=运算符重载函数

	char &operator[](std::size_t index);//下标运算符重载
	const char &operator[](std::size_t index) const;//const类型下标运算符重载

	std::size_t size() const;//字符串大小
	const char* c_str() const;//存储的字符串

	friend bool operator==(const String &, const String &);//==运算符友元函数重载
	friend bool operator!=(const String &, const String &);//！=运算符友元函数重载

	friend bool operator<(const String &, const String &);//<运算符重载
	friend bool operator>(const String &, const String &);//>运算符重载
	friend bool operator<=(const String &, const String &);//<=运算符重载
	friend bool operator>=(const String &, const String &);//>=运算符重载

	friend std::ostream &operator<<(std::ostream &os, const String &s);//<<输出运算符重载
	friend std::istream &operator>>(std::istream &is, String &s);//>>输入运算符重载

private:
	char * _pstr;
};

String operator+(const String &, const String &);//+运算符重载（两个参数为string）
String operator+(const String &, const char *);//+运算符重载（一个sting,一个c风格字符串）
String operator+(const char *, const String &);//+运算符重载（一个c风格字符串，一个string）

