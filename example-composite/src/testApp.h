#pragma once

//#define THERE_IS_CAM

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
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
#ifdef THERE_IS_CAM
    ofVideoGrabber  video;
#else
    ofImage     image;
#endif
    
    ofxBlur     blurEffect;
    ofImage     maskImage;
    ofxMask     maskEffect;
    ofxOldTv    oldTvEffect;
    
    float       beat;
    int         width, height;
};
