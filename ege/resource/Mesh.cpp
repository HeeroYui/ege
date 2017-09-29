/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ege/debug.hpp>
#include <ege/resource/Mesh.hpp>
#include <gale/resource/Manager.hpp>
#include <gale/renderer/openGL/openGL-include.hpp>
#include <etk/os/FSNode.hpp>
#include <ege/resource/tools/viewBox.hpp>
#include <ege/resource/tools/isoSphere.hpp>
#include <ege/resource/tools/icoSphere.hpp>

ege::resource::Mesh::Mesh() :
  m_normalMode(ege::resource::Mesh::normalMode::none),
  m_checkNormal(false),
  m_GLPosition(-1),
  m_GLMatrix(-1),
  m_GLMatrixPosition(-1),
  m_GLNormal(-1),
  m_GLtexture(-1),
  m_GLColor(-1),
  m_bufferOfset(-1),
  m_numberOfElments(-1),
  m_listFaces(0,false),
  m_materials(0,false),
  m_pointerShape(nullptr),
  m_functionFreeShape(nullptr) {
	addResourceType("ege::resource::Mesh");
}

void ege::resource::Mesh::init(const etk::String& _fileName, const etk::String& _shaderName) {
	gale::Resource::init(_fileName);
	EGE_VERBOSE("Load a new mesh : '" << _fileName << "'");
	// get the shader resource :
	m_GLPosition = 0;
	
	m_light.setDirection(vec3(0.0f,0.0f,10));
	m_light.setHalfPlane(vec3(1.0f,0.0f,0.0f));
	m_light.setAmbientColor(vec4(1.0f,1.0f,1.0f,1.0f)); // Global anbiant color of the scene
	m_light.setDiffuseColor(vec4(1.0f,1.0f,1.0f,1.0f)); // Color of the object
	m_light.setSpecularColor(vec4(0.0f,0.0f,0.0f,1.0f)); // Light color reflection
	
	EGE_VERBOSE(m_name << "  " << m_light << " shader=" << _shaderName);
	m_GLprogram = gale::resource::Program::create(_shaderName);
	if (m_GLprogram != nullptr) {
		m_GLPosition = m_GLprogram->getAttribute("EW_coord3d");
		m_GLtexture = m_GLprogram->getAttribute("EW_texture2d");
		m_GLNormal = m_GLprogram->getAttribute("EW_normal");
		m_GLColor = m_GLprogram->getAttribute("EW_color");
		m_GLMatrix = m_GLprogram->getUniform("EW_MatrixTransformation");
		m_GLMatrixPosition = m_GLprogram->getUniform("EW_MatrixPosition");
		// Link material and Lights
		m_GLMaterial.link(m_GLprogram, "EW_material");
		m_light.link(m_GLprogram, "EW_directionalLight");
	}
	// this is the properties of the buffer requested : "r"/"w" + "-" + buffer type "f"=flaot "i"=integer
	m_verticesVBO = gale::resource::VirtualBufferObject::create(5);
	if (m_verticesVBO == nullptr) {
		EGE_ERROR("can not instanciate VBO ...");
		return;
	}
	// TO facilitate some debugs we add a name of the VBO:
	m_verticesVBO->setName("[VBO] of " + _fileName);
	// load the curent file :
	etk::String tmpName = etk::tolower(_fileName);
	// select the corect loader :
	if (etk::end_with(tmpName, ".obj") == true) {
		if (loadOBJ(_fileName) == false) {
			EGE_ERROR("Error To load OBJ file " << tmpName );
			return;
		}
	} else if (etk::end_with(tmpName, ".emf") ) {
		if (loadEMF(_fileName) == false) {
			EGE_ERROR("Error To load EMF file " << tmpName );
			return;
		}
		//EGE_CRITICAL("Load a new mesh : '" << _fileName << "' (end)");
	} else {
		// nothing to do  == > reqiest an enmpty mesh ==> user manage it ...
	}
}


ege::resource::Mesh::~Mesh() {
	// remove dynamics dependencies :
	if (m_functionFreeShape != nullptr) {
		m_functionFreeShape(m_pointerShape);
		m_pointerShape = nullptr;
	}
}

void ege::resource::Mesh::clean() {
	m_physics.clear();
	m_materials.clear();
	m_listFaces.clear();
	m_listColor.clear();
	m_listVertexNormal.clear();
	m_listFacesNormal.clear();
	m_listUV.clear();
	m_listVertex.clear();
}


//#define DISPLAY_NB_VERTEX_DISPLAYED

void ege::resource::Mesh::draw(mat4& _positionMatrix,
                               bool _enableDepthTest,
                               bool _enableDepthUpdate) {
	// TODO : Remove this, it is just for test the 23-04-2016
	//m_checkNormal = false;
	EGE_VERBOSE("draw Mesh : " << m_name << " (start)");
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
	// position :
	m_GLprogram->sendAttributePointer(m_GLPosition, m_verticesVBO, MESH_VBO_VERTICES);
	// Texture (if needed):
	if (m_listUV.size() != 0) {
		m_GLprogram->sendAttributePointer(m_GLtexture, m_verticesVBO, MESH_VBO_TEXTURE);
	}
	// position :
	if (m_normalMode != ege::resource::Mesh::normalMode::none) {
		m_GLprogram->sendAttributePointer(m_GLNormal, m_verticesVBO, MESH_VBO_VERTICES_NORMAL);
		#if DEBUG
			// TODO : ...
		#endif
	#if DEBUG
		} else {
			// TODO : ...
		}
	#else
		}
	#endif
	// colors :
	m_GLprogram->sendAttributePointer(m_GLColor, m_verticesVBO, MESH_VBO_COLOR);
	// draw lights :
	//EGE_INFO("light : " << m_light);
	m_light.draw(m_GLprogram);
	#ifdef DISPLAY_NB_VERTEX_DISPLAYED
	int32_t nbElementDrawTheoric = 0;
	int32_t nbElementDraw = 0;
	#endif
	for (size_t kkk=0; kkk<m_listFaces.size(); kkk++) {
		if (m_materials.exist(m_listFaces.getKey(kkk)) == false) {
			EGE_WARNING("missing materials : '" << m_listFaces.getKey(kkk) << "'");
			continue;
		}
		m_materials[m_listFaces.getKey(kkk)]->draw(m_GLprogram, m_GLMaterial);
		if (true) { // TODO : understand why the optimisation does not work at all ... : if (m_checkNormal == false) {
			gale::openGL::drawElements(m_materials[m_listFaces.getKey(kkk)]->getRenderModeOpenGl(), m_listFaces.getValue(kkk).m_index);
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
			etk::Vector<uint32_t> tmpIndexResult;
			etk::Vector<ege::Face>& tmppFaces = m_listFaces.getValue(kkk).m_faces;
			//etk::Vector<uint32_t>& tmppIndex = m_listFaces.getValue(kkk).m_index;
			switch(m_normalMode) {
				case ege::resource::Mesh::normalMode::face:
					for(size_t iii=0; iii<tmppFaces.size() ; ++iii) {
						if((mattttt * m_listFacesNormal[tmppFaces[iii].m_normal[0]]).dot(cameraNormal) >= 0.0f) {
							tmpIndexResult.pushBack(iii*3);
							tmpIndexResult.pushBack(iii*3+1);
							tmpIndexResult.pushBack(iii*3+2);
						}
					}
					break;
				case ege::resource::Mesh::normalMode::vertex:
					for(size_t iii=0; iii<tmppFaces.size() ; ++iii) {
						if(    ((mattttt * m_listVertexNormal[tmppFaces[iii].m_normal[0]]).dot(cameraNormal) >= -0.2f)
						    || ((mattttt * m_listVertexNormal[tmppFaces[iii].m_normal[1]]).dot(cameraNormal) >= -0.2f)
						    || ((mattttt * m_listVertexNormal[tmppFaces[iii].m_normal[2]]).dot(cameraNormal) >= -0.2f) ) {
							tmpIndexResult.pushBack(iii*3);
							tmpIndexResult.pushBack(iii*3+1);
							tmpIndexResult.pushBack(iii*3+2);
						}
					}
					break;
				default:
					for(size_t iii=0; iii<tmppFaces.size() ; ++iii) {
						tmpIndexResult.pushBack(iii*3);
						tmpIndexResult.pushBack(iii*3+1);
						tmpIndexResult.pushBack(iii*3+2);
					}
					break;
			}
			gale::openGL::drawElements(m_materials[m_listFaces.getKey(kkk)]->getRenderModeOpenGl(), tmpIndexResult);
			#ifdef DISPLAY_NB_VERTEX_DISPLAYED
				nbElementDraw += tmpIndexResult.size();
				nbElementDrawTheoric += m_listFaces.getValue(kkk).m_index.size();
			#endif
		}
	}
	#ifdef DISPLAY_NB_VERTEX_DISPLAYED
		if (m_listFaces.size() == 0) {
			EGE_ERROR(" !!!! No Face to display elements [" << m_name << "]");
		} else {
			if (nbElementDrawTheoric != 0) {
				EGE_WARNING(((float)nbElementDraw/(float)nbElementDrawTheoric*100.0f) << "% Request draw : " << m_listFaces.size() << ":" << nbElementDraw << "/" << nbElementDrawTheoric << " elements [" << m_name << "]");
			} else {
				EGE_WARNING("0% Request draw : " << m_listFaces.size() << ":" << nbElementDraw << "/" << nbElementDrawTheoric << " elements [" << m_name << "]");
			}
		}
	#endif
	m_GLprogram->unUse();

	if (_enableDepthTest == true){
		if (_enableDepthUpdate == false) {
			glDepthMask(GL_TRUE);
		}
		gale::openGL::disable(gale::openGL::flag_depthTest);
	}
	// TODO : UNDERSTAND why ... it is needed
	glBindBuffer(GL_ARRAY_BUFFER,0);
	EGE_VERBOSE("draw Mesh : " << m_name << " ( end )");
}

void ege::resource::Mesh::drawNormal(mat4& _positionMatrix,
                                     ememory::SharedPtr<ewol::resource::Colored3DObject> _draw) {
	etk::Color<float> tmpColor(0.0, 1.0, 0.0, 1.0);
	etk::Vector<vec3> vertices;
	// generate element in 2 pass : 
	//    - create new index dependeng a vertex is a unique componenet of position, texture, normal
	//    - the index list generation (can be dynamic ... (TODO later)
	for (size_t kkk=0; kkk<m_listFaces.size(); kkk++) {
		// clean faces indexes :
		size_t nbIndicInFace = 3;
		switch (m_materials[m_listFaces.getKey(kkk)]->getRenderMode()) {
			case gale::openGL::renderMode::triangle:
			case gale::openGL::renderMode::triangleStrip:
			case gale::openGL::renderMode::triangleFan:
				nbIndicInFace = 3;
				break;
			case gale::openGL::renderMode::line:
			case gale::openGL::renderMode::lineStrip:
			case gale::openGL::renderMode::lineLoop:
				nbIndicInFace = 2;
				break;
			case gale::openGL::renderMode::point:
				nbIndicInFace = 1;
				break;
			case gale::openGL::renderMode::quad:
			case gale::openGL::renderMode::quadStrip:
				nbIndicInFace = 4;
				break;
			case gale::openGL::renderMode::polygon:
				nbIndicInFace = 3;
				break;
		}
		FaceIndexing& tmpFaceList = m_listFaces.getValue(kkk);
		for (size_t iii=0; iii<tmpFaceList.m_faces.size() ; iii++) {
			switch(m_normalMode) {
				case ege::resource::Mesh::normalMode::vertex:
					{
						// dsplay normal for each vertice ... (TODO: not tested ...)
						for(size_t indice=0 ; indice<nbIndicInFace; indice++) {
							vec3 position = m_listVertex[tmpFaceList.m_faces[iii].m_vertex[indice]];
							vec3 normal = m_listVertexNormal[tmpFaceList.m_faces[iii].m_normal[indice]];
							vertices.pushBack(position);
							vertices.pushBack(position+normal*0.5f);
						}
					} break;
				case ege::resource::Mesh::normalMode::face:
					{
						vec3 center(0,0,0);
						for(size_t indice=0 ; indice<nbIndicInFace; indice++) {
							vec3 position = m_listVertex[tmpFaceList.m_faces[iii].m_vertex[indice]];
							center += position;
						}
						center /= float(nbIndicInFace);
						size_t index = tmpFaceList.m_faces[iii].m_normal[0];
						if (index >= m_listFacesNormal.size()) {
							EGE_ERROR("not enougth normal in the buffer ... " << index << " >= " << m_listFacesNormal.size());
							return;
						}
						vec3 normal = m_listFacesNormal[index];
						vertices.pushBack(center);
						vertices.pushBack(center+normal*0.5f);
					} break;
				case ege::resource::Mesh::normalMode::none:
					break;
			}
		}
	}
	_draw->drawLine(vertices, tmpColor, _positionMatrix);
}

void ege::resource::Mesh::setNormalMode(enum normalMode _mode) {
	m_normalMode = _mode;
}

// normal calculation of the normal face is really easy :
// TODO : Use it for multiple Material interface
void ege::resource::Mesh::calculateNormaleFace(const etk::String& _materialName) {
	m_listFacesNormal.clear();
	if (m_normalMode == ege::resource::Mesh::normalMode::face) {
		EGE_VERBOSE("calculateNormaleFace(" << _materialName << ")");
		gale::openGL::renderMode tmpRenderMode = m_materials[_materialName]->getRenderMode();
		if (    tmpRenderMode == gale::openGL::renderMode::point
		     || tmpRenderMode == gale::openGL::renderMode::line
		     || tmpRenderMode == gale::openGL::renderMode::lineStrip
		     || tmpRenderMode == gale::openGL::renderMode::lineLoop) {
			EGE_ERROR("calculateNormaleFace(" << _materialName << ") : can not calculate normal on lines ...");
			m_normalMode = ege::resource::Mesh::normalMode::none;
			return;
		}
		for(auto &it : m_listFaces[_materialName].m_faces) {
			// for all case, We use only the 3 vertex for quad element, in theory 3D modeler export element in triangle if it is not a real plane.
			vec3 normal = (m_listVertex[it.m_vertex[0]]-m_listVertex[it.m_vertex[1]]).cross(m_listVertex[it.m_vertex[1]]-m_listVertex[it.m_vertex[2]]);
			//EGE_INFO("normal: " << normal.normalized());
			if (normal == vec3(0,0,0)) {
				EGE_ERROR("Null vertor for a face ... " << m_listVertex[it.m_vertex[0]] << " " << m_listVertex[it.m_vertex[1]] << " " << m_listVertex[it.m_vertex[2]]);
				m_listFacesNormal.pushBack(vec3(1,0,0));
			} else {
				m_listFacesNormal.pushBack(normal.normalized());
			}
			int32_t normalID = m_listFacesNormal.size() - 1;
			it.m_normal[0] = normalID;
			it.m_normal[1] = normalID;
			it.m_normal[2] = normalID;
		}
	}
}

void ege::resource::Mesh::calculateNormaleEdge(const etk::String& _materialName) {
	m_listVertexNormal.clear();
	if (m_normalMode == ege::resource::Mesh::normalMode::vertex) {
		EGE_INFO("calculateNormaleEdge(" << _materialName << ")");
		gale::openGL::renderMode tmpRenderMode = m_materials[_materialName]->getRenderMode();
		if (    tmpRenderMode == gale::openGL::renderMode::point
		     || tmpRenderMode == gale::openGL::renderMode::line
		     || tmpRenderMode == gale::openGL::renderMode::lineStrip
		     || tmpRenderMode == gale::openGL::renderMode::lineLoop) {
			EGE_ERROR("calculateNormaleEdge(" << _materialName << ") :  can not calculate normal on lines ...");
			m_normalMode = ege::resource::Mesh::normalMode::none;
			return;
		}
		for(size_t iii=0 ; iii<m_listVertex.size() ; iii++) {
			etk::Vector<Face>& tmpFaceList = m_listFaces[_materialName].m_faces;
			vec3 normal(0,0,0);
			// add the vertex from all the element in the list for face when the element in the face ...
			for(size_t jjj=0 ; jjj<tmpFaceList.size() ; jjj++) {
				m_verticesVBO->pushOnBuffer(MESH_VBO_VERTICES_NORMAL, normal);
				if(    tmpFaceList[jjj].m_vertex[0] == (int32_t)iii
				    || tmpFaceList[jjj].m_vertex[1] == (int32_t)iii
				    || tmpFaceList[jjj].m_vertex[2] == (int32_t)iii) {
					normal += m_listFacesNormal[jjj];
				}
			}
			if (normal == vec3(0,0,0)) {
				m_listVertexNormal.pushBack(vec3(1,1,1));
			} else {
				m_listVertexNormal.pushBack(normal.normalized());
			}
		}
	}
}

// for debugging ...
//#define PRINT_HALF (1)
//#define TRY_MINIMAL_VBO

void ege::resource::Mesh::generateVBO() {
	// calculate the normal of all faces if needed
	if (    m_normalMode != ege::resource::Mesh::normalMode::none
	     && m_listFacesNormal.size() == 0) {
		// when no normal detected  == > auto generate Face normal ....
		EGE_ERROR("Calculate normal face ... in case ????");
		calculateNormaleFace(m_listFaces.getKeys()[0]);
	}
	EGE_WARNING("Generate VBO for nb faces layers: " << m_listFaces.size() << " list layer=" << etk::toString(m_listFaces.getKeys()));
	
	// generate element in 2 pass:
	//    - create new index depending on a vertex is a unique component of position, texture, normal
	//    - the index list generation (can be dynamic ... (TODO later))
	for (size_t kkk=0; kkk<m_listFaces.size(); kkk++) {
		// clean faces indexes :
		m_listFaces.getValue(kkk).m_index.clear();
		int32_t nbIndicInFace = 3;
		if (m_materials[m_listFaces.getKey(kkk)] == nullptr) {
			EGE_ERROR("Can not get material : " << m_listFaces.getKey(kkk) << " pointer value: " << size_t(m_materials[m_listFaces.getKey(kkk)].get()));
			continue;
		}
		switch (m_materials[m_listFaces.getKey(kkk)]->getRenderMode()) {
			case gale::openGL::renderMode::triangle:
			case gale::openGL::renderMode::triangleStrip:
			case gale::openGL::renderMode::triangleFan:
				nbIndicInFace = 3;
				break;
			case gale::openGL::renderMode::line:
			case gale::openGL::renderMode::lineStrip:
			case gale::openGL::renderMode::lineLoop:
				nbIndicInFace = 2;
				break;
			case gale::openGL::renderMode::point:
				nbIndicInFace = 1;
				break;
			case gale::openGL::renderMode::quad:
			case gale::openGL::renderMode::quadStrip:
				nbIndicInFace = 4;
				break;
			case gale::openGL::renderMode::polygon:
				nbIndicInFace = 3;
				break;
		}
		#ifdef TRY_MINIMAL_VBO
			int64_t tmpppppp=0;
		#endif
		FaceIndexing& tmpFaceList = m_listFaces.getValue(kkk);
		for (size_t iii=0; iii<tmpFaceList.m_faces.size() ; iii++) {
			int32_t vertexVBOId[3];
			for(size_t indice=0 ; indice<nbIndicInFace; indice++) {
				// ghet position
				vec3 position = m_listVertex[tmpFaceList.m_faces[iii].m_vertex[indice]];
				// get Color
				etk::Color<float> color;
				if (tmpFaceList.m_faces[iii].m_color[indice] != -1) {
					color = m_listColor[tmpFaceList.m_faces[iii].m_color[indice]];
				} else {
					color = etk::color::white;
				}
				// get Normal
				vec3 normal;
				switch(m_normalMode) {
					case ege::resource::Mesh::normalMode::vertex:
						normal = m_listVertexNormal[tmpFaceList.m_faces[iii].m_normal[indice]];
						break;
					case ege::resource::Mesh::normalMode::face:
						normal = m_listFacesNormal[tmpFaceList.m_faces[iii].m_normal[indice]];
						break;
					default:
					case ege::resource::Mesh::normalMode::none:
						break;
				}
				// get Texture Position
				vec2 texturepos;
				if (tmpFaceList.m_faces[iii].m_uv[indice] == -1) {
					texturepos.setValue(0,0);
				} else {
					texturepos.setValue(m_listUV[tmpFaceList.m_faces[iii].m_uv[indice]].x(),1.0f-m_listUV[tmpFaceList.m_faces[iii].m_uv[indice]].y());
				}
				// Create the vectex Buffer list:
				bool elementFind = false;
				#ifdef TRY_MINIMAL_VBO
				for (int32_t jjj=0; jjj<m_verticesVBO->sizeOnBufferVec3(MESH_VBO_VERTICES); jjj++) {
					if(    m_verticesVBO->getOnBufferVec3(MESH_VBO_VERTICES,jjj) == position
					    && m_verticesVBO->getOnBufferVec3(MESH_VBO_VERTICES_NORMAL,jjj) == normal
					    && m_verticesVBO->getOnBufferVec2(MESH_VBO_TEXTURE,jjj) == texturepos) {
						vertexVBOId[indice] = jjj;
						elementFind = true;
						//EGE_DEBUG("search indice : " << jjj);
						tmpppppp += jjj;
						// stop searching ...
						break;
					}
				}
				#endif
				if (elementFind == false) {
					m_verticesVBO->pushOnBuffer(MESH_VBO_VERTICES, position);
					if (m_normalMode != ege::resource::Mesh::normalMode::none) {
						m_verticesVBO->pushOnBuffer(MESH_VBO_VERTICES_NORMAL, normal);
					}
					m_verticesVBO->pushOnBuffer(MESH_VBO_TEXTURE, texturepos);
					m_verticesVBO->pushOnBuffer(MESH_VBO_COLOR, color);
					vertexVBOId[indice] = m_verticesVBO->bufferSize(MESH_VBO_VERTICES)-1;
				}
			}
			for(size_t indice=0 ; indice<nbIndicInFace; indice++) {
				tmpFaceList.m_index.pushBack(vertexVBOId[indice]);
			}
		}
		#ifdef TRY_MINIMAL_VBO
			EGE_DEBUG("nb cycle ? : " << tmpppppp);
		#endif
	}
	// update all the VBO elements ...
	m_verticesVBO->flush();
}


void ege::resource::Mesh::createViewBox(const etk::String& _materialName,float _size) {
	m_normalMode = ege::resource::Mesh::normalMode::none;
	ege::viewBox::create(m_materials, m_listFaces, m_listVertex, m_listUV,
	                     _materialName, _size);
	calculateNormaleFace(_materialName);
}

void ege::resource::Mesh::createIcoSphere(const etk::String& _materialName,float _size, int32_t _subdivision) {
	m_normalMode = ege::resource::Mesh::normalMode::none;
	ege::icoSphere::create(m_materials, m_listFaces, m_listVertex, m_listUV,
	                       _materialName, _size, _subdivision);
	calculateNormaleFace(_materialName);
}


void ege::resource::Mesh::addMaterial(const etk::String& _name, ememory::SharedPtr<ege::Material> _data) {
	if (_data == nullptr) {
		EGE_ERROR(" can not add material with null pointer");
		return;
	}
	if (_name == "") {
		EGE_ERROR(" can not add material with no name");
		return;
	}
	// really add the material :
	m_materials.add(_name, _data);
}

void ege::resource::Mesh::setShape(void* _shape) {
	if (m_functionFreeShape!=nullptr) {
		m_functionFreeShape(m_pointerShape);
		m_pointerShape = nullptr;
	}
	m_pointerShape=_shape;
}

int32_t ege::resource::Mesh::findPositionInList(const vec3& _pos) {
	for (size_t iii=0; iii<m_listVertex.size(); ++iii) {
		if (m_listVertex[iii] == _pos) {
			return iii;
		}
	}
	m_listVertex.pushBack(_pos);
	return m_listVertex.size()-1;
}
int32_t ege::resource::Mesh::findTextureInList(const vec2& _uv) {
	for (size_t iii=0; iii<m_listUV.size(); ++iii) {
		if (m_listUV[iii] == _uv) {
			return iii;
		}
	}
	m_listUV.pushBack(_uv);
	return m_listUV.size()-1;
}
int32_t ege::resource::Mesh::findColorInList(const etk::Color<float>& _color) {
	for (size_t iii=0; iii<m_listColor.size(); ++iii) {
		if (m_listColor[iii] == _color) {
			return iii;
		}
	}
	m_listColor.pushBack(_color);
	return m_listColor.size()-1;
}


void ege::resource::Mesh::addFaceIndexing(const etk::String& _layerName) {
	if (m_listFaces.exist(_layerName) == false) {
		FaceIndexing empty;
		m_listFaces.add(_layerName, empty);
	}
}

void ege::resource::Mesh::addPoint(const etk::String& _layerName, const vec3& _pos, const etk::Color<float>& _color) {
	if (    m_listFaces.exist(_layerName) == false
	     || m_materials.exist(_layerName) == false) {
		EGE_ERROR("Mesh layer : " << _layerName << " does not exist in list faces=" << m_listFaces.exist(_layerName) << " materials=" << m_listFaces.exist(_layerName) << " ...");
		return;
	}
	gale::openGL::renderMode tmpRenderMode = m_materials[_layerName]->getRenderMode();
	if (tmpRenderMode != gale::openGL::renderMode::point) {
		EGE_ERROR("try to add Point in a mesh material section that not support Point");
		return;
	}
	// try to find position:
	int32_t pos = findPositionInList(_pos);
	// try to find UV mapping:
	int32_t color = findColorInList(_color);
	Face tmpFace;
	tmpFace.setVertex(pos);
	tmpFace.setColor(color, color, color);
	m_listFaces[_layerName].m_faces.pushBack(tmpFace);
}

void ege::resource::Mesh::addLine(const etk::String& _layerName, const vec3& _pos1, const vec3& _pos2, const etk::Color<float>& _color1, const etk::Color<float>& _color2) {
	if (    m_listFaces.exist(_layerName) == false
	     || m_materials.exist(_layerName) == false) {
		EGE_ERROR("Mesh layer : " << _layerName << " does not exist in list faces=" << m_listFaces.exist(_layerName) << " materials=" << m_listFaces.exist(_layerName) << " ...");
		return;
	}
	gale::openGL::renderMode tmpRenderMode = m_materials[_layerName]->getRenderMode();
	if (    tmpRenderMode != gale::openGL::renderMode::line
	     && tmpRenderMode != gale::openGL::renderMode::lineStrip
	     && tmpRenderMode != gale::openGL::renderMode::lineLoop) {
		EGE_ERROR("try to add Line in a mesh material section that not support Line");
		return;
	}
	// try to find position:
	int32_t pos1 = findPositionInList(_pos1);
	int32_t pos2 = findPositionInList(_pos2);
	// try to find UV mapping:
	int32_t color1 = findColorInList(_color1);
	int32_t color2 = findColorInList(_color2);
	Face tmpFace;
	tmpFace.setVertex(pos1, pos2);
	tmpFace.setColor(color1, color2, color2);
	m_listFaces[_layerName].m_faces.pushBack(tmpFace);
}

void ege::resource::Mesh::addLines(const etk::String& _layerName, const etk::Vector<vec3>& _list, const etk::Color<float>& _color) {
	for (size_t iii=1; iii<_list.size(); ++iii) {
		addLine(_layerName, _list[iii-1], _list[iii], _color);
	}
}

void ege::resource::Mesh::addLines(const etk::String& _layerName, const etk::Vector<vec3>& _list, const etk::Vector<etk::Color<float>>& _color) {
	if (_color.size() != _list.size()) {
		EGE_ERROR("Can not add line with changing color without same number of color");
		return;
	}
	for (size_t iii=1; iii<_list.size(); ++iii) {
		addLine(_layerName, _list[iii-1], _list[iii], _color[iii-1], _color[iii]);
	}
}


void ege::resource::Mesh::addTriangle(const etk::String& _layerName,
                                      const vec3& _pos1, const vec3& _pos2, const vec3& _pos3,
                                      const vec2& _uv1, const vec2& _uv2, const vec2& _uv3,
                                      const etk::Color<float>& _color1, const etk::Color<float>& _color2, const etk::Color<float>& _color3) {
	EGE_INFO("add Triangle: " << _layerName << " ...");
	if (    m_listFaces.exist(_layerName) == false
	     || m_materials.exist(_layerName) == false) {
		EGE_ERROR("Mesh layer : " << _layerName << " does not exist in list faces=" << m_listFaces.exist(_layerName) << " materials=" << m_listFaces.exist(_layerName) << " ...");
		return;
	}
	gale::openGL::renderMode tmpRenderMode = m_materials[_layerName]->getRenderMode();
	if (    tmpRenderMode != gale::openGL::renderMode::triangle
	     && tmpRenderMode != gale::openGL::renderMode::triangleStrip
	     && tmpRenderMode != gale::openGL::renderMode::triangleFan) {
		EGE_ERROR("try to add Line in a mesh material section that not support Line");
		return;
	}
	// try to find position:
	int32_t pos1 = findPositionInList(_pos1);
	int32_t pos2 = findPositionInList(_pos2);
	int32_t pos3 = findPositionInList(_pos3);
	// try to find UV mapping:
	int32_t uv1 = findTextureInList(_uv1);
	int32_t uv2 = findTextureInList(_uv2);
	int32_t uv3 = findTextureInList(_uv3);
	// try to find Color:
	int32_t color1 = findColorInList(_color1);
	int32_t color2 = findColorInList(_color2);
	int32_t color3 = findColorInList(_color3);
	Face tmpFace(pos1, uv1,
	             pos2, uv2,
	             pos3, uv3);
	tmpFace.setColor(color1, color2, color3);
	m_listFaces[_layerName].m_faces.pushBack(tmpFace);
	EGE_INFO("    nbFace: " << m_listFaces[_layerName].m_faces.size());
}

void ege::resource::Mesh::addTriangle(const etk::String& _layerName, const vec3& _pos1, const vec3& _pos2, const vec3& _pos3,
                                      const etk::Color<float>& _color1, const etk::Color<float>& _color2, const etk::Color<float>& _color3) {
	if (    m_listFaces.exist(_layerName) == false
	     || m_materials.exist(_layerName) == false) {
		EGE_ERROR("Mesh layer : " << _layerName << " does not exist in list faces=" << m_listFaces.exist(_layerName) << " materials=" << m_listFaces.exist(_layerName) << " ...");
		return;
	}
	gale::openGL::renderMode tmpRenderMode = m_materials[_layerName]->getRenderMode();
	if (    tmpRenderMode == gale::openGL::renderMode::quad
	     || tmpRenderMode == gale::openGL::renderMode::quadStrip) {
		EGE_TODO("Create quad interface ...");
	} else if (    tmpRenderMode == gale::openGL::renderMode::triangle
	            || tmpRenderMode == gale::openGL::renderMode::lineStrip
	            || tmpRenderMode == gale::openGL::renderMode::triangleFan) {
		
		// try to find position:
		int32_t pos1 = findPositionInList(_pos1);
		int32_t pos2 = findPositionInList(_pos2);
		int32_t pos3 = findPositionInList(_pos3);
		// try to find Color:
		int32_t color1 = findColorInList(_color1);
		int32_t color2 = findColorInList(_color2);
		int32_t color3 = findColorInList(_color3);
		Face tmpFace(pos1, -1,
		             pos2, -1,
		             pos3, -1);
		tmpFace.setColor(color1, color2, color3);
		m_listFaces[_layerName].m_faces.pushBack(tmpFace);
	} else {
		EGE_ERROR("try to add Quad in a mesh material section that not support Quad");
		return;
	}
}

#include <ege/physics/shape/Concave.hpp>
const etk::Vector<ememory::SharedPtr<ege::physics::Shape>>& ege::resource::Mesh::getPhysicalProperties() {
	for (auto &it: m_physics) {
		if (it->getType() == ege::physics::Shape::type::concave) {
			// need to generate the internal list of point and triangle needed:
			ege::physics::shape::Concave* tmpElement = it->toConcave();
			if (tmpElement == nullptr) {
				EGE_ERROR("    Concave ==> can not cast in Concave");
				return m_physics;
			}
			tmpElement->clear();
			//EGE_INFO(" add vertices : " << m_listVertex);
			tmpElement->setListOfVertex(m_listVertex);
			for (size_t kkk=0; kkk<m_listFaces.size(); ++kkk) {
				etk::Vector<uint32_t> index;
				for (auto &it : m_listFaces.getValue(kkk).m_faces) {
					index.pushBack(it.m_vertex[0]);
					index.pushBack(it.m_vertex[1]);
					index.pushBack(it.m_vertex[2]);
				}
				//EGE_INFO(" add triangle : " << m_listFaces.getValue(kkk).m_index);
				
				//tmpElement->addTriangle(m_listFaces.getValue(kkk).m_index);
				tmpElement->addTriangle(index);
			}
			//EGE_CRITICAL("kjlkj");
			// Can have only one concave element in a mesh ...
			//return m_physics;
		}
	}
	return m_physics;
}

