/*
 * @Description: 
 * @Version: 1.0
 * @Autor: chen zhan
 * @Date: 2022-07-19 16:50:31
 * @LastEditors: chen zhan
 * @LastEditTime: 2022-07-20 21:23:14
 */

#include"serial.h"
#include<iostream>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <vector>
using namespace std;

int openPort() 
{
	int fd = -1;
	fd = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1)
	{
		fd = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);
		if (fd == -1)
	{
		perror("Open Serial Port Error!\n");
	}
		return fd;
	}

    struct termios options;
    tcgetattr(fd, &options);
    // 115200, 8N1
    options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    options.c_cc[VTIME] = 0;
    options.c_cc[VMIN] = 1;
    tcflush(fd, TCIOFLUSH);
    tcsetattr(fd, TCSANOW, &options); //应用上面的设置
    return fd;
}
double serial_send(vector<unsigned char> &send_data){
    int fd;
    fd=openPort();
    int send_size=send_data.size()+2;
    unsigned char send_buffer[send_size];
    for (size_t i = 0; i < send_size; i++)
    {
        if(i==0){send_buffer[i]=0x0a;}
        else if(i==send_size-1){send_buffer[i]=0x0d;}
        else{
            send_buffer[i]=send_data[i-1];
        }
    }
    int fd2=write(fd, send_buffer, sizeof(send_buffer));  //Send data
    std::cout<<"STATUS: "<<fd2<<std::endl;
    std::cout<<"finish!"<<std::endl;
    close(fd);
    return 0.0;
}
