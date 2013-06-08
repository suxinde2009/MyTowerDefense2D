//
//  ProjectTile.h
//  MyTowerDefense2D
//
//  Created by su xinde on 13-6-8.
//
//

#ifndef __MyTowerDefense2D__ProjectTile__
#define __MyTowerDefense2D__ProjectTile__

#include "cocos2d.h"

class Enemy;
class GameMediator;

class ProjectTile : public cocos2d::CCNode {
public:
	virtual bool initWithFile(const char* filename);
    
	virtual cocos2d::CCRect getRect();
    
	CC_SYNTHESIZE(float, speed, Speed);
	CC_SYNTHESIZE(cocos2d::CCPoint, targetPos, TargetPos);
    
	CC_SYNTHESIZE(int, damage, Damage);
    
protected:
	cocos2d::CCSprite* mySprite;
    
	GameMediator* gm;
    
	bool hasRemoved;
    
	void removeSelf();
	
};

class MachineProjectTile: public ProjectTile{
public:
	bool initWithTargetPos(cocos2d::CCPoint pos);
	void update(float dt);
    
	static MachineProjectTile* create(cocos2d:: CCPoint pos);
    
private:
    
	void moveToTargetPos();
};


class IceProjectTile : public ProjectTile {
public:
	bool initWithTargetPos(cocos2d::CCPoint pos);
	void update(float dt);
    
	static IceProjectTile* create(cocos2d::CCPoint pos);
    
private:
	void moveToTargetPos();
};

class CannonProjectTile : public ProjectTile {
public:
	bool initWithTarget(Enemy* enemy);
	void update(float dt);
    
	static CannonProjectTile* create(Enemy* enemy);
    
private:
    
	float angularVelocity ;
    
	cocos2d::CCParticleFire* m_emitter;
    
	float to360Angle(float angle)
	{
		angle = (int)angle % 360;
		if (angle < 0)
			angle += 360;
		return angle;
	}
    
	Enemy* myEnemy;
};

#endif /* defined(__MyTowerDefense2D__ProjectTile__) */
