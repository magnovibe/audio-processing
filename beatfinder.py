# audio download from pixabay
# https://librosa.org/doc/0.10.1/generated/librosa.display.waveshow.html#librosa.display.waveshow

import librosa
import subprocess
import serial
import time
import librosa.display
import matplotlib.pyplot as plt
import serial.tools.list_ports 
# initialize serial stuff
ports = serial.tools.list_ports.comports()
serialInst = serial.Serial()

portList = []
count = 0

# extract tempo
audio_file = "jazztheme.mp3"
y, sr = librosa.load(audio_file)

onset_env = librosa.onset.onset_strength(y=y, sr=sr)
tempo, beats = librosa.beat.beat_track(y=y, sr=sr)

print(f"Tempo: {tempo} BPM")

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
print("port opened") 
serialInst.write(str(tempo).encode('utf-8'))
serialInst.close()
