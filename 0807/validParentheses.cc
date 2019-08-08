#include <iostream>
#include <string>
#include <stack>
using std::cout;
using std::endl;
using std::string;
using std::stack;

class Solution {
public:
    bool isValid(string s){
        stack<char> ss;
        if(s=="") return true;
        if(s.size()%2!=0) return false;
        for(auto c:s){
            if(c=='(' || c=='[' || c=='{'){
                ss.push(c);
            }
            else{
                if(ss.size()==0 && (c==')' || c==']' || c=='}')){
                    return false;
                }
                else if(c==')' && ss.top()!='(' || c==']' && ss.top()!='[' || c=='}' && ss.top()!='{'){
                    return false;
                }
                else{
                    ss.pop();
                }
            }
        }
        if(ss.size()!=0) return false;
        return true;
    }
};
