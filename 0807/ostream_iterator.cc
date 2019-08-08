#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
using namespace std;

int main(){
    int sz[6]={1,2,3,4,5,6};
    vector<int> ob(sz,sz+6);
    ostream_iterator <int,char> osi(cout," ");//创建输出流迭代器osi
    copy(ob.begin(),ob.end(),osi);
    cout << endl;
    return 0;
}