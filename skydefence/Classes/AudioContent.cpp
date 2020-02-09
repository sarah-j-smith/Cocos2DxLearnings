//
//  AudioContent.cpp
//  Sky Defense
//
//  Created by Sarah Smith on 12/Jan/20.
//

#include "AudioContent.h"

#include "AudioEngine.h"

AudioContent &AudioContent::getInstance()
{
    static AudioContent sharedAudioContent;
    
    return sharedAudioContent;
}

AudioContent::AudioContent()
    : audioVolume(1.0)
    , effectsVolume(1.0)
{
}

void AudioContent::init()
{
//    cocos2d::AudioEngine::preload("background.mp3");
//
//    cocos2d::AudioEngine::preload("bombFail.wav");
//    cocos2d::AudioEngine::preload("bombRelease.wav");
//    cocos2d::AudioEngine::preload("boom.wav");
//    cocos2d::AudioEngine::preload("health.wav");
//    cocos2d::AudioEngine::preload("fire_truck.wav");
}

void AudioContent::setAudioVolume(int volume)
{
    audioVolume = volume;
}

void AudioContent::setEffectsVolume(int volume)
{
    effectsVolume = volume;
}

void AudioContent::play(AudioContent::Id contentId) const
{
    
    switch (contentId) {
        case Id::Background:
            cocos2d::AudioEngine::play2d("background.mp3", audioVolume);
            break;
            
        case Id::BombFail:
            cocos2d::AudioEngine::play2d("bombFail.wav", effectsVolume);
            break;
            
        case Id::BombRelease:
            cocos2d::AudioEngine::play2d("bombRelease.wav", effectsVolume);
            break;
            
        case Id::Boom:
            cocos2d::AudioEngine::play2d("boom.wav", effectsVolume);
            break;
            
        case Id::Health:
            cocos2d::AudioEngine::play2d("health.wav", effectsVolume);
            break;
            
        case Id::FireTruck:
            cocos2d::AudioEngine::play2d("fire_truck.wav", effectsVolume);
            break;
            
        default:
            break;
    }
}
