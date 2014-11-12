/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ege/debug.h>
#include <ege/resource/Mesh.h>
#include <etk/os/FSNode.h>



static void jumpEndLine(etk::FSNode& _file) {
	char current=_file.fileGet();
	while(    current != '\0'
	       && current != '\n') {
		//printf("%c", current);
		current=_file.fileGet();
	}
}

static int32_t countIndent(etk::FSNode& _file) {
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

static char* loadNextData(char* _elementLine,
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

static void removeEndLine(char* _val) {
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