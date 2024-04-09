import IPython
import matplotlib.pyplot as plt
import numpy as np
import rythm_forge_load_py
import librosa

data, sr = rythm_forge_load_py.read_bytes('../data/arcade.wav')

S = librosa.feature.melspectrogram(y=data, sr=sr, n_mels=128,
                                    fmax=20000)

fig, ax = plt.subplots()
S_dB = librosa.power_to_db(S, ref=np.max)
img = librosa.display.specshow(S_dB, x_axis='time',
                         y_axis='mel', sr=sr,
                         fmax=20000, ax=ax)
fig.colorbar(img, ax=ax, format='%+2.0f dB')
ax.set(title='Mel-frequency spectrogram')
plt.show()