#version 330 core

out vec4 frag_color;

//DONE: Define uniforms for the slope and the intercept
uniform float slope = 0.0f; // slope: which defines the slope of the separating line.
uniform float intercept = 0.0f; // intercept: which defines the intercept of the separating line in pixels.
uniform vec4 inside_color = vec4(1.0, 0.0, 0.0, 1.0);
uniform vec4 outside_color = vec4(0.0, 0.0, 0.0, 1.0);

void main(){
    //DONE: Write code that will draw the line
    // The line where a point is considered inside the shape it satisfies the condition y <= ax + b
    // where a is the "slope" and b is the "intercept".
    if(gl_FragCoord.y <= (slope*gl_FragCoord.x + intercept)){
        frag_color = inside_color;
    } else {
        frag_color = outside_color;
    }
}