//
//  GameMediator.cpp
//  MyTowerDefense2D
//
//  Created by su xinde on 13-6-8.
//
//

#include "GameMediator.h"

using namespace cocos2d;

static GameMediator _sharedContext;

GameMediator* GameMediator::sharedMediator(){
	static bool s_bFirstUse = true;
	if(s_bFirstUse){
		_sharedContext.init();
		s_bFirstUse = false;
	}
	return &_sharedContext;
}

GameMediator::~GameMediator(){
	CC_SAFE_RELEASE_NULL(_gameLayer);
	CC_SAFE_RELEASE_NULL(_targets);
	
	CC_SAFE_RELEASE_NULL(_waves);
	CC_SAFE_RELEASE_NULL(_towers);
	CC_SAFE_RELEASE_NULL(_ProjectTiles);
}

bool GameMediator::init(){
	bool bRet = false;
	do
	{
		_gameLayer = NULL;
		_gameHUDLayer = NULL;
        
		_targets = CCArray::create();
		_targets->retain();
        
		_waves = CCArray::create();
		_waves->retain();
        
		_towers = CCArray::create();
		_towers->retain();
        
		_ProjectTiles = CCArray::create();
		_ProjectTiles->retain();
        
		bRet = true;
	} while (0);
	return bRet;
}
