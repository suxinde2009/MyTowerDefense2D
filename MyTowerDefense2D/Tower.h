//
//  Tower.h
//  MyTowerDefense2D
//
//  Created by su xinde on 13-6-8.
//
//

#ifndef __MyTowerDefense2D__Tower__
#define __MyTowerDefense2D__Tower__

#include "cocos2d.h"
#include "Enemy.h"

class Tower : public cocos2d::CCNode, public cocos2d::CCTouchDelegate {
public:
	virtual ~Tower();
	virtual bool initWithFileAndRange(const char *pszFilename, int range);
    
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
	CC_SYNTHESIZE(int, _range,  Range);
    
	CC_SYNTHESIZE_RETAIN(Enemy*, _target, Target);
    
protected:
	cocos2d::CCSprite* sprite;
    
	cocos2d::CCSprite* sprite1;
	cocos2d::CCSprite* sprite2;
	cocos2d::CCSprite* sprite3;
	cocos2d::CCSprite* sprite4;
    
	virtual Enemy* getClosestTarget();
    
	void towerLogic(float dt);
    
	bool isShowing;
	void show();
	void unShow();
    
};

class MachineGunTower : public Tower {
public:
	bool initWithFileAndRange(const char *pszFilename, int range);
	void fire(float dt);
    
	static MachineGunTower* create();
	static MachineGunTower* create(const char *pszFilename, int range);
};

class FreezeTower : public Tower {
public:
	bool initWithFileAndRange(const char *pszFilename, int range);
	void fire(float dt);
    
	static FreezeTower* create();
	static FreezeTower* create(const char *pszFilename, int range);
};

class CannonTower : public Tower {
public:
	bool initWithFileAndRange(const char *pszFilename, int range);
	void fire(float dt);
    
	Enemy* getClosestTarget();
    
	static CannonTower* create();
	static CannonTower* create(const char *pszFilename, int range);
};

#endif /* defined(__MyTowerDefense2D__Tower__) */
