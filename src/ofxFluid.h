//
//  ofxFluid.h
//  GPU fluid
//
//  Created by Patricio González Vivo on 9/29/11.
//  Copyright 2011 PatricioGonzalezVivo.com. All rights reserved.
//
//  Created ussing:
//
//    - Mark Harris article from GPU Gems 1
//      http://http.developer.nvidia.com/GPUGems/gpugems_ch38.html
//
//    - Phil Rideout
//      http://prideout.net/blog/?p=58

#ifndef OFXFLUID
#define OFXFLUID

#include "ofMain.h"

typedef struct  {
    ofFbo   *src;       // Source       ->  Ping
    ofFbo   *dst;       // Destination  ->  Pong
    ofFbo   FBOs[2];    // Real addresses of ping/pong FBO´s 
    int     flag;       // Integer for making a quick swap
    float   diss;       // Dissipation
} Buffer;

typedef struct  {
    ofVec3f color;
    ofVec2f pos;
    ofVec2f vel;
    float   rad;
    float   temp;
    float   den;
} punctualForce;

class ofxFluid {
public:
    ofxFluid();
    
    ofxFluid&   allocate(int _width, int _height, float _scale);
    
    ofxFluid&   setDensity(ofTexture & _tex){setTextureToBuffer(_tex, densityBuffer); return * this; };
    ofxFluid&   setVelocity(ofTexture & _tex){setTextureToBuffer(_tex, velocityBuffer); return * this; };
    ofxFluid&   setTemperature(ofTexture & _tex){setTextureToBuffer(_tex, temperatureBuffer); return * this; };
    ofxFluid&   setGravity(ofVec2f _force){ gForce = _force; return * this;};
    ofxFluid&   setDensityDissipation(float _diss){densityBuffer.diss = _diss; return * this;};
    ofxFluid&   setVelocityDissipation(float _diss){velocityBuffer.diss = _diss; return * this;};
    ofxFluid&   setTemperatureDissipation(float _diss){temperatureBuffer.diss = _diss; return * this;};
    
    void    addTemporalForce(ofVec2f _pos, ofVec2f _dir, ofFloatColor _col, float _rad = 1.0f, float _temp = 10.f, float _den = 1.0f );
    void    addConstantForce(ofVec2f _pos, ofVec2f _dir, ofFloatColor _col, float _rad = 1.0f, float _temp = 10.f, float _den = 1.0f );
    
    void    obstaclesBegin(); // FBO for obstables
    void    obstaclesEnd();
    
    void    update();
    void    draw();
    
private:
    void    initFbo(ofFbo & _fbo, int _width, int _height, int _internalformat);
    
    void    initBuffer(Buffer & _buffer, float _dissipation ,int _width, int _height, int _internalformat);
    void    setTextureToBuffer(ofTexture & _tex, Buffer & _buffer);
    void    swapBuffer(Buffer & _buffer);
    
    void    renderFrame(float _width, float _height);
    
    void    advect(Buffer& buffer);
    void    jacobi();
    void    subtractGradient();
    void    computeDivergence();
    
    void    applyImpulse(Buffer& _buffer, ofVec2f _force, ofVec3f _value, float _radio = 3.0f);
    void    applyBuoyancy();

    ofShader advectShader;
    ofShader jacobiShader;
    ofShader subtractGradientShader;
    ofShader computeDivergenceShader;
    ofShader applyImpulseShader;
    ofShader applyBuoyancyShader;
    
    Buffer  velocityBuffer;
    Buffer  densityBuffer;
    Buffer  temperatureBuffer;
    Buffer  pressureBuffer;
    
    ofFbo   divergenceFbo;
    ofFbo   obstaclesFbo;
    ofFbo   hiresObstaclesFbo;
    
    vector<punctualForce> constantForces;
    vector<punctualForce> temporalForces;
    ofVec2f gForce;
    
    float   smokeBuoyancy;
    float   smokeWeight;
    float   gradientScale;
    float   ambientTemperature;
    
    float   width,height;
    float   gridWidth,gridHeight;
    float   cellSize;
    float   timeStep;
    float   scale;
    
    int     numJacobiIterations;
};
#endif