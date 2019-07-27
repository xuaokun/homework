#include <iostream>
using namespace std;

class Singleton
{
private:
        static Singleton *_pinstance;
        int _x;
        int _y;
        Singleton(int x=0,int y=0){
            this->_x=x;
            this->_y=y;
            cout << "构造函数被调用" << this <<endl;
        }
public:
        static Singleton * getinstance(int x=0,int y=0){
            if(_pinstance==nullptr){
                _pinstance=new Singleton(x,y);
            }
            return _pinstance;
        }
        void display()const{
            cout << "(" <<this->_x << "," <<this->_y << ")" <<endl;
        }
        static void destory(){
            if(_pinstance){
                delete _pinstance;
            }
        }
};
Singleton *Singleton::_pinstance=nullptr;
int main(){
    //Singleton s1;//error 不能让该语句编译通过，因此构造函数被定义为私有
    //Singleton *p1=new Singleton();//error
    Singleton *p1=Singleton::getinstance(3,4);//直接通过类名调用静态成员函数
    Singleton *p2=Singleton::getinstance(2,1);
    Singleton *p3=Singleton::getinstance(6,2);
    cout << p1 <<endl;
    cout << p2 <<endl;
    cout << p3 <<endl;
    p1->display();
    p2->display();
    p3->display();
    //delete p1;//error
    //Singleton::destory();
    return 0;
}

