//
//  ofxWater.h
//
//  Created by Patricio González Vivo on 9/26/11.
//  Copyright 2011 http://PatricioGonzalezVivo.com All rights reserved.
//
//  Based on http://freespace.virgin.net/hugo.elias/graphics/x_water.htm

#ifndef OFXWATER
#define OFXWATER

#include "ofMain.h"

#include "ofxFXObject.h"
#include "ofxBlurFast.h"

class ofxWater : public ofxFXObject {
public:
    ofxWater();
    
    ofxWater& loadBackground(string file);
    ofxWater& linkBackground(ofTexture * _backText);
    
    ofxWater& setThreshold(float _threshold){ threshold = _threshold; return * this; };
    ofxWater& setDensity(float _density){ density = _density; return * this; };
    ofxWater& setFade(float _fade){ blurFade = _fade; return * this; };
    
    ofTexture& getTextureReference(){ return textures[3].getTextureReference(); };
    
    void begin();
	void end();
    
    void update();
    void draw(int x = 0, int y = 0, float _width = -1, float _height = -1);
    
private:
    ofShader    renderShader;
    ofShader    blurShader;

    float       blurFade, density, threshold;
};

#endif
