precision lowp float;
varying vec2 coordVarying;
attribute vec4 position;
attribute vec2 coord;

void main() {
    
    coordVarying = coord;
    gl_Position = position;
}
