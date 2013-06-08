//
//  ProjectTile.cpp
//  MyTowerDefense2D
//
//  Created by su xinde on 13-6-8.
//
//

#include "ProjectTile.h"
#include "GameMediator.h"
#include "Enemy.h"

using namespace cocos2d;

bool ProjectTile::initWithFile(const char* filename){
    
	mySprite = CCSprite::create(filename);
	this->addChild(mySprite);
    
	speed = 960.0f;
	targetPos = CCPointZero;
	damage = 3;
    
	gm = GameMediator::sharedMediator();
    
	hasRemoved = false;
    
	scheduleUpdate();
    
	return true;
}

void ProjectTile::removeSelf(){
	if(hasRemoved)
		return;
    
	hasRemoved = true;
    
	unscheduleAllSelectors();
	this->stopAllActions();
	this->removeFromParentAndCleanup(true);
}

CCRect ProjectTile::getRect(){
	CCRect rect = CCRectMake(this->getPosition().x - mySprite->getContentSize().width * 0.5f,
                             this->getPosition().y - mySprite->getContentSize().height* 0.5f,
                             mySprite->getContentSize().width,
                             mySprite->getContentSize().height);
	return rect;
}

MachineProjectTile* MachineProjectTile::create(cocos2d:: CCPoint pos){
	MachineProjectTile* m = new MachineProjectTile;
	if(m && m->initWithTargetPos(pos)){
		m->autorelease();
		return m;
	}
	CC_SAFE_DELETE(m);
	return NULL;
}

bool MachineProjectTile::initWithTargetPos(cocos2d::CCPoint pos){
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!initWithFile("bullet1.png"));
        
		this->setTargetPos(pos);
        
		moveToTargetPos();
        
		bRet = true;
	} while (0);
	return bRet;
}

void MachineProjectTile::update(float dt){
	//GameMediator* gm = GameMediator::sharedMediator();
    
	CCObject* temp;
	CCARRAY_FOREACH(gm->getTargets(), temp){
		Enemy* target = (Enemy*)temp;
        
		if(CCRect::CCRectIntersectsRect(this->getRect(), target->getRect()) && target->getHP() > 0){
			target->setHP(target->getHP() - this->getDamage());
			removeSelf();
			break;
		}
	}
    
}

void MachineProjectTile::moveToTargetPos(){
	float distance = ccpDistance(this->getPosition(), this->getTargetPos());
	float moveDur = distance / speed;
	auto moveTo = CCMoveTo::create(moveDur, this->getTargetPos());
	auto moveDone = CCCallFunc::create(this, callfunc_selector(MachineProjectTile::removeSelf));
	this->runAction(CCSequence::create(moveTo, moveDone, NULL));
}

IceProjectTile* IceProjectTile::create(cocos2d:: CCPoint pos){
	IceProjectTile* m = new IceProjectTile;
	if(m && m->initWithTargetPos(pos)){
		m->autorelease();
		return m;
	}
	CC_SAFE_DELETE(m);
	return NULL;
}

bool IceProjectTile::initWithTargetPos(cocos2d::CCPoint pos){
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!initWithFile("bullet2.png"));
        
		this->setTargetPos(pos);
        
		moveToTargetPos();
        
		bRet = true;
	} while (0);
	return bRet;
}

void IceProjectTile::update(float dt){
	//GameMediator* gm = GameMediator::sharedMediator();
    
	CCObject* temp;
	CCARRAY_FOREACH(gm->getTargets(), temp){
		Enemy* target = (Enemy*)temp;
        
		if(CCRect::CCRectIntersectsRect(this->getRect(), target->getRect())){
			target->changeSpeed();
			removeSelf();
			break;
		}
	}
    
}

void IceProjectTile::moveToTargetPos(){
	float distance = ccpDistance(this->getPosition(), this->getTargetPos());
	float moveDur = distance / speed;
	auto moveTo = CCMoveTo::create(moveDur, this->getTargetPos());
	auto moveDone = CCCallFunc::create(this, callfunc_selector(IceProjectTile::removeSelf));
	this->runAction(CCSequence::create(moveTo, moveDone, NULL));
}

CannonProjectTile* CannonProjectTile::create(Enemy* enemy){
	CannonProjectTile* cp = new CannonProjectTile;
	if(cp && cp->initWithTarget(enemy)){
		cp->autorelease();
		return cp;
	}
	CC_SAFE_DELETE(cp);
	return NULL;
}

bool CannonProjectTile::initWithTarget(Enemy* enemy){
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!initWithFile("rocket.png"));
        
		myEnemy = enemy;
		this->setDamage(10);
		this->setSpeed(120);
        
		angularVelocity = 5.0f;
        
		bRet = true;
	} while (0);
	return bRet;
}

void CannonProjectTile::update(float dt){
	//GameMediator* gm = GameMediator::sharedMediator();
    
	CCPoint targetPos = myEnemy->getPosition();
	CCPoint myPos = this->getPosition();
    
	if(gm->getGameLayer()->isOutOfMap(targetPos) || myEnemy->getHP() <=0){
		removeSelf();
	}
    
	if(CCRect::CCRectIntersectsRect(this->getRect(), myEnemy->getRect()) && myEnemy->getHP() > 0){
		myEnemy->setHP(myEnemy->getHP() - this->getDamage());
		
		removeSelf();
	}
    
	float radian = atan2f(targetPos.y - myPos.y, targetPos.x - myPos.x);
	float angle = -CC_RADIANS_TO_DEGREES(radian);
	angle = to360Angle(angle);
    
	float myAngle = to360Angle(this->getRotation());
	float tempAngle;
	if(myAngle < angle){
		tempAngle = angularVelocity;
	}else if(myAngle >angle){
		tempAngle = -angularVelocity;
	}else{
		tempAngle = 0;
	}
    
	if(360 - abs(angle - myAngle) < abs(angle - myAngle)){
		tempAngle *= -1;
	}
    
	this->setRotation(this->getRotation() + tempAngle);
	float myRadian = CC_DEGREES_TO_RADIANS(this->getRotation());
    
	float x = cosf(myRadian) * this->getSpeed() * dt;
	float y = -sinf(myRadian) * this->getSpeed() * dt;
    
	this->setPosition(ccp(myPos.x + x, myPos.y + y));
}

