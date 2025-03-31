precision lowp float;
varying vec2 varyingCoord;
uniform sampler2D texture;

void main() {
    vec4 color = texture2D(texture, varyingCoord);
    if (color.a < 0.01) {
        discard;
    }
    gl_FragColor = color;
}
