#pragma once

namespace Roasting
{
    class IStream
    {
    protected:
        IStream() {}

    public:
        virtual ~IStream() {}

        virtual void play() = 0;
        virtual void pause() = 0;
        virtual void resume() = 0;
        virtual void stop() = 0;

        void setVolume(const float volume);
        float getVolume() const { _volume; }

        void setPitch(const float pitch);
        float getPitch() const { return _pitch; }

        void setLoop(const bool loop);
        bool isLoop() const { return _loop; }

        int getLength() const { return _length; }
        const int getPlayTime();

        void setMaxDistance(const float maxDistance);
        float getMaxDistance() const { return _maxDistance; }

        void setPosition(const AudioPosition& position);

        virtual bool isProcess() = 0;
        virtual void pushBuffer() = 0;

    };
}