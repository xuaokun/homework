#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <sstream>
using std::vector;
using std::set;
using std::map;
using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;
using std::stringstream;

class TextQuery
{
public:
    void readFile(const string filename);
    void query(const string & word);
private:
    vector<string> line;//记录文本的每一行
    map<string, set<int> > word2Line;//记录单词出现的行号
    map<string, int> dict;//记录每个单词出现的次数
};

void TextQuery::readFile(const string filename){
    ifstream ifs;
    stringstream ss;
    ifs.open(filename);
    string tmp;
    string word;
    while(tmp.clear(),getline(ifs,tmp)){
        line.push_back(tmp);
        ss.clear();
        ss << tmp;
        while(word.clear(),!ss.eof()){
            ss >> word;
            if(word[0]>='0'&&word[0]<='9'){
                continue;
            }
            word2Line[word].insert(line.size());//将单词出现的行号插入
            ++dict[word];//将单词出现次数加1
        }
    }
}

void TextQuery::query(const string &word){
    cout << word << " occurs " << dict[word] << " times" << endl;
    for(auto wordLine:word2Line[word]){
        cout << "(line " << wordLine << ")" << line[wordLine-1] << endl;
    } 
}

int main(int argc, char *argv[])
{
    string  queryWord("market");

    TextQuery tq;
    tq.readFile("china_daily.txt");
    tq.query(queryWord);			   
    return 0;
}     
