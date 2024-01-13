#version 450 core

struct Rectangle {
    float x, y, width, height;
};

layout(std430, binding = 0) buffer RectangleSSBO {
    Rectangle rectangles[];
};

void main() {
    Rectangle rect = rectangles[gl_InstanceID];
    vec2 p = vec2(rect.x, rect.y);
    float hw = rect.width * 0.5;
    float hh = rect.height * 0.5;
    vec2 vertices[6] = {
        p + vec2(-hw, -hh), p + vec2(-hw, hh), p + vec2(hw, hh),
        p + vec2(hw, hh), p + vec2(hw, -hh), p + vec2(-hw, -hh)
    };
    gl_Position = vec4(vertices[gl_VertexID], 0.0, 1.0);
}
