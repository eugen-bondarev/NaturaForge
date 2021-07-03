#include "CharacterRenderer.h"

#include "Assets/ImageAsset.h"
#include "Assets/TextAsset.h"

#include "Maths/Primitive.h"

#include "Core/Window.h"

CharacterRenderer::CharacterRenderer(const std::vector<Ref<Character>>& characters, const Ref<Camera>& camera) : characters { characters }, camera { camera } {
	const ImageAsset characterBodyTextureAsset("Assets/Images/Characters/Char_NoHand2.png");
	characterBodyTexture = CreateRef<Werwel::Texture>(
		characterBodyTextureAsset.GetSize(),
		characterBodyTextureAsset.GetData(),
		GL_RGBA,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_NEAREST },
		Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_NEAREST }
	);

	const ImageAsset characterHandTextureAsset("Assets/Images/Characters/Hand2.png");
	characterHandTexture = CreateRef<Werwel::Texture>(
		characterHandTextureAsset.GetSize(),
		characterHandTextureAsset.GetData(),
		GL_RGBA,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_NEAREST },
		Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_NEAREST }
	);

	const ImageAsset pickaxeTextureAsset("Assets/Images/Pickaxe1.png");
	pickaxeTexture = CreateRef<Werwel::Texture>(
		pickaxeTextureAsset.GetSize(),
		pickaxeTextureAsset.GetData(),
		GL_RGBA,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_NEAREST },
		Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_NEAREST }
	);

	const auto& vertices = Primitives::Char::Vertices(32 * 2.0f, 48 * 2.0f);
	const auto& indices = Primitives::Char::indices;

	characterVAO = CreateRef<Werwel::VAO>();
	characterVAO->Bind();		
		characterVAO->AddVBO(Werwel::VBO::Type::Array, Werwel::VBO::Usage::Static, vertices.size(), sizeof(Vertex2D), &vertices[0], Vertex2D::GetLayout());
		characterVAO->AddVBO(Werwel::VBO::Type::Indices, Werwel::VBO::Usage::Static, indices.size(), sizeof(int), &indices[0]);

	TextAsset vsCode("Assets/Shaders/Characters/Default.vs");
	TextAsset fsCode("Assets/Shaders/Characters/Default.fs");
	characterShader = CreateRef<Werwel::Shader>(
		vsCode.GetContent(), fsCode.GetContent(),
		"u_Proj", "u_View", "u_Model", "u_Frame", "u_Frame1", "u_Frames_Vert", "u_Direction"
	);
}

void CharacterRenderer::Render() {  
	characterShader->Bind();
	characterShader->SetMat4x4("u_Proj", Math::ToPtr(Window::GetSpace()));
	characterShader->SetMat4x4("u_View", Math::ToPtr(camera->GetTransform()));

  characterVAO->Bind();
  characterVAO->GetIndexBuffer()->Bind();

  for (const auto& character : characters) {
		Mat4 t = character->GetTransform();
		t = Math::Translate(t, Vec3(character->animator->GetDirection() * -24.0f, 0, 0));
		t = Math::Scale(t, Vec3(character->animator->GetDirection(), 1, 1));
    characterShader->SetMat4x4("u_Model", Math::ToPtr(t));

		characterShader->SetFloat("u_Frame", truncf(character->animator->GetFrame()));
		characterShader->SetFloat("u_Frame1", 0);
		characterShader->SetFloat("u_Frames_Vert", 1.0f);
		characterShader->SetFloat("u_Direction", static_cast<float>(character->animator->GetDirection()));
  		characterBodyTexture->Bind();
				glDrawElements(GL_TRIANGLES, characterVAO->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr);

		if (character->animator->state == 1) {
			characterShader->SetFloat("u_Frame", truncf(character->animator->GetAttackFrame()));
		} else {
			characterShader->SetFloat("u_Frame", truncf(character->animator->GetFrame()));
		}
		
		characterShader->SetFloat("u_Frame1", character->animator->state);
		characterShader->SetFloat("u_Frames_Vert", 2.0f);
  		pickaxeTexture->Bind();
				glDrawElements(GL_TRIANGLES, characterVAO->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
		characterShader->SetFloat("u_Frame1", character->animator->state);
		characterShader->SetFloat("u_Frames_Vert", 2.0f);
  		characterHandTexture->Bind();
				glDrawElements(GL_TRIANGLES, characterVAO->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
  }
}