
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::string;
using std::stringstream;
using std::map;


int main(){
    map<string,int> words;
    string filename("The_Holy_Bible.txt");
    ifstream ifs;
    stringstream ss;
    ifs.open(filename);
    string strTmp;
    while(strTmp.clear(),std::getline(ifs,strTmp)){
        ss << strTmp;
        int flag;
        string wordTmp;
        while(!ss.eof()){
            wordTmp.clear();
            ss >> wordTmp;
            if((wordTmp[0]>='0'&&wordTmp[0]<='9')||wordTmp[0]==' '){
                ss.clear();
                continue;
            }
            words[wordTmp]++;
        }
        ss.clear();
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