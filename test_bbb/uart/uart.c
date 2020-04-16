//https://class.ece.uw.edu/474/peckol/code/BeagleBone/ForestExamples/Example_6_UART/uart.c

// http://man7.org/linux/man-pages/man3/termios.3.html
// Exploring Beaglebone - Derek Molly for UART pins and example C code
// Use child-parent, by using fork() to transmit and receive between Tiva and BBB. UART1 and UART5

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdint.h>

int main(void)
{
    int fd1, count;
    struct termios options;
    char tx[20] = "UART Tiva";//, rx[20];

    printf("Testing uart implementation with Tiva");

    if ((fd1 = open("/dev/ttyO1", O_RDWR | O_NOCTTY | O_NDELAY)) < 0)
    {
        perror("open\n");
        return -1;
    }

    tcgetattr(fd1, &options);
    // if((cfsetispeed(&options, B9600)) == -1)
    // {
    //     perror("Input baud rate\n");
    //     return -1;
    // }
    // if((cfsetospeed(&options, B9600)) == -1)
    // {
    //     perror("Output baud rate\n");
    //     return -1;
    // } 

    // options.c_cflag |= (CLOCAL | CS8);
    // options.c_iflag &= ~(ISTRIP | IXON | INLCR | PARMRK | ICRNL | IGNBRK);
    // options.c_oflag &= ~(OPOST);
    // options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);

    options.c_cflag = B9600 | CS8 | CREAD | CLOCAL;
  	options.c_iflag = IGNPAR | ICRNL; 

    tcflush(fd1, TCIFLUSH);
    tcsetattr(fd1, TCSANOW, &options);

    while(1)
    {
        printf("Sending char: %s\n", tx);
        if ((count = write(fd1, &tx, 10)) < 0)
        {
            perror("write");
            return -1;
        }
        usleep(100000);
    }

    // fcntl(fd1, F_SETFL, 0);

    // while(1)
    // {

    //     printf("Receive characters\n");

    //     if ((count = read(fd1, (void *)rx, 1)) < 0)
    //     {
    //         perror("read");
    //         return -1;
    //     }

    //     if(count)
    //     {
    //         printf("Received-> '%s'", rx);
    //     }
    // }

    close(fd1);
    return 0;
}