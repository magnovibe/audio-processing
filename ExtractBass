import librosa
import librosa.display
import matplotlib.pyplot as plt

audio_file = "your_audio_file.wav"  # Replace with the path to your audio file
y, sr = librosa.load(audio_file)

# Compute the STFT of the audio
D = librosa.stft(y)

# Extract the magnitude spectrum
mag = np.abs(D)

# Define the frequency range for the bass (e.g., 20Hz to 200Hz)
# Adjust this range based on your requirements
bass_freq_range = (20, 300)
mid_freq_range = (300, 4000)
high_freq_range = (4000, 15000)

# Convert the frequency range to bin indices
bass_bin_range = librosa.core.freq_to_bins(bass_freq_range, sr=sr)
mid_bin_range = librosa.core.freq_to_bins(mid_freq_range, sr=sr)
high_bin_range = librosa.core.freq_to_bins(high_freq_range, sr=sr)


# Sum the magnitude spectrum within the specified frequency range
bass_amplitude = np.sum(mag[bass_bin_range[0]:bass_bin_range[1], :], axis=0)
mid_amplitude = np.sum(mag[mid_bin_range[0]:mid_bin_range[1], :], axis=0)
high_amplitude = np.sum(mag[high_bin_range[0]:high_bin_range[1], :], axis=0)

# Create a time array for the x-axis
bass_time = librosa.times_like(bass_amplitude, sr=sr)


# Plot the bass amplitude
plt.figure(figsize=(12, 4))
librosa.display.waveshow(bass_amplitude, sr=sr, x_axis='time')
plt.xlabel('Time (s)')
plt.ylabel('Bass Amplitude')
plt.title('Bass Amplitude Extraction')
plt.show()
