#include <iostream>
using std::cout;
using std::endl;

class Point{
public:
    //Point()=default;
    Point(int ix,int iy)
    :_ix(ix)
    ,_iy(iy)
    {
        cout << "Point(int,int)" <<endl;
    }
    Point(const Point &rhs)
    :_ix(rhs._ix)
    ,_iy(rhs._iy)
    {
        cout << this->_ix <<endl;
        cout << "Point(const Point &rhs)" << endl;
    }
    Point & operator=(const Point &rhs){
        cout << "Point & operator=(const Point &rhs)" <<endl;
        this->_ix=rhs._ix;
        this->_iy=rhs._iy;
        return *this;
    }
    void print(){
        cout <<"(" << this->_ix
             <<"," << this->_iy
             << ")" << endl;
    }
    ~Point(){
        cout << "~Point()" << endl;
    }
private:
    int _ix=0;
    int _iy=0;
};