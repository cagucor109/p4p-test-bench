import csv
import serial
import sys

arduino_port = "/dev/ttyUSB0"
baud = 9600
spring_k = 1
if len(sys.argv) == 1:
    file_name = "output.csv"    
else:
    file_name = sys.argv[1]

waiting_data = True
waiting_flag = True
starting_flag = True
numeric_data = False

# Clear previous file contents
f = open(file_name, "w+")
f.close()

# Open com port
ser = serial.Serial(arduino_port, baud)
print("Connected to Arduino port:" + arduino_port)
ser.flushInput()

while True:
    ser_bytes = ser.readline().strip()
    ser_line = ser_bytes.decode("utf-8")
    if not ser_line.isalpha() or not ser_line:
        waiting_data = False
        if starting_flag:
            print("Starting to log data ...")
            starting_flag = False
        ser_list = ser_line.split(",")

        try :  
            temp_float = float(ser_list[0])
            numeric_data = True
        except : 
            numeric_data = False

        print(ser_list)
        
        if numeric_data:
            user_in = input("Enter extension reading:   ")
            measurement = float(user_in)
            spring_extension = temp_float - measurement
            # spring extension is in mm so needs to be converted to m
            spring_force = spring_k * spring_extension / 1000
            ser_list.insert(0, spring_force)

        with open(file_name, "a") as f:
            writer = csv.writer(f, delimiter = ",")
            writer.writerow(ser_list)
    else:
        if waiting_flag:
            print("Waiting for data ...")
            waiting_flag = False
            continue
        elif not waiting_data:
            print("Data logging complete, exiting program ...")
            break

