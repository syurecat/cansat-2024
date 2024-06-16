import serial
 
ser = serial.Serial('COM3',19200,timeout=None)

while True:
    line = ser.readline()
    print(line)
ser.close()