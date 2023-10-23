#pragma once

#include "drawable.h"

#include "stb_image/stb_image.h"

// Higher = repeat more
struct TextureRepeatFactor
{
	float factorU = 1.0f;
	float factorV = 1.0f;
};

struct TerrainOptions
{
	int DesiredWidth;
	int DesiredLength;
	float MaxHeight;
	TextureRepeatFactor RepeatFactor;
};


class Terrain : public Drawable
{
public:
	using Drawable::Drawable;
	Terrain(const char* heightmapPath, const Shader& shader, const TerrainOptions& options);
	Terrain(const char* heightmapPath, const Shader& shader, const Texture& texture, const TerrainOptions& options);
	void Draw(PerspectiveCamera& camera) override;
	void Draw(PerspectiveCamera& camera, const glm::vec3& color) override;

private:
	unsigned int m_desiredSize;
	int m_width, m_height, m_nChannels;
};

