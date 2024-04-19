from rythm_forge.load import to_mono
import numpy as np
from numpy.testing import assert_allclose

RTOL = 1e-8
ATOL = 1e-5


def test_stereo_to_mono():
    stereo = np.array([[10., 10., 50., 2., 86.], [20., 10., 50., 8., 14.]])
    assert stereo.shape == (2, 5)

    mono = to_mono(stereo)

    assert mono.shape == (1, 5)
    assert_allclose(mono[0][0], 15., rtol=RTOL, atol=ATOL)
    assert_allclose(mono[0][1], 10., rtol=RTOL, atol=ATOL)
    assert_allclose(mono[0][2], 50., rtol=RTOL, atol=ATOL)
    assert_allclose(mono[0][3], 5., rtol=RTOL, atol=ATOL)
    assert_allclose(mono[0][4], 50., rtol=RTOL, atol=ATOL)
