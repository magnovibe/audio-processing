# audio download from pixabay

import librosa
import librosa.display
import matplotlib.pyplot as plt

audio_file = "jazztheme.mp3"
y, sr = librosa.load(audio_file)

onset_env = librosa.onset.onset_strength(y=y, sr=sr)
tempo, beats = librosa.beat.beat_track(onset_envelope=onset_env, sr=sr)

print(f"Tempo: {tempo} BPM")
# plt.figure(figsize=(15, 4))
# librosa.display.waveshow(y, sr=sr, alpha=0.5)
# plt.vlines(librosa.frames_to_time(beats), -1, 1, color='r', alpha=0.7, label='Beats')
# plt.legend()
# plt.show()

