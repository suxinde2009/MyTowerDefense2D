//
//  Enemy.cpp
//  MyTowerDefense2D
//
//  Created by su xinde on 13-6-8.
//
//

#include "Enemy.h"
#include "GameMediator.h"
#include "MainLayer.h"

using namespace cocos2d;

Enemy::~Enemy(){
	CC_SAFE_RELEASE_NULL(spOpenSteps);
	CC_SAFE_RELEASE_NULL(spClosedSteps);
	CC_SAFE_RELEASE_NULL(shortestPath);
}

bool Enemy::initWithMem(const char* filename, int hp, float speed){
	bool bRet = false;
	do
	{
        
		mainLayer = GameMediator::sharedMediator()->getGameLayer();
		int maxTileWidth = mainLayer->getMaxTileWidth();
		int maxTileHeight = mainLayer->getMaxTileHeight();
        
		sprite = CCSprite::create(filename);
		this->addChild(sprite);
		
		this->setHP(hp);
		this->setSpeed(speed);
        
		int x, y;
		x = 0;
		y= maxTileHeight / 2 ;
		startPos = ccp(x, y);
		x = maxTileWidth  - 1;
		endPos = ccp(x, y);
        
		hasRemoved = false;
        
		spOpenSteps = CCArray::create();
		spOpenSteps->retain();
        
		spClosedSteps = CCArray::create();
		spClosedSteps->retain();
        
		shortestPath = CCArray::create();
		shortestPath->retain();
        
		attack();
        
		times = 0;
		orgSpeed = speed;
        
		pre = NULL;
        
		schedule(schedule_selector(Enemy::enemyLogic), 1.0f);
		schedule(schedule_selector(Enemy::timer), 0.2f);
        
		bRet = true;
	} while (0);
	return bRet;
}

void Enemy::changeSpeed(float time){
	times = time * 5;//time / 0.2;
	if(speed < 0.5f)
		return;
	speed = speed * 0.5f;
	
}

void Enemy::timer(float dt){
	if(times <=0 ){
		if(speed != orgSpeed)
			speed = orgSpeed;
		return;
	}
    
	times --;
}

void Enemy::enemyLogic(float dt){
	GameMediator* gm = GameMediator::sharedMediator();
    
	if(this->getPosition().x > mainLayer->getMaxTileWidth() * TILE_WIDTH + 200.0f){
		unscheduleAllSelectors();
        
		gm->getGameHUDLayer()->updateBaseHp(-10);
		removeSelf();
		
	}
    
	if(this->getHP() <= 0){
		unscheduleAllSelectors();
        
		gm->getGameHUDLayer()->updateResources(5);
        
		this->stopAllActions();
		auto deadAction = CCBlink::create(1.0f, 5);
		auto deadDone = CCCallFunc::create(this, callfunc_selector(Enemy::removeSelf));
        
		this->runAction(CCSequence::create(deadAction, deadDone, NULL));
	}
}

CCRect Enemy::getRect(){
	CCRect rect = CCRectMake(this->getPosition().x - sprite->getContentSize().width * 0.5f,
                             this->getPosition().y - sprite->getContentSize().height* 0.5f,
                             sprite->getContentSize().width,
                             sprite->getContentSize().height);
	return rect;
}

void Enemy::insertInOpenSteps(TileData* td){
	int tdFScore = td->fScore();
	unsigned int count = spOpenSteps->count();
    
	unsigned int i = 0;
	for(; i < count; i++){
		TileData* temp = (TileData*)spOpenSteps->objectAtIndex(i);
		if(tdFScore <= temp->fScore()){
			break;
		}
	}
	spOpenSteps->insertObject(td, i);
}

int Enemy::costToMoveFromTileToAdjacentTile(TileData* fromTile, TileData* toTile){
	return 1;
}

int Enemy::computeHScoreFromCoordToCoord(cocos2d::CCPoint fromCoord, cocos2d::CCPoint toCoord){
	return abs(toCoord.x - fromCoord.x) + abs(toCoord.y - fromCoord.y);
}

void Enemy::constructPathAndStartAnimationFromStep(TileData* tile){
	shortestPath->removeAllObjects();
	do
	{
		if(tile->getParentTile() != NULL){
			shortestPath->insertObject(tile, 0);
		}
		tile = tile->getParentTile();
	} while (tile != NULL);
    
	this->popStepAndAnimate();
}

void Enemy::popStepAndAnimate(){
	if(getHP() <= 0) return;
    
	if(pre){
		pre->setIsThroughing(false);
	}
    
	if(shortestPath->count()  == 0){
		this->runAction(CCMoveBy::create(1.0f, ccp(300, 0)));
        
		return;
	}
    
	TileData* s = (TileData*)shortestPath->objectAtIndex(0);
	if(s->getIsUsed()){
		shortestPath->removeAllObjects();
		moveToTarget();
		return;
	}
    
	pre = s;
	s->setIsThroughing(true);
	float moveTime = 32 / speed;
	auto moveAction = CCMoveTo::create(moveTime, mainLayer->positionForTileCoord(s->getPosition()));
	auto moveCallback = CCCallFunc::create(this, callfunc_selector(Enemy::popStepAndAnimate));
	shortestPath->removeObjectAtIndex(0);
	this->runAction(CCSequence::create(moveAction, moveCallback, NULL));
}

void Enemy::attack(){
	auto moveToStart = CCMoveTo::create(1.0f, mainLayer->positionForTileCoord(startPos));
	auto moveDone = CCCallFunc::create(this, callfunc_selector(Enemy::moveToTarget));
	this->runAction(CCSequence::create(moveToStart, moveDone, NULL));
}

void Enemy::moveToTarget(){
	CCPoint startPosition = mainLayer->tileCoordForPosition(this->getPosition());
	TileData* startTd = mainLayer->getTileData(startPosition);
	startTd->setParentTile(NULL);
	this->insertInOpenSteps(startTd);
	do
	{
		TileData* _currentTile = (TileData*)spOpenSteps->objectAtIndex(0);
		CCPoint _currentTileCoord = _currentTile->getPosition();
		spClosedSteps->addObject(_currentTile);
		spOpenSteps->removeObjectAtIndex(0);
		if(CCPoint::CCPointEqualToPoint(_currentTileCoord, endPos)){
			constructPathAndStartAnimationFromStep(_currentTile);
			spOpenSteps->removeAllObjects();
			spClosedSteps->removeAllObjects();
			break;
		}
        
		CCArray* tiles = mainLayer->getTilesNextToTile(_currentTileCoord);
		CCObject* temp;
		CCARRAY_FOREACH(tiles, temp){
			CCString* string = (CCString*)temp;
			CCPoint tileCoord = CCPointFromString(string->getCString());
			TileData* _neighbourTile = mainLayer->getTileData(tileCoord);
			if(myArrayGetIndexOfObject(spClosedSteps, _neighbourTile) != CC_INVALID_INDEX){
				continue;
			}
            
			if(_neighbourTile->getIsUsed()){
				continue;
			}
            
			int moveCost = costToMoveFromTileToAdjacentTile(_currentTile, _neighbourTile);
			int index = myArrayGetIndexOfObject(spOpenSteps, _neighbourTile);
			if(index == CC_INVALID_INDEX){
				_neighbourTile->setParentTile(NULL);
				_neighbourTile->setParentTile(_currentTile);
				_neighbourTile->setGScore(_currentTile->getGScore() + moveCost);
				_neighbourTile->setHScore(computeHScoreFromCoordToCoord(_neighbourTile->getPosition(), endPos));
				insertInOpenSteps(_neighbourTile);
			}else{
				_neighbourTile = (TileData*)spOpenSteps->objectAtIndex(index);
				if(_currentTile->getGScore() + moveCost < _neighbourTile->getGScore()){
					_neighbourTile->setGScore(_currentTile->getGScore() + moveCost);
                    
					_neighbourTile->retain();
					spOpenSteps->removeObjectAtIndex(index);
					this->insertInOpenSteps(_neighbourTile);
					_neighbourTile->release();
				}
			}
            
		}
        
	} while (spOpenSteps->count() > 0);
}

void Enemy::removeSelf(){
	if(hasRemoved)
		return;
    
	hasRemoved = true;
	GameMediator* gm = GameMediator::sharedMediator();
	gm->getTargets()->removeObject(this);
    
	this->stopAllActions();
	this->removeFromParentAndCleanup(true);
}

FastRedEnemy* FastRedEnemy::create(const char* filename, int hp, float speed){
	FastRedEnemy* fre = new FastRedEnemy;
	if(fre && fre->initWithMem(filename, hp, speed)){
		fre->autorelease();
		return fre;
	}
	CC_SAFE_DELETE(fre);
	return NULL;
}

FastRedEnemy* FastRedEnemy::create(){
	return FastRedEnemy::create("Enemy1.png", 100, 64.0f);
}

StrongGreenEnemy* StrongGreenEnemy::create(const char* filename, int hp, float speed){
	StrongGreenEnemy* sge = new StrongGreenEnemy;
	if(sge && sge->initWithMem(filename, hp, speed)){
		sge->autorelease();
		return sge;
	}
	CC_SAFE_DELETE(sge);
	return NULL;
}

StrongGreenEnemy* StrongGreenEnemy::create(){
	return StrongGreenEnemy::create("Enemy2.png", 200, 32.0f);
}
