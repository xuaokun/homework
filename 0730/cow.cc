//实现写时复制的cowstring
//当对字符串进行赋值时，并不马上真正进行复制（只会将指针地址复制），只有当字符串内容要进行修改时，才进行深拷贝。

#include <string.h>

#include <iostream>
using std::cout;
using std::endl;

class CowString{
public:
    CowString()//无参数构造函数
    :_pstr(new char[4+1]() + 4)//_pstr指向字符串首地址
    {
        initrefCount();
        cout << "CowString()" <<endl;
    }
    CowString(const char *pstr)//c风格字符串为参数的构造函数
    :_pstr(new char[strlen(pstr)+5]()+4)//+1+4
    {
        initrefCount();
        strcpy(_pstr,pstr);
        cout << "CowString(const char *)" << endl;
    }
    CowString(const CowString & rhs)//拷贝构造函数
    :_pstr(rhs._pstr)
    {
        initrefCount();
        cout << "CowString(const CowString &rhs)" << endl;
        increaserefCount();
    }
    CowString & operator=(const CowString &rhs){//赋值构造函数
        cout << "CowString & operator=(const CowString &)" <<endl;
        if(this != &rhs){
            release();
            _pstr=rhs._pstr;
            increaserefCount();
        }
        return *this;
    }
    const char *c_str()const{//求字符串函数
        return _pstr;
    }
    int size() const{
        return strlen(_pstr);
    }
    ~CowString(){//析构函数：当销毁对象时应该减引用值
        release();
    }
    int refCount()const {
        return *(int *)(_pstr - 4);
    }
    char &operator[](int index);//下标访问函数
    const char &operator[](int index)const{
        cout << "const char &CowString::operator[](int index)const" << endl;
        return _pstr[index];
    }
    friend std::ostream & operator<<(std::ostream & os, const CowString & rhs);
private:
    void initrefCount(){
        *(int *)(_pstr - 4)=1;//将前4个字节初始化为int类型的1
    }
    void increaserefCount(){
        ++*(int *)(_pstr - 4) ;//*和++优先级同，从右至左结合
    }
    void decreaserefCount(){
        --*(int *)(_pstr - 4) ;//*和++优先级同，从右至左结合
    }
    void release(){
        decreaserefCount();
        if(0==refCount()){
            delete [] (_pstr-4);
            cout << "delete heap data" <<endl;
        }
    }
    char *_pstr;
};
char &CowString::operator[](int index){
    if(index >= 0 && index < size()){
        if(refCount()>1){
            decreaserefCount();
            char *ptmp=new char[size()+5]()+4;
            strcpy(ptmp,_pstr);
            _pstr=ptmp;
            initrefCount();
        }
        return _pstr[index];
    }
    else{
        static char nullchar='\0';
        return nullchar;
    }
}
std::ostream & operator<<(std::ostream & os, const CowString & rhs){
    os << rhs._pstr;
    return os;
}

 int main(void)
{
	CowString s1 = "hello,world";

	CowString s2 = "hello,world";
	s2 = s1;

	CowString s3 = "shenzhen";
	cout << "s3 = " << s3 << endl;
	s3 = s1;
 
	cout << "执行赋值操作之后:" << endl;
	cout << "s1 = " << s1 << endl;
	cout << "s2 = " << s2 << endl;
	cout << "s3 = " << s3 << endl;
	printf("s1's address is %p\n", s1.c_str());
	printf("s2's address is %p\n", s2.c_str());
	printf("s3's address is %p\n", s3.c_str());
	cout << "s1's refCount = " << s1.refCount() << endl;
	cout << "s2's refCount = " << s2.refCount() << endl;
	cout << "s3's refCount = " << s3.refCount() << endl;

	// cout << "\n 修改共享字符串的内容之后:" << endl;
	// s3[0] = 'X';
	// cout << "s1 = " << s1 << endl;
	// cout << "s2 = " << s2 << endl;
	// cout << "s3 = " << s3 << endl;
	// printf("s1's address is %p\n", s1.c_str());
	// printf("s2's address is %p\n", s2.c_str());
	// printf("s3's address is %p\n", s3.c_str());
	// cout << "s1's refCount = " << s1.refCount() << endl;
	// cout << "s2's refCount = " << s2.refCount() << endl;
	// cout << "s3's refCount = " << s3.refCount() << endl;

	cout << "\n 执行读操作之后:" << endl;
	cout << "s1[0] = " << s1[0] << endl;
	cout << "s1 = " << s1 << endl;
	cout << "s2 = " << s2 << endl;
	cout << "s3 = " << s3 << endl;
	printf("s1's address is %p\n", s1.c_str());
	printf("s2's address is %p\n", s2.c_str());
	printf("s3's address is %p\n", s3.c_str());
	cout << "s1's refCount = " << s1.refCount() << endl;
	cout << "s2's refCount = " << s2.refCount() << endl;
	cout << "s3's refCount = " << s3.refCount() << endl;
	return 0;
}