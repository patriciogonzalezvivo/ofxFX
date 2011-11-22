 //
//  ofxWater.cpp
//
//  Created by Patricio Gonzalez Vivo on 9/26/11.
//  Copyright 2011 PatricioGonzalezVivo.com. All rights reserved.
//

#include "ofxWater.h"

ofxWater::ofxWater(){
    frame = 0;
    
    threshold = 0.3;
    blurFade = 0.0005;
    density = 1.0;
    
    texture = new ofTexture();
    texture->allocate(ofGetScreenWidth(), ofGetScreenHeight(), GL_RGB);
    texture->clear();
    
}

ofxWater& ofxWater::loadBackground(string file){ 
    backgroundImage.loadImage(file); 

    delete(texture);
    texture = &(backgroundImage.getTextureReference());
    
    allocate(backgroundImage.getWidth(), backgroundImage.getHeight());
    
    return * this;
}

ofxWater& ofxWater::linkBackground(ofTexture * _backText){
    delete(texture);
    texture = _backText;
    
    return * this;
}

ofxWater& ofxWater::allocate(int tWidth, int tHeight){ 
    width = tWidth; 
    height = tHeight; 
    
    for(int i = 0; i < 2; i++){
        mapFbo[i].allocate(width,height);
        mapFbo[i].begin();
        ofClear(0);
        mapFbo[i].end();
    }
    
    string fragmentUpdateShader = "#version 120\n \
    #extension GL_ARB_texture_rectangle : enable\n \
    \
    uniform sampler2DRect buffer1Sample;\
    uniform sampler2DRect buffer2Sample;\
    \
    uniform float damping;\
    \
    vec2 offset[4];\
    \
    void main(){\
    vec2 st = gl_TexCoord[0].st;\
    \
    offset[0] = vec2(-1.0, 0.0);\
    offset[1] = vec2(1.0, 0.0);\
    offset[2] = vec2(0.0, 1.0);\
    offset[3] = vec2(0.0, -1.0);\
    \
    vec3 sum = vec3(0.0, 0.0, 0.0);\
    \
    for (int i = 0; i < 4 ; i++){\
    sum += texture2DRect(buffer2Sample, st + offset[i]).rgb;\
    }\
    \
    sum = (sum / 2.0) - texture2DRect(buffer1Sample, st).rgb;\
    sum *= damping;\
    \
    gl_FragColor.rgb = sum;\
    gl_FragColor.a = 1.0;\
    }";
    updateShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentUpdateShader);
    updateShader.linkProgram();
    updateFbo.allocate(width,height,GL_RGB);
    updateFbo.begin();
    ofClear(0);
    updateFbo.end();
    
    string fragmentRenderShader = "#version 120\n \
    #extension GL_ARB_texture_rectangle : enable\n \
    \
    uniform sampler2DRect tex;\
    uniform sampler2DRect heightMap;\
    \
    void main(){\
    vec2 pos = gl_TexCoord[0].st;\
    \
    float offsetX = texture2DRect(heightMap, pos + vec2(-1.0, 0.0)).r - texture2DRect(heightMap, pos + vec2(1.0, 0.0)).r;\
    float offsetY = texture2DRect(heightMap, pos + vec2(0.0,- 1.0)).r - texture2DRect(heightMap, pos + vec2(0.0, 1.0)).r;\
    \
    float shading = offsetX;\
    \
    vec3 pixel = texture2DRect(tex, pos + vec2(offsetX, offsetY)).rgb;\
    \
    pixel.r += shading;\
    pixel.g += shading;\
    pixel.b += shading;\
    \
    gl_FragColor.rgb =  pixel;\
    gl_FragColor.a = 1.0;\
    }";
    renderShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentRenderShader);
    renderShader.linkProgram();
    renderFbo.allocate(width,height,GL_RGB);
    renderFbo.begin();
    ofClear(0,255);
    renderFbo.end();
    
    string fragmentBlurShader = "#version 120\n \
    #extension GL_ARB_texture_rectangle : enable\n \
    \
    float kernel[9];\
    \
    uniform sampler2DRect tex;\
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
    vec4 tmp = texture2DRect(tex, st + offset[i]);\
    sum += tmp * kernel[i];\
    }\
    \
    for (i = 5; i < 9; i++){\
    vec4 tmp = texture2DRect(tex, st + offset[i]);\
    sum += tmp * kernel[i];\
    }\
    \
    vec4 color0 = texture2DRect(tex, st + offset[4]);\
    sum += color0 * kernel[4];\
    \
    gl_FragColor = (1.0 - fade_const) * color0 +  fade_const * vec4(sum.rgb, color0.a);\
    }";
    blurShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentBlurShader);
    blurShader.linkProgram();

    return * this;
}

void ofxWater::begin(){
    int buffer2 = (frame+1)%2;  // Prev/Next buffer
    
    // Open the access to the changed buffer
    mapFbo[buffer2].begin();
    
}

void ofxWater::end( bool drawIt ){
    int buffer2 = (frame+1)%2;  // Prev/Next buffer
    // Close the access to the changed buffer
    mapFbo[buffer2].end();
    
    update();
    
    if (drawIt)
        draw();
}

void ofxWater::update(){
    int buffer1 = frame%2;      // Actual buffer
    int buffer2 = (frame+1)%2;  // Prev/Next buffer

    // Calculate the difference between buffers and spread the waving
    updateFbo.begin();
    ofClear(0);
    updateShader.begin();
    updateShader.setUniformTexture("buffer1Sample", mapFbo[buffer1].getTextureReference(), 0);
    updateShader.setUniformTexture("buffer2Sample", mapFbo[buffer2].getTextureReference(), 1);
    updateShader.setUniform1f("damping", (float)density );
    renderFrame();
    updateShader.end();
    updateFbo.end();
    
    // Blur the waving in order to make it smooth
    mapFbo[buffer1].begin();
    blurShader.begin();
    blurShader.setUniformTexture("tex", updateFbo.getTextureReference(), 0);
    blurShader.setUniform1f("fade_const", (float)(blurFade));
    renderFrame();
    blurShader.end();
    mapFbo[buffer1].end();
    
    // Use the buffer as a bumpmap to morph the surface of the background texture
    renderFbo.begin();
    ofClear(0);
    renderShader.begin();
    renderShader.setUniformTexture("tex", *texture , 0);
    renderShader.setUniformTexture("heightMap", updateFbo.getTextureReference(), 1);
    renderFrame();
    renderShader.end();
    renderFbo.end();
    
    // Switch buffers
    frame++;
}

void ofxWater::renderFrame(){
    
    // Rendering canvas frame in order to make it cleaner to read.
    ofSetColor(255,255,255,255);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
	glTexCoord2f(width, 0); glVertex3f(width, 0, 0);
	glTexCoord2f(width, height); glVertex3f(width, height, 0);
	glTexCoord2f(0,height);  glVertex3f(0,height, 0);
	glEnd();
}

void ofxWater::draw(){
    renderFbo.draw(0,0);
    
    // Add more lighten effect
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetColor(100,255);
    updateFbo.draw(0,0);
    ofDisableBlendMode();
}

