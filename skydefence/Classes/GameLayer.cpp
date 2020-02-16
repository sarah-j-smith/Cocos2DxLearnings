/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

//
//  GameSprite.cpp
//  airhockey
//
//  Modifications & additions started by Sarah Smith on 29/Nov/19.
//  Copyright © 2019 Smithsoft. All rights reserved.
//

#include "GameLayer.h"

#include "AudioContent.h"

USING_NS_CC;

Scene* GameLayer::scene()
{
    auto gameScene = new cocos2d::Scene();
    auto gameLayer = GameLayer::create();
    gameScene->addChild(gameLayer);
    return gameScene;
}

GameLayer::~GameLayer()
{
    //
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GameLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    _screenSize = Director::getInstance()->getWinSize();
    
    _running = false;
    
    createGameScreen();
    
    createPools();
    
    createActions();

    schedule(CC_SCHEDULE_SELECTOR(GameLayer::update));
    
    AudioContent::getInstance().play(AudioContent::Id::Background);
    return true;
}

void GameLayer::onTouchesBegan(const std::vector<Touch *> &touches, Event *event)
{
//
}


void GameLayer::update(float dt)
{
    if (!_running) return;
    
    _meteorTimer += dt;
    if (_meteorTimer > _meteorInterval)
    {
        _meteorTimer = 0;
        resetMeteor();
    }
    
    _healthTimer += dt;
    if (_healthTimer > _healthInterval)
    {
        _healthTimer = 0;
        resetHealth();
    }
    
    _difficultyTimer += dt;
    if (_difficultyTimer > _difficultyInterval)
    {
        _difficultyTimer = 0;
        increaseDifficulty();
    }
}

void GameLayer::fallingObjectDone(cocos2d::Node *pSender)
{
    _fallingObjects.remove_if([pSender](const std::unique_ptr<Sprite> &p){ return p.get() == pSender; });
    pSender->stopAllActions();
    pSender->setRotation(0);
    
    if (pSender->getTag() == kSpriteMeteor)
    {
        _energy -= 15;
        auto groundHit = _groundHit->clone();
        pSender->runAction(groundHit);
        AudioContent::getInstance().play(AudioContent::Id::Boom);
    }
    else
    {
        // object is a health drop
        if (_energy == 100)
        {
            _score += 25;
            std::stringstream ss;
            ss << _score;
            _scoreDisplay->setString(ss.str());
        }
        else
        {
            _energy += 10;
            if (_energy > 100)
            {
                _energy = 100;
            }
        }
        AudioContent::getInstance().play(AudioContent::Id::Health);
    }
    // if energy is zero game over
    if (_energy <= 0)
    {
        _energy = 0;
        stopGame();
        AudioContent::getInstance().play(AudioContent::Id::FireTruck);
        
        _gameOverMessage->setVisible(true);
    }
    std::stringstream ss;
    ss << _energy << "%";
    _energyDisplay->setString(ss.str());
}

void GameLayer::animationDone(cocos2d::Node *pSender)
{
    pSender->setVisible(false);
}

void GameLayer::shockwaveDone()
{
    _shockWave->setVisible(false);
}

void GameLayer::resetMeteor() {
    //
}

void GameLayer::resetHealth()
{
    if (_fallingObjects.size() > 30) return;
    
    auto &health = _healthPool[_healthPoolIndex];
    _healthPoolIndex++;
    if (_healthPoolIndex == _healthPool.size())
    {
        _healthPoolIndex = 0;
    }
    
    int health_x = rand() % (int)(_screenSize.width * 0.8f) + _screenSize.width * 0.1f;
    int health_target_x = rand() % (int)(_screenSize.width * 0.8f) + _screenSize.width * 0.1f;
    
    auto healthBB = health->getBoundingBox();
    auto sz = healthBB.size;
    health->stopAllActions();
    health->setPosition(Vec2(health_x, _screenSize.height + sz.height * 0.5));
    
    auto f = std::bind(&GameLayer::fallingObjectDone, this, std::placeholders::_1);
    FiniteTimeAction *sequence = Sequence::create(
        MoveTo::create(_healthSpeed, Vec2(health_target_x, _screenSize.height * 0.15f)),
        CallFuncN::create(f), NULL);
    health->setVisible(true);
    health->runAction((cocos2d::Action *)_swingHealth->clone());
    health->runAction(sequence);
}


void GameLayer::createPools()
{
    
}

void GameLayer::createActions()
{
    
}

//if player is killed, stop all actions, clear screen
void GameLayer::stopGame()
{
    
}

void GameLayer::createGameScreen()
{
    //add bg
    auto bg = Sprite::create("bg.png");
    bg->setPosition(Vec2(_screenSize.width * 0.5f, _screenSize.height * 0.5f));
    addChild(bg);
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("sprite_sheet.plist");
    
    auto batchNode = SpriteBatchNode::create("sprite_sheet.png");
    _gameBatchNode = std::unique_ptr<cocos2d::SpriteBatchNode>(batchNode);
    
    Sprite *sprite;
    for (int i = 0; i < 2; ++i)
    {
        sprite = Sprite::createWithSpriteFrameName("city_dark.png");
        auto x = _screenSize.width * (0.25f + i * 0.5f);
        auto y = sprite->getBoundingBox().size.height  * 0.5f;
        sprite->setPosition(Vec2(x, y));
        _gameBatchNode->addChild(sprite, kForeground);
        
        sprite = Sprite::createWithSpriteFrameName("city_light.png");
        y = sprite->getBoundingBox().size.height  * 0.9f;
        sprite->setPosition(x, y);
        _gameBatchNode->addChild(sprite, kBackground);
    }
    for (int i = 0; i < 3; ++i)
    {
        sprite = Sprite::createWithSpriteFrameName("trees.png");
        auto x = _screenSize.width * (0.2f + i * 0.3f);
        auto y = sprite->getBoundingBox().size.height  * 0.5f;
        sprite->setPosition(Vec2(x, y));
        _gameBatchNode->addChild(sprite, kForeground);
    }
}

//this function is called at regular intervals
void GameLayer::increaseDifficulty ()
{
    _meteorInterval -= 0.2f;
    if (_meteorInterval < 0.25f) _meteorInterval = 0.25f;
    _meteorSpeed -= 1;
    if (_meteorSpeed < 4) _meteorSpeed = 4;
    _healthSpeed -= 1;
    if (_healthSpeed < 8) _healthSpeed = 8;
}


//start
void GameLayer::resetGame(void) {
    
}
