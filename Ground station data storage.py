import serial
import serial.tools.list_ports
comgood = 0
file = open('LoRa.txt', "a")
ports = list(serial.tools.list_ports.comports())
for p in ports:
    if 'CH340' in p.description:
        print (p[1] + ' <--Probably receiver')
    else:
        print(p)
while comgood == 0:

    try:
        port = input('Type the COM port: ')
        COMport = 'COM' + str(port)
        ser = serial.Serial(COMport, 9600)
        comgood = 1
    except:
        print("Wrong port. Insert another port.")
ser.close()
ser.open()
while True:
    data = ser.readline()
    input = data.decode()
    file = open('LoRa.txt', "a")
    file.write(input)
    file.close()
    print(input)