precision lowp float;
varying vec2 coordVarying;
uniform sampler2D sampleY;
uniform sampler2D sampleUV;
uniform mat3 colorMtrix;

void main() {
    lowp vec3 rgb;
    lowp vec3 YUV;
    YUV.x = texture2D(sampleY, coordVarying).r;
    YUV.yz = texture2D(sampleUV, coordVarying).ra - vec2(0.5, 0.5);
    rgb = colorMtrix * YUV;
    gl_FragColor = vec4(rgb, 1);
}
