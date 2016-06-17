#include <stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
int main()
{
/*char buf[512]="0";
FILE *fp = fopen("/dev/demo", "w+");
if(fp == NULL) {
printf("can't open device!\n");
return 0;
}
fread(buf, sizeof(buf), 1, fp);
fwrite(buf,1,sizeof(buf),fp);
fclose(fp);*/
 int fd,num = 0;

        char buf_write[512] = "0x00000001 0x00000002 0x00000003 0x00000004";
        char buf_read[512]  = "user_read";

        fd=open("/dev/dma",O_RDWR);
        if(fd == -1){
                perror("open");
                return -1;
        }

        //printf("please input to num,num = ");
        //scanf("%d",&num);

        //write(fd,&num,sizeof(int));
        //read(fd,&num,sizeof(int));

        //printf("second read ,num = %d\n",(int)num);

        printf("please input to buf_write,write =");
        //scanf("%s",buf_write);
	
	
        write(fd,buf_write,43);
        read(fd,buf_read,43);
        printf("after write,buf_read = %s\n",buf_read);

        close(fd);


return 0;
}
