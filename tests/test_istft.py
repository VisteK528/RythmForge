import numpy as np
from rythm_forge.core import istft


def test_istft_zeros_input():
    stft_matrix = np.zeros((2, 1024, 1025), dtype=np.complex128)
    result = istft(stft_matrix, n_fft=2048, hop_size=512, window_length=2048, center=False)
    assert np.all(result == 0)


def test_istft_single_frequency():
    stft_matrix = np.zeros((1, 1024, 1025), dtype=np.complex128)
    stft_matrix[0, 5, :] = 1 + 0j
    result = istft(stft_matrix, n_fft=2048, hop_size=512, window_length=2048, center=False)
    assert np.argmax(np.abs(result)) == 0


def test_istft_high_values():
    stft_matrix = (np.ones((1, 1024, 1025)) * 1e10 + 1j * np.ones((1, 1024, 1025)) * 1e10).astype(np.complex128)
    result = istft(stft_matrix, n_fft=2048, hop_size=512, window_length=2048, center=False)
    assert np.isfinite(result).all()


def test_istft_negative_values():
    stft_matrix = (-np.ones((1, 1024, 1025)) + 1j * -np.ones((1, 1024, 1025))).astype(np.complex128)
    result = istft(stft_matrix, n_fft=2048, hop_size=512, window_length=2048, center=False)
    assert np.isfinite(result).all()


def test_istft_complex_input():
    stft_matrix = np.random.rand(1, 1024, 1025) + 1j * np.random.rand(1, 1024, 1025)
    result = istft(stft_matrix, n_fft=2048, hop_size=512, window_length=2048, center=False)
    assert np.isrealobj(result)


def test_istft_edge_case_length_1():
    stft_matrix = np.random.rand(1, 1, 2) + 1j * np.random.rand(1, 1, 2)
    result = istft(stft_matrix, n_fft=2, hop_size=1, window_length=2, center=False)
    assert result.shape == (3,)
