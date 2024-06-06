import numpy as np
from ..exceptions.exceptions import RythmForgeValueError
from .._lib import rythm_forge_core_cpp as core_backend
from .fft_functions import stft


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

    resampled_y, new_sr = core_backend.resample(y, sr, new_sr)
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
    Converts matrix filled with complex values to matrix of magnitudes of elements, similar to np.abs(array)
    :param complex_matrix: np.ndarray
        Array with complex values, most often from stft
    :return: np.ndarray
    """

    return np.abs(complex_matrix)


def find_peaks(y: np.ndarray):
    """
    Iterates through 1D array and returns indices of peak samples, meaning sample before and after are
     smaller than checked.
    :param y: 1D array, like onset_strength envelope, which elements will be compared
    :return: 1D np.nadrray, with elements being indices of beats that are peaks in the series
    """

    if y.ndim != 2:
        raise ValueError(f"Given vector y must be 1D, but {y.ndim}  was given")
    return core_backend.find_peaks(y)


def onset_strength(y: np.ndarray, sr: int):
    """
    Compute the onset strength envelope of an audio signal.

        The onset strength envelope is a measure of the sudden increases in energy across frequency bands,
        which typically correspond to note onsets or other transient events in the audio signal.

        :param y: np.ndarray
            The input audio signal as a 2D numpy array.
        :param sr: int
            The sample rate of the input audio signal.

        :return: np.ndarray
            The onset strength envelope of the audio signal as a 1D numpy array.
    """

    y_resampled, sr = resample(y, sr, 8000)
    S = stft(y_resampled, 2048, 512)
    S = magnitude(S)
    mel_filter = mel_filter_bank(sr, 2048, 40)
    mel_filter_expanded = mel_filter[np.newaxis, :, :]
    S = np.abs(np.dot(mel_filter_expanded, S)[0])
    S = power_to_dB(S)
    ref = S

    lag = 3
    onset_env = S[..., lag:] - ref[..., :-lag]
    onset_env = np.maximum(0.0, onset_env)

    pad_width = lag
    pad_width += 2048 // (2 * 512)

    padding = [(0, 0) for _ in onset_env.shape]
    padding[-1] = (int(pad_width), 0)
    onset_env = np.pad(onset_env, padding, mode="constant")

    return np.sum(onset_env, axis=0)


def tempo_estimation(y: np.ndarray, sr: int):
    """
    Does not work, don't know why
    Estimates tempo of the signal, returns bpm
    :param y: np.ndarray
        The input audio signal as a 1D numpy array.
    :param sr: int
        The sample rate of the input audio signal.
    :return: int representing calculated tempo in bpm
    """

    envelope = onset_strength(y, sr)
    peaks = find_peaks(envelope)
    peak_intervals = np.diff(peaks) / sr
    return 60 / np.mean(peak_intervals)


def beat_estimation(y: np.ndarray, sr: int):
    """
    Does not work, don't know why
    :param y: np.ndarray
        The input audio signal as a 1D numpy array.
    :param sr: int
        The sample rate of the input audio signal.
    :return: np.ndarray, with samples numbers being beats
    """

    tempo = tempo_estimation(y, sr)
    beat_interval = 60 / tempo
    beat_interval_samples = int(beat_interval / 8000)
    peaks = find_peaks(y)
    beat_location = [p * beat_interval_samples for p in peaks]
    return beat_location


def amplitude_to_dB(A, ref=1.0, amin=1e-10, top_db=80.0):
    """
    Convert an amplitude spectrogram to decibel (dB) units.

    :param A: np.ndarray
        Input amplitude spectrogram.
    :param ref: float or callable
        Reference value. If scalar, amplitude is scaled relative to `ref`. If callable, the reference value is computed as `ref(A)`.
    :param amin: float
        Minimum threshold for `A` and `ref`.
    :param top_db: float
        Threshold the output at `top_db` below the peak.
    :return: np.ndarray
        The dB-scaled spectrogram.
    """

    A = np.asarray(A)
    if callable(ref):
        ref_value = ref(A)
    else:
        ref_value = ref

    log_spec = 20.0 * np.log10(np.maximum(amin, A) / np.maximum(amin, ref_value))
    log_spec = np.maximum(log_spec, log_spec.max() - top_db)


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


def power_to_dB(S, ref=1.0, amin=1e-10, top_db=80.0):
    """
    Convert a power spectrogram to decibel (dB) units.

    :param S: np.ndarray
        Input power spectrogram.
    :param ref: float or callable
        Reference value. If scalar, amplitude is scaled relative to `ref`. If callable, the reference value is computed as `ref(S)`.
    :param amin: float
        Minimum threshold for `S` and `ref`.
    :param top_db: float
        Threshold the output at `top_db` below the peak.
    :return: np.ndarray
        The dB-scaled spectrogram.
    """
    S = np.asarray(S)
    if callable(ref):
        ref_value = ref(S)
    else:
        ref_value = ref

    log_spec = 10.0 * np.log10(np.maximum(amin, S) / np.maximum(amin, ref_value))
    log_spec = np.maximum(log_spec, log_spec.max() - top_db)

    return log_spec
