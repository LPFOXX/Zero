#type vertex
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTextureCoordinates;

uniform mat4 uViewProjection;

out vec2 vTextureCoordinates;

void main()
{
	vTextureCoordinates = aTextureCoordinates;
	gl_Position = uViewProjection * vec4(aPosition, 1.0);
}

#type fragment
#version 330 core

uniform vec4 uColor;
uniform sampler2D uTexture;
uniform vec2 uTextureScalingFactor;

out vec4 color;
in vec2 vTextureCoordinates;

void main()
{
	color = uColor * texture(uTexture, vTextureCoordinates * uTextureScalingFactor);
}
