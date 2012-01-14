//
//  ofxFlocking.cpp
//
//  Created by Patricio Gonzalez Vivo on 9/27/11.
//  Copyright 2011 PatricioGonzalezVivo.com. All rights reserved.
//

#include "ofxFlocking.h"

ofxFlocking::ofxFlocking(){
    passes = 1;
    nTextures = 1;
    internalFormat = GL_RGBA;
    
    timeStep = 0.005f;
    
    particleSize = 30.0f;
    
    maxDist = 0.019;
    minDist = 0.02;
    maxSpeed = 1.0f;
    maxForce = 0.5f;
    
    separation = 1.5f;
    alineation = 1.0f;
    cohesion = 1.0f;
    
    fragmentShader = "#version 120\n \
    #extension GL_ARB_texture_rectangle : enable \n \
    #define KERNEL_SIZE 9\n \
    \
    uniform sampler2DRect backbuffer;\
    uniform sampler2DRect tex0;\
    uniform sampler2DRect posData;\
    \
    uniform vec2  screen;\
    uniform int   resolution;\
    \
    uniform float timestep;\
    uniform float minDist;\
    uniform float maxDist;\
    uniform float maxSpeed;\
    uniform float maxForce;\
    \
    uniform float separation;\
    uniform float alineation;\
    uniform float cohesion;\
    \
    vec2 limit(vec2 vector, float max) {\
        float lengthSquared = dot(vector,vector);\
        if( lengthSquared > max*max && lengthSquared > 0.0 ) {\
            float ratio = max/sqrt(lengthSquared);\
            vector.x *= ratio;\
            vector.y *= ratio;\
        }\
        return vector;\
    }\
    \
    vec2 avoid(vec2 pos, vec2 target, bool weight){\
        vec2 steer = target - pos;\
        \
        if(weight)\
            steer *= 1.0/sqrt(distance(pos,target));\
        \
        return steer;\
    }\
    \
    void main(void){\
        vec2 st = gl_TexCoord[0].st;\
        vec2 pos = texture2DRect( posData, st).xy;\
        vec2 vel = texture2DRect( backbuffer, st ).xy;\
        vec2 acc = vec2(0.0,0.0);\
        \
        vec2  oSt;\
        oSt.x = pos.x * screen.x;\
        oSt.y = pos.y * screen.y;\
        float obst = texture2DRect( tex0, oSt).r;\
        \
        if (obst > 0.0){\
            vec2 offset[KERNEL_SIZE];\
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
            float lessDense = 2.0;\
            int lessDenseOffset = 4;\
            for (int i = 0; i < KERNEL_SIZE; i++){\
                if (i != 4){\
                    float nearby = texture2DRect(tex0, oSt + offset[i] ).r;\
                    if ( nearby < lessDense){\
                        lessDense = nearby;\
                        lessDenseOffset = i;\
                    }\
                }\
            }\
            acc += offset[lessDenseOffset] * 0.5;\
        } else {\
            vec2 sep = vec2(0.0,0.0);\
            vec2 ali = vec2(0.0,0.0);\
            vec2 coh = vec2(0.0,0.0);\
            \
            float near = 0.0;\
            float toNear = 0.0;\
            \
            for (int x = 0; x < resolution; x++){\
                for ( int y = 0; y < resolution; y++){\
                    \
                    if(st != vec2(x,y)){\
                        vec2 tPos = texture2DRect( posData, vec2(x,y) ).xy;\
                        vec2 tVel = texture2DRect( backbuffer, vec2(x,y) ).xy;\
                        \
                        vec2 diff = pos - tPos;\
                        float d = length(diff);\
                        \
                        if ( d < maxDist){\
                            ali += tVel;\
                            coh += tPos;\
                            near++;\
                            \
                            if ( d < minDist){\
                                sep += (normalize(diff) / d);\
                                toNear++;\
                            }\
                        }\
                    }\
                }\
            }\
            \
            if (toNear > 0.0){\
                sep /= toNear;\
                sep = normalize(sep);\
                sep *= maxSpeed;\
                sep -= vel;\
                sep = limit(sep,maxForce);\
            }\
            \
            if (near > 0.0){\
                ali /= near;\
                ali = normalize(ali);\
                ali *= maxSpeed;\
                ali -= vel;\
                ali = limit(ali,maxForce);\
                \
                coh /= near;\
                coh -= pos;\
                coh = normalize(coh);\
                coh *= maxSpeed;\
                coh -= vel;\
                coh = limit(coh,maxForce);\
            }\
            \
            acc += sep * separation;\
            acc += ali * alineation;\
            acc += coh * cohesion;\
        }\
        \
        vel += acc;\
        vel = limit(vel,maxSpeed);\
        \
        vec2 nextPos = pos;\
        nextPos += vel * timestep;\
        \
        if ( nextPos.x < 0.0)\
            vel.x = 0.5 * abs(vel.x);\
        \
        if ( nextPos.x > 1.0)\
            vel.x = -0.5 * abs(vel.x);\
        \
        if (nextPos.y < 0.0)\
            vel.y = 0.5 * abs(vel.y);\
        \
        if ( nextPos.y > 1.0)\
            vel.y = -0.5 * abs(vel.y);\
        \
        gl_FragColor = vec4(vel.x,vel.y,0.0,1.0);\
    }";
    
    string fragmentPosUpdateShader = "#version 120\n \
    #extension GL_ARB_texture_rectangle : enable \n \
    \
    uniform sampler2DRect prevPosData;\
    uniform sampler2DRect velData;\
    \
    uniform float timestep;\
    \
    void main(void){\
        vec2 st = gl_TexCoord[0].st;\
        \
        vec2 pos = texture2DRect( prevPosData, st ).xy;\
        vec2 vel = texture2DRect( velData, st ).xy;\
        \
        pos += vel * timestep;\
        \
        gl_FragColor.rgba = vec4(pos.x,pos.y,1.0,1.0);\
    }";
    posUpdateShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentPosUpdateShader);
    posUpdateShader.linkProgram();
    
    renderShader.unload();
    renderShader.setGeometryInputType(GL_POINTS);
	renderShader.setGeometryOutputType(GL_TRIANGLE_STRIP);
	renderShader.setGeometryOutputCount(6);
    
    string fragmentRenderShader = "#version 120\n \
    #extension GL_ARB_texture_rectangle : enable \n \
    \
    uniform sampler2DRect sparkTex;\
    void main() {\
        vec2 st = gl_TexCoord[0].st;\
        gl_FragColor = texture2DRect(sparkTex, st);\
    }";
    renderShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentRenderShader);
    
    string vertexRenderShader = "#version 120 \n \
    #extension GL_EXT_gpu_shader4 : enable \n \
    #extension GL_ARB_texture_rectangle : enable \n \
    \
    uniform sampler2DRect posTex;\
    uniform vec2 screen;\
    uniform int resolution;\
    \
    void main() {\
        vec4 pos = gl_Vertex;\
        \
        pos.x *= screen.x;\
        pos.y *= screen.y;\
        \
        gl_Position = pos;\
        gl_FrontColor =  gl_Color;\
    }";
    renderShader.setupShaderFromSource(GL_VERTEX_SHADER, vertexRenderShader);
    
    string geometryRenderShader = "#version 120 \n \
    #extension GL_EXT_geometry_shader4 : enable \n \
    #extension GL_EXT_gpu_shader4 : enable \n \
    \
    uniform float size;\
    uniform float imgWidth, imgHeight; \
    \
    void main(void){\
    for(int i = 0; i < gl_VerticesIn; i++){\
        gl_Position = gl_ModelViewProjectionMatrix * ( gl_PositionIn[i] + vec4(-size,-size,0.0,0.0));\
        gl_TexCoord[0].x = 0.0;\
        gl_TexCoord[0].y = 0.0;\
        EmitVertex();\
        \
        gl_Position = gl_ModelViewProjectionMatrix * (gl_PositionIn[i] + vec4(size,-size,0.0,0.0));\
        gl_TexCoord[0].x = imgWidth;\
        gl_TexCoord[0].y = 0.0;\
        EmitVertex();\
        \
        gl_Position = gl_ModelViewProjectionMatrix * (gl_PositionIn[i] + vec4(size,size,0.0,0.0));\
        gl_TexCoord[0].x = imgWidth;  \
        gl_TexCoord[0].y = imgHeight;  \
        EmitVertex();\
        EndPrimitive();\
        \
        gl_Position = gl_ModelViewProjectionMatrix * (gl_PositionIn[i] + vec4(-size,-size,0.0,0.0));\
        gl_TexCoord[0].x = 0.0;  \
        gl_TexCoord[0].y = 0.0;  \
        EmitVertex();\
        \
        gl_Position = gl_ModelViewProjectionMatrix * (gl_PositionIn[i] + vec4(-size,size,0.0,0.0));\
        gl_TexCoord[0].x = 0.0;  \
        gl_TexCoord[0].y = imgHeight;  \
        EmitVertex();\
        \
        gl_Position = gl_ModelViewProjectionMatrix * (gl_PositionIn[i] + vec4(size,size,0.0,0.0));\
        gl_TexCoord[0].x = imgWidth;  \
        gl_TexCoord[0].y = imgHeight;  \
        EmitVertex();\
        EndPrimitive();\
    }\
    }";
    
    renderShader.unload();
    renderShader.setupShaderFromSource(GL_GEOMETRY_SHADER, geometryRenderShader);
    renderShader.linkProgram();
}


void ofxFlocking::allocate(int _width, int _height, int _nBoids){
    width = _width;
    height = _height;
    
    injectShader();
    
    resolution = (int) sqrt(_nBoids);
    nBoids = resolution * resolution;
    
    sparkImage.loadImage("spark.png");
    imageWidth = sparkImage.getWidth();
    imageHeight = sparkImage.getHeight();
    
    // 1. Process birth and death
    float * pos = new float[nBoids*3];    
    float offset = 1.0/resolution;
    for (int x = 0; x < resolution; x++){
        for (int y = 0; y < resolution; y++){
            int i = resolution * y + x;
            
            pos[i*3 + 0] = ofRandom(1.0); //x*offset;
            pos[i*3 + 1] = ofRandom(1.0); //y*offset;
            pos[i*3 + 2] = 0.0;
        }
    }
    
    posBuffer.allocate(resolution, resolution,GL_RGB32F);
    posBuffer.src->getTextureReference().loadData(pos, resolution, resolution, GL_RGB);
    posBuffer.dst->getTextureReference().loadData(pos, resolution, resolution, GL_RGB);
    
    delete(pos);
    
    float * vel = new float[nBoids*3];
    for (int i = 0; i < nBoids; i++){
        vel[i*3 + 0] = ofRandom(-1.0,1.0);
        vel[i*3 + 1] = ofRandom(-1.0,1.0);
        vel[i*3 + 2] = 1.0;
    }
    
    pingPong.allocate(resolution, resolution,GL_RGB32F);
    pingPong.src->getTextureReference().loadData(vel, resolution, resolution, GL_RGB);
    pingPong.dst->getTextureReference().loadData(vel, resolution, resolution, GL_RGB);
    
    delete(vel);
    
    pixels.allocate(resolution,resolution, 4);
    particles = new ofPoint[nBoids];
    for (int i = 0; i < nBoids; i++){
        particles[i].set(0.0,0.0,0.0);
    }
    
    initFbo(renderFbo, width, height);
    //initFbo(textures[0], width, height);
}

void ofxFlocking::update(){
    // 2. Update Velocities
    pingPong.dst->begin();
    ofClear(0);
    shader.begin();
    shader.setUniformTexture("backbuffer", pingPong.src->getTextureReference(), 0);
    shader.setUniformTexture("tex0", textures[0].getTextureReference(), 1);
    shader.setUniformTexture("posData", posBuffer.src->getTextureReference(), 2);
    shader.setUniform1i("resolution", (int)resolution); 
    shader.setUniform2f("screen", (float)width, (float)height);
    shader.setUniform1f("timestep", (float)timeStep);
    shader.setUniform1f("minDist",(float) minDist );
    shader.setUniform1f("maxDist",(float) maxDist );
    shader.setUniform1f("maxSpeed",(float) maxSpeed );
    shader.setUniform1f("maxForce",(float) maxForce);
    shader.setUniform1f("separation",(float) separation );
    shader.setUniform1f("alineation",(float) alineation );
    shader.setUniform1f("cohesion",(float) cohesion );
    renderFrame(resolution,resolution);
    shader.end();
    pingPong.dst->end();
    
    pingPong.swap();
    
    // 3. Update Positions
    posBuffer.dst->begin();
    ofClear(0);
    posUpdateShader.begin();
    posUpdateShader.setUniformTexture("prevPosData", posBuffer.src->getTextureReference(), 0);
    posUpdateShader.setUniformTexture("velData", pingPong.src->getTextureReference(), 1);
    posUpdateShader.setUniform1f("timestep",(float) timeStep );
    renderFrame(resolution,resolution);
    posUpdateShader.end();
    posBuffer.dst->end();
    
    posBuffer.swap();
    
    posBuffer.src->readToPixels(pixels);
    for(int x = 0; x < resolution; x++){
        for(int y = 0; y < resolution; y++){
            ofFloatColor p = pixels.getColor(x, y);
            int i = y * resolution + x;
            particles[i].set(p.r,p.g,p.b);
        }
    }
    
    // 4. sort for alpha blending (optional)
    
    
    
    // 5. Transfer texture data to vertex data
    renderFbo.begin();
    ofClear(0);
    renderShader.begin();
    renderShader.setUniformTexture("posTex", posBuffer.dst->getTextureReference(), 0);
    renderShader.setUniformTexture("sparkTex", sparkImage.getTextureReference() , 1);
    renderShader.setUniform1i("resolution", (int)resolution); 
    renderShader.setUniform2f("screen", (float)width, (float)height);
    renderShader.setUniform1f("size", (float)particleSize); 
    renderShader.setUniform1f("imgWidth", (float)sparkImage.getWidth());
    renderShader.setUniform1f("imgHeight", (float)sparkImage.getHeight());
    
    ofPushStyle();
    ofEnableBlendMode( OF_BLENDMODE_ADD );
    ofSetColor(255);
    glBegin( GL_POINTS );
    for(int i = 0; i < nBoids; i++){
        glVertex3d(particles[i].x, particles[i].y,0.0);
    }
    ofDisableBlendMode();
    glEnd();
    renderShader.end();
    renderFbo.end();
    ofPopStyle();
}

void ofxFlocking::draw(int x, int y, float _width, float _height){
    if (_width == -1) _width = width;
    if (_height == -1) _height = height;
    
    ofPushStyle();
    ofSetColor(255, 255, 255);
    textures[0].draw(x,y,_width,_height);
    ofSetColor(055, 255, 0);
    renderFbo.draw(x,y,_width,_height);
    ofPopStyle();
}