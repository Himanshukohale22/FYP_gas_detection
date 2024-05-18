
import csv
import serial
import pandas as pd
import time

ser = serial.Serial('/dev/ttyACM0', 9600,timeout=2)  # Replace 'COM3' with your Arduino's serial port

Readings = 1
Data_list = [0]*Readings

def getReadings():
    ser.write(b'g')
    VAAYU_data = ser.readline().decode().strip(',\n')

    return VAAYU_data

while (1):
    for i in range (0,Readings):
        Data = getReadings()
        Data_list[i] = Data

    print(Data_list)



