attribute highp vec4 a_position;
attribute highp vec2 a_texCoord0;
attribute highp vec3 a_normal;
uniform highp mat4 u_ModelMatrix;
uniform highp mat4 u_ProjectionMatrix;
uniform highp mat4 u_ViewMatrix;
varying highp vec2 v_texCoord0;
varying highp vec3 v_normal;
varying highp vec4 v_position;
uniform highp vec4 u_lightPower;

uniform highp vec4 u_addColor;
varying highp vec4 v_addColor;

void main(void)
{
    v_addColor = u_addColor.rgba;
    mat4 mv_matrix = u_ViewMatrix * u_ModelMatrix;
    gl_Position = u_ProjectionMatrix * mv_matrix * a_position;
    v_texCoord0 = a_texCoord0;
    v_normal = normalize(vec3(mv_matrix * vec4(a_normal, 0.0)));
    v_position = mv_matrix * v_position;
    u_lightPower = u_lightPower;
}
