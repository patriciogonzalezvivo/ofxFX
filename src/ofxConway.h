//
//  ofxConway.h
//
//  Created by kalwalt alias Walter Perdan on 24/12/11
//  Copyright 2011 http://www.kalwaltart.it/ All rights reserved.
//

#ifndef OFXCONWAY
#define OFXCONWAY

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxConway : public ofxFXObject {
public:

    ofxConway();

	void allocate(int _width, int _height);

    ofTexture& getTextureReference() { return pingPong[1].getTextureReference(); };

	void begin();
    void end(bool drawIt = false);

    void update();

    void draw(int x = 0, int y = 0){ pingPong.dst->draw(x, y);};

private:

    void loadShaders();

    ofShader	shader;
    swapBuffer	pingPong;

	int         iterations;
};
#endif
