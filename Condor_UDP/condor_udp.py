import serial
import socket
import sys
import time

port = 55278
soc = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
# s.settimeout(10)
soc.bind(("", port))
print("waiting for data on port:", port)

ser = serial.Serial('COM4', baudrate=115200)
time.sleep(2)


keywords = ["time", "airspeed", "altitude", "vario=", "gforce"]
data_dict = dict()


while True:
    try:
        # Read från UDP port
        udp_string, addr = soc.recvfrom(1024)
        data_string = udp_string.decode('utf-8') # ascii?

        # Parse data
        data_array = data_string.split("\n")
        for data in data_array:
            if any(keyword in data for keyword in keywords):
                tmp = data.split('=')
                data_dict[tmp[0]] = float(tmp[1])

        # Convert units
        data_dict['airspeed'] = int(data_dict['airspeed'] * 3.6 *10)
        data_dict['vario'] = int(data_dict['vario'] * 10)
        data_dict['altitude'] = int(data_dict['altitude'])

        # Print to prompt
        # for item in data_dict.items():
        #     print(f'{item[0]}: {float(item[1]):4.2f}', end=", ")
        # print("")

        print(f'{data_dict["airspeed"]} {data_dict["vario"]} {data_dict["altitude"]}')


        # Send to Arduino
        # needle = str(int(data_dict['airspeed'])).encode('utf-8')
        needle = f'{data_dict["airspeed"]} {data_dict["vario"]} {data_dict["altitude"]}'.encode('utf-8')
        ser.write(needle)


    except KeyboardInterrupt:
        print("Ctrl + C\nClosing serial connection and exit")
        ser.close()
        sys.exit()
