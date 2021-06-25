#pragma once

#include "Werwel/Texture.h"
#include "Werwel/VAO.h"
#include "Werwel/Shader.h"

#include "Game/Character.h"

#include "Game/Control/Camera.h"

#include "../Renderer.h"

class CharacterRenderer : public IRenderer {
public:
  CharacterRenderer(const std::vector<Ref<Character>>& characters, const Ref<Camera>& camera);
  void Render() override;

private:
  const std::vector<Ref<Character>>& characters;
  const Ref<Camera>& camera;

	Ref<Werwel::Texture> characterTexture;
	Ref<Werwel::VAO> characterVAO;
	Ref<Werwel::Shader> characterShader;

  CharacterRenderer(const CharacterRenderer&) = delete;
  CharacterRenderer& operator=(const CharacterRenderer&) = delete;
};