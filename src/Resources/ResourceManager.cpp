#include "ResourceManager.h"
#include "../Renderer/ShaderProgramm.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimatedSprite.h"

#include <sstream>
#include <fstream>
#include <iostream>

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>


#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

ResourceManager::ShaderProgramsMap ResourceManager::m_shaderPorgrams;
ResourceManager::TexturesMap ResourceManager::m_textures;
ResourceManager::SpriteMap ResourceManager::m_sprites;
ResourceManager::AnimatedSpriteMap ResourceManager::m_animatedSprites;
std::string ResourceManager::m_path;
std::vector<std::vector<std::string>> ResourceManager::m_levels;

void ResourceManager::setExecutablePath(const std::string& executablePath)
{
	size_t found = executablePath.find_last_of("/\\");
	m_path = executablePath.substr(0, found);
}

void ResourceManager::unloadAllResources()
{
	m_shaderPorgrams.clear();
	m_textures.clear();
	m_sprites.clear();
	m_animatedSprites.clear();
}

std::shared_ptr<RenderEngine::ShaderProgram> ResourceManager::loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath) 
{
	std::string vertexString = getFileString(vertexPath);
	if (vertexString.empty())
	{
		std::cerr << "No vertex shader!" << std::endl;
		return nullptr;
	}

	std::string fragmentString = getFileString(fragmentPath);
	if (fragmentString.empty())
	{
		std::cerr << "No fragment shader!" << std::endl;
		return nullptr;
	}

	std::shared_ptr<RenderEngine::ShaderProgram> newShader = m_shaderPorgrams.emplace(shaderName, std::make_shared<RenderEngine::ShaderProgram>(vertexString, fragmentString)).first->second;
	if (newShader->isCompiled())
	{
		return newShader;
	}

	std::cerr << "Can't load shader program:\n"
		<< "Vertex: " << vertexPath << "\n"
		<< "Fragment: " << fragmentPath << std::endl;
	
	return nullptr;
}

std::shared_ptr<RenderEngine::ShaderProgram> ResourceManager::getShaderProgram(const std::string& shaderName)
{
	ShaderProgramsMap::const_iterator it = m_shaderPorgrams.find(shaderName);
	if (it != m_shaderPorgrams.end())
	{
		return it->second;
	}

	std::cerr << "Can't find the shader program: " << shaderName << std::endl;

	return nullptr;
}

std::shared_ptr<RenderEngine::Texture2D> ResourceManager::loadTexture(const std::string& textureName, const std::string& texturePath)
{
	int channels = 0;
	int width = 0;
	int hieght = 0;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* pixels = stbi_load(std::string(m_path + "/" + texturePath).c_str(), &width, &hieght, &channels, 0);
	
	if (!pixels)
	{
		std::cerr << "Can't load the texture: " << textureName << std::endl;
		return nullptr;
	}

	std::shared_ptr<RenderEngine::Texture2D> newtexture = m_textures.emplace(textureName, std::make_shared<RenderEngine::Texture2D>(width, hieght, pixels, channels, 
																															GL_NEAREST, GL_CLAMP_TO_EDGE)).first->second;
	stbi_image_free(pixels);
	
	return newtexture;
}

std::shared_ptr<RenderEngine::Texture2D> ResourceManager::getTexture(const std::string& textureName)
{
	TexturesMap::const_iterator it = m_textures.find(textureName);
	if (it != m_textures.end())
	{
		return it->second;
	}

	std::cerr << "Can't find the texture: " << textureName << std::endl;

	return nullptr;
}

std::shared_ptr<RenderEngine::Sprite> ResourceManager::loadSprite(const std::string& spriteName, 
																const std::string& textureName,
																const std::string& shaderName,
																const std::string& subTextureName)
{
	auto pTexture = getTexture(textureName);
	if (!pTexture)
	{
		std::cerr << "Can't find the texture: " << textureName << "for the sprite: " << spriteName << std::endl;
		return nullptr;
	}

	auto pShader = getShaderProgram(shaderName);
	if (!pShader)
	{
		std::cerr << "Can't find the shader program: " << shaderName << "for the sprite: " << spriteName << std::endl;
		return nullptr;
	}

	std::shared_ptr<RenderEngine::Sprite> newsprite = m_sprites.emplace(spriteName, std::make_shared<RenderEngine::Sprite>(pTexture, subTextureName, pShader)).first->second;

	return newsprite;
}

std::shared_ptr<RenderEngine::Sprite> ResourceManager::getSprite(const std::string& spriteName)
{
	SpriteMap::const_iterator it = m_sprites.find(spriteName);
	if (it != m_sprites.end())
	{
		return it->second;
	}

	std::cerr << "Can't find the sprite: " << spriteName << std::endl;

	return nullptr;
}

std::shared_ptr<RenderEngine::AnimatedSprite> ResourceManager::loadAnimatedSprite(const std::string& spriteName, const std::string& textureName, const std::string& shaderName, const std::string& subTextureName)
{
	auto pTexture = getTexture(textureName);
	if (!pTexture)
	{
		std::cerr << "Can't find the texture: " << textureName << "for the sprite: " << spriteName << std::endl;
		return nullptr;
	}

	auto pShader = getShaderProgram(shaderName);
	if (!pShader)
	{
		std::cerr << "Can't find the shader program: " << shaderName << "for the sprite: " << spriteName << std::endl;
		return nullptr;
	}

	std::shared_ptr<RenderEngine::AnimatedSprite> newAnimatedSprite = m_animatedSprites.emplace(spriteName, std::make_shared<RenderEngine::AnimatedSprite>(pTexture, subTextureName, pShader)).first->second;

	return newAnimatedSprite;
}

std::shared_ptr<RenderEngine::AnimatedSprite> ResourceManager::getAnimatedSprite(const std::string& spriteName)
{
	AnimatedSpriteMap::const_iterator it = m_animatedSprites.find(spriteName);
	if (it != m_animatedSprites.end())
	{
		return it->second;
	}

	std::cerr << "Can't find the animated sprite: " << spriteName << std::endl;

	return nullptr;
}

std::shared_ptr<RenderEngine::Texture2D> ResourceManager::loadTextureAtlas(const std::string& textureName, 
																		const std::string& texturePath,
																		const std::vector<std::string> subTextures,
																		const unsigned int subTextureWidth, 
																		const unsigned int subTextureHieght)
{
	auto pTexture = loadTexture(textureName, texturePath);
	if (pTexture)
	{
		const unsigned int textureWidth = pTexture->getWidth();
		const unsigned int textureHieght = pTexture->getHieght();
		unsigned int currentTextureOffsetX = 0;
		unsigned int currentTextureOffsetY = textureHieght;
		for (const auto& currentSubTextureName : subTextures)
		{
			glm::vec2 leftBottomUB(static_cast<float>(currentTextureOffsetX)/textureWidth,					   static_cast<float>(currentTextureOffsetY- subTextureHieght)/ textureHieght);
			glm::vec2 rightTopUB(static_cast<float>(currentTextureOffsetX + subTextureWidth) / textureWidth, static_cast<float>(currentTextureOffsetY) / textureHieght);
			
			pTexture->addSubTexture(currentSubTextureName, leftBottomUB, rightTopUB);

			currentTextureOffsetX += subTextureWidth;
			if (currentTextureOffsetX >= textureWidth)
			{
				currentTextureOffsetX = 0;
				currentTextureOffsetY -= subTextureHieght;
			}
		}
	}
	return pTexture;
}

bool ResourceManager::loadJSONResiurces(const std::string& JSONPath)
{
	const std::string JSONString = getFileString(JSONPath);
	if (JSONString.empty())
	{
		std::cerr << "No JSON resource file!" << std::endl;
		return false;
	}
	
	rapidjson::Document document;
	rapidjson::ParseResult parseResult = document.Parse(JSONString.c_str());
	if (!parseResult)
	{
		std::cerr << "JSON parse error: " << rapidjson::GetParseError_En(parseResult.Code()) << "(" << parseResult.Offset() << ")" << std::endl;
		std::cerr << "In JSON file: " << JSONPath << std::endl;
		return false;
	}

	auto shaderIt = document.FindMember("shaders");
	if (shaderIt != document.MemberEnd())
	{
		for (const auto& currentShader : shaderIt->value.GetArray())
		{
			const std::string name = currentShader["name"].GetString();
			const std::string filePath_v = currentShader["filePath_v"].GetString();
			const std::string filePath_f = currentShader["filePath_f"].GetString();
			loadShaders(name, filePath_v, filePath_f);
		}
	}

	auto textureAtlasesIt = document.FindMember("textureAtlases");
	if (textureAtlasesIt != document.MemberEnd())
	{
		for (const auto& currentTextureAtlas : textureAtlasesIt->value.GetArray())
		{
			const std::string name = currentTextureAtlas["name"].GetString();
			const std::string filePath = currentTextureAtlas["filePath"].GetString();
			const unsigned int subTextureWidth = currentTextureAtlas["subTextureWidth"].GetUint();
			const unsigned int subTextureHieght = currentTextureAtlas["subTextureHeight"].GetUint();

			const auto subTextureArray = currentTextureAtlas["subTextures"].GetArray();
			std::vector<std::string> subTextures;
			subTextures.reserve(subTextureArray.Size());

			for (const auto& currentSubTexture : subTextureArray)
			{
				subTextures.emplace_back(currentSubTexture.GetString());
			}

			loadTextureAtlas(name, filePath, std::move(subTextures), subTextureWidth, subTextureHieght);
		}
	}

	auto spritesIt = document.FindMember("sprites");
	if (spritesIt != document.MemberEnd())
	{
		for (const auto& currentSprite : spritesIt->value.GetArray())
		{
			const std::string name = currentSprite["name"].GetString();
			const std::string textureAtlas = currentSprite["textureAtlas"].GetString();
			const std::string shader = currentSprite["shader"].GetString();
			const std::string subTextureName = currentSprite["subTextureName"].GetString();

			auto pSprite = loadSprite(name, textureAtlas, shader, subTextureName);
			if (!pSprite)
			{
				continue;
			}
		}
	}

	auto animatedSpritesIt = document.FindMember("animateSprites");
	if (animatedSpritesIt != document.MemberEnd())
	{
		for (const auto& currentAnimatedSprite : animatedSpritesIt->value.GetArray())
		{
			const std::string name = currentAnimatedSprite["name"].GetString();
			const std::string textureAtlas = currentAnimatedSprite["textureAtlas"].GetString();
			const std::string shader = currentAnimatedSprite["shader"].GetString();
			const std::string initialSubTexture = currentAnimatedSprite["initialSubTexture"].GetString();

			auto pAnimatedSprite = loadAnimatedSprite(name, textureAtlas, shader, initialSubTexture);
			if (!pAnimatedSprite)
			{
				continue;
			}

			const auto statesArray = currentAnimatedSprite["states"].GetArray();
			std::vector<std::pair<std::string, uint64_t>> states;
			states.reserve(statesArray.Size());
			for (const auto& currentState : statesArray)
			{
				const std::string stateName = currentState["stateName"].GetString();
				const auto framesArray = currentState["frames"].GetArray();
				std::vector<std::pair<std::string, uint64_t>> frames;
				frames.reserve(framesArray.Size());
				for (const auto& currentFrame : framesArray)
				{
					const std::string subTexture = currentFrame["subTexture"].GetString();
					const uint64_t duration = currentFrame["duration"].GetUint();
					frames.emplace_back(std::move(std::pair<std::string, uint64_t>(subTexture, duration)));
				}
				pAnimatedSprite->insertState(stateName, std::move(frames));
			}

		}
	}

	auto levelsIt = document.FindMember("levels");
	if (levelsIt != document.MemberEnd())
	{
		for (const auto& currentLevel : levelsIt->value.GetArray())
		{
			const auto description = currentLevel["description"].GetArray();
			std::vector<std::string> levelRows;
			levelRows.reserve(description.Size());

			size_t maxLenght = 0;
			for (const auto& currentRow : description)
			{
				levelRows.emplace_back(currentRow.GetString());
				if (maxLenght < levelRows.back().length())
				{
					maxLenght = levelRows.back().length();
				}
			}
			
			for (auto& currentRow : levelRows)
			{
				while (currentRow.length() < maxLenght)
				{
					currentRow.append("D");
				}
			}
			
			m_levels.emplace_back(std::move(levelRows));
		}
	}

	return true;
}

std::string ResourceManager::getFileString(const std::string& relativeFilePath)
{
	std::ifstream f;
	f.open(m_path + "/" + relativeFilePath.c_str(), std::ios::in | std::ios::binary);
	if (!f.is_open())
	{
		std::cerr << "Failed to open file: " << relativeFilePath << std::endl;
		return std::string{};
	}

	std::stringstream buffer;
	buffer << f.rdbuf();
	return buffer.str();
}