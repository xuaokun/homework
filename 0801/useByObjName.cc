//通过对象名访问虚函数时, 调用哪个函数取决于对象名的类型
#include <iostream>
using namespace std;

class base
{
public:
	virtual void disp()
	{
		cout << "hello,base" << endl;
	}
};

class child:public base
{
public:
	void disp()
	{
		cout << "hello,child" << endl;
	}
};

int main()
{
	base obj_base;					//创建基类对象obj_base
	child obj_child;				//创建派生类对象obj_child

	obj_base.disp();				//通过对象名调用虚函数
	obj_child.disp();				//通过对象名调用虚函数

	obj_base = obj_child;
	obj_base.disp();

	obj_child.base::disp();			//通过类名加作用域限定符指明要调用的版本
	obj_child.child::disp();		//通过类名加作用域限定符指明要调用的版本

	return 0;
}
