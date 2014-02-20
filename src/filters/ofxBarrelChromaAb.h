//
//  BarrelChromaAb.h
//
//  Created by Patricio Gonzalez Vivo on 5/15/13.
//
//

#pragma once

#define STRINGIFY(A) #A

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxBarrelChromaAb : public ofxFXObject {
public:
    ofxBarrelChromaAb(){
        passes = 1;
        internalFormat = GL_RGBA;
        
        
        zoom = 0.99;
        maxDistort = 1.2;
        offset.set(0.0,0.0);//99);
        
        fragmentShader = STRINGIFY(uniform sampler2DRect tex0;
                                   uniform vec2 resolution;
                                   uniform vec2 offset;
                                   uniform float zoom;
                                   uniform float max_distort;
                                   
                                   vec2 barrelDistortion(vec2 coord, float amt) {
                                       vec2 cc = coord - 0.5;
                                       float dist = dot(cc, cc);
                                       return coord + cc * dist * amt;
                                   }
                                   
                                   float sat( float t ){
                                       return clamp( t, 0.0, 1.0 );
                                   }
                                   
                                   float linterp( float t ) {
                                       return sat( 1.0 - abs( 2.0*t - 1.0 ) );
                                   }
                                   
                                   float remap( float t, float a, float b ) {
                                       return sat( (t - a) / (b - a) );
                                   }
                                   
                                   vec3 spectrum_offset( float t ) {
                                       vec3 ret;
                                       float lo = step(t,0.5);
                                       float hi = 1.0-lo;
                                       float w = linterp( remap( t, 1.0/6.0, 5.0/6.0 ) );
                                       ret = vec3(lo,1.0,hi) * vec3(1.0-w, w, 1.0-w);
                                       
                                       return pow( ret, vec3(1.0/2.2) );
                                   }
                                   
                                   const int num_iter = 12;
                                   const float reci_num_iter_f = 1.0 / float(num_iter);
                                   
                                   void main(){	
                                       vec2 st = gl_FragCoord.xy;// * vec2(1,-1);
                                       vec2 uv = (st/resolution.xy*zoom)+offset;
                                       
                                       vec3 sumcol = vec3(0.0);
                                       vec3 sumw = vec3(0.0);	
                                       for ( int i=0; i<num_iter;++i ){
                                           float t = float(i) * reci_num_iter_f;
                                           vec3 w = spectrum_offset( t );
                                           sumw += w;
                                           sumcol += w * texture2DRect( tex0, barrelDistortion(uv, max_distort*t)*resolution.xy ).rgb;
                                       }
                                       
                                       gl_FragColor = vec4(sumcol.rgb / sumw, 1.0);
                                   });
    }
    
    void update(){
        pingPong.dst->begin();
        
//        ofClear(0);
        shader.begin();
        
//        shader.setUniformTexture( "tex0" , textures[0].getTextureReference(), 0 );
        shader.setUniform2f("resolution", width, height);
        shader.setUniform2f("offset", offset.x, offset.y );
        shader.setUniform1f("max_distort", maxDistort);
        shader.setUniform1f("zoom", zoom);
        renderFrame();
        
        textures[0].getTextureReference().draw(0,0);
        
        shader.end();
        
        pingPong.dst->end();
    };
    
    ofVec2f offset;
    float zoom;
    float maxDistort;
};