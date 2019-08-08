#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

template <class T, int kSize=10>
class Queue{
public:
    Queue()
    :_data(new T[kSize]())
    ,_front(0)
    ,_rear(0)
    {}
    ~Queue();
    bool full() const;
    bool empty() const;
    void push(const T &t);
    void pop();
    T &front() const;
    T &back() const;

private:
    int _front;
    int _rear;
    T *_data;
};

template <class T,int kSize>
Queue<T,kSize>::~Queue(){
    if(_data){
        delete [] _data;
        cout << "~Queue()" << endl;
    }
}

template <class T,int kSize>
bool Queue<T,kSize>::full() const{
    return _front==(_rear+1)%10;
}

template <class T,int kSize>
bool Queue<T,kSize>::empty() const{
    return _front==_rear;
}

template <class T,int kSize>
void Queue<T,kSize>::push(const T &t){
    if(!full()){
        _data[_rear]=t;
        _rear=(_rear+1)%10;
    }
    else{
        cout << "Queue is full,cannot push any more data." << endl;
    }
}

template <class T,int kSize>
void Queue<T,kSize>::pop(){
    if(!empty()){
        _front=(_front+1)%10;
    }
    else{
        cout << "Queue is empty,no more data!" << endl;
    }
}

template<class T,int kSize>
T & Queue<T,kSize>::front()const{
    if(!empty()){
        return _data[_front];
    }
    else{
        cout << "Queue is empty!" << endl;
    }
}

template<class T,int kSize>
T & Queue<T,kSize>::back()const{
    if(!empty()){
        return _data[(_rear-1+10)%10];
    }
    else{
        cout << "Queue is empty!" << endl;
    }
}

void test1(){
    Queue<string> queue;
    cout << "此时队列是否为空？" << queue.empty() << endl;
    queue.push("aaa");
    cout << "此时队列是否为空？" << queue.empty() << endl;

    for(int i=1;i!=11;++i){
        queue.push(string(3,'a'+i));
    }
    cout << "此时队列是否已满？" << queue.full() << endl;
    while(!queue.empty()){
        cout << "队首：" <<queue.front() << endl;
        cout << "队尾：" <<queue.back() << endl;
        queue.pop();
    }
    cout << "此时队列是否为空？" << queue.empty() << endl;
}

int main(){
    test1();
    return 0;
}