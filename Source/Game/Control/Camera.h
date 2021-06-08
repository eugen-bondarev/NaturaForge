#pragma once

#include "Math/Math.h"

class Camera {
public:
  Camera() = default;

  inline const Mat4& GetViewMatrix() const {
    return matrix;
  }

  inline const Vec2& GetPosition() const {
    return position;
  }

  inline const Vec2& GetLastPosition() const {
    return lastPosition;
  }

  template <typename... Args>
  inline void SetPosition(Args... args) {
    position = Vec2(std::forward<Args>(args)...);
    CalculateViewMatrix();
  }

  template <typename... Args>
  inline void AddPosition(Args... args) {
    Vec2 delta = Vec2(std::forward<Args>(args)...);
    SetPosition(position + delta);
  }

  template <typename... Args>
  inline void SetLastPosition(Args... args) {
    lastPosition = Vec2(std::forward<Args>(args)...);
  }

  inline void CalculateViewMatrix() {
	  matrix = Math::Inverse(Math::Translate(Mat4(1), Vec3(position, 0.0f)));
  }

private:
  Mat4 matrix;
  Vec2 position;
  Vec2 lastPosition;
};