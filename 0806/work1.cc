#include <iostream>
#include <unordered_map>
#include<vector>
#include<string>
#include<map>
#include<fstream>
#include<sstream>
#include<algorithm>
using std::fstream;
using std::ifstream;
using std::ofstream;
using std::sort;
using std::string;
using std::istringstream;
using std::endl;
using std::cout;
using std::vector;
using std::unordered_map;
using std:: map;
using std::string;
map<string,int> Map;

void wordStatic(const char* fileName1,const char * fileName2){
   ifstream ifs(fileName1);
   if(!ifs){
   cout<<"ifs open file error"<<endl;
   return;
   }
   string word;
   int i;
   ofstream ofs(fileName2);
   if(!ofs){
     cout<<"ofs open file error"<<endl;
     return ;
    }
    cout << "ok" << endl;
   while(ifs >> word){
      ++Map[word]; 
   }
    for(auto & m:Map){
       ofs<<m.first<<" "<<m.second<<endl;
    }
   ifs.close();
   ofs.close();
}
int main(){
      wordStatic("The_Holy_Bible.txt","result");
      return 0;
}



