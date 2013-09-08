#pragma once

#include "ofMain.h"

#include "ofxGrayscale.h"
#include "ofxBlur.h"
#include "ofxNormals.h"
#include "ofxDisplacePixels.h"


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
		
    ofVideoGrabber      video;
    
    ofxGrayscale        grayscale;
    ofxBlur             blur;
    ofxNormals          normals;
    ofxDisplacePixels   disp;
    
    int             width, height;
    
    bool            bDrawVideo;
    bool            bDrawNormals;
    bool            bDrawField;
    bool            bCapture;
};
