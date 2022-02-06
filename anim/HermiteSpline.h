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
#include <fstream>
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

	float f1(float t);
	float f2(float t);
	float f3(float t);
	float f4(float t);

	float getArcLength(float t);
	void calculateArcLengthTable();

	double* findTangentStartPoint(double* y0, double* y1, double* y2);
	double* findTangentEndPoint(double* y0, double* y1, double* y2);
	double* findTangentMidPoint(double* y0, double* y1);

	Vector m_pos;
	Vector p_pos;
	std::vector<HermiteSplineSystem::vert_pos> vertices;
	std::vector<float> arc_length_table;

	GLMmodel m_model;

	bool cat_rom;
	
};
#endif
