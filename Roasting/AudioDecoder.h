#pragma once

#include "IDecoder.h"
#include <vector>

namespace Roasting
{
    class AudioDecoder
    {
    private:
        std::vector<IDecoder*> _decoders;
        IDecoder* findDecoder(const std::string& filename);

    protected:
        AudioDecoder();

    public:
        ~AudioDecoder();
        static IDecoder* decode(const std::string& filename);

    };
}