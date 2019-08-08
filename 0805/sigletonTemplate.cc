#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

template <class T>
class Singleton{    
public: 
    template <typename ...Args>
    static T *getInstance(Args... args);
    static void destory();
private:
    Singleton();
    ~Singleton();
    static T *_pstr;
};

template <class T>
T * Singleton<T>::_pstr=nullptr;

template <class T>
template <typename ...Args>
T *Singleton<T>::getInstance(Args... args){
    if(nullptr==_pstr){
        ::atexit(destory);
        _pstr=new T(args...);
    }
    return _pstr;
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
    Point(double x,double y)
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
    Computer * p1 = Singleton<Computer>::getInstance("Xiaomi", 6666);
    Computer * p2 = Singleton<Computer>::getInstance("Mac", 8888);

    Point * p3 = Singleton<Point>::getInstance(1, 2);
    Point * p4 = Singleton<Point>::getInstance(3, 4);

    return 0;
} 