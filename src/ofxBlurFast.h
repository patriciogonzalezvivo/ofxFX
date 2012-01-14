//
//  ofxBlurFast.h
//
//  Created by Patricio González Vivo on 25/11/11.
//  Copyright (c) 2011 http://PatricioGonzalezVivo.com All rights reserved.
//
//  It´s faster because it´s simple matrix convultion in one pass... 
//  it don´t have different horizontal and vertical shaders
//

#ifndef OFXBLURFAST
#define OFXBLURFAST

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxBlurFast : public ofxFXObject {	
public:
    ofxBlurFast(){
        fade = 1.f;
        passes = 3;
        
        fragmentShader = "#version 120\n \
        #extension GL_ARB_texture_rectangle : enable\n \
        \
        float kernel[9];\
        \
        uniform sampler2DRect backbuffer;\
        uniform float fade_const;\
        \
        vec2 offset[9];\
        \
        void main(void){\
            vec2  st = gl_TexCoord[0].st;\
            vec4 sum = vec4(0.0);\
            \
            offset[0] = vec2(-1.0, -1.0);\
            offset[1] = vec2(0.0, -1.0);\
            offset[2] = vec2(1.0, -1.0);\
            \
            offset[3] = vec2(-1.0, 0.0);\
            offset[4] = vec2(0.0, 0.0);\
            offset[5] = vec2(1.0, 0.0);\
            \
            offset[6] = vec2(-1.0, 1.0);\
            offset[7] = vec2(0.0, 1.0);\
            offset[8] = vec2(1.0, 1.0);\
            \
            kernel[0] = 1.0/16.0;   kernel[1] = 2.0/16.0;   kernel[2] = 1.0/16.0;\
            kernel[3] = 2.0/16.0;   kernel[4] = 4.0/16.0;   kernel[5] = 2.0/16.0;\
            kernel[6] = 1.0/16.0;   kernel[7] = 2.0/16.0;   kernel[8] = 1.0/16.0;\
            \
            int i = 0;\
            for (i = 0; i < 4; i++){\
                vec4 tmp = texture2DRect(backbuffer, st + offset[i]);\
            sum += tmp * kernel[i];\
            }\
            \
            for (i = 5; i < 9; i++){\
                vec4 tmp = texture2DRect(backbuffer, st + offset[i]);\
                sum += tmp * kernel[i];\
            }\
            \
            vec4 color0 = texture2DRect(backbuffer, st + offset[4]);\
            sum += color0 * kernel[4];\
            \
            gl_FragColor = (1.0 - fade_const) * color0 +  fade_const * vec4(sum.rgb, color0.a);\
        }";  
    };
    
	ofxBlurFast& setFade(float _fade) { fade = _fade; return * this;};
    
	void update(){
        pingPong.src->begin();
        texture.draw(0,0);
        pingPong.src->end();
        
        for(int i = 0; i < passes; i++) {
            pingPong.dst->begin();
            shader.begin();
            shader.setUniformTexture("backbuffer", pingPong.src->getTextureReference(), 0 );
            shader.setUniform1f("fade_const", fade);
            renderFrame();
            shader.end();
            pingPong.dst->end();
            
            pingPong.swap();
        }
        
        pingPong.swap();
	}
	
protected:
	float       fade;
};
#endif
