//
//  Tower.cpp
//  MyTowerDefense2D
//
//  Created by su xinde on 13-6-8.
//
//

#include "Tower.h"
#include "GameMediator.h"
#include "ProjectTile.h"

using namespace cocos2d;

Tower::~Tower(){
	
}

bool Tower::initWithFileAndRange(const char *pszFilename, int range){
	bool bRet = false;
	do
	{
		sprite = CCSprite::create(pszFilename);
		this->addChild(sprite, 10);
        
		sprite1 = CCSprite::create(pszFilename);
		this->addChild(sprite1);
		sprite1->setVisible(false);
		sprite1->setColor(ccBLUE);
        
		sprite2 = CCSprite::create(pszFilename);
		this->addChild(sprite2);
		sprite2->setVisible(false);
		sprite2->setColor(ccGREEN);
        
		sprite3 = CCSprite::create(pszFilename);
		this->addChild(sprite3);
		sprite3->setVisible(false);
		sprite3->setColor(ccBLACK);
        
		sprite4 = CCSprite::create(pszFilename);
		this->addChild(sprite4);
		sprite4->setVisible(false);
		sprite4->setColor(ccRED);
        
		this->setRange(range);
        
		_target = NULL;
        
		isShowing =false;
        
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);
        
		this->schedule(schedule_selector(Tower::towerLogic), 0.2f);
        
		bRet = true;
	} while (0);
	return bRet;
}

Enemy* Tower::getClosestTarget(){
	Enemy* closestEnemy = NULL;
	double maxDistant = 99999;
    
	GameMediator* m = GameMediator::sharedMediator();
	CCObject* temp;
	CCARRAY_FOREACH(m->getTargets(), temp){
		Enemy* enemy = (Enemy*)temp;
		if(enemy->getHP() <= 0){
			continue;
		}
		double curDistance = ccpDistance(this->getPosition(), enemy->getPosition());
		if(curDistance < maxDistant){
			closestEnemy = enemy;
			maxDistant = curDistance;
		}
	}
    
	if(maxDistant < this->getRange()){
		return closestEnemy;
	}
    
	return NULL;
}

void Tower::towerLogic(float dt){
	if(this->getTarget() == NULL ){
		this->setTarget(this->getClosestTarget());
	}else{
		double curDistance = ccpDistance(this->getPosition(), this->getTarget()->getPosition());
		if(this->getTarget()->getHP() <= 0 || curDistance > this->getRange()){
			this->setTarget(this->getClosestTarget());
		}
	}
    
	if(this->getTarget() != NULL){
		CCPoint shootVector = ccpSub(this->getTarget()->getPosition(), this->getPosition());
		float shootAngle = ccpToAngle(shootVector);
		float cocosAngle = CC_RADIANS_TO_DEGREES(-1 * shootAngle);
        
		float rotateSpeed = (float)(0.25 / M_PI);
		float rotateDuration = fabs(shootAngle * rotateSpeed);
		this->runAction(CCRotateTo::create(rotateDuration, cocosAngle));
	}
}

void Tower::show(){
	sprite1->runAction(CCMoveBy::create(0.5f, ccp(- sprite->getContentSize().width -10, 0)));
	sprite1->setVisible(true);
    
	sprite2->runAction(CCMoveBy::create(0.5f, ccp(0, sprite->getContentSize().height + 10)));
	sprite2->setVisible(true);
    
	sprite3->runAction(CCMoveBy::create(0.5f, ccp(sprite->getContentSize().width +10,0)));
	sprite3->setVisible(true);
    
	sprite4->runAction(CCMoveBy::create(0.5f, ccp(0, - sprite->getContentSize().height - 10)));
	sprite4->setVisible(true);
}

void Tower::unShow(){
	sprite1->runAction(CCSequence::create(CCMoveTo::create(0.5f, sprite->getPosition()), CCHide::create(), NULL));
    
	sprite2->runAction(CCSequence::create(CCMoveTo::create(0.5f, sprite->getPosition()), CCHide::create(), NULL));
    
	sprite3->runAction(CCSequence::create(CCMoveTo::create(0.5f, sprite->getPosition()), CCHide::create(), NULL));
    
	sprite4->runAction(CCSequence::create(CCMoveTo::create(0.5f, sprite->getPosition()), CCHide::create(), NULL));
	
}

bool Tower::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
	CCPoint touchLocation = this->convertTouchToNodeSpace(pTouch);
	if(CCRect::CCRectContainsPoint(sprite->boundingBox(), touchLocation)){
		if(isShowing == false){
			isShowing = true;
			show();
		}else{
			isShowing = false;
			unShow();
		}
		return true;
	}
	if(isShowing && CCRect::CCRectContainsPoint(sprite1->boundingBox(), touchLocation)){
		CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
		GameMediator::sharedMediator()->getGameLayer()->removeTower(this);
		return true;
	}
	if(isShowing && CCRect::CCRectContainsPoint(sprite2->boundingBox(), touchLocation)){
		sprite2->runAction(CCBlink::create(1.0f, 5));
		return true;
	}
	if(isShowing && CCRect::CCRectContainsPoint(sprite3->boundingBox(), touchLocation)){
		sprite3->runAction(CCSequence::create(CCFadeOut::create(0.5), CCFadeIn::create(0.5f), NULL));
		return true;
	}
	if(isShowing && CCRect::CCRectContainsPoint(sprite4->boundingBox(), touchLocation)){
		sprite4->runAction(CCRotateBy::create(1.0f, 360.0f));
		return true;
	}
    
	return false;
}

MachineGunTower* MachineGunTower::create(const char *pszFilename, int range){
	MachineGunTower* t = new MachineGunTower;
	if(t && t->initWithFileAndRange(pszFilename, range)){
		t->autorelease();
		return t;
	}
	CC_SAFE_DELETE(t);
	return NULL;
}

MachineGunTower* MachineGunTower::create(){
	return MachineGunTower::create("MachineGunTurret.png", 200);
}

bool MachineGunTower::initWithFileAndRange(const char *pszFilename, int range){
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!Tower::initWithFileAndRange(pszFilename, range));
        
		schedule(schedule_selector(MachineGunTower::fire), 0.5f);
        
		bRet = true;
	} while (0);
	return bRet;
}

void MachineGunTower::fire(float dt){
	if(this->getTarget() != NULL){
		GameMediator* m = GameMediator::sharedMediator();
        
		CCPoint shootVector = ccpSub(this->getTarget()->getPosition(), this->getPosition());
		CCPoint normalizedShootVector = ccpNormalize(shootVector);
		CCPoint overshotVector = ccpMult(normalizedShootVector, this->getRange());
		CCPoint offscreenPoint = ccpAdd(this->getPosition(), overshotVector);
        
		MachineProjectTile* ProjectTile = MachineProjectTile::create(offscreenPoint);
		ProjectTile->setPosition(this->getPosition());
		m->getGameLayer()->addChild(ProjectTile);
	}
}

FreezeTower* FreezeTower::create(const char *pszFilename, int range){
	FreezeTower* t = new FreezeTower;
	if(t && t->initWithFileAndRange(pszFilename, range)){
		t->autorelease();
		return t;
	}
	CC_SAFE_DELETE(t);
	return NULL;
}

FreezeTower* FreezeTower::create(){
	return FreezeTower::create("FreezeTurret.png", 150);
}

bool FreezeTower::initWithFileAndRange(const char *pszFilename, int range){
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!Tower::initWithFileAndRange(pszFilename, range));
        
		schedule(schedule_selector(FreezeTower::fire), 1.0f);
        
		bRet = true;
	} while (0);
	return bRet;
}

void FreezeTower::fire(float dt){
	if(this->getTarget() != NULL){
		GameMediator* m = GameMediator::sharedMediator();
        
		CCPoint shootVector = ccpSub(this->getTarget()->getPosition(), this->getPosition());
		CCPoint normalizedShootVector = ccpNormalize(shootVector);
		CCPoint overshotVector = ccpMult(normalizedShootVector, this->getRange());
		CCPoint offscreenPoint = ccpAdd(this->getPosition(), overshotVector);
        
		IceProjectTile* ProjectTile = IceProjectTile::create(offscreenPoint);
		ProjectTile->setPosition(this->getPosition());
		ProjectTile->setRotation(this->getRotation());
		m->getGameLayer()->addChild(ProjectTile);
	}
}

CannonTower* CannonTower::create(const char *pszFilename, int range){
	CannonTower* t = new CannonTower;
	if(t && t->initWithFileAndRange(pszFilename, range)){
		t->autorelease();
		return t;
	}
	CC_SAFE_DELETE(t);
	return NULL;
}

CannonTower* CannonTower::create(){
	return CannonTower::create("CannonTurret.png", 100);
}

bool CannonTower::initWithFileAndRange(const char *pszFilename, int range){
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!Tower::initWithFileAndRange(pszFilename, range));
        
		this->setRange(400);
        
		schedule(schedule_selector(CannonTower::fire), 2.0f);
        
		bRet = true;
	} while (0);
	return bRet;
}

void CannonTower::fire(float dt){
	if(this->getTarget() != NULL){
		GameMediator* m = GameMediator::sharedMediator();
        
		CannonProjectTile* ProjectTile = CannonProjectTile::create(this->getTarget());
		ProjectTile->setPosition(this->getPosition());
		ProjectTile->setRotation(this->getRotation());
		m->getGameLayer()->addChild(ProjectTile);
	}
}

Enemy* CannonTower::getClosestTarget(){
	Enemy* farthestEnemy = NULL;
    
	GameMediator* m = GameMediator::sharedMediator();
	CCObject* temp;
	CCARRAY_FOREACH(m->getTargets(), temp){
		Enemy* enemy = (Enemy*)temp;
		if(enemy->getHP() <= 0){
			continue;
		}
		float curDistance = ccpDistance(this->getPosition(), enemy->getPosition());
		if(curDistance < this->getRange()){
			farthestEnemy = enemy;
			break;
		}
	}
    
	return farthestEnemy;
}

