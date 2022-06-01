#version 330

// The texture holding the scene pixels
uniform sampler2D tex;

// Read "assets/shaders/fullscreen.vert" to know what "tex_coord" holds;
in vec2 tex_coord;
out vec4 frag_color;

void main(){
    // To apply the invertscale effect, we invert values of the red/blue/green channels
    frag_color = texture(tex, tex_coord);
    frag_color.r = 1 - frag_color.r;
    frag_color.g = 1 - frag_color.g;
    frag_color.b = 1 - frag_color.b;
}