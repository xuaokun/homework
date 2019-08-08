#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <queue>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::unordered_set;
using std::queue;

class Solution {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        unordered_set<string> visited;//存储路径上已经存在的单词
        visited.insert(beginWord);//将起始单词插入
        unordered_set<string> dict(wordList.begin(),wordList.end());
        queue<string> q;
        q.push(beginWord);
        int res=1;
        while(!q.empty()){
            int size=q.size();
            while(size--){
                string e=q.front();
                q.pop();
                for(int i=0;i<e.size();i++){//遍历单词的每一个字母
                    string word=e;
                    for(char ch='a';ch<'z';ch++){//将该单词替换一个字母后所有可能的单词
                        word[i]=ch;
                        if(word==endWord){
                            return res+1;
                        }
                        if(!dict.count(word) || visited.count(word)) continue;
                        visited.insert(word);
                        q.push(word);
                    }
                }
            }
            res++;
        }
        return 0;
    }
};