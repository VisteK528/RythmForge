import numpy as np
from rythm_forge.core import stft
import pytest


def test_stft_zeros_input():
    samples = np.zeros(2048)
    result = stft(samples)
    assert np.all(result == 0)


def test_stft_single_frequency():
    samples = np.sin(2 * np.pi * 5 * np.arange(2048) / 2048)
    result = stft(samples)
    assert 5 in np.argmax(np.abs(result), axis=0)


def test_stft_large_input():
    samples = np.random.rand(8192)
    result = stft(samples, n_fft=4096, hop_size=2048)
    assert result.shape[0] == 4096 // 2 + 1


def test_stft_small_input():
    samples = np.random.rand(512)
    result = stft(samples, n_fft=256, hop_size=128)
    assert result.shape[0] == 256 // 2 + 1


def test_stft_non_power_of_two():
    samples = np.random.rand(3000)
    with pytest.raises(RuntimeError):
        stft(samples, n_fft=1500, hop_size=750)


def test_stft_high_values():
    samples = np.ones(2048) * 1e10
    result = stft(samples)
    assert np.isfinite(result).all()


def test_stft_negative_values():
    samples = -np.ones(2048)
    result = stft(samples)
    assert np.isfinite(result).all()


def test_stft_multi_channel_different_lengths():
    samples = np.random.rand(2, 3072)
    result = stft(samples, n_fft=1024, hop_size=512)
    assert result.shape[0] == 2
    assert result.shape[1] == 1024 // 2 + 1


def test_stft_with_window():
    samples = np.random.rand(2048)
    result = stft(samples, window_length=1024, center=False)
    assert result.shape[1] == (2048 - 1024) / 512 + 1


def test_stft_center_false():
    samples = np.random.rand(2048)
    result = stft(samples, center=False)
    assert result.shape[0] == 2048 // 2 + 1
