#include "WavDecoder.h"

using namespace Roasting;

WavDecoder::WavDecoder()
    : _buffer(0)
    , _length(0)
{   }

WavDecoder::~WavDecoder()
{   }

bool WavDecoder::decode(const std::string& filename)
{
    FILE* fp = nullptr;
    fopen_s(&fp, filename.c_str(), "rb");
    if (fp == nullptr)
    {
        assert(!"Audio file not found.");
        return false;
    }
    char type[4];
    long filesize, chunkSize;
    short formatType;
    long sampleRate, avgBytesPerSec;
    short bytesPerSample;

    short   bitsPerSample;
    void*   data;
    int     size;
    int     freq;
    short   channels;
    int     format;

    fread(type, sizeof(char), 4, fp);
    if (type[0] != 'R' ||
        type[1] != 'I' ||
        type[2] != 'F' ||
        type[3] != 'F')
    {
        return false;
    }

    fread(&filesize, sizeof(long), 1, fp);
    fread(type, sizeof(char), 4, fp);
    if (type[0] != 'W' ||
        type[1] != 'A' ||
        type[2] != 'V' ||
        type[3] != 'E')
    {
        return false;
    }
    fread(type, sizeof(char), 4, fp);

    fread(&chunkSize, sizeof(long), 1, fp);
    fread(&formatType, sizeof(short), 1, fp);
    fread(&channels, sizeof(short), 1, fp);
    fread(&sampleRate, sizeof(long), 1, fp);
    fread(&avgBytesPerSec, sizeof(long), 1, fp);
    fread(&bytesPerSample, sizeof(short), 1, fp);
    fread(&bitsPerSample, sizeof(short), 1, fp);
    fread(type, sizeof(char), 4, fp);
    fread(&size, sizeof(long), 1, fp);

    data = new unsigned char[size];
    fread(data, sizeof(unsigned char), size, fp);

    freq = sampleRate;

    alGenBuffers(1, &_buffer);
    if (bitsPerSample == 8)
    {
        if (channels == 1)
        {
            format = AL_FORMAT_MONO8;
        }

        else if (channels == 2)
        {
            format = AL_FORMAT_STEREO8;
        }
    }
    else if (bitsPerSample == 16)
    {
        if (channels == 1)
        {
            format = AL_FORMAT_MONO16;
        }

        else if (channels == 2)
        {
            format = AL_FORMAT_STEREO16;
        }
    }
    alBufferData(_buffer, format, data, size, freq);
    _length = (size * 8 / (channels * bitsPerSample)) / freq;
    fclose(fp);
    delete[] data;

    return true;
}