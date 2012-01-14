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
    ofxGlow(){
        passes = 1;
        nTextures = 1;
        internalFormat = GL_RGBA;
        
        radius = 3;
    }
    
	void draw(int x = 0, int y = 0, float _width = -1, float _height = -1){
        if (_width == -1) _width = width;
        if (_height == -1) _height = height;
        
        ofPushStyle();
        pingPong.dst->draw(x, y, _width, _height);
		ofEnableBlendMode(OF_BLENDMODE_ADD);
		textures[0].draw(x,y, _width, _height);
        ofDisableBlendMode();
        ofPopStyle();
	}
    
private:
};

#endif