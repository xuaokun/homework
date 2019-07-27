#include <string.h>
#include <iostream>
using std::cout;
using std::endl;

class Computer{
public:
    void setBrand(const char *brand);
    void setPrice(float price);
    void print(); 
private:
    char _brand[20];
    float _price;
};

inline void Computer::setBrand(const char * brand){
    strcpy(_brand,brand);
}
inline void Computer::setPrice(float price){
    _price=price;
}
inline void Computer::print(){
    cout << "brand:" << _brand <<endl
         << "price:" << _price <<endl;
}
int main (void){
    int number;
    cout << "number=" <<number << endl;

    Computer pc;
    pc.setBrand("Thinkpad");
    pc.setPrice(8888);
    pc.print();
    Computer pc2;
    pc2.setBrand("ACER");
    pc2.setPrice(6666);
    pc2.print();
    return 0;
}
