
#version 450 core
// Taking in position
layout (location = 0) in vec4 position;

uniform mat4 translation;
uniform mat4 scale;
uniform mat4 ori;
uniform mat4 toSpace;
uniform mat4 camera;
uniform mat4 perspective;

uniform vec3 obj_color;

out vec4 fragColor;

void main() {
   gl_Position = (perspective * camera * toSpace * position) / position.w;
   float v = gl_Position.z/100.0f;
   fragColor = vec4(obj_color.x - v, obj_color.y - v, obj_color.z - v, 1.0f);
}