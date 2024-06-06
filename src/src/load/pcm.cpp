#include "../../include/load/pcm.hpp"
#include <iostream>
#include <numeric>


namespace rythm_forge {
    PCMData::PCMData(std::unique_ptr<d2array> samples, unsigned int sampleRate) : samples_(std::move(samples)), sampleRate_(sampleRate) {
        if (this->samples_) {
            this->numChannels_ = samples_->shape()[1];
        }
    }
    PCMData::PCMData(const PCMData &other) : sampleRate_(other.sampleRate_) {
        if (other.samples_) {
            this->samples_ = std::make_unique<d2array>(*other.samples_);
            this->numChannels_ = samples_->shape()[1];
        } else {
            this->samples_ = nullptr;
        }
    }

    double PCMData::getDurationTime() const {
        return static_cast<double>(this->samples_->size()) / static_cast<double>(this->sampleRate_);
    }

    unsigned int PCMData::getSampleRate() const {
        return this->sampleRate_;
    }

    const std::unique_ptr<d2array> &PCMData::getSamples() const {
        return this->samples_;
    }

    void PCMData::toMono() {
        if (this->numChannels_ > 1) {
            std::unique_ptr<d2array> array_mono = std::make_unique<d2array>(boost::extents[this->samples_->shape()[0]][1]);

            for (size_t i = 0; i < array_mono->shape()[0]; ++i) {
                double sum = std::accumulate((*this->samples_)[i].begin(), (*this->samples_)[i].end(), 0.0) / this->samples_->shape()[1];
                (*array_mono)[i][0] = sum;
            }
            this->samples_ = std::move(array_mono);
        }
    }

    PCMData PCMData::resample(const PCMData &other, unsigned int target_sr) {

        unsigned int sr = other.getSampleRate();
        uint16_t numChannels = other.numChannels_;
        double ratio = (double) target_sr / sr;

        int32_t newSampleCount = (ratio * other.getSamples()->shape()[0]);
        std::unique_ptr<d2array> newSample = std::make_unique<d2array>(boost::extents[newSampleCount][numChannels]);
        for (int64_t i = 0; i < (int64_t) (other.getSamples()->shape()[0] * ratio); ++i) {
            double index = (double) i / ratio;
            auto index_floor = (int64_t) floor(index);
            double frac = index - index_floor;

            for (int j = 0; j < numChannels; ++j) {
                double sample = (1 - frac) * other.getSamples()->operator[](index_floor)[j] + frac * other.getSamples()->operator[](index_floor + 1)[j];
                newSample->operator[](i)[j] = sample;
            }
        }
        return {std::move(newSample), target_sr};
    }
}// namespace rythm_forge
