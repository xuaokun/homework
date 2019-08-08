
#include <iostream>
#include <fstream>
#include <string>
#include <map>

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::string;
using std::map;

int main(){
    map<string,int> words;
    string filename("The_Holy_Bible.txt");
    ifstream ifs;
    ifs.open(filename);
    string strTmp;
        string wordTmp;
        int flag;
        while(wordTmp.clear(),ifs>>wordTmp){
            flag=0;
            if((wordTmp[0]>='0'&&wordTmp[0]<='9')){
                continue;
            }
            words[wordTmp]++;
        }
    ofstream ofs("词典");
    for(auto & word : words){
        ofs << word.first << " " << word.second << "\n";
    }
    ifs.close();
    ofs.close();
    cout << "ok" <<endl;
    return 0;
}