import serial
# import Leap, sys, serial
import time
# from Leap import CircleGesture, KeyTapGesture, ScreenTapGesture, SwipeGesture

# ser = serial.Serial('/dev/tty.usbserial-A9003ZlA', 9600, timeout=1, bytesize=8, parity='N')
# print ser


# time.sleep(1)
ser = serial.Serial('/dev/tty.usbserial-A9003ZlA',9600)
print ser
time.sleep(2)
ser.write('255,255,0\n')
time.sleep(.5)
ser.write('240,255,50\n')
time.sleep(.5)
ser.write('200,255,100\n')
time.sleep(.5)
ser.write('150,255,150\n')
time.sleep(.5)
ser.write('100,255,255\n')











# ser = serial.Serial('/dev/tty.usbserial-A9003ZlA', 9600)

# ser.open()
# sio = io.TextIOWrapper(io.BufferedRWPair(ser, ser))
# sio.write(unicode("255,255,255\n"))
# while 1:
#      print ser.readline()
# time.sleep(2)
# ser.write(str(","))
# ser.write(str("\n"))
# ser.write('255,255,255\n')
# ser.write(unicode('\n'))
# ser.write("255,255,255")
# ser.write(unicode("255,255,255\n"))
# print "sleeping"
# time.sleep(1)
# ser.write(unicode("255,255,255\n"))
# print "sleeping"
# time.sleep(1)
# 
# ser.write(unicode("255,255,255\n"))
# print "sleeping"
# time.sleep(1)
# ser.write(unicode("255,255,255\n"))
# ser.write(unicode("255,255,255\n"))
# ser.write(unicode("255,255,255\n"))
# ser.write(unicode("255,255,255\n"))
# ser.write(unicode("255,0,255\n"))
# time.sleep(1)
# ser.write(("255,0,255"))
# ser.flush()
# ser.write(("0,0,255").encode('ascii'))


# print ser.readline()
# print ser.readline()
# print ser.readline()
# print ser.readline()
# ser.close()
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
