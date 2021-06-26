#pragma once

#include "Maths/Maths.h"

#include "Core/Window.h"

#include "../ECS/ITransform.h"

class Camera : public ITransform {
public:
  Camera() = default;

  const Vec2& GetLastPosition() const;

  template <typename... Args>
  inline void SetLastPosition(Args... args) {
    lastPosition = Vec2(std::forward<Args>(args)...);
  }

  Vec2 GetPositionOnScreen(Vec2 worldPosition) const;
  void OnPositionChange(std::function<void()> Callback);

private:
  Vec2 lastPosition; 

  virtual void CalculateTransform() override;
};