//
//  ofxDisplacePixels.h
//
//  Created by Patricio Gonzalez Vivo on 11/24/12.
//  Copyright 2012 http://PatricioGonzalezVivo.com All rights reserved.
//
//  Use a normalMap (tex0) to displace pixels
//

#pragma once

#define STRINGIFY(A) #A

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxDisplacePixels : public ofxFXObject {
public:
    ofxDisplacePixels(){
        passes = 1;
        internalFormat = GL_RGBA;
        
        fragmentShader = STRINGIFY(uniform sampler2DRect backbuffer;
                                   uniform sampler2DRect tex0;          // normalMap that tell the direction where to move the pixels

                                   void main(){
                                       vec2 st = gl_TexCoord[0].st;
                                       
                                       vec4 newFrame = texture2DRect(backbuffer, st);
                                       vec4 color   = vec4(0,0,0,0);
                                       vec2 norm	= ( texture2DRect(tex0, st).rg - 0.5 ) * 2.0;
                                       float inc	= ( abs(norm.x) + abs(norm.y) ) * 0.5;

//                                       vec2 offset[9];
//                                       offset[0] = vec2(-1.0, -1.0);
//                                       offset[1] = vec2(0.0, -1.0);
//                                       offset[2] = vec2(1.0, -1.0);
//                                       offset[3] = vec2(-1.0, 0.0);
//                                       offset[4] = vec2(0.0, 0.0);
//                                       offset[5] = vec2(1.0, 0.0);
//                                       offset[6] = vec2(-1.0, 1.0);
//                                       offset[7] = vec2(0.0, 1.0);
//                                       offset[8] = vec2(1.0, 1.0);
//                                       
//                                       float sources = 0.0;
//                                       for (int i = 0; i < 9; i++){
//                                           if ( i != 4 ){
//                                               vec4 goingTo = ( texture2DRect( tex0, st + offset[i] ) - 0.5 ) * 2.0;
//                                               vec2 match = vec2(goingTo.x + offset[9-i-1].x, goingTo.y + offset[9-i-1].y);
//                                               
//                                               if ( (match.x <= 1.0) && (match.x >= -1.0) && (match.y <= 1.0) && (match.y >= -1.0) ){
//                                                   sources += 1.0;
//                                                   color += texture2DRect(backbuffer, st + offset[i]);
//                                               }
//                                           }
//                                       }
                                       
                                       vec2 offset[36];
                                       int iTotal = 36;
                                       float fTotal = 36.0;
                                       
                                       float pi = 3.14159265358979323846;
                                       float step = (pi*2.0)/fTotal;
                                       float angle = 0.0;
                                       for (int i = 0; i < iTotal; i++) {
                                           offset[i].x = cos(angle);
                                           offset[i].y = sin(angle);
                                           angle += step;
                                       }
                                       
                                       float sources = 0.0;
                                       for (int i = 0; i < iTotal; i++){
                                           vec4 goingTo = ( texture2DRect( tex0, st + offset[i] ) - 0.5 ) * 2.0;
                                           
                                           if ( dot(goingTo.rg,offset[i]) < -1.0/fTotal ){
                                               sources += 1.0;
                                               color += texture2DRect(backbuffer, st + offset[i]);
                                           }
                                       }
                                       
                                       color = color / sources;
                                       
                                       gl_FragColor = color*(1.0-inc) + newFrame*inc ;
                                   }
                                   );
    }
    
    void update(){
        pingPong.dst->begin();
        
        shader.begin();
        shader.setUniformTexture( "tex0" , textures[0].getTextureReference(), 1 );
        
        ofSetColor(255);
        pingPong.src->draw(0, 0);
        
        shader.end();
        pingPong.dst->end();
        
        pingPong.swap();
    };
};
