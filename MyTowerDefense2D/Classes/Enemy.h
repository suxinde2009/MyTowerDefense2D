//
//  Enemy.h
//  MyTowerDefense2D
//
//  Created by su xinde on 13-6-8.
//
//

#ifndef __MyTowerDefense2D__Enemy__
#define __MyTowerDefense2D__Enemy__

#include "cocos2d.h"
#include "TileData.h"
class MainLayer;

class Enemy : public cocos2d::CCNode {
public:
	~Enemy();
	bool initWithMem(const char* filename, int hp, float speed);
    
	void attack();
	virtual cocos2d::CCRect getRect();
	virtual void changeSpeed(float time = 1.0f);
    
	CC_SYNTHESIZE(int, hp, HP);
	CC_SYNTHESIZE(float, speed, Speed);
	CC_SYNTHESIZE(cocos2d::CCPoint, startPos, StartPos);
	CC_SYNTHESIZE(cocos2d::CCPoint, endPos, EndPos);
    
protected:
	cocos2d::CCSprite* sprite;
	MainLayer* mainLayer;
    
	float orgSpeed;
	float times;
    
	bool hasRemoved;
	TileData* pre;
    
	void timer(float dt);
    
	void moveToTarget();
	void insertInOpenSteps(TileData* td);
	int costToMoveFromTileToAdjacentTile(TileData* fromTile, TileData* toTile);
	int computeHScoreFromCoordToCoord(cocos2d::CCPoint fromCoord, cocos2d::CCPoint toCoord);
	void constructPathAndStartAnimationFromStep(TileData* tile);
	void popStepAndAnimate();
    
	void removeSelf();
	void enemyLogic(float dt);
    
	cocos2d::CCArray* spOpenSteps;
	cocos2d::CCArray* spClosedSteps;
	cocos2d::CCArray* shortestPath;
    
	inline unsigned int myArrayGetIndexOfObject(cocos2d::CCArray* arr, TileData* object){
		for ( unsigned int i = 0; i < arr->count(); i++)
		{
			TileData* temp = (TileData*)arr->objectAtIndex(i);
			if (cocos2d::CCPoint::CCPointEqualToPoint(temp->getPosition(), object->getPosition()))
			{
				return i;
			}
		}
        
		return CC_INVALID_INDEX;
	}
    
};

class FastRedEnemy : public Enemy {
public:
	static FastRedEnemy* create();
	static FastRedEnemy* create(const char* filename, int hp, float speed);
};

class StrongGreenEnemy : public Enemy {
public:
	static StrongGreenEnemy* create();
	static StrongGreenEnemy* create(const char* filename, int hp, float speed);
};

#endif /* defined(__MyTowerDefense2D__Enemy__) */
