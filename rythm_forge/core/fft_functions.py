import numpy as np
from ..exceptions.exceptions import RythmForgeValueError
from .._lib import rythm_forge_core_cpp as core_backend


def stft(samples: np.ndarray, n_fft=2048, hop_size=512, window_length=None, center=True):
    """
    Compute the Short-Time Fourier Transform (STFT) of the input samples.

    :param samples: A 1D or 2D numpy array of input audio samples.
    :param n_fft: The number of frequency_bins. Default is 2048.
    :param hop_size: The hop size between successive frames. Default is 512.
    :param window_length: The length of the window function applied to each frame. If None, it
                          defaults to `n_fft`.
    :param center: If True, the signal is padded so that the t-th frame is centered at
                   time t*hop_size. Default is True.
    :return: A 2D or 3D numpy array containing the STFT of the input samples.
    """
    if window_length is None:
        window_length = n_fft

    if samples.ndim == 1:
        samples = samples.reshape((1, samples.shape[0]))

    samples = samples.astype(np.float64)
    result = core_backend.stft(samples, n_fft, hop_size, window_length, center)

    if result.ndim == 3 and result.shape[0] == 1:
        result = result.reshape((result.shape[1], result.shape[2]))
    return result


def istft(stft_matrix: np.ndarray, n_fft=2048, hop_size=512, window_length=None, center=True):
    """
    Compute the Inverse Short-Time Fourier Transform (ISTFT) of the input STFT matrix.

    :param stft_matrix: A 2D or 3D numpy array containing the STFT of an audio signal.
    :param n_fft: The number of frequency_bins. Default is 2048.
    :param hop_size: The hop (or stride) size between successive frames. Default is 512.
    :param window_length: The length of the window function applied to each frame. If None, it
                          defaults to `n_fft`.
    :param center: If True, the signal is padded so that the t-th frame is centered at
                   time t*hop_size. Default is True.
    :return: A 1D or 2D numpy array containing the reconstructed audio signal.
    """
    if window_length is None:
        window_length = n_fft

    if stft_matrix.ndim == 2:
        stft_matrix = stft_matrix.reshape((1, stft_matrix.shape[0], stft_matrix.shape[1]))

    stft_matrix.astype(np.complex128)
    result = core_backend.istft(stft_matrix, n_fft, hop_size, window_length, center)

    if result.ndim == 2 and result.shape[0] == 1:
        result = result.reshape((result.shape[1],))
    return result


def fft(samples: np.ndarray) -> np.ndarray:
    """
    Compute the Fast Fourier Transform (FFT) of the input samples.

    :param samples: A 1D numpy array of input audio samples.
    :return: A 1D numpy array containing the frequency bins of the FFT.
    :raises RythmForgeValueError: If the input samples array is not 1D.
    """
    if samples.ndim != 1:
        raise RythmForgeValueError("Samples array should be of ndim == 1!")
    samples.astype(np.float64)
    return core_backend.fft(samples)


def ifft(frequency_bins: np.ndarray) -> np.ndarray:
    """
    Compute the Inverse Fast Fourier Transform (IFFT) of the input frequency bins.

    :param frequency_bins: A 1D numpy array of frequency bins.
    :return: A 1D numpy array containing the reconstructed audio samples.
    :raises RythmForgeValueError: If the input frequency bins array is not 1D.
    """
    if frequency_bins.ndim != 1:
        raise RythmForgeValueError("Frequency bins array should be of ndim == 1!")
    frequency_bins.astype(np.complex128)
    return core_backend.ifft(frequency_bins)
