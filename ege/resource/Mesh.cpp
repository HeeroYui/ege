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
#define __class__	"resource::Mesh"

ege::resource::Mesh::Mesh() :
  m_normalMode(normalModeNone),
  m_checkNormal(false),
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
	m_GLprogram->uniformMatrix4fv(m_GLMatrix, 1, tmpMatrix.m_mat);
	m_GLprogram->uniformMatrix4fv(m_GLMatrixPosition, 1, _positionMatrix.m_mat);
	// position :
	m_GLprogram->sendAttributePointer(m_GLPosition, m_verticesVBO, MESH_VBO_VERTICES);
	// Texture :
	m_GLprogram->sendAttributePointer(m_GLtexture, m_verticesVBO, MESH_VBO_TEXTURE);
	// position :
	m_GLprogram->sendAttributePointer(m_GLNormal, m_verticesVBO, MESH_VBO_VERTICES_NORMAL);
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
			if (normalModeFace == m_normalMode) {
				for(size_t iii=0; iii<tmppFaces.size() ; ++iii) {
					if(btDot(mattttt * m_listFacesNormal[tmppFaces[iii].m_normal[0]], cameraNormal) >= 0.0f) {
						tmpIndexResult.push_back(iii*3);
						tmpIndexResult.push_back(iii*3+1);
						tmpIndexResult.push_back(iii*3+2);
					}
				}
			} else {
				for(size_t iii=0; iii<tmppFaces.size() ; ++iii) {
					if(    (btDot(mattttt * m_listVertexNormal[tmppFaces[iii].m_normal[0]], cameraNormal) >= -0.2f)
					    || (btDot(mattttt * m_listVertexNormal[tmppFaces[iii].m_normal[1]], cameraNormal) >= -0.2f)
					    || (btDot(mattttt * m_listVertexNormal[tmppFaces[iii].m_normal[2]], cameraNormal) >= -0.2f) ) {
						tmpIndexResult.push_back(iii*3);
						tmpIndexResult.push_back(iii*3+1);
						tmpIndexResult.push_back(iii*3+2);
					}
				}
			}
			ewol::openGL::drawElements(m_materials[m_listFaces.getKey(kkk)]->getRenderModeOpenGl(), tmpIndexResult);
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
	if (m_normalMode != ege::resource::Mesh::normalModeFace) {
		EGE_INFO("calculateNormaleFace(" << _materialName << ")");
		ewol::openGL::renderMode tmpRenderMode = m_materials[_materialName]->getRenderMode();
		if (    tmpRenderMode == ewol::openGL::renderPoint
		     || tmpRenderMode == ewol::openGL::renderLine
		     || tmpRenderMode == ewol::openGL::renderLineStrip
		     || tmpRenderMode == ewol::openGL::renderLineLoop) {
			// can not calculate normal on lines ...
			m_normalMode = ege::resource::Mesh::normalModeNone;
			return;
		}
		for(auto &it : m_listFaces[_materialName].m_faces) {
			// for all case, We use only the 3 vertex for quad element, in theory 3D modeler export element in triangle if it is not a real plane.
			vec3 normal = btCross(m_listVertex[it.m_vertex[0]]-m_listVertex[it.m_vertex[1]],
			                      m_listVertex[it.m_vertex[1]]-m_listVertex[it.m_vertex[2]]);
			m_listFacesNormal.push_back(normal.normalized());
		}
		m_normalMode = ege::resource::Mesh::normalModeFace;
	}
}

void ege::resource::Mesh::calculateNormaleEdge(const std::string& _materialName) {
	m_listVertexNormal.clear();
	if (m_normalMode != ege::resource::Mesh::normalModeVertex) {
		EGE_INFO("calculateNormaleEdge(" << _materialName << ")");
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
		m_normalMode = ege::resource::Mesh::normalModeVertex;
	}
}

// for debugging ...
//#define PRINT_HALF (1)
//#define TRY_MINIMAL_VBO

void ege::resource::Mesh::generateVBO() {
	// calculate the normal of all faces if needed
	if (m_normalMode == ege::resource::Mesh::normalModeNone) {
		// when no normal detected  == > auto generate Face normal ....
		calculateNormaleFace(m_listFaces.getKeys()[0]);
	}
	
	
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
					m_verticesVBO->pushOnBuffer(MESH_VBO_VERTICES_NORMAL, normal);
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

bool ege::resource::Mesh::loadOBJ(const std::string& _fileName) {
	m_normalMode = normalModeNone;
#if 0
	etk::FSNode fileName(_fileName);
	// get the fileSize ...
	int32_t size = fileName.fileSize();
	if (size == 0 ) {
		EGE_ERROR("No data in the file named=\"" << fileName << "\"");
		return false;
	}
	if(false == fileName.fileOpenRead() ) {
		EGE_ERROR("Can not find the file name=\"" << fileName << "\"");
		return false;
	}
	char inputDataLine[2048];
	
	int32_t lineID = 0;
	while (nullptr != fileName.fileGets(inputDataLine, 2048) )
	{
		lineID++;
		if (inputDataLine[0] == 'v') {
			if (inputDataLine[1] == 'n') {
				// Vertice normal   : vn 0.000000 0.000000 -1.000000
				// we did not use normal  == > we recalculated it if needed (some .obj does not export normal, then it is simple like this ...
				// TODO : Use the normal provided ... => can be smooth or not ... (cf check "s 1")
			} else if (inputDataLine[1] == 't') {
				// Texture position : vt 0.748573 0.750412
				vec2 vertex(0,0);
				sscanf(&inputDataLine[3], "%f %f", &vertex.m_floats[0], &vertex.m_floats[1]);
				m_listUV.push_back(vertex);
			} else {
				// Vertice position : v 1.000000 -1.000000 -1.000000
				vec3 vertex(0,0,0);
				sscanf(&inputDataLine[2], "%f %f %f", &vertex.m_floats[0], &vertex.m_floats[1], &vertex.m_floats[2] );
				m_listVertex.push_back(vertex);
			}
		} else if (inputDataLine[0] == 'f') {
			// face : f 5/1/1 1/2/1 4/3/1*
			uint32_t vertexIndex[4], uvIndex[4], normalIndex[4];
			bool quadMode = true;
			int32_t matches = sscanf(&inputDataLine[2], "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
			                         &vertexIndex[0], &uvIndex[0], &normalIndex[0],
			                         &vertexIndex[1], &uvIndex[1], &normalIndex[1],
			                         &vertexIndex[2], &uvIndex[2], &normalIndex[2],
			                         &vertexIndex[3], &uvIndex[3], &normalIndex[3] );
			if (12 != matches){
				// no normal mode :
				matches = sscanf(&inputDataLine[2], "%d/%d %d/%d %d/%d %d/%d\n",
				                 &vertexIndex[0], &uvIndex[0],
				                 &vertexIndex[1], &uvIndex[1],
				                 &vertexIndex[2], &uvIndex[2],
				                 &vertexIndex[3], &uvIndex[3] );
				if (8 != matches){
					quadMode = false;
					matches = sscanf(&inputDataLine[2], "%d/%d/%d %d/%d/%d %d/%d/%d\n",
					                 &vertexIndex[0], &uvIndex[0], &normalIndex[0],
					                 &vertexIndex[1], &uvIndex[1], &normalIndex[1],
					                 &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
					if (9 != matches){
						// no normal mode :
						matches = sscanf(&inputDataLine[2], "%d/%d %d/%d %d/%d\n",
						                 &vertexIndex[0], &uvIndex[0],
						                 &vertexIndex[1], &uvIndex[1],
						                 &vertexIndex[2], &uvIndex[2] );
						if (6 != matches){
							EGE_ERROR("Parsing error in the .obj files : " << fileName << " (l=" << lineID << ") in 'f' section : \"" << &inputDataLine[2] << "\" expected : %d/%d(/%d) %d/%d(/%d) %d/%d(/%d) (%d/%d(/%d)) () for option");
							continue;
						}
					}
				}
			}
			if (true == quadMode) {
				m_listFaces.push_back(Face(vertexIndex[0]-1, uvIndex[0]-1,
				                          vertexIndex[1]-1, uvIndex[1]-1,
				                          vertexIndex[2]-1, uvIndex[2]-1,
				                          vertexIndex[3]-1, uvIndex[3]-1));
			} else {
				m_listFaces.push_back(Face(vertexIndex[0]-1, uvIndex[0]-1,
				                          vertexIndex[1]-1, uvIndex[1]-1,
				                          vertexIndex[2]-1, uvIndex[2]-1));
			}
			/*
			EGE_DEBUG(" plop : " << tmpFace.m_nbElement << " ? " << m_listFaces[m_listFaces.size()-1].m_nbElement);
			EGE_DEBUG("      : " << tmpFace.m_vertex[0] << " ? " << m_listFaces[m_listFaces.size()-1].m_vertex[0]);
			EGE_DEBUG("      : " << tmpFace.m_uv[0] << " ? " << m_listFaces[m_listFaces.size()-1].m_uv[0]);
			*/
		} else if (inputDataLine[0] == 's') {
			// ??? : s off
		} else if (inputDataLine[0] == '#') {
			// comment
			// nothing to do ... just go to the new line ...
		} else if(    inputDataLine[0] == 'u'
		           && inputDataLine[1] == 's'
		           && inputDataLine[2] == 'e'
		           && inputDataLine[3] == 'm'
		           && inputDataLine[4] == 't'
		           && inputDataLine[5] == 'l' ) {
			// Use Material : usemtl imageName.xxx
			while(    inputDataLine[strlen(inputDataLine)-1] == '\n'
			       || inputDataLine[strlen(inputDataLine)-1] == '\r'
			       || inputDataLine[strlen(inputDataLine)-1] == ' ') {
				if (1 == strlen(inputDataLine) ){
					break;
				}
				inputDataLine[strlen(inputDataLine)-1] = '\0';
			}
			std::string tmpVal(&inputDataLine[7]);
			setTexture(fileName.getRelativeFolder() + tmpVal);
		} else if(    inputDataLine[0] == 'm'
		           && inputDataLine[1] == 't'
		           && inputDataLine[2] == 'l'
		           && inputDataLine[3] == 'l'
		           && inputDataLine[4] == 'i'
		           && inputDataLine[5] == 'b' ) {
			// ???? : mtllib cube.mtl
		}
	}
	fileName.fileClose();
	generateVBO();
#endif
	return true;
}

void jumpEndLine(etk::FSNode& _file) {
	char current=_file.fileGet();
	while(    current != '\0'
	       && current != '\n') {
		//printf("%c", current);
		current=_file.fileGet();
	}
}

int32_t countIndent(etk::FSNode& _file) {
	int32_t nbIndent=0;
	int32_t nbSpacesTab=0;
	int32_t nbChar=0;
	//EGE_DEBUG(" start count Indent");
	for(char current=_file.fileGet(); current != '\0'; current=_file.fileGet()) {
		nbChar++;
		//EGE_DEBUG("parse : " << current);
		if (current == '\t') {
			nbSpacesTab = 0;
			nbIndent++;
		} else if (current == ' ') {
			nbSpacesTab++;
			if (nbSpacesTab == 4) {
				nbSpacesTab = 0;
				nbIndent++;
			}
		} else if (current == '#') {
			// Auto remove comment ...
			jumpEndLine(_file);
			return countIndent(_file);
		} else {
			
			break;
		}
	}
	//EGE_DEBUG("indent : " << nbIndent);
	_file.fileSeek(-nbChar, etk::FSN_SEEK_CURRENT);
	return nbIndent;
}

char* loadNextData(char* _elementLine,
                   int64_t _maxData,
                   etk::FSNode& _file,
                   bool _removeTabs=false,
                   bool _stopColomn=false,
                   bool _stopPipe=true) {
	memset(_elementLine, 0, _maxData);
	char * element = _elementLine;
	int64_t outSize = 0;
	/*
	if (m_zipReadingOffset >= m_zipContent->size()) {
		element[0] = '\0';
		return nullptr;
	}
	*/
	char current = _file.fileGet();
	while (current != '\0') {
		if(    _removeTabs == false
		    || element != _elementLine) {
			*element = current;
			element++;
		}
		if(    current == '\n'
		    || current == '\r'
		    || (    current == '|'
		         && _stopPipe == true)
		    || (    current == ':'
		         && _stopColomn == true) )
		{
			*element = '\0';
			//EGE_DEBUG(" plop : '" << _elementLine << "'" );
			return _elementLine;
		} else if(    element == _elementLine
		           && current != '\t') {
			*element = current;
			element++;
		}
		// check maxData size ...
		if (outSize >= _maxData-1) {
			*element = '\0';
			return _elementLine;
		}
		current = _file.fileGet();
	}
	if (outSize == 0) {
		return nullptr;
	} else {
		// send last line
		return _elementLine;
	}
	return nullptr;
}

void removeEndLine(char* _val) {
	int32_t len = strlen(_val);
	if(    len>0
	    && (    _val[len-1] == '\n'
	         || _val[len-1] == '\r' ) ) {
		_val[len-1] = '\0';
	}
	len--;
	if(    len>0
	    && (    _val[len-1] == '\n'
	         || _val[len-1] == '\r') ) {
		_val[len-1] = '\0';
	}
}

enum emfModuleMode {
	EMFModuleNone,
	EMFModuleMesh,
	EMFModuleMeshNamed,
	EMFModuleMeshVertex,
	EMFModuleMeshUVMapping,
	EMFModuleMeshNormalVertex,
	EMFModuleMeshNormalFace,
	EMFModuleMeshFace,
	EMFModuleMeshFaceMaterial,
	EMFModuleMeshPhysics,
	EMFModuleMeshPhysicsNamed,
	EMFModuleMesh_END,
	EMFModuleMaterial,
	EMFModuleMaterialNamed,
	EMFModuleMaterial_END,
};
// TODO : rework with string line extractor
bool ege::resource::Mesh::loadEMF(const std::string& _fileName) {
	m_checkNormal = true;
	m_normalMode = normalModeNone;
	etk::FSNode fileName(_fileName);
	// get the fileSize ...
	int32_t size = fileName.fileSize();
	if (size == 0 ) {
		EGE_ERROR("No data in the file named=\"" << fileName << "\"");
		return false;
	}
	if(false == fileName.fileOpenRead() ) {
		EGE_ERROR("Can not find the file name=\"" << fileName << "\"");
		return false;
	}
	char inputDataLine[2048];
	// load the first line :
	fileName.fileGets(inputDataLine, 2048);
	if(0 == strncmp(inputDataLine, "EMF(STRING)", 11)) {
		// parse in string mode ...
	} else if (0 == strncmp(inputDataLine, "EMF(BINARY)", 11)) {
		EGE_ERROR(" file binary mode is not supported now : 'EMF(BINARY)'");
		return false;
	} else {
		EGE_ERROR(" file mode is not supported now : 'EMF(? ? ?)' = '" << inputDataLine << "'");
		return false;
	}
	enum emfModuleMode currentMode = EMFModuleNone;
	EGE_VERBOSE("Start parsing Mesh file : " << fileName);
	// mesh global param :
	std::string currentMeshName = "";
	int32_t meshFaceMaterialID = -1;
	// material global param :
	std::string materialName = "";
	std::shared_ptr<ege::Material> material;
	// physical shape:
	std::shared_ptr<ege::PhysicsShape> physics;
	while(1) {
		int32_t level = countIndent(fileName);
		if (level == 0) {
			// new section ...
			if (nullptr == loadNextData(inputDataLine, 2048, fileName)) {
				// reach end of file ...
				break;
			}
			if(0 == strncmp(inputDataLine, "Mesh :", 6) ) {
				currentMode = EMFModuleMesh;
				EGE_VERBOSE("Parse Mesh :");
			} else if(0 == strncmp(inputDataLine, "Materials : ", 11) ) {
				currentMode = EMFModuleMaterial;
				EGE_VERBOSE("Parse Material :");
			} else {
				currentMode = EMFModuleNone;
			}
		} else {
			if (currentMode >= EMFModuleMesh && currentMode <= EMFModuleMesh_END) {
				if (level == 1) {
					//Find mesh name ...
					if (nullptr == loadNextData(inputDataLine, 2048, fileName, true)) {
						// reach end of file ...
						break;
					}
					removeEndLine(inputDataLine);
					currentMeshName = inputDataLine;
					currentMode = EMFModuleMeshNamed;
					EGE_VERBOSE("    "<< currentMeshName);
					continue;
				}
				if (level == 2) {
					// In the mesh level 2 the line size must not exced 2048
					if (nullptr == loadNextData(inputDataLine, 2048, fileName, true)) {
						// reach end of file ...
						break;
					}
					removeEndLine(inputDataLine);
					if(0 == strncmp(inputDataLine, "Vertex", 6) ) {
						currentMode = EMFModuleMeshVertex;
						EGE_VERBOSE("        Vertex ...");
					} else if(0 == strncmp(inputDataLine, "UV-mapping", 10) ) {
						currentMode = EMFModuleMeshUVMapping;
						EGE_VERBOSE("        UV-mapping ...");
					} else if(0 == strncmp(inputDataLine, "Normal(vertex)", 14) ) {
						currentMode = EMFModuleMeshNormalVertex;
						EGE_VERBOSE("        Normal(vertex) ...");
					} else if(0 == strncmp(inputDataLine, "Normal(face)", 12) ) {
						currentMode = EMFModuleMeshNormalFace;
						EGE_VERBOSE("        Normal(face) ...");
					} else if(0 == strncmp(inputDataLine, "Face", 4) ) {
						currentMode = EMFModuleMeshFace;
						EGE_VERBOSE("        Face ...");
					} else if(0 == strncmp(inputDataLine, "Physics", 7) ) {
						currentMode = EMFModuleMeshPhysics;
						EGE_VERBOSE("        Physics ...");
					} else {
						EGE_ERROR("        Unknow mesh property '"<<inputDataLine<<"'");
						currentMode = EMFModuleMeshNamed;
					}
					continue;
				}
				// level > 2
				switch (currentMode) {
					default:
						EGE_ERROR("Unknow ... "<< level);
						jumpEndLine(fileName);
						break;
					case EMFModuleMeshVertex: {
						vec3 vertex(0,0,0);
						while (nullptr != loadNextData(inputDataLine, 2048, fileName, true, true) ) {
							if (inputDataLine[0] == '\0') {
								break;
							}
							sscanf(inputDataLine, "%f %f %f", &vertex.m_floats[0], &vertex.m_floats[1], &vertex.m_floats[2] );
							m_listVertex.push_back(vertex);
							int32_t len = strlen(inputDataLine)-1;
							if(    inputDataLine[len] == '\n'
							    || inputDataLine[len] == '\r') {
								break;
							}
						}
						EGE_VERBOSE("            " << m_listVertex.size() << " vertex");
						break;
					}
					case EMFModuleMeshUVMapping: {
						vec2 uvMap(0,0);
						while (nullptr != loadNextData(inputDataLine, 2048, fileName, true, true) ) {
							if (inputDataLine[0] == '\0') {
								break;
							}
							sscanf(inputDataLine, "%f %f", &uvMap.m_floats[0], &uvMap.m_floats[1]);
							m_listUV.push_back(uvMap);
							int32_t len = strlen(inputDataLine)-1;
							if(    inputDataLine[len] == '\n'
							    || inputDataLine[len] == '\r') {
								break;
							}
						}
						EGE_VERBOSE("            " << m_listUV.size() << " coord");
						break;
					}
					case EMFModuleMeshNormalVertex: {
						m_normalMode = normalModeVertex;
						vec3 normal(0,0,0);
						// find the vertex Normal list.
						while (nullptr != loadNextData(inputDataLine, 2048, fileName, true, true) ) {
							if (inputDataLine[0] == '\0') {
								break;
							}
							sscanf(inputDataLine, "%f %f %f", &normal.m_floats[0], &normal.m_floats[1], &normal.m_floats[2] );
							m_listVertexNormal.push_back(normal);
							int32_t len = strlen(inputDataLine)-1;
							if(    inputDataLine[len] == '\n'
							    || inputDataLine[len] == '\r') {
								break;
							}
						}
						EGE_VERBOSE("            " << m_listVertexNormal.size() << " Normals");
						break;
					}
					case EMFModuleMeshNormalFace: {
						m_normalMode = normalModeFace;
						vec3 normal(0,0,0);
						// find the face Normal list.
						while (nullptr != loadNextData(inputDataLine, 2048, fileName, true, true) ) {
							if (inputDataLine[0] == '\0') {
								break;
							}
							sscanf(inputDataLine, "%f %f %f", &normal.m_floats[0], &normal.m_floats[1], &normal.m_floats[2] );
							m_listFacesNormal.push_back(normal);
							int32_t len = strlen(inputDataLine)-1;
							if(    inputDataLine[len] == '\n'
							    || inputDataLine[len] == '\r') {
								break;
							}
						}
						EGE_VERBOSE("            " << m_listFacesNormal.size() << " Normals");
						break;
					}
					case EMFModuleMeshFace:
					case EMFModuleMeshFaceMaterial:
						if (level == 3) {
							//Find mesh name ...
							if (nullptr == loadNextData(inputDataLine, 2048, fileName, true)) {
								// reach end of file ...
								break;
							}
							removeEndLine(inputDataLine);
							// new maretial selection
							currentMode = EMFModuleMeshFaceMaterial;
							FaceIndexing empty;
							m_listFaces.add(inputDataLine, empty);
							meshFaceMaterialID = m_listFaces.getId(inputDataLine);
							EGE_VERBOSE("            " << inputDataLine);
						} else if (currentMode == EMFModuleMeshFaceMaterial) {
							while (nullptr != loadNextData(inputDataLine, 2048, fileName, true, true) ) {
								if (inputDataLine[0] == '\0') {
									// end of line
									break;
								}
								if (meshFaceMaterialID < 0) {
									continue;
								}
								uint32_t vertexIndex[3], uvIndex[3], normalIndex[3];
								vertexIndex[0] = 0;
								vertexIndex[1] = 0;
								vertexIndex[2] = 0;
								uvIndex[0] = 0;
								uvIndex[1] = 0;
								uvIndex[2] = 0;
								normalIndex[0] = 0;
								normalIndex[1] = 0;
								normalIndex[2] = 0;
								sscanf(inputDataLine, "%d/%d/%d %d/%d/%d %d/%d/%d",
								       &vertexIndex[0], &uvIndex[0], &normalIndex[0],
								       &vertexIndex[1], &uvIndex[1], &normalIndex[1],
								       &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
								m_listFaces.getValue(meshFaceMaterialID).m_faces.push_back(Face(vertexIndex[0], uvIndex[0], normalIndex[0],
								                                                               vertexIndex[1], uvIndex[1], normalIndex[1],
								                                                               vertexIndex[2], uvIndex[2], normalIndex[2]));
								/*
								EGE_DEBUG("face :" << vertexIndex[0] << "/" << uvIndex[0] << "/" << normalIndex[0] <<
								           " " << vertexIndex[1] << "/" << uvIndex[1] << "/" << normalIndex[1] <<
								           " " << vertexIndex[2] << "/" << uvIndex[2] << "/" << normalIndex[2]);
								*/
								int32_t len = strlen(inputDataLine)-1;
								if(    inputDataLine[len] == '\n'
								    || inputDataLine[len] == '\r') {
									break;
								}
							}
							EGE_VERBOSE("                " << m_listFaces.getValue(meshFaceMaterialID).m_faces.size() << " faces");
						} else {
							// insert element without material ...
							EGE_ERROR(" try to add face without material selection ...");
							jumpEndLine(fileName);
						}
						break;
					case EMFModuleMeshPhysics:
					case EMFModuleMeshPhysicsNamed:
						if (nullptr == loadNextData(inputDataLine, 2048, fileName, true, false, false)) {
							// reach end of file ...
							break;
						}
						removeEndLine(inputDataLine);
						if (level == 3) {
							physics = ege::PhysicsShape::create(inputDataLine);
							if (physics == nullptr) {
								EGE_ERROR("Allocation error when creating physical shape ...");
								continue;
							}
							m_physics.push_back(physics);
							EGE_VERBOSE("            " << m_physics.size() << " " << inputDataLine);
							currentMode = EMFModuleMeshPhysicsNamed;
						} else if (currentMode == EMFModuleMeshPhysicsNamed) {
							if (physics == nullptr) {
								EGE_ERROR("Can not parse :'" << inputDataLine << "' in physical shape ...");
								continue;
							}
							if (false == physics->parse(inputDataLine)) {
								EGE_ERROR("ERROR when parsing :'" << inputDataLine << "' in physical shape ...");
							}
						}
						break;
				}
				continue;
			} else if (currentMode >= EMFModuleMaterial && currentMode <= EMFModuleMaterial_END) {
				// all material element is stored on 1 line (size < 2048)
				if (nullptr == loadNextData(inputDataLine, 2048, fileName, true)) {
					// reach end of file ...
					break;
				}
				removeEndLine(inputDataLine);
				if (level == 1) {
					// add previous material :
					if(    materialName != ""
					    && material!=nullptr) {
						m_materials.add(materialName, material);
						materialName = "";
						material = nullptr;
					}
					material = std::make_shared<ege::Material>();
					materialName = inputDataLine;
					currentMode = EMFModuleMaterialNamed;
					EGE_VERBOSE("    "<< materialName);
					continue;
				}
				// level >1
				if (currentMode != EMFModuleMaterialNamed) {
					EGE_WARNING("        Unknow element ..."<< level);
					jumpEndLine(fileName);
					continue;
				}
				if (nullptr == material) {
					EGE_ERROR("material allocation error");
					jumpEndLine(fileName);
					continue;
				}
				if(0 == strncmp(inputDataLine,"Ns ",3)) {
					float tmpVal=0;
					sscanf(&inputDataLine[3], "%f", &tmpVal);
					material->setShininess(tmpVal);
					EGE_VERBOSE("        Shininess " << tmpVal);
				} else if(0 == strncmp(inputDataLine,"Ka ",3)) {
					float tmpVal1=0;
					float tmpVal2=0;
					float tmpVal3=0;
					sscanf(&inputDataLine[3], "%f %f %f", &tmpVal1, &tmpVal2, &tmpVal3);
					vec4 tmp(tmpVal1, tmpVal2, tmpVal3, 1);
					material->setAmbientFactor(tmp);
					EGE_VERBOSE("        AmbientFactor " << tmp);
				} else if(0 == strncmp(inputDataLine,"Kd ",3)) {
					float tmpVal1=0;
					float tmpVal2=0;
					float tmpVal3=0;
					sscanf(&inputDataLine[3], "%f %f %f", &tmpVal1, &tmpVal2, &tmpVal3);
					vec4 tmp(tmpVal1, tmpVal2, tmpVal3, 1);
					material->setDiffuseFactor(tmp);
					EGE_VERBOSE("        DiffuseFactor " << tmp);
				} else if(0 == strncmp(inputDataLine,"Ks ",3)) {
					float tmpVal1=0;
					float tmpVal2=0;
					float tmpVal3=0;
					sscanf(&inputDataLine[3], "%f %f %f", &tmpVal1, &tmpVal2, &tmpVal3);
					vec4 tmp(tmpVal1, tmpVal2, tmpVal3, 1);
					material->setSpecularFactor(tmp);
					EGE_VERBOSE("        SpecularFactor " << tmp);
				} else if(0 == strncmp(inputDataLine,"Ni ",3)) {
					float tmpVal=0;
					sscanf(&inputDataLine[3], "%f", &tmpVal);
					// TODO : ...
					EGE_VERBOSE("        Ni " << tmpVal);
				} else if(0 == strncmp(inputDataLine,"d ",2)) {
					float tmpVal=0;
					sscanf(&inputDataLine[2], "%f", &tmpVal);
					// TODO : ...
					EGE_VERBOSE("        d " << tmpVal);
				} else if(0 == strncmp(inputDataLine,"illum ",6)) {
					int tmpVal=0;
					sscanf(&inputDataLine[6], "%d", &tmpVal);
					// TODO : ...
					EGE_VERBOSE("        illum " << tmpVal);
				} else if(0 == strncmp(inputDataLine,"map_Kd ",7)) {
					material->setTexture0(fileName.getRelativeFolder() + &inputDataLine[7]);
					EGE_VERBOSE("        Texture " << &inputDataLine[7]);
				} else if(0 == strncmp(inputDataLine,"renderMode ",11)) {
					ewol::openGL::renderMode mode;
					etk::from_string(mode, &inputDataLine[11]);
					material->setRenderMode(mode);
					EGE_VERBOSE("        Texture " << mode);
				} else {
					EGE_ERROR("unknow material property ... : '" << inputDataLine << "'");
				}
			} else {
				// unknow ...
				EGE_WARNING("Unknow type of line  == > jump end of line ... ");
				jumpEndLine(fileName);
			}
		}
	}
	// add last material ...
	if(    materialName != ""
	    && material!=nullptr) {
		m_materials.add(materialName, material);
		materialName = "";
		material.reset();
	}
	EGE_VERBOSE("Stop parsing Mesh file");
	
	fileName.fileClose();
	generateVBO();
	return true;
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

void ege::resource::Mesh::addLine(const std::string& _layerName, const vec3& _pos1, const vec3& _pos2, const etk::Color<float>& _color) {
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
	int32_t color = findColorInList(_color);
	Face tmpFace;
	tmpFace.setVertex(pos1, pos2);
	tmpFace.setColor(color, color, color);
	m_listFaces[_layerName].m_faces.push_back(tmpFace);
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
	if (    tmpRenderMode != ewol::openGL::renderQuad
	     || tmpRenderMode != ewol::openGL::renderQuadStrip) {
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
