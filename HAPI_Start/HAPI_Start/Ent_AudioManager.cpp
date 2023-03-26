#include "Ent_AudioManager.h"

void Ent_AudioManager::PlaySound(int id, bool overrideSettings, float volume, float pan, float pitch)
{
    if (id >= m_soundList.size())
    {
        HAPI.UserMessage("Warning: tried to pass a sound id larger than list of sounds.", "Sound play error");
        return;
    }

    HAPI_TSoundOptions options;

    if (!overrideSettings)
    {
        options = HAPI_TSoundOptions(m_sfxVolume, false, m_pan);
    }
    else
    {
        options = HAPI_TSoundOptions(volume * m_sfxVolume, false, pan, pitch);
    }


    HAPI.PlaySound("Data/Audio/" + m_soundList[id], options);
}

void Ent_AudioManager::PlaySound(std::string soundName, bool overrideSettings, float volume, float pan, float pitch)
{
    for (const std::string& name : m_soundList)
    {
        if (soundName == name)
        {

            HAPI_TSoundOptions options;

            if (!overrideSettings)
            {
                options = HAPI_TSoundOptions(m_sfxVolume, false, m_pan);
            }
            else
            {
                options = HAPI_TSoundOptions(volume * m_sfxVolume, false, pan, pitch);
            }

            HAPI.PlaySound("Data/Audio/" + name, options);

        }
    }
    HAPI.UserMessage("Warning: could not find sound of name '" + soundName + "'.", "Sound play error");
    return;
}
