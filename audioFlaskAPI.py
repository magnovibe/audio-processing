from flask import Flask, request, jsonify
import librosa
import serial
import serial.tools.list_ports
from flask_cors import CORS, cross_origin
app = Flask(__name__)
import os
CORS(app, resources={r"/*": {"origins": ""}})

app.config['CORS_HEADERS'] = 'Content-Type'

class Audio:
    def __init__(self, audioFile):
        self.beatTempo = 0.0
        self.beatVolume = []
        self.beatVolumeString = ""
        self.bigMagArray = []
        self.bigMagString = ""
        self.littleMagArray = []
        self.littleMagString = ""
        self.beatStep = 0.0
        self.beatPerSec = 0.0
        self.audioFile = audioFile
        self.calcTempo()
        self.calcVolume()
        self.calcBPS()

    def calcTempo(self):
        # extract tempo
        y, sr = librosa.load(self.audioFile)

        onset_env = librosa.onset.onset_strength(y=y, sr=sr)
        tempo, beats = librosa.beat.beat_track(y=y, sr=sr)

        #print(f"Tempo: {tempo} BPM")
        self.beatTempo = tempo
        self.beatStep = beats

    def calcVolume(self):
        # extract tempo
        y, sr = librosa.load(self.audioFile)

        # Set signal frequency (in seconds) as once per beat
        interval_duration = 60/(self.beatTempo*2)

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
        print(self.beatVolume);
        print(self.beatVolumeString);

    def calcBPS(self):
        self.beatPerSec = self.beatTempo/60

    def magStrengthVol(self, volume):
        mean = np.mean(self.beatVolume)
        std = np.std(self.beatVolume)
        lowerBound = mean - (1.5*std)
        upperBound = mean + (1.5*std)
        if (volume < lowerBound):
            magWithVol = 255
        elif (volume > upperBound):
            magWithVol = 130
        else:
            magWithVol = (upperBound - volume) * ((255-130) / (upperBound - lowerBound)) + 130
        print("mag strength w/ vol: " + str(magWithVol))
        return int(magWithVol)

    def magStrengthVolOB(self, volume):
        mean = np.mean(self.beatVolume)
        std = np.std(self.beatVolume)
        lowerBound = mean - (1.5*std)
        upperBound = mean + (1.5*std)
        if (volume < lowerBound):
            magWithVol = 255
        elif (volume > upperBound):
            magWithVol = 130
        else:
            magWithVol = (upperBound - volume) * ((255-190) / (upperBound - lowerBound)) + 190
        print("mag strength w/ vol OB: " + str(magWithVol))
        return int(magWithVol)

    def bigMagStrengthString(self, beatVolume):
        for i in range(len(beatVolume)):
            if i%2 == 0:
                currVolume = beatVolume[i]
                print(currVolume)
                self.bigMagArray.append(self.magStrengthVol(currVolume))
            else:
                self.bigMagArray.append(self.magStrengthVolOB(beatVolume[i]))

        # turn bigMagArray into string
        for i in range(len(self.bigMagArray)):
            self.bigMagString += str(self.bigMagArray[i]) + ";"
        print("BIG MAG STRENGTH")
        print(self.bigMagString)

    def littleMagStrengthString(self, beatVolume):
        # swap 0 and 1, 2 and 3, etc. in bigMagArray and add to littleMagArray
        for i in range(len(beatVolume)):
            if i%2 == 0:
                if (i == len(beatVolume)-1):
                    self.littleMagArray.append(self.bigMagArray[i])
                else:
                    self.littleMagArray.append(self.bigMagArray[i+1])
            else:
                self.littleMagArray.append(self.bigMagArray[i-1])
        # turn littleMagArray into string
        for i in range(len(self.littleMagArray)):
            self.littleMagString += str(self.littleMagArray[i]) + ";"
        print("LITTLE MAG STRENGTH")
        print(self.littleMagString)


@app.route('/analyze-audio', methods=['POST'])
def analyze_audio():
    if 'file' not in request.files:
        return jsonify({"error": "No file part"}), 400
    file = request.files['file']
    if file.filename == '':
        return jsonify({"error": "No selected file"}), 400
    audio_file = file.read()
    # Save the audio file and perform analysis
    # Example: audio = Audio('path/to/your/audio.wav')
    audio = Audio(audio_file)
    return jsonify({
        "tempo": audio.beatTempo,
        "volume": audio.beatVolume,
        "volume_string": audio.beatVolumeString,
        "beats_per_sec": audio.beatPerSec
    }), 200

@app.route('/serial-establish', methods=['POST'])
@cross_origin()
def serial_establish():
    # Establish serial connection and send data to Arduino
    
    data = request.json
    song_index = data.get('index')
    song_url = data.get('url')

    print("called :D")

    current_dir = os.getcwd()
    audio_path = os.path.join(current_dir, song_url[1:]) # Have to get rid of first slash to get absolute path to work
    print("Audio Path:", audio_path)

    try:
        with open(audio_path, 'rb') as audio_file:
            audio_data = audio_file.read()
            # print("Audio", audio_data)
    except FileNotFoundError:
        return jsonify({"error": "Audio file not found"}), 404

    audio = Audio(audio_data)
    ports = serial.tools.list_ports.comports()
    # Further code for serial connection goes here
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
    # pause = input()
    #send tempo, volume array size, volume array delimited by : 
    dataString = str(audio.beatTempo) + ':' + str(len(audio.beatVolume)) + ':' + audio.bigMagString + ':' + audio.littleMagString

    serialInst.write(str(dataString).encode('utf-8'))
    # pause1 = input()
    print('hiiiiii')
    return jsonify({"message": "Serial connection established"}), 200

@app.route('/serial-stop', methods=['POST'])
@cross_origin()
def stop_music():
    serialInst = serial.Serial()
    serialInst.baudrate = 9600
    
    serialInst.write("!".encode('utf-8'))
    return jsonify({"message": "shawty paused"}), 200

app.run(debug=True)