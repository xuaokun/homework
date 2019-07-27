#define MAXSIZE 11
#include <iostream>
using std::cout;
using std::endl;

class Queue{
public:
        Queue()=default;
        bool empty(){
            return f==r;
        }
        bool full(){
            return f=(r+1)%MAXSIZE;
        }
        int front(){
            if(!empty()){
                return queue[f];
            }
            else{
                cout << "queue is empty" << endl;
            }
        }
        int back(){
            if(!full()){
                return queue[(r-1+MAXSIZE)%MAXSIZE];
            }
            else{
                cout << "queue is full" << endl;
            }
        }
        void push(int x){
            if(!full()){
                queue[r]=x;
                r=(r+1)%MAXSIZE;
            }
            else{
                cout <<"queue is full" <<endl;
            }
        }
        void pop(){
             if(!this->empty()){
                queue[f]=0;
                f=(f+1)%MAXSIZE;
            }
            else{
                cout << "queue is empty" <<endl;
            }
        }
        
private:
        int queue[11]={0};
        int f=0;
        int r=0;
};

int main(){
    Queue q;
    q.push(1);
    q.push(2);
    q.push(3);
    int x=q.front();
    cout << "front=" << x << endl;
    x=q.back();
    cout << "back=" << x << endl;
    q.pop();
    x=q.front();
    cout << "front=" << x << endl;
}