#include <iostream>
using std::cout;
using std::endl;

class complex{
public:
    complex(double dreal,double dimag)
    :_dreal(dreal)
    ,_dimag(dimag)
    {
        cout << "complex(double,double)" << endl;
    }
    void display() const{
        cout << _dreal << " + " << _dimag << "i" <<endl;
    }
    double getReal() const{return _dreal;}
    double getImag() const{return _dimag;}
    void setReal(double real){_dreal=real;}
private:
    double _dreal;
    double _dimag;
};
//运算符重载之普通函数（数据成员是public)
complex operator+(const complex &lhs,const complex &rhs){
    return complex((lhs.getReal()+rhs.getReal()),
    lhs.getImag()+rhs.getImag());
}

int main(){
    complex c1(1,2),c2(3,4);
    complex c3=c1+c2;
    cout << "c33= ";
    c3.display();
    return 0;
}