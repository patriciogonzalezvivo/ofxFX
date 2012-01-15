#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
	
    int width = 640;
    int height = 480;
    
    ofSetWindowShape(width, height);
    
#ifdef THERE_IS_CAM
    video.initGrabber(width , height);
#else
    image.loadImage("logo.jpg");
#endif
    
    frags[0] = "#version 120\n\
    #extension GL_ARB_texture_rectangle : enable\n \
    uniform float time;\
    uniform vec2 mouse;\
    uniform vec2 resolution;\
     \
    float viewAngle = 1.0;\
    float speed = 9.0;\
    float rate = 15.0;\
    float baseamp = 0.10;\
     \
    void main(void){\
        vec2 p = -1.0 + 3.0 * ( gl_FragCoord.xy / resolution.xy );\
        p.y = 1.0 - p.y;\
        float x = speed * viewAngle * time + rate * p.x;\
        float base = (1.0 + cos(x * 2.5 + time)) * (1.0 + sin(x * 3.5 + time));\
        float z = fract(0.05 * x);\
        z = max(z, 1.0 - z);\
        z = pow(z, 2.0);\
        float pulse = exp(-10.0 * z);\
        vec4 ecg_color = vec4(0.3, 1.0, 0.4, 1.0);\
        vec4 c = pow(clamp(1.0 - abs(p.y - (baseamp * base + pulse - 0.5)), 0.0, 1.0), 16.0) * ecg_color;\
        gl_FragColor = c;\
    }";
    
    frags[1] = "#version 120\n\
                    #extension GL_ARB_texture_rectangle : enable\n \
                    uniform float time; \
                    uniform vec2 mouse;\
                    uniform vec2 resolution;\
                    \
                    float pi = 3.141592653589793238462643383279;\
                    \
                    void main( void ) {\
                        float tscale = 2.5;\
                        \
                        vec2 position = gl_FragCoord.xy / resolution.xy;\
                        \
                        float lum   = abs(tan(position.y * pi)) - pi/5.0;\
                        \
                        float red   = sin(position.x * 5.0 + time*tscale*1.00) * 2.0 - 1.0;\
                        float green = sin(position.x * 8.0 + time*tscale*1.33) * 2.0 - 1.0;\
                        float blue  = sin(position.x * 2.0 + time*tscale*1.93) * 2.0 - 1.0;\
                        \
                        gl_FragColor = vec4( vec3( lum + red, lum + green, lum + blue ), 1.0 );\
                    }";
    
    frags[2] =  "#version 120\n\
                    #extension GL_ARB_texture_rectangle : enable\n \
                    uniform float time;\
                    uniform vec2 mouse;\
                    uniform vec2 resolution;\
                    \
                    void main( void ){ \
                        vec2 position = (gl_FragCoord.xy - resolution * 0.5) / resolution.yy;\
                        \
                        float longest = sqrt(float(resolution.x*resolution.x) + float(resolution.y*resolution.y))*0.5;\
                        float dx = gl_FragCoord.x-resolution.x/2.0;\
                        float dy = 0.2+gl_FragCoord.y-resolution.y/2.0;\
                        float len = sqrt(dx*dx+dy*dy);\
                        float ds = len/longest;\
                        float md = time*2.0;\
                        \
                        float ang = 2.0*atan(dy,(len+dx));\
                        ang += pow(len, 0.5)*5.0;\
                        \
                        float red = (128.0 - sin(ang + md*3.141592*2.0) * 127.0)*(1.0-ds);\
                        float green = (128.0 - cos(ang + md*3.141592*2.0) * 127.0)*(1.0-ds);\
                        float blue = (128.0 + sin(ang  + md*3.141592*2.0) * 127.0)*(1.0-ds);\
                        \
                        gl_FragColor = vec4( vec3( red/255.0, green/255.0, blue/255.0), 1.0 );\
                    }";
    
    frags[3] =   "#version 120\n\
                #extension GL_ARB_texture_rectangle : enable\n \
                \
                uniform float time;\
                uniform vec2 mouse;\
                uniform vec2 resolution;\
                \
                float rand(vec2 co){\
                    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);\
                }\
                \
                float noise2f( in vec2 p ){ \
                    vec2 ip = vec2(floor(p));\
                    vec2 u = fract(p);\
                    u = u*u*(3.0-2.0*u);\
                    \
                    float res = mix(mix(rand(ip),  rand(ip+vec2(1.0,0.0)),u.x),\
                                    mix(rand(ip+vec2(0.0,1.0)),   rand(ip+vec2(1.0,1.0)),u.x),\
                                    u.y);\
                    return res*res;\
                }\
                \
                float fbm(vec2 c) {\
                    float f = 0.0;\
                    float w = 1.0;\
                    for (int i = 0; i < 8; i++) {\
                        f += w*noise2f(c);\
                        c *=2.0;\
                        w*=0.5;\
                    }\
                    return f;\
                }\
                \
                vec2 cMul(vec2 a, vec2 b) {\
                    return vec2( a.x*b.x -  a.y*b.y,a.x*b.y + a.y * b.x);\
                }\
                \
                float pattern(  vec2 p, out vec2 q, out vec2 r ){\
                    q.x = fbm( p  +0.00*time);\
                    q.y = fbm( p + vec2(1.0));\
                    \
                    r.x = fbm( p +1.0*q + vec2(1.7,9.2)+0.15*time );\
                    r.y = fbm( p+ 1.0*q + vec2(8.3,2.8)+0.126*time);\
                    return fbm(p +1.0*r + 0.0* time);\
                }\
                \
                const vec3 color1 = vec3(0.101961,0.619608,0.666667);\
                const vec3 color2 = vec3(0.666667,0.666667,0.498039);\
                const vec3 color3 = vec3(0,0,0.164706);\
                const vec3 color4 = vec3(0.666667,1,1);\
                \
                void main() {\
                    vec2 q;\
                    vec2 r;\
                    vec2 c = 1000.0 * gl_FragCoord.xy / resolution.xy;\
                    float f = pattern( c * 0.01, q, r);\
                    vec3 col = mix( color1, color2, clamp( (f*f) * 4.0,0.0,1.0));\
                    col = color2;\
                    col = mix(col,color3,clamp(length(q),0.0,1.0));\
                    col = mix(col,color4,clamp(length(r.x),0.0,1.0));\
                    gl_FragColor =  vec4((0.2*f*f*f+0.6*f*f+0.5*f)*col,1.0);\
                }";
    
    //
    // Description : Array and textureless GLSL 2D simplex noise function.
    //      Author : Ian McEwan, Ashima Arts.
    //  Maintainer : ijm
    //     Lastmod : 20110822 (ijm)
    //     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
    //               Distributed under the MIT License. See LICENSE file.
    //               https://github.com/ashima/webgl-noise
    // 
    
    frags[4] =   "#version 120\n\
                #extension GL_ARB_texture_rectangle : enable\n \
                \
                uniform float time;\
                uniform vec2 mouse;\
                uniform vec2 resolution;\
                uniform sampler2D backbuffer;\
                \
                vec3 mod289(vec3 x) {\
                    return x - floor(x * (1.0 / 289.0)) * 289.0;\
                }\
                \
                vec2 mod289(vec2 x) {\
                    return x - floor(x * (1.0 / 289.0)) * 289.0;\
                }\
                \
                vec3 permute(vec3 x) {\
                    return mod289(((x*34.0)+1.0)*x);\
                }\
                \
                float snoise(vec2 v){\
                    const vec4 C = vec4(0.211324865405187, 0.366025403784439,  -0.577350269189626, 0.024390243902439);\
                    vec2 i  = floor(v + dot(v, C.yy) );\
                    vec2 x0 = v -   i + dot(i, C.xx);\
                    \
                    vec2 i1;\
                    \
                    i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);\
                    \
                    vec4 x12 = x0.xyxy + C.xxzz;\
                    x12.xy -= i1;\
                    \
                    i = mod289(i);\
                    vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 )) + i.x + vec3(0.0, i1.x, 1.0 ));\
                    \
                    vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);\
                    m = m*m ;\
                    m = m*m ;\
                    \
                    vec3 x = 2.0 * fract(p * C.www) - 1.0;\
                    vec3 h = abs(x) - 0.5;\
                    vec3 ox = floor(x + 0.5);\
                    vec3 a0 = x - ox;\
                    \
                    m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );\
                    \
                    vec3 g;\
                    g.x  = a0.x  * x0.x  + h.x  * x0.y;\
                    g.yz = a0.yz * x12.xz + h.yz * x12.yw;\
                    return 130.0 * dot(m, g);\
                }\
                \
                void main( void ) {\
                    vec4 buf = texture2D(backbuffer, (gl_FragCoord.xy / resolution.xy));\
                    vec2 position = ( gl_FragCoord.xy + time + 100.0);\
                    float color = snoise(position /1000.0 + snoise(vec2(time / 100.0, time / 100.0 + 100.))) * 0.9 + snoise(position / 10.0) * 0.3 + snoise(position / 1.0) * 0.2;\
                    \
                    gl_FragColor = vec4( vec3( color ), 1.0 );\
                }";
    
    
    frags[5] = "#version 120\n\
    #extension GL_ARB_texture_rectangle : enable\n \
    \
    uniform float time;\
    uniform vec2 mouse;\
    uniform vec2 resolution;\
    \
    float getValue(vec2 p, float x2, float y2, float cmin, float cmax){\
        float x=p.x;\
        float y=p.y;\
        float theta=atan(y-y2, x+x2);\
        vec2 d=vec2(cos(theta * x2), sin(theta * cmin));\
        d *= abs(sin(time)) * (cmax-cmin) + cmin;\
        d += vec2(x2, y2) + length(p) * dot(y,x);\
        d *= vec2(y2, x2) * length(p * (time) * 0.1) * distance(y,x);\
        return length(d-p);\
    }\
    \
    void main( void ) {\
        vec2 position = ( gl_FragCoord.xy / resolution.xy ) * 2.0 - vec2(1.0,resolution.y/resolution.x);\
        position = position * 2.0 ;\
        \
        float a = 0.0;\
        a += getValue(position, tan(time)*.77, sin(time)*.2, 5.3, 0.8);\
    	a *= getValue(position, cos(time)*.1, cos(-1.*time)*.1, 8.3, 0.18);\
        a += getValue(position, sin(time)*.8, cos(time)*.2, 4.3, 1.18);\
        a *= getValue(position, tan(time)*.3, cos(time)*.8, -1.3, 5.18);\
        a = 1./a*1.75;\
        \
        float b = 0.0;\
        b += getValue(position, cos(time)*.81, sin(time)*.2, 5.3, 0.8);\
    	b *= getValue(position, sin(time)*.1, tan(time)*.2, 1.3, 1.18);\
        b += getValue(position, tan(time)*.8, cos(time)*.01, 7.3, 1.18);\
        b *= getValue(position, cos(time)*.8, cos(time)*.2, -1.3, 5.18);\
        b = 1./b*1.75;\
        \
        float c = 0.0;\
        c += getValue(position, cos(time)*.41, cos(time)*.2, 5.3, 10.8);\
    	c *= getValue(position, sin(time)*.1, sin(time)*.1, 1.3, 1.18);\
        c += getValue(position, tan(time)*.8, tan(time)*.2, 4.3, 0.18);\
        c *= getValue(position, cos(time)*1.8, sin(time)*.8, -10.3, 2.18);\
        c = 1./c*1.75;\
        \
        gl_FragColor = vec4( a, b, c, 1.0 );\
    }";
    
    // blobbies - by @P_Malin
    frags[6] = "uniform float time;\
    uniform vec2 mouse;\
    uniform vec2 resolution;\
    \
    float k_PI = 3.141592654;\
    float k_MoveSpeed = 0.5;\
    float k_BlobSize = 0.5;\
    float k_OrbitRadius = 0.5;\
    \
    vec2 GetRandom2( in vec2 vInput ){\
        vec2 temp1 = vInput * vec2(17.1232, 33.32432);\
        vec2 temp2 = vInput * vec2(34.32432, 12.9742);\
        vec2 temp3 = vec2(sin(temp1.x) + sin(temp2.y), sin(temp1.y) + sin(temp2.x)) * 121.1231;\
        return fract(temp3);\
    }\
    \
    vec2 GetOffset2( in vec2 vInput ){\
        return vec2(sin(vInput.x * k_PI * 2.0), cos(vInput.y * k_PI * 2.0));\
    }\
    \
    float BlobTexture( in vec2 vPos ){\
        float fLen = length(vPos) / k_BlobSize;\
        return max(1.0 - fLen, 0.0);\
    }\
    \
    float GetBlobValue( in vec2 vPosition, in vec2 vTileOffset ){\
        vec2 vTilePos = floor(vPosition - vTileOffset);\
        \
        vec2 vRandom = GetRandom2(vTilePos);\
        vec2 vTime2d = vec2(time, time * 0.92312) * k_MoveSpeed;\
        \
        vec2 vBlobPos = GetOffset2(vRandom + vTime2d) * k_OrbitRadius;\
        \
        vec2 vSubTilePos = fract(vPosition) + vTileOffset;\
        \
        return BlobTexture(vSubTilePos - vBlobPos);\
    }\
    \
    vec3 GetColour( in vec2 vPixelPosition ){\
        vec2 vPosition = vPixelPosition;\
        \
        float fValue = 0.0;\
        \
        fValue += GetBlobValue( vPosition, vec2( 0.0, 0.0 ) );\
        fValue += GetBlobValue( vPosition, vec2( -1.0, 0.0 ) );\
        fValue += GetBlobValue( vPosition, vec2( 0.0, -1.0 ) );\
        fValue += GetBlobValue( vPosition, vec2( -1.0, -1.0 ) );\
        \
        fValue = max(fValue, 0.0);\
        fValue = 1.0 - abs(0.7 - fValue);\
        fValue = max(fValue, 0.0);\
        \
        fValue = pow(fValue, 1.5);\
        \
        vec3 cColA = vec3(1.0, 0., 0.4);\
        vec3 cColB = vec3(0., 0.0, 0.0);\
        \
        return cColA * fValue + cColB * (1.0 - fValue);\
    }\
    \
    void main( void ) {\
        vec3 vOutputColour = GetColour(gl_FragCoord.xy * ( mouse.x / 8.0 ));\
        \
        gl_FragColor = vec4( vOutputColour, 1.0 );\
    }";
    
    // By @paulofalcao
    // Blobs with nice gradient
    frags[7] = "#version 120\n\
    #extension GL_ARB_texture_rectangle : enable\n \
    \
    uniform float time;\
    uniform vec2 mouse;\
    uniform vec2 resolution;\
    \
    float makePoint(float x,float y,float fx,float fy,float sx,float sy,float t){\
        float xx=x+sin(t*fx)*sx;\
        float yy=y+cos(t*fy)*sy;\
        return 1.0/sqrt(xx*xx+yy*yy);\
    }\
    \
    vec3 gu(vec4 a,vec4 b,float f){\
        return mix(a.xyz,b.xyz,(f-a.w)*(1.0/(b.w-a.w)));\
    }\
    \
    vec3 grad(float f){\
        vec4 c01=vec4(0.0,0.0,0.0,0.00);\
        vec4 c02=vec4(0.5,0.0,0.0,0.50);\
        vec4 c03=vec4(1.0,0.0,0.0,0.55);\
        vec4 c04=vec4(1.0,1.0,0.0,0.80);\
        vec4 c05=vec4(1.0,1.0,1.0,1.00);\
        return (f<c02.w)?gu(c01,c02,f):\
        (f<c03.w)?gu(c02,c03,f):\
        (f<c04.w)?gu(c03,c04,f):\
        gu(c04,c05,f);\
    }\
    \
    void main( void ) {\
        vec2 p=(gl_FragCoord.xy/resolution.x)*2.0-vec2(1.0,resolution.y/resolution.x);\
        \
        p=p*2.0;\
        \
        float x=p.x;\
        float y=p.y;\
        \
        float a=\
        makePoint(x,y,3.3,2.9,0.3,0.3,time);\
        a=a+makePoint(x,y,1.9,2.0,0.4,0.4,time);\
        a=a+makePoint(x,y,0.8,0.7,0.4,0.5,time);\
        a=a+makePoint(x,y,2.3,0.1,0.6,0.3,time);\
        a=a+makePoint(x,y,0.8,1.7,0.5,0.4,time);\
        a=a+makePoint(x,y,0.3,1.0,0.4,0.4,time);\
        a=a+makePoint(x,y,1.4,1.7,0.4,0.5,time);\
        a=a+makePoint(x,y,1.3,2.1,0.6,0.3,time);\
        a=a+makePoint(x,y,1.8,1.7,0.5,0.4,time);\
        a=a+makePoint(x,y,1.2,1.9,0.3,0.3,time);\
        a=a+makePoint(x,y,0.7,2.7,0.4,0.4,time);\
        a=a+makePoint(x,y,1.4,0.6,0.4,0.5,time);\
        a=a+makePoint(x,y,2.6,0.4,0.6,0.3,time);\
        a=a+makePoint(x,y,0.7,1.4,0.5,0.4,time);\
        a=a+makePoint(x,y,0.7,1.7,0.4,0.4,time);\
        a=a+makePoint(x,y,0.8,0.5,0.4,0.5,time);\
        a=a+makePoint(x,y,1.4,0.9,0.6,0.3,time);\
        a=a+makePoint(x,y,0.7,1.3,0.5,0.4,time);\
        a=a+makePoint(x,y,3.7,0.3,0.3,0.3,time);\
        a=a+makePoint(x,y,1.9,1.3,0.4,0.4,time);\
        a=a+makePoint(x,y,0.8,0.9,0.4,0.5,time);\
        a=a+makePoint(x,y,1.2,1.7,0.6,0.3,time);\
        a=a+makePoint(x,y,0.3,0.6,0.5,0.4,time);\
        a=a+makePoint(x,y,0.3,0.3,0.4,0.4,time);\
        a=a+makePoint(x,y,1.4,0.8,0.4,0.5,time);\
        a=a+makePoint(x,y,0.2,0.6,0.6,0.3,time);\
        a=a+makePoint(x,y,1.3,0.5,0.5,0.4,time);\
        \
        vec3 a1=grad(a/128.0);\
        \
        gl_FragColor = vec4(a1.x,a1.y,a1.z,1.0);\
    }";
    
    // Sea Noise
    // By @paulofalcao
    // Simple raymarching sandbox with camera
    
    // Raymarching Distance Fields
    // About http://www.iquilezles.org/www/articles/raymarchingdf/raymarchingdf.htm
    // Also known as Sphere Tracing
    // ******** Ashima Noise3D Start https://github.com/ashima/webgl-noise/blob/master/src/noise3D.glsl
    
    // Util Start
    frags[8] = "uniform vec2 resolution;\
    uniform float time;\
    uniform vec2 mouse;\
    \
    float PI=3.14159265;\
    \
    vec3 mod289(vec3 x) {\
        return x - floor(x * (1.0 / 289.0)) * 289.0;\
    }\
    \
    vec4 mod289(vec4 x) {\
        return x - floor(x * (1.0 / 289.0)) * 289.0;\
    }\
    \
    vec4 permute(vec4 x) {\
        return mod289(((x*34.0)+1.0)*x);\
    }\
    \
    vec4 taylorInvSqrt(vec4 r){\
        return 1.79284291400159 - 0.85373472095314 * r;\
    }\
    \
    float snoise(vec3 v){\
        const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;\
        const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);\
        \
        vec3 i  = floor(v + dot(v, C.yyy) );\
        vec3 x0 =   v - i + dot(i, C.xxx) ;\
        \
        vec3 g = step(x0.yzx, x0.xyz);\
        vec3 l = 1.0 - g;\
        vec3 i1 = min( g.xyz, l.zxy );\
        vec3 i2 = max( g.xyz, l.zxy );\
        \
        vec3 x1 = x0 - i1 + C.xxx;\
        vec3 x2 = x0 - i2 + C.yyy;\
        vec3 x3 = x0 - D.yyy;\
        \
        i = mod289(i);\
        vec4 p = permute( permute( permute( \
                                           i.z + vec4(0.0, i1.z, i2.z, 1.0 ))\
                                  + i.y + vec4(0.0, i1.y, i2.y, 1.0 )) \
                         + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));\
        \
        float n_ = 0.142857142857;\
        vec3  ns = n_ * D.wyz - D.xzx;\
        \
        vec4 j = p - 49.0 * floor(p * ns.z * ns.z);\
        \
        vec4 x_ = floor(j * ns.z);\
        vec4 y_ = floor(j - 7.0 * x_ );\
        \
        vec4 x = x_ *ns.x + ns.yyyy;\
        vec4 y = y_ *ns.x + ns.yyyy;\
        vec4 h = 1.0 - abs(x) - abs(y);\
        \
        vec4 b0 = vec4( x.xy, y.xy );\
        vec4 b1 = vec4( x.zw, y.zw );\
        \
        vec4 s0 = floor(b0)*2.0 + 1.0;\
        vec4 s1 = floor(b1)*2.0 + 1.0;\
        vec4 sh = -step(h, vec4(0.0));\
        \
        vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;\
        vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;\
        \
        vec3 p0 = vec3(a0.xy,h.x);\
        vec3 p1 = vec3(a0.zw,h.y);\
        vec3 p2 = vec3(a1.xy,h.z);\
        vec3 p3 = vec3(a1.zw,h.w);\
        \
        vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));\
        p0 *= norm.x;\
        p1 *= norm.y;\
        p2 *= norm.z;\
        p3 *= norm.w;\
        \
        vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);\
        m = m * m;\
        return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1), \
                                     dot(p2,x2), dot(p3,x3) ) );\
    }\
    \
    float obj(in vec3 p){\
        float d=p.y;\
        p.y=p.y+time;\
        if (d<0.2)\
            d=d-snoise(vec3(p*2.0))*0.15+0.05;\
        return d;\
    }\
    \
    vec3 obj_c(in vec3 p){\
        p.y=p.y+time;\
        float c=snoise(vec3(p*2.0))*0.5+0.5;\
        c=c*c*(3.0-2.0*c);\
        vec3 c3=mix(vec3(0,0.5,1),vec3(1,0,0),c);\
        return c3;\
    }\
    \
    void main(void){\
        vec2 vPos=gl_FragCoord.xy/resolution.xy;\
        vPos.y = 1.0 - vPos.y;\
        vPos = -1.0+2.0*vPos;\
        \
        vec3 vuv=vec3(0,1,sin(time*.23)*0.2);\
        vec3 prp=vec3(sin(time*.53)*4.0,2,cos(time*.39)*4.0);\
        vec3 vrp=prp-vec3(cos(time*.42)*0.5,1.0,sin(time*.34)*0.5);\
        \
        vec3 vpn=normalize(vrp-prp);\
        vec3 u=normalize(cross(vuv,vpn));\
        vec3 v=cross(vpn,u);\
        vec3 vcv=(prp+vpn);\
        vec3 scrCoord=vcv+vPos.x*u*resolution.x/resolution.y+vPos.y*v;\
        vec3 scp=normalize(scrCoord-prp);\
        \
        const vec3 e=vec3(0.1,0,0);\
        const float maxd=15.0;\
        float s=0.1;\
        vec3 c,p,n;\
        \
        float f=-prp.y/scp.y;\
        if (f>0.0) p=prp+scp*f;\
        else f=maxd;\
        \
        for(int i=0;i<4;i++){\
            f+=s;\
            p=prp+scp*f;\
            s=obj(p);\
        }\
        \
        if (f<maxd){\
            c=obj_c(p);\
            n=normalize(\
                        vec3(s-obj(p-e.xyy),\
                             s-obj(p-e.yxy),\
                             s-obj(p-e.yyx)));\
            float b=dot(n,normalize(prp-p));\
            gl_FragColor=vec4((b*c+pow(b,16.0))*(1.0-f*.06),1.0);\
        }\
        else gl_FragColor=vec4(0,0,0,1);\
    }";
    
    /* lame-ass tunnel by kusma */
    frags[9] = "uniform float time;\
    uniform vec2 mouse;\
    uniform vec2 resolution;\
    \
    void main( void ) {\
        vec2 position = (gl_FragCoord.xy - resolution * 0.5) / resolution.yy;\
        float th = atan(position.y, position.x) / (2.0 * 3.1415926) + 0.5 + mouse.x;\
        float dd = length(position);\
        float d = 0.25 / dd + time + mouse.y;\
        \
        vec3 uv = vec3(th + d, th - d, th + sin(d) * 0.1);\
        float a = 0.5 + cos(uv.x * 3.1415926 * 2.0) * 0.5;\
        float b = 0.5 + cos(uv.y * 3.1415926 * 2.0) * 0.5;\
        float c = 0.5 + cos(uv.z * 3.1415926 * 6.0) * 0.5;\
        vec3 color = mix(vec3(1.0, 0.8, 0.9), vec3(0.1, 0.1, 0.2), pow(a, 0.2)) * 3.;\
        color += mix(vec3(0.8, 0.9, 1.0), vec3(0.1, 0.1, 0.2),  pow(b, 0.1)) * 0.75;\
        color += mix(vec3(0.9, 0.8, 1.0), vec3(0.1, 0.2, 0.2),  pow(c, 0.1)) * 0.75;\
        gl_FragColor = vec4(color * clamp(dd, 0.0, 1.0), 1.0);\
    }";
    
    // "bumpmapping" @Flexi23
    // tweaks, anony_gt
    frags[10] = "uniform float time;\
    uniform vec2 mouse;\
    uniform vec2 resolution;\
    \
    uniform sampler2D backbuffer;\
    \
    float orbitDistance = 0.000025;\
    float waveLength = 300.00;\
    \
    vec3 sim(vec3 p,float s);\
    vec2 rot(vec2 p,float r);\
    vec2 rotsim(vec2 p,float s);\
    \
    vec2 makeSymmetry(vec2 p){\
        vec2 ret=p;\
        ret.y=abs(ret.y);\
        ret=rotsim(ret,sin(time*0.1)*2.0+9.0); \
        ret.x=abs(ret.x);\
        return ret;\
    }\
    const float PI=3.14159265;\
    \
    vec3 sim(vec3 p,float s){\
        vec3 ret=p;\
        ret=p+s/2.0;\
        ret=fract(ret/s)*s-s/2.0;\
        return ret;\
    }\
    \
    vec2 rot(vec2 p,float r){\
        vec2 ret;\
        ret.x=p.x*cos(r)-p.y*sin(r);\
        ret.y=p.x*sin(r)+p.y*cos(r);\
        return ret;\
    }\
    \
    vec2 rotsim(vec2 p,float s){\
        vec2 ret=p;\
        ret=rot(p,-PI/(s*2.0));\
        ret=rot(p,floor(atan(ret.x,ret.y)/PI*s)*(PI/s));\
        return ret;\
    }\
    \
    void main( void ) {\
        vec2 uv = (gl_FragCoord.xy/resolution.x)*2.0-vec2(1.0,resolution.y/resolution.x);\
        uv=uv*.2;\
        \
        uv=makeSymmetry(uv);\
        \
        vec2 p1 = (vec2(sin(time), cos(time))*orbitDistance)+.1;\
        vec2 p2 = (vec2(sin(time+3.142), cos(time+3.142))*orbitDistance)+.125;\
        \
        float d1 = .1-length(uv +p1);\
        float d2 = .1-length(uv -p2);\
        \
        float wave1 = sin(d1*waveLength+(time*5.))*0.5 + 0.5 * (((d1 - 0.5) * .5) - .5);\
        float wave2 = sin(d2*waveLength+(time*5.))*0.5 + 0.5 * (((d1 - 0.5) * 1.5) + 1.5);\
        float c = d1 > 0.99 || d2 > 0.995 ? 1. : 0.;\
        c + wave1*wave2;\
        gl_FragColor = vec4(c + wave1*wave2,c,c,1.);\
        \
        vec2 d = 1./resolution;\
        float dx = texture2D(backbuffer, uv + vec2(-1.,0.)*d).x - texture2D(backbuffer, uv + vec2(1.,0.)*d).x ;\
        float dy = texture2D(backbuffer, uv + vec2(0.,-1.)*d).x - texture2D(backbuffer, uv + vec2(0.,1.)*d).x ;\
        d = vec2(dx,dy)*resolution/resolution.x*2.;\
        gl_FragColor.z = pow(clamp(1.-1.5*length(uv  - vec2(1.15*cos(time*.2)+.5,1.*sin(time*.1)+.5) + d),0.,1.),4.0);\
        gl_FragColor.y = gl_FragColor.z*1.5 + gl_FragColor.x*0.3;\
        \
        gl_FragColor *=10.25;\
    }";
    
    frags[11] = "uniform float time;\
    uniform vec2 mouse;\
    uniform vec2 resolution;\
    \
    void main( void ) {\
        vec2 position = ( gl_FragCoord.xy / resolution.xy ) + mouse / 4.0;\
        \
        float color = 0.0;\
        color += sin( position.x * cos( time / 197.0 ) * 80.0 ) + cos( position.y * cos( time / 150.0 ) * 10.0 );\
        color += sin( position.y * sin( time / 10.0 ) * 40.0 ) + cos( position.x * sin( time / 25.0 ) * 40.0 );\
        color += sin( position.x * sin( time / 5.0 ) * 10.0 ) + sin( position.y * sin( time / 35.0 ) * 80.0 );\
        color *= sin( time / 10.0 ) * 0.5;\
        \
        gl_FragColor = vec4( vec3( color, color * 0.5, sin( color + time / 3.0 ) * 0.75 ), 1.0 );\
        \
    }";
    
    // By @paulofalcao
    //
    // Some blobs modifications with symmetries
    frags[12] = "uniform float time;\
    uniform vec2 mouse;\
    uniform vec2 resolution;\
    \
    vec3 sim(vec3 p,float s);\
    vec2 rot(vec2 p,float r);\
    vec2 rotsim(vec2 p,float s);\
    \
    vec2 makeSymmetry(vec2 p){\
        vec2 ret=p;\
        ret.y=abs(ret.y);\
        ret=rotsim(ret,sin(time*0.3)*2.0+3.0);\
        ret.x=abs(ret.x);\
        return ret;\
    }\
    \
    float makePoint(float x,float y,float fx,float fy,float sx,float sy,float t){\
        float xx=x+tan(t*fx)*sx;\
        float yy=y-tan(t*fy)*sy;\
        return 0.5/sqrt(abs(x*xx+yy*yy));\
    }\
    \
    const float PI=3.14159265;\
    \
    vec3 sim(vec3 p,float s){\
        vec3 ret=p;\
        ret=p+s/2.0;\
        ret=fract(ret/s)*s-s/2.0;\
        return ret;\
    }\
    \
    vec2 rot(vec2 p,float r){\
        vec2 ret;\
        ret.x=p.x*cos(r)-p.y*sin(r);\
        ret.y=p.x*sin(r)+p.y*cos(r);\
        return ret;\
    }\
    \
    vec2 rotsim(vec2 p,float s){\
        vec2 ret=p;\
        ret=rot(p,-PI/(s*2.0));\
        ret=rot(p,floor(atan(ret.x,ret.y)/PI*s)*(PI/s));\
        return ret;\
    }\
    \
    void main( void ) {\
        vec2 p=(gl_FragCoord.xy/resolution.x)*2.0-vec2(1.0,resolution.y/resolution.x);\
        p=p*2.0;\
        p=makeSymmetry(p);\
        \
        float x=p.x;\
        float y=p.y;\
        \
        float t=time*0.5;\
        \
        float a=\
        makePoint(x,y,3.3,2.9,0.3,0.3,t);\
        a=a+makePoint(x,y,1.9,2.0,0.4,0.4,t);\
        a=a+makePoint(x,y,0.8,0.7,0.4,0.5,t);\
        a=a+makePoint(x,y,2.3,0.1,0.6,0.3,t);\
        a=a+makePoint(x,y,0.8,1.7,0.5,0.4,t);\
        a=a+makePoint(x,y,0.3,1.0,0.4,0.4,t);\
        a=a+makePoint(x,y,1.4,1.7,0.4,0.5,t);\
        a=a+makePoint(x,y,1.3,2.1,0.6,0.3,t);\
        a=a+makePoint(x,y,1.8,1.7,0.5,0.4,t);\
        \
        float b=\
        makePoint(x,y,1.2,1.9,0.3,0.3,t);\
        b=b+makePoint(x,y,0.7,2.7,0.4,0.4,t);\
        b=b+makePoint(x,y,1.4,0.6,0.4,0.5,t);\
        b=b+makePoint(x,y,2.6,0.4,0.6,0.3,t);\
        b=b+makePoint(x,y,0.7,1.4,0.5,0.4,t);\
        b=b+makePoint(x,y,0.7,1.7,0.4,0.4,t);\
        b=b+makePoint(x,y,0.8,0.5,0.4,0.5,t);\
        b=b+makePoint(x,y,1.4,0.9,0.6,0.3,t);\
        b=b+makePoint(x,y,0.7,1.3,0.5,0.4,t);\
        \
        float c=\
        makePoint(x,y,3.7,0.3,0.3,0.3,t);\
        c=c+makePoint(x,y,1.9,1.3,0.4,0.4,t);\
        c=c+makePoint(x,y,0.8,0.9,0.4,0.5,t);\
        c=c+makePoint(x,y,1.2,1.7,0.6,0.3,t);\
        c=c+makePoint(x,y,0.3,0.6,0.5,0.4,t);\
        c=c+makePoint(x,y,0.3,0.3,0.4,0.4,t);\
        c=c+makePoint(x,y,1.4,0.8,0.4,0.5,t);\
        c=c+makePoint(x,y,0.2,0.6,0.6,0.3,t);\
        c=c+makePoint(x,y,1.3,0.5,0.5,0.4,t);\
        \
        vec3 d=vec3(a,b,c)/32.0;\
        \
        gl_FragColor = vec4(d.x,d.y,d.z,1.0);\
    }";
    
    frags[13] = "uniform vec2 resolution;\
    uniform float time;\
    uniform sampler2D tex0;\
    \
    vec3 deform( in vec2 p ){\
        vec2 uv;\
        \
        vec2 q = vec2( sin(1.1*time+p.x),sin(1.2*time+p.y) );\
        \
        float a = atan(q.y,q.x);\
        float r = sqrt(dot(q,q));\
        \
        uv.x = sin(0.0+1.0*time)+p.x*sqrt(r*r+1.0);\
        uv.y = sin(0.6+1.1*time)+p.y*sqrt(r*r+1.0);\
        \
        return texture2D(tex0,uv*.5).xyz;\
    }\
    \
    void main(void){\
        vec2 p = -1.0 + 2.0 * gl_FragCoord.xy / resolution.xy;\
        vec2 s = p;\
        \
        vec3 total = vec3(0.0);\
        vec2 d = (vec2(0.0,0.0)-p)/40.0;\
        float w = 1.0;\
        for( int i=0; i<40; i++ ){\
            vec3 res = deform(s);\
            res = smoothstep(0.1,1.0,res*res);\
            total += w*res;\
            w *= .99;\
            s += d;\
        }\
        total /= 40.0;\
        float r = 1.5/(1.0+dot(p,p));\
        gl_FragColor = vec4( total*r,1.0);\
    }";
    
    fxObject.allocate(width,height);
}

//--------------------------------------------------------------
void testApp::update(){
#ifdef THERE_IS_CAM
    
    video.update(); // Get Video input image
    if (video.isFrameNew() ){
        ofSetColor(255);
        
        fxObject.begin();
        video.draw(0,0); 
        fxObject.end();
    }
    
#else
    
    fxObject.begin();
    ofClear(0);
    image.draw(640*0.5 - image.getWidth()*0.5, 480*0.5 - image.getHeight()*0.5);
    fxObject.end();
    
#endif
    
    fxObject.update();
        
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    fxObject.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch(key){
        case '1':
            fxObject.injectShader(frags[0]);
            break;
        case '2':
            fxObject.injectShader(frags[1]);
            break;
        case '3':
            fxObject.injectShader(frags[2]);
            break;
        case '4':
            fxObject.injectShader(frags[3]);
            break;
        case '5':
            fxObject.injectShader(frags[4]);
            break;
        case '6':
            fxObject.injectShader(frags[5]);
            break;
        case '7':
            fxObject.injectShader(frags[6]);
            break;
        case '8':
            fxObject.injectShader(frags[7]);
            break;
        case '9':
            fxObject.injectShader(frags[8]);
            break;
        case '0':
            fxObject.injectShader(frags[9]);
            break;
        case 'q':
            fxObject.injectShader(frags[10]);
            break;
        case 'w':
            fxObject.injectShader(frags[11]);
            break;
        case 'e':
            fxObject.injectShader(frags[12]);
            break;
        case 'r':
            fxObject.injectShader(frags[13]);
            break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}