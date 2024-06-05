            import pytest
import numpy as np
from rythm_forge.exceptions import RythmForgeValueError
from rythm_forge.core import ifft


def test_ifft_valid_input():
    frequency_bins = np.random.rand(1024) + 1j * np.random.rand(1024)
    result = ifft(frequency_bins)
    assert result.shape == frequency_bins.shape
    assert np.iscomplexobj(result)


def test_ifft_invalid_input():
    frequency_bins = np.random.rand(1024, 2)
    with pytest.raises(RythmForgeValueError):
        ifft(frequency_bins)


def test_ifft_real_values():
    frequency_bins = np.ones(1024)
    result = ifft(frequency_bins)
    assert result.shape == frequency_bins.shape
    assert np.iscomplexobj(result)


def test_ifft_zero_input():
    frequency_bins = np.zeros(1024)
    result = ifft(frequency_bins)
    assert result.shape == frequency_bins.shape
    assert np.all(result == 0)


def test_ifft_high_dynamic_range():
    frequency_bins = np.concatenate((np.zeros(512), np.ones(512))) + 1j * np.concatenate((np.ones(512), np.zeros(512)))
    result = ifft(frequency_bins)
    assert result.shape == frequency_bins.shape
    assert np.iscomplexobj(result)


def test_ifft_length_odd():
    frequency_bins = np.random.rand(1023) + 1j * np.random.rand(1023)
    result = ifft(frequency_bins)
    assert result.shape[0] == frequency_bins.shape[0] - 1
    assert np.iscomplexobj(result)


def test_ifft_length_power_of_two():
    frequency_bins = np.random.rand(1024) + 1j * np.random.rand(1024)
    result = ifft(frequency_bins)
    assert result.shape == frequency_bins.shape
    assert np.iscomplexobj(result)


def test_ifft_negative_values():
    frequency_bins = -np.random.rand(1024) + 1j * (-np.random.rand(1024))
    result = ifft(frequency_bins)
    assert result.shape == frequency_bins.shape
    assert np.iscomplexobj(result)


def test_ifft_large_input():
    frequency_bins = (np.random.rand(2 ** 20) + 1j * np.random.rand(2 ** 20))
    result = ifft(frequency_bins)
    assert result.shape == frequency_bins.shape
    assert np.iscomplexobj(result)


def test_ifft_complex_input():
    frequency_bins = np.random.rand(1024) + 1j * np.random.rand(1024)
    result = ifft(frequency_bins)
    assert result.shape == frequency_bins.shape
    assert np.iscomplexobj(result)
