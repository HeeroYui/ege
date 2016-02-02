/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */
#pragma once

#include <ege/resource/Mesh.h>

namespace ege {
	namespace resource {
		class ParticuleMesh : public ege::resource::Mesh {
			protected:
				int32_t m_GLMainColor;
			protected:
				ParticuleMesh();
				void init(const std::string& _fileName, const std::string& _shaderName="DATA:ParticuleMesh.prog");
			public:
				DECLARE_RESOURCE_NAMED_FACTORY(ParticuleMesh);
				virtual ~ParticuleMesh();
			public:
				virtual void draw(mat4& _positionMatrix,
				                  const etk::Color<float>& _mainColor,
				                  bool _enableDepthTest = true,
				                  bool _enableDepthUpdate = true);
		};
	}
}