/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include <ofMain.h>
#include "fft.h"
#include <ofxVboSet.h>
#include "sj_common.h"

/************************************************************
************************************************************/

/**************************************************
**************************************************/
class DRAW_LOGO{
private:
	/****************************************
	****************************************/
	enum STATE{
		STATE_RANGE,
		STATE_LOW,
	};
	
	/****************************************
	****************************************/
	ofImage LogoImg;
	float _RotVel;
	float _rot = 0;
	
	float t_Last = 0;
	
	STATE State = STATE_LOW;
	float _scale = 1.0;
	float _scale_vel = 0;
	float _LastScale = 1;
	float _scale_a = 0;
	
public:
	/****************************************
	****************************************/
	DRAW_LOGO();
	~DRAW_LOGO();
	
	void setup();
	void update(const FFT& fft);
	void draw(ofFbo& fbo);
};



