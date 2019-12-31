//
//  LaunchScreen.cpp
//  airhockey
//
//  Created by Sarah Smith on 28/Dec/19.
//  Copyright © 2019 Smithsoft. All rights reserved.
//

#include "LaunchScreen.h"
#include "GameLayer.h"

#include "cocos2d.h"

Scene* LaunchScreen::createScene()
{
    auto gameScene = Scene::create();
    auto gameLayer = LaunchScreen::create();
    gameScene->addChild(gameLayer);
    return gameScene;
}

LaunchScreen::~LaunchScreen()
{
    //
}

bool LaunchScreen::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    _shouldDismiss = false;
    
    _screenSize = Director::getInstance()->getWinSize();
    
    _logo = cocos2d::Sprite::create("logo.png");
    _logo->setPosition(_screenSize / 2);
    addChild(_logo);
    
    schedule(schedule_selector(LaunchScreen::update));
    setupTouchHandlers();
    
    return true;
}

void LaunchScreen::onTouchesBegan(const std::vector<Touch *> &touches, Event *event)
{
    // don't do transitions inside touch handlers - just set a flag and do outside
    // so we can tear down the touch handlers when flag is true.
    _shouldDismiss = true;
}

void LaunchScreen::update(float dt)
{
    if (_shouldDismiss)
    {
        _shouldDismiss = false;
        unscheduleAllCallbacks();
        tearDownTouchHandlers();
        loadMainScene();
    }
}

void LaunchScreen::setupTouchHandlers()
{
    using namespace std::placeholders;
    auto touchListener = EventListenerTouchAllAtOnce::create();
    touchListener->onTouchesBegan = std::bind(&LaunchScreen::onTouchesBegan,this, _1, _2);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void LaunchScreen::tearDownTouchHandlers()
{
    _eventDispatcher->removeAllEventListeners();
}

void LaunchScreen::loadMainScene()
{
    auto gameScene = cocos2d::Scene::create();
    auto gameLayer = GameLayer::create();
    gameScene->addChild(gameLayer);
    Director::getInstance()->replaceScene(TransitionFlipX::create(1, gameScene));
}
