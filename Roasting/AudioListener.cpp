#include "AudioListener.h"

using namespace Roasting;

AudioListener::AudioListener()
    : _device(nullptr)
    , _context(nullptr)
{   }

AudioListener::~AudioListener()
{
    if (_context != nullptr)
    {
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(_context);
        _context = nullptr;
    }

    if (_device != nullptr)
    {
        alcCloseDevice(_device);
        _device = nullptr;
    }
}

void AudioListener::init()
{
    _device = alcOpenDevice(nullptr);
    _context = alcCreateContext(_device, nullptr);
    alcMakeContextCurrent(_context);
    alListenerfv(AL_POSITION, _position.toArray);
}

void AudioListener::setPosition(AudioPosition position)
{
    _position = position;
    alListenerfv(AL_POSITION, _position.toArray);
}