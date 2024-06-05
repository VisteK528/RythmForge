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
    return mel_filter.T


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


def melspectrogram(
    stft_matrix: np.ndarray, n_fft=2048, sr=44100, n_mels=128
) -> np.ndarray:
    """
    Convert an STFT matrix to a mel spectrogram.

    This function transforms a Short-Time Fourier Transform (STFT) matrix into a mel spectrogram,
    where the frequency axis is mapped to the mel scale, which is a perceptually motivated scale of pitches.

    :param stft_matrix: np.ndarray
        The input STFT matrix of shape (..., n_freqs, n_times), representing the magnitude of the STFT of the audio signal.
    :param n_fft: int, optional, default=2048
        The number of FFT components, corresponding to the number of frequency bins in the STFT. This value determines the resolution of the frequency axis.
    :param sr: int, optional, default=44100
        The sample rate of the audio signal. This is used to compute the mel filter bank.
    :param n_mels: int, optional, default=128
        The number of mel bands to generate. This determines the resolution of the mel scale.
    :return: np.ndarray
        The mel spectrogram of shape (..., n_mels, n_times), where the frequency bins are replaced by mel bands.
    """

    if stft_matrix.ndim != 2:
        raise RythmForgeValueError(
            "Wrong STFT matrix dim number! STFT should have ndim=2"
        )

    mel_filter = mel_filter_bank(sr, n_fft, n_mels)

    return np.einsum("...ft,mf->...mt", stft_matrix, mel_filter, optimize=True)


def beat_estimation():
    pass


def tempo_estimation():
    pass
