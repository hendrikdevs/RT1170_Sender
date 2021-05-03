#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include "message/message.h"

int main(void) 
{
    message_t message = 
    {
        .priority = 2,
        .sleep_in_ms = 2000,
        .text = "abcdefghi"
    };

    int s; 
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;

    /* create CAN socket */
    if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
    {
        perror("Socket");
        return 1;
    }

    /* receive interface index */
    strcpy(ifr.ifr_name, "can0");
	ioctl(s, SIOCGIFINDEX, &ifr);

    /* bind CAN socket */
	memset(&addr, 0, sizeof(addr));
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("Bind");
		return 1;
	}

    /* send CAN frame */
	frame.can_id = 0x1;
	frame.can_dlc = 5;
	sprintf(frame.data, "Hello");

	if (write(s, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
		perror("Write");
		return 1;
	}

	if (close(s) < 0) {
		perror("Close");
		return 1;
	}

    return 0;
}