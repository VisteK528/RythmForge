import numpy as np
from ..exceptions.exceptions import RythmForgeValueError
from .._lib import rythm_forge_core_cpp as core_backend


def resample(y: np.ndarray, sr: int, new_sr=8000) -> tuple[np.ndarray, int]:
    """
        Resample a time series from sr to new_sr
    :param y: np.ndarray
        A 1D or 2D numpy array of input audio samples, with each row being different channel
    :param sr: int
        Original sampling rate at which y has been acquired.
    :param new_sr: int
        Target sampling rate
    :return: ,int
        y_hat: mp.ndarray, y resampled from sr to new_sr
        new_sr:int sampling rate used in resampling
    """

    resampled_y = core_backend.resample(y, sr, new_sr)
    return resampled_y, new_sr


def mel_filter_bank(sr: int, n_fft: int, n_mel: int) -> np.ndarray:
    """
    Create a Mel filter-bank.
    This produces a linear transformation matrix to project FFT bins onto Mel-frequency bins.
    :param sr : int > 0 [scalar]
        Sampling rate of the incoming signal.

    :param n_fft: int > 0 [scalar]
        Number of FFT components

    :param n_mel: int > 0 [scalar]
        number of Mel bands to generate

    :return: M np.ndarray [shape=(n_mels, 1 + n_fft/2)]
        Mel transform matrix


    """
    mel_filter = core_backend.mel_filter_bank(sr, n_fft, n_mel)
    return mel_filter


def hz_to_mel(array: np.ndarray):
    """
    Converts Hz to Mels.
    :param array: np.ndarray of values in Hz to be converted to Mels
    """
    return core_backend.hz_to_mel(array)


def mel_to_hz(array: np.ndarray):
    """
    Converts Mels to Hz
    :param array: np.ndarray of values in Mels to be converted to Hz
    """
    return core_backend.mel_to_hz(array)


def magnitude(complex_matrix: np.ndarray):
    """
    Converts matrix filled with complex values to matrix of magnitudes of elements, simillar to np.abs(array)
    :param complex_matrix: np.ndarray
        Array with complex values, most often from stft
    :return: np.ndarray
    """
    return core_backend.magntude(complex_matrix)


def beat_estimation():
    pass


def tempo_estimation():
    pass
