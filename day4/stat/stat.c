#include <func.h>

int main(int argc,char *argv[]){
	ARGS_CHECK(argc,2);
	int ret;
	DIR *dir=opendir(argv[1]);
	struct dirent *p;
	struct stat buf={0};  
	while((p=readdir(dir))!=NULL){
        if(strcmp(p->d_name,".")==0 ||strcmp(p->d_name,"..")==0){
            continue;
        }
		ret=stat(p->d_name,&buf);
		ERROR_CHECK(ret,-1,"stat");
		char *time;
        time=ctime(&buf.st_mtime);
        time[16]=0;
        int mode=buf.st_mode;
        int pow[10]={0};
        for(int i=8;i>=0;i--){
            pow[i]=mode & 1;
            mode=mode>>1;
        }
        char power[5]="-rwx";
        int mod[9]={0};
        for(int i=0;i<9;i++){
            if(pow[i]==1){
                mod[i]=1;
            }
            else mod[i]=0;
            if(pow[i+1]==1){
                mod[i+1]=2;
            }
            else mod[i+1]=0;
            if(pow[i+2]==1){
                mod[i+2]=3;
            }
            else mod[i+2]=0;  
            i=i+2;
        }
        mode=mode>>3;
        int type=mode&15;
        if(4==type){
            printf("d");
        }
        else printf("-");
        for(int i=0;i<9;i++){
            printf("%c",power[mod[i]]);
        }
	    printf(" %ld %s %s %5ld %s %s\n",buf.st_nlink,getpwuid(buf.st_uid)->pw_name,getgrgid(buf.st_gid)->gr_name,buf.st_size,time+4,p->d_name);
	}
}
