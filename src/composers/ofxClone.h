//
//  ofxClone.h
//
//  The shaders and class was created by Arturo Castro and Kyle McDonald 
//  for FaceSubstitution https://github.com/arturoc/FaceSubstitution
//  
//  Then addapted to ofxFXObject structure
//
//  src ->  tex0
//  mask -> tex1 
//  dst ->  tex2
//

#pragma once

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxClone : public ofxFXObject {	
public:
    ofxClone(){
        passes = 1;
        internalFormat = GL_RGBA;
        
        strength = 0;
        
        fragmentShader = "#version 120\n\
#extension GL_ARB_texture_rectangle : enable\n\
\n\
uniform sampler2DRect tex0;\n\
uniform sampler2DRect tex1;\n\
uniform sampler2DRect tex2;\n\
\n\
void main() {\n\
    vec2 st = gl_TexCoord[0].st;\n\
    vec4 srcColorBlur = texture2DRect(tex1, st);\n\
    \n\
    if(srcColorBlur.a > 0.) {\n\
        vec3 srcColor = texture2DRect(tex0, st).rgb;\n\
        vec4 dstColorBlur = texture2DRect(tex2, st);\n\
        vec3 offset = dstColorBlur.rgb - srcColorBlur.rgb;\n\
        gl_FragColor = vec4(srcColor + offset, 1.);\n\
    } else {\n\
        gl_FragColor = vec4(0.);\n\
    }\n\
}\n ";
        
        string fragmentMaskBlurShader = "#version 120\n \
        #extension GL_ARB_texture_rectangle : enable\n \
        \
        uniform sampler2DRect tex; \
        uniform sampler2DRect mask;\
        \
        uniform vec2 direction;\
        uniform int k;\
        \
        void main() {\
            vec2 st = gl_TexCoord[0].st;\
            vec4 sum = texture2DRect(tex, st);\
            int i;\
            \
            for(i = 1; i < k; i++) {\
                vec2 curOffset = float(i) * direction;\
                vec4 leftMask = texture2DRect(mask, st - curOffset);\
                vec4 rightMask = texture2DRect(mask, st + curOffset);\
                bool valid = leftMask.r == 1. && rightMask.r == 1.;\
                if(valid) {\
                    sum +=\
                    texture2DRect(tex, st + curOffset) +\
                    texture2DRect(tex, st - curOffset);\
                } else {\
                    break;\
                }\
            }\
            int samples = 1 + (i - 1) * 2;\
            gl_FragColor = sum / float(samples);\
        }";
        
        maskBlurShader.unload();
        maskBlurShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentMaskBlurShader);
        maskBlurShader.linkProgram();
        
        buffer.allocate(640,480);
        srcBlur.allocate(640,480);
        dstBlur.allocate(640,480);
    }
    
	void setStrength(int _strength) { strength = _strength;};
    
    void update() {
        maskedBlur(textures[0].getTextureReference() , textures[1].getTextureReference(), srcBlur);
        maskedBlur(textures[2].getTextureReference() , textures[1].getTextureReference(), dstBlur);
        
        ofPushStyle();
        ofEnableAlphaBlending();
        
        pingPong.dst->begin();
        
        textures[2].draw(0, 0);	
        shader.begin();
        shader.setUniformTexture("tex0", textures[0], 1);
        shader.setUniformTexture("tex1", srcBlur, 2);
        shader.setUniformTexture("tex2", dstBlur, 3);
        textures[2].draw(0, 0);
        
        shader.end();
        ofDisableAlphaBlending();
        ofPopStyle();
        
        pingPong.dst->end();
    }
	
    int strength;
protected:
    void maskedBlur(ofTexture& tex, ofTexture& mask, ofFbo& result){
        int k = strength;
        
        buffer.begin();
        maskBlurShader.begin();
        maskBlurShader.setUniformTexture("tex", tex, 1);
        maskBlurShader.setUniformTexture("mask", mask, 2);
        maskBlurShader.setUniform2f("direction", 1, 0);
        maskBlurShader.setUniform1i("k", k);
        tex.draw(0, 0);
        maskBlurShader.end();
        buffer.end();
        
        result.begin();
        maskBlurShader.begin();
        maskBlurShader.setUniformTexture("tex", buffer, 1);
        maskBlurShader.setUniformTexture("mask", mask, 2);
        maskBlurShader.setUniform2f("direction", 0, 1);
        maskBlurShader.setUniform1i("k", k);
        buffer.draw(0, 0);
        maskBlurShader.end();
        result.end();
    };
    
	ofFbo       buffer, srcBlur, dstBlur;
	ofShader    maskBlurShader;
};
