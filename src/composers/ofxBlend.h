//
//  ofxBlend.h
//
//  Created by Akira Hayasaka on 5/14/12.
//

#pragma once

#include "ofMain.h"
#include "ofxFXObject.h"

#define STRINGIFY(A) #A

typedef enum {
    BLEND_NORMAL,
    BLEND_MULTIPLY,
    BLEND_AVERAGE,
    BLEND_ADD,
    BLEND_SUBSTRACT,
    BLEND_DIFFERENCE,
    BLEND_NEGATION,
    BLEND_EXCLUSION,
    BLEND_SCREEN,
    BLEND_OVERLAY,
    BLEND_SOFTLIGHT,
    BLEND_HARDLIGHT,
    BLEND_COLORDODGE,
    BLEND_COLORBURN,
    BLEND_LINEARLIGHT,
    BLEND_VIVIDLIGHT,
    BLEND_PINLIGHT,
    BLEND_HARDMIX,
    BLEND_REGLECT,
    BLEND_GLOW,
    BLEND_PHOENIX,
    BLEND_HUE,
    BLEND_SATURATION,
    BLEND_COLOR,
    BLEND_LUMINOSITY
} PSBlendMode;

class ofxBlend : public ofxFXObject {	
public:
    ofxBlend(){
        passes          = 1;
        blendMode       = BLEND_NORMAL;
        internalFormat  = GL_RGBA;
        
        fragmentShader  = "#version 120\n \
    	#extension GL_ARB_texture_rectangle : enable\n \
        \
        #define BlendLinearDodgef BlendAddf\n \
        #define BlendLinearBurnf BlendSubstractf\n \
        #define BlendAddf(base, blend) min(base + blend, 1.0)\n \
        #define BlendSubstractf(base, blend) max(base + blend - 1.0, 0.0)\n \
        #define BlendLightenf(base, blend) max(blend, base)\n \
        #define BlendDarkenf(base, blend) min(blend, base)\n \
        #define BlendLinearLightf(base, blend) (blend < 0.5 ? BlendLinearBurnf(base, (2.0 * blend)) : BlendLinearDodgef(base, (2.0 * (blend - 0.5))))\n \
        #define BlendScreenf(base, blend) (1.0 - ((1.0 - base) * (1.0 - blend)))\n \
        #define BlendOverlayf(base, blend) (base < 0.5 ? (2.0 * base * blend) : (1.0 - 2.0 * (1.0 - base) * (1.0 - blend)))\n \
        #define BlendSoftLightf(base, blend) ((blend < 0.5) ? (2.0 * base * blend + base * base * (1.0 - 2.0 * blend)) : (sqrt(base) * (2.0 * blend - 1.0) + 2.0 * base * (1.0 - blend)))\n \
        #define BlendColorDodgef(base, blend) ((blend == 1.0) ? blend : min(base / (1.0 - blend), 1.0))\n \
        #define BlendColorBurnf(base, blend) ((blend == 0.0) ? blend : max((1.0 - ((1.0 - base) / blend)), 0.0))\n \
        #define BlendVividLightf(base, blend) ((blend < 0.5) ? BlendColorBurnf(base, (2.0 * blend)) : BlendColorDodgef(base, (2.0 * (blend - 0.5))))\n \
        #define BlendPinLightf(base, blend) ((blend < 0.5) ? BlendDarkenf(base, (2.0 * blend)) : BlendLightenf(base, (2.0 *(blend - 0.5))))\n \
        #define BlendHardMixf(base, blend) ((BlendVividLightf(base, blend) < 0.5) ? 0.0 : 1.0)\n \
        #define BlendReflectf(base, blend) ((blend == 1.0) ? blend : min(base * base / (1.0 - blend), 1.0))\n \
        #define Blend(base, blend, funcf) vec3(funcf(base.r, blend.r), funcf(base.g, blend.g), funcf(base.b, blend.b))\n \
        #define BlendLightenBlendLightenf\n \
        #define BlendDarkenBlendDarkenf\n \
        #define BlendNormal(base, blend) (blend)\n \
        #define BlendMultiply(base, blend) (base * blend)\n \
        #define BlendAverage(base, blend) ((base + blend) / 2.0)\n \
        #define BlendAdd(base, blend) min(base + blend, vec3(1.0))\n \
        #define BlendSubstract(base, blend) max(base + blend - vec3(1.0), vec3(0.0))\n \
        #define BlendDifference(base, blend) abs(base - blend)\n \
        #define BlendNegation(base, blend) (vec3(1.0) - abs(vec3(1.0) - base - blend))\n \
        #define BlendExclusion(base, blend) (base + blend - 2.0 * base * blend)\n \
        #define BlendScreen(base, blend) Blend(base, blend, BlendScreenf)\n \
        #define BlendOverlay(base, blend) Blend(base, blend, BlendOverlayf)\n \
        #define BlendSoftLight(base, blend) Blend(base, blend, BlendSoftLightf)\n \
        #define BlendHardLight(base, blend) BlendOverlay(blend, base)\n \
        #define BlendColorDodge(base, blend) Blend(base, blend, BlendColorDodgef)\n \
        #define BlendColorBurn(base, blend) Blend(base, blend, BlendColorBurnf)\n \
        #define BlendLinearDodgeBlendAdd\n \
        #define BlendLinearBurnBlendSubstract\n \
        #define BlendLinearLight(base, blend) Blend(base, blend, BlendLinearLightf)\n \
        #define BlendVividLight(base, blend) Blend(base, blend, BlendVividLightf)\n \
        #define BlendPinLight(base, blend) Blend(base, blend, BlendPinLightf)\n \
        #define BlendHardMix(base, blend) Blend(base, blend, BlendHardMixf)\n \
        #define BlendReflect(base, blend) Blend(base, blend, BlendReflectf)\n \
        #define BlendGlow(base, blend) BlendReflect(blend, base)\n \
        #define BlendPhoenix(base, blend) (min(base, blend) - max(base, blend) + vec3(1.0))\n \
        #define BlendOpacity(base, blend, F, O) (F(base, blend) * O + blend * (1.0 - O))\n \
        #define GammaCorrection(color, gamma)pow(color, 1.0 / gamma)\n \
        #define LevelsControlInputRange(color, minInput, maxInput)min(max(color - vec3(minInput), vec3(0.0)) / (vec3(maxInput) - vec3(minInput)), vec3(1.0))\n \
        #define LevelsControlInput(color, minInput, gamma, maxInput)GammaCorrection(LevelsControlInputRange(color, minInput, maxInput), gamma)\n \
        #define LevelsControlOutputRange(color, minOutput, maxOutput) mix(vec3(minOutput), vec3(maxOutput), color)\n \
        #define LevelsControl(color, minInput, gamma, maxInput, minOutput, maxOutput) LevelsControlOutputRange(LevelsControlInput(color, minInput, gamma, maxInput), minOutput, maxOutput)\n ";
        
        fragmentShader += STRINGIFY(
                                    vec4 Desaturate(vec3 color, float Desaturation){
                                        vec3 grayXfer = vec3(0.3, 0.59, 0.11);
                                        vec3 gray = vec3(dot(grayXfer, color));
                                        return vec4(mix(color, gray, Desaturation), 1.0);
                                    }
                                    
                                    vec3 RGBToHSL(vec3 color){
                                        vec3 hsl;
                                        float fmin = min(min(color.r, color.g), color.b);
                                        float fmax = max(max(color.r, color.g), color.b);
                                        float delta = fmax - fmin;
                                        hsl.z = (fmax + fmin) / 2.0;
                                        if (delta == 0.0){
                                            hsl.x = 0.0;
                                            hsl.y = 0.0;
                                        } else {
                                            if (hsl.z < 0.5)
                                                hsl.y = delta / (fmax + fmin);
                                            else
                                                hsl.y = delta / (2.0 - fmax - fmin);
                                            
                                            float deltaR = (((fmax - color.r) / 6.0) + (delta / 2.0)) / delta;
                                            float deltaG = (((fmax - color.g) / 6.0) + (delta / 2.0)) / delta;
                                            float deltaB = (((fmax - color.b) / 6.0) + (delta / 2.0)) / delta;
                                            
                                            if (color.r == fmax )
                                                hsl.x = deltaB - deltaG;
                                            else if (color.g == fmax)
                                                hsl.x = (1.0 / 3.0) + deltaR - deltaB;
                                            else if (color.b == fmax)
                                                hsl.x = (2.0 / 3.0) + deltaG - deltaR;
                                            
                                            if (hsl.x < 0.0)
                                                hsl.x += 1.0;
                                            else if (hsl.x > 1.0)
                                                hsl.x -= 1.0;
                                        }
                                        return hsl;
                                    }
                                    
                                    float HueToRGB(float f1, float f2, float hue){
                                        if (hue < 0.0)
                                            hue += 1.0;
                                        else if (hue > 1.0)
                                            hue -= 1.0;
                                        
                                        float res;
                                        if ((6.0 * hue) < 1.0)
                                            res = f1 + (f2 - f1) * 6.0 * hue;
                                        else if ((2.0 * hue) < 1.0)
                                            res = f2;
                                        else if ((3.0 * hue) < 2.0)
                                            res = f1 + (f2 - f1) * ((2.0 / 3.0) - hue) * 6.0;
                                        else
                                            res = f1;
                                        
                                        return res;
                                    }
                                    
                                    vec3 HSLToRGB(vec3 hsl){
                                        vec3 rgb;
                                        if (hsl.y == 0.0)
                                            rgb = vec3(hsl.z);
                                        else {
                                            float f2;
                                            
                                            if (hsl.z < 0.5)
                                                f2 = hsl.z * (1.0 + hsl.y);
                                            else
                                                f2 = (hsl.z + hsl.y) - (hsl.y * hsl.z);
                                            
                                            float f1 = 2.0 * hsl.z - f2;
                                            rgb.r = HueToRGB(f1, f2, hsl.x + (1.0/3.0));
                                            rgb.g = HueToRGB(f1, f2, hsl.x);
                                            rgb.b= HueToRGB(f1, f2, hsl.x - (1.0/3.0));
                                        }
                                        return rgb;
                                    }
                                    
                                    vec3 ContrastSaturationBrightness(vec3 color, float brt, float sat, float con){
                                        const float AvgLumR = 0.5;
                                        const float AvgLumG = 0.5;
                                        const float AvgLumB = 0.5;
                                        const vec3 LumCoeff = vec3(0.2125, 0.7154, 0.0721);
                                        vec3 AvgLumin = vec3(AvgLumR, AvgLumG, AvgLumB);
                                        vec3 brtColor = color * brt;
                                        vec3 intensity = vec3(dot(brtColor, LumCoeff));
                                        vec3 satColor = mix(intensity, brtColor, sat);
                                        vec3 conColor = mix(AvgLumin, satColor, con);
                                        return conColor;
                                    }
                                    
                                    vec3 BlendHue(vec3 base, vec3 blend){
                                        vec3 baseHSL = RGBToHSL(base);
                                        return HSLToRGB(vec3(RGBToHSL(blend).r, baseHSL.g, baseHSL.b));
                                    }
                                    
                                    vec3 BlendSaturation(vec3 base, vec3 blend){
                                        vec3 baseHSL = RGBToHSL(base);
                                        return HSLToRGB(vec3(baseHSL.r, RGBToHSL(blend).g, baseHSL.b));
                                    }
                                    
                                    vec3 BlendColor(vec3 base, vec3 blend){
                                        vec3 blendHSL = RGBToHSL(blend);
                                        return HSLToRGB(vec3(blendHSL.r, blendHSL.g, RGBToHSL(base).b));
                                    }
                                    
                                    vec3 BlendLuminosity(vec3 base, vec3 blend){
                                        vec3 baseHSL = RGBToHSL(base);
                                        return HSLToRGB(vec3(baseHSL.r, baseHSL.g, RGBToHSL(blend).b));
                                    }
                                    
                                    uniform sampler2DRect backbuffer;
                                    uniform sampler2DRect tex0;
                                    uniform int mode;
                                    
                                    void main(){
                                        vec4 baseCol = texture2DRect(backbuffer, gl_TexCoord[0].st);
                                        vec4 blendCol = texture2DRect(tex0, gl_TexCoord[0].st);
                                        
                                        vec3 result;
                                        if (mode == 0){
                                            result = mix(baseCol.rgb, blendCol.rgb, blendCol.a);
                                        } else if (mode == 1) {
                                            result = BlendMultiply(baseCol.rgb, blendCol.rgb);
                                        } else if (mode == 2) {
                                            result = BlendAverage(baseCol.rgb, blendCol.rgb);
                                        } else if (mode == 3) {
                                            result = BlendAdd(baseCol.rgb, blendCol.rgb);
                                        } else if (mode == 4) {
                                            result = BlendSubstract(baseCol.rgb, blendCol.rgb);
                                        } else if (mode == 5) {
                                            result = BlendDifference(baseCol.rgb, blendCol.rgb);
                                        } else if (mode == 6) {
                                            result = BlendNegation(baseCol.rgb, blendCol.rgb);
                                        } else if (mode == 7) {
                                            result = BlendExclusion(baseCol.rgb, blendCol.rgb);
                                        } else if (mode == 8) {
                                            result = BlendScreen(baseCol.rgb, blendCol.rgb);
                                        } else if (mode == 9) {
                                            result = BlendOverlay(baseCol.rgb, blendCol.rgb);
                                        } else if (mode == 10){
                                            result = BlendSoftLight(baseCol.rgb, blendCol.rgb);
                                        } else if (mode == 11){
                                            result = BlendHardLight(baseCol.rgb, blendCol.rgb);
                                        } else if (mode == 12){
                                            result = BlendColorDodge(baseCol.rgb, blendCol.rgb);
                                        } else if (mode == 13){
                                            result = BlendColorBurn(baseCol.rgb, blendCol.rgb);
                                        } else if (mode == 14){
                                            result = BlendLinearLight(baseCol.rgb, blendCol.rgb);
                                        } else if (mode == 15){
                                            result = BlendVividLight(baseCol.rgb, blendCol.rgb);
                                        } else if (mode == 16){
                                            result = BlendPinLight(baseCol.rgb, blendCol.rgb);
                                        } else if (mode == 17){
                                            result = BlendHardMix(baseCol.rgb, blendCol.rgb);
                                        } else if (mode == 18){
                                            result = BlendReflect(baseCol.rgb, blendCol.rgb);
                                        } else if (mode == 19){
                                            result = BlendGlow(baseCol.rgb, blendCol.rgb);
                                        } else if (mode == 20){
                                            result = BlendPhoenix(baseCol.rgb, blendCol.rgb);
                                        } else if (mode == 21){
                                            result = BlendHue(baseCol.rgb, blendCol.rgb);
                                        } else if (mode == 22){
                                            result = BlendSaturation(baseCol.rgb, blendCol.rgb);
                                        } else if (mode == 23){
                                            result = BlendColor(baseCol.rgb, blendCol.rgb);
                                        } else if (mode == 24){
                                            result = BlendLuminosity(baseCol.rgb, blendCol.rgb);
                                        } else {
                                            result = BlendNormal(baseCol.rgb, blendCol.rgb);
                                        }
                                        
                                        gl_FragColor = vec4(result, max(baseCol.a, min(blendCol.a*3.0,1.0) ) );
                                    }
        
                                    );
    }
  
    //gl_FragColor = vec4(result, max( max(result.r,max(result.g,result.b)),blendCol.a));
    
    void    setBlendMode( PSBlendMode _blendMode){ blendMode = _blendMode; };
    
    void    begin(int _texNum = 0 ) {
        if ((_texNum < nTextures) && ( _texNum >= 0)){
            ofPushStyle();
            ofPushMatrix();
            textures[_texNum].begin();
            //ofClear(0,0);
        }
    }
    
    void    end(int _texNum = 0) {
        if ((_texNum < nTextures) && ( _texNum >= 0)){
            textures[_texNum].end();
            ofPopMatrix();
            ofPopStyle();
        }
    }
    
    void    addLayer( ofTexture& tex, PSBlendMode _blendMode){
        setBlendMode(_blendMode);
        
        textures[0].begin(); 
            ofClear(0,0);
            ofSetColor(255);
            tex.draw(0,0); 
        textures[0].end();
    };
    
    void    beginBackground(){
        pingPong.dst->begin();
    }
    
    void    endBackground(){
        pingPong.dst->end();
    }
    
    void    update(){
        
        ofEnableAlphaBlending();
        
        pingPong.swap();
        
        pingPong.dst->begin();
        //ofClear(0,0);
        ofSetColor(255,255);
        
        shader.begin();
        shader.setUniformTexture("tex0", textures[0].getTextureReference(), 1);
        shader.setUniformTexture("backbuffer", pingPong.src->getTextureReference(), 0);
        shader.setUniform1i("mode", blendMode); 
        
        renderFrame();
        shader.end();
        pingPong.dst->end();
    };

    void    draw(int _x, int _y){
        pingPong.dst->draw(_x,_y);
    }
    
    int blendMode;
};
