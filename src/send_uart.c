#include "message/message.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/limits.h>

int main(int argc, char* argv[]){
    char path[PATH_MAX];
    if(argc < 2){
        printf("USAGE: %s DEVICEPATH\n", argv[0]);
        printf("No device specified, defaulting to /dev/ttyACM1\n\n");
        strcpy(path, "/dev/ttyACM1");
    } else {
        strcpy(path, argv[1]);
    }


    int uart_fs = -1;
    uart_fs = open(path, O_RDWR | O_NOCTTY | O_NDELAY);
    if(uart_fs == -1) {
        printf("[ERROR] UART open:\n"); 
        printf("%s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    struct termios options;
    tcgetattr(uart_fs, &options);
        options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
        options.c_iflag = IGNPAR;
        options.c_oflag = 0;
        options.c_lflag = 0;
    tcflush(uart_fs, TCIFLUSH);
    tcsetattr(uart_fs, TCSANOW, &options);

    const struct Message msg = {0, 1000, "hello_usb"};
    union Serialized_Message data;
    data.message = msg;

    int tx_len = write(uart_fs, data.buffer, sizeof(data.buffer));
    if(tx_len < 0) {
        printf("[ERROR] UART write\n");
        printf("%s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    int rx_len;
    union Serialized_Message rx_data;

    rx_len = read(uart_fs, rx_data.buffer, sizeof(struct Message));
    if(rx_len < 0){
        printf("[ERROR] UART read\n");
        printf("%s\n", strerror(errno));
        return EXIT_FAILURE;
    } else if(rx_len == 0) {
        printf("[ERROR] No data\n");
    } else{
        printf("Recieved: \n");
        printf("prio: %d\n", rx_data.message.priority);
        printf("time: %d\n", rx_data.message.sleep_in_ms);
        printf("text: %s\n", rx_data.message.text);
    }

    close(uart_fs);
    return EXIT_SUCCESS;
}