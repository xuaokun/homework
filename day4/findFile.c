#include <func.h>
int  printdir(char *path,char *fName){
	DIR *dir;
   	dir=opendir(path);
  	if(NULL==dir){
		return -1;
	}
	struct dirent *p;
	char buf[1024]={0};
	while(p=readdir(dir)){
		if(!strcmp(p->d_name,".")||!strcmp(p->d_name,"..")){
			continue;
		}
		if(strcmp(p->d_name,fName)==0){
			sprintf(buf,"%s%s%s",path,"/",p->d_name);
			printf("%s\n",buf);
		}
		if(4==p->d_type){
			printdir(buf,fName);
		}
	}
	closedir(dir);
}
int main (int argc,char *argv[]){
	ARGS_CHECK(argc,3);
	printdir(argv[1],argv[2]);
	return 0;
}
