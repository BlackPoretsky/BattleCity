#include "Renderer.h"

void RenderEngine::Renderer::draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const ShaderProgram& shader)
{
	shader.use();
	vertexArray.bind();
	indexBuffer.bind();

	glDrawElements(GL_TRIANGLES, indexBuffer.getCount(), GL_UNSIGNED_INT, nullptr);
}

void RenderEngine::Renderer::setClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void RenderEngine::Renderer::clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void RenderEngine::Renderer::setViewport(unsigned int width, unsigned int hieght, unsigned int leftOffset, unsigned int bottomOffset)
{
	glViewport(leftOffset, bottomOffset, width, hieght);
}

std::string RenderEngine::Renderer::getRendererStr()
{
	return (char*)glGetString(GL_RENDERER);
}

std::string RenderEngine::Renderer::getVersionStr()
{
	return (char*)glGetString(GL_VERSION);
}
