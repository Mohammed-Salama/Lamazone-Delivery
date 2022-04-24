#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 tex_coord;
layout(location = 3) in vec3 normal;

out Varyings {
    vec3 position;
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
} vs_out;

void main(){
    // const vec3 positions[3] =vec3[3] (
    //     vec3(0 , 0 ,0),
    //     vec3(0.5 , 0 , 0),
    //     vec3(0 , 0.5 , 0)
    // );
    // gl_Position =  vec4(positions[gl_VertexID], 1.0);
    gl_Position =  vec4(position, 1.0);
    vs_out.position = position;
    vs_out.color = color;
    vs_out.tex_coord = tex_coord;
    vs_out.normal = normal;
}