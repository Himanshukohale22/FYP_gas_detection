import pandas as pd
import serial
import time
import csv


Readings = 1
Data_list = [0]*Readings

# import /  
# Open serial port
ser = serial.Serial('/dev/ttyACM0', 9600)  # Replace 'COM3' with your Arduino's serial port

def getReadings():
    ser.write(b'g')
    VAAYU_data = ser.readline().decode().strip(',')

    return VAAYU_data

while (1):
    for i in range (0,Readings):
        Data = getReadings()
        Data_list[i] = Data

    print(Data_list)


with open('data2.csv', 'w', newline='') as csvfile:

    writer = csv.writer(csvfile)
    
    # Write header
    writer.writerow(["Temperature","Humidity","Heat Index","LPG","CO","Smoke","CO2","Toleune","NH4","Acetone","Dust density","AQI"])

    writer.writerow(Data_list)





#     # Read serial data and write to CSV
#     while True:
#             def getReadings():
#                 ser.write(b'g')
#                 VAAYU_data = ser.readline().decode().strip(',\n')

#                 return VAAYU_data

#             for i in range (0,Readings):
#             Data = getReadings()
#             Data_list[i] = Data

#             print(Data_list)
# #           # Write to CSV
#             writer.writerow([T,])
            
# #             # Optionally, print the data to console
#             print(value)
        
# #         except KeyboardInterrupt:
# #             # Close serial port and CSV file on KeyboardInterrupt
# #             ser.close()
# #             break

# # # Close serial port
# # ser.close()

# # while 1:
# #     arduinodata = ser.readline()
# #     print(arduinodata);