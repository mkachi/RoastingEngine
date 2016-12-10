#include "OggDecoder.h"
#include <vector>

using namespace Roasting;

enum { OGG_BUFFER_SIZE = 32768, };

OggDecoder::OggDecoder()
    : _buffer(0)
    , _length(0)
{   }

OggDecoder::~OggDecoder()
{   }

bool OggDecoder::decode(const std::string& filename)
{
    FILE*           fp = nullptr;
    OggVorbis_File  vf;

    int freq;

    int     endian = 0;
    int     bitStream;
    long    bytes;
    char    buffers[OGG_BUFFER_SIZE];

    std::vector<char> bufferData;

    fopen_s(&fp, filename.c_str(), "rb");
    if (fp == nullptr)
    {
        assert(!"Audio file not found.");
        return false;
    }

    int result = ov_test(fp, &vf, NULL, NULL);
    if (result != 0)
    {
        ov_clear(&vf);
        return false;
    }
    result = ov_test_open(&vf);
    if (result != 0)
    {
        return false;
    }

    vorbis_info* info = ov_info(&vf, -1);
    ALenum format;
    if (info->channels == 1)
    {
        format = AL_FORMAT_MONO16;
    }
    else if (info->channels == 2)
    {
        format = AL_FORMAT_STEREO16;
    }
    else if (info->channels == 4)
    {
        format = alGetEnumValue("AL_FORMAT_QUAD16");
    }
    else if (info->channels == 6)
    {
        format = alGetEnumValue("AL_FORMAT_51CHN16");
    }
    freq = info->rate;

    do
    {
        bytes = ov_read(&vf, buffers, OGG_BUFFER_SIZE, endian, 2, 1, &bitStream);
        if (bytes < 0)
        {
            ov_clear(&vf);
        }
        bufferData.insert(bufferData.end(), buffers, buffers + bytes);
    } while (bytes > 0);

    _length = ov_time_total(&vf, -1);
    ov_clear(&vf);

    alGenBuffers(1, &_buffer);
    alBufferData(_buffer, format, bufferData.data(), static_cast<int>(bufferData.size()), freq);
    fclose(fp);
    return true;
}