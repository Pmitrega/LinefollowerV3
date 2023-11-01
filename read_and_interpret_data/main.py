import serial
import keyboard
import binascii

logs = []
f = open("data4.txt", "w")
rec_frames = 0
with serial.Serial() as ser:
    ser.baudrate = 115000
    ser.port = 'COM5'
    ser.open()
    print("connected")
    while True:
        data = ser.read()
        if data:
            try:
                f.write(data.hex())
                if(data.hex() == "ff"):
                    rec_frames = rec_frames + 1
                    print(rec_frames)
            except:
                print("wtf")
                pass
        if keyboard.is_pressed('q'):
            f.close()
            break

