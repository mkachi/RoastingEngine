#include <iostream>
#include <Windows.h>
#include "AudioListener.h"
#include "OggStream.h"

using namespace Roasting;

#define COUNT 400

int main()
{
    AudioListener listener;
    listener.init();
    /*OggStream* stream = new OggStream();
    stream->init("Test2.ogg");

    while (stream->isPlaying())
    {
        stream->processing();
        stream->pushBuffer();

        if (GetKeyState('Q') & 0x8000)
        {
            stream->pause();
        }

        if (GetKeyState('W') & 0x8000)
        {
            stream->resume();
        }

        if (GetKeyState('E') & 0x8000)
        {
            stream->play();
        }

        if (GetKeyState('R') & 0x8000)
        {
            stream->stop();
        }
    }
    delete stream;*/

    OggStream* stream[COUNT];
    for (int i = 0; i < COUNT; ++i)
    {
        stream[i] = new OggStream();
        stream[i]->init("Test.ogg");
    }

    int breakCheck = 0;
    while (1)
    {
        breakCheck = 0;
        for (int i = 0; i < COUNT; ++i)
        {
            if (stream[i]->isProcess())
            {
                ++breakCheck;
                stream[i]->pushBuffer();
            }
        }
        if (breakCheck == 0)
        {
            break;
        }
    }

    for (int i = 0; i < COUNT; ++i)
    {
        delete stream[i];
    }

    return 0;
}