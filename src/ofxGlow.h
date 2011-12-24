//
//  ofxGlow.h
//
//  Created by Patricio González Vivo on 10/1/11.
//  Copyright 2011 http://PatricioGonzalezVivo.com All rights reserved.
//
//  This is a simple implementation of Glow based on bluring and adding the orriginal layer. 
//  There is a better way of doing this by a shader... comming soon...
//

#ifndef OFXGLOW
#define OFXGLOW

#include "ofMain.h"
#include "ofxBlur.h"

class ofxGlow : public ofxBlur {	
public:
    void allocate(int width, int height) {
		original.allocate(width, height);
        pingPong.allocate(width, height);
        
        passes = 1;
        radius = 3;
        
        loadShaders();
    }
	
	void begin() {
		ofPushStyle();
		ofPushMatrix();
		original.begin();
	}
	
	void end(bool bDraw = true) {
		original.end();
		ofPopStyle();
		ofPopMatrix();
        
		ofSetColor(255,255);
		
        pingPong[1].begin();
        original.draw(0,0);
        pingPong[1].end();
        
        for(int i = 0; i < passes; i++) {
            for(int j = 0; j < 2; j++) {    
                pingPong[j].begin();
                blurShader[j].begin();
                blurShader[j].setUniform1f("radius", radius);
                pingPong[(j+1)%2].draw(0,0);
                blurShader[j].end();
                pingPong[j].end();
            }
        }
    
		if(bDraw)
			draw(0, 0);
	}
    
	void draw( int x = 0, int y = 0){
        pingPong[passes%2].draw(x,y);
		ofEnableAlphaBlending();
		ofEnableBlendMode(OF_BLENDMODE_ADD);
		original.draw(x,y);
		ofDisableAlphaBlending();
	}
    
private:
	ofFbo    original;
};

#endif