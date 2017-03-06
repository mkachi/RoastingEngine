#include "OggStream.h"

using namespace Roasting;

inline void swap(short& a, short& b)
{
    short temp = a;
    a = b;
    b = temp;
}

size_t read_func(void *ptr, size_t size, size_t nmemb, void *datasource)
{
    return fread(ptr, size, nmemb, (FILE*)datasource);
}

int seek_func(void *datasource, ogg_int64_t offset, int whence)
{
    return fseek((FILE*)datasource, (long)offset, whence);
}

int close_func(void *datasource)
{
    return fclose((FILE*)datasource);
}

long tell_func(void *datasource)
{
    return ftell((FILE*)datasource);
}

OggStream::OggStream()
    : _source(0)
    , _buffer(0)
    , _state(0)
    , _proc(0)
    , _queueBuffer(0)
    , _pause(false)
    , _data(nullptr)
    , _freq(0)
    , _format(0)
    , _channels(0)
    , _bufferSize(0)
    , _writeSize(0)
    , _fp(nullptr)
    , _info(nullptr)
{   }

OggStream::~OggStream()
{
    ov_clear(&_vf);
    fclose(_fp);
}

bool OggStream::init(const std::string& filename)
{
    _callBacks.read_func = read_func;
    _callBacks.seek_func = seek_func;
    _callBacks.tell_func = tell_func;
    _callBacks.close_func = close_func;

    if (_fp == nullptr)
    {
        fopen_s(&_fp, filename.c_str(), "rb");
    }
    ov_open_callbacks(_fp, &_vf, nullptr, 0, _callBacks);

    _info = ov_info(&_vf, -1);
    _freq = _info->rate;
    _channels = _info->channels;

    if (_info->channels == 1)
    {
        _format = AL_FORMAT_MONO16;
        _bufferSize = _freq >> 1;
        _bufferSize -= (_bufferSize % 2);
    }
    else if (_info->channels == 2)
    {
        _format = AL_FORMAT_STEREO16;
        _bufferSize = _freq;
        _bufferSize -= (_bufferSize % 4);
    }
    else if (_info->channels == 4)
    {
        _format = alGetEnumValue("AL_FORMAT_QUAD16");
        _bufferSize = _freq * 2;
        _bufferSize -= (_bufferSize % 8);
    }
    else if (_info->channels == 6)
    {
        _format = alGetEnumValue("AL_FORMAT_51CHN16");
        _bufferSize = _freq * 3;
        _bufferSize -= (_bufferSize % 12);
    }
    _data = new char[_bufferSize];

    alGenBuffers(4, _buffers);
    alGenSources(1, &_source);

    for (int i = 0; i < 4; ++i)
    {
        _writeSize = DecodeOggVorbis(&_vf, _data, _bufferSize, _channels);
        if (_writeSize)
        {
            alBufferData(_buffers[i], _format, _data, _writeSize, _freq);
            alSourceQueueBuffers(_source, 1, &_buffers[i]);
        }
    }
    return true;
}

bool OggStream::isProcess()
{
    alGetSourcei(_source, AL_SOURCE_STATE, &_state);
    if (_state == AL_PAUSED)
    {
        _pause = true;
        return true;
    }
    _pause = false;

    if (_state == AL_STOPPED)
    {
        return false;
    }

    if (_state != AL_PLAYING)
    {
        alSourcePlay(_source);
        alGetSourcei(_source, AL_SOURCE_STATE, &_state);
    }
    return true;
}

void OggStream::pushBuffer()
{
    if (_pause)
    {
        return;
    }

    _proc = 0;
    alGetSourcei(_source, AL_BUFFERS_PROCESSED, &_proc);

    while (_proc)
    {
        _buffer = NULL;
        alSourceUnqueueBuffers(_source, 1, &_buffer);
        _writeSize = DecodeOggVorbis(&_vf, _data, _bufferSize, _channels);
        if (_writeSize)
        {
            alBufferData(_buffer, _format, _data, _writeSize, _freq);
            alSourceQueueBuffers(_source, 1, &_buffer);
        }
        --_proc;
    }
}

void OggStream::play()
{
    alSourceStop(_source);
    alSourcePlay(_source);
}

void OggStream::pause()
{
    alSourcePause(_source);
}

void OggStream::resume()
{
    if (_pause)
    {
        alSourcePlay(_source);
    }
}

void OggStream::stop()
{
    alSourceStop(_source);
}

unsigned long OggStream::DecodeOggVorbis(OggVorbis_File* psOggVorbisFile, char* pDecodeBuffer, unsigned long ulBufferSize, unsigned long ulChannels)
{
    int currentSection;
    long decodeSize;
    short* sampleData;

    unsigned long bytesDone = 0;
    while (1)
    {
        decodeSize = ov_read(psOggVorbisFile, pDecodeBuffer + bytesDone, ulBufferSize - bytesDone, 0, 2, 1, &currentSection);
        if (decodeSize > 0)
        {
            bytesDone += decodeSize;

            if (bytesDone >= ulBufferSize)
                break;
        }
        else
        {
            break;
        }
    }

    if (ulChannels == 6)
    {
        sampleData = (short*)pDecodeBuffer;
        for (int i = 0; i < (ulBufferSize >> 1); i += 6)
        {
            swap(sampleData[i + 1], sampleData[i + 2]);
            swap(sampleData[i + 3], sampleData[i + 5]);
            swap(sampleData[i + 4], sampleData[i + 5]);
        }
    }

    return bytesDone;
}