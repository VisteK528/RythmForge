import pytest
import numpy as np
from rythm_forge.exceptions import RythmForgeValueError
from rythm_forge.core import fft, ifft, stft, istft


def test_fft_valid_input():
    samples = np.random.rand(1024)
    result = fft(samples)
    assert result.shape == samples.shape
    assert np.iscomplexobj(result)


def test_fft_invalid_input():
    samples = np.random.rand(1024, 2)
    with pytest.raises(RythmForgeValueError):
        fft(samples)


def test_ifft_valid_input():
    frequency_bins = np.random.rand(1024) + 1j * np.random.rand(1024)
    result = ifft(frequency_bins)
    assert result.shape == frequency_bins.shape
    assert np.iscomplexobj(result)


def test_ifft_invalid_input():
    frequency_bins = np.random.rand(1024, 2)
    with pytest.raises(RythmForgeValueError):
        ifft(frequency_bins)


def test_stft_valid_input():
    samples = np.random.rand(2048)
    result = stft(samples)
    assert result.ndim == 2
    assert result.shape[0] == 2048 // 2 + 1


def test_stft_multi_channel():
    samples = np.random.rand(2, 2048)
    result = stft(samples)
    assert result.ndim == 3
    assert result.shape[0] == 2
    assert result.shape[1] == 2048 // 2 + 1


def test_istft_valid_input():
    stft_matrix = np.random.rand(2, 1024, 1025) + 1j * np.random.rand(2, 1024, 1025)
    result = istft(stft_matrix)
    assert result.ndim == 2
    assert result.shape[0] == 2


def test_istft_single_channel():
    stft_matrix = np.random.rand(1024, 1025) + 1j * np.random.rand(1024, 1025)
    result = istft(stft_matrix)
    assert result.ndim == 1
    assert result.shape[0] > 0
