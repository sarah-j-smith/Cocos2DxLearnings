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

#pragma once

#include "cocos2d.h"

enum {
    kSpriteBomb,
    kSpriteShockwave,
    kSpriteMeteor,
    kSpriteHealth,
    kSpriteHalo,
    kSpriteSparkle
};

enum {
    kBackground,
    kMiddleground,
    kForeground
};

using SpriteList = std::vector<std::unique_ptr<cocos2d::Sprite>>;

class GameLayer : public cocos2d::Layer
{
public:


    virtual ~GameLayer();
    
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameLayer);
    
    static cocos2d::Scene *scene();
    
    virtual void onTouchesBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
    
    void update(float dt);
    
    void fallingObjectDone(cocos2d::Node* pSender);
    void animationDone(cocos2d::Node* pSender);
    void shockwaveDone();
    
private:
    
    void setupDebugShapes();
    
    std::vector<std::unique_ptr<cocos2d::Sprite>> _meteorPool;
    int _meteorPoolIndex;
    
    std::vector<std::unique_ptr<cocos2d::Sprite>> _healthPool;
    int _healthPoolIndex;
    
    std::list<std::unique_ptr<cocos2d::Sprite>> _fallingObjects;
    std::vector<std::unique_ptr<cocos2d::Sprite>> _clouds;
    
    std::unique_ptr<cocos2d::SpriteBatchNode> _gameBatchNode;
    
    cocos2d::Sprite * _bomb;
    cocos2d::Sprite * _shockWave;
    
    cocos2d::Sprite * _introMessage;
    cocos2d::Sprite * _gameOverMessage;
    
    cocos2d::Label * _energyDisplay;
    cocos2d::Label * _scoreDisplay;
    
    cocos2d::Action * _growBomb;
    cocos2d::Action * _rotateSprite;
    cocos2d::Action * _shockwaveSequence;
    cocos2d::Action * _swingHealth;
    
    cocos2d::Action * _groundHit;
    cocos2d::Action * _explosion;
    
    
    cocos2d::Size _screenSize;
    
    float _meteorInterval;
    float _meteorTimer;
    float _meteorSpeed;
    float _healthInterval;
    float _healthTimer;
    float _healthSpeed;
    float _difficultyInterval;
    float _difficultyTimer;
    
    int _energy;
    int _score;
    int _shockwaveHits;
    bool _running;
    
    void resetMeteor(void);
    void resetHealth(void);
    void resetGame(void);
    void stopGame(void);
    void increaseDifficulty(void);
    
    void createGameScreen (void);
    void createPools(void);
    void createActions(void);
};
