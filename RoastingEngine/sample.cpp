#include <iostream>
#include <Windows.h>
#include "Roasting.h"

using namespace Roasting;

#define SHOW_PLAYTIME(_AUDIO_) \
    if (_AUDIO_->getState() != AudioState::Stopped) \
    { \
        printf("%s (%d / %d)\n", #_AUDIO_, _AUDIO_->getPlayTime(), _AUDIO_->getLength()); \
    }

#define KEY_EVENT(_AUDIO_, _PLAY_, _PAUSE_, _RESUME_, _STOP_) \
    if(GetKeyState(_PLAY_) & 0x8000) \
    { \
        _AUDIO_->play(); \
    } \
    else if(GetKeyState(_PAUSE_) & 0x8000) \
    { \
        _AUDIO_->pause(); \
    } \
    else if(GetKeyState(_RESUME_) & 0x8000) \
    { \
        _AUDIO_->resume(); \
    } \
    else if(GetKeyState(_STOP_) & 0x8000) \
    { \
        _AUDIO_->stop(); \
    }

int main()
{
    AudioListener* listener = new AudioListener();
    listener->init();

    AudioSource* wav = new AudioSource();
    wav->init("Test.wav", true);

    AudioSource* mp3 = new AudioSource();
    mp3->init("Test.mp3", false);

    AudioSource* ogg = new AudioSource();
    ogg->init("Test.ogg", false);

    while (1)
    {
        system("cls");
        printf("Wav | q. Play | w. Pause | e. Resume | r.Stop\n");
        printf("Ogg | a. Play | s. Pause | d. Resume | f.Stop\n");
        printf("Mp3 | z. Play | x. Pause | c. Resume | v.Stop\n");
        printf("p. exit\n");

        SHOW_PLAYTIME(wav);
        SHOW_PLAYTIME(mp3);
        SHOW_PLAYTIME(ogg);

        KEY_EVENT(wav, 'Q', 'W', 'E', 'R');
        KEY_EVENT(ogg, 'A', 'S', 'D', 'F');
        KEY_EVENT(mp3, 'Z', 'X', 'C', 'V');

        if (GetKeyState('P') & 0x8000)
        {
            break;
        }
    }
    delete mp3;
    delete ogg;
    delete wav;
    delete listener;

    return 0;
}