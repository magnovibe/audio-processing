from flask import Flask, request, jsonify
import librosa
import serial
import serial.tools.list_ports
from flask_cors import CORS, cross_origin
app = Flask(__name__)
import os
CORS(app, resources={r"/*": {"origins": "*"}})

app.config['CORS_HEADERS'] = 'Content-Type'

class Audio:
    def __init__(self, audioFile):
        self.audioFile = audioFile
        self.beatTempo = 0.0
        self.beatVolume = []
        self.beatVolumeString = ""
        self.beatStep = 0.0
        self.beatPerSec = 0.0
        self.calcTempo()
        self.calcVolume()
        self.calcBPS()

    def calcTempo(self):
        y, sr = librosa.load(self.audioFile)
        onset_env = librosa.onset.onset_strength(y=y, sr=sr)
        tempo, beats = librosa.beat.beat_track(y=y, sr=sr)
        self.beatTempo = tempo
        self.beatStep = beats

    def calcVolume(self):
        y, sr = librosa.load(self.audioFile)
        interval_duration = 60/self.beatTempo
        total_frames = len(y)
        interval_frames = int(interval_duration * sr)
        volume_data = []
        volume_data_string = ""
        for i in range(0, total_frames, interval_frames):
            interval = y[i:i+interval_frames]
            rms = librosa.feature.rms(y=interval)
            volume_data.append(rms[0][0])
            volume_data_string += f'{rms[0][0]:f}'
            volume_data_string += "; "
        self.beatVolume = volume_data
        self.beatVolumeString = volume_data_string

    def calcBPS(self):
        self.beatPerSec = self.beatTempo/60

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
            print("Audio", audio_data)
    except FileNotFoundError:
        return jsonify({"error": "Audio file not found"}), 404

    audio = Audio(audio_data)
    ports = serial.tools.list_ports.comports()
    # Further code for serial connection goes here
    return jsonify({"message": "Serial connection established"}), 200

app.run(debug=True)