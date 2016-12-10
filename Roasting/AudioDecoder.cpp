#include "AudioDecoder.h"
#include "WavDecoder.h"
#include "Mp3Decoder.h"
#include "OggDecoder.h"

using namespace Roasting;

static AudioDecoder* _instance = nullptr;

AudioDecoder::AudioDecoder()
{
    _decoders.push_back(new WavDecoder());
    _decoders.push_back(new Mp3Decoder());
    _decoders.push_back(new OggDecoder());
}

AudioDecoder::~AudioDecoder()
{
    if (!_decoders.empty())
    {
        for (int i = 0; i < _decoders.size(); ++i)
        {
            delete _decoders[i];
        }
        _decoders.clear();
    }
}

IDecoder* AudioDecoder::findDecoder(const std::string& filename)
{
    for (int i = 0; i < _decoders.size(); ++i)
    {
        if (_decoders[i]->decode(filename))
        {
            return _decoders[i];
        }
    }
    return nullptr;
}

IDecoder* AudioDecoder::decode(const std::string& filename)
{
    if (_instance == nullptr)
    {
        _instance = new AudioDecoder();
    }
    return _instance->findDecoder(filename);
}
