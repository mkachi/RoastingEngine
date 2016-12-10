#pragma once

#include "OpenAL.h"
#include "AudioPosition.h"

namespace Roasting
{
    class AudioListener
    {
    private:
        ALCdevice*      _device;
        ALCcontext*     _context;
        AudioPosition   _position;

    public:
        AudioListener();
        ~AudioListener();

        void init();
        void setPosition(AudioPosition position);
        AudioPosition getPosition() { return _position; }

    };
}