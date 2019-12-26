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

#include "GameLayer.h"
#include "SimpleAudioEngine.h"

#include "GameSprite.h"

#include <memory>

#include <iostream>

USING_NS_CC;

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

    _player1Score = 0;
    _player2Score = 0;
    
    _screenSize = Director::getInstance()->getWinSize();

    auto court = Sprite::create("court.png");
    _player1 = GameSprite::gameSpriteWithFile("mallet.png");
    _player2 = GameSprite::gameSpriteWithFile("mallet.png");
    _ball = GameSprite::gameSpriteWithFile("puck.png");
    
    _player1ScoreLabel = Label::createWithTTF("0", "arial.ttf", 30);
    _player1ScoreLabel->setAnchorPoint(Vec2(0.5, 1.0));
    _player1ScoreLabel->setRotation(90);
    _player2ScoreLabel = Label::createWithTTF("0", "arial.ttf", 30);
    _player2ScoreLabel->setAnchorPoint(Vec2(0.5, 1.0));
    _player2ScoreLabel->setRotation(90);

    auto l = new GameSpriteList { _player1, _player2 };
    _players = std::unique_ptr<GameSpriteList>(l);
    
    const float labelOffset = 30.0f;
    Vec2 center = _screenSize / 2;
    Vec2 halfwayLineRight = Vec2(_screenSize.width, center.y);
    float paddleDiam = _player1->radius() * 2;
    float ballDiam = _ball->radius() * 2;

    court->setPosition(center);
    addChild(court);
    
    _player1->setPosition(Vec2(center.x, paddleDiam));
    addChild(_player1);
    
    _player2->setPosition(Vec2(center.x, _screenSize.height - paddleDiam));
    addChild(_player2);
    
    _ball->setPosition(Vec2(center.x, center.y - ballDiam));
    addChild(_ball);
    
    auto p1ScorePos = halfwayLineRight + Vec2(0, -labelOffset);
    _player1ScoreLabel->setPosition(p1ScorePos);
    addChild(_player1ScoreLabel);

    // Move the co-ord system for rendering the Draw commands that are executed by the
    // draw node to be the same position as the label, and then draw a large dot there.
    auto debugCircle = DrawNode::create();
    debugCircle->drawDot(Vec2::ZERO, 3.0, Color4F::RED);
    _player1ScoreLabel->addChild(debugCircle);
    
    auto debugBoundingBox = DrawNode::create();
    auto labelSize = _player1ScoreLabel->getContentSize();
    debugBoundingBox->drawRect(Vec2::ZERO, labelSize, Color4F::ORANGE);
    _player1ScoreLabel->addChild(debugBoundingBox);

    auto p2ScorePos = halfwayLineRight + Vec2(0, +labelOffset);
    _player2ScoreLabel->setPosition(p2ScorePos);
    addChild(_player2ScoreLabel);
    
    setupTouchHandlers();
    
    schedule(schedule_selector(GameLayer::update));

    return true;
}

void GameLayer::onTouchesBegan(const std::vector<Touch *> &touches, Event *event)
{
    for (auto touch : touches)
    {
        if (touch)
        {
            auto tap = touch->getLocation();
            for ( auto p : *_players)
            {
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
        std::cout << "Tap: " << tap.x << ", " << tap.y << std::endl;
        for (auto p : *_players)
        {
            if (p->getTouch() == touch)
            {
                // Keep puck within the court
                auto minPlayer = Vec2(p->radius(), p->radius());
                auto maxPlayer = Vec2(_screenSize.width - p->radius(), _screenSize.height - p->radius());
                auto nextPosition = tap.getClampPoint(minPlayer, maxPlayer);

                // Keep puck in its own half of the court
                auto halfWayLine = _screenSize.height * 0.5f;
                auto lowerHalfBound = halfWayLine - p->radius();
                auto upperHalfBound = halfWayLine + p->radius();
                if (p->getPositionY() < halfWayLine)
                {
                    if (nextPosition.y > lowerHalfBound)
                    {
                        nextPosition.y = lowerHalfBound;
                    }
                } else {
                    if (nextPosition.y < upperHalfBound)
                    {
                        nextPosition.y = upperHalfBound;
                    }
                }
                
                p->setNextPosition(nextPosition);
                p->setVector(tap - p->getPosition());
            }
        }
    }
}

void GameLayer::onTouchesEnded(const std::vector<Touch *> &touches, Event *event)
{
    for (auto touch: touches)
    {
        for (auto p : *_players)
        {
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
        for (auto p : *_players)
        {
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
    for (auto p: *_players)
    {
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
            float angle = atan2(deltaBallNextPlayer.x, deltaBallNextPlayer.y);
            ballVector.x = force * cos(angle);
            ballVector.y = force * sin(angle);
            ballNextPosition.x = playerNextPosition.x + (p->radius() + _ball->radius() + force) * cos(angle);
            ballNextPosition.y = playerNextPosition.y + (p->radius() + _ball->radius() + force) * sin(angle);
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("hit.wav");
        }
        
        _player1->setPosition(_player1->getNextPosition());
        _player2->setPosition(_player2->getNextPosition());
        _ball->setPosition(_ball->getNextPosition());
    }
}

void GameLayer::playerScore(int player)
{
    //
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

