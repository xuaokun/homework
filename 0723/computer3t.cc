#include <string.h>
#include <iostream>
using std::cout;
using std::endl;

class Computer{
public:
    Computer(const char *brand,float price)
    :_brand(new char[strlen(brand)+1]())
    ,_price(price)
    {
        strcpy(_brand,brand);
        cout<< "Computer(const char *brand,float price)" <<endl;
    }
    void print(); 
    ~Computer(){
        if(_brand){
            delete [] _brand;
            cout << "~Computer()" << endl;
        }
    }
private:
    char *_brand;
    float _price;
};

inline void Computer::print(){
    cout << "brand:" << _brand <<endl
         << "price:" << _price <<endl;
}
int main (void){
    int number;
    cout << "number=" <<number << endl;

    Computer pc("Thinkpad",8888);
    pc.print();
    Computer pc2("ACER",6666);
    pc2.print();
    return 0;
}
