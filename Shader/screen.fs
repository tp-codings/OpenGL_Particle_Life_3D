#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

const float offset = 1.0 / 300.0;
float blurRadius = 5.0;

vec3 col = vec3(0.0);
vec3 color = vec3(0.0);

vec3 glowColor = vec3(1.0, 1.0, 0.0);
const float bloomThreshold = 0.7;  // Threshold for bright pixels
const float bloomIntensity = 1.5;  // Intensity of the bloom effect


uniform int choice;

void main()
{
	vec2 offsets[9] = vec2[](
		vec2(-offset, offset),
		vec2(0.0f, offset),
		vec2(offset, offset),
		vec2(-offset, 0.0f),
		vec2(0.0f, 0.0f),
		vec2(offset, 0.0f),
		vec2(-offset, -offset),
		vec2(0.0f, -offset),
		vec2(offset, -offset)
	);

	float sharpnessKernel[9] = float[](
		-1, -1, -1,
		-1,  9, -1, 
		-1, -1, -1
	);

	float blurKernel[9] = float[](
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16
	);

	float edgeDetectionKernel[9] = float[](
		1,  1, 1,
		1, -8, 1, 
		1,  1, 1
	);

	float dafuqKernel[9] = float[](
		-2,  -1, 0,
		-1, 1, 1, 
		1,  1, 2
	);

	vec3 sampleTex[9];

	for(int i= 0; i<9; i++)
	{
		sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
	}


	switch(choice)
	{
		case 0:
			for(int i = 0; i<9; i++)
				col += sampleTex[i] * sharpnessKernel[i];
			break;
		case 1:
			for(int i = 0; i<9; i++)
				col += sampleTex[i] * blurKernel[i];
			break;
		case 2:
			for(int i = 0; i<9; i++)
				col += sampleTex[i] * edgeDetectionKernel[i];
			break;
		case 3:
			//Inversion
			col = vec3(vec3(1.0 - texture(screenTexture, TexCoords)));
			break;
		case 4:
			//Grayscale
			col = vec3(texture(screenTexture, TexCoords));
			FragColor = vec4(col, 1.0);
			float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
			col = vec3(average, average, average);
			break;
		case 5:
			for(int i = 0; i<9; i++)
				col += sampleTex[i] * dafuqKernel[i];
			break;

		case 6:
			// Originalfarbe des Fragments
			vec4 texColor = texture(screenTexture, TexCoords);
    
			// Helligkeitswert des Pixels
			float brightness = (texColor.r + texColor.g + texColor.b) / 3.0;

			// Originalfarbe beibehalten
			vec3 originalColor = texColor.rgb;
    
			// Pr�fen, ob der Pixel �ber dem Schwellenwert liegt
			if (brightness > bloomThreshold)
			{
				// Gl�hende Farbe berechnen
				vec3 finalGlowColor = glowColor * brightness;
        
				// Blur-Effekt
				vec3 blurColor = vec3(0.0);
				vec2 texelSize = 1.0 / textureSize(screenTexture, 0);
        
				for (int i = -1; i <= 1; i++)
				{
					for (int j = -1; j <= 1; j++)
					{
						vec2 offsettt = vec2(i, j) * texelSize * blurRadius;
						blurColor += texture(screenTexture, TexCoords + offsettt).rgb * blurKernel[(i + 1) * 3 + (j + 1)];
					}
				}
        
				// Gl�hende Farbe zum Blur-Ergebnis hinzuf�gen
				color = vec3(originalColor + texColor.rgb + finalGlowColor + blurColor);
			}
			else
			{
				FragColor = texColor;
			}
   

			col = color;
			FragColor = vec4(col, 1.0);
			break;
		default:
			col = vec3(texture(screenTexture, TexCoords));
			break;
	}


	FragColor = vec4(col, 1.0);
}
