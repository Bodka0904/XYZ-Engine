#include "stdafx.h"
#include "Material.h"

namespace XYZ {
	Material::Material(const std::shared_ptr<Shader>& shader)
	{
		m_Shader = shader;
		m_Buffer = new unsigned char[shader->GetUniformSize()];
		m_Shader->AddReloadCallback(std::bind(&Material::OnShaderReload, this));
	}

	Material::~Material()
	{
		delete[] m_Buffer;
	}

	void Material::Bind()
	{
		m_Shader->Bind();
		for (size_t i = 0; i < m_Textures.size(); i++)
		{
			auto& texture = m_Textures[i];
			if (texture)
				texture->Bind(0);

		}
		m_Shader->SetUniforms(m_Buffer);
		m_Shader->UploadRoutines();
	}

	std::shared_ptr<Material> Material::Create(const std::shared_ptr<Shader>& shader)
	{
		return std::make_shared<Material>(shader);
	}

	void Material::OnShaderReload()
	{
		delete[] m_Buffer;
		m_Buffer = new unsigned char[m_Shader->GetUniformSize()];

		for (auto& it : m_MaterialInstances)
			it->OnShaderReload();
	}

	MaterialInstance::MaterialInstance(const std::shared_ptr<Material>& material)
		: m_Material(material)
	{
		m_Buffer = new unsigned char[m_Material->m_Shader->GetUniformSize()];
		material->m_MaterialInstances.insert(this);
	}

	MaterialInstance::~MaterialInstance()
	{
		m_Material->m_MaterialInstances.erase(this);
		delete[] m_Buffer;
	}

	void MaterialInstance::Bind()
	{
		m_Material->m_Shader->SetUniforms(m_Buffer);
	}

	std::shared_ptr<MaterialInstance> MaterialInstance::Create(const std::shared_ptr<Material>& material)
	{
		return std::make_shared<MaterialInstance>(material);
	}

	void MaterialInstance::OnShaderReload()
	{
		delete[] m_Buffer;
		m_Buffer = new unsigned char[m_Material->m_Shader->GetUniformSize()];
	}

	void MaterialInstance::UpdateMaterialValue(const Uniform* uni)
	{
		if (m_UpdatedValues.find(uni->name) != m_UpdatedValues.end())
		{
			memcpy(m_Buffer + uni->offset, m_Material->m_Buffer + uni->offset, uni->size);
		}
	}
}