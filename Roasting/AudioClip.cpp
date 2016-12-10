#include "AudioClip.h"

using namespace Roasting;

AudioClip::AudioClip()
    : _buffer(0)
    , _length(0)
{   }

AudioClip::~AudioClip()
{
    if (_buffer != 0)
    {
        alDeleteBuffers(1, &_buffer);
        _buffer = 0;
    }
}

bool AudioClip::init(const std::string& filename)
{
    IDecoder* decoder = AudioDecoder::decode(filename);
    if (decoder == nullptr)
    {
        assert(!"Unsupported format.");
        return false;
    }
    _buffer = decoder->getBuffer();
    _length = decoder->getLength();

    return true;
}

AudioClip* AudioClip::create(const std::string& filename)
{
    AudioClip* result = new (std::nothrow) AudioClip();
    if (result != nullptr && result->init(filename))
    {
        return result;
    }
    return nullptr;
}