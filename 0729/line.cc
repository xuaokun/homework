#include <math.h>//sqrt函数使用

#include <iostream>
using std::cout;
using std::endl;
class Point;//
class Line{
public:
    float distance(const Point &lhs,const Point &rhs);
};
class Point{
public:
    Point(int ix=0,int iy=0)
    :_ix(ix)
    ,_iy(iy)
    {
        cout << "Point(int,int)" << endl;
    }
    void print() const{
        cout << "(" << _ix
             << "," << _iy
             << ")";
    }
    friend float Line::distance(const Point &lhs,const Point &rhs);
private:
    int _ix;
    int _iy;
};
float Line::distance(const Point &lhs,const Point &rhs){
    return sqrt((lhs._ix-rhs._ix)*(lhs._ix-rhs._ix)+(lhs._iy-rhs._iy)*(lhs._iy-rhs._iy));
}
int main(){
    Point pt1(10,11);
    Point pt2(21,22);
    pt1.print();
    cout << "--->";
    pt2.print();
    cout << "的距离是：" << Line().distance(pt1,pt2) << endl;
    return 0;
}

