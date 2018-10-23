/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ege/debug.hpp>
#include <ege/resource/Mesh.hpp>
#include <etk/uri/uri.hpp>



static void jumpEndLine(ememory::SharedPtr<etk::io::Interface>& _file) {
	char current=_file->get();
	while(    current != '\0'
	       && current != '\n') {
		//printf("%c", current);
		current=_file->get();
	}
}

static int32_t countIndent(ememory::SharedPtr<etk::io::Interface>& _file) {
	int32_t nbIndent=0;
	int32_t nbSpacesTab=0;
	int32_t nbChar=0;
	//EGE_DEBUG(" start count Indent");
	for(char current=_file->get(); current != '\0'; current=_file->get()) {
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
	_file->seek(-nbChar, etk::io::SeekMode::Current);
	return nbIndent;
}

static bool loadNextData(etk::String& _elementLine,
                          int64_t _maxData,
                          ememory::SharedPtr<etk::io::Interface>& _file,
                          bool _removeTabs=false,
                          bool _stopColomn=false,
                          bool _stopPipe=true) {
	_elementLine.clear();
	int64_t outSize = 0;
	/*
	if (m_zipReadingOffset >= m_zipContent->size()) {
		element[0] = '\0';
		return null;
	}
	*/
	char current = _file->get();
	while (current != '\0') {
		if(    _removeTabs == false
		    || _elementLine.isEmpty() == false) {
			_elementLine.pushBack(current);
		}
		if(    current == '\n'
		    || current == '\r'
		    || (    current == '|'
		         && _stopPipe == true)
		    || (    current == ':'
		         && _stopColomn == true) )
		{
			//EGE_DEBUG(" plop : '" << _elementLine << "'" );
			return true;
		} else if(    _elementLine.isEmpty() == true
		           && current != '\t') {
			_elementLine.pushBack(current);
		}
		// check maxData size ...
		if (outSize >= _maxData-1) {
			return true;
		}
		current = _file->get();
	}
	if (outSize == 0) {
		return false;
	} else {
		// send last line
		return true;
	}
	return false;
}

static void removeEndLine(etk::String& _value) {
	while (    _value.size() > 0
	        && (    _value.back() == '\n'
	             || _value.back() == '\r') ) {
		_value.popBack();
	}
}


enum emfModuleMode {
	EMFModuleNone,
	EMFModuleMesh,
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
	EMFModuleMaterial_END,
	EMFModulePhysics,
	EMFModulePhysicsNamed,
	EMFModulePhysics_END,
};

// TODO : rework with string line extractor
bool ege::resource::Mesh::loadEMF(const etk::Uri& _fileName) {
	m_checkNormal = true;
	m_normalMode = ege::resource::Mesh::normalMode::none;
	ememory::SharedPtr<etk::io::Interface> fileIO =  etk::uri::get(_fileName);
	// get the fileSize ...
	int32_t size = fileIO->size();
	if (fileIO == null ) {
		EGE_ERROR("CAn not get the file named='" << _fileName << "'");
		return false;
	}
	if (fileIO->open(etk::io::OpenMode::Read) == false) {
		EGE_ERROR("Can not find the file name='" << _fileName << "'");
		return false;
	}
	etk::String inputDataLine;
	// load the first line :
	fileIO->gets(inputDataLine);
	if (inputDataLine.startWith("EMF(STRING)") == true) {
		// parse in string mode ...
	} else if (inputDataLine.startWith("EMF(BINARY)") == true) {
		EGE_ERROR(" file binary mode is not supported now : 'EMF(BINARY)'");
		return false;
	} else {
		EGE_ERROR(" file mode is not supported now : 'EMF(? ? ?)' = '" << inputDataLine << "'");
		return false;
	}
	enum emfModuleMode currentMode = EMFModuleNone;
	EGE_VERBOSE("Start parsing Mesh file : " << _fileName);
	// mesh global param :
	etk::String currentMeshName = "";
	int32_t meshFaceMaterialID = -1;
	// material global param :
	etk::String materialName = "";
	ememory::SharedPtr<ege::Material> material;
	// physical shape:
	ememory::SharedPtr<ege::physics::Shape> physics;
	bool haveUVMapping = false;
	size_t offsetVertexId = 0;
	size_t offsetUV = 0;
	size_t offsetFaceNormal = 0;
	size_t offsetVertexNormal = 0;
	while (1) {
		int32_t level = countIndent(fileIO);
		if (level == 0) {
			// new section ...
			if (loadNextData(inputDataLine, 2048, fileIO) == false) {
				// reach end of file ...
				break;
			}
			if(inputDataLine.startWith("Mesh:") == true) {
				currentMode = EMFModuleMesh;
				removeEndLine(inputDataLine);
				currentMeshName = inputDataLine + 6;
				EGE_VERBOSE("Parse Mesh: " << currentMeshName);
				offsetVertexId = m_listVertex.size();
				offsetUV = m_listUV.size();
				offsetFaceNormal = m_listFacesNormal.size();
				offsetVertexNormal = m_listVertexNormal.size();
				//EGE_ERROR("new offset: " << offsetVertexId << " " << offsetUV << " " << offsetFaceNormal << " " << offsetVertexNormal);
			} else if(inputDataLine.startWith("Materials:") == true) {
				currentMode = EMFModuleMaterial;
				// add previous material:
				if(    materialName != ""
				    && material != null) {
					m_materials.add(materialName, material);
					materialName = "";
					material = null;
				}
				material = ememory::makeShared<ege::Material>();
				removeEndLine(inputDataLine);
				materialName = inputDataLine.extract(10, etk::String::npos);
				EGE_VERBOSE("Parse Material: " << materialName);
			} else if(inputDataLine.startWith("Physics:") == true) {
				currentMode = EMFModulePhysics;
				removeEndLine(inputDataLine);
				EGE_VERBOSE("Parse global Physics: ");
			} else {
				currentMode = EMFModuleNone;
			}
		} else {
			if (    currentMode >= EMFModuleMesh
			     && currentMode <= EMFModuleMesh_END) {
				if (level == 1) {
					// In the mesh level 2 the line size must not exced 2048
					if (loadNextData(inputDataLine, 2048, fileIO, true) == false) {
						// reach end of file ...
						break;
					}
					removeEndLine(inputDataLine);
					if(inputDataLine.startWith("Vertex") == true) {
						currentMode = EMFModuleMeshVertex;
						EGE_VERBOSE("        Vertex ...");
					} else if(inputDataLine.startWith("UV-mapping") == true) {
						currentMode = EMFModuleMeshUVMapping;
						haveUVMapping = true;
						EGE_VERBOSE("        UV-mapping ...");
					} else if(inputDataLine.startWith("Normal(vertex)") == true) {
						currentMode = EMFModuleMeshNormalVertex;
						EGE_VERBOSE("        Normal(vertex) ...");
					} else if(inputDataLine.startWith("Normal(face)") == true) {
						currentMode = EMFModuleMeshNormalFace;
						EGE_VERBOSE("        Normal(face) ...");
					} else if(inputDataLine.startWith("Face") == true) {
						currentMode = EMFModuleMeshFace;
						EGE_VERBOSE("        Face ...");
					} else if(inputDataLine.startWith("Physics") == true) {
						currentMode = EMFModuleMeshPhysics;
						EGE_VERBOSE("        Physics ...");
					} else {
						EGE_ERROR("        Unknow mesh property '" << inputDataLine << "'");
						currentMode = EMFModuleMesh;
					}
					continue;
				}
				// level > 1
				switch (currentMode) {
					default:
						EGE_ERROR("Unknow ... "<< level);
						jumpEndLine(fileIO);
						break;
					case EMFModuleMeshVertex: {
						vec3 vertex(0,0,0);
						while (loadNextData(inputDataLine, 2048, fileIO, true, true) == true) {
							if (inputDataLine[0] == '\0') {
								break;
							}
							sscanf(&inputDataLine[0], "%f %f %f", &vertex.m_floats[0], &vertex.m_floats[1], &vertex.m_floats[2] );
							m_listVertex.pushBack(vertex);
							if(    inputDataLine.back() == '\n'
							    || inputDataLine.back() == '\r') {
								break;
							}
						}
						EGE_VERBOSE("            " << m_listVertex.size() << " vertex");
						break;
					}
					case EMFModuleMeshUVMapping: {
						vec2 uvMap(0,0);
						while (loadNextData(inputDataLine, 2048, fileIO, true, true) == true) {
							if (inputDataLine[0] == '\0') {
								break;
							}
							sscanf(&inputDataLine[0], "%f %f", &uvMap.m_floats[0], &uvMap.m_floats[1]);
							m_listUV.pushBack(uvMap);
							if(    inputDataLine.back() == '\n'
							    || inputDataLine.back() == '\r') {
								break;
							}
						}
						EGE_VERBOSE("            " << m_listUV.size() << " coord");
						break;
					}
					case EMFModuleMeshNormalVertex: {
						m_normalMode = ege::resource::Mesh::normalMode::vertex;
						vec3 normal(0,0,0);
						// find the vertex Normal list.
						while (loadNextData(inputDataLine, 2048, fileIO, true, true) == true) {
							if (inputDataLine[0] == '\0') {
								break;
							}
							sscanf(&inputDataLine[0], "%f %f %f", &normal.m_floats[0], &normal.m_floats[1], &normal.m_floats[2] );
							m_listVertexNormal.pushBack(normal);
							if(    inputDataLine.back() == '\n'
							    || inputDataLine.back() == '\r') {
								break;
							}
						}
						EGE_VERBOSE("            " << m_listVertexNormal.size() << " Normals");
						break;
					}
					case EMFModuleMeshNormalFace: {
						EGE_ERROR("Change mode in face mode ...");
						m_normalMode = ege::resource::Mesh::normalMode::face; // TODO : check if it is the same mode of display the normal  from the start of the file
						vec3 normal(0,0,0);
						// find the face Normal list.
						while (loadNextData(inputDataLine, 2048, fileIO, true, true) == true) {
							if (inputDataLine[0] == '\0') {
								break;
							}
							sscanf(&inputDataLine[0], "%f %f %f", &normal.m_floats[0], &normal.m_floats[1], &normal.m_floats[2] );
							m_listFacesNormal.pushBack(normal);
							if(    inputDataLine.back() == '\n'
							    || inputDataLine.back() == '\r') {
								break;
							}
						}
						EGE_VERBOSE("            " << m_listFacesNormal.size() << " Normals");
						break;
					}
					case EMFModuleMeshFace:
					case EMFModuleMeshFaceMaterial:
						if (level == 2) {
							//Find mesh name ...
							if (loadNextData(inputDataLine, 2048, fileIO, true) == false) {
								// reach end of file ...
								break;
							}
							removeEndLine(inputDataLine);
							// new maretial selection
							currentMode = EMFModuleMeshFaceMaterial;
							FaceIndexing empty;
							if (m_listFaces.exist(inputDataLine) == false) {
								m_listFaces.add(inputDataLine, empty);
							}
							meshFaceMaterialID = m_listFaces.getId(inputDataLine);
							EGE_VERBOSE("            " << inputDataLine);
						} else if (currentMode == EMFModuleMeshFaceMaterial) {
							while (loadNextData(inputDataLine, 2048, fileIO, true, true) == true) {
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
								uvIndex[0] = -1;
								uvIndex[1] = -1;
								uvIndex[2] = -1;
								normalIndex[0] = 0;
								normalIndex[1] = 0;
								normalIndex[2] = 0;
								if (haveUVMapping == true) {
									sscanf(&inputDataLine[0], "%d/%d/%d %d/%d/%d %d/%d/%d",
									       &vertexIndex[0], &uvIndex[0], &normalIndex[0],
									       &vertexIndex[1], &uvIndex[1], &normalIndex[1],
									       &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
									vertexIndex[0] += offsetVertexId;
									vertexIndex[1] += offsetVertexId;
									vertexIndex[2] += offsetVertexId;
									uvIndex[0] += offsetUV;
									uvIndex[1] += offsetUV;
									uvIndex[2] += offsetUV;
									if (m_normalMode == ege::resource::Mesh::normalMode::face) {
										normalIndex[0] += offsetFaceNormal;
										normalIndex[1] += offsetFaceNormal;
										normalIndex[2] += offsetFaceNormal;
									} else {
										normalIndex[0] += offsetVertexNormal;
										normalIndex[1] += offsetVertexNormal;
										normalIndex[2] += offsetVertexNormal;
									}
								} else {
									sscanf(&inputDataLine[0], "%d/%d %d/%d %d/%d",
									       &vertexIndex[0], &normalIndex[0],
									       &vertexIndex[1], &normalIndex[1],
									       &vertexIndex[2], &normalIndex[2] );
									vertexIndex[0] += offsetVertexId;
									vertexIndex[1] += offsetVertexId;
									vertexIndex[2] += offsetVertexId;
									if (m_normalMode == ege::resource::Mesh::normalMode::face) {
										normalIndex[0] += offsetFaceNormal;
										normalIndex[1] += offsetFaceNormal;
										normalIndex[2] += offsetFaceNormal;
									} else {
										normalIndex[0] += offsetVertexNormal;
										normalIndex[1] += offsetVertexNormal;
										normalIndex[2] += offsetVertexNormal;
									}
								}
								m_listFaces.getValue(meshFaceMaterialID).m_faces.pushBack(Face(vertexIndex[0], uvIndex[0], normalIndex[0],
								                                                                vertexIndex[1], uvIndex[1], normalIndex[1],
								                                                                vertexIndex[2], uvIndex[2], normalIndex[2]));
								/*
								EGE_DEBUG("face :" << vertexIndex[0] << "/" << uvIndex[0] << "/" << normalIndex[0] <<
								           " " << vertexIndex[1] << "/" << uvIndex[1] << "/" << normalIndex[1] <<
								           " " << vertexIndex[2] << "/" << uvIndex[2] << "/" << normalIndex[2]);
								*/
								if(    inputDataLine.back() == '\n'
								    || inputDataLine.back() == '\r') {
									break;
								}
							}
							EGE_VERBOSE("                " << m_listFaces.getValue(meshFaceMaterialID).m_faces.size() << " faces");
						} else {
							// insert element without material ...
							EGE_ERROR(" try to add face without material selection ...");
							jumpEndLine(fileIO);
						}
						break;
					case EMFModuleMeshPhysics:
					case EMFModuleMeshPhysicsNamed:
						if (loadNextData(inputDataLine, 2048, fileIO, true, false, false) == false) {
							// reach end of file ...
							break;
						}
						removeEndLine(inputDataLine);
						if (level == 2) {
							physics = ege::physics::Shape::create(inputDataLine);
							if (physics == null) {
								EGE_ERROR("Allocation error when creating physical shape ...");
								continue;
							}
							addPhysicElement(physics);
							EGE_VERBOSE("            " << m_physics.size() << " " << inputDataLine);
							currentMode = EMFModuleMeshPhysicsNamed;
						} else if (currentMode == EMFModuleMeshPhysicsNamed) {
							if (physics == null) {
								EGE_ERROR("Can not parse :'" << inputDataLine << "' in physical shape ...");
								continue;
							}
							if (physics->parse(&inputDataLine[0]) == false) {
								EGE_ERROR("ERROR when parsing :'" << inputDataLine << "' in physical shape ...");
							}
						}
						break;
				}
				continue;
			} else if (    currentMode >= EMFModuleMaterial
			            && currentMode <= EMFModuleMaterial_END) {
				// all material element is stored on 1 line (size < 2048)
				if (loadNextData(inputDataLine, 2048, fileIO, true) == false) {
					// reach end of file ...
					break;
				}
				removeEndLine(inputDataLine);
				if (material == null) {
					EGE_ERROR("material allocation error");
					jumpEndLine(fileIO);
					continue;
				}
				if(inputDataLine.startWith("Ns ") == true) {
					float tmpVal=0;
					sscanf(&inputDataLine[3], "%f", &tmpVal);
					material->setShininess(tmpVal);
					EGE_VERBOSE("        Shininess " << tmpVal);
				} else if(inputDataLine.startWith("Ka ") == true) {
					float tmpVal1=0;
					float tmpVal2=0;
					float tmpVal3=0;
					sscanf(&inputDataLine[3], "%f %f %f", &tmpVal1, &tmpVal2, &tmpVal3);
					vec4 tmp(tmpVal1, tmpVal2, tmpVal3, 1);
					material->setAmbientFactor(tmp);
					EGE_VERBOSE("        AmbientFactor " << tmp);
				} else if(inputDataLine.startWith("Kd ") == true) {
					float tmpVal1=0;
					float tmpVal2=0;
					float tmpVal3=0;
					EGE_ERROR("************************** DiffuseFactor '" << inputDataLine << "'");
					EGE_ERROR("************************** DiffuseFactor '" << &inputDataLine[3] << "'");
					
					sscanf(&inputDataLine[3], "%f %f %f", &tmpVal1, &tmpVal2, &tmpVal3);
					vec4 tmp(tmpVal1, tmpVal2, tmpVal3, 1);
					material->setDiffuseFactor(tmp);
					EGE_ERROR("        DiffuseFactor " << tmp);
				} else if(inputDataLine.startWith("Ks ") == true) {
					float tmpVal1=0;
					float tmpVal2=0;
					float tmpVal3=0;
					sscanf(&inputDataLine[3], "%f %f %f", &tmpVal1, &tmpVal2, &tmpVal3);
					vec4 tmp(tmpVal1, tmpVal2, tmpVal3, 1);
					material->setSpecularFactor(tmp);
					EGE_VERBOSE("        SpecularFactor " << tmp);
				} else if(inputDataLine.startWith("Ni ") == true) {
					float tmpVal=0;
					sscanf(&inputDataLine[3], "%f", &tmpVal);
					// TODO : ...
					EGE_VERBOSE("        Ni " << tmpVal);
				} else if(inputDataLine.startWith("d ") == true) {
					float tmpVal=0;
					sscanf(&inputDataLine[2], "%f", &tmpVal);
					// TODO : ...
					EGE_VERBOSE("        d " << tmpVal);
				} else if(inputDataLine.startWith("illum ") == true) {
					int tmpVal=0;
					sscanf(&inputDataLine[6], "%d", &tmpVal);
					// TODO : ...
					EGE_VERBOSE("        illum " << tmpVal);
				} else if(inputDataLine.startWith("map_Kd ") == true) {
					etk::Uri tmpTexture = _fileName;
					tmpTexture.setPath(_fileName.getPath().getParent() / &inputDataLine[7]);
					material->setTexture0(tmpTexture);
					EGE_VERBOSE("        Texture " << &inputDataLine[7]);
				} else if(inputDataLine.startWith("renderMode ") == true) {
					gale::openGL::renderMode mode;
					etk::from_string(mode, &inputDataLine[11]);
					material->setRenderMode(mode);
					EGE_VERBOSE("        Texture " << mode);
				} else {
					EGE_ERROR("unknow material property ... : '" << inputDataLine << "'");
				}
			} else if (    currentMode >= EMFModulePhysics
			            && currentMode <= EMFModulePhysics_END) {
				if (loadNextData(inputDataLine, 2048, fileIO, true, false, false) == false) {
					// reach end of file ...
					break;
				}
				removeEndLine(inputDataLine);
				if (level == 1) {
					EGE_ERROR("Load shape : " << inputDataLine);
					physics = ege::physics::Shape::create(inputDataLine);
					if (physics == null) {
						EGE_ERROR("Allocation error when creating physical shape ...");
						continue;
					}
					addPhysicElement(physics);
					EGE_VERBOSE("            " << m_physics.size() << " " << inputDataLine);
					currentMode = EMFModulePhysicsNamed;
				} else if (currentMode == EMFModulePhysicsNamed) {
					if (physics == null) {
						EGE_ERROR("Can not parse :'" << inputDataLine << "' in physical shape ...");
						continue;
					}
					if (physics->parse(&inputDataLine[0]) == false) {
						EGE_ERROR("ERROR when parsing :'" << inputDataLine << "' in physical shape ...");
					}
				}
			} else {
				// unknow ...
				EGE_WARNING("Unknow type of line  == > jump end of line ... " << inputDataLine);
				jumpEndLine(fileIO);
			}
		}
	}
	// add last material ...
	if(    materialName != ""
	    && material != null) {
		m_materials.add(materialName, material);
		materialName = "";
		material.reset();
	}
	EGE_VERBOSE("Stop parsing Mesh file");
	
	fileIO->close();
	
	EGE_VERBOSE("New mesh : ");
	EGE_VERBOSE("        nb vertex: " << m_listVertex.size());
	EGE_VERBOSE("        nb UV: " << m_listUV.size());
	EGE_VERBOSE("        nb Colors: " << m_listColor.size());
	EGE_VERBOSE("        nb Normal Face: " << m_listFacesNormal.size());
	EGE_VERBOSE("        nb vertex Face: " << m_listVertexNormal.size());
	EGE_VERBOSE("        nb Faces: " << m_listFaces.size());
	EGE_VERBOSE("        nb material: " << m_materials.size());
	EGE_VERBOSE("        nb physic: " << m_physics.size());
	generateVBO();
	
	return true;
}