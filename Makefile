INC_DIR=-I"../defines"
DIRS=build
$(shell mkdir -p $(DIRS))

default: main
main: send_uart send_can

send_uart: send_uart.o
	gcc -o build/send_uart build/send_uart.o

send_uart.o: src/send_uart.c
	gcc -Wall $(INC_DIR) -c src/send_uart.c -o build/send_uart.o


send_can: send_can.o
	gcc -o build/send_can build/send_can.o

send_can.o: src/send_can.c
	gcc -Wall $(INC_DIR) -c src/send_can.c -o build/send_can.o


.phony: clean
clean: 
	rm -rf build/