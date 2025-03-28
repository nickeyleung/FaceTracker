precision lowp float;
attribute vec4 position;
attribute vec2 coord;
varying vec2 varyingCoord;
uniform mat4 mat;

void main() {
    varyingCoord = coord;
    gl_Position = mat * vec4(position.xyz, 1.0);
}
