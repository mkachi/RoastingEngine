#pragma once

#include "AudioDecoder.h"

namespace Roasting
{
    class AudioClip
    {
    private:
        ALuint    _buffer;
        int       _length;
        bool init(const std::string& filename);
        AudioClip();

    public:
        ~AudioClip();
        static AudioClip* create(const std::string& filename);

        unsigned int getBuffer() const { return _buffer; }
        int getLength() const { return _length; }

    };
}