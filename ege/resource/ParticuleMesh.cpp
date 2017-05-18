/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ege/debug.hpp>
#include <ege/resource/ParticuleMesh.hpp>
#include <gale/resource/Manager.hpp>
#include <gale/renderer/openGL/openGL-include.hpp>

ege::resource::ParticuleMesh::ParticuleMesh() {
	addResourceType("ege::resource::ParticuleMesh");
}

void ege::resource::ParticuleMesh::init(const std::string& _fileName, const std::string& _shaderName) {
	ege::resource::Mesh::init(_fileName, _shaderName);
	if (m_GLprogram != nullptr) {
		m_GLMainColor = m_GLprogram->getUniform("EW_mainColor");
	}
}

ege::resource::ParticuleMesh::~ParticuleMesh() {
	
}

void ege::resource::ParticuleMesh::draw(mat4& _positionMatrix,
                                        const etk::Color<float>& _mainColor,
                                        bool _enableDepthTest,
                                        bool _enableDepthUpdate)
{
	if (m_GLprogram == nullptr) {
		EGE_ERROR("No shader ...");
		return;
	}
	//EGE_DEBUG(m_name << "  " << m_light);
	if (_enableDepthTest == true) {
		gale::openGL::enable(gale::openGL::flag_depthTest);
		if (_enableDepthUpdate == false) {
			glDepthMask(GL_FALSE);
		}
	} else {
		gale::openGL::disable(gale::openGL::flag_depthTest);
	}
	//EGE_DEBUG("    display " << m_coord.size() << " elements" );
	m_GLprogram->use();
	// set Matrix : translation/positionMatrix
	mat4 projMatrix = gale::openGL::getMatrix();
	mat4 camMatrix = gale::openGL::getCameraMatrix();
	mat4 tmpMatrix = projMatrix * camMatrix;
	m_GLprogram->uniformMatrix(m_GLMatrix, tmpMatrix);
	m_GLprogram->uniformMatrix(m_GLMatrixPosition, _positionMatrix);
	vec4 tmpColor(_mainColor.r(), _mainColor.g(), _mainColor.b(), _mainColor.a());
	m_GLprogram->uniform4(m_GLMainColor, tmpColor);
	// position :
	m_GLprogram->sendAttributePointer(m_GLPosition, m_verticesVBO, MESH_VBO_VERTICES);
	// Texture :
	m_GLprogram->sendAttributePointer(m_GLtexture, m_verticesVBO, MESH_VBO_TEXTURE);
	// position :
	m_GLprogram->sendAttributePointer(m_GLNormal, m_verticesVBO, MESH_VBO_VERTICES_NORMAL);
	// draw lights :
	m_light.draw(m_GLprogram);
	#ifdef DISPLAY_NB_VERTEX_DISPLAYED
	int32_t nbElementDrawTheoric = 0;
	int32_t nbElementDraw = 0;
	#endif
	for (int32_t kkk=0; kkk<m_listFaces.size(); kkk++) {
		if (m_materials.exist(m_listFaces.getKey(kkk)) == false) {
			EGE_WARNING("missing materials : '" << m_listFaces.getKey(kkk) << "'");
			continue;
		}
		m_materials[m_listFaces.getKey(kkk)]->draw(m_GLprogram, m_GLMaterial);
		if (m_checkNormal == false) {
			gale::openGL::drawElements(gale::openGL::renderMode::triangle, m_listFaces.getValue(kkk).m_index);
			#ifdef DISPLAY_NB_VERTEX_DISPLAYED
				nbElementDraw += m_listFaces.getValue(kkk).m_index.size();
				nbElementDrawTheoric += m_listFaces.getValue(kkk).m_index.size();
			#endif
		} else {
			mat4 mattttt = (projMatrix * camMatrix) * _positionMatrix;
			mattttt.m_mat[3] = 0;
			mattttt.m_mat[7] = 0;
			mattttt.m_mat[11] = 0;
			//vec3 cameraNormal = vec3(-mattttt.m_mat[2], -mattttt.m_mat[6], -mattttt.m_mat[10]);
			vec3 cameraNormal = vec3(0,0,-1);
			cameraNormal.normalized();
			// remove face that is notin the view ...
			std::vector<uint32_t> tmpIndexResult;
			std::vector<ege::Face>& tmppFaces = m_listFaces.getValue(kkk).m_faces;
			//std::vector<uint32_t>& tmppIndex = m_listFaces.getValue(kkk).m_index;
			if (normalModeFace == m_normalMode) {
				for(size_t iii=0; iii<tmppFaces.size() ; ++iii) {
					if((mattttt * m_listFacesNormal[tmppFaces[iii].m_normal[0]]).dot(cameraNormal) >= 0.0f) {
						tmpIndexResult.push_back(iii*3);
						tmpIndexResult.push_back(iii*3+1);
						tmpIndexResult.push_back(iii*3+2);
					}
				}
			} else {
				for(size_t iii=0; iii<tmppFaces.size() ; ++iii) {
					if(    ((mattttt * m_listVertexNormal[tmppFaces[iii].m_normal[0]]).dot(cameraNormal) >= -0.2f)
					    || ((mattttt * m_listVertexNormal[tmppFaces[iii].m_normal[1]]).dot(cameraNormal) >= -0.2f)
					    || ((mattttt * m_listVertexNormal[tmppFaces[iii].m_normal[2]]).dot(cameraNormal) >= -0.2f) ) {
						tmpIndexResult.push_back(iii*3);
						tmpIndexResult.push_back(iii*3+1);
						tmpIndexResult.push_back(iii*3+2);
					}
				}
			}
			gale::openGL::drawElements(gale::openGL::renderMode::triangle, tmpIndexResult);
			#ifdef DISPLAY_NB_VERTEX_DISPLAYED
				nbElementDraw += tmpIndexResult.size();
				nbElementDrawTheoric += m_listFaces.getValue(kkk).m_index.size();
			#endif
		}
	}
	#ifdef DISPLAY_NB_VERTEX_DISPLAYED
		EGE_DEBUG(((float)nbElementDraw/(float)nbElementDrawTheoric*100.0f) << "% Request draw : " << m_listFaces.size() << ":" << nbElementDraw << "/" << nbElementDrawTheoric << " elements [" << m_name << "]");
	#endif
	m_GLprogram->unUse();
	if (_enableDepthTest == true){
		if (_enableDepthUpdate == false) {
			glDepthMask(GL_TRUE);
		}
		gale::openGL::disable(gale::openGL::flag_depthTest);
	}
	glBindBuffer(GL_ARRAY_BUFFER,0);
}
