#pragma once

#include "IDecoder.h"

namespace Roasting
{
    class OggDecoder final
        : public IDecoder
    {
    private:
        ALuint  _buffer;
        int     _length;

    public:
        OggDecoder();
        virtual ~OggDecoder();

        bool decode(const std::string& filename) override;
        
        ALuint getBuffer() const override { return _buffer; }
        int getLength() const override { return _length; }

    };
}