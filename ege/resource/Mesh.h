/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EGE_MESH_H__
#define __EGE_MESH_H__

#include <etk/types.h>
#include <etk/Hash.h>
#include <ewol/resource/Resource.h>
#include <ewol/resource/Image.h>
#include <ewol/resource/Shader.h>
#include <ewol/resource/Program.h>
#include <ewol/resource/VirtualBufferObject.h>
#include <ege/Light.h>
#include <ege/Material.h>
#include <ege/resource/tools/Face.h>
#include <ege/resource/tools/FaceIndexing.h>

#include <ege/physicsShape/PhysicsShape.h>
// 3 "float" elements
#define MESH_VBO_VERTICES  (0)
// 2 "float" elements
#define MESH_VBO_TEXTURE   (1)
// 3 "float" elements
#define MESH_VBO_VERTICES_NORMAL    (2)
// Face normal position :
#define MESH_VBO_FACE_NORMAL       (3)
// 4 "float" elements
#define MESH_VBO_COLOR     (4)

namespace ege {
	namespace resource {
		class Mesh : public ewol::Resource {
			public:
				/**
				 * @not-in-doc
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
				std::shared_ptr<ewol::resource::Program> m_GLprogram;
				int32_t m_GLPosition;
				int32_t m_GLMatrix;
				int32_t m_GLMatrixPosition;
				int32_t m_GLNormal;
				int32_t m_GLtexture;
				int32_t m_bufferOfset;
				int32_t m_numberOfElments;
				MaterialGlId   m_GLMaterial;
				ege::Light    m_light;
			protected:
				std::vector<vec3> m_listVertex; //!< List of all vertex in the element
				std::vector<vec2> m_listUV; //!< List of all UV point in the mesh (for the specify texture)
				std::vector<vec3> m_listFacesNormal; //!< List of all Face normal, when calculated
				std::vector<vec3> m_listVertexNormal; //!< List of all Face normal, when calculated
				etk::Hash<FaceIndexing> m_listFaces; //!< List of all Face for the mesh
				etk::Hash<ege::Material*> m_materials;
				std::vector<ege::PhysicsShape*> m_physics; //!< collision shape module ... (independent of bullet lib)
				void clean();
			protected:
				std::shared_ptr<ewol::resource::VirtualBufferObject> m_verticesVBO;
			protected:
				Mesh();
				void init(const std::string& _fileName, const std::string& _shaderName="DATA:textured3D2.prog");
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
				void calculateNormaleFace();
				void calculateNormaleEdge();
			public :
				void createViewBox(const std::string& _materialName,float _size=1.0);
				void createIcoSphere(const std::string& _materialName,float _size=1.0);
			private:
				bool loadOBJ(const std::string& _fileName);
				bool loadEMF(const std::string& _fileName);
			public:
				void addMaterial(const std::string& _name, ege::Material* _data);
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
				const std::vector<ege::PhysicsShape*>& getPhysicalProperties() const {
					return m_physics;
				};
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
		};
	};
};


#endif
