#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
using namespace std;

int main(){
    vector<int> ob;
    istream_iterator<int,char> isi(cin);
    copy(isi,istream_iterator<int,char>(),back_inserter(ob));
    for(vector<int>::iterator it=ob.begin();it!=ob.end();it++){
        cout << (*it) << " ";
    }
    cout << endl;
    return 0;
}