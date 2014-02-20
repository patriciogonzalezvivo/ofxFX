/*
 *  ofxGlow.h
 *
 *  Created by Patricio Gonzalez Vivo on 03/07/11.
 *  Copyright 2011 http://www.PatricioGonzalezVivo.com All rights reserved.
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
 *  This is a simple implementation of Glow based on bluring and adding the orriginal layer. 
 *  There is a better way of doing this by a shader... comming soon...
 *
 */

#pragma once

#include "ofMain.h"

#include "ofxGaussianBlur.h"
class ofxGlow : public ofxGaussianBlur {	
public:
    ofxGlow(){
        passes = 1;
        internalFormat = GL_RGBA;
        
        radius = 3;
    }
    
	void draw(int x = 0, int y = 0, float _width = -1, float _height = -1){
        if (_width == -1) _width = width;
        if (_height == -1) _height = height;
        
        ofPushStyle();
        pingPong.dst->draw(x, y, _width, _height);
		//ofEnableBlendMode(OF_BLENDMODE_ADD);
		glBlendFunc(GL_ONE, GL_ONE);
        textures[0].draw(x,y, _width, _height);
        //ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        ofPopStyle();
	}
};
//
//#define STRINGIFY(A) #A
//
//#include "ofMain.h"
//#include "ofxFXObject.h"
//
//class ofxGlow : public ofxFXObject {
//public:
//    ofxGlow(){
//        passes = 1;
//        internalFormat = GL_RGBA;
//        
//        fragmentShader = STRINGIFY(uniform sampler2DRect tex0;
//                                   uniform vec2 mouse;
//                                   uniform vec2 resolution;
//                                   
//                                   vec3 highlights(vec3 pixel, float thres){
//                                       float val = (pixel.x + pixel.y + pixel.z) / 3.0;
//                                       return pixel * smoothstep(thres - 0.1, thres + 0.1, val);
//                                   }
//                                   
//                                   vec3 sample(vec2 tc){
//                                       return pow( texture2DRect(tex0, tc).xyz, vec3(2.2, 2.2, 2.2));
//                                   }
//                                   
//                                   vec3 hsample(vec2 tc){
//                                       return highlights(sample(tc), 0.6);
//                                   }
//                                   
//                                   vec3 blur(vec2 tc, float offs){
//                                       vec4 xoffs = offs * vec4(-2.0, -1.0, 1.0, 2.0);// / iResolution.x;
//                                       vec4 yoffs = offs * vec4(-2.0, -1.0, 1.0, 2.0);// / iResolution.y;
//                                       
//                                       vec3 color = vec3(0.0, 0.0, 0.0);
//                                       color += hsample(tc + vec2(xoffs.x, yoffs.x)) * 0.00366;
//                                       color += hsample(tc + vec2(xoffs.y, yoffs.x)) * 0.01465;
//                                       color += hsample(tc + vec2(    0.0, yoffs.x)) * 0.02564;
//                                       color += hsample(tc + vec2(xoffs.z, yoffs.x)) * 0.01465;
//                                       color += hsample(tc + vec2(xoffs.w, yoffs.x)) * 0.00366;
//                                       
//                                       color += hsample(tc + vec2(xoffs.x, yoffs.y)) * 0.01465;
//                                       color += hsample(tc + vec2(xoffs.y, yoffs.y)) * 0.05861;
//                                       color += hsample(tc + vec2(    0.0, yoffs.y)) * 0.09524;
//                                       color += hsample(tc + vec2(xoffs.z, yoffs.y)) * 0.05861;
//                                       color += hsample(tc + vec2(xoffs.w, yoffs.y)) * 0.01465;
//                                       
//                                       color += hsample(tc + vec2(xoffs.x, 0.0)) * 0.02564;
//                                       color += hsample(tc + vec2(xoffs.y, 0.0)) * 0.09524;
//                                       color += hsample(tc + vec2(    0.0, 0.0)) * 0.15018;
//                                       color += hsample(tc + vec2(xoffs.z, 0.0)) * 0.09524;
//                                       color += hsample(tc + vec2(xoffs.w, 0.0)) * 0.02564;
//                                       
//                                       color += hsample(tc + vec2(xoffs.x, yoffs.z)) * 0.01465;
//                                       color += hsample(tc + vec2(xoffs.y, yoffs.z)) * 0.05861;
//                                       color += hsample(tc + vec2(    0.0, yoffs.z)) * 0.09524;
//                                       color += hsample(tc + vec2(xoffs.z, yoffs.z)) * 0.05861;
//                                       color += hsample(tc + vec2(xoffs.w, yoffs.z)) * 0.01465;
//                                       
//                                       color += hsample(tc + vec2(xoffs.x, yoffs.w)) * 0.00366;
//                                       color += hsample(tc + vec2(xoffs.y, yoffs.w)) * 0.01465;
//                                       color += hsample(tc + vec2(    0.0, yoffs.w)) * 0.02564;
//                                       color += hsample(tc + vec2(xoffs.z, yoffs.w)) * 0.01465;
//                                       color += hsample(tc + vec2(xoffs.w, yoffs.w)) * 0.00366;
//                                       
//                                       return color;
//                                   }
//                                   
//                                   void main(void){
//                                       vec2 tc = gl_FragCoord.xy;
//                                       vec3 color = blur(tc, 2.0);
//                                       color += blur(tc, 3.0);
//                                       color += blur(tc, 5.0);
//                                       color += blur(tc, 7.0);
//                                       color /= 4.0;
//            
//                                       color += sample(tc);
//            
//                                       float divider = smoothstep(-0.01, 0.01, tc.x);
//                                       gl_FragColor.xyz = mix(sample(tc), color, divider) * (divider * divider + (1.0 - divider) * (1.0 - divider));
//                                       gl_FragColor.w = 1.0;
//                                   }
//                                   
//                                   
//                                   );
//    }
//};
