#include "Engine.h"

#include "Core/Input/Input.h"
#include "Core/Window.h"
#include "Core/Time.h"
#include "Core/Gui.h"

#include "Renderer/Entities/RectVao.h"

#include "Werwel/GraphicsContext.h"

Engine::Engine() {
	Window::Create();
	Gui::Create();
	Input::Create(Window::GetGlfwWindow());
	Primitives::Rect::Create();

	Linow::Init();
}

void Engine::InitResources() {
	FORGIO_PROFILER_SCOPE();
	map = CreateRef<Map>(Size(5, 5), Size(500, 500));
	camera = CreateRef<Camera>();
	camera->SetPosition(map->GetCenter() * map->GetBlockSize());
	map->CalculateVisibleChunks(camera->GetPosition());
	mapRenderer = CreateRef<MapRenderer>(map, camera);

	character = CreateRef<Character>(map);
	character->SetPosition(camera->GetPosition() + Vec2(0, 250.0f));
	characterRenderer = CreateRef<CharacterRenderer>();
}

bool Engine::IsRunning() const {
	return !Window::ShouldClose();
}

void Engine::BeginFrame() {
	Window::BeginFrame();
	Gui::BeginFrame();
	Time::BeginFrame();
	Input::BeginFrame();

	Linow::Clear();
}

static float foo;

void Engine::Control() {	
	if (Input::KeyDown(Key::Space)) {
		if (character->GetOnGround()) {
			character->Jump();
		}
	}

	if (Input::KeyDown(Key::A) && character->CanMoveLeft()) {
		character->SetPosition(character->GetPosition() + Vec2(-1, 0) * Time::GetDelta() * 75.0f);
		foo -= 0.3f;
		character->frame = truncf(foo);
		character->direction = -1;
	} else if (Input::KeyDown(Key::D) && character->CanMoveRight()) {
		character->SetPosition(character->GetPosition() + Vec2(1, 0) * Time::GetDelta() * 75.0f);
		foo += 0.3f;
		character->frame = truncf(foo);
		character->direction = 1;
	} else {
		character->frame = character->direction == 1 ? 0 : -1;
	}

	if (Input::KeyDown(Key::W)) {
		character->SetPosition(character->GetPosition() + Vec2(0, 1) * Time::GetDelta() * 100.0f);
	}

	if (Input::KeyDown(Key::S)) {
		if (character->GetOnGround()) {
			character->SetPosition(character->GetPosition() + Vec2(0, -1) * Time::GetDelta() * 100.0f);
		}
	}

	if (Input::MouseButtonDown(Button::Left)) {
		Map::BlockSettingData& settingBlock = map->SetBlock(camera->GetPosition(), BlockType::Empty);

		if (settingBlock.IsSet()) {
			mapRenderer->rerender = true;
			mapRenderer->chunksUpdated = true;
		}
	}

	if (Input::MouseButtonDown(Button::Right)) {
		Map::BlockSettingData& settingBlock = map->SetBlock(camera->GetPosition(), BlockType::Dirt);

		if (settingBlock.IsSet()) {
			mapRenderer->rerender = true;
			mapRenderer->chunksUpdated = true;
		}
	}
}

void Engine::Render() {
	camera->OnPositionChange([&]() {
		map->CalculateVisibleChunks(camera->GetPosition());
		mapRenderer->rerender = true;
	});

	character->Update(Time::GetDelta());
	character->CollectLights(mapRenderer->GetAdditionalLightData());
	camera->SetPosition(character->GetPosition());

	mapRenderer->Render([&]() {
		characterRenderer->Render({ character }, camera);
	});

	Linow::Render(Math::ToPtr(Window::GetSpace()), Math::ToPtr(camera->GetTransform()));

	ImGui::Begin("Info");
		ImGui::Text(("OnGround:" + std::to_string(character->GetOnGround())).c_str());
		ImGui::Text(("Ceiling:" + std::to_string(character->GetCeiling())).c_str());
		ImGui::Text(("FPS:" + std::to_string(Time::GetFps())).c_str());
		ImGui::Text(("Blocks:" + std::to_string(mapRenderer->GetAmountOfRenderedBlocks())).c_str());
		ImGui::Text(("Walls:" + std::to_string(mapRenderer->GetAmountOfRenderedWalls())).c_str());
		ImGui::Text(("Lights:" + std::to_string(mapRenderer->GetAmountOfRenderedLights())).c_str());
	ImGui::End();
	
	ImGui::Begin("View");
		ImGui::Text(("x: " + std::to_string(mapRenderer->GetVisibleChunks().x.start) + " " + std::to_string(mapRenderer->GetVisibleChunks().x.end)).c_str());
		ImGui::Text(("y: " + std::to_string(mapRenderer->GetVisibleChunks().y.start) + " " + std::to_string(mapRenderer->GetVisibleChunks().y.end)).c_str());
	ImGui::End();
}

void Engine::EndFrame() {
	Input::EndFrame();
	Time::EndFrame();
	Gui::EndFrame();
	Window::EndFrame();
}

Engine::~Engine() {
	Gui::Destroy();
	Window::Destroy();
}