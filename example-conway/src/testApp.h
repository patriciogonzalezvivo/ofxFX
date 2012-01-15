//  Conway Life GLSL Game
//
//  Created by kalwalt alias Walter Perdan on 24/12/11
//  Copyright 2011 http://www.kalwaltart.it/ All rights reserved.
//
//  http://forum.openframeworks.cc/index.php/topic,8389.msg39174.html#msg39174

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
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);


        ofImage      image;

        ofxFXObject  conway;

};
