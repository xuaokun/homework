#include <iostream>
using std::cout;
using std::endl;

struct X{
    short & _data;
};

int main(void){
    cout << "sizeof(X)=" <<sizeof(X) <<endl;
    return 0;
}