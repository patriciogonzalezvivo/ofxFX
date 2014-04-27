![logos](http://patriciogonzalezvivo.com/2011/ofxfx/trees.png)

I [start this addon](https://github.com/patriciogonzalezvivo/ofxFX) while I was working on [Efecto Mariposa](http://patriciogonzalezvivo.com/2012/eden/). Is the result of adapting different well know algorithms to GLSL Shaders for openFrameworks.

An important feature of [**ofxFX**](https://github.com/patriciogonzalezvivo/ofxFX) is it ability for fast-prototyping mixing different classes using the `<<` operator.

[**ofxFX**](https://github.com/patriciogonzalezvivo/ofxFX) was originally develop for GLSL 120 but now is been edited to support openGL ES and openGL 2.0 thanks to the new programable render of openFrameworks. [Oliver](https://github.com/OliverUv) is giving me a hand with that. Anybody else that want to collaborate is wellcome.

## ofxFXObject ##
It´s the parent class of all the other effect. If you want to make a new filter you may want to start looking at the ofxFXObject.h .

The structure it´s easy.

1. Constructor: here it´s necessary to set three vital variables: 
    - ```int passes```: the number of passes or itineration of the main ping pong betweens FBO´s  
    - ```int internalFormat```: if it use GL_RGB, GL_RGBA, GL_RGB16f, GL_RGBA16f, GL_RGB32f, GL_RGBA32f, etc...
    - ```string fragShader```: it´s the code of the shader. Note that some changes have to be made in order to fill everything on a string
    
2. ```allocate(width,height,GL_RGBA)```: This usually it´s no need to be re-define. It´s basically allocate the FBO´s and loads the shader by using injectShader();

3. ```setCode(string fragContent)```: here is where the shaders are loaded. See the example below.

4. ```begin(int texN)``` and ```end(int texN)```: remember nTextures variable? you can passthrough information to it by using this end() and begin() and giving the argument the number of texture you want to open. This texture can be access from the shader by the ```uniform sample2DRect tex0``` or ```tex1``` or ```tex2``` and so on. 

    4.1. ```<<``` and ```setTexture(texture-like, num=0)``` if you have an FBO, a texture, an ofxFXObject, or something similar, you can use this function to write it to ```tex0```, ```tex1```, etc (as indicated by the num argument). ```<<``` always writes to ```tex0```.

    4.2. If you're making a multi-pass shader, you can access to the previously rendered frame with ```uniform sample2DRect backbuffer``` in your shader. This will also contain the information from ```tex0``` on the first pass.

5. ```update()```: This is the core of the class, where the magic happens. If you check  for the ```ofxFXObject::update()``` you will see how the tex's, the backbuffer and other default uniforms variables (time, mouse, resolution) are loaded. **NOTE:** If you are using the ```<<``` operator you **don't** need to update. It's done automatically for you

6. ```draw(x,y,w,h)```: After all you definitely want to look at it.

An example of an implementation of an ofxFXObject could be:

On setup:

```c++
ofxFXObject fxObject = ofxFXObject();

fxObject.allocate(800,600); // At this point it will load a default timer shader

//Let´s play a little injecting a new one
fxObject.setCode("#version 120\n\
                    \
                    uniform sampler2DRect backbuffer;\
                    uniform sampler2DRect tex0;\
                    uniform sampler2DRect tex1;\
                    uniform sampler2DRect tex2;\
                    \
                    uniform float time; \
                    uniform vec2 mouse;\
                    uniform vec2 resolution;\
                    \
                    void main( void ) {\
                        \
                        ... \
                        \
                        gl_FragColor = ... ;\
                    }"); 
```

Also look at the functions load, loadVersioned, and the different shader strings
to see how you can specify your shader code. You can also see how this is used
to easily load OpenGL2 and OpenGL3 shaders correctly if you look inside the
test-shader-load example. Note that if you don't specify any Vertex Shader for
OpenGL3, a simple pass-through shader will be added for you. :)

On update:

```c++
fxObject.begin();   // for tex0 you don´t need to pass the 0 as an argument 
    //What ever you want to render to tex0
fxObject.end();

fxObject.begin(1);
    //What ever you want to render to tex1
fxObject.end(1);

fxObject.setTexture(image.getTextureReference(),2); // For object that have ofTexture you could just pass the ofTexture reference 


fxObject.update();
```

On draw:

```c++
fxObject.draw();
``` 

### Copy, Inject & Share ###
If you are curious and want to learn the best way it´s to see and edit code. You could go to [Ricardo Caballero´s webGL Sandbox](http://mrdoob.com/projects/glsl_sandbox/) or [Inigo Quilez´s ShaderToy](http://www.iquilezles.org/apps/shadertoy/) to found some inspiration. Explore. Make changes. Inject. Compile.
May be it´s the case things not compile as you spect. Some times (most of them when you are dealing with other textures) you will need to make some changes. As far as I know openGL and GPU hardware it´s makes lot´s of changes and improves year after year. So lot´s of problems related to compatibility will happened. Also openFrameworks works fine with openGL 1.2 and it use by default the ARB Rect Textures. 

Having that in mind the process of adapting code it´s a matter of searching on google. Fortunately you can see the ones I already implement and make them work.

General tips:

* Casting: openGL it will not cast for you. So if you assign an int to a float it will not compile properly. 

```c++

float f = 1;    // FAIL 
float f = 1.0;  // GOOD

``` 

* Norm coordinates: as far as I know there are two types of textures sample2D and sample2DRect. The first ones have the same length in both sides while the second one not. When you are using texture2D( tex, pos) the position have to be normalized while on texture2DRect(tex, pos) it´s not normalized. 

![Filters](http://patriciogonzalezvivo.com/2011/ofxfx/filters.png)

### Mix them all ###

ofxFXObject have some handy operators that let you combine them really easily. 
At the Mix´s Example you will find how to do it. Basicaly it´s somethin like this

```c++

ofxFXObject objA;
ofxGrayScott objB;
ofxBlur     blur;

objA.allocate(width, height);
objB.allocate(width, height);
blur.allocate(width, height);

...

blur << objB << objA;

...

blur.draw();

``` 

<iframe src="http://player.vimeo.com/video/35230297" width="575" height="453" frameborder="0" webkitAllowFullScreen mozallowfullscreen allowFullScreen></iframe>
<iframe src="http://player.vimeo.com/video/34304485" width="575" height="431" frameborder="0" webkitAllowFullScreen mozallowfullscreen allowFullScreen></iframe>
<iframe src="http://player.vimeo.com/video/30684308" width="575" height="429" frameborder="0" webkitAllowFullScreen mozallowfullscreen allowFullScreen></iframe>
<iframe src="http://player.vimeo.com/video/29887776" width="575" height="431" frameborder="0" webkitAllowFullScreen mozallowfullscreen allowFullScreen></iframe>
<iframe src="http://player.vimeo.com/video/29896245" width="575" height="431" frameborder="0" webkitAllowFullScreen mozallowfullscreen allowFullScreen></iframe>
<iframe src="http://player.vimeo.com/video/29894099" width="575" height="431" frameborder="0" webkitAllowFullScreen mozallowfullscreen allowFullScreen></iframe>
