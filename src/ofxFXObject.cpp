/*
 *  ofxFXObject.h
 *
 *  Created by Patricio González Vivo on 03/07/11.
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
 *  Definition: Parent class of all the other effect. It also can be use in order to compile single and simple
 *  shaders as the ones you could find at: Ricardo Caballero´s webGL Sandbox http://mrdoob.com/projects/glsl_sandbox/ 
 *  and ShaderToy by Inigo Quilez http://www.iquilezles.org/apps/shadertoy/
 *  
 */ 

#include "ofxFXObject.h"

ofxFXObject::ofxFXObject():nTextures(0){
    // Simple effect just need this three variables
    // For something more complex that require another structure, logic or more shaders working together
    // think on making a new stand-alone class as the ofxBlur, ofxFluid, ofxGlow, etc ...
    // Collaborations are welcome
    
    passes = 1;                 // Number of itinerations needs. Default it´s 1;
    
    internalFormat = GL_RGBA;   // Tipe of GL textures 
    
    // And the fragSahder it self. Note that are defaul variables:
    //
    // - time
    // - mouse position (normalized)
    // - resolution
    // - backbuffer texture
    // - tex0, tex1, tex2, ... : this are dynamicaly defined and allocated and can be
    //   filled with information by using .begin(0) and .end(0), or .begin(1) and .end(1), etc 
    //
    // This dafault shader it´s timer made of a mix on Ricardo Caballero´s webGL Sandbox
    // http://mrdoob.com/projects/glsl_sandbox/
    //
    
    fragmentShader = STRINGIFY(uniform float time;
                               uniform vec2 mouse;
                               uniform vec2 resolution;
                               
                               float box( vec2 p, vec4 rect ){
                                   float trim = min(rect.z, rect.w) * 0.5;
                                   float minX = min(p.x - rect.x, rect.x + rect.z - p.x);
                                   float minY = min(p.y - rect.y, rect.y + rect.w - p.y);
                                   return ((minX > 0.0) && (minY > 0.0) && ((minX + minY) > trim)) ? 1.0 : 0.0;
                               }
                               
                               float digit( vec2 p, vec4 dim, float d){
                                   d = (d - mod(d,1.0)) / 10.0;
                                   d = mod( d, 1.0 );
                                   
                                   p.xy -= dim.xy;
                                   p.xy /= dim.zw;
                                   
                                   float c = 0.0;
                                   
                                   c += box( p, vec4( 0.05, 0.9, 0.9, 0.1 ) ) * ( cos( (0.85*d+0.1)*30.0) - sin(pow(d,1.0)) < 0.0 ? 1.0 : 0.0 );
                                   c += box( p, vec4( 0.05, 0.45, 0.9, 0.1 ) ) * ( min( pow(6.0*d,2.0), pow(20.0*(d-0.7),2.0) ) < 1.0 ? 0.0 : 1.0 );
                                   c += box( p, vec4( 0.05, 0.0, 0.9, 0.1 ) ) * ( max( cos(18.6*pow(d,0.75)), 1.0-pow(40.0*(d-0.8),2.0)) > 0.0 ? 1.0 : 0.0 );
                                   c += box( p, vec4( 0.0, 0.08, 0.1, 0.39 ) ) * ( cos( d * 30.0 ) * abs(d-0.4) > 0.1 ? 1.0 : 0.0 );
                                   c += box( p, vec4( 0.9, 0.08, 0.1, 0.39) ) * ( pow( 4.0*d-0.8, 2.0) > 0.1 ? 1.0 : 0.0 );
                                   c += box( p, vec4( 0.0, 0.52, 0.1, 0.39 ) ) * ( sin((d-0.05)*10.5) - 12.0*sin(pow(d,10.0)) > 0.0 ? 0.0 : 1.0 );
                                   c += box( p, vec4( 0.9, 0.52, 0.1, 0.39 ) ) * ( pow( d-0.55, 2.0 ) > 0.02 ? 1.0 : 0.0 );
                                   
                                   return c;
                               }
                               
                               void main( void ){
                                   vec2 p = (gl_FragCoord.xy / resolution.xy);
                                   p.y = 1.0 - p.y;
                                   
                                   float c= 0.0;
                                   c += ( time < 100.0 ) ? 0.0 : digit( p, vec4( 0.2, 0.5, 0.09, 0.1 ), time/100.0 );
                                   c += ( time < 10.0) ? 0.0 : digit( p, vec4( 0.3, 0.5, 0.09, 0.1 ), time/10.0 );
                                   c += digit( p, vec4( 0.4, 0.5, 0.09, 0.1 ), time );
                                   c += box( p, vec4( 0.5, 0.5, 0.01, 0.01 ) );
                                   c += digit( p, vec4( 0.52, 0.5, 0.09, 0.1 ), time*10.0 );
                                   
                                   gl_FragColor = vec4( 0.0, c * 0.5, c, 1.0 )*(abs(sin(time*0.5))+0.5);
                               }
                               );
}

ofxFXObject::ofxFXObject(ofxFXObject& parent){
    passes = parent.getPasses();
    internalFormat = parent.getInternalFormat();
    fragmentShader = parent.getCode();
}

ofxFXObject::~ofxFXObject(){
    if (textures != NULL ){
        if (nTextures > 0) {
            delete [] textures;
        }
    }
}

ofxFXObject& ofxFXObject::operator =(ofxFXObject& parent){
    passes = parent.getPasses();
    internalFormat = parent.getInternalFormat();
    fragmentShader = parent.getCode();
    ofVec2f resolution = parent.getResolution();
    allocate(resolution.x, resolution.y);
    
    return * this;
}


void ofxFXObject::allocate(int _width, int _height, int _internalFormat){
    internalFormat = _internalFormat;
    allocate(_width, _height);
};

void ofxFXObject::allocate(int _width, int _height){
    width = _width;
    height = _height;
    
    pingPong.allocate(width, height, internalFormat);
    compileCode();
};

bool ofxFXObject::setCode(string _fragShader){
    bool loaded = false;
    
    if ( fragmentShader != _fragShader ){
        
        ofShader test;
        test.setupShaderFromSource(GL_FRAGMENT_SHADER, _fragShader);
        bFine = test.linkProgram();
        
        if( bFine ){
            fragmentShader = _fragShader;
            loaded = compileCode();
        }
    }
    
    return loaded;
}

bool ofxFXObject::compileCode(){
    
    // Looks how many textures it´s need on the injected fragment shader
    int num = 0;
    for (int i = 0; i < 10; i++){
        string searchFor = "tex" + ofToString(i);
        if ( fragmentShader.find(searchFor)!= -1)
            num++;
        else 
            break;
    }
    
    // Check if it´s the same number of tectures already created and allocated
    if ( num != nTextures ){
        // If the number of textures it´s different
        if (textures != NULL ){
            if (nTextures > 0) {
                delete [] textures;
            }
        }
            // And initialate the right amount of textures
        nTextures = num;
        if (nTextures > 0){
            textures = new ofFbo[nTextures];
        } else if ( nTextures == 0 ){
            textures = NULL;
        }
        
        // In any case it will allocate the total amount of textures with the internalFormat need
        for( int i = 0; i < nTextures; i++){
            initFbo(textures[i], width, height, internalFormat);
        }
    }
    
    //bool loaded;
    // Compile the shader and loadit to the GPU
    shader.unload();
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
    bFine = shader.linkProgram();
    
    return bFine;
}

// A simplified way of filling the insides texture
void ofxFXObject::setTexture(ofTexture& tex, int _texNum){ 
    if ((_texNum < nTextures) && ( _texNum >= 0)){
        textures[_texNum].begin(); 
        ofClear(0,255);
        ofSetColor(255);
        tex.draw(0,0); 
        textures[_texNum].end();
    }
};

// With begin(int) and end(int) the textures allocated can be filled with data
void ofxFXObject::begin(int _texNum ) {
    if ((_texNum < nTextures) && ( _texNum >= 0)){
        ofPushStyle();
        ofPushMatrix();
        textures[_texNum].begin();
        ofClear(0,255);
    }
}

void ofxFXObject::end(int _texNum) {
    if ((_texNum < nTextures) && ( _texNum >= 0)){
        textures[_texNum].end();
        ofPopMatrix();
        ofPopStyle();
    }
}

// UPDATE
// -------------------------------------
//

// As most objects on openFrameworks, ofxFXObject have to be updated() in order to process the information on the GPU
void ofxFXObject::update(){
    
    // This process is going to be repited as many times as passes variable said
    for(int i = 0; i < passes; i++) {
        
        // All the process it´s done on the pingPong ofxSwapBuffer ( basicaly two ofFbo that have a swap() funtion )
        pingPong.dst->begin();
        
        ofClear(0);
        shader.begin();
        
        // The other ofFbo of the ofxSwapBuffer can be access by calling the unicode "backbuffer"
        shader.setUniformTexture("backbuffer", pingPong.src->getTextureReference(), 0 );
        
        // All the needed textures are provided to the shader by this loop
        for( int i = 0; i < nTextures; i++){
            string texName = "tex" + ofToString(i); 
            shader.setUniformTexture(texName.c_str(), textures[i].getTextureReference(), i+1 );
            string texRes = "size" + ofToString(i); 
            shader.setUniform2f(texRes.c_str() , (float)textures[i].getWidth(), (float)textures[i].getHeight());
        }
        
        // Also there are some standar variables that are passes to the shaders
        // this ones follows the standar used by Ricardo Caballero's webGL Sandbox
        // http://mrdoob.com/projects/glsl_sandbox/ and ShaderToy by Inigo Quilez http://www.iquilezles.org/apps/shadertoy/
        // webGL interactive GLSL editors
        //
        shader.setUniform1f("time", ofGetElapsedTimef() );
        shader.setUniform2f("size", (float)width, (float)height);
        shader.setUniform2f("resolution", (float)width, (float)height);
        shader.setUniform2f("mouse", (float)(ofGetMouseX()/width), (float)(ofGetMouseY()/height));
        
        // renderFrame() is a built-in funtion of ofxFXObject that only draw a white box in order to 
        // funtion as a frame here the textures could rest.
        // If you want to distort the points of a textures, probably you want to re-define the renderFrame funtion.
        renderFrame();
        
        shader.end();
        
        pingPong.dst->end();
        
        pingPong.swap();    // Swap the ofFbo's. Now dst is src and src is dst
    }
    
    pingPong.swap();        // After the loop the render information will be at the src ofFbo of the ofxSwapBuffer 
    // this extra swap() call will put it on the dst one. Witch sounds more reasonable...
};

// DRAW
// --------------------------------------

// Finaly the drawing funtion. It can be use with or with-out arguments in order to make it more flexible
void ofxFXObject::draw(int _x, int _y, float _width, float _height){
    if (_x == -1) _x = x;
    if (_y == -1) _y = y;
    
    if (_width == -1) _width = width;
    if (_height == -1) _height = height;
    
    ofPushStyle();
    ofEnableAlphaBlending();
    pingPong.dst->draw(_x, _y, _width, _height);
    ofPopStyle();
}

// BUILD-IN functions & variables
// -------------------------------------
//

// Allocates and cleans an ofFbo´s
void ofxFXObject::initFbo(ofFbo & _fbo, int _width, int _height, int _internalformat) {
    _fbo.allocate(_width, _height, _internalformat);
    _fbo.begin();
    ofClear(0,255);
    _fbo.end();
}

// Draw a white box in order to let the final texture could be render
// It acts as a frame where the dst textures could rest.
// If you want to distort the points of a textures, probably here you want to re-define something
void ofxFXObject::renderFrame(float _width, float _height){
    if (_width == -1) _width = width;
    if (_height == -1) _height = height;
    
    // If it´s not well compiled it will show an image little more gray.
    //
    if (bFine)
        ofSetColor(255,255);  
    else
        ofSetColor(150,255);
    
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
    glTexCoord2f(_width, 0); glVertex3f(_width, 0, 0);
    glTexCoord2f(_width, _height); glVertex3f(_width, _height, 0);
    glTexCoord2f(0,_height);  glVertex3f(0,_height, 0);
    glEnd();
}
