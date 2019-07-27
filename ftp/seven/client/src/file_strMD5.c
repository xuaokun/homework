#include "client.h"
#define READ_DATA_SIZE 1000
// int main(){
// 	char md5_str[36];
// 	computerStringMD5("ak123456",md5_str);
// 	return 0;
// }
int computerFileMD5(char *filepath, char *md5_str){
    int fileFd,i;
    int ret=0;
    char data[READ_DATA_SIZE];
    //char md5_str[36];
    unsigned char md5_value[16];
    MD5_CTX md5;
    MD5_Init(&md5);
    fileFd=open(filepath,O_RDONLY);
	ERROR_CHECK(ret,-1,"open");
    while(1){
		ret = read(fileFd, data,READ_DATA_SIZE);
		if (-1 == ret)
		{
			perror("read");
			close(fileFd);
			return -1;
		}

		MD5_Update(&md5, data, ret);

		if (0 == ret || ret < READ_DATA_SIZE)
		{
			break;
		}
	} 
    close(fileFd);
    MD5_Final( md5_value,&md5);
  
	// convert md5 value to md5 string
	for(i = 0; i < 16; i++)
	{
		snprintf(md5_str + i*2, 2+1, "%02x", md5_value[i]);
	}
	return 0;
}
int computerStringMD5(char *str, char *md5_str){
    int i;
    int len=strlen(str);
    unsigned char md5_value[16];
    MD5_CTX md5;
    MD5_Init(&md5);
	MD5_Update(&md5, str,len);
    MD5_Final( md5_value,&md5);
  
	// convert md5 value to md5 string
	for(i = 0; i < 16; i++)
	{
		snprintf(md5_str + i*2, 2+1, "%02x", md5_value[i]);
	}
	return 0;
}