//
//  ofxFlocking.h
//
//  Using:
//      http://www.gamasutra.com/view/feature/2122/building_a_millionparticle_system.php?page=2
//      Daniel shiffman Flocking Sketch
//
//  Created by Patricio González Vivo on 9/27/11.
//  Copyright 2011 PatricioGonzalezVivo.com. All rights reserved.
//

/*
 STEPS
 -------
 1. Process birth and death
 2. Update velocities
 3. Update positions
 4. Sort for alpha blending (optional)
 5. Transfer texture data to vertex data
 6. Render particles
 */

#ifndef OFXFLOCKING
#define OFXFLOCKING

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxFlocking : public ofxFXObject {
public:
    ofxFlocking();
    
    void allocate(int _width, int _height, int _nBoids);
    
    ofxFlocking& setParticleSize(float _size){particleSize = _size; return * this;};
    ofxFlocking& setTimpeStep(float _timeStep){timeStep = _timeStep; return * this;};

    ofTexture& getTextureReference(){ return renderFbo.getTextureReference();  };
    
    void    update();
    void    draw(int x = 0, int y = 0, float _width = -1, float _height = -1);
    
private:
    ofImage  sparkImage;
    
    ofShader posUpdateShader;
    ofShader renderShader;
    
    ofPoint *particles;
    ofFloatPixels pixels;
    
    ofxSwapBuffer posBuffer;
    
    ofFbo   renderFbo;
    
    float   timeStep, particleSize, imageWidth, imageHeight;
    float   maxDist, minDist, maxSpeed, maxForce, separation, alineation, cohesion;
    int     nBoids, resolution;
};

#endif
