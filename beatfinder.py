# audio download from pixabay
# https://librosa.org/doc/0.10.1/generated/librosa.display.waveshow.html#librosa.display.waveshow

import librosa
import subprocess
import serial
import time
import librosa.display
import matplotlib.pyplot as plt

audio_file = "jazztheme.mp3"
y, sr = librosa.load(audio_file)

onset_env = librosa.onset.onset_strength(y=y, sr=sr)
tempo, beats = librosa.beat.beat_track(onset_envelope=onset_env, sr=sr)

print(f"Tempo: {tempo} BPM")

#preprocTemp = subprocess.check_output(['python3', 'beatfinder.py']).decode('utf-8').strip()
myData = "HI ARDUINO!"
ser = serial.Serial('/dev/cu.usbmodem*',9600) # needs to be changed to COM3 on windows
ser.write(myData.encode('utf-8'))

ser.close()
