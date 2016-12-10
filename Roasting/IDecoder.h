#pragma once

#include "OpenAL.h"
#include <string>
#include <cassert>

namespace Roasting
{
    class IDecoder
    {
    protected:
        IDecoder() {}

    public:
        virtual ~IDecoder() {}

        virtual bool decode(const std::string& filename) = 0;

        virtual ALuint getBuffer() const = 0;
        virtual int getLength() const = 0;


    };
}