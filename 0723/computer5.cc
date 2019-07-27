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
    Computer(const Computer &rhs)
    :_brand(new char[strlen(rhs._brand)+1]())
    ,_price(rhs._price)
    {
        cout << "Computer(const Computer &)" <<endl;
        strcpy(_brand,rhs._brand);
    }
    Computer & operator=(const Computer &rhs){
        if(this!=&rhs){
            cout << "Computer & operator=(const Computer &rhs)" <<endl;
            delete [] _brand;
            _brand=new char[strlen(rhs._brand)+1]();
            strcpy(_brand,rhs._brand);
            _price=rhs._price;
        }
        return *this;
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
    printf("brand:%p\n",_brand);
    cout << "brand:" << _brand <<endl
         << "price:" << _price <<endl;
}
void test0(){
   Computer *pc=new Computer("MateBook",7500);
   pc->print();
   delete pc;
}
void test1(){
    Computer pc("Thinkpad",8888);
    cout << "pc=";
    pc.print();
    Computer pc2=pc;
    cout << "pc2=";
    pc2.print();
}
Computer getComputer(){
    Computer pc3("xiaomi",7500);
    cout << "pc3=";
    pc3.print();
    return pc3;
}
void test2(){
    getComputer();
    int number =1;
    &number;
    const int &ref=1;
    cout << "ref= " << ref << endl;
    Computer pc4=getComputer();
    cout<<"pc4=";
    pc4.print();
}
void test3(){
    Computer pc1("Thinkpad",8888);
    Computer pc2("Xiaomi",6666);
    pc2=pc1;
    cout << "赋值后" <<endl;
    cout << "pc1=";
    pc1.print();
    cout << "pc2=" ;
    pc2.print();
}
int main (void){
    test3();
    return 0;
}