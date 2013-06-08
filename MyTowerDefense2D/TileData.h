//
//  TileData.h
//  MyTowerDefense2D
//
//  Created by su xinde on 13-6-8.
//
//

#ifndef __MyTowerDefense2D__TileData__
#define __MyTowerDefense2D__TileData__

#include "cocos2d.h"

class TileData : public cocos2d::CCObject {
public:
	//~TileData();
	bool initWithPosition(cocos2d::CCPoint pos);
    
	static TileData* create(cocos2d::CCPoint pos);
    
	int fScore();
    
	bool getIsThroughing();
	void setIsThroughing(bool var);
    
	CC_SYNTHESIZE(TileData*, parentTile, ParentTile);
    
	CC_SYNTHESIZE(cocos2d::CCPoint, position, Position);
	CC_SYNTHESIZE(int, gScore, GScore);
	CC_SYNTHESIZE(int, hScore, HScore);
    
	CC_SYNTHESIZE(bool, isUsed, IsUsed);
    
protected:
	bool isThroughing;
	int isThroughingCount;
    
	int _fScore;
    
};

#endif /* defined(__MyTowerDefense2D__TileData__) */
