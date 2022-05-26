import serial
import csv

PuertoSerial = serial.Serial(port = '/tmp/ttyS1') #fuente: https://maker.pro/pic/tutorial/introduction-to-python-serial-ports

f= open("data.csv",'w') 
writer = csv.writer(f) #fuente: https://www.pythontutorial.net/python-basics/python-write-csv-file/
print("Connected")

while(1):
    	input = PuertoSerial.readline().decode().split(' ')
    	writer.writerow(input)
f.close()
