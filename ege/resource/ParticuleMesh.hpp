/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <ege/resource/Mesh.hpp>

namespace ege {
	namespace resource {
		class ParticuleMesh : public ege::resource::Mesh {
			protected:
				int32_t m_GLMainColor;
			protected:
				ParticuleMesh();
				void init(const etk::String& _fileName, const etk::String& _shaderName="DATA:ParticuleMesh.prog");
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