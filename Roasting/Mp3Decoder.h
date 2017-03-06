#pragma once

#include "IDecoder.h"

namespace Roasting
{
    class Mp3Decoder final
        : public IDecoder
    {
    private:
        mpg123_handle*  _handle;
        ALuint          _buffer;
        int             _length;

    public:
        Mp3Decoder();
        virtual ~Mp3Decoder();

        bool decode(const std::string& filename) override;

        ALuint getBuffer() const override { return _buffer; }
        int getLength() const override { return _length; }

    };
}