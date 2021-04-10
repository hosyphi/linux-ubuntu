#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define DEVICE_NODE "/dev/driver_module_device"

#define MEM_SIZE 1024

char write_buf[MEM_SIZE];
char read_buf[MEM_SIZE];
int key, i ;

int main()
{
	int fd;
	char opt;
	printf("**************************************************\n");
	printf("*******user application to test char driver*******\n");
        
        fd = open(DEVICE_NODE, O_RDWR);
        
        if(fd < 0)
        {
        	printf("Cannot open device file ... \n");
        	return 0;
        }
        
        
        while(1)
        {
        	printf("\n*********Please Enter the Option*******\n");
        	printf("*        1. Nhap vao mot xau.		*\n");
        	printf("*        2. Encrypt Caesar and save.	*\n");
        	printf("*        3. Read and decrypt Caesar.	*\n"); 
        	printf("*        4. Exit.			*\n");
        	printf("***************************************\n");
        	
        	scanf("%c", &opt);
        	printf("\nYou Option: %c\n", opt);
        	
        	switch(opt)
        	{
        		case '1':
        			*write_buf = 1;
        			printf("\nNhap xau ro: ");
        			scanf("%s", write_buf + 1);
        			write(fd, write_buf, strlen(write_buf) + 1);
        			break;
        		case '2':
        			*write_buf = 2;
				printf("\nNhap key de ma hoa: ");
				scanf(" %s", write_buf + 1);
				write(fd, write_buf, strlen(write_buf)+1);
				read(fd, read_buf, MEM_SIZE);
				printf("\nXau sau khi ma hoa ---->> %s\n",read_buf);
        			break;
        		case '3':
        			*write_buf = 3;
        			printf("\nNhap key de giai ma: ");
        			scanf("%s", write_buf + 1);
        			write(fd, write_buf, strlen(write_buf) + 1);
        			read(fd, read_buf, MEM_SIZE);
        			printf("\nXau sau khi giai ma ---->> %s\n", read_buf);
        			break;
        		case '4':
        			close(fd);
        			exit(1);
        			break;
        		default:
        			printf("Enter Valid option = %c\n",opt);
     				break;
        	}
        }
       
	return 0;
}
