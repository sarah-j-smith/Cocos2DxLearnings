//
//  GameSprite.cpp
//  airhockey
//
//  Created by Sarah Smith on 29/Nov/19.
//  Copyright © 2019 Smithsoft. All rights reserved.
//

#include "GameSprite.h"

GameSprite::GameSprite()
{
    _vector = Vec2(0, 0);
}

GameSprite::~GameSprite()
{
    //
}

GameSprite* GameSprite::gameSpriteWithFile(const char *fileName)
{
    GameSprite *sprite = new GameSprite();
    if (sprite && sprite->initWithFile(fileName))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void GameSprite::setPosition(const Point &pos)
{
    Sprite::setPosition(pos);
    if (!_nextPosition.equals(pos))
    {
        _nextPosition = pos;
    }
}

float GameSprite::radius()
{
    return getTexture()->getContentSize().width * 0.5f;
}
