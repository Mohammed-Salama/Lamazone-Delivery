#version 330

// The texture holding the scene pixels
uniform sampler2D tex;

// Read "assets/shaders/fullscreen.vert" to know what "tex_coord" holds;
in vec2 tex_coord;

out vec4 frag_color;

#define offset 0
#define radius 8 // Determines the strength of the wave.

// This brings a small wave effect to the object whether being under water or a little dizzy after being hit.

void main(void) {
  vec2 new_coord = tex_coord;
  new_coord.x += sin(tex_coord.y *radius*2*3.14159 + offset) / 100;
  frag_color = texture(tex, new_coord);
}