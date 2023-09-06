#include "IndexBuffer.h"

namespace RenderEngine {

	IndexBuffer::IndexBuffer() : m_idBuffer(0), m_count(0)
	{
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_idBuffer);
	}

	IndexBuffer::IndexBuffer(IndexBuffer&& indexBuffer) noexcept
	{
		m_idBuffer = indexBuffer.m_idBuffer;
		indexBuffer.m_idBuffer = 0;
		m_count = indexBuffer.m_count;
		indexBuffer.m_count = 0;
	}

	IndexBuffer& IndexBuffer::operator=(IndexBuffer&& indexBuffer) noexcept
	{
		m_idBuffer = indexBuffer.m_idBuffer;
		indexBuffer.m_idBuffer = 0;
		m_count = indexBuffer.m_count;
		indexBuffer.m_count = 0;
		return *this;
	}

	void IndexBuffer::init(const void* data, const unsigned int count)
	{
		m_count = count;
		glGenBuffers(1, &m_idBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_idBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);
	}

	void IndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_idBuffer);
	}

	void IndexBuffer::unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}
