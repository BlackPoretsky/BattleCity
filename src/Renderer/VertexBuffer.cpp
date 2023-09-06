#include "VertexBuffer.h"

namespace RenderEngine {

	VertexBuffer::VertexBuffer() : m_idBuffer(0)
	{
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_idBuffer);
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& VertexBuffer) noexcept
	{
		m_idBuffer = VertexBuffer.m_idBuffer;
		VertexBuffer.m_idBuffer = 0;
	}

	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& VertexBuffer) noexcept
	{
		m_idBuffer = VertexBuffer.m_idBuffer;
		VertexBuffer.m_idBuffer = 0;
		return *this;
	}

	void VertexBuffer::init(const void* data, const unsigned int size)
	{
		glGenBuffers(1, &m_idBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_idBuffer);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	void VertexBuffer::update(const void* data, const unsigned int size) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_idBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	void VertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_idBuffer);
	}

	void VertexBuffer::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}
