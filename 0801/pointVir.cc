//通过指针访问虚函数
#include <iostream>
using namespace std;

class base
{
public:
	virtual void disp()
	{
		cout << "hello,base" << endl;
	}
	void print()
	{
		cout << "base::print()" << std::endl;
	}
};

class child:public base
{
public:
	void disp()
	{
		cout << "hello,child" << endl;
	}
	void print()
	{
		cout << "child::print()" << std::endl;
	}
};

int main()
{
	base obj_base;			//创建一个基类对象
	base* pBase = &obj_base;	//使用基类对象地址为基类指针赋值
	pBase->disp();			//使用基类指针调用虚函数

	child obj_child;		//创建一派生类对象
	child* pChild = &obj_child;//使用派生类对象地址为派生类指针赋值
	pChild->disp();			//使用派生类指针调用虚函数

	cout << endl;

	pBase = pChild;			//将派生类指针赋值给基类指针
	pBase->disp();			//使用基类指针调用虚函数,”hello,child”

	pChild = (child *)&obj_base;//反向转换，使用基类对象地址为派生类指针赋值
	pChild->disp();		//使用派生类指针调用虚函数，只取决于赋值对象,"hello,base"

	pChild->base::disp();	  //使用类名加作用域限定符指明要调用的版本, 静态联编

	return 0;
}
