/************************************************************
************************************************************/
#include "DrawLogo.h"
#include <time.h>

/************************************************************
************************************************************/

/******************************
******************************/
DRAW_LOGO::DRAW_LOGO()
{
}

/******************************
******************************/
DRAW_LOGO::~DRAW_LOGO()
{
}

/******************************
******************************/
void DRAW_LOGO::setup()
{
	/********************
	********************/
	print_separatoin();
	printf("> setup : DRAW_LOGO\n");
	fflush(stdout);
	
	/********************
	********************/
	LogoImg.load("texture/Logo.png");
}

/******************************
******************************/
void DRAW_LOGO::update(const FFT& fft)
{
	/********************
	********************/
	float now = (float)ofGetElapsedTimeMillis() / 1000.0f;
	float dt = now - t_Last;
	dt = ofClamp( dt, 0.0, 0.1 );
	t_Last = now;
	
	/********************
	********************/
	// _RotVel = 200;
	_RotVel = ofMap(fft.get_Gain((int)Gui_Global->bandRad), 1, 2, 25.0, 700.0, true);
	_rot += _RotVel * dt;
	while(360 <= _rot) { _rot -= 360; }
	
	/********************
	********************/
	if(!Gui_Global->b_Logo_ScaleSync_Dumping){ // for test.
		const float thresh_min = 1.0;
		_scale = ofMap(fft.get_Gain((int)Gui_Global->bandRad), thresh_min, 2, thresh_min, 2, true);
		
	}else{
		const float thresh_min = 1.0;
		float RealScale = ofMap(fft.get_Gain((int)Gui_Global->bandRad), thresh_min, 2, thresh_min, 2, false);
		
		switch(State){
			case STATE_RANGE:
				_scale = RealScale;
				_scale_vel = (_scale - _LastScale) / dt;
				
				if(RealScale < thresh_min){
					State = STATE_LOW;
					_scale_a = Gui_Global->Logo_ScaleSync_K * (thresh_min - _scale);
				}
				
				_LastScale = _scale;
				break;
				
			case STATE_LOW:
				if(thresh_min < RealScale){
					State = STATE_RANGE;
					
					_scale = RealScale;
					_scale_vel = (_scale - _LastScale) / dt;
				}else{
					_scale_vel = (_scale_vel + _scale_a * dt) * Gui_Global->Logo_ScaleSync_Friction;
					_scale = _scale + _scale_vel * dt;
					
					_scale_a = Gui_Global->Logo_ScaleSync_K * (thresh_min - _scale);
				}
				
				_LastScale = _scale;
				
				break;
		}
	}
}

/******************************
******************************/
void DRAW_LOGO::draw(ofFbo& fbo)
{
	/********************
	********************/
	ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	// ofEnableBlendMode(OF_BLENDMODE_ADD);
	// ofEnableSmoothing(); // OF_BLENDMODE_ADDとofEnableSmoothingは同時に有効化できない
	
	ofDisableDepthTest();
	
	/********************
	********************/
	fbo.begin();
		ofPushMatrix();
			/********************
			********************/
			ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
			if(Gui_Global->b_Logo_RotSync)		ofRotateDeg(_rot);
			if(Gui_Global->b_Logo_ScaleSync)	ofScale(_scale, _scale, 1);
			
			/********************
			********************/
			ofDisableDepthTest();
			
			ofSetColor(Gui_Global->col_Logo);
			
			LogoImg.draw(-Gui_Global->Size_Logo/2, -Gui_Global->Size_Logo/2, Gui_Global->Size_Logo, Gui_Global->Size_Logo);
		ofPopMatrix();
	fbo.end();
}
