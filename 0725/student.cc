#include <stdlib.h>
#include <string.h>

#include <iostream>
using std::cout;
using std::endl;

 void * operator new(size_t sz){
        cout << "void * operator new(size_t)" << endl;
        void * ret =malloc(sz);
        return ret;
    }
    void operator delete(void *p){
        cout << "void operator delete(void *)" << endl;
        free(p);
    }
class Student{
public:
    Student(int id,const char *name)
    :_id(id)
    ,_name (new char [strlen(name)+1]()){
        cout <<  "Student(int id,const char *name)" << endl;
        strcpy(_name,name);
    }
    ~Student(){
        cout << "~Student()" <<endl;
        if(_name){
            delete [] _name;
            _name = nullptr;
        }
    }

    void print() const{
        cout << "id: "<< _id <<endl
             <<"name: "<< _name << endl;
    }
#if 0
    void * operator new(size_t sz){
        cout << "void * operator new(size_t)" << endl;
        void * ret =malloc(sz);
        return ret;
    }
    void operator delete(void *p){
        cout << "void operator delete(void *)" << endl;
        free(p);
    }
#endif
private:
    int _id;
    char *_name;
};

int main(){
    Student * pstu=new Student(100,"Jackie");
    pstu->print();
    delete pstu;
    return 0;
}