# audio download from pixabay
# https://librosa.org/doc/0.10.1/generated/librosa.display.waveshow.html#librosa.display.waveshow

import subprocess
import serial
import time
import matplotlib.pyplot as plt
import serial.tools.list_ports 

# including audioClass
from audioClass import Audio

# initialize serial stuff
ports = serial.tools.list_ports.comports()
serialInst = serial.Serial()

portList = []
count = 0

# for getting audio file initialized in code
audio = input("Enter audio file: ")
song = Audio(audio)
tempo = song.beatTempo
vol = song.beatVolume
beats = song.beatStep
# general testing
print("Tempo: " + str(tempo))
print("Volume: " + str(vol))
print("Beats: " + str(beats))
song.magStrengthVol(vol, 2)

for i in range(20):
    song.magStrengthVol(vol, i)


#establish serial connection
for onePort in ports: 
    portList.append(str(onePort))
    count += 1
    print(str(count) + ": " + str(onePort))

val = input("Select port: ")
split_val = portList[int(val) - 1].split()
port_name = split_val[0]
print(port_name) # portList[int(val) - 1])


serialInst.baudrate = 9600 # Needs to match the baud rate in the arduino code
serialInst.port = port_name 
print("about to open")
serialInst.open()
pause = input()
print("port opened") 
serialInst.write(str(tempo).encode('utf-8'))
pause1 = input()
serialInst.close()
