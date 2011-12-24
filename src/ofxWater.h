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

class ofxWater : public ofxFXObject {
public:
    
    ofxWater();
    
    ofxWater& allocate(int _width, int _height);
    
    ofxWater& loadBackground(string file);
    ofxWater& linkBackground(ofTexture * _backText);
    
    ofxWater& setThreshold(float _threshold){ threshold = _threshold; return * this; };
    ofxWater& setDensity(float _density){ density = _density; return * this; };
    ofxWater& setFade(float _fade){ blurFade = _fade; return * this; };
    
    ofTexture& getTextureReference(){ return renderFbo.getTextureReference(); };
    
    void begin();
    void end(bool drawIt = false);
    
    void update();
    void draw(int x = 0, int y = 0);
    
    
private:
    ofShader    updateShader;
    ofShader    renderShader;
    ofShader    blurShader;
    
    swapBuffer  pingPong;
    ofFbo       updateFbo;
    ofFbo       renderFbo;
    
    ofImage     backgroundImage;
    ofTexture  *texture;
    
    float       blurFade, density, threshold;
    int         frame;
};

#endif
