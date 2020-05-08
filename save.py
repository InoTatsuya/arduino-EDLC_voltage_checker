import serial
import datetime
import os

def write(filename, text):
    file_path = os.path.dirname(filename)
    if not os.path.exists(file_path):
        os.makedirs(file_path)
    with open(filename, 'a') as f:
        f.write(text + "\n")

PORT = "/dev/tty.usbmodem143301"
filepath = "./"
filename = "akiduki.log"

# シリアル接続
ser = serial.Serial(PORT,115200)

while True:
    time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    line = ser.readline().decode("utf-8")
#    line = "\'" + time + "," + line.rstrip() + "\'"
    line = time + "," + line.rstrip()
    write( filepath + filename, line )
    print(line)

