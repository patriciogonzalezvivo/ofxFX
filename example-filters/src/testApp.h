#pragma once

#include "ofMain.h"
#include "ofxFX.h"

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
    
    ofxBloom    bloom;
    ofxBlur     blur;
    ofxBokeh    bokeh;
    ofxGlow     glow;
    ofxUnsharp  unsharp;
    
    float       beat;
    int         width,height;
    int         selection;
    int         dirLoadIndex;
};
