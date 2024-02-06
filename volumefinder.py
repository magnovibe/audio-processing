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

# Set signal frequency (in seconds) as once per beat
interval_duration = 60/tempo

# Calculate the total number of frames in the audio
total_frames = len(y)

# Calculate the number of frames corresponding to the specified interval
interval_frames = int(interval_duration * sr)

# Initialize an array to store the volume values
volume_data = []

# Intialize a string to store the volume values in order to send through serial
volume_data_string = ""

# Iterate through the audio in intervals and calculate RMS for each interval
for i in range(0, total_frames, interval_frames):
    # Extract the current interval
    interval = y[i:i+interval_frames]

    # Calculate RMS for the interval
    rms = librosa.feature.rms(y=interval)

    # Append the RMS value to the volume_data array
    volume_data.append(rms[0][0])

    # Concatenate the volume data string with RMS value and add "; " delimiter
    volume_data_string += f'{rms[0][0]:f}'
    volume_data_string += "; "

# Print or use the volume_data array as needed
for volume in volume_data:
    print(volume)

# Print the volume_data string
print(volume_data_string)