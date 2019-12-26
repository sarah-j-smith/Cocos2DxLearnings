//
//  GameSprite.h
//  airhockey
//
//  Created by Sarah Smith on 29/Nov/19.
//  Copyright © 2019 Smithsoft. All rights reserved.
//

#pragma once

#include "cocos2d.h"

USING_NS_CC;

class GameSprite : public Sprite
{
public:

    CC_SYNTHESIZE(Point, _nextPosition, NextPosition);
    CC_SYNTHESIZE(Point, _vector, Vector);
    CC_SYNTHESIZE(Touch *, _touch, Touch);
    
    GameSprite(void);
    ~GameSprite(void);
    
    static GameSprite* gameSpriteWithFile(const char *fileName);
    
    virtual void setPosition(const Point &pos);
    
    float radius();
};

using GameSpritePtr = GameSprite* ;
using GameSpriteList = cocos2d::Vector<GameSpritePtr> ;

