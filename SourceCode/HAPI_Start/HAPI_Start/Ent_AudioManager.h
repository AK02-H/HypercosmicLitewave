#pragma once
#include "Entity.h"

class Ent_AudioManager :
    public Entity
{
private:

    std::vector<std::string> m_soundList;

    float m_sfxVolume = 1.0f;
    float m_mxVolume = 0.7f;
    float m_pan = 0;

    bool musicStartedPlaying = false;
    HAPI_TSoundOptions musicOptions = HAPI_TSoundOptions(m_mxVolume, true, 0, 1);

    int musicInstanceId = 0;

public:

    ~Ent_AudioManager()
    {
        HAPI.StopStreamedMedia(musicInstanceId);

    }

    Ent_AudioManager(float sfxVolume = 1.0f, float mxVolume = 0.7f)
    {
        //Should this be done here or in world.cpp?
        /*AddNewSound("playerfire.wav");
        AddNewSound("explosion1.wav");*/

        //Load music
        m_sfxVolume = sfxVolume;
        m_mxVolume = mxVolume;

        musicOptions = HAPI_TSoundOptions(m_mxVolume, true, 0, 1);
        
        PlayMusic();

    }

    void Load() override{};

    void Update(float dt) override final
    {
        /*std::cout << "HHHHHHHHHHHHHHHHHH" << std::endl;
        if (!musicStartedPlaying)
        {
            PlayMusic();
            musicStartedPlaying = true;
        }*/
    };

    void Render(IGraphics& viz) override final {};
    void LateUpdate(float dt) override final {};

    void AddNewSound(std::string soundName)
    {
        if (!HAPI.LoadSound("Data/Audio/" + soundName))
        {
            HAPI.UserMessage("Warning: sound '" + soundName + "' failed to load.", "Sound load error");
            return;
        }

        m_soundList.push_back(soundName);
    }

    void PlayMusic()
    {

        if (!HAPI.PlayStreamedMedia("Data/Audio/music1.wav", musicOptions, musicInstanceId))
        {
            HAPI.UserMessage("Music file could not be found", "Le music errore");
        }
    }

    void StopMusic()
    {
        HAPI.StopStreamedMedia(musicInstanceId);
    }

    void PlaySound(eSFX sound, bool overrideSettings = false, float volume = 1.0f, float pan = 0, float pitch = 1.0f)
    {
        PlaySound((int)sound, overrideSettings, volume, pan, pitch);
    }

    void PlaySound(int id, bool overrideSettings = false, float volume = 1.0f, float pan = 0, float pitch = 1.0f);
    

    void PlaySound(std::string soundName, bool overrideSettings = false, float volume = 1.0f, float pan = 0, float pitch = 1.0f);
    
};

