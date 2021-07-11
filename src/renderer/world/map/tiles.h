#pragma once

#include "game/world/types.h"

bool TopBlockIsNot(const Blocks_t &blocks, int x, int y, BlockType type = BlockType::Empty);
bool LeftBlockIsNot(const Blocks_t &blocks, int x, int y, BlockType type = BlockType::Empty);
bool BottomBlockIsNot(const Blocks_t &blocks, int x, int y, BlockType type = BlockType::Empty);
bool RightBlockIsNot(const Blocks_t &blocks, int x, int y, BlockType type = BlockType::Empty);

using TileFunction = std::function<Vec2(const Blocks_t&, int, int)>;
using TileFunction1 = std::function<Vec2(BlockType up, BlockType down, BlockType left, BlockType right, int, int)>;

Vec2 GrassGenerator(const Blocks_t &blocks, int x, int y);
Vec2 DirtGenerator(const Blocks_t &blocks, int x, int y);

Vec2 GrassGenerator1(BlockType up, BlockType down, BlockType left, BlockType right, int x, int y);
Vec2 DirtGenerator1(BlockType up, BlockType down, BlockType left, BlockType right, int x, int y);

inline static std::map<BlockType, TileFunction> tileFunctions = {
	{ BlockType::Grass, GrassGenerator},
	{ BlockType::Dirt, DirtGenerator},
};


inline static std::map<BlockType, TileFunction1> tileFunctions1 = {
	{ BlockType::Grass, GrassGenerator1 },
	{ BlockType::Dirt, DirtGenerator1 },
};

TileFunction PickTileFunction(TileType type);
TileFunction1 PickTileFunction1(TileType type);

Vec2 PickRightAngularBlock(const Blocks_t &blocks, int x, int y);
Vec2 PickRightAngularWall(const Blocks_t &blocks, int x, int y);

Vec2 PickRightAngularBlock1(BlockType up, BlockType down, BlockType left, BlockType right, int x, int y);
Vec2 PickRightAngularWall1(BlockType up, BlockType down, BlockType left, BlockType right, int x, int y);