/* 1、建立堆

　　2、得到堆顶元素为最大元素

　　3、去掉堆顶，将堆最后一个元素放到堆顶，此时可通过一次调整重新使堆有序。

　　4、堆顶元素为第二大元素。

　　5、重复步骤3，直到堆变空 */
#include <iostream>
#include <string.h>
using std::cout;
using std::endl;

template <typename T, typename Compare = std::less<T> >
class HeapSort
{
public:
    HeapSort(T* arr,int arrlen)
    :_arrlen(arrlen)
    ,_arr(arr)
    {}
    void heapAdjust(int adjPos,int len);
    void sort();
private:
    T *_arr;
    int _arrlen;
};

template <typename T, typename Compare>
void HeapSort<T,Compare>::heapAdjust(int adjPos,int len) {
	int dad = adjPos;
	int son = 2 * dad + 1;
	while (son < len) {
		if (son + 1 < len && _arr[son] < _arr[son + 1]) {
			son++;
		}
		if (_arr[dad] < _arr[son]) {
			T tmp=_arr[dad];
            _arr[dad]=_arr[son];
            _arr[son]=tmp;
			dad = son;
			son = 2 * dad + 1;
		}
		else {
			break;
		}
	}
}
template <typename T, typename Compare >
void HeapSort<T,Compare>::sort() {
	for (int i = _arrlen / 2 - 1; i >= 0;i--) {
		heapAdjust( i,_arrlen);
	}
	T Ttmp=_arr[0];
    _arr[0]=_arr[_arrlen-1]; 
    _arr[_arrlen-1]=Ttmp;
	for (int i = _arrlen - 1; i > 1; i--) {
		heapAdjust( 0,i);
		T Ttmp=_arr[0];
        _arr[0]=_arr[i-1]; 
        _arr[i-1]=Ttmp;                                                                                                                                                                                                                                                                                                                                                                                                       
	}
}

void test(){
    int arr[6]={2,1,3,4,7,5};
    HeapSort<int> h1(arr,6);
    h1.sort();
    for(int i=0;i<6;i++){
        cout << arr[i] << " "; 
    }
    cout << endl;
}
int main(){
    test();
    return 0;
}