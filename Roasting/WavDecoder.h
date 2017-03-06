#pragma once

#include "IDecoder.h"

namespace Roasting
{
    class WavDecoder final
        : public IDecoder
    {
    private:
        ALuint  _buffer;
        int     _length;

    public:
        WavDecoder();
        virtual ~WavDecoder();

        bool decode(const std::string& filename) override;

        ALuint getBuffer() const override { return _buffer; }
        int getLength() const override { return _length; }

    };
}