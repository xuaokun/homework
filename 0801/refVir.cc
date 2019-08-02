//通过引用访问虚函数
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
	base obj_base;				//创建基类对象
	base& rBase1 = obj_base;	//声明基类引用，用基类对象初始化
	rBase1.disp();				//基类引用 调用虚函数: 基类中的disp版本

	child obj_child;			//创建派生类对象
	base& rBase2 = obj_child;	//声明基类引用，用派生类对象初始化
	rBase2.disp();				//基类引用 调用虚函数: 派生类中的disp版本

	cout << endl;

	//引用一经声明后，其调用的函数就不会再改变
	rBase1 = obj_child; //引用本身不可以改变, 这里是将obj_child赋值给
		        //了rBase1指向的obj_base，相当于obj_base = obj_child;
	rBase1.disp();      //还是调用原来的虚函数: 基类版本

	rBase2 = obj_base;  //同上
	rBase2.disp();      //还是调用原来的虚函数: 派生类版本

	return 0;
}
