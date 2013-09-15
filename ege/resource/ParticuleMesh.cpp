/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ege/debug.h>
#include <ege/resource/ParticuleMesh.h>
#include <ewol/resources/ResourceManager.h>

ege::resource::ParticuleMesh::ParticuleMesh(const etk::UString& _fileName, const etk::UString& _shaderName) : 
	ewol::Mesh(_fileName, _shaderName)
{
	if (m_GLprogram != NULL) {
		m_GLMainColor = m_GLprogram->GetUniform("EW_mainColor");
	}
}

ege::resource::ParticuleMesh::~ParticuleMesh(void)
{
	
}

void ege::resource::ParticuleMesh::Draw(mat4& _positionMatrix,
                                        const etk::Color<float>& _mainColor,
                                        bool _enableDepthTest,
                                        bool _enableDepthUpdate)
{
	if (m_GLprogram==NULL) {
		EWOL_ERROR("No shader ...");
		return;
	}
	//EWOL_DEBUG(m_name << "  " << m_light);
	if (_enableDepthTest==true) {
		ewol::openGL::Enable(ewol::openGL::FLAG_DEPTH_TEST);
		if (false==_enableDepthUpdate) {
			glDepthMask(GL_FALSE);
		}
	} else {
		ewol::openGL::Disable(ewol::openGL::FLAG_DEPTH_TEST);
	}
	//EWOL_DEBUG("    Display " << m_coord.Size() << " elements" );
	m_GLprogram->Use();
	// set Matrix : translation/positionMatrix
	mat4 projMatrix = ewol::openGL::GetMatrix();
	mat4 camMatrix = ewol::openGL::GetCameraMatrix();
	mat4 tmpMatrix = projMatrix * camMatrix;
	m_GLprogram->UniformMatrix4fv(m_GLMatrix, 1, tmpMatrix.m_mat);
	m_GLprogram->UniformMatrix4fv(m_GLMatrixPosition, 1, _positionMatrix.m_mat);
	vec4 tmpColor(_mainColor.r(), _mainColor.g(), _mainColor.b(), _mainColor.a());
	m_GLprogram->Uniform4(m_GLMainColor, tmpColor);
	// position :
	m_GLprogram->SendAttributePointer(m_GLPosition, 3/*x,y,z*/, m_verticesVBO, MESH_VBO_VERTICES);
	// Texture :
	m_GLprogram->SendAttributePointer(m_GLtexture, 2/*u,v*/, m_verticesVBO, MESH_VBO_TEXTURE);
	// position :
	m_GLprogram->SendAttributePointer(m_GLNormal, 3/*x,y,z*/, m_verticesVBO, MESH_VBO_VERTICES_NORMAL);
	// draw lights :
	m_light.Draw(m_GLprogram);
	#ifdef DISPLAY_NB_VERTEX_DISPLAYED
	int32_t nbElementDrawTheoric = 0;
	int32_t nbElementDraw = 0;
	#endif
	for (esize_t kkk=0; kkk<m_listFaces.Size(); kkk++) {
		if (false == m_materials.Exist(m_listFaces.GetKey(kkk))) {
			EWOL_WARNING("missing materials : '" << m_listFaces.GetKey(kkk) << "'");
			continue;
		}
		m_materials[m_listFaces.GetKey(kkk)]->Draw(m_GLprogram, m_GLMaterial);
		if (m_checkNormal==false) {
			ewol::openGL::DrawElements(GL_TRIANGLES, m_listFaces.GetValue(kkk).m_index);
			#ifdef DISPLAY_NB_VERTEX_DISPLAYED
				nbElementDraw += m_listFaces.GetValue(kkk).m_index.Size();
				nbElementDrawTheoric += m_listFaces.GetValue(kkk).m_index.Size();
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
			etk::Vector<ewol::Face>& tmppFaces = m_listFaces.GetValue(kkk).m_faces;
			etk::Vector<uint32_t>& tmppIndex = m_listFaces.GetValue(kkk).m_index;
			if (normalModeFace == m_normalMode) {
				for(int32_t iii=0; iii<tmppFaces.Size() ; ++iii) {
					if(btDot(mattttt * m_listFacesNormal[tmppFaces[iii].m_normal[0]], cameraNormal) >= 0.0f) {
						tmpIndexResult.PushBack(iii*3);
						tmpIndexResult.PushBack(iii*3+1);
						tmpIndexResult.PushBack(iii*3+2);
					}
				}
			} else {
				for(int32_t iii=0; iii<tmppFaces.Size() ; ++iii) {
					if(    (btDot(mattttt * m_listVertexNormal[tmppFaces[iii].m_normal[0]], cameraNormal) >= -0.2f)
					    || (btDot(mattttt * m_listVertexNormal[tmppFaces[iii].m_normal[1]], cameraNormal) >= -0.2f)
					    || (btDot(mattttt * m_listVertexNormal[tmppFaces[iii].m_normal[2]], cameraNormal) >= -0.2f) ) {
						tmpIndexResult.PushBack(iii*3);
						tmpIndexResult.PushBack(iii*3+1);
						tmpIndexResult.PushBack(iii*3+2);
					}
				}
			}
			ewol::openGL::DrawElements(GL_TRIANGLES, tmpIndexResult);
			#ifdef DISPLAY_NB_VERTEX_DISPLAYED
				nbElementDraw += tmpIndexResult.Size();
				nbElementDrawTheoric += m_listFaces.GetValue(kkk).m_index.Size();
			#endif
		}
	}
	#ifdef DISPLAY_NB_VERTEX_DISPLAYED
		EWOL_DEBUG(((float)nbElementDraw/(float)nbElementDrawTheoric*100.0f) << "% Request Draw : " << m_listFaces.Size() << ":" << nbElementDraw << "/" << nbElementDrawTheoric << " elements [" << m_name << "]");
	#endif
	m_GLprogram->UnUse();
	if (_enableDepthTest==true){
		if (false==_enableDepthUpdate) {
			glDepthMask(GL_TRUE);
		}
		ewol::openGL::Disable(ewol::openGL::FLAG_DEPTH_TEST);
	}
	glBindBuffer(GL_ARRAY_BUFFER,0);
}

ege::resource::ParticuleMesh* ege::resource::ParticuleMesh::Keep(const etk::UString& _meshName, const etk::UString& _shaderName)
{
	ege::resource::ParticuleMesh* object = static_cast<ege::resource::ParticuleMesh*>(GetManager().LocalKeep(_meshName));
	if (NULL != object) {
		return object;
	}
	object = new ege::resource::ParticuleMesh(_meshName, _shaderName);
	if (object == NULL) {
		EWOL_ERROR("allocation error of a resource : ??Mesh??" << _meshName);
		return NULL;
	}
	GetManager().LocalAdd(object);
	return object;
}

void ege::resource::ParticuleMesh::Release(ege::resource::ParticuleMesh*& _object)
{
	if (_object == NULL) {
		return;
	}
	ewol::Resource* object2 = static_cast<ewol::Resource*>(_object);
	GetManager().Release(object2);
	_object = NULL;
}
