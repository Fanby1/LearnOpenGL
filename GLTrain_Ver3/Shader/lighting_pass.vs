#version 450 core

out vec2 TexCoords;

const vec2 positions[6] = vec2[6](
        vec2(-1.0,  1.0), // Top-left
        vec2(-1.0, -1.0), // Bottom-left
        vec2( 1.0, -1.0), // Bottom-right
        vec2(-1.0,  1.0), // Top-left
        vec2( 1.0, -1.0), // Bottom-right
        vec2( 1.0,  1.0)  // Top-right
    );

const vec2 texCoords[6] = vec2[6](
        vec2(0.0, 1.0), // Top-left
        vec2(0.0, 0.0), // Bottom-left
        vec2(1.0, 0.0), // Bottom-right
        vec2(0.0, 1.0), // Top-left
        vec2(1.0, 0.0), // Bottom-right
        vec2(1.0, 1.0)  // Top-right
    );

void main()
{
    TexCoords = texCoords[gl_VertexID];
    gl_Position = vec4(positions[gl_VertexID], 0.0, 1.0);
}
