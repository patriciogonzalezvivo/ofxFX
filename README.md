# ofxFX #

This addons originaly born while I was working on [Efecto Mariposa](http://patriciogonzalezvivo.com/butterfly.html "Efecto Mariposa") and I need to do fast GPU image processing throught GLSL Shaders.

The main goal of this addon it´s to make super easy to use and edit shaders on-the-fly for adding them on your proyects.

## ofxFXObject ##
It´s the parent class of all the other effect. If you want to make a new filter it´s better if you start looking at the ofxFXObject.h in order to see how it works. 

It have a simple structre:

1. Constructor: here it´s necesary to set three vital variables: 
    - passes: the number of passes or itinerations of the main ping pong betweens FBO´s  
    - nTextures: number of textures used;
    - internalFormat: if it use GL_RGB, GL_RGBA, GL_RGB16f, GL_RGBA16f, GL_RGB32f, GL_RGBA32f, etc...
    
2. allocate(width,height): This usualy it´s no need to bee re-define. It´s basically allocate the FBO´s and loads the shader by using injectShader();

3. injectShader(): here is where the shaders are loaded. The good stuff cames when you load them on the fly through a string using something like this:

```
ofxFXObject fxObject = ofxFXObject();

fxObject.allocate(800,600); // At this point it will load a default timer shader

//Let´s play a little injecting a new one
fxObject.injectShader("#version 120\n\
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
                    }"); 
```

### Copy, Pase & Inject ###
http://www.iquilezles.org/apps/shadertoy/
http://mrdoob.com/projects/glsl_sandbox/

### Portability ###
Each ofxFX class have a fragment, vertex o geometry Shader embebed on it´s code. So you don´t need to add any kind of .frag , .vert or .geom on your /data forlder

### Coherence ###
The implementation of each ofxFX class it´s consistent with each other. All of them are extensions of ofxFXObject

* All of them need to be allocated first in order to 
```fx.allocate(width,height);```

* All of them have a ofxFbo style. So you can draw or render directly inside of them. Allowing you to easyly interact with them
```
fx.begin();
ofEllipse(50,50,10,10);
fx.end();
```

Watch some of this classes in action [HERE](http://www.patriciogonzalezvivo.com/blog/?p=488)

![ofxFlocking](http://patriciogonzalezvivo.com/images/flock.png) ![ofxFluid](http://patriciogonzalezvivo.com/images/fluid.png) ![ofxGlow](http://patriciogonzalezvivo.com/images/oldTv.png) ![ofxGrayScott](http://patriciogonzalezvivo.com/images/grayScott.png) ![ofxWater](http://patriciogonzalezvivo.com/images/water.png)
