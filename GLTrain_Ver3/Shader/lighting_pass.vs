#version 450 core

out vec2 TexCoords;

void main()
{
    vec2 positions[6] = vec2[6](
        vec2(-1.0,  1.0), // Top-left
        vec2(-1.0, -1.0), // Bottom-left
        vec2( 1.0, -1.0), // Bottom-right
        vec2(-1.0,  1.0), // Top-left
        vec2( 1.0, -1.0), // Bottom-right
        vec2( 1.0,  1.0)  // Top-right
    );

    vec2 texCoords[6] = vec2[6](
        vec2(0.0, 1.0), // Top-left
        vec2(0.0, 0.0), // Bottom-left
        vec2(1.0, 0.0), // Bottom-right
        vec2(0.0, 1.0), // Top-left
        vec2(1.0, 0.0), // Bottom-right
        vec2(1.0, 1.0)  // Top-right
    );

    TexCoords = texCoords[gl_VertexID];
    gl_Position = vec4(positions[gl_VertexID], 0.0, 1.0);
}
