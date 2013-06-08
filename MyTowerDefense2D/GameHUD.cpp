//
//  GameHUD.cpp
//  MyTowerDefense2D
//
//  Created by su xinde on 13-6-8.
//
//

#include "GameHUD.h"
#include "GameMediator.h"

using namespace cocos2d;

int waveCount;
static GameHUD _sharedHUD;

GameHUD* GameHUD::sharedHUD(){
	static bool s_bFirstUseHUd= true;
	if(s_bFirstUseHUd){
		s_bFirstUseHUd = false;
		_sharedHUD.init();
	}
	return &_sharedHUD;
}

GameHUD::~GameHUD(){
	CC_SAFE_RELEASE_NULL(movableSprites);
}

bool GameHUD::init(){
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!CCLayer::init());
        
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        
		CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGB565);
		background = CCSprite::create("hud.png");
		background->setAnchorPoint(CCPointZero);
		this->addChild(background);
		CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_Default);
        
		movableSprites = CCArray::create();
		movableSprites->retain();
        
		selSprite = NULL;
		selSpriteRange = NULL;
        
		CCArray* images = CCArray::create(CCString::create("MachineGunTurret.png"), CCString::create("FreezeTurret.png"),
                                          CCString::create("CannonTurret.png"), NULL);
        
		for(unsigned int i = 0; i < images->count(); i++){
			CCString* image = (CCString*)images->objectAtIndex(i);
			CCSprite* sprite = CCSprite::create(image->getCString());
			float offsetFraction = ((float)(i+1))/(images->count()+1);
			sprite->setPosition(ccp(winSize.width * offsetFraction, 35));
			sprite->setTag(i + 1);
			this->addChild(sprite);
			movableSprites->addObject(sprite);
            
			CCLabelTTF* towerCost = CCLabelTTF::create("$", "Marker Felt", 10);
			towerCost->setPosition(ccp(winSize.width * offsetFraction, 15));
			towerCost->setColor(ccc3(0, 0, 0));
			this->addChild(towerCost, 1);
            
			switch(i){
                case 0:
                    towerCost->setString("25");
                    break;
                case 1:
                    towerCost->setString("35");
                    break;
                case 2:
                    towerCost->setString("25");
                    break;
                case 3:
                    towerCost->setString("25");
                    break;
                default:
                    break;
			}
		}
        
		resources = 300;
		resourceLabel = CCLabelTTF::create("Money 300", "Marker Felt", 20, CCSizeMake(150, 25), kCCTextAlignmentRight);
		resourceLabel->setPosition(ccp(30, winSize.height -15));
		resourceLabel->setColor(ccc3(255, 80, 20));
		this->addChild(resourceLabel, 1);
        
		CCLabelTTF* baseHpLabel = CCLabelTTF::create("Base Health", "Marker Felt", 20, CCSizeMake(150, 25), kCCTextAlignmentRight);
		baseHpLabel->setPosition(ccp(winSize.width - 185, winSize.height -15));
		resourceLabel->setColor(ccc3(255, 80, 20));
		this->addChild(baseHpLabel, 1);
        
		waveCount = 1;
		waveCountLabel = CCLabelTTF::create("Wave 1", "Marker Felt", 20, CCSizeMake(150, 25), kCCTextAlignmentRight);
		waveCountLabel->setPosition(ccp(winSize.width * 0.5f - 80, winSize.height - 15));
		waveCountLabel->setColor(ccc3(100, 0, 100));
		this->addChild(waveCountLabel, 1);
        
		baseHpPercentage = 100;
		healthBar = CCProgressTimer::create(CCSprite::create("health_bar_green.png"));
		healthBar->setType(kCCProgressTimerTypeBar);
		healthBar->setMidpoint(ccp(0, 0));
		healthBar->setBarChangeRate(ccp(1, 0));
		healthBar->setPercentage(baseHpPercentage);
		healthBar->setScale(0.5f);
		healthBar->setPosition(ccp(winSize.width -55, winSize.height -15));
		this->addChild(healthBar);
        
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0 , true);
        
		bRet = true;
	} while (0);
	return bRet;
}

bool GameHUD::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
	CCPoint touchLocation = this->convertTouchToNodeSpace(pTouch);
	if(!CCRect::CCRectContainsPoint(background->boundingBox(), touchLocation)){
		return false;
	}
    
	CCSprite* newSprite = NULL;
	CCObject* temp;
	CCARRAY_FOREACH(movableSprites, temp){
		CCSprite* sprite = (CCSprite*)temp;
		if(CCRect::CCRectContainsPoint(sprite->boundingBox(), touchLocation)){
            
			selSpriteRange = CCSprite::create("Range.png");
			selSpriteRange->setScale(2.0f);
			this->addChild(selSpriteRange, -1);
			selSpriteRange->setPosition(sprite->getPosition());
            
			newSprite = CCSprite::createWithTexture(sprite->getTexture());
			newSprite->setPosition(ccpAdd(sprite->getPosition(), ccp(0, 30)));
			selSprite = newSprite;
			selSprite->setTag(sprite->getTag());
			this->addChild(newSprite);
			break;
		}
	}
	return true;
}

void GameHUD::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
	CCPoint touchLocation = this->convertTouchToNodeSpace(pTouch);
    
	CCPoint oldTouchLocation = pTouch->previousLocationInView();
	oldTouchLocation = CCDirector::sharedDirector()->convertToGL(oldTouchLocation);
	oldTouchLocation = this->convertToNodeSpace(oldTouchLocation);
    
	CCPoint translation = ccpSub(touchLocation, oldTouchLocation);
	if(selSprite){
		CCPoint newPos = ccpAdd(selSprite->getPosition(), translation);
		selSprite->setPosition(newPos);
		selSpriteRange->setPosition(newPos);
        
		GameMediator* m = GameMediator::sharedMediator();
		CCPoint touchLocationInGameLayer = m->getGameLayer()->convertTouchToNodeSpace(pTouch);
        
		bool isBuildable = m->getGameLayer()->canBuildOnTilePosition(touchLocationInGameLayer);
		if(isBuildable){
			selSprite->setOpacity(200);
		}else{
			selSprite->setOpacity(50);
		}
	}
}

void GameHUD::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
	CCPoint touchLocation = this->convertTouchToNodeSpace(pTouch);
    
	GameMediator* m = GameMediator::sharedMediator();
	if(selSprite){
		CCRect backgroundRect = CCRectMake(background->getPosition().x, background->getPosition().y,
                                           background->getContentSize().width, background->getContentSize().height);
        
		if(!CCRect::CCRectContainsPoint(backgroundRect, touchLocation)){
			CCPoint touchLocationInGameLayer = m->getGameLayer()->convertTouchToNodeSpace(pTouch);
			m->getGameLayer()->addTower(touchLocationInGameLayer, selSprite->getTag());
		}
		this->removeChild(selSprite, true);
		selSprite = NULL;
		this->removeChild(selSpriteRange, true);
		selSpriteRange = NULL;
	}
}

void GameHUD::updateBaseHp(int amount){
	baseHpPercentage += amount;
    
	if(baseHpPercentage <= 25){
		healthBar->setSprite(CCSprite::create("health_bar_red.png"));
		healthBar->setScale(0.5f);
	}
    
	if(baseHpPercentage <=0){
		CCLog("Game Over");
	}
    
	healthBar->setPercentage(baseHpPercentage);
}

void GameHUD::updateResources(int amount){
	resources += amount;
	resourceLabel->setString(CCString::createWithFormat("Money %d",resources)->getCString());
}

void GameHUD::updateResourcesNom(){
	resources += 1;
	resourceLabel->setString(CCString::createWithFormat("Money %d",resources)->getCString());
}

void GameHUD::updateWaveCount(){
	waveCount ++;
	waveCountLabel->setString(CCString::createWithFormat("Wave %d",waveCount)->getCString());
}


