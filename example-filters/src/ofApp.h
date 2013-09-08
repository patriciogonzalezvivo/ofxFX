#pragma once

#include "ofMain.h"

#include "ofxBloom.h"
#include "ofxGaussianBlur.h"
#include "ofxBokeh.h"
#include "ofxBlur.h"
#include "ofxLUT.h"
#include "ofxMultiTexture.h"
#include "ofxGlow.h"

class testApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	
	ofVideoGrabber  video;
    
    ofImage     polaroid;
    ofImage     map;
    ofxMultiTexture mult;
    
    ofDirectory dir;
    ofxLUT      lut;
    
    ofxBloom        bloom;
    ofxGaussianBlur gaussianBlur;
    ofxBokeh        bokeh;
    ofxGlow         glow;
    ofxBlur         blur;
    
    int         width,height;
    int         selection;
    int         dirLoadIndex;
};
