#include <numeric>
#include "../../include/load/pcm.hpp"

PCMData::PCMData(std::unique_ptr<mdarray > samples, unsigned int sampleRate): samples_(std::move(samples)), sampleRate_(sampleRate) {
    if(this->samples_){
        this->numChannels_ = samples_->shape()[0];
    }
}
PCMData::PCMData(const PCMData& other) : sampleRate_(other.sampleRate_) {
    if (other.samples_) {
        this->samples_ = std::make_unique<mdarray>(*other.samples_);
        this->numChannels_ = samples_->shape()[0];
    }
    else {
        this->samples_ = nullptr;
    }
}

double PCMData::getDurationTime() const {
    return static_cast<double>(this->samples_->size()) / static_cast<double>(this->sampleRate_);
}

unsigned int PCMData::getSampleRate() const {
    return this->sampleRate_;
}

const std::unique_ptr<mdarray> &PCMData::getSamples() const {
    return this->samples_;
}

void PCMData::toMono() {
    if(this->numChannels_ > 1){
        std::unique_ptr<mdarray> array_mono = std::make_unique<mdarray>(boost::extents[this->samples_->shape()[0]][1]);

        for(size_t i = 0; i < array_mono->shape()[0]; ++i){
            double sum = std::accumulate((*this->samples_)[i].begin(),(*this->samples_)[i].end(), 0.0) / this->samples_->shape()[1];
            (*array_mono)[i][0] = sum;
        }
        this->samples_ = std::move(array_mono);
    }
}
