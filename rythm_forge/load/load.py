import numpy as np

from .._lib import rythm_forge_load_cpp as load_backend
from ..exceptions import exceptions


def load(filename: str, mono=True):
    """
    Load an audio file and optionally convert it to mono.

    :param filename: str
        Path to the audio file to be loaded. The function currently supports only ".wav" files.
    :param mono: bool, optional
        If True, the loaded audio will be converted to mono. Default is True.
    :return: np.ndarray
        Loaded audio data, optionally converted to mono.
    :raises RythmForgeUnsupportedAudioFormat: If the audio file format is unsupported.
    """
    if filename.endswith(".wav"):
        y, sr = load_backend.load_wav_file(filename)
    else:
        raise exceptions.RythmForgeUnsupportedAudioFormat(
            "Audio file has extension for unsupported format!"
        )

    y.astype(np.float64)
    if mono:
        y = to_mono(y)
        y = y.reshape(
            y.shape[1],
        )
    return y, sr


def to_mono(y: np.ndarray) -> np.ndarray:
    """
    Convert a multi-channel audio signal to mono by averaging the channels.

    :param y: np.ndarray
        Multi-channel audio data.
    :return: np.ndarray
        Mono audio data.
    """
    return load_backend.to_mono(y.astype(np.float64))
