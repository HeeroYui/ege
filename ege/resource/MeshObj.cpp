/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ege/debug.hpp>
#include <ege/resource/Mesh.hpp>


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
	if (fileName.fileOpenRead() == false) {
		EGE_ERROR("Can not find the file name=\"" << fileName << "\"");
		return false;
	}
	char inputDataLine[2048];
	
	int32_t lineID = 0;
	while (fileName.fileGets(inputDataLine, 2048) != nullptr)
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
			if (quadMode == true) {
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
