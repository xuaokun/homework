#include <iostream>
using std::cout;
using std::endl;

class Example{
public:
    //函数调用运算符
    int operator()(int x, int y){
        ++_count;
        return x + y;
    }
    int operator()(int x, int y, int z){
        return x * y * z;
    }
    int callTime() const{
        return _count;
    }
private:
    int _count=0;
};
int main(){
    Example e1;
    int a = 3,b = 4,c = 5;
    cout << "e1(a,b)= " << e1(a, b) << endl;
    cout << "e1(a,b,c)=" << e1(a,b,c) << endl;
    cout << "e1(22,33)=" << e1(22,33) << endl;
    cout << "e1.calltime()=" << e1.callTime() <<endl;
    return 0;
}