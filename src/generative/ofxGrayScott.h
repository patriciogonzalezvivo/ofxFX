/*
 *  ofxGrayScott.h
 *
 *  Created by Patricio Gonzalez Vivo on 10/1/11.
 *  Copyright 2011 http://PatricioGonzalezVivo.com All rights reserved.
 *
 *  Based on Cinder's Reaction Diffusion GLSL Shader 
 *  https://github.com/cinder/Cinder/blob/master/samples/RDiffusion/resources/gsrd_frag.glsl
 *  Which is based Gray Scott Frag shader from rdex-fluxus
 *  https://code.google.com/p/rdex-fluxus/source/browse/trunk/reactiondiffusion.frag
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
 *  ************************************************************************************ */
#pragma once

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxGrayScott : public ofxFXObject {
public:
    
    ofxGrayScott(){
        passes = 2;
        internalFormat = GL_RGB;
        
        diffU = 0.25f;
        f = 0.0195f;
        
        diffV = 0.04f;
        k = 0.066f;

        fragmentShader = STRINGIFY(
                                   float kernel[9];
                                   vec2 offset[9];
                                   
                                   uniform sampler2DRect backbuffer;
                                   uniform sampler2DRect tex0;

                                   uniform float diffU;
                                   uniform float diffV;
                                   uniform float f;
                                   uniform float k;

                                   void main(void){
                                       vec2 st   = gl_TexCoord[0].st;
                                       kernel[0] = 0.707106781;
                                       kernel[1] = 1.0;
                                       kernel[2] = 0.707106781;
                                       kernel[3] = 1.0;
                                       kernel[4] = -6.82842712;
                                       kernel[5] = 1.0;
                                       kernel[6] = 0.707106781;
                                       kernel[7] = 1.0;
                                       kernel[8] = 0.707106781;
                                       
                                       offset[0] = vec2( -1.0, -1.0);
                                       offset[1] = vec2(  0.0, -1.0);
                                       offset[2] = vec2(  1.0, -1.0);

                                       offset[3] = vec2( -1.0, 0.0);
                                       offset[4] = vec2(  0.0, 0.0);
                                       offset[5] = vec2(  1.0, 0.0);
                                       
                                       offset[6] = vec2( -1.0, 1.0);
                                       offset[7] = vec2(  0.0, 1.0);
                                       offset[8] = vec2(  1.0, 1.0);
                                       
                                       vec2 texColor = texture2DRect( backbuffer, st ).rb;
                                       float srcTexColor = texture2DRect( tex0, st ).r;
                                       
                                       vec2 lap = vec2( 0.0, 0.0 );
                                       
                                       for( int i=0; i < 9; i++ ){
                                           vec2 tmp = texture2DRect( backbuffer, st + offset[i] ).rb;
                                           lap += tmp * kernel[i];
                                       }
                                       
                                       float F  = f + srcTexColor * 0.025 - 0.0005;
                                       float K  = k + srcTexColor * 0.025 - 0.0005;
                                       
                                       float u  = texColor.r;
                                       float v  = texColor.g + srcTexColor * 0.5;
                                       
                                       float uvv = u * v * v;
                                       
                                       float du = diffU * lap.r - uvv + F * (1.0 - u);
                                       float dv = diffV * lap.g + uvv - (F + K) * v;
                                       
                                       u += du * 0.6;
                                       v += dv * 0.6;
            
                                       gl_FragColor = vec4(clamp( u, 0.0, 1.0 ), 1.0 - u/v ,clamp( v, 0.0, 1.0 ), 1.0);
                                   });
    };
    
    ofxGrayScott& setPasses(int _i){ passes = _i; return * this;};
    ofxGrayScott& setDiffU(float _diffU){ diffU = _diffU; return * this;};
    ofxGrayScott& setDiffV(float _diffV){ diffV = _diffV; return * this;};
    ofxGrayScott& setK(float _k){ k = _k; return * this;};
    ofxGrayScott& setF(float _f){ f = _f; return * this;};
    
    float getDiffU(){return diffU;};
    float getDiffV(){return diffV;};
    float getK(){return k;};
    float getF(){return f;};
    
    void update(){            
        for( int i = 0; i < passes ; i++ ){
            pingPong.dst->begin();
            ofClear(0,0,0,255);
            
            shader.begin();
            shader.setUniformTexture("backbuffer", pingPong.src->getTextureReference(), 0 );
            shader.setUniformTexture("tex0", textures[0].getTextureReference(), 1 );
            shader.setUniform1f( "diffU", (float)diffU);
            shader.setUniform1f( "diffV", (float)diffV);
            shader.setUniform1f( "f", (float)f );
            shader.setUniform1f( "k", (float)k );
            renderFrame();
            shader.end();
            pingPong.dst->end();
            
            pingPong.swap();
        }
        
        pingPong.swap();
    };
    
    float		diffU, diffV, k, f;
};
