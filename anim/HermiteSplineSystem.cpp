#include "HermiteSpline.h"
#include <iostream>

HermiteSplineSystem::HermiteSplineSystem(const std::string& name) :
	BaseSystem(name),
	m_sx(1.0f),
	m_sy(1.0f),
	m_sz(1.0f)
{

	setVector(m_pos, 0, 0, 0);
	setVector(p_pos, 0, 0, 0);
	//setVector(vert_pos, 0, 0, 0);
	vertices = std::vector<vert_pos>();

}	// HermiteSplineSystem

void HermiteSplineSystem::getState(double* p)
{
	VecCopy(p, m_pos);
	std::vector<HermiteSplineSystem::vert_pos> verts = getVert();
	for (HermiteSplineSystem::vert_pos i : verts) {
		animTcl::OutputMessage("New Vertex");
		
		std::string s = std::to_string(i.x);

		char* char_type = new char[s.length()];
		strcpy(char_type, s.c_str());
		animTcl::OutputMessage(char_type);

		s = std::to_string(i.y);

		char_type = new char[s.length()];
		strcpy(char_type, s.c_str());
		animTcl::OutputMessage(char_type);

		s = std::to_string(i.z);

		char_type = new char[s.length()];
		strcpy(char_type, s.c_str());
		animTcl::OutputMessage(char_type);
	}
	
}	// HermiteSplineSystem::getState

void HermiteSplineSystem::setState(double* p)
{
	
	VecCopy(m_pos, p);
	addVert(p);
	glBegin(GL_LINE);
	glVertex3f(.25, 0.25, 0);
	glVertex3f(.75, .75, 0);
	glEnd();
	

}	// HermiteSplineSystem::setState

void HermiteSplineSystem::reset(double time)
{

	setVector(m_pos, 0, 0, 0);

}	// HermiteSplineSystem::Reset

void HermiteSplineSystem::addVert(double* p) {
	
	HermiteSplineSystem::vert_pos vertex = { p[0], p[1], p[2] };

	vertices.push_back(vertex);
}

std::vector<HermiteSplineSystem::vert_pos> HermiteSplineSystem::getVert() {
	return vertices;
}


int HermiteSplineSystem::command(int argc, myCONST_SPEC char** argv)
{
	if (argc < 1)
	{
		animTcl::OutputMessage("system %s: wrong number of params.", m_name.c_str());
		return TCL_ERROR;
	}
	else if (strcmp(argv[0], "read") == 0)
	{
		if (argc == 2)
		{
			m_model.ReadOBJ(argv[1]);
			glmFacetNormals(&m_model);
			glmVertexNormals(&m_model, 90);
			return TCL_OK;
		}
		else
		{
			animTcl::OutputMessage("Usage: read <file_name>");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "scale") == 0)
	{
		if (argc == 4)
		{
			m_sx = (float)atof(argv[1]);
			m_sy = (float)atof(argv[2]);
			m_sz = (float)atof(argv[3]);
		}
		else
		{
			animTcl::OutputMessage("Usage: scale <sx> <sy> <sz> ");
			return TCL_ERROR;

		}
	}
	else if (strcmp(argv[0], "pos") == 0)
	{
		if (argc == 4)
		{
			m_pos[0] = atof(argv[1]);
			m_pos[1] = atof(argv[2]);
			m_pos[2] = atof(argv[3]);
		}
		else
		{
			animTcl::OutputMessage("Usage: pos <x> <y> <z> ");
			return TCL_ERROR;

		}
	}
	else if (strcmp(argv[0], "flipNormals") == 0)
	{
		flipNormals();
		return TCL_OK;

	}
	else if (strcmp(argv[0], "reset") == 0)
	{
		double p[3] = { 0,0,0 };
		setState(p);
	}

	glutPostRedisplay();
	return TCL_OK;

}	// HermiteSplineSystem::command

void HermiteSplineSystem::display(GLenum mode)
{
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glTranslated(m_pos[0], m_pos[1], m_pos[2]);
	glScalef(m_sx, m_sy, m_sz);
	//glutSolidSphere(0.1, 20, 4);

	glPopMatrix();
	glPopAttrib();

}	// HermiteSplineSystem::display