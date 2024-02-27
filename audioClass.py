#class for audio extraction
import librosa
import subprocess
import time
import librosa.display
import matplotlib.pyplot as plt
import numpy as np

class audio:   
      
    beatTempo = 0.0
    beatVolume = []
    beatVolumeString = ""

    # Initialize object
    def __init__(self, audioFile):
        self.audioFile = audioFile

    # Get tempo as float and set to beatTempo
    def calcTempo(self):
        # extract tempo
        y, sr = librosa.load(self.audioFile)

        onset_env = librosa.onset.onset_strength(y=y, sr=sr)
        tempo, beats = librosa.beat.beat_track(y=y, sr=sr)

        print(f"Tempo: {tempo} BPM")
        self.beatTempo = tempo

        
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


    # Getter for mp3 tempo
    def getTempo(self):
        return self.beatTempo
    

    # Getter for volume data
    def getVolumeData(self):
        return self.beatVolume
    
    
    # Getter for volume data string
    def getVolumeString(self):
        return self.beatVolumeString


        