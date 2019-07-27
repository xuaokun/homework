#include <iostream>
using std::cout;
using std::endl;

class Stack{
public:
        Stack(int num)
        :_stack(new int[num]())
        ,_size(0)
        {}
       bool empty(){
           if(0==_size){
               return 1;
           }
           return 0;
       }
       bool full(){
           if(10==_size){
               return 1;
           }
            return 0;
       }
       int top(){
           if(_size){
             return this->_stack[this->_size-1];  
           }
           return 0;
       } 
       void push(int x){
           if(_size!=10){
               this->_stack[this->_size]=x;
               this->_size++;
           }
           else{
               cout << "栈溢出" <<endl;
           }
       }
       void pop(){
           if(_size){
               this->_stack[_size-1]=0;
               this->_size--;
           }
       }
private:
        int *_stack;
        int _size;
};

int main(){
    Stack s(10);
    s.push(10);
    s.push(12);
    s.push(14);
    int x=s.top();
    cout << x << endl;
    s.pop();
    x=s.top();
    cout << x << endl;
    return 0;
}