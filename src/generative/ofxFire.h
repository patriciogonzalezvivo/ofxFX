//
//  ofxFire.h
//
//  Created by Patricio Gonzalez Vivo on 4/12/12.
//  Copyright (c) 2012 http://www.PatricioGonzalezVivo.com. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "ofxFXObject.h"

#define STRINGIFY(A) #A

class ofxFire : public ofxFXObject {
public:
    
    ofxFire(){
        zoom = 60;
        internalFormat = GL_RGBA;
        fragmentShader = STRINGIFY(uniform float time;
                                   uniform float zoom;
                                   uniform vec2 resolution;
                                   
                                   vec2 noise(vec2 n) {
                                       vec2 ret;
                                       ret.x=fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453)*2.0-1.0;
                                       ret.y=fract(sin(dot(n.xy, vec2(34.9865, 65.946)))* 28618.3756)*2.0-1.0;
                                       return normalize(ret);
                                   }
                                   
                                   float perlin(vec2 p) {
                                       vec2 q = vec2(floor(p));
                                       vec2 r = vec2(fract(p));
                                       float s = dot(noise(q),p-q);
                                       float t = dot(noise(vec2(q.x+1.0,q.y)),p-vec2(q.x+1.0,q.y));
                                       float u = dot(noise(vec2(q.x,q.y+1.0)),p-vec2(q.x,q.y+1.0));
                                       float v = dot(noise(vec2(q.x+1.0,q.y+1.0)),p-vec2(q.x+1.0,q.y+1.0));
                                       
                                       float Sx = 3.0*(r.x*r.x)-2.0*(r.x*r.x*r.x);
                                       float a = s+Sx*(t-s);
                                       float b = u+Sx*(v-u);
                                       float Sy = 3.0*(r.y*r.y)-2.0*(r.y*r.y*r.y);
                                       return a+Sy*(b-a);
                                   }
                                   
                                   float fbm(vec2 p) {
                                       float f = 0.0;
                                       float tme =- time*1.0;
                                       f += perlin(p+tme);
                                       f += perlin(p*2.0+tme)*0.5;
                                       f += perlin(p*4.0-tme)*0.25;
                                       f += perlin(p*8.0+tme)*0.125;
                                       f += perlin(p*16.0-tme)*0.0625;
                                       return f;
                                   }
                                   
                                   void main() {
                                       vec2 p = (gl_FragCoord.xy/resolution.xy)*zoom;
                                       p.x  -= resolution.x/resolution.y*5.0;
                                       p.y  -= 5.0;
                                       
                                       float dist = 0.0;
                                       float fx = 0.0;
                                       float col =-p.y/2.0+fbm(p);
                                       
                                       gl_FragColor += vec4(col*4.0,col*2.0,col,1.0);
                                   }
                                   );
    };
    
    void    setZoom(float _zoom){zoom = _zoom;};
    
    void clear(){
        for(int i = 0; i < 2; i++){
            pingPong[i].begin();
            ofClear(255,255);
            pingPong[i].end();
        }
    }
    
    void update(){
        pingPong.dst->begin();
        ofClear(0,0);
        shader.begin();
        
        shader.setUniform1f("time", ofGetElapsedTimef());
        shader.setUniform1f("zoom", zoom);
        shader.setUniform2f("resolution", (float)width, (float)height);
        
        pingPong.src->draw(0,0);
        
        shader.end();
        pingPong.dst->end();
        
        pingPong.swap();
    };
    
    void draw(int _x = -1, int _y = -1, float _width = -1, float _height = -1){
        if (_x == -1) _x = x;
        if (_y == -1) _y = y;
        
        if (_width == -1) _width = width;
        if (_height == -1) _height = height;
        
        getTextureReference().draw(_x, _y, _width, _height);
    }
    
    float   zoom;
};