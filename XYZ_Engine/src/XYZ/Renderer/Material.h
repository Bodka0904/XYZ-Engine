#pragma once
#include "Shader.h"
#include "Texture.h"

#include <queue>
#include <bitset>
#include <unordered_set>

namespace XYZ {
	class Material
	{
		friend class MaterialInstance;
	public:
		Material(const std::shared_ptr<Shader>& shader);
		~Material();

		template<typename T>
		void Set(const std::string& name, const T& val)
		{
			auto uni = m_Shader->FindUniform(name);
			XYZ_ASSERT(uni, "Material uniform does not exist ", name.c_str());
			XYZ_ASSERT(uni->offset + uni->size <= m_Shader->GetUniformSize(), "Material uniform buffer out of range");
			memcpy(m_Buffer + uni->offset, (unsigned char*)& val, uni->size);

			for (auto& it : m_MaterialInstances)
				it->UpdateMaterialValue(uni);
		}
		void Set(const std::string& name, std::shared_ptr<Texture2D> texture)
		{
			auto tex = m_Shader->FindTexture(name);
			XYZ_ASSERT(tex, "Material texture does not exist ", name.c_str());

			if ((int)m_Textures.size() <= tex->slot)
				m_Textures.resize((size_t)tex->slot + 1);

			m_Textures[tex->slot] = texture;
		}
		void SetRoutine(const std::string& name)
		{
			m_Shader->SetSubRoutine(name);
		}
		void SetFlags(int32_t renderFlags)
		{		
			m_Key |= (int64_t)renderFlags << 16;
		}
		void SetFlag(int32_t renderFlag)
		{		
			m_Key |= (int64_t)renderFlag << 16;
		}

		void Bind();
	

		int64_t GetSortKey() { return m_Key; }

		const std::shared_ptr<Shader>& GetShader() { return m_Shader; }

		static std::shared_ptr<Material> Create(const std::shared_ptr<Shader>& shader);

		//TODO TEMPORARY
		void ReloadShader() { m_Shader->Reload(); };
	private:
		void OnShaderReload();

	private:
		std::shared_ptr<Shader>	   m_Shader;
		std::unordered_set<MaterialInstance*> m_MaterialInstances;
		std::vector<std::shared_ptr<Texture>> m_Textures;

		unsigned char* m_Buffer;
		int64_t m_Key = 0;
	};


	class MaterialInstance
	{
		friend class Material;
	public:
		MaterialInstance(const std::shared_ptr<Material>& material);
		~MaterialInstance();

		template<typename T>
		void Set(const std::string& name, const T& val)
		{
			auto uni = m_Material->m_Shader->FindUniform(name);
			XYZ_ASSERT(uni, "Material uniform does not exist");
			XYZ_ASSERT(uni->offset + uni->size <= m_Material->m_Shader->GetUniformSize(), "Material uniform buffer out of range");
			memcpy(m_Buffer + uni->offset, (unsigned char*)& val, uni->size);
			m_UpdatedValues.insert(name);
		}

		void Bind();

		static std::shared_ptr<MaterialInstance> Create(const std::shared_ptr<Material>& material);

	private:
		void OnShaderReload();
		void UpdateMaterialValue(const Uniform* uni);

	private:
		std::shared_ptr<Material> m_Material;

		unsigned char* m_Buffer;
		std::unordered_set<std::string> m_UpdatedValues;
	};

}