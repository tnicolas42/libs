#ifndef COMMONINCLUDE_HPP_
#define COMMONINCLUDE_HPP_

// glad
#include <glad/glad.h>

// glm
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtx/string_cast.hpp"  // to print vect/mat with glm::to_string

#define GET_R(color) ((color >> 16) & 0xFF)
#define GET_G(color) ((color >>  8) & 0xFF)
#define GET_B(color) ((color >>  0) & 0xFF)
#define TO_OPENGL_COLOR(color) glm::vec4(GET_R(color) / 255.0, GET_G(color) / 255.0, GET_B(color) / 255.0, 1.0)

namespace glm {
  template <typename T>
  bool operator<(tvec3<T> const &lhs, tvec3<T> const &rhs) {
    return (lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y)
	|| (lhs.x < rhs.x && lhs.y < rhs.y)
	|| (lhs.x == rhs.x && lhs.y == rhs.y && lhs.z < rhs.z));
  }
};

#endif  // COMMONINCLUDE_HPP_
