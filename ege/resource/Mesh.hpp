/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <etk/Hash.hpp>
#include <gale/resource/Resource.hpp>
#include <ewol/resource/Image.hpp>
#include <gale/resource/Shader.hpp>
#include <gale/resource/Program.hpp>
#include <gale/resource/VirtualBufferObject.hpp>
#include <ege/Light.hpp>
#include <ege/Material.hpp>
#include <ege/resource/tools/Face.hpp>
#include <ege/resource/tools/FaceIndexing.hpp>

#include <ege/physicsShape/PhysicsShape.hpp>
// VBO table property:
#define MESH_VBO_VERTICES  (0)
#define MESH_VBO_TEXTURE   (1)
#define MESH_VBO_VERTICES_NORMAL    (2)
#define MESH_VBO_FACE_NORMAL       (3)
#define MESH_VBO_COLOR     (4)

namespace ege {
	namespace resource {
		class Mesh : public gale::Resource {
			public:
				static ememory::SharedPtr<ege::resource::Mesh> createGrid(int32_t _lineCount,
				                                                       const vec3& _position=vec3(0,0,0),
				                                                       float _size=1.0f,
				                                                       const std::string& _materialName="basics");
				static ememory::SharedPtr<ege::resource::Mesh> createCube(float _size=1.0f,
				                                                       const std::string& _materialName="basics",
				                                                       const etk::Color<float>& _color=etk::color::white);
			public:
				/**
				 * @not_in_doc
				 */
				enum normalMode {
					normalModeNone,
					normalModeFace,
					normalModeVertex,
				};
			protected:
				enum normalMode m_normalMode; // select the normal mode of display
				bool m_checkNormal; //!< when enable, this check the normal of the mesh before sending it at the 3d card
			protected:
				ememory::SharedPtr<gale::resource::Program> m_GLprogram;
				int32_t m_GLPosition;
				int32_t m_GLMatrix;
				int32_t m_GLMatrixPosition;
				int32_t m_GLNormal;
				int32_t m_GLtexture;
				int32_t m_GLColor;
				int32_t m_bufferOfset;
				int32_t m_numberOfElments;
				MaterialGlId m_GLMaterial;
				ege::Light m_light;
			protected:
				std::vector<vec3> m_listVertex; //!< List of all vertex in the element
				std::vector<vec2> m_listUV; //!< List of all UV point in the mesh (for the specify texture)
				std::vector<etk::Color<float>> m_listColor; //!< List of all Color point in the mesh
				std::vector<vec3> m_listFacesNormal; //!< List of all Face normal, when calculated
				std::vector<vec3> m_listVertexNormal; //!< List of all Face normal, when calculated
				etk::Hash<FaceIndexing> m_listFaces; //!< List of all Face for the mesh
				etk::Hash<ememory::SharedPtr<ege::Material>> m_materials;
				std::vector<ememory::SharedPtr<ege::PhysicsShape>> m_physics; //!< collision shape module ... (independent of bullet lib)
				void clean();
			protected:
				ememory::SharedPtr<gale::resource::VirtualBufferObject> m_verticesVBO;
			protected:
				Mesh();
				void init(const std::string& _fileName="---", const std::string& _shaderName="DATA:textured3D2.prog");
			public:
				virtual ~Mesh();
				DECLARE_RESOURCE_NAMED_FACTORY(Mesh);
			public:
				virtual void draw(mat4& _positionMatrix, bool _enableDepthTest=true, bool _enableDepthUpdate=true);
				virtual void draw(mat4& _positionMatrix,
				                  const etk::Color<float>& _mainColor,
				                  bool _enableDepthTest = true,
				                  bool _enableDepthUpdate = true) {
					draw(_positionMatrix, _enableDepthTest, _enableDepthUpdate);
				}
				void generateVBO();
			private:
				void calculateNormaleFace(const std::string& _materialName);
				void calculateNormaleEdge(const std::string& _materialName);
			public :
				void createViewBox(const std::string& _materialName,float _size=1.0);
				void createIcoSphere(const std::string& _materialName,float _size=1.0, int32_t _subdivision=3);
			private:
				bool loadOBJ(const std::string& _fileName);
				bool loadEMF(const std::string& _fileName);
			public:
				void addMaterial(const std::string& _name, ememory::SharedPtr<ege::Material> _data);
			public:
				/**
				 * @brief set the check of normal position befor sending it to the openGl card
				 * @param[in] _status New state.
				 */
				void setCheckNormal(bool _status) {
					m_checkNormal=_status;
				};
				/**
				 * @brief get the check value of normal position befor sending it to the openGl card
				 * @return get the chcking stus of normal or not
				 */
				bool getCheckNormal() {
					return m_checkNormal;
				};
				const std::vector<ememory::SharedPtr<ege::PhysicsShape>>& getPhysicalProperties() const {
					return m_physics;
				};
				void addPhysicElement(const ememory::SharedPtr<ege::PhysicsShape>& _shape) {
					m_physics.push_back(_shape);
				}
			private:
				void* m_pointerShape; //!< all mesh have a basic shape (bullet or other) the void pointer mermit to not depent on the bullet lib
			public:
				/**
				 * @brief set the shape pointer (no type  == > user might know it ...)
				 * @param[in] _shape The new shape (this remove the previous one)
				 */
				void setShape(void* _shape);
				/**
				 * @brief get the pointer on the shame (no type)
				 * @return Pointer on shape.
				 */
				void* getShape() {
					return m_pointerShape;
				};
			private:
				void (*m_functionFreeShape)(void* _pointer);
			public:
				void setFreeShapeFunction(void (*_functionFreeShape)(void* _pointer)) {
					m_functionFreeShape = _functionFreeShape;
				};
				/**
				 * @brief Add in the faces list the layer requested
				 * @param[in] _layerName face index to add
				 */
				void addFaceIndexing(const std::string& _layerName);
			public:
				
				void addPoint(const std::string& _layerName, const vec3& _pos, const etk::Color<float>& _color);
				
				void addLine(const std::string& _layerName, const vec3& _pos1, const vec3& _pos2, const etk::Color<float>& _color) {
					addLine( _layerName, _pos1, _pos2, _color, _color);
				}
				void addLine(const std::string& _layerName, const vec3& _pos1, const vec3& _pos2, const etk::Color<float>& _color1, const etk::Color<float>& _color2);
				void addLines(const std::string& _layerName, const std::vector<vec3>& _list, const etk::Color<float>& _color);
				void addLines(const std::string& _layerName, const std::vector<vec3>& _list, const std::vector<etk::Color<float>>& _color);
				
				/**
				 * @not_in_doc
				 * @brief draw a colored triangle (usefull for debug and test)
				 * @param[in] _layerName Material and face indexing layer name
				 * @param[in] _pos1 First point position
				 * @param[in] _pos2 Second point position
				 * @param[in] _pos3 Third point position
				 * @param[in] _color1 color of the _pos1 element
				 * @param[in] _color2 color of the _pos2 element
				 * @param[in] _color3 color of the _pos3 element
				 */
				void addTriangle(const std::string& _layerName, const vec3& _pos1, const vec3& _pos2, const vec3& _pos3, const etk::Color<float>& _color) {
					addTriangle(_layerName, _pos1, _pos2, _pos3, _color, _color, _color);
				}
				/**
				 * @not_in_doc
				 * @brief draw a colored triangle (usefull for debug and test)
				 * @param[in] _layerName Material and face indexing layer name
				 * @param[in] _pos1 First point position
				 * @param[in] _pos2 Second point position
				 * @param[in] _pos3 Third point position
				 * @param[in] _color1 color of the _pos1 element
				 * @param[in] _color2 color of the _pos2 element
				 * @param[in] _color3 color of the _pos3 element
				 */
				void addTriangle(const std::string& _layerName, const vec3& _pos1, const vec3& _pos2, const vec3& _pos3,
				                 const etk::Color<float>& _color1, const etk::Color<float>& _color2, const etk::Color<float>& _color3);
				/**
				 * @not_in_doc
				 * @brief draw a colored quad (usefull for debug and test)
				 * @param[in] _layerName Material and face indexing layer name
				 * @param[in] _pos1 First point position
				 * @param[in] _pos2 Second point position
				 * @param[in] _pos3 Third point position
				 * @param[in] _pos4 faurth point position
				 * @param[in] _color color of all elements
				 */
				void addQuad(const std::string& _layerName, const vec3& _pos1, const vec3& _pos2, const vec3& _pos3, const vec3& _pos4, const etk::Color<float>& _color) {
					addQuad(_layerName, _pos1, _pos2, _pos3, _pos4, _color, _color, _color, _color);
				}
				/**
				 * @not_in_doc
				 * @brief draw a colored quad (usefull for debug and test)
				 * @param[in] _layerName Material and face indexing layer name
				 * @param[in] _pos1 First point position
				 * @param[in] _pos2 Second point position
				 * @param[in] _pos3 Third point position
				 * @param[in] _pos4 faurth point position
				 * @param[in] _color1 color of the _pos1 element
				 * @param[in] _color2 color of the _pos2 element
				 * @param[in] _color3 color of the _pos3 element
				 * @param[in] _color4 color of the _pos4 element
				 */
				void addQuad(const std::string& _layerName, const vec3& _pos1, const vec3& _pos2, const vec3& _pos3, const vec3& _pos4,
				                 const etk::Color<float>& _color1, const etk::Color<float>& _color2, const etk::Color<float>& _color3, const etk::Color<float>& _color4) {
					addTriangle(_layerName, _pos1, _pos2, _pos3, _color1, _color2, _color3);
					addTriangle(_layerName, _pos1, _pos3, _pos4, _color1, _color3, _color4);
				}
				/**
				 * @not_in_doc
				 * @brief draw a textured colored triangle (usefull for debug and test)
				 * @param[in] _layerName Material and face indexing layer name
				 * @param[in] _pos1 First point position
				 * @param[in] _pos2 Second point position
				 * @param[in] _pos3 Third point position
				 * @param[in] _color color of all elements
				 * @param[in] _uv1 texture position of the _pos1 element
				 * @param[in] _uv2 texture position of the _pos2 element
				 * @param[in] _uv3 texture position of the _pos3 element
				 */
				void addTriangle(const std::string& _layerName,
				                 const vec3& _pos1, const vec3& _pos2, const vec3& _pos3,
				                 const vec2& _uv1, const vec2& _uv2, const vec2& _uv3,
				                 const etk::Color<float>& _color) {
					addTriangle(_layerName, _pos1, _pos2, _pos3, _uv1, _uv2, _uv3, _color, _color, _color);
				}
				/**
				 * @not_in_doc
				 * @brief draw a textured colored triangle (usefull for debug and test)
				 * @param[in] _layerName Material and face indexing layer name
				 * @param[in] _pos1 First point position
				 * @param[in] _pos2 Second point position
				 * @param[in] _pos3 Third point position
				 * @param[in] _color1 color of the _pos1 element
				 * @param[in] _color2 color of the _pos2 element
				 * @param[in] _color3 color of the _pos3 element
				 * @param[in] _uv1 texture position of the _pos1 element
				 * @param[in] _uv2 texture position of the _pos2 element
				 * @param[in] _uv3 texture position of the _pos3 element
				 */
				void addTriangle(const std::string& _layerName,
				                 const vec3& _pos1, const vec3& _pos2, const vec3& _pos3,
				                 const vec2& _uv1, const vec2& _uv2, const vec2& _uv3,
				                 const etk::Color<float>& _color1=etk::color::white, const etk::Color<float>& _color2=etk::color::white, const etk::Color<float>& _color3=etk::color::white);
				/**
				 * @not_in_doc
				 * @brief draw a textured colored quad (usefull for debug and test)
				 * @param[in] _layerName Material and face indexing layer name
				 * @param[in] _pos1 First point position
				 * @param[in] _pos2 Second point position
				 * @param[in] _pos3 Third point position
				 * @param[in] _pos4 faurth point position
				 * @param[in] _color color of all elements
				 * @param[in] _uv1 texture position of the _pos1 element
				 * @param[in] _uv2 texture position of the _pos2 element
				 * @param[in] _uv3 texture position of the _pos3 element
				 * @param[in] _uv4 texture position of the _pos4 element
				 */
				void addQuad(const std::string& _layerName,
				             const vec3& _pos1, const vec3& _pos2, const vec3& _pos3, const vec3& _pos4,
				             const vec2& _uv1, const vec2& _uv2, const vec2& _uv3, const vec2& _uv4,
				             const etk::Color<float>& _color) {
					addQuad(_layerName, _pos1, _pos2, _pos3, _pos4, _uv1, _uv2, _uv3, _uv4, _color, _color, _color, _color);
				}
				/**
				 * @not_in_doc
				 * @brief draw a textured quad (usefull for debug and test)
				 * @param[in] _layerName Material and face indexing layer name
				 * @param[in] _pos1 First point position
				 * @param[in] _pos2 Second point position
				 * @param[in] _pos3 Third point position
				 * @param[in] _pos4 faurth point position
				 * @param[in] _uv1 texture position of the _pos1 element
				 * @param[in] _uv2 texture position of the _pos2 element
				 * @param[in] _uv3 texture position of the _pos3 element
				 * @param[in] _uv4 texture position of the _pos4 element
				 * @param[in] _color1 color of the _pos1 element
				 * @param[in] _color2 color of the _pos2 element
				 * @param[in] _color3 color of the _pos3 element
				 * @param[in] _color4 color of the _pos4 element
				 */
				void addQuad(const std::string& _layerName,
				             const vec3& _pos1, const vec3& _pos2, const vec3& _pos3, const vec3& _pos4,
				             const vec2& _uv1, const vec2& _uv2, const vec2& _uv3, const vec2& _uv4,
				             const etk::Color<float>& _color1=etk::color::white, const etk::Color<float>& _color2=etk::color::white, const etk::Color<float>& _color3=etk::color::white, const etk::Color<float>& _color4=etk::color::white) {
					addTriangle(_layerName, _pos1, _pos2, _pos3, _uv1, _uv2, _uv3, _color1, _color2, _color3);
					addTriangle(_layerName, _pos1, _pos3, _pos4, _uv1, _uv3, _uv4, _color1, _color3, _color4);
				}
			protected:
				int32_t findPositionInList(const vec3& _pos);
				int32_t findTextureInList(const vec2& _uv);
				int32_t findColorInList(const etk::Color<float>& _color);
		};
	}
}

