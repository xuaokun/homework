#include <func.h>
int  printdir(char *path,int width){
	DIR *dir;
   	dir=opendir(path);
	ERROR_CHECK(dir,NULL,"opendir");
  	struct dirent *p;
	char buf[1024]={0};
	while(p=readdir(dir)){
		if(!strcmp(p->d_name,".")||!strcmp(p->d_name,"..")){
			continue;
		}
		printf("%*s%s\n",width," ",p->d_name);
		sprintf(buf,"%s%s%s",path,"/",p->d_name);
		if(4==p->d_type){
			printdir(buf,width+4);
		}
	}
	closedir(dir);
}
int main (int argc,char *argv[]){
	ARGS_CHECK(argc,2);
	puts(argv[1]);
	printdir(argv[1],4);
	return 0;
}

