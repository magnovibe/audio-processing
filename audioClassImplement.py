#class for audio extraction
import librosa
import subprocess
import time
import librosa.display
import numpy as np
import subprocess
import serial
import time
import matplotlib.pyplot as plt
import serial.tools.list_ports 


class Audio:       
      
    beatTempo = 0.0
    beatVolume = []
    beatVolumeString = ""
    beatStep = 0.0
    beatPerSec = 0.0

    # Initialize object
    def __init__(self, audioFile):
        self.audioFile = audioFile
        self.calcTempo()
        self.calcVolume()
        self.calcBPS()

    # Get tempo as float and set to beatTempo
    def calcTempo(self):
        # extract tempo
        y, sr = librosa.load(self.audioFile)

        onset_env = librosa.onset.onset_strength(y=y, sr=sr)
        tempo, beats = librosa.beat.beat_track(y=y, sr=sr)

        #print(f"Tempo: {tempo} BPM")
        self.beatTempo = tempo
        self.beatStep = beats

        
    # Get array for volume and set to beatVolume
    def calcVolume(self):

        # extract tempo
        y, sr = librosa.load(self.audioFile)

        # Set signal frequency (in seconds) as once per beat
        interval_duration = 60/self.beatTempo

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
        
        self.beatVolume = volume_data
        self.beatVolumeString = volume_data_string

    # Get beats per second
    def calcBPS(self):
        self.beatPerSec = self.beatTempo/60

    def magStrengthVol(self, beatVol, index):
        maxVolume = max(beatVol)
        minVolume = min(beatVol)
        magWithVol = (maxVolume - beatVol[index]) * ((255-80) / (maxVolume - minVolume)) + 80
        print("mag strength w/ vol: " + str(magWithVol))

    def magStrengthVolOB(self, beatVolOB, index):
        maxVolume = 120
        minVolume = 50
        magWithVol = (maxVolume - beatVolOB[index]) * ((120-50) / (maxVolume - minVolume)) + 50
        print("mag strength w/ vol: " + str(magWithVol))
        #perhaps make flat 50

    def serialEstablish(self):
        #establish serial connection
        
        ports = serial.tools.list_ports.comports()
        serialInst = serial.Serial()
        portList = []
        count = 0

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
        serialInst.open()
        pause = input()

        #send tempo 
        serialInst.write(str(self.beatTempo).encode('utf-8')) 
        time.delay(5000)
        
        # send volume array size
        serialInst.write(str(len(self.beatVolume)).encode('utf-8'))

        # send volume array
        serialInst.write(str(self.beatVolumeString).encode('utf-8'))
        
        pause1 = input()
        serialInst.close()
