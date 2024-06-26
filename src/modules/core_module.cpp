#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "../include/core/fft.hpp"
#include "../include/load/pcm.hpp"
#include "../include/core/np_boost_array.hpp"
#include "../include/core/feature.hpp"


namespace py = pybind11;

static py::array_t<rythm_forge::dcomplex> assignNumpyArray(const std::unique_ptr<rythm_forge::c3array> &samples) {
    const size_t numChannels = samples->shape()[0];
    const size_t frequencyBins = samples->shape()[1];
    const size_t frames = samples->shape()[2];

    py::array_t<rythm_forge::dcomplex> numpyArray({numChannels, frequencyBins, frames});

    auto numpyArrayData = numpyArray.mutable_unchecked<3>();
    for (size_t i = 0; i < numChannels; ++i) {
        for (size_t j = 0; j < frequencyBins; ++j) {
            for (size_t k = 0; k < frames; ++k) {
                numpyArrayData(i, j, k) = (*samples)[i][j][k];
            }
        }
    }
    return numpyArray;
}

[[maybe_unused]] static py::array_t<double> assignNumpyArray(const std::unique_ptr<rythm_forge::d3array> &samples) {
    const size_t numChannels = samples->shape()[0];
    const size_t frequencyBins = samples->shape()[1];
    const size_t frames = samples->shape()[2];

    py::array_t<rythm_forge::dcomplex> numpyArray({numChannels, frequencyBins, frames});

    auto numpyArrayData = numpyArray.mutable_unchecked<3>();
    for (size_t i = 0; i < numChannels; ++i) {
        for (size_t j = 0; j < frequencyBins; ++j) {
            for (size_t k = 0; k < frames; ++k) {
                numpyArrayData(i, j, k) = (*samples)[i][j][k];
            }
        }
    }
    return numpyArray;
}
static py::array_t<double> assignNumpyArrayDouble(const std::unique_ptr<rythm_forge::d2array> &samples, size_t numChannels) {
    py::array_t<double> numpyArray({numChannels, samples->size()});

    auto numpyArrayData = numpyArray.mutable_unchecked<2>();
    for (size_t i = 0; i < numChannels; ++i) {
        for (size_t j = 0; j < samples->size(); ++j) {
            numpyArrayData(i, j) = (*samples)[j][i];
        }
    }
    return numpyArray;
}

py::array_t<rythm_forge::dcomplex> fft_python(py::array_t<rythm_forge::dcomplex> &input_samples) {

    auto input = std::vector<rythm_forge::dcomplex>(input_samples.size());

    for (long i = 0; i < input_samples.size(); ++i) {
        input[i] = input_samples.at(i);
    }
    std::vector<rythm_forge::dcomplex> result = rythm_forge::fft::fft(input);
    py::array_t<rythm_forge::dcomplex> numpyArray(result.size(), result.data());
    return numpyArray;
}

py::array_t<rythm_forge::dcomplex> ifft_python(py::array_t<rythm_forge::dcomplex> &input_samples) {

    auto input = std::vector<rythm_forge::dcomplex>(input_samples.size());

    for (long i = 0; i < input_samples.size(); ++i) {
        input[i] = input_samples.at(i);
    }
    std::vector<rythm_forge::dcomplex> result = rythm_forge::fft::ifft(input);
    py::array_t<rythm_forge::dcomplex> numpyArray(result.size(), result.data());
    return numpyArray;
}

py::array_t<rythm_forge::dcomplex> stft_python(py::array_t<double> &input_samples, const int nFft, const int hopSize, const int windowLength, bool center) {
    auto r = input_samples.unchecked<2>();
    size_t channels = r.shape(0);
    size_t samples_len = r.shape(1);

    auto result = std::make_unique<rythm_forge::d2array>(boost::extents[samples_len][channels]);

    for (size_t i = 0; i < samples_len; ++i) {
        for (size_t j = 0; j < channels; ++j) {
            (*result)[i][j] = r(j, i);
        }
    }

    std::unique_ptr<rythm_forge::c3array> stft_matrix = rythm_forge::fft::stft(result, nFft,
                                                                               hopSize, windowLength,
                                                                               center);
    py::array_t<rythm_forge::dcomplex> numpyArray = assignNumpyArray(stft_matrix);
    return numpyArray;
}

py::array_t<double> istft_python(py::array_t<rythm_forge::dcomplex> &input_samples, const int nFft, const int hopSize, const int windowLength, bool center) {
    auto r = input_samples.unchecked<3>();
    size_t channels = r.shape(0);
    size_t frequencyBins = r.shape(1);
    size_t frames = r.shape(2);

    auto result = std::make_unique<rythm_forge::c3array>(boost::extents[channels][frequencyBins][frames]);
    for (size_t i = 0; i < channels; ++i) {
        for (size_t j = 0; j < frequencyBins; ++j) {
            for (size_t k = 0; k < frames; ++k) {
                (*result)[i][j][k] = r(i, j, k);
            }
        }
    }

    std::unique_ptr<rythm_forge::d2array> istftResult = rythm_forge::fft::istft(result, nFft,
                                                                                hopSize, windowLength,
                                                                                center);

    py::array_t<double> numpyArray = assignNumpyArrayDouble(istftResult, channels);
    return numpyArray;
}

py::tuple resample_python(py::array_t<double>& inputSample, int sr,int newSr){
    py::buffer_info buf_info = inputSample.request();
    auto dataArray = np2DtoMultiarray(inputSample);
    rythm_forge::PCMData data(std::move(dataArray), sr);
    auto newPCM = rythm_forge::PCMData::resample(data,newSr);

    py::array_t<double> numpyArray = multiarray2DtoNp(newPCM.getSamples());
    return py::make_tuple(numpyArray,newSr);
}


double hz_to_mel(double hz_value){
    return rythm_forge::feature::hzToMel(hz_value);
}

double mel_to_hz(double mel_value){
        return rythm_forge::feature::melToHz(mel_value);
}

py::array_t<double> calculate_magnitude_python(const py::array_t<rythm_forge::dcomplex>& complexMatrix){

    auto matrix = np3DtoMultiarrayComplex(complexMatrix);
    auto magnitude = rythm_forge::fft::calculateMagnitude(matrix);
    return multiarray3DtoNp(magnitude);

}

py::array_t<double> calculate_mel_filter_bank_python(uint32_t sampleRate,uint32_t nFft,uint32_t nMels){
    auto bank = rythm_forge::feature::create_mel_filter_bank(sampleRate,nFft,nMels);
    return multiarray2DtoNp(bank);
}

py::array_t<int> find_peaks_python(const py::array_t<float>& onset_envelope){
    py::buffer_info buf = onset_envelope.request();
    std::vector<double> envelope_vector(reinterpret_cast<double*>(buf.ptr),reinterpret_cast<double*>(buf.ptr) + buf.size);
    std::vector<int> peaks_samples = rythm_forge::feature::find_pick(envelope_vector);
    py::array_t<int> output_array(peaks_samples.size());
    auto output_buf = output_array.request();
    int* output_ptr = static_cast<int*>(output_buf.ptr);
    std::copy(peaks_samples.begin(), peaks_samples.end(), output_ptr);
    return output_array;
}
PYBIND11_MODULE(rythm_forge_core_cpp, m) {
    m.doc() = "RythmForge core module";
    m.def("stft", &stft_python, "Short Time Fourier Transform");
    m.def("istft", &istft_python, "Inverse Short Time Fourier Transform");
    m.def("fft", &fft_python, "Fast Fourier Transform");
    m.def("ifft", &ifft_python, "Fast Fourier Transform");
    m.def("resample",&resample_python, "Resample");
    m.def("magnitude",&calculate_magnitude_python,"Calculate magnitude of a complex-valued matrix");
    m.def("hz_to_mel",py::vectorize(hz_to_mel),"Convert from hz to mel");
    m.def("mel_to_hz",py::vectorize(mel_to_hz),"Convert from mel scale to hz");
    m.def("mel_filter_bank",&calculate_mel_filter_bank_python,"Get mel filter bank");
    m.def("find_peaks",&find_peaks_python,"Finds peaks from 1D array");
}