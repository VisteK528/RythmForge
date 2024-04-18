#include "../../include/core/pcm.hpp"

PCMData::PCMData(const PCMData& other) : sampleRate_(other.sampleRate_) {
    if (other.samples_) {
        samples_ = std::make_unique<std::vector<std::vector<double>>>(*other.samples_);
    }
    else {
        samples_ = nullptr;
    }
}

double PCMData::getDurationTime() const {
    return static_cast<double>(this->samples_->size()) / static_cast<double>(this->sampleRate_);
}

unsigned int PCMData::getSampleRate() const {
    return this->sampleRate_;
}

const std::unique_ptr<std::vector<std::vector<double>>> &PCMData::getSamples() const {
    return this->samples_;
}
