#version 330 core

out vec4 frag_color;

//DONE: Define uniforms for the center and the side-length
uniform float side_length = 0.0f;
uniform vec2 center = vec2(0.0, 0.0);
uniform vec4 inside_color = vec4(0.0, 0.0, 0.0, 1.0);
uniform vec4 outside_color = vec4(0.0, 0.0, 0.0, 1.0);

void main(){
    //DONE: Write code that will draw the diamond
    if(abs(gl_FragCoord.x-center[0])+abs(gl_FragCoord.y-center[1]) <= side_length/2){
        frag_color = inside_color;
    } else {
        frag_color = outside_color;
    }
}