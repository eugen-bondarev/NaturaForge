#include "rigidbody.h"

#include "Linow/Linow.h"

#include "interface/console.h"

#include "sstream"

Rigidbody::Rigidbody(Entity *entity) : Component(entity)
{
}

Blocks::Representations::IndexAndPosition Rigidbody::GetBlockNearby(Vec2 fix, Vec2 offset) const
{
	Blocks::Representations::IndexAndPosition block;

	block.worldPosition = ToInt((entity->position + fix + offset * Map::BLOCK_SIZE) / Map::BLOCK_SIZE) * Map::BLOCK_SIZE - 8.0f;
	block.index = (block.worldPosition + 8.0f) / Map::BLOCK_SIZE;

	return block;
}

static constexpr bool RENDER_COLLIDERS = false;

void Rigidbody::CheckCollisions()
{
	// auto &blocks = map->GetBlocks();
	auto& blocks = Map::GetBlocks();

	onGround = false;
	ceiling = false;
	canMoveLeft = true;
	canMoveRight = true;

	// - Map::VisibleChunks.x.start;
	// - Map::VisibleChunks.y.start;

	for (int i = 2; i < 4; i++)
	{
		Blocks::Representations::IndexAndPosition block = GetBlockNearby(Vec2(-10.0f, 10.0f), Vec2(i, 0));

		if (RENDER_COLLIDERS)
			Linow::AddQuad(block.worldPosition, block.worldPosition + Map::BLOCK_SIZE);

		if (blocks[block.index.x - Map::VisibleChunks.x.start][block.index.y - Map::VisibleChunks.y.start].type != BlockType::Empty)
		{
			// entity->SetPositionY(block.worldPosition.y + Map::BLOCK_SIZE - 4.0f);

			if (velocity.y < 0)
			{
				// We don't want to test if we fell through a couple of blocks, if velocity is low.
				int amountOfBlocksToTest = std::max<float>(sqrt(abs(velocity.y)) - 25.0f, 0.0f);
				for (int j = amountOfBlocksToTest; j >= 1; j--)
				{
					if (blocks[block.index.x - Map::VisibleChunks.x.start][block.index.y - Map::VisibleChunks.y.start + j].type != BlockType::Empty)
					{
						// entity->SetPositionY(block.worldPosition.y + Map::BLOCK_SIZE * j + Map::BLOCK_SIZE - 4.0f);
						break;
					}
				}
			}

			onGround = true;
			break;
		}
	}

	for (int i = 2; i < 4; i++)
	{
		Blocks::Representations::IndexAndPosition block = GetBlockNearby(Vec2(-10.0f, -10.0f), Vec2(i, 6));

		if (RENDER_COLLIDERS)
			Linow::AddQuad(block.worldPosition, block.worldPosition + Map::BLOCK_SIZE);

		if (blocks[block.index.x - Map::VisibleChunks.x.start][block.index.y - Map::VisibleChunks.y.start - 1].type != BlockType::Empty)
		{
			// entity->SetPositionY(block.worldPosition.y - 5 * Map::BLOCK_SIZE);
			ceiling = true;
			break;
		}
	}

	for (int i = 1; i < 5; i++)
	{
		Blocks::Representations::IndexAndPosition block = GetBlockNearby(Vec2(-2.0f, 10.0f * (onGround ? 1 : -1)), Vec2(1, i));

		if (RENDER_COLLIDERS)
			Linow::AddQuad(block.worldPosition, block.worldPosition + Map::BLOCK_SIZE);

		if (blocks[block.index.x - Map::VisibleChunks.x.start][block.index.y - Map::VisibleChunks.y.start].type != BlockType::Empty)
		{
			canMoveLeft = false;
			break;
		}
	}

	for (int i = 1; i < 5; i++)
	{
		Blocks::Representations::IndexAndPosition block = GetBlockNearby(Vec2(-2.0f, 10.0f * (onGround ? 1 : -1)), Vec2(3, i));

		if (RENDER_COLLIDERS)
			Linow::AddQuad(block.worldPosition, block.worldPosition + Map::BLOCK_SIZE);

		if (blocks[block.index.x - Map::VisibleChunks.x.start][block.index.y - Map::VisibleChunks.y.start].type != BlockType::Empty)
		{
			canMoveRight = false;
			break;
		}
	}
}

void Rigidbody::CheckCollisions1()
{
	Blocks_t& blocks = Map::GetBlocks();

	onGround = false;
	ceiling = false;
	canMoveLeft = true;
	canMoveRight = true;

	Vec2 pos = trunc((entity->position + 8.0f) / 16.0f) * 16.0f;

	// MW_LOG_VAR(entity->position.y);
	Console::PushInfo(std::to_string(pos.y));

	for (int i = 0; i < Map::SolidBlocks.size(); i++)
	{
		if (pos.y == Map::SolidBlocks[i].y && abs(pos.x - Map::SolidBlocks[i].x) <= 8.0f)
		{
			onGround = true;

			// std::ostringstream ss;
			// ss << "Block pos: " << std::to_string(pos.x) << ' ' << std::to_string(pos.y);
			// Console::PushInfo(ss.str());
			entity->SetPositionY(pos.y + 7);

			break;

		}
	}

	// int chunksx = Map::VisibleChunks.x.end - Map::VisibleChunks.x.start;
	// int chunksy = Map::VisibleChunks.y.end - Map::VisibleChunks.y.start;
	// chunksx *= 2;
	// chunksy *= 2;
	// chunksx /= 2;
	// chunksy /= 2;

	// Vec2 block_coords = trunc(Vec2(chunksx, chunksy));
	// BlockType block = Map::GetBlocks()[block_coords.x][block_coords.y].type;

	// if (block != BlockType::Empty)
	// {
	// 	onGround = true;
	// }
}

void Rigidbody::Update()
{
	MW_PROFILER_SCOPE();

	canMoveLeft = true;
	canMoveRight = true;

	if (!onGround)
	{
		entity->SetPosition(entity->position + velocity * Vec2(0, 1) * mw::Time::GetDelta());
		velocity += -Physics::g * mw::Time::GetDelta() * velocityFactor;

		if (ceiling)
		{
			velocity.y = 0.0f;
			ceiling = false;
		}
	}
	else
	{
		velocity.y = 0.0f;
	}

	CheckCollisions1();
	// CheckCollisions();
}

void Rigidbody::Jump()
{
	entity->SetPositionY(entity->position.y + Map::BLOCK_SIZE - 4.0f - 10.0f);
	velocity = Vec2(0.0f, 3.25f * velocityFactor);
	onGround = false;
}

void Rigidbody::SetCeiling(bool value) { ceiling = value; }
void Rigidbody::SetOnGround(bool value) { onGround = value; }
void Rigidbody::CanMoveLeft(bool value) { canMoveLeft = value; }
void Rigidbody::CanMoveRight(bool value) { canMoveRight = value; }

bool Rigidbody::GetCeiling() const { return ceiling; }
bool Rigidbody::GetOnGround() const { return onGround; }
bool Rigidbody::CanMoveLeft() const { return canMoveLeft; }
bool Rigidbody::CanMoveRight() const { return canMoveRight; }