#ifndef MY_HERMITE_H
#define MY_HERMITE_H

/*

	This is a sample system. It accepts the command "read" followed by the
	path to an OBJ model.

*/


#include "BaseSystem.h"
#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include <GLmodel/GLmodel.h>
#include <vector>

#include "shared/opengl.h"

// a sample system
class HermiteSplineSystem : public BaseSystem
{

public:
	struct vert_pos {
		float x;
		float y;
		float z;
	};
	HermiteSplineSystem(const std::string& name);
	virtual void getState(double* p);
	virtual void setState(double* p);
	void reset(double time);

	void display(GLenum mode = GL_RENDER);

	void readModel(char* fname) { m_model.ReadOBJ(fname); }
	void flipNormals(void) { glmReverseWinding(&m_model); }
	int command(int argc, myCONST_SPEC char** argv);

	void addVert(double* p);
	std::vector<HermiteSplineSystem::vert_pos> getVert();

protected:
	
	float m_sx;
	float m_sy;
	float m_sz;

	Vector m_pos;
	Vector p_pos;
	std::vector<HermiteSplineSystem::vert_pos> vertices;

	GLMmodel m_model;

};
#endif
