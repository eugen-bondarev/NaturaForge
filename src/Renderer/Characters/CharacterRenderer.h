#pragma once

#include "Werwel/Texture.h"
#include "Werwel/VAO.h"
#include "Werwel/Shader.h"

#include "Game/Entities/Character.h"
#include "Game/Entities/Camera.h"

#include "../Renderer.h"

class CharacterRenderer : public IRenderer {
public:
  CharacterRenderer(const std::vector<Ref<Character>>& characters, const Ref<Camera>& camera);
  void Render() override;

  inline static const Vec2 HUMANOID_SIZE = Vec2(64.0f, 96.0f);

private:

  const std::vector<Ref<Character>>& characters;
  const Ref<Camera>& camera;

	Ref<Werwel::Texture> characterBodyTexture;
	Ref<Werwel::Texture> characterHandTexture;
  
	Ref<Werwel::VAO> characterVAO;
	Ref<Werwel::Shader> characterShader;

  CharacterRenderer(const CharacterRenderer&) = delete;
  CharacterRenderer& operator=(const CharacterRenderer&) = delete;
};