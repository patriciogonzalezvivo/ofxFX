#pragma once

#include "ofMain.h"

#include "ofxFXObject.h"
#include "ofxBloom.h"
#include "ofxGaussianBlur.h"
#include "ofxBlur.h"
#include "ofxBokeh.h"
#include "ofxGlow.h"
#include "ofxOldTv.h"

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
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofxFXObject sandbox;
    
    ofxBloom    bloom;
    ofxGaussianBlur gaussianBlur;
    ofxBlur     blur;
    ofxBokeh    bokeh;
    ofxGlow     glow;
    ofxOldTv    oldtv;
    
    enum        { nTotalFrag = 19 };
    string      frags[ nTotalFrag ];
    string      fragsTitles[ nTotalFrag];
    float       beat;
    int         nFrag, selection, width, height;
};
