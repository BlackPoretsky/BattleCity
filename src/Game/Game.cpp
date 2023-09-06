#include "Game.h"
#include "../Resources/ResourceManager.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/ShaderProgramm.h"
#include "../Renderer/AnimatedSprite.h"
#include "../Renderer/Texture2D.h"

#include "GameObject/Tank.h"
#include "Level.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

Game::Game(const glm::ivec2& windowSize) : m_eCurrentGameState(EGameState::Active), m_windowSize(windowSize)
{
    m_keys.fill(0);
}

Game::~Game()
{

}

void Game::render()
{
    if (m_pTank) 
    {
        m_pTank->render();
    }

    if (m_pLevel)
    {
        m_pLevel->render();
    }
}

void Game::update(const uint64_t delta)
{
    if (m_pLevel)
    {
        m_pLevel->update(delta);
    }

    if (m_pTank) 
    {
        if (m_keys[GLFW_KEY_W])
        {
            m_pTank->setOrientation(Tank::EOrientation::Top);
            m_pTank->move(true);
        }
        else if (m_keys[GLFW_KEY_A])
        {
            m_pTank->setOrientation(Tank::EOrientation::Left);
            m_pTank->move(true);
        }
        else if (m_keys[GLFW_KEY_D])
        {
            m_pTank->setOrientation(Tank::EOrientation::Right);
            m_pTank->move(true);
        }
        else if (m_keys[GLFW_KEY_S])
        {
            m_pTank->setOrientation(Tank::EOrientation::Bottom);
            m_pTank->move(true);
        }
        else
        {
            m_pTank->move(false);
        }
        m_pTank->update(delta);
    }
}

void Game::setKey(const int key, const int action)
{
    m_keys[key] = action;
}

bool Game::init()
{

    ResourceManager::loadJSONResiurces("res/resources.json");
    auto pSpriteShaderProgram = ResourceManager::getShaderProgram("spriteShader");
    if (!pSpriteShaderProgram)
    {
        std::cerr << "Can't find shader program: spriteShader" << std::endl;
        return false;
    }

    auto pTextureAtlas = ResourceManager::getTexture("mapTextureAtlas_8x8");
    if (!pTextureAtlas)
    {
        std::cerr << "Can't find texture atlas: mapTextureAtlas_8x8" << std::endl;
        return false;
    }

    auto pTanksTextureAtlas = ResourceManager::getTexture("tanksTextureAtlas");
    if (!pTanksTextureAtlas)
    {
        std::cerr << "Can't find texture atlas: tanksTextureAtlas" << std::endl;
        return false;
    }


    glm::mat4 porjectionMatrix = glm::ortho(0.f, static_cast<float>(m_windowSize.x), 0.f, static_cast<float>(m_windowSize.y), -100.f, 100.f);

    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMatrix4("projectionMat", porjectionMatrix);

    auto pTanksAnimatedSprite = ResourceManager::getAnimatedSprite("tankState");
    if (!pTanksAnimatedSprite)
    {
        std::cerr << "Can't find animated sprite: animateSprites" << std::endl;
        return false;
    }
    pTanksAnimatedSprite->setState("tankSprite_top");

    m_pTank = std::make_unique<Tank>(pTanksAnimatedSprite, 0.0000001f, glm::vec2(0), glm::vec2(16.f, 16.f));
    m_pLevel = std::make_unique<Level>(ResourceManager::getLevels()[0]);
    return true;
}
