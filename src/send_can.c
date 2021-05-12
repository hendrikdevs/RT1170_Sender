#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

int main(void) 
{
    int s; 
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;

    int nbytes;

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
	frame.can_dlc = 8;
    frame.data[0] = 1;
    frame.data[1] = 200;
    
    char text[] = "HALLO";
    const size_t data_text_size = MIN(sizeof(frame.data) - sizeof(frame.data[0]) * 2, sizeof(text));

    memcpy(&frame.data[2], text, data_text_size);

	if (write(s, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
		perror("Write");
		return 1;
	}

    printf("CAN Message send!\n");

    /* read CAN frame */
    nbytes = read(s, &frame, sizeof(struct can_frame));

    if (nbytes < 0) {
            perror("can raw socket read");
            return 1;
    }

    /* paranoid check ... */
    if (nbytes < sizeof(struct can_frame)) {
            fprintf(stderr, "read: incomplete CAN frame\n");
            return 1;
    }

    printf("CAN Frame read: %d\n", frame.can_id);

    printf("CAN Frame read: %d\n", frame.data[0]);
    printf("CAN Frame read: %d\n", frame.data[1]);
    printf("CAN Frame read: %x\n", frame.data[2]);
    printf("CAN Frame read: %x\n", frame.data[3]);
    printf("CAN Frame read: %x\n", frame.data[4]);
    printf("CAN Frame read: %x\n", frame.data[5]);
    printf("CAN Frame read: %x\n", frame.data[6]);
    printf("CAN Frame read: %x\n", frame.data[7]);

    /* Close CAN socket */
	if (close(s) < 0) {
		perror("Close");
		return 1;
	}

    return 0;
}