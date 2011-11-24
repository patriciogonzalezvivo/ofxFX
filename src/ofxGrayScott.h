//
//  ofxGrayScott.h
//  Reaction Diffusion
//
//  Created by Patricio González Vivo on 10/1/11.
//  Copyright 2011 PatricioGonzalezVivo.com. All rights reserved.
//
//  Based on Reaction Diffusion by Gray-Scott Model
//  http://mrob.com/pub/comp/xmorphia/
//

#ifndef OFXGRAYSCOTT
#define OFXGRAYSCOTT

#include "ofMain.h"

class ofxGrayScott{
public:
    
    ofxGrayScott();
    
    ofxGrayScott& allocate(int _width, int _height);
    ofxGrayScott& setIterations(int _i){ iterations = _i; return * this;};
    ofxGrayScott& setDiffU(float _diffU){ diffU = _diffU; return * this;};
    ofxGrayScott& setDiffV(float _diffV){ diffV = _diffV; return * this;};
    ofxGrayScott& setK(float _k){ k = _k; return * this;};
    ofxGrayScott& setF(float _f){ f = _f; return * this;};
    
    ofTexture& getTextureReference(){ return fbo[(frame+1)%2].getTextureReference(); };
    float getDiffU(){return diffU;};
    float getDiffV(){return diffV;};
    float getK(){return k;};
    float getF(){return f;};
    
    void begin();
    void end(bool drawIt = false);
    
    void update();
    void draw();
    
private:
    void        renderFrame();
    
	ofShader	shader;
	ofFbo		fbo[2];
	ofFbo       texture;
    
    float		diffU, diffV, k, f;
    
    int         iterations;
    int         width, height;
    int         frame;
};
#endif
