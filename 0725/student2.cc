#include <stdlib.h>
#include <string.h>

#include <iostream>
using std::cout;
using std::endl;

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
private:
    void * operator new(size_t sz){
        cout << "void * operator new(size_t)" <<endl;
    }
    void operator delete(void *p){
        cout << "void operator delete(void *p)" <<endl;
    }
private:
    int _id;
    char *_name;
};

int main(){
    Student s1(101,"caixukun");
    //Student *pstr=new Student(100,"jackie"); //error
    s1.print();
    return 0;
}