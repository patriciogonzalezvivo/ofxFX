/*
 *  ofxFlocking.h
 *
 *  Created by Patricio González Vivo on 9/27/11.
 *  Copyright 2011 PatricioGonzalezVivo.com. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the author nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 *  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 *  OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 *  OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 *  OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  ************************************************************************************ 
 *
 *  STEPS
 *  -------
 *  1. Process birth and death
 *  2. Update velocities
 *  3. Update positions
 *  4. Sort for alpha blending (optional)
 *  5. Transfer texture data to vertex data
 *  6. Render particles
 *
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
