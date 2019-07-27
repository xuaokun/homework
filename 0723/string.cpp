#include <string.h>

#include <string>
#include <iostream>
using std::cout;
using std::endl;
using std::string;

void test0(){
    char str1[]="hello";
    char str2[]="world";

    //str1=0x1000;//数组名是一个常量不能修改
    //++str1;
    const char * pstr="hello,world";
    //*pstr='x';指针指向文字常量区不能修改
    cout << "sizeof(str1)=" << sizeof(str1) <<endl;

    char * ptmp=(char *)malloc(sizeof(str1)+sizeof(str2));
    strcpy(ptmp,str1);
    strcat(ptmp,str2);
    printf("ptmp=%s\n",ptmp);
    printf("strlen(ptmp)=%ld\n",strlen(ptmp));
    free(ptmp);
}
void test1(){
    string s1="hello";
    string s2="world";
    string s3=s1+'x'+"nihao"+s2;
    cout <<"s3=" << s3 <<endl;
    s1.append(s2);
    cout <<"s1=" <<s1 <<endl;
    cout <<"s1,size()=" << s1.size() <<endl;
    cout <<"s1.length()=" << s1.length() <<endl;
    for(size_t idx=0;idx!=s1.size();idx++)
        cout << s1[idx] <<endl;
        size_t pos=s3.find("world");
        string s4=s3.substr(pos,4);
        cout << "s4=" <<s4 <<endl;

        const char *pstr=s4.c_str();
        const char *pstr2=s4.data();
        printf("pstr=%p\n",pstr);
        printf("pstr2=%p\n",pstr2);
        cout << "pstr=" << pstr <<endl;
        cout << "pstr2=" << pstr2 <<endl;
}
int main(){
    test1();
    return 0;
}
