# audio download from pixabay
# https://librosa.org/doc/0.10.1/generated/librosa.display.waveshow.html#librosa.display.waveshow

import librosa
import librosa.display
import matplotlib.pyplot as plt

audio_file = "jazztheme.mp3"
y, sr = librosa.load(audio_file)

onset_env = librosa.onset.onset_strength(y=y, sr=sr)
tempo, beats = librosa.beat.beat_track(onset_envelope=onset_env, sr=sr)

print(f"Tempo: {tempo} BPM")

with open('tempo.txt', 'w') as file:
    file.write(str(tempo))

#plt.figure(figsize=(15, 4))
#fig, ax = plt.subplots()
#plt.vlines(librosa.frames_to_time(beats), -1, 1, color='r', alpha=0.7, label='Beats')
#plt.legend()
#plt.show()


#fig, ax = plt.subplots(nrows=3, sharex=True)
#y_harm, y_perc = librosa.effects.hpss(y)
#librosa.display.waveshow(y_harm, sr=sr, alpha=0.5, ax=ax[2], label='Harmonic')
#librosa.display.waveshow(y_perc, sr=sr, color='r', alpha=0.5, ax=ax[2], label='Percussive')
#ax[2].set(title='Multiple waveforms')
#ax[2].legend()

