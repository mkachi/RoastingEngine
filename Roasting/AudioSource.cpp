#include "AudioSource.h"
#include "OpenAL.h"

using namespace Roasting;

AudioSource::AudioSource()
    : _source(0)
    , _volume(1.0f)
    , _pitch(1.0f)
    , _playTime(0.0f)
    , _length(0.0f)
    , _loop(false)
    , _pause(false)
    , _2d(true)
{   }

AudioSource::~AudioSource()
{
    if (_clip != nullptr)
    {
        delete _clip;
    }

    if (_source != 0)
    {
        alDeleteSources(1,&_source);
        _source = 0;
    }
}

bool AudioSource::init(const std::string& filename, bool loop, AudioType type)
{
    _clip = AudioClip::create(filename);
    alGenSources(1, &_source);
    if (_source == 0)
    {
        return false;
    }

    alSourcei(_source, AL_BUFFER, _clip->getBuffer());
    _length = _clip->getLength();

    alSourcei(_source, AL_LOOPING, loop);

    alSourcef(_source, AL_MIN_GAIN, 0.0f);
    alSourcef(_source, AL_MAX_GAIN, 1.0f);
    alSourcef(_source, AL_PITCH, _pitch);
    alSourcef(_source, AL_GAIN, _volume);

    if (_2d)
    {
        float zero[] = { 0.0f, 0.0f, 0.0f };
        alSourcefv(_source, AL_POSITION, zero);
    }
    else
    {
        alSourcefv(_source, AL_POSITION, _position.toArray);
    }

    return true;
}

bool AudioSource::init(const AudioClip* clip, bool loop, AudioType type)
{
    _clip = _clip;
    alGenSources(1, &_source);
    if (_source == 0)
    {
        return false;
    }

    alSourcei(_source, AL_BUFFER, _clip->getBuffer());
    _length = _clip->getLength();

    alSourcei(_source, AL_LOOPING, loop);
    alSourcef(_source, AL_MIN_GAIN, 0.0f);
    alSourcef(_source, AL_MAX_GAIN, 1.0f);
    alSourcef(_source, AL_PITCH, _pitch);
    alSourcef(_source, AL_GAIN, _volume);

    float zero[] = { 0.0f, 0.0f, 0.0f };
    if (_2d)
    {
        alSourcefv(_source, AL_POSITION, zero);
    }
    alSourcefv(_source, AL_VELOCITY, _position.toArray);

    return true;
}

void AudioSource::play()
{
    _pause = false;
    alSourceStop(_source);
    alSourcePlay(_source);
}

void AudioSource::pause()
{
    _pause = true;
    alSourcePause(_source);
}

void AudioSource::resume()
{
    if (_pause)
    {
        _pause = false;
        alSourcePlay(_source);
    }
}

void AudioSource::stop()
{
    _pause = false;
    alSourceStop(_source);
}

void AudioSource::setClip(const std::string& filename)
{
    alSourceStop(_source);
    if (_clip != nullptr)
    {
        delete _clip;
        alDeleteSources(1, &_source);
    }
    _clip = AudioClip::create(filename);
    alGenSources(1, &_source);
    alSourcei(_source, AL_BUFFER, _clip->getBuffer());
    _length = _clip->getLength();
}

void AudioSource::setVolume(const float volume)
{
    _volume = volume;
    if (_volume > 1.0f)
    {
        _volume = 1.0f;
    }
    else if (_volume < 0.0f)
    {
        _volume = 0.0f;
    }
    alSourcef(_source, AL_GAIN, _volume);
}

void AudioSource::setPitch(const float pitch)
{
    _pitch = pitch;
    alSourcef(_source, AL_PITCH, pitch);
}

void AudioSource::setLoop(const bool loop)
{
    _loop = loop;
    alSourcef(_source, AL_LOOPING, loop);
}

const int AudioSource::getPlayTime()
{
    alGetSourcei(_source, AL_SEC_OFFSET, &_playTime);
    return _playTime;
}

void AudioSource::setMaxDistance(const float maxDistance)
{
    _maxDistance = maxDistance;
    alSourcef(_source, AL_MAX_DISTANCE, _maxDistance);
}

void AudioSource::setPosition(const AudioPosition& position)
{
    if (_2d)
    {
        return;
    }
    _position = position;
    alSourcefv(_source, AL_POSITION, _position.toArray);
}

void AudioSource::set2D(const bool audio2D)
{
    _2d = audio2D;
    float zero[] = { 0.0f, 0.0f, 0.0f };
    if (_2d)
    {
        alSourcefv(_source, AL_POSITION, zero);
    }
    else
    {
        alSourcefv(_source, AL_POSITION, _position.toArray);
    }
}

AudioState AudioSource::getState() const
{
    int state = AL_STOPPED;
    alGetSourcei(_source, AL_SOURCE_STATE, &state);

    if (state == AL_PLAYING)
    {
        return AudioState::Playing;
    }
    else if (state == AL_PAUSED)
    {
        return AudioState::Paused;
    }
    else if (state == AL_STOPPED || state == AL_INITIAL)
    {
        return AudioState::Stopped;
    }
    return AudioState::Error;
}