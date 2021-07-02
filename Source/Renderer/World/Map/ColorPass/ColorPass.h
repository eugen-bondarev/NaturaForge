#pragma once

#include "ColorFBO.h"

#include "Werwel/Texture.h"
#include "Werwel/Shader.h"
#include "Werwel/VAO.h"

#include "Game/Entities/Camera.h"

#include "Renderer/Atlas/TextureAtlas.h"

#include "Renderer/Renderer.h"

struct BlockData {
  struct {
    float x { 0 }, y { 0 };
  } pos;

  struct {
    float x { 0 }, y { 0 };
  } tile;

  BlockData() = default;
  BlockData(float posX, float posY, float tileX, float tileY) {
    pos.x = posX;
    pos.y = posY;
    tile.x = tileX;
    tile.y = tileY;
  }
};

using WallData = BlockData;

class ColorPass {
public:
  ColorPass(int amountOfBlocks);

  void Perform(const Ref<Camera>& camera, int amountOfWalls, int amountOfBlocks, const std::vector<Ref<IRenderer>>& additionalRenderers = {});

  inline const Ref<Werwel::VBO>& GetBlocksVBO() const {
    return blocks.vbo;
  }

  inline const Ref<Werwel::VBO>& GetWallsVBO() const {
    return walls.vbo;
  }

  inline const Ref<ColorFBO>& GetFBO() const {
    return fbo;
  }

private:
  Ref<ColorFBO> fbo;
  Ref<Werwel::Shader> shader;

  struct {
    Ref<Werwel::VAO> vao;
    Ref<Werwel::VBO> vbo;
  } blocks;

  struct {
    Ref<Werwel::VAO> vao;
    Ref<Werwel::VBO> vbo;    
  } walls;

  Ref<BlocksTileMap> tileMap;
};