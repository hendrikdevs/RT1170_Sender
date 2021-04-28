send: send.o
	gcc -o send send.o

send.o: src/send.c
	gcc -c src/send.c

.phony: clean
clean: 
	rm -rf *.o