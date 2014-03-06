//
//  ofxFlow.h
//
//  Created by Patricio Gonzalez Vivo on 3/2/14.
//  Based on Andrew Benson's GPU OpticalFlow implementation
//

#pragma once

#define STRINGIFY(A) #A

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxFlow : public ofxFXObject {
public:
    ofxFlow(){
        offset = 1.0;
        lambda = 1.0;
        scale = 1.0;
        fade = 0.5;
        
        internalFormat = GL_RGBA;
        
        fragmentShader = STRINGIFY(uniform sampler2DRect tex0;
                                   uniform sampler2DRect tex1;
                                   uniform sampler2DRect backbuffer;
                                   
                                   uniform float fade;
                                   uniform float scale;
                                   uniform float offset;
                                   uniform float lambda;
                                   
                                   float getIntensity(sampler2DRect tex, vec2 u){
                                       vec3 a = texture2DRect(tex,u).xyz;
                                       return (a.x+a.y+a.z)/3.0;
                                   }
                                   
                                   void main(){
                                       vec2 st = gl_TexCoord[0].st;
                                       vec2 pVel = texture2DRect(backbuffer,st).rg;
                                       
                                       float a = getIntensity(tex0,st);
                                       float b = getIntensity(tex1, st);
                                       vec2 x1 = vec2(offset*10.0,0.0);
                                       vec2 y1 = vec2(0.0,offset*10.0);
                                       
                                       float curdif = b-a;
                                       
                                       float gradx = getIntensity(tex1, st+x1)-getIntensity(tex1, st-x1);
                                       gradx += getIntensity(tex0, st+x1)-getIntensity(tex0, st-x1);
                                       
                                       float grady = getIntensity(tex1, st+y1)-getIntensity(tex1, st-y1);
                                       grady += getIntensity(tex0, st+y1)-getIntensity(tex0, st-y1);
                                       
                                       float gradmag = sqrt((gradx*gradx)+(grady*grady)+lambda*0.1);
                                       
                                       vec2 vel = vec2(curdif*(gradx/gradmag),
                                                       curdif*(grady/gradmag));
                                       
                                       pVel -= 0.5;
                                       pVel *= 2.0;
                                       
                                       pVel *= (1.0-fade*0.5);
                                       pVel += vel;
                                       
                                       pVel *= 0.5;
                                       pVel += 0.5;
                                       
                                       gl_FragColor = vec4(pVel.x,pVel.y,0.5,1.0);
                                   }
                                   );
    }
    
    void update(){
        ofPushStyle();
        ofSetColor(255,255);
        
        pingPong.swap();
        pingPong.dst->begin();
        ofClear(0);
        shader.begin();
        shader.setUniformTexture("backbuffer", pingPong.src->getTextureReference(), 0 );
        shader.setUniformTexture("tex0", textures[0].getTextureReference(), 1 );
        shader.setUniformTexture("tex1", textures[1].getTextureReference(), 2 );
        shader.setUniform1f("fade",fade);
        shader.setUniform1f("scale",scale);
        shader.setUniform1f("offset", offset);
        shader.setUniform1f("lambda",lambda);
        renderFrame();
        shader.end();
        pingPong.dst->end();
        
        textures[1].begin();
        textures[0].draw(0,0);
        textures[1].end();
        
        ofPopStyle();
        bUpdate = false;
    };
    
    float scale;
    float fade;
    float lambda;
    float offset;
    
};