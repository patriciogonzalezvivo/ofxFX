//
//  ofxGrayScott.h
//  Reaction Diffusion
//
//  Created by Patricio González Vivo on 10/1/11.
//  Copyright 2011 http://PatricioGonzalezVivo.com All rights reserved.
//
//  Based on Reaction Diffusion by Gray-Scott Model
//  http://mrob.com/pub/comp/xmorphia/
//

#ifndef OFXGRAYSCOTT
#define OFXGRAYSCOTT

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxGrayScott : public ofxFXObject {
public:
    
    ofxGrayScott(){
        passes = 10;
        
        diffU = 0.25f;
        diffV = 0.04f;//0.04f;
        k = 0.047f;//0.037f;
        f = 0.1f;//0.2f;
        
        fragmentShader = "#version 120 \n \
        #extension GL_ARB_texture_rectangle : enable \n \
        #define KERNEL_SIZE 9 \n \
        \
        float kernel[KERNEL_SIZE];\
        vec2 offset[KERNEL_SIZE];\
        \
        uniform sampler2DRect backbuffer;\
        uniform sampler2DRect tex;\
        \
        uniform float diffU;\
        uniform float diffV;\
        uniform float f;\
        uniform float k;\
        \
        void main(void){\
            vec2 st   = gl_TexCoord[0].st;\
            \
            kernel[0] = 0.707106781;\
            kernel[1] = 1.0;\
            kernel[2] = 0.707106781;\
            kernel[3] = 1.0;\
            kernel[4] = -6.82842712;\
            kernel[5] = 1.0;\
            kernel[6] = 0.707106781;\
            kernel[7] = 1.0;\
            kernel[8] = 0.707106781;\
            \
            offset[0] = vec2( -1.0, -1.0);\
            offset[1] = vec2(  0.0, -1.0);\
            offset[2] = vec2(  1.0, -1.0);\
            \
            offset[3] = vec2( -1.0, 0.0);\
            offset[4] = vec2(  0.0, 0.0);\
            offset[5] = vec2(  1.0, 0.0);\
            \
            offset[6] = vec2( -1.0, 1.0);\
            offset[7] = vec2(  0.0, 1.0);\
            offset[8] = vec2(  1.0, 1.0);\
            \
            vec2 texColor		= texture2DRect( backbuffer, st ).rb;\
            float srcTexColor   = texture2DRect( tex, st ).r;\
            \
            vec2 lap            = vec2( 0.0, 0.0 );\
            \
            for( int i=0; i < KERNEL_SIZE; i++ ){\
                vec2 tmp    = texture2DRect( backbuffer, st + offset[i] ).rb;\
                lap         += tmp * kernel[i];\
            }\
            \
            float F     = f + srcTexColor * 0.025 - 0.0005;\
            float K     = k + srcTexColor * 0.025 - 0.0005;\
            \
            float u		= texColor.r;\
            float v		= texColor.g + srcTexColor * 0.5;\
            \
            float uvv   = u * v * v;\
            \
            float du    = diffU * lap.r - uvv + F * (1.0 - u);\
            float dv    = diffV * lap.g + uvv - (F + K) * v;\
            \
            u += du * 0.6;\
            v += dv * 0.6;\
            \
            gl_FragColor.rgba = vec4(clamp( u, 0.0, 1.0 ), 1.0 - u/v ,clamp( v, 0.0, 1.0 ), 1.0);\
        }";
    };
    
    void allocate(int _width, int _height){
        width = _width; 
        height = _height; 
        
        pingPong.allocate(width, height,GL_RGB);
        
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
        shader.linkProgram();
        
        initFbo(texture, width, height,GL_RGB);
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
            shader.setUniformTexture("tex", texture.getTextureReference(), 1 );
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
    
private:
    float		diffU, diffV, k, f;
};
#endif
