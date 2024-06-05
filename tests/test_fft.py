import pytest
import numpy as np
from rythm_forge.exceptions import RythmForgeValueError
from rythm_forge.core import fft


def test_fft_valid_input():
    samples = np.random.rand(1024)
    result = fft(samples)
    assert result.shape == samples.shape
    assert np.iscomplexobj(result)


def test_fft_invalid_input():
    samples = np.random.rand(1024, 2)
    with pytest.raises(RythmForgeValueError):
        fft(samples)


def test_fft_length_odd():
    samples = np.random.rand(1023)
    result = fft(samples)
    assert result.shape[0] == samples.shape[0] - 1
    assert np.iscomplexobj(result)


def test_fft_length_power_of_two():
    samples = np.random.rand(1024)
    result = fft(samples)
    assert result.shape == samples.shape
    assert np.iscomplexobj(result)


def test_fft_real_values():
    samples = np.ones(1024)
    result = fft(samples)
    assert result.shape == samples.shape
    assert np.iscomplexobj(result)


def test_fft_complex_values():
    samples = np.random.rand(1024) + 1j * np.random.rand(1024)
    result = fft(samples)
    assert result.shape == samples.shape
    assert np.iscomplexobj(result)


def test_fft_large_input():
    samples = np.random.rand(2 ** 20)
    result = fft(samples)
    assert result.shape == samples.shape
    assert np.iscomplexobj(result)


def test_fft_zero_input():
    samples = np.zeros(1024)
    result = fft(samples)
    assert result.shape == samples.shape
    assert np.all(result == 0)


def test_fft_negative_values():
    samples = -np.random.rand(1024)
    result = fft(samples)
    assert result.shape == samples.shape
    assert np.iscomplexobj(result)


def test_fft_high_dynamic_range():
    samples = np.concatenate((np.zeros(512), np.ones(512)))
    result = fft(samples)
    assert result.shape == samples.shape
    assert np.iscomplexobj(result)
