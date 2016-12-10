#pragma once

#include "AudioClip.h"
#include "AudioPosition.h"

namespace Roasting
{
    enum class AudioState
    {
        Playing,
        Paused,
        Stopped,
        Error,
    };

    class AudioSource
    {
    private:
        AudioClip*      _clip;
        ALuint          _source;
        float           _volume;
        float           _pitch;
        float           _maxDistance;
        int             _playTime;
        int             _length;
        bool            _loop;
        bool            _pause;
        bool            _2d;
        AudioPosition   _position;

    public:
        AudioSource();
        ~AudioSource();

        AudioSource* init(const std::string& filename, bool loop = false);
        AudioSource* init(const AudioClip* clip, bool loop = false);

        void play();
        void pause();
        void resume();
        void stop();

        void setClip(const std::string& filename);
        AudioClip& getClip() const { return *_clip; }

        void setVolume(const float volume);
        float getVolume() const { return _volume; }

        void setPitch(const float pitch);
        float getPitch() const { return _pitch; }

        void setLoop(const bool loop);
        bool isLoop() const { return _loop; }

        int getLength() const { return _length; }
        const int getPlayTime();

        void setMaxDistance(const float maxDistance);
        float getMaxDistance() const { return _maxDistance; }

        void setPosition(const AudioPosition position);
        AudioPosition& getPosition() { return _position; }

        void set2D(const bool audio2D);
        bool is2D() const { return _2d; }

        AudioState getState() const;

    };
}