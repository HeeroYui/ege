/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#include <ege/debug.hpp>
#include <ege/physicsShape/PhysicsConvexHull.hpp>



bool ege::PhysicsConvexHull::parse(const char* _line) {
	if (ege::PhysicsShape::parse(_line) == true) {
		return true;
	}
	if(strncmp(_line, "points:", 6) == 0) {
		//EGE_DEBUG("convex hull point parsing " << _line);
		char* base = (char*)(&_line[6]);
		char* tmp= strchr(base, '|');
		vec3 pos(0,0,0);
		while (tmp != nullptr) {
			*tmp = '\0';
			sscanf(base, "%f %f %f", &pos.m_floats[0], &pos.m_floats[1], &pos.m_floats[2] );
			m_points.push_back(pos);
			base = tmp+1;
			tmp= strchr(base, '|');
		}
		sscanf(base, "%f %f %f", &pos.m_floats[0], &pos.m_floats[1], &pos.m_floats[2] );
		m_points.push_back(pos);
		/*
		for (int32_t iii=0; iii<m_points.size(); iii++) {
			EGE_VERBOSE("    parsed " << m_points[iii]);
		}
		*/
		return true;
	}
	if(strncmp(_line, "scale:", 6) == 0) {
		sscanf(&_line[6], "%f %f %f", &m_scale.m_floats[0], &m_scale.m_floats[1], &m_scale.m_floats[2] );
		EGE_VERBOSE("                scale=" << m_scale);
		return true;
	}
	return false;
}
