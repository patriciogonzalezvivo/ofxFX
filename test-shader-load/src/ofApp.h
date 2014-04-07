#pragma once

#include "ofMain.h"
#include "ofxFXObject.h"
#include "ofxBlur.h"
#include "ofxFileBlur.h"

class ofApp : public ofBaseApp{
    public:

        void setup();
        void update();
        void draw();

        // Inherited but unused.
        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y);
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);

    private:
        // To make it easier to draw to fbo in either update() or draw().
        // Does not make a difference where it is called from, but is
        // called from update() in sandbox-example.
        void draw_rect_to_fbo_and_update_effect();

        int width = 300;
        int height = 300;
        ofxFileBlur fx_file;
        ofxBlur fx_string;
        ofFbo render_buffer;
};
