#include "can/message.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

union Serialize {
    struct Message msg;
    char buffer[sizeof(struct Message)];
};

int main(void){
    int uart_fs = -1;
    uart_fs = open("/dev/ttyACM1", O_RDWR | O_NOCTTY | O_NDELAY);
    if(uart_fs == -1) {
        printf("[ERROR] UART open\n");
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

    const struct Message msg = {0, 1000, "hello\0"};
    union Serialize data;
    data.msg = msg;

    int tx_len = write(uart_fs, data.buffer, sizeof(data.buffer));
    if(tx_len < 0) {
        printf("[ERROR] UART write\n");
        return EXIT_FAILURE;
    }

    sleep(1);

    int rx_len;
    union Serialize rx_data;

    rx_len = read(uart_fs, rx_data.buffer, sizeof(struct Message));
    if(rx_len < 0){
        printf("[ERROR] UART read\n");
        printf("Error: %s\n", strerror(errno));
        return EXIT_FAILURE;
    } else if(rx_len == 0) {
        printf("[ERROR] No data\n");
    } else{
        printf("Recieved: \n");
        printf("prio: %d\n", rx_data.msg.priority);
        printf("time: %d\n", rx_data.msg.sleep_in_ms);
        printf("text: %s\n", rx_data.msg.text);
    }

    close(uart_fs);
    return EXIT_SUCCESS;
}