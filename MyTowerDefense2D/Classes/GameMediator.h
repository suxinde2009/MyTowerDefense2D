//
//  GameMediator.h
//  MyTowerDefense2D
//
//  Created by su xinde on 13-6-8.
//
//

#ifndef __MyTowerDefense2D__GameMediator__
#define __MyTowerDefense2D__GameMediator__

#include "cocos2d.h"
#include "MainLayer.h"
#include "GameHUD.h"

class GameMediator : public cocos2d::CCObject {
public:
	~GameMediator();
	bool init();
    
	static GameMediator* sharedMediator();
    
	CC_SYNTHESIZE_RETAIN(MainLayer*, _gameLayer, GameLayer);
	CC_SYNTHESIZE_RETAIN(GameHUD*, _gameHUDLayer, GameHUDLayer);
    
	CC_SYNTHESIZE_RETAIN(cocos2d::CCArray*, _targets, Targets);
	CC_SYNTHESIZE_RETAIN(cocos2d::CCArray*, _waves, Waves);
    
	CC_SYNTHESIZE_RETAIN(cocos2d::CCArray*, _towers, Towers);
    
	CC_SYNTHESIZE_RETAIN(cocos2d::CCArray*, _ProjectTiles, ProjectTiles);
};


#endif /* defined(__MyTowerDefense2D__GameMediator__) */
