//
//  Lightning.h
//  MyTowerDefense2D
//
//  Created by su xinde on 13-6-8.
//
//

#ifndef __MyTowerDefense2D__Lightning__
#define __MyTowerDefense2D__Lightning__

#include "cocos2d.h"

extern int getNextRandom(unsigned long *seed);

extern cocos2d::CCPoint drawLightning(cocos2d::CCPoint pt1, cocos2d::CCPoint pt2, int displace, int minDisplace, unsigned long randSeed);


class Lightning : public cocos2d::CCNode, public cocos2d::CCRGBAProtocol {
public:
	bool initWithStrikePoint(cocos2d::CCPoint strikePoint);
	bool initWithStrikePoint(cocos2d::CCPoint strikePoint, cocos2d::CCPoint strikePoint2);
    
	static Lightning* create(cocos2d::CCPoint strikePoint);
	static Lightning* create(cocos2d::CCPoint strikePoint, cocos2d::CCPoint strikePoint2);
    
	void strikeRandom();
	void strikeWithSeed(unsigned long seed);
	void strike();
    
	virtual void draw();
    
	virtual void setColor(const cocos2d::ccColor3B& color);
	virtual const cocos2d::ccColor3B& getColor(void);
    
	virtual GLubyte getOpacity(void);
	virtual void setOpacity(GLubyte opacity);
    
	virtual void setOpacityModifyRGB(bool bValue) {}
	virtual bool isOpacityModifyRGB(void) {return true;}
    
	CC_SYNTHESIZE_PASS_BY_REF(cocos2d::CCPoint, _strikePoint, StrikePoint);
	CC_SYNTHESIZE_PASS_BY_REF(cocos2d::CCPoint, _strikePoint2, StrikePoint2);
	CC_SYNTHESIZE_PASS_BY_REF(cocos2d::CCPoint, _strikePoint3, StrikePoint3);
    
	CC_SYNTHESIZE(bool, _split, Split);
    
	CC_SYNTHESIZE(int, _displacement, Displacement);
	CC_SYNTHESIZE(int, _minDisplacement, MinDisplacement);
	CC_SYNTHESIZE(float, _lighteningWidth, LighteningWidth);
    
	CC_SYNTHESIZE(unsigned long, _seed, Seed);
    
protected:
	cocos2d::ccColor3B _color;
	GLubyte _opacity;
    
};

#endif /* defined(__MyTowerDefense2D__Lightning__) */
