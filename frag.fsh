uniform sampler2D u_texture0;
varying highp vec2 v_texCoord0;
varying highp vec3 v_normal;
varying highp vec4 v_position;
uniform highp vec4 u_lightSourcePos;
uniform highp float u_lightPower;

varying highp vec4 v_addColor;

void main(void)
{
    vec4 observerPos = vec4(0.0,0.0,0.0,1.0);

    vec4 resultColor = vec4(0.0,0.0,0.0,0.1);

    vec4 diffMatColor = texture2D(u_texture0, v_texCoord0) * v_addColor;
    vec3 lightVec = normalize(v_position.xyz - u_lightSourcePos.xyz);

    float len = length(v_position.xyz - observerPos.xyz);

    resultColor = diffMatColor;
    vec4 diffColor = dot(v_normal, -lightVec) / (1.0 + 0.25 * pow(len,2.0))*0.25;
    resultColor += vec4(diffColor.r,diffColor.g,diffColor.b,0);

    gl_FragColor = resultColor;
}
