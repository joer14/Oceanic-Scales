import Leap, sys, serial
import io, time
from Leap import CircleGesture, KeyTapGesture, ScreenTapGesture, SwipeGesture

# ser = serial.Serial('/dev/tty.usbserial-A9003ZlA', 9600, timeout=1, bytesize=8, parity='N')
ser = serial.Serial('/dev/tty.usbserial-A9003ZlA', 9600)

# ser.open()
# sio = io.TextIOWrapper(io.BufferedRWPair(ser, ser))
# sio.write(unicode("255,255,255\n"))
# while 1:
#      print ser.readline()
time.sleep(2)
# ser.write(str(","))
# ser.write(str("\n"))
# print ser.readline()
# print ser.readline()
# print ser.readline()
# print ser.readline()
# print ser.readline()
# print ser.readline()
# print ser.readline()
# 
# ser.write('255,255,255\n')
# ser.write(unicode('\n'))
# ser.write("255,255,255")
ser.write(unicode("255,255,255"))
# ser.write("255,255,255")
# ser.write("255,255,255")
# ser.write("255,255,255")
# print ser.readline()
# print ser.readline()
# print ser.readline()
# print ser.readline()
# print ser.readline()
# print ser.readline()
# print ser.readline()
