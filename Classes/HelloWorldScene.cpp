/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 Copyright (c) 2021 Bytedance Inc.

 https://axmolengine.github.io/

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

#include "HelloWorldScene.h"
#include "PhysicsShapeCache.h"

USING_NS_AX;

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf(
        "Depending on how you compiled you might have to add 'Resources/' in front of filenames in "
        "HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = _director->getVisibleSize();
    auto origin      = _director->getVisibleOrigin();
    auto safeArea    = _director->getSafeAreaRect();
    auto safeOrigin  = safeArea.origin;

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
                                           AX_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr || closeItem->getContentSize().width <= 0 || closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = safeOrigin.x + safeArea.size.width - closeItem->getContentSize().width / 2;
        float y = safeOrigin.y + closeItem->getContentSize().height / 2;
        closeItem->setPosition(Vec2(x, y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    initPhysicsWorld();
    
    getPhysicsWorld()->setGravity(Vec2(0, -900));
   
    // optional: enable debug draw
//    getPhysicsWorld()->setDebugDrawMask(0xffff);
    
    // Load the sprite sheet
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("spritesheet.plist");
    
    // Load shapes
    PhysicsShapeCache::getInstance()->addShapesWithFile("shapes.plist");
        
    // Add background image
    Sprite *background = Sprite::createWithSpriteFrameName("background.png");
    Vec2 pos = origin + visibleSize/2;
    background->setPosition(pos);
    addChild(background);
    
    // Set the ground
    spawnSprite("ground.png", origin);
    
    // and add a physics sprite
    spawnSprite("banana.png", Vec2(600,800));
 
    // Add touch listener
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = AX_CALLBACK_2(HelloWorld::onTouchesBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    // scheduleUpdate() is required to ensure update(float) is called on every loop
    scheduleUpdate();

    return true; 
}

void HelloWorld::spawnSprite(const std::string &name, const Vec2 &pos)
{
    // create a sprite with the given image name
    auto sprite = Sprite::createWithSpriteFrameName(name);

    // attach physics body
    PhysicsShapeCache::getInstance()->setBodyOnSprite(name, sprite);

    // set position and add it to the scene
    sprite->setPosition(pos);
    addChild(sprite);
}

bool HelloWorld::onTouchesBegan(Touch *touch, Event *event)
{
    auto touchLoc = touch->getLocation();

    static std::vector<std::string> sprites {"banana.png", "cherries.png", "crate.png", "orange.png"};
    
    int i = rand() % sprites.size();
    spawnSprite(sprites[i], touchLoc);

    return false;
}

void HelloWorld::update(float delta)
{
    switch (_gameState)
    {
    case ExampleGameState::init:
        _gameState = ExampleGameState::update;
        break;

    case ExampleGameState::update:
        /////////////////////////////
        // Add your codes below...like....
        // 
        // UpdateJoyStick();
        // UpdatePlayer();
        // UpdatePhysics();
        // ...
        break;

    case ExampleGameState::pause:
        /////////////////////////////
        // Add your codes below...like....
        //
        // anyPauseStuff()
        break;

    case ExampleGameState::menu1:
        /////////////////////////////
        // Add your codes below...like....
        // 
        // UpdateMenu1();
        break;

    case ExampleGameState::menu2:
        /////////////////////////////
        // Add your codes below...like....
        // 
        // UpdateMenu2();
        break;

    case ExampleGameState::end:
        /////////////////////////////
        // Add your codes below...like....
        // 
        // CleanUpMyCrap();
        menuCloseCallback(this);
        break;
    }
}

void HelloWorld::menuCloseCallback(Ref* sender)
{
    // Close the axmol game scene and quit the application
    _director->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use
     * _director->end() as given above,instead trigger a custom event created in RootViewController.mm
     * as below*/

    // EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}
