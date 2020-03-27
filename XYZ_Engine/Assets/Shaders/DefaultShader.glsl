#type vertex
#version 430


layout(location = 0) in vec4  a_Color;
layout(location = 1) in vec3  a_Position;
layout(location = 2) in vec2  a_TexCoord;
layout(location = 3) in float a_TextureID;


out vec2 v_TexCoord;
out vec4 v_Color;
uniform mat4 u_ViewProjection;

void main()
{
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 430

in vec2 v_TexCoord;
in vec4 v_Color;

uniform sampler2D u_Texture;
out vec4 FragColor;

void main()
{	
	FragColor = texture(u_Texture, v_TexCoord) * v_Color;
}