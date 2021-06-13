#pragma once

#include "GpuEntity.h"

#include "VertexBufferLayout.h"

namespace Werwel {

class VBO : public GpuEntity {
public:
  enum class Usage {
    Static,
    Dynamic,
    Stream
  };

  enum class Type {
    Array,
    Indices
  };

  inline VBO(GLuint attribute, Type type, Usage usage, size_t amount = 0, size_t size = 0, const void* data = nullptr, std::vector<VertexBufferLayout> layouts = {}) {
    this->usage = VBOUsageToGLUsage(usage);
    this->type = VBOTypeToGLUsage(type);
    this->size = size;

    GLuint startAttribute = attribute;
    attributes.resize(layouts.size());
    
		glGenBuffers(1, &handle);
		glBindBuffer(this->type, handle);

      if (attribute != -1) {
        glEnableVertexAttribArray(attribute);
          for (int i = 0; i < attributes.size(); i++) {
            int attrib = startAttribute + i;
            glVertexAttribPointer(attrib, layouts[i].size, GL_FLOAT, GL_FALSE, layouts[i].stride, reinterpret_cast<void *>(layouts[i].offset));
            glVertexAttribDivisor(attrib, layouts[i].divisor);

            attributes[i] = attrib;
          }
        glDisableVertexAttribArray(attribute);
      }

      glBufferData(this->type, size * amount, data, this->usage);
		glBindBuffer(this->type, 0);

    if (type == VBO::Type::Indices) {
      indexCount = amount;
    }
  }

  inline ~VBO() override {
    glBindBuffer(this->type, 0);
    glDeleteBuffers(1, &handle);

		WERWEL_DEBUG_LOG_OUT("[Call] VBO destructor");
  }

  inline void Bind() const override {
    glBindBuffer(this->type, handle);
  }
  
  inline void Unbind() const override {
	  glBindBuffer(this->type, 0);
  }

  template <bool bind = true>
  inline void Store(size_t amount, const void* data) const {
    if (bind) {
      glBindBuffer(type, handle);
    }
    glBufferData(type, size * amount, data, usage);
  }

  template <typename T, bool bind = true>
  inline void Store(const std::vector<T>& vec) const {
    if (bind) {
      glBindBuffer(type, handle);
    }
    glBufferData(type, vec.size() * size, vec.data(), usage);
  }

  template <typename T, bool bind = true>
  inline void Update(const std::vector<T>& vec, int amount, int pos = 0) const {
    if (bind) {
      glBindBuffer(type, handle);
    }
    glBufferSubData(type, size * pos, size * amount, vec.data());
  }

  template <bool bind = true>
  inline void* Map() const {
    if (bind) {
      glBindBuffer(type, handle);
    }
    return glMapBuffer(type, GL_WRITE_ONLY);
  }

  inline bool Unmap() const {
    return glUnmapBuffer(type);
  }

  inline GLenum GetUsage() const {
    return usage;
  }

  inline GLenum GetType() const {
    return type;
  }

  inline const std::vector<GLuint>& GetUsedAttributes() const {
    return attributes;
  }

  inline GLuint GetIndexCount() const {
    return indexCount;
  }

  inline size_t GetSize() const {
    return size;
  }

private:
  std::vector<GLuint> attributes;

  GLuint indexCount;
  GLenum type;
  GLenum usage;
  size_t size;

  inline static GLenum VBOUsageToGLUsage(VBO::Usage vboUsage) {
    switch (vboUsage) {
      case VBO::Usage::Static:
        return GL_STATIC_DRAW;
      case VBO::Usage::Dynamic:
        return GL_DYNAMIC_DRAW;
      case VBO::Usage::Stream:
        return GL_STREAM_DRAW;
    }

    return GL_STATIC_DRAW;
  }

  inline static GLenum VBOTypeToGLUsage(VBO::Type vboUsage) {
    switch (vboUsage) {
      case VBO::Type::Array:
        return GL_ARRAY_BUFFER;
      case VBO::Type::Indices:
        return GL_ELEMENT_ARRAY_BUFFER;
    }

    return GL_ARRAY_BUFFER;
  }

  VBO(const VBO&) = delete;
  VBO& operator=(const VBO&) = delete;
};

}