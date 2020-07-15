#version 330 core

uniform sampler2D uTexture;
uniform vec2 uTextureScalingFactor;
uniform vec4 uTintingColor;

out vec4 color;
in vec2 vTextureCoordinates;

void main()
{
	color = texture(uTexture, vTextureCoordinates * uTextureScalingFactor) * uTintingColor;
}
