#include "Mp3Decoder.h"

using namespace Roasting;

Mp3Decoder::Mp3Decoder()
    : _buffer(0)
    , _length(0)
{   
    mpg123_init();
    _handle = mpg123_new(NULL, NULL);
    if (MPG123_OK != mpg123_format(_handle, 44100, MPG123_MONO | MPG123_STEREO, MPG123_ENC_UNSIGNED_8 | MPG123_ENC_SIGNED_16))
    {
        assert(!"Do not use this format.");
    }
}

Mp3Decoder::~Mp3Decoder()
{   
    mpg123_delete(_handle);
}

bool Mp3Decoder::decode(const std::string& filename)
{
    if (MPG123_OK != mpg123_open(_handle, filename.c_str()))
    {
        assert(!"Audio file not found.");
        return false;
    }

    ALenum format = AL_NONE;
    ALsizei freq = 0;
    ALsizei size = 0;

    // getInfo
    int channels = 0;
    int encoding = 0;
    long rate = 0;
    int bit = 8;

    int Error = mpg123_getformat(_handle, &rate, &channels, &encoding);
    if (MPG123_OK != Error)
    {
        return false;
    }
    size = mpg123_length(_handle);
    if (size == MPG123_ERR)
    {
        return false;
    }
    freq = rate;
    if (encoding == MPG123_ENC_UNSIGNED_8 ||
        encoding == MPG123_ENC_SIGNED_8 ||
        encoding == MPG123_ENC_ULAW_8 ||
        encoding == MPG123_ENC_ALAW_8 ||
        encoding == MPG123_ENC_8)
    {
        if (channels == 1)
        {
            format = AL_FORMAT_MONO8;
        }
        else
        {
            format = AL_FORMAT_STEREO8;
        }
        bit = 8;

    }
    else if (
        encoding == MPG123_ENC_UNSIGNED_16 ||
        encoding == MPG123_ENC_SIGNED_16 ||
        encoding == MPG123_ENC_16)
    {
        if (channels == 1)
        {
            format = AL_FORMAT_MONO16;
        }
        else
        {
            format = AL_FORMAT_STEREO16;
        }
        bit = 16;
    }
    else if (
        encoding == MPG123_ENC_UNSIGNED_24 ||
        encoding == MPG123_ENC_SIGNED_24 ||
        encoding == MPG123_ENC_24)
    {
        if (channels == 1)
        {
            format = AL_FORMAT_MONO16;
        }
        else
        {
            format = AL_FORMAT_STEREO16;
        }

        bit = 24;
    }
    else if (encoding == MPG123_ENC_UNSIGNED_32 ||
        encoding == MPG123_ENC_SIGNED_32 ||
        encoding == MPG123_ENC_FLOAT_32 ||
        encoding == MPG123_ENC_32)
    {
        if (channels == 1)
        {
            format = AL_FORMAT_MONO16;
        }
        else
        {
            format = AL_FORMAT_STEREO16;
        }

        bit = 32;
    }
    else if (encoding == MPG123_ENC_FLOAT_64)
    {
        if (channels == 1)
        {
            format = AL_FORMAT_MONO16;
        }
        else
        {
            format = AL_FORMAT_STEREO16;
        }

        bit = 64;
    }

    if (format == AL_FORMAT_MONO16 || format == AL_FORMAT_STEREO16)
    {
        size *= 2;
    }
    char* data = new char[size];
    size_t done = 0;
    if (MPG123_DONE == mpg123_read(_handle, (unsigned char*)data, size, &done))
    {
        return false;
    }

    alGenBuffers(1, &_buffer);
    alBufferData(_buffer, format, data, size, freq);

    _length = (size * 8 / (channels * bit)) / freq;

    mpg123_close(_handle);
    delete[] data;

    return true;
}