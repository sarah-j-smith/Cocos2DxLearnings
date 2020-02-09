//
//  AudioContent.hpp
//  Sky Defense
//
//  Created by Sarah Smith on 12/Jan/20.
//

#pragma once

class AudioContent
{
public:
    enum class Id
    {
        Background,
        BombFail,
        BombRelease,
        Boom,
        Health,
        FireTruck
    };
    
    AudioContent();
    
    void init();
    void setAudioVolume(int volume);
    void setEffectsVolume(int volume);
    void play(Id contentId) const;
    
    static AudioContent &getInstance();
    
private:
    int audioVolume;
    int effectsVolume;
};
