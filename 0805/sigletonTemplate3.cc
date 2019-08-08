#include <pthread.h>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

template <class T>
class Singleton{    
public: 
    static T *getInstance();
    static void init();
    static void destory();
private:
    Singleton();
    ~Singleton();
    static T *_pstr;
    static pthread_once_t _once;
};

template <class T>
pthread_once_t Singleton<T>::_once = PTHREAD_ONCE_INIT;

template <class T>
T * Singleton<T>::_pstr=nullptr;

template <class T>
T *Singleton<T>::getInstance(){
   pthread_once(&_once,init);
   return _pstr;
}
template <class T>
void Singleton<T>::init()
{
	_pstr = new T();
	atexit(destory);
}

template <class T>
void Singleton<T>::destory(){
    if(_pstr){
        delete _pstr;
        cout << "destory()" << endl;
    }
}

 class Point{
public:
    Point(double x=0,double y=0)
    :_x(x)
    ,_y(y)
    {
        cout << "(" << _x 
        <<"," << _y <<")" << endl;
    }
private:
    double _x;
    double _y;
};

class Computer{
public:
    Computer(string branch,double price)
    :_branch(branch)
    ,_price(price)
    {
        cout << "branch:" << _branch << endl
             <<"price:" << _price << endl;
    }
private:
    string _branch;
    double _price;
};
int main(void)
{
    Point * p3 = Singleton<Point>::getInstance();
    Point * p4 = Singleton<Point>::getInstance();

    return 0;
} 