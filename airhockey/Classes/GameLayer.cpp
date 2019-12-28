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
#include "SimpleAudioEngine.h"

#include "GameSprite.h"

USING_NS_CC;

Scene* GameLayer::createScene()
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

void GameLayer::setupPlayers()
{
    _player1 = GameSprite::gameSpriteWithFile("mallet.png");
    _player2 = GameSprite::gameSpriteWithFile("mallet.png");
    _players = {
        { 0, Player::South, _player1, _player1ScoreLabel },
        { 0, Player::North, _player2, _player2ScoreLabel }
    };
}

void GameLayer::setupScoreLabels()
{
    _player1ScoreLabel = Label::createWithTTF("0", "arial.ttf", 60);
    _player1ScoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _player1ScoreLabel->setRotation(90);
    _player2ScoreLabel = Label::createWithTTF("0", "arial.ttf", 60);
    _player2ScoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _player2ScoreLabel->setRotation(90);
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

    setupScoreLabels();
    setupPlayers();
    
    const float labelOffset = 80.;
    Vec2 center = _screenSize / 2;
    Vec2 halfwayLineRight = Vec2(_screenSize.width, center.y);
    float paddleDiam = _player1->radius() * 2;

    auto court = Sprite::create("court.png");
    court->setPosition(center);
    addChild(court);
    
    _ball = GameSprite::gameSpriteWithFile("puck.png");
    auto ballPosition = Vec2(center.x, center.y - _ball->radius() * 2);
    _ball->setPosition(ballPosition);
    addChild(_ball);
    
    _player1->setPosition(Vec2(center.x, paddleDiam));
    addChild(_player1);
    
    _player2->setPosition(Vec2(center.x, _screenSize.height - paddleDiam));
    addChild(_player2);
    
    auto p1ScorePos = halfwayLineRight + Vec2(-20, -labelOffset);
    _player1ScoreLabel->setPosition(p1ScorePos);
    addChild(_player1ScoreLabel);

    auto p2ScorePos = halfwayLineRight + Vec2(-20, +labelOffset);
    _player2ScoreLabel->setPosition(p2ScorePos);
    addChild(_player2ScoreLabel);
    
    setupTouchHandlers();
    setupDebugShapes();
    
    schedule(schedule_selector(GameLayer::update));

    return true;
}

void GameLayer::setupDebugShapes()
{
    auto debugCircle = DrawNode::create();
    debugCircle->drawDot(Vec2::ZERO, 3.0, Color4F::RED);
    _player1ScoreLabel->addChild(debugCircle);
    
    auto debugBoundingBox = DrawNode::create();
    auto labelSize = _player1ScoreLabel->getContentSize();
    debugBoundingBox->drawRect(Vec2::ZERO, labelSize, Color4F::ORANGE);
    _player1ScoreLabel->addChild(debugBoundingBox);
}

void GameLayer::onTouchesBegan(const std::vector<Touch *> &touches, Event *event)
{
    for (auto touch : touches)
    {
        if (touch)
        {
            auto tap = touch->getLocation();
            for ( const Player &player : _players)
            {
                auto p = player.sprite;
                if (p->getBoundingBox().containsPoint(tap))
                {
                    p->setTouch(touch);
                }
            }
        }
    }
}

void GameLayer::onTouchesMoved(const std::vector<Touch *> &touches, Event *event)
{
    for (auto touch: touches)
    {
        auto tap = touch->getLocation();
        for (const Player &player : _players)
        {
            auto p = player.sprite;
            if (p->getTouch() != touch) continue;
            
            auto halfWayLine = _screenSize.height * 0.5f;
            auto paddleRadius = p->radius();
            auto courtMargin = Vec2(paddleRadius, paddleRadius);
            auto courtTopRight = (Vec2)_screenSize - courtMargin;
            auto northCourtBottomLeft = Vec2(paddleRadius, halfWayLine + paddleRadius);
            auto southCourtTopRight = Vec2(_screenSize.width - paddleRadius, halfWayLine - paddleRadius);
            Vec2 nextPosition = tap;
            
            // Keep the paddles inside their respective courts
            switch (player.side) {
                case Player::North:
                    nextPosition = nextPosition.getClampPoint(northCourtBottomLeft, courtTopRight);
                    break;
                    
                case Player::South:
                    nextPosition = nextPosition.getClampPoint(courtMargin, southCourtTopRight);
                    break;
            }
            p->setNextPosition(nextPosition);
            p->setVector(tap - p->getPosition());
        }
    }
}

void GameLayer::onTouchesEnded(const std::vector<Touch *> &touches, Event *event)
{
    for (auto touch: touches)
    {
        for (const Player &player : _players)
        {
            auto p = player.sprite;
            if (p->getTouch() != nullptr && p->getTouch() == touch)
            {
                p->setTouch(nullptr);
                p->setVector(Vec2());
            }
        }
    }
}

void GameLayer::onTouchesCancelled(const std::vector<Touch *> &touches, Event *event)
{
    for (auto touch: touches)
    {
        for (const Player &player: _players)
        {
            auto p = player.sprite;
            if (p->getTouch() != nullptr && p->getTouch() == touch)
            {
                p->setTouch(nullptr);
                p->setVector(Vec2());
            }
        }
    }
}

void GameLayer::update(float dt)
{
    auto ballVector = _ball->getVector() * 0.98;
    auto ballNextPosition = _ball->getNextPosition() + ballVector;
    
    Point playerNextPosition;
    Point playerVector;
    
    float squared_radii = pow(_player1->radius() + _ball->radius(), 2);
    for (const Player &player: _players)
    {
        auto p = player.sprite;
        playerNextPosition = p->getNextPosition();
        playerVector = p->getVector();
        
        auto deltaBallNextPlayer = ballNextPosition - p->getPosition();
        auto d1 = deltaBallNextPlayer.lengthSquared();
        auto deltaBallPlayerNext = _ball->getPosition() - playerNextPosition;
        auto d2 = deltaBallPlayerNext.lengthSquared();
        
        if (d1 <= squared_radii || d2 <= squared_radii)
        {
            float mag_ball = ballVector.lengthSquared();
            float mag_player = playerVector.lengthSquared();
            float force = sqrt(mag_ball + mag_player);
            float angle = atan2(deltaBallNextPlayer.y, deltaBallNextPlayer.x);
            ballVector.x = force * cos(angle);
            ballVector.y = force * sin(angle);
            ballNextPosition.x = playerNextPosition.x + (p->radius() + _ball->radius() + force) * cos(angle);
            ballNextPosition.y = playerNextPosition.y + (p->radius() + _ball->radius() + force) * sin(angle);
            playHit();
        }
        
        if (ballNextPosition.x < _ball->radius()) {
            ballNextPosition.x = _ball->radius();
            ballVector.x *= -0.8f;
            playHit();
        }
        
        if (ballNextPosition.x > _screenSize.width - _ball->radius())
        {
            ballNextPosition.x = _screenSize.width - _ball->radius();
            ballVector.x *= -0.8f;
            playHit();
        }
        
        if (ballNextPosition.y > _screenSize.height - _ball->radius())
        {
            if (!isBallInsideGoal())
            {
                ballNextPosition.y = _screenSize.height - _ball->radius();
                ballVector.y *= -0.8f;
                playHit();
            }
        }
        
        if (ballNextPosition.y < _ball->radius())
        {
            if (!isBallInsideGoal())
            {
                ballNextPosition.y = _ball->radius();
                ballVector.y *= -0.8f;
                playHit();
            }
        }
        
        _ball->setVector(ballVector);
        _ball->setNextPosition(ballNextPosition);
        
        // If the ball has completely gone off the court to the south then
        // the North player has scored.
        if (ballNextPosition.y < -_ball->radius() * 2)
        {
            playerScore(Player::North);
        }
        
        if (ballNextPosition.y > _screenSize.height + _ball->radius() * 2)
        {
            playerScore(Player::South);
        }
        
        _player1->setPosition(_player1->getNextPosition());
        _player2->setPosition(_player2->getNextPosition());
        _ball->setPosition(_ball->getNextPosition());
    }
}

bool GameLayer::isBallInsideGoal()
{
    if (_ball->getPosition().x < (_screenSize.width - GOAL_WIDTH) * 0.5f ||
        _ball->getPosition().x > (_screenSize.width + GOAL_WIDTH) * 0.5f)
    {
        return false;
    }
    return true;
}

void GameLayer::playHit()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("hit.wav");
}

void GameLayer::playerScore(Player::Side side)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("score.wav");
    _ball->setVector(Vec2::ZERO);
    char score_buffer[10];

    auto player = std::find_if(_players.begin(), _players.end(), [side](Player &p)->bool { return p.side == side; });
    player->score += 1;
    snprintf(score_buffer, 9, "%i", player->score);
    player->label->setString(score_buffer);
    
    Vec2 center = _screenSize / 2;
    float ballOffset = (side == Player::South ? _ball->radius() * 2 : _ball->radius() * -2);
    _ball->setNextPosition(center + Vec2(0, ballOffset));

    resetPlayers();
}

void GameLayer::resetPlayers()
{
    Vec2 center = _screenSize / 2;
    float paddleDiam = _player1->radius() * 2;
    _player1->setPosition(Vec2(center.x, paddleDiam));
    _player1->setTouch(nullptr);
    _player2->setPosition(Vec2(center.x, _screenSize.height - paddleDiam));
    _player2->setTouch(nullptr);
}

void GameLayer::setupTouchHandlers()
{
    using namespace std::placeholders;
    auto touchListener = EventListenerTouchAllAtOnce::create();
    touchListener->onTouchesBegan = std::bind(&GameLayer::onTouchesBegan,this, _1, _2);
    touchListener->onTouchesMoved = std::bind(&GameLayer::onTouchesMoved, this, _1, _2);
    touchListener->onTouchesEnded = std::bind(&GameLayer::onTouchesEnded, this, _1, _2);
    touchListener->onTouchesCancelled = std::bind(&GameLayer::onTouchesCancelled, this, _1, _2);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}
