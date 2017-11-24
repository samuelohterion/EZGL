# ShaderFun
A ShaderToy like standalone Fun

You can test your fragment shaders by editing the file "frag.glsl".

# INPUTS
## UNIFORMS
### TIME
#### time
`uniform in float time;`
time stores the running time in seconds.
its precision is microseconds.
### SCREEN SIZE
#### dims
`unfiorm vec2 dims;`
dims.x is the screen width.
dims.y is the screen height.
`unfiorm vec2 rdims;`
dims.x is the reciproke screen width 1. / dims.x.
dims.y is the reciproke screen height 1. / dims.y.
### MOUSE
#### mouse
`uniform vec2 mouse;`
mouse is the current position of the mouse cursor.
## VERTEX ATTRIBUTES
### POSITION
#### pos
`in vec2 pos;`
