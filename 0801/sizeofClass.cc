class A
{
int i; 
void f(){} 
virtual void run(){} 
virtual void run1(){}
virtual void run2(){}
};
class B : virtual public A
{
virtual void run(){} 
virtual void run1(){}
};
// 其中 sizeof(A) = 8;(int i占4个字节，run()是virtual，会有一个指向虚函数表的指针，占4个字节，此时虽然run1和run2也都是virtual但是已经有一个虚函数表，则不再创建。所以为8)
//      sizeof(B) = 12;(继承来的int i占4个字节，虚继承，会创建虚表，占4个字节，run()是virtual，占4个字节，此时虽然run1也是virtual，但是已经存在虚函数表，则不在创建，所以为12) 

class A
{
int i; 
void f(){} 
virtual void run(){} 
virtual void run1(){}
virtual void run2(){}
};
class B : virtual public A
{
virtual void run(){} 
virtual void run1(){}
virtual void run3(){}
};
// 其中 sizeof(A) = 8;(int i占4个字节，run()是virtual，会有一个指向虚函数表的指针，占4个字节，此时虽然run1和run2也都是virtual但是已经有一个虚函数表，则不再创建。所以为8)
//      sizeof(B) = 16;(继承来的int i占4个字节，虚继承，会创建虚表，占4个字节，run()和run1()都是基类已经存在的虚函数，此时覆盖基类虚函数，占4个字节，此时run3是新定义的virtual，则又多了4个字节，所以为16) 
class A
{
int i; 
void f(){} 
virtual void run(){} 
virtual void run1(){}
virtual void run2(){}
};
class B : virtual public A
{
virtual void run(){} 
virtual void run1(){}
};
// 其中 sizeof(A) = 8;(int i占4个字节，run()是virtual，会有一个指向虚函数表的指针，占4个字节，此时虽然run1和run2也都是virtual但是已经有一个虚函数表，则不再创建。所以为8)
//      sizeof(B) = 12;继承来的int i占4个字节，虚继承，会创建虚表，占4个字节，run()是virtual，占4个字节，此时虽然run1也是virtual，但是已经存在虚函数表，则不在创建，所以为12) 

class A
{
int i; 
void f(){} 
virtual void run(){} 
virtual void run1(){}
virtual void run2(){}
};
class B : virtual public A
{
virtual void run(){} 
virtual void run1(){}
virtual void run3(){}
};
// 其中 sizeof(A) = 8;(int i占4个字节，run()是virtual，会有一个指向虚函数表的指针，占4个字节，此时虽然run1和run2也都是virtual但是已经有一个虚函数表，则不再创建。所以为8)
//      sizeof(B) = 16;(继承来的int i占4个字节，虚继承，会创建虚表，占4个字节，run()和run1()都是基类已经存在的虚函数，此时覆盖基类虚函数，占4个字节，此时run3是新定义的virtual，则又多了4个字节，所以为16) 
