uniform sampler2D u_texture0;
varying highp vec2 v_texCoord0;
varying highp vec3 v_normal;
varying highp vec4 v_position;
uniform highp vec4 u_lightSourcePos;
uniform highp float u_lightPower;

void main(void)
{
    vec4 observerPos = vec4(0.0,0.0,0.0,1.0);

    vec4 resultColor = vec4(0.0,0.0,0.0,0.1);
    vec4 diffMatColor = texture2D(u_texture0, v_texCoord0);
    vec3 eye = normalize(v_position.xyz - observerPos.xyz);
    vec3 lightVec = normalize(v_position.xyz - u_lightSourcePos.xyz);
    vec3 reflectLight = normalize(reflect(lightVec,v_normal));
    float specularFactor = 5.0;
    float ambientFactor = 0.75;

    float len = length(v_position.xyz - observerPos.xyz);

    vec4 diffColor = diffMatColor * u_lightPower * dot(v_normal, -lightVec) / (1.0 + 0.25 * pow(len,2.0));

    resultColor += diffColor;

    gl_FragColor = texture2D(u_texture0, v_texCoord0);
}
