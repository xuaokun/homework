#include <math.h>//sqrt函数使用

#include <iostream>
using std::cout;
using std::endl;
class Point;//
class Line{
public:
    float distance(const Point &lhs,const Point &rhs);
    void setPoint(Point & pt,int ix, int iy);
    friend Point;
    void print(){
        cout << _data <<endl;
    }
private:
    int _data;
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
    void setLine(Line &line,int data){
        line._data=data;
    }
    friend Line;
private:
    int _ix;
    int _iy;
};
float Line::distance(const Point &lhs,const Point &rhs){
    return sqrt((lhs._ix-rhs._ix)*(lhs._ix-rhs._ix)+(lhs._iy-rhs._iy)*(lhs._iy-rhs._iy));
}
void Line::setPoint(Point &pt,int ix,int iy){
    pt._ix = ix;
    pt._iy = iy;
}
int main(){
    Point pt1(10,11);
    Point pt2(21,22);
    pt1.print();
    cout << "--->";
    pt2.print();
    Line line;
    cout << "的距离是：" << line.distance(pt1,pt2) << endl;
    line.setPoint(pt2,31,32);
    cout << "pt2=";
    pt2.print();
    cout << endl;
    pt2.setLine(line,88);
    cout << "_data:" ;
    line.print();
    return 0;
}

