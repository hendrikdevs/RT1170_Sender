#include <stdlib.h>
#include <string.h>

#include "can/message.h"

int main(void) 
{
    message_t message = 
    {
        .priority = 10,
        .sleep_in_ms = 2000,
        .text = "abcdefghij"
    };

    serialized_message_t serialized_message = 
    {
        .message = message
    };

    char* command_base = "cansend can0 1#";
    char* command = strcat(command_base, serialized_message.buffer); 
    system(command);

    return EXIT_SUCCESS;
}