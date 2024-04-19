#include <numeric>
#include "../../include/load/pcm.hpp"

PCMData::PCMData(std::unique_ptr<std::vector<std::vector<double>>> samples, unsigned int sampleRate): samples_(std::move(samples)), sampleRate_(sampleRate) {
    if(this->samples_){
        this->numChannels_ = samples_->at(0).size();
    }
}
PCMData::PCMData(const PCMData& other) : sampleRate_(other.sampleRate_) {
    if (other.samples_) {
        this->samples_ = std::make_unique<std::vector<std::vector<double>>>(*other.samples_);
        this->numChannels_ = samples_->at(0).size();
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

const std::unique_ptr<std::vector<std::vector<double>>> &PCMData::getSamples() const {
    return this->samples_;
}

void PCMData::toMono() {
    if(this->numChannels_ > 1){
        long int index = 0;
        for(std::vector<double>& channelSamples: *this->samples_){
            double averageAcrossChannels = std::accumulate(channelSamples.cbegin(), channelSamples.cend(), 0.0) / static_cast<double>(channelSamples.size());
            this->samples_->at(index) = {averageAcrossChannels};
            ++index;
        }
    }
}
