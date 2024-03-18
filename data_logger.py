import pandas as pd
import serial
import time
import csv

SERIAL_PORT = "COM3"  # ardiuno connected with com port 
BOUD_RATE = 9600      # boud rate same as set in ide

#serial data fetching 
ser = serial.Serial(SERIAL_PORT,BOUD_RATE)

# sensors data in array
x_vals = []
sensor_dht11 = []
sensor_MQ2 = []
sensor_MQ8 = []
sensor_pm2 = []
sensor_MQ135 = []



def read_data_arduino():
    line = ser.readline().decode('utf-8').strip()
    sensor_MQ2.append(int(sensor_MQ2[3]))
    sensor_pm2.append(int(sensor_pm2[1]))

# with open('SensorData.csv', mode='a') as sensor_file:
#     sensor_writer = csv.writer(sensor_file, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
#     sensor_writer.writerow(["Value", "Time"])
    
def on_close(event):
    with open('SensorData.csv','w',newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerows(['lpg','smoke','co','pm2.5'])
        for x, s1, s2 in zip(x_vals, sensor_MQ2, sensor_pm2)
            writer.writerow([x,s1,s2])

# x = serial.Serial(com, baud, timeout=0.1)

# while x.isOpen() is True:
#     data = str(x.readline().decode('utf-8')).rstrip()
#     if data is not '':
#         print(data)
#         with open('SensorData.csv', mode='a') as sensor_file:
#             sensor_writer = csv.writer(sensor_file, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
#             sensor_writer.writerow([int(data), str(time.asctime())])
            
            
# function for data fetchining from serial port 
# function for data inserting to .csv file

            
            