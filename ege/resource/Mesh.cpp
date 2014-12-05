/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ege/debug.h>
#include <ege/resource/Mesh.h>
#include <ewol/resource/Manager.h>
#include <etk/os/FSNode.h>
#include <ege/resource/tools/viewBox.h>
#include <ege/resource/tools/isoSphere.h>
#include <ege/resource/tools/icoSphere.h>

#undef __class__
#define __class__ "resource::Mesh"

ege::resource::Mesh::Mesh() :
  m_normalMode(normalModeNone),
  m_checkNormal(false),
  m_GLPosition(-1),
  m_GLMatrix(-1),
  m_GLMatrixPosition(-1),
  m_GLNormal(-1),
  m_GLtexture(-1),
  m_GLColor(-1),
  m_bufferOfset(-1),
  m_numberOfElments(-1),
  m_pointerShape(nullptr),
  m_functionFreeShape(nullptr) {
	addObjectType("ege::resource::Mesh");
}

void ege::resource::Mesh::init(const std::string& _fileName, const std::string& _shaderName) {
	ewol::Resource::init(_fileName);
	addObjectType("ege::resource::Mesh");
	EGE_VERBOSE("Load a new mesh : '" << _fileName << "'");
	// get the shader resource :
	m_GLPosition = 0;
	
	m_light.setDirection(vec3(0.0f,-cos(M_PI/4),0.0f));
	m_light.setHalfPlane(vec3(1.0f,0.0f,0.0f));
	m_light.setAmbientColor(vec4(1.0f,1.0f,1.0f,1.0f));
	m_light.setDiffuseColor(vec4(1.0f,1.0f,1.0f,1.0f));
	m_light.setSpecularColor(vec4(0.0f,0.0f,0.0f,1.0f));
	
	//EGE_DEBUG(m_name << "  " << m_light);
	m_GLprogram = ewol::resource::Program::create(_shaderName);
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
	m_verticesVBO = ewol::resource::VirtualBufferObject::create(5);
	if (m_verticesVBO == nullptr) {
		EGE_ERROR("can not instanciate VBO ...");
		return;
	}
	// TO facilitate some debugs we add a name of the VBO :
	m_verticesVBO->setName("[VBO] of " + _fileName);
	// load the curent file :
	std::string tmpName = etk::tolower(_fileName);
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
	if (m_functionFreeShape!=nullptr) {
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
	if (m_GLprogram == nullptr) {
		EGE_ERROR("No shader ...");
		return;
	}
	//EGE_DEBUG(m_name << "  " << m_light);
	if (_enableDepthTest == true) {
		ewol::openGL::enable(ewol::openGL::FLAG_DEPTH_TEST);
		if (false == _enableDepthUpdate) {
			glDepthMask(GL_FALSE);
		}
	} else {
		ewol::openGL::disable(ewol::openGL::FLAG_DEPTH_TEST);
	}
	//EGE_DEBUG("    display " << m_coord.size() << " elements" );
	m_GLprogram->use();
	// set Matrix : translation/positionMatrix
	mat4 projMatrix = ewol::openGL::getMatrix();
	mat4 camMatrix = ewol::openGL::getCameraMatrix();
	mat4 tmpMatrix = projMatrix * camMatrix;
	m_GLprogram->uniformMatrix(m_GLMatrix, tmpMatrix);
	m_GLprogram->uniformMatrix(m_GLMatrixPosition, _positionMatrix);
	// position :
	m_GLprogram->sendAttributePointer(m_GLPosition, m_verticesVBO, MESH_VBO_VERTICES);
	// Texture :
	m_GLprogram->sendAttributePointer(m_GLtexture, m_verticesVBO, MESH_VBO_TEXTURE);
	// position :
	if (m_normalMode != normalModeNone) {
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
	// position :
	m_GLprogram->sendAttributePointer(m_GLColor, m_verticesVBO, MESH_VBO_COLOR);
	// draw lights :
	m_light.draw(m_GLprogram);
	#ifdef DISPLAY_NB_VERTEX_DISPLAYED
	int32_t nbElementDrawTheoric = 0;
	int32_t nbElementDraw = 0;
	#endif
	for (int32_t kkk=0; kkk<m_listFaces.size(); kkk++) {
		if (false == m_materials.exist(m_listFaces.getKey(kkk))) {
			EGE_WARNING("missing materials : '" << m_listFaces.getKey(kkk) << "'");
			continue;
		}
		m_materials[m_listFaces.getKey(kkk)]->draw(m_GLprogram, m_GLMaterial);
		if (m_checkNormal == false) {
			ewol::openGL::drawElements(m_materials[m_listFaces.getKey(kkk)]->getRenderModeOpenGl(), m_listFaces.getValue(kkk).m_index);
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
			switch(m_normalMode) {
				case normalModeFace:
					for(size_t iii=0; iii<tmppFaces.size() ; ++iii) {
						if(btDot(mattttt * m_listFacesNormal[tmppFaces[iii].m_normal[0]], cameraNormal) >= 0.0f) {
							tmpIndexResult.push_back(iii*3);
							tmpIndexResult.push_back(iii*3+1);
							tmpIndexResult.push_back(iii*3+2);
						}
					}
					break;
				case normalModeVertex:
					for(size_t iii=0; iii<tmppFaces.size() ; ++iii) {
						if(    (btDot(mattttt * m_listVertexNormal[tmppFaces[iii].m_normal[0]], cameraNormal) >= -0.2f)
						    || (btDot(mattttt * m_listVertexNormal[tmppFaces[iii].m_normal[1]], cameraNormal) >= -0.2f)
						    || (btDot(mattttt * m_listVertexNormal[tmppFaces[iii].m_normal[2]], cameraNormal) >= -0.2f) ) {
							tmpIndexResult.push_back(iii*3);
							tmpIndexResult.push_back(iii*3+1);
							tmpIndexResult.push_back(iii*3+2);
						}
					}
					break;
				default:
					for(size_t iii=0; iii<tmppFaces.size() ; ++iii) {
						tmpIndexResult.push_back(iii*3);
						tmpIndexResult.push_back(iii*3+1);
						tmpIndexResult.push_back(iii*3+2);
					}
					break;
			}
			ewol::openGL::drawElements(m_materials[m_listFaces.getKey(kkk)]->getRenderModeOpenGl(), tmpIndexResult);
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
				EGE_DEBUG(((float)nbElementDraw/(float)nbElementDrawTheoric*100.0f) << "% Request draw : " << m_listFaces.size() << ":" << nbElementDraw << "/" << nbElementDrawTheoric << " elements [" << m_name << "]");
			} else {
				EGE_DEBUG("0% Request draw : " << m_listFaces.size() << ":" << nbElementDraw << "/" << nbElementDrawTheoric << " elements [" << m_name << "]");
			}
		}
	#endif
	m_GLprogram->unUse();

	if (_enableDepthTest == true){
		if (false == _enableDepthUpdate) {
			glDepthMask(GL_TRUE);
		}
		ewol::openGL::disable(ewol::openGL::FLAG_DEPTH_TEST);
	}
	// TODO : UNDERSTAND why ... it is needed
	glBindBuffer(GL_ARRAY_BUFFER,0);
}

// normal calculation of the normal face is really easy :
// TODO : Use it for multiple Material interface
void ege::resource::Mesh::calculateNormaleFace(const std::string& _materialName) {
	m_listFacesNormal.clear();
	if (m_normalMode == ege::resource::Mesh::normalModeFace) {
		EGE_INFO("calculateNormaleFace(" << _materialName << ")");
		ewol::openGL::renderMode tmpRenderMode = m_materials[_materialName]->getRenderMode();
		if (    tmpRenderMode == ewol::openGL::renderPoint
		     || tmpRenderMode == ewol::openGL::renderLine
		     || tmpRenderMode == ewol::openGL::renderLineStrip
		     || tmpRenderMode == ewol::openGL::renderLineLoop) {
			EGE_ERROR("calculateNormaleFace(" << _materialName << ") : can not calculate normal on lines ...");
			m_normalMode = ege::resource::Mesh::normalModeNone;
			return;
		}
		for(auto &it : m_listFaces[_materialName].m_faces) {
			// for all case, We use only the 3 vertex for quad element, in theory 3D modeler export element in triangle if it is not a real plane.
			vec3 normal = btCross(m_listVertex[it.m_vertex[0]]-m_listVertex[it.m_vertex[1]],
			                      m_listVertex[it.m_vertex[1]]-m_listVertex[it.m_vertex[2]]);
			m_listFacesNormal.push_back(normal.normalized());
		}
	}
}

void ege::resource::Mesh::calculateNormaleEdge(const std::string& _materialName) {
	m_listVertexNormal.clear();
	if (m_normalMode == ege::resource::Mesh::normalModeVertex) {
		EGE_INFO("calculateNormaleEdge(" << _materialName << ")");
		ewol::openGL::renderMode tmpRenderMode = m_materials[_materialName]->getRenderMode();
		if (    tmpRenderMode == ewol::openGL::renderPoint
		     || tmpRenderMode == ewol::openGL::renderLine
		     || tmpRenderMode == ewol::openGL::renderLineStrip
		     || tmpRenderMode == ewol::openGL::renderLineLoop) {
			EGE_ERROR("calculateNormaleEdge(" << _materialName << ") :  can not calculate normal on lines ...");
			m_normalMode = ege::resource::Mesh::normalModeNone;
			return;
		}
		for(size_t iii=0 ; iii<m_listVertex.size() ; iii++) {
			std::vector<Face>& tmpFaceList = m_listFaces[_materialName].m_faces;
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
				m_listVertexNormal.push_back(vec3(1,1,1));
			} else {
				m_listVertexNormal.push_back(normal.normalized());
			}
		}
	}
}

// for debugging ...
//#define PRINT_HALF (1)
//#define TRY_MINIMAL_VBO

void ege::resource::Mesh::generateVBO() {
	// calculate the normal of all faces if needed
	if (m_normalMode != ege::resource::Mesh::normalModeNone) {
		// when no normal detected  == > auto generate Face normal ....
		calculateNormaleFace(m_listFaces.getKeys()[0]);
	}
	EGE_WARNING("Generate VBO for nb faces : " << m_listFaces.size());
	
	// generate element in 2 pass : 
	//    - create new index dependeng a vertex is a unique componenet of position, texture, normal
	//    - the index list generation (can be dynamic ... (TODO later)
	for (int32_t kkk=0; kkk<m_listFaces.size(); kkk++) {
		// clean faces indexes :
		m_listFaces.getValue(kkk).m_index.clear();
		int32_t nbIndicInFace = 3;
		switch (m_materials[m_listFaces.getKey(kkk)]->getRenderMode()) {
			case ewol::openGL::renderTriangle:
			case ewol::openGL::renderTriangleStrip:
			case ewol::openGL::renderTriangleFan:
				nbIndicInFace = 3;
				break;
			case ewol::openGL::renderLine:
			case ewol::openGL::renderLineStrip:
			case ewol::openGL::renderLineLoop:
				nbIndicInFace = 2;
				break;
			case ewol::openGL::renderPoint:
				nbIndicInFace = 1;
				break;
			case ewol::openGL::renderQuad:
			case ewol::openGL::renderQuadStrip:
				nbIndicInFace = 4;
				break;
			case ewol::openGL::renderPolygon:
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
				// get µNormal
				vec3 normal;
				switch(m_normalMode) {
					case normalModeVertex:
						normal = m_listVertexNormal[tmpFaceList.m_faces[iii].m_normal[indice]];
						break;
					case normalModeFace:
						normal = m_listFacesNormal[tmpFaceList.m_faces[iii].m_normal[indice]];
						break;
					default:
					case normalModeNone:
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
				if (false == elementFind) {
					m_verticesVBO->pushOnBuffer(MESH_VBO_VERTICES, position);
					if (m_normalMode != normalModeNone) {
						m_verticesVBO->pushOnBuffer(MESH_VBO_VERTICES_NORMAL, normal);
					}
					m_verticesVBO->pushOnBuffer(MESH_VBO_TEXTURE, texturepos);
					m_verticesVBO->pushOnBuffer(MESH_VBO_COLOR, color);
					vertexVBOId[indice] = m_verticesVBO->bufferSize(MESH_VBO_VERTICES)-1;
				}
			}
			for(size_t indice=0 ; indice<nbIndicInFace; indice++) {
				tmpFaceList.m_index.push_back(vertexVBOId[indice]);
			}
		}
		#ifdef TRY_MINIMAL_VBO
			EGE_DEBUG("nb cycle ? : " << tmpppppp);
		#endif
	}
	// update all the VBO elements ...
	m_verticesVBO->flush();
}


void ege::resource::Mesh::createViewBox(const std::string& _materialName,float _size) {
	m_normalMode = normalModeNone;
	ege::viewBox::create(m_materials, m_listFaces, m_listVertex, m_listUV,
	                     _materialName, _size);
	calculateNormaleFace(_materialName);
}

void ege::resource::Mesh::createIcoSphere(const std::string& _materialName,float _size, int32_t _subdivision) {
	m_normalMode = normalModeNone;
	ege::icoSphere::create(m_materials, m_listFaces, m_listVertex, m_listUV,
	                       _materialName, _size, _subdivision);
	calculateNormaleFace(_materialName);
}


void ege::resource::Mesh::addMaterial(const std::string& _name, std::shared_ptr<ege::Material> _data) {
	if (nullptr == _data) {
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
	m_listVertex.push_back(_pos);
	return m_listVertex.size()-1;
}
int32_t ege::resource::Mesh::findTextureInList(const vec2& _uv) {
	for (size_t iii=0; iii<m_listUV.size(); ++iii) {
		if (m_listUV[iii] == _uv) {
			return iii;
		}
	}
	m_listUV.push_back(_uv);
	return m_listUV.size()-1;
}
int32_t ege::resource::Mesh::findColorInList(const etk::Color<float>& _color) {
	for (size_t iii=0; iii<m_listColor.size(); ++iii) {
		if (m_listColor[iii] == _color) {
			return iii;
		}
	}
	m_listColor.push_back(_color);
	return m_listColor.size()-1;
}


void ege::resource::Mesh::addFaceIndexing(const std::string& _layerName) {
	if (m_listFaces.exist(_layerName) == false) {
		FaceIndexing empty;
		m_listFaces.add(_layerName, empty);
	}
}

void ege::resource::Mesh::addPoint(const std::string& _layerName, const vec3& _pos, const etk::Color<float>& _color) {
	if (    m_listFaces.exist(_layerName) == false
	     || m_materials.exist(_layerName) == false) {
		EGE_ERROR("Mesh layer : " << _layerName << " does not exist in list faces=" << m_listFaces.exist(_layerName) << " materials=" << m_listFaces.exist(_layerName) << " ...");
		return;
	}
	ewol::openGL::renderMode tmpRenderMode = m_materials[_layerName]->getRenderMode();
	if (tmpRenderMode != ewol::openGL::renderPoint) {
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
	m_listFaces[_layerName].m_faces.push_back(tmpFace);
}

void ege::resource::Mesh::addLine(const std::string& _layerName, const vec3& _pos1, const vec3& _pos2, const etk::Color<float>& _color1, const etk::Color<float>& _color2) {
	if (    m_listFaces.exist(_layerName) == false
	     || m_materials.exist(_layerName) == false) {
		EGE_ERROR("Mesh layer : " << _layerName << " does not exist in list faces=" << m_listFaces.exist(_layerName) << " materials=" << m_listFaces.exist(_layerName) << " ...");
		return;
	}
	ewol::openGL::renderMode tmpRenderMode = m_materials[_layerName]->getRenderMode();
	if (    tmpRenderMode != ewol::openGL::renderLine
	     && tmpRenderMode != ewol::openGL::renderLineStrip
	     && tmpRenderMode != ewol::openGL::renderLineLoop) {
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
	m_listFaces[_layerName].m_faces.push_back(tmpFace);
}

void ege::resource::Mesh::addLines(const std::string& _layerName, const std::vector<vec3>& _list, const etk::Color<float>& _color) {
	for (size_t iii=1; iii<_list.size(); ++iii) {
		addLine(_layerName, _list[iii-1], _list[iii], _color);
	}
}

void ege::resource::Mesh::addLines(const std::string& _layerName, const std::vector<vec3>& _list, const std::vector<etk::Color<float>>& _color) {
	if (_color.size() != _list.size()) {
		EGE_ERROR("Can not add line with changing color without same number of color");
		return;
	}
	for (size_t iii=1; iii<_list.size(); ++iii) {
		addLine(_layerName, _list[iii-1], _list[iii], _color[iii-1], _color[iii]);
	}
}


void ege::resource::Mesh::addTriangle(const std::string& _layerName,
                                      const vec3& _pos1, const vec3& _pos2, const vec3& _pos3,
                                      const vec2& _uv1, const vec2& _uv2, const vec2& _uv3,
                                      const etk::Color<float>& _color1, const etk::Color<float>& _color2, const etk::Color<float>& _color3) {
	if (    m_listFaces.exist(_layerName) == false
	     || m_materials.exist(_layerName) == false) {
		EGE_ERROR("Mesh layer : " << _layerName << " does not exist in list faces=" << m_listFaces.exist(_layerName) << " materials=" << m_listFaces.exist(_layerName) << " ...");
		return;
	}
	ewol::openGL::renderMode tmpRenderMode = m_materials[_layerName]->getRenderMode();
	if (    tmpRenderMode != ewol::openGL::renderTriangle
	     && tmpRenderMode != ewol::openGL::renderTriangleStrip
	     && tmpRenderMode != ewol::openGL::renderTriangleFan) {
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
	m_listFaces[_layerName].m_faces.push_back(tmpFace);
}

void ege::resource::Mesh::addTriangle(const std::string& _layerName, const vec3& _pos1, const vec3& _pos2, const vec3& _pos3,
                                      const etk::Color<float>& _color1, const etk::Color<float>& _color2, const etk::Color<float>& _color3) {
	if (    m_listFaces.exist(_layerName) == false
	     || m_materials.exist(_layerName) == false) {
		EGE_ERROR("Mesh layer : " << _layerName << " does not exist in list faces=" << m_listFaces.exist(_layerName) << " materials=" << m_listFaces.exist(_layerName) << " ...");
		return;
	}
	ewol::openGL::renderMode tmpRenderMode = m_materials[_layerName]->getRenderMode();
	if (    tmpRenderMode == ewol::openGL::renderQuad
	     || tmpRenderMode == ewol::openGL::renderQuadStrip) {
		EGE_TODO("Create quad interface ...");
	} else if (    tmpRenderMode == ewol::openGL::renderTriangle
	            || tmpRenderMode == ewol::openGL::renderLineStrip
	            || tmpRenderMode == ewol::openGL::renderTriangleFan) {
		
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
		m_listFaces[_layerName].m_faces.push_back(tmpFace);
	} else {
		EGE_ERROR("try to add Quad in a mesh material section that not support Quad");
		return;
	}
}
