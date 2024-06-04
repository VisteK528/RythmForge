

class RythmForgeTypeError(Exception):
    def __init__(self, message: str):
        super().__init__(message)


class RythmForgeValueError(Exception):
    def __init__(self, message: str):
        super().__init__(message)


class RythmForgeUnsupportedAudioFormat(Exception):
    def __init__(self, message: str):
        super().__init__(message)