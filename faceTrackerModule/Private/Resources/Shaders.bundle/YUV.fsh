precision lowp float;
varying vec2 coordVarying;
uniform sampler2D sampleY;
uniform sampler2D sampleU;
uniform sampler2D sampleV;
uniform mat3 colorMtrix;

void main() {
    vec3 rgb, yuv;
    yuv.x = texture2D(sampleY, coordVarying).r;
    yuv.y = texture2D(sampleU, coordVarying).r - 0.5;
    yuv.z = texture2D(sampleV, coordVarying).r - 0.5;
    rgb = colorMtrix * yuv;
    gl_FragColor = vec4(rgb, 1);
    //gl_FragColor = vec4(1, 0, 0, 1);

}
