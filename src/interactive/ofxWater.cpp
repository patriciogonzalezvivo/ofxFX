//
//  ofxWater.cpp
//
//  Created by Patricio Gonzalez Vivo on 9/26/11.
//  Copyright 2011 http://www.patriciogonzalezvivo.com/ All rights reserved.
//

#include "ofxWater.h"

ofxWater::ofxWater(){
    passes = 1;
    internalFormat = GL_RGBA32F ;

    density = 0.995;
    
    fragmentShader = STRINGIFY(
    uniform sampler2DRect backbuffer;   // previus buffer
    uniform sampler2DRect tex0;         // actual buffer
                               
    //  This two are not going to be used in this shader
    //  but are need to tell ofxFXObject that need to create them
    //
    uniform sampler2DRect tex1;         // is going to be the background
    uniform sampler2DRect tex2;         // is going to be the render FBO
    
    uniform float damping;
    
    vec2 offset[4];
    
    void main(){
        vec2 st = gl_TexCoord[0].st;
        
        offset[0] = vec2(-1.0, 0.0);
        offset[1] = vec2(1.0, 0.0);
        offset[2] = vec2(0.0, 1.0);
        offset[3] = vec2(0.0, -1.0);
        
        //  Grab the information arround the active pixel
        //
        //      [3]
        //
        //  [0]  st  [1]
        //
        //      [2]
        
        vec3 sum = vec3(0.0, 0.0, 0.0);
        
        for (int i = 0; i < 4 ; i++){
            sum += texture2DRect(tex0, st + offset[i]).rgb;
        }
        
        //  make an average and substract the center value
        //
        sum = (sum / 2.0) - texture2DRect(backbuffer, st).rgb;
        sum *= damping;
        
        gl_FragColor = vec4(sum, 1.0);
    } );
    shader.unload();
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
    shader.linkProgram();
    
    // This map the desplacement to the background 
    //
    string fragmentRenderShader = STRINGIFY(
    uniform sampler2DRect tex0; //  background
    uniform sampler2DRect tex1; //  displacement
    
    void main(){
        vec2 st = gl_TexCoord[0].st;
        
        float offsetX = texture2DRect(tex1, st + vec2(-1.0, 0.0)).r - texture2DRect(tex1, st + vec2(1.0, 0.0)).r;
        float offsetY = texture2DRect(tex1, st + vec2(0.0,- 1.0)).r - texture2DRect(tex1, st + vec2(0.0, 1.0)).r;
        
        float shading = offsetX;
        
        vec4 pixel = texture2DRect(tex0, st + vec2(offsetX, offsetY));
        
        pixel.r += shading;
        pixel.g += shading;
        pixel.b += shading;
        
        gl_FragColor =  pixel;
    } );
    renderShader.unload();
    renderShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentRenderShader);
    renderShader.linkProgram();
}

ofxWater& ofxWater::loadBackground(string file){
    ofImage backgroundImage;
    backgroundImage.loadImage(file); 
    allocate(backgroundImage.getWidth(), backgroundImage.getHeight());
    
    textures[0].begin();
    backgroundImage.draw(0,0);
    textures[0].end();
    
    return * this;
}

ofxWater& ofxWater::linkBackground(ofTexture * _backText){
    textures[0].begin();
    _backText->draw(0,0);
    textures[0].end();
    
    return * this;
}

void ofxWater::begin() {
    ofPushStyle();
    ofPushMatrix();
    pingPong.src->begin();
}

void ofxWater::end() {
    pingPong.src->end();
    ofPopMatrix();
    ofPopStyle();
}

void ofxWater::update(){
    // Calculate the difference between buffers and spread the waving
    textures[1].begin();
    shader.begin();
    shader.setUniformTexture("backbuffer", pingPong.dst->getTextureReference(), 0);
    shader.setUniformTexture("tex0", pingPong.src->getTextureReference(), 1);
    shader.setUniform1f("damping", (float)density );
    renderFrame();
    shader.end();
    textures[1].end();
    
    // TODO: improve this, it's almost non-sense
    pingPong.dst->begin();
    textures[1].draw(0, 0);
    pingPong.dst->end();
    
    // Use the buffer as a bumpmap to morph the surface of the background texture
    textures[2].begin();
    ofClear(0);
    renderShader.begin();
    renderShader.setUniformTexture("tex0", textures[0].getTextureReference(), 0);
    renderShader.setUniformTexture("tex1", textures[1].getTextureReference(), 1);
    renderFrame();
    renderShader.end();
    textures[2].end();
    
    // Switch buffers
    pingPong.swap();
}

void ofxWater::draw(int x, int y, float _width, float _height){
    if (_width == -1) _width = width;
    if (_height == -1) _height = height;
    textures[2].draw(x,y, _width, _height);
}

