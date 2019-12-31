//
//  LaunchScreen.hpp
//  airhockey
//
//  Created by Sarah Smith on 28/Dec/19.
//  Copyright © 2019 Smithsoft. All rights reserved.
//

#pragma once

#include "cocos2d.h"

class LaunchScreen: public cocos2d::Layer
{
public:
    cocos2d::Sprite *_logo;
    
    virtual ~LaunchScreen();
    
    virtual bool init();
    
    CREATE_FUNC(LaunchScreen);
    
    static cocos2d::Scene *createScene();
    
    virtual void onTouchesBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
    
    void update(float dt);
    
private:
    cocos2d::Size _screenSize;
    bool _shouldDismiss;
    
    void setupTouchHandlers();
    void tearDownTouchHandlers();
    void loadMainScene();
};
