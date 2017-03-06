#pragma once

#include "OpenAL.h"
#include "AudioPosition.h"

namespace Roasting
{
    class OggStream
    {
    private:
        ALuint          _buffers[4];
        ALuint          _source;
        ALuint          _buffer;
        ALint           _state;
        ALint           _proc;
        ALint           _queueBuffer;
        bool            _pause;
        char*           _data;
        unsigned long   _freq;
        unsigned long   _format;
        unsigned long   _channels;
        unsigned long   _bufferSize;
        unsigned long   _writeSize;

        FILE*           _fp;
        OggVorbis_File  _vf;
        ov_callbacks    _callBacks;
        vorbis_info*    _info;

        unsigned long DecodeOggVorbis(OggVorbis_File *psOggVorbisFile, char *pDecodeBuffer, unsigned long ulBufferSize, unsigned long ulChannels);

    public:
        OggStream();
        ~OggStream();

        bool init(const std::string& filename);
        
        void play();
        void pause();
        void resume();
        void stop();

        bool isProcess();
        void pushBuffer();

    };
}