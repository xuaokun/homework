#include <iostream>
#include <list>
#include <map>
#include <unordered_map>
using std::endl;
using std::cout;
using std::list;
using std::map;
using std::unordered_map;

class LRUCache {
public:
    LRUCache(int capacity)
    :_capacity(capacity)
    ,_size(0)
    {}
    int get(int key) {
        if(!map.count(key))
        {
            cout << "未找到" << endl;
            return -1;
        }
        int tmp=map[key];
        map.erase(key);
        map.insert({key,tmp});
        cout << tmp << endl;
        return tmp;
    } 
    void put(int key, int value) {
        if(map.count(key)){//元素已经存在
            map.erase(key);
            map.insert({key,value});
        }else{
            if(_size<_capacity){//未满
                map.insert({key,value});
            }
            else{//已满
                 auto it=map.begin();
                for(int i=1;i<_capacity;i++){
                    it++;
                }
                cout << it->first << "," << it->second << "被删除" << endl;
                map.erase(it);
                map.insert({key,value});
            }
            _size++;
        }
    }
private:
     int _capacity;
     unordered_map<int,int> map;//按最久未使用的顺序排列中的元素
     int _size;
};

int main(){
    LRUCache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    cache.get(1);       // 返回  1
    cache.put(3, 3);    // 该操作会使得密钥 2 作废
    cache.get(2);       // 返回 -1 (未找到)
    cache.put(4, 4);    // 该操作会使得密钥 1 作废
    cache.get(1);       // 返回 -1 (未找到)
    cache.get(3);       // 返回  3
    cache.get(4);       // 返回  4
    return 0;
}
