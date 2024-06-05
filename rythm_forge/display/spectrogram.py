import matplotlib.pyplot as plt
import numpy as np
from ..exceptions import exceptions


def spectrogram(data: np.ndarray, sr=44100, hop_length=512, yaxis=None, time=True, fmin=None, fmax=None, ax=None)\
        -> None:
    """
    Displays a precomputed spectrogram in linear, log, or mel scale on the y-axis.

    Parameters:
    -----------
    data : np.ndarray
        The STFT matrix (2D array) containing the magnitude values of the spectrogram.
        Must be of type np.floating.

    sr : int, optional
        The sampling rate of the original audio signal. Default is 44100 Hz.

    hop_length : int, optional
        The number of samples between successive frames in the STFT. Default is 512.

    yaxis : str, optional
        The scale of the y-axis. Options are:
        - "linear" (default)
        - "log"
        - "mel"

    time : bool, optional
        If True, the x-axis will be labeled with time in seconds. Default is True.

    fmin : float, optional
        The minimum frequency to display on the y-axis. Default is 20 Hz.

    fmax : float, optional
        The maximum frequency to display on the y-axis. Default is sr/2.

    ax : matplotlib.axes.Axes, optional
        A matplotlib Axes object to plot on. If None, a new figure and axes will be created.

    Raises:
    -------
    RythmForgeTypeError
        If the data type of the STFT matrix is not np.float64 or an unknown yaxis type is provided.

    RythmForgeValueError
        If the STFT matrix does not have exactly 2 dimensions.

    Returns:
    --------
    None
        The function displays the spectrogram plot and does not return any value.
    """

    if not np.issubdtype(data.dtype, np.floating):
        raise exceptions.RythmForgeTypeError(
            "Unsupported data type in STFT matrix! Provide matrix with elements of type np.float64")

    if data.ndim != 2:
        raise exceptions.RythmForgeValueError("Wrong STFT matrix dim number! STFT should have ndim=2")

    if fmin is None:
        fmin = 20

    if fmax is None:
        fmax = sr / 2

    if ax is None:
        fig, ax = plt.subplots(figsize=(10, 5))
        plt.tight_layout()

    ax.imshow(data, aspect='auto', origin='lower', cmap='magma',
               extent=[0, data.shape[1], fmin, fmax], vmin=-80,
               vmax=0)

    if yaxis in [None, "linear", "mel"]:
        plt.yscale("linear")
    elif yaxis == "log":
        plt.yscale("log")
    else:
        raise exceptions.RythmForgeTypeError(f"Unknown yscale {yaxis}!")

    if time:
        ticks = np.linspace(0, data.shape[1], 5)[:-1]
        est_times = [round(x * hop_length / sr) for x in ticks]
        labels = [f"{x // 60}:{x % 60}" for x in est_times]
        plt.xticks(ticks, labels)
        plt.xlabel("Time")

    plt.ylabel("Hz")
    plt.ylim([fmin, fmax])


def melspectrogram():
    pass
