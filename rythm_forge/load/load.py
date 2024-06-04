import numpy as np

from .._lib import rythm_forge_load_cpp as load_backend
from ..exceptions import exceptions


def load(filename: str, mono=True):
    file = None

    if filename.endswith(".wav"):
        file = load_backend.load_wav_file(filename)
    else:
        exceptions.RythmForgeUnsupportedAudioFormat("Audio file has extension for unsupported format!")

    if mono:
        return to_mono(file)


def to_mono(x: np.ndarray):
    return load_backend.to_mono(x)