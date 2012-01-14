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
```fx.begin();
ofEllipse(50,50,10,10);
fx.end();```

Watch some of this classes in action [HERE](http://www.patriciogonzalezvivo.com/blog/?p=488)

![ofxFlocking](http://patriciogonzalezvivo.com/images/flock.png) ![ofxFluid](http://patriciogonzalezvivo.com/images/fluid.png) ![ofxGlow](http://patriciogonzalezvivo.com/images/oldTv.png) ![ofxGrayScott](http://patriciogonzalezvivo.com/images/grayScott.png) ![ofxWater](http://patriciogonzalezvivo.com/images/water.png)
