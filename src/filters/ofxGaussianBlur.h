//
//  ofxBlur.h
//
//  Created by Patricio Gonzalez Vivo on 25/11/11.
//  Copyright (c) 2011 http://PatricioGonzalezVivo.com All rights reserved.
//

#pragma once

#define STRINGIFY(A) #A

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxGaussianBlur : public ofxFXObject {
public:
    
    ofxGaussianBlur(){
        passes = 1;
        internalFormat = GL_RGBA;
        
        radius = 10;
    }
    
	ofxGaussianBlur& setRadius(float _radius) { this->radius = _radius; return * this;};
    
    void update(){
        ofPushStyle();
        
        pingPong.src->begin();
        textures[0].draw(0,0);
        pingPong.src->end();
        
        for(int i = 0; i < passes; i++) {
            for(int j = 0; j < 2; j++) {
                pingPong.dst->begin();
                blurShader[j].begin();
                blurShader[j].setUniformTexture("backbuffer", pingPong.src->getTextureReference(), 0 );
                blurShader[j].setUniform1f("radius", radius);
                renderFrame();
                blurShader[j].end();
                pingPong.dst->end();
                
                pingPong.swap();
            }
        }
        
        pingPong.swap();
        
        ofPopStyle();
	}
    
    float   radius;
    int     passes;
    
protected:
    bool compileCode(){
        nTextures = 1;
        textures = new ofFbo[nTextures];
        
        for( int i = 0; i < nTextures; i++){
            initFbo(textures[i], width, height, internalFormat);
        }
        
        string fragmentHorizontalBlurShader = STRINGIFY(
                                                        uniform sampler2DRect backbuffer;
                                                        uniform float radius;
                                                        
                                                        const float total = (1. + 8. + 28. + 56.) * 2. + 70.;
                                                        
                                                        void main(void) {
                                                            vec2 st = gl_TexCoord[0].st;
                                                            
                                                            vec4 color = vec4(0.0,0.0,0.0,0.0);
                                                            color += (1. / total) * texture2DRect(backbuffer, st - radius * vec2(4. / 4., 0.));
                                                            color += (8. / total)  * texture2DRect(backbuffer, st - radius * vec2(3. / 4., 0.));
                                                            color += (28. / total)  * texture2DRect(backbuffer, st - radius * vec2(2. / 4., 0.));
                                                            color += (56. / total)  * texture2DRect(backbuffer, st - radius * vec2(1. / 4., 0.));
                                                            
                                                            color +=  (70. / total) * texture2DRect(backbuffer, st);
                                                            
                                                            color += (1. / total) * texture2DRect(backbuffer, st + radius * vec2(4. / 4., 0.));
                                                            color += (8. / total)  * texture2DRect(backbuffer, st + radius * vec2(3. / 4., 0.));
                                                            color += (28. / total)  * texture2DRect(backbuffer, st + radius * vec2(2. / 4., 0.));
                                                            color += (56. / total)  * texture2DRect(backbuffer, st + radius * vec2(1. / 4., 0.));
                                                            
                                                            gl_FragColor = color;
//                                                            gl_FragColor.a = 1.0;
                                                        }
                                                        );
        blurShader[0].unload();
        blurShader[0].setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentHorizontalBlurShader);
    	blurShader[0].linkProgram();
        
		string fragmentVerticalBlurShader = STRINGIFY(
                                                      uniform sampler2DRect backbuffer;
                                                      uniform float radius;
                                                      
                                                      const float total = (1. + 8. + 28. + 56.) * 2. + 70.;
                                                      
                                                      void main(void) {
                                                          vec2 st = gl_TexCoord[0].st;
                                                          
                                                          vec4 color = vec4(0.0,0.0,0.0,0.0);
                                                          color += (1. / total) * texture2DRect(backbuffer, st - radius * vec2(0., 4. / 4.));
                                                          color += (8. / total)  * texture2DRect(backbuffer, st - radius * vec2(0., 3. / 4.));
                                                          color += (28. / total)  * texture2DRect(backbuffer, st - radius * vec2(0., 2. / 4.));
                                                          color += (56. / total)  * texture2DRect(backbuffer, st - radius * vec2(0., 1. / 4.));
                                                          
                                                          color +=  (70. / total) * texture2DRect(backbuffer, st);
                                                          
                                                          color += (1. / total) * texture2DRect(backbuffer, st + radius * vec2(0., 4. / 4.));
                                                          color += (8. / total)  * texture2DRect(backbuffer, st + radius * vec2(0., 3. / 4.));
                                                          color += (28. / total)  * texture2DRect(backbuffer, st + radius * vec2(0., 2. / 4.));
                                                          color += (56. / total)  * texture2DRect(backbuffer, st + radius * vec2(0., 1. / 4.));
                                                          
                                                          gl_FragColor = color;
//                                                          gl_FragColor.a = 1.0;
                                                      }
                                                      );
        blurShader[1].unload();
        blurShader[1].setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentVerticalBlurShader);
    	blurShader[1].linkProgram();
        
        return true;
    }
    
    ofShader    blurShader[2];
};