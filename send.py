import serial
import time
import sys


try:
    ser = serial.Serial("/dev/ttyACM1", 115200)
    print("opened ACM1")
except IOError:
    ser = serial.Serial("/dev/ttyACM0", 115200)
    print("opened ACM0")

ser.flushInput()
ser.timeout = 1

struct = "Hello World"
for c in struct:
    print("sending: %s" % c)
    ser.write(c.encode())
    print("recieved: %s" % ser.read())


ser.close()