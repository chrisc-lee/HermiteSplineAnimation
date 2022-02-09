#include "HermiteSpline.h"
#include "anim.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>

// blending functions for hermite curve
float HermiteSplineSystem::f1(float t) {
	return 2 * t * t * t - 3 * t * t + 1;
}
float HermiteSplineSystem::f2(float t) {
	return -2 * t * t * t + 3 * t * t;
}
float HermiteSplineSystem::f3(float t) {
	return t * t * t - 2 * t * t + t;
}
float HermiteSplineSystem::f4(float t) {
	return t * t * t - t * t;
}

// Catmull-Rom calculations
// calculate s0
double* HermiteSplineSystem::findTangentStartPoint (double* y0, double* y1, double* y2) {
	double result_tan[3] = { (2 * (y2[0] - y0[0])) - ((y1[0] - y0[0]) / 2.0), (2 * (y2[1] - y0[1])) - ((y1[1] - y0[1]) / 2.0), (2 * (y2[2] - y0[2])) - ((y2[2] - y0[2]) / 2.0) };
	return result_tan;
}

// calculate si i != 0 and i != n - 1
double* HermiteSplineSystem::findTangentEndPoint (double* y0, double* y1, double* y2) {
	double result_tan[3] = { (2.0 * (y0[0] - y2[0])) - ((y0[0] - y1[0]) / 2.0), (2.0 * (y0[1] - y2[1])) - ((y0[1] - y1[1]) / 2.0), (2.0 * (y0[2] - y2[2])) - ((y0[2] - y1[2]) / 2.0) };
	return result_tan;
}

// calculate sn - 1
double* HermiteSplineSystem::findTangentMidPoint(double* y0, double* y1) {

	double result_tan[3] = { (y1[0] - y0[0]) / 2.0, (y1[1] - y0[1]) / 2.0, (y1[2] - y0[2]) / 2.0 };
	return result_tan;
}

float HermiteSplineSystem::getArcLength(float t) {
	if (!arc_length_table.empty()) {
		if (floor(t / (1.0 / (float)arc_length_table.size())) >= arc_length_table.size() - 1) {
			int index = arc_length_table.size() - 1;
			float arc = arc_length_table.at(index);
			return arc;
		}
		else {
			int index = floor(t / (1.0 / (float)arc_length_table.size()));
			float s_i = arc_length_table.at(index);
			float s_i_1 = arc_length_table.at(index + 1);
			float u_i = (1.0 / (float)arc_length_table.size() * (float)index);
			float u_i_1 = (1.0 / (float)arc_length_table.size() * (float)index) + (1.0 / (float)arc_length_table.size());
			float arc = s_i + ((t - u_i) / (u_i_1 - u_i)) * (s_i_1 - s_i);
			return arc;
		}
	}
	return 0.0;
}

void HermiteSplineSystem::calculateArcLengthTable() {
	arc_length_table.clear();
	arc_length_table.push_back(0.0);
	int j = 0;
	std::string s = std::to_string(arc_length_table[j]);
	char* char_type = new char[s.length()];
	strcpy(char_type, s.c_str());
	animTcl::OutputMessage(char_type);
	if (vertices.size() / 2 > 2)
	{
		for (int i = 0; i < vertices.size() - 2; i += 2) {
			arc_length_table.push_back(sqrt(pow((vertices.at(i + 2).x - vertices.at(i).x), 2) + pow((vertices.at(i + 2).y - vertices.at(i).y), 2) + pow((vertices.at(i + 2).z - vertices.at(i).z), 2)) + arc_length_table.at(j));
			s = std::to_string(arc_length_table[j + 1]);
			char_type = new char[s.length()];
			strcpy(char_type, s.c_str());
			animTcl::OutputMessage(char_type);
			j++;
		}

	}

}
HermiteSplineSystem::HermiteSplineSystem(const std::string& name) :
	BaseSystem(name),
	m_sx(1.0f),
	m_sy(1.0f),
	m_sz(1.0f)
{
	cat_rom = false;
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
	if ((vertices.size() / 2) + 1 > 40) {
		animTcl::OutputMessage("Too many control points. Maximum 40 control points permitted.");
		return;
	}
	Vector pos;
	setVector(pos, p[0], p[1], p[2]);
	VecCopy(m_pos, pos);
	addVert(p);
	calculateArcLengthTable();

}	// HermiteSplineSystem::setState

void HermiteSplineSystem::reset(double time)
{

	setVector(m_pos, 0, 0, 0);

}	// HermiteSplineSystem::Reset

void HermiteSplineSystem::addVert(double* p) {
	
	HermiteSplineSystem::vert_pos vertex = { p[0], p[1], p[2] };
	HermiteSplineSystem::vert_pos tangent = { p[3], p[4], p[5] };

	vertices.push_back(vertex);
	vertices.push_back(tangent);
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
	//else if (strcmp(argv[0], "read") == 0)
	//{
	//	if (argc == 2)
	//	{
	//		m_model.ReadOBJ(argv[1]);
	//		glmFacetNormals(&m_model);
	//		glmVertexNormals(&m_model, 90);
	//		return TCL_OK;
	//	}
	//	else
	//	{
	//		animTcl::OutputMessage("Usage: read <file_name>");
	//		return TCL_ERROR;
	//	}
	//}
	//else if (strcmp(argv[0], "scale") == 0)
	//{
	//	if (argc == 4)
	//	{
	//		m_sx = (float)atof(argv[1]);
	//		m_sy = (float)atof(argv[2]);
	//		m_sz = (float)atof(argv[3]);
	//	}
	//	else
	//	{
	//		animTcl::OutputMessage("Usage: scale <sx> <sy> <sz> ");
	//		return TCL_ERROR;

	//	}
	//}

	//else if (strcmp(argv[0], "pos") == 0)
	//{
	//	if (argc == 4)
	//	{
	//		m_pos[0] = atof(argv[1]);
	//		m_pos[1] = atof(argv[2]);
	//		m_pos[2] = atof(argv[3]);
	//	}
	//	else
	//	{
	//		animTcl::OutputMessage("Usage: pos <x> <y> <z> ");
	//		return TCL_ERROR;

	//	}
	//}
	//else if (strcmp(argv[0], "flipNormals") == 0)
	//{
	//	flipNormals();
	//	return TCL_OK;

	//}
	else if ((strcmp(argv[0], "set") == 0 && strcmp(argv[1], "point") == 0)) {
		if (vertices.empty()) {
			animTcl::OutputMessage("No control points to set. Please add some control points.");
			return TCL_ERROR;
		}
		if ((atoi(argv[2]) * 2) > (vertices.size() - 1)) {
			animTcl::OutputMessage("Index out of bounds.");
			return TCL_ERROR;
		}
		if (argc < 6)
		{
			animTcl::OutputMessage("Usage: set point <index> <x y z>");
			return TCL_ERROR;
		}
		HermiteSplineSystem::vert_pos vertex = { atof(argv[3]), atof(argv[4]), atof(argv[5]) };
		vertices.at(atoi(argv[2]) * 2) = vertex;
		calculateArcLengthTable();
		glutPostRedisplay();
		return TCL_OK;
	}
	else if (strcmp(argv[0], "set point") == 0)
	{
		if (vertices.empty()) {
			animTcl::OutputMessage("No control points to set. Please add some control points.");
			return TCL_ERROR;
		}
		if ((atoi(argv[1]) * 2) > (vertices.size() - 1)) {
			animTcl::OutputMessage("Index out of bounds.");
			return TCL_ERROR;
		}
		if (argc < 5)
		{
			animTcl::OutputMessage("Usage: set point <index> <x y z>");
			return TCL_ERROR;
		}
		HermiteSplineSystem::vert_pos vertex = { atof(argv[2]), atof(argv[3]), atof(argv[4]) };
		vertices.at(atoi(argv[1]) * 2) = vertex;
		calculateArcLengthTable();
		glutPostRedisplay();
		return TCL_OK;

	}
	else if ((strcmp(argv[0], "set") == 0 && strcmp(argv[1], "tangent") == 0)) 
	{
		if (vertices.empty()) {
			animTcl::OutputMessage("No control points to set. Please add some control points.");
			return TCL_ERROR;
		}
		if ((atoi(argv[2]) * 2) > (vertices.size() - 1)) {
			animTcl::OutputMessage("Index out of bounds.");
			return TCL_ERROR;
		}
		if (argc < 6)
		{
			animTcl::OutputMessage("Usage: set tangent <index> <x y z>");
			return TCL_ERROR;
		}
		HermiteSplineSystem::vert_pos vertex = { atof(argv[3]), atof(argv[4]), atof(argv[5]) };
		vertices.at((atoi(argv[2]) * 2) + 1) = vertex;
		glutPostRedisplay();
		return TCL_OK;
	}
	else if (strcmp(argv[0], "set tangent") == 0)
	{
		if (vertices.empty()) {
			animTcl::OutputMessage("No control points to set. Please add some control points.");
			return TCL_ERROR;
		}
		if ((atoi(argv[1]) * 2) > (vertices.size() - 1)) {
			animTcl::OutputMessage("Index out of bounds.");
			return TCL_ERROR;
		}
		if (argc < 5)
		{
			animTcl::OutputMessage("Usage: set tangent <index> <x y z>");
			return TCL_ERROR;
		}
		HermiteSplineSystem::vert_pos vertex = { atof(argv[2]), atof(argv[3]), atof(argv[4]) };
		vertices.at((atoi(argv[1]) * 2) + 1) = vertex;
		glutPostRedisplay();
		return TCL_OK;

	}
	else if ((strcmp(argv[0], "add") == 0 && strcmp(argv[1], "point") == 0)) {
		if (argc < 8)
		{
			animTcl::OutputMessage("Usage: add point <x y z sx sy sz>");
			return TCL_ERROR;
		}
		double vertex[6] = { atof(argv[2]), atof(argv[3]), atof(argv[4]), atof(argv[5]), atof(argv[6]), atof(argv[7]) };
		setState(vertex);
		glutPostRedisplay();
		return TCL_OK;
	}
	else if (strcmp(argv[0], "add point") == 0)
	{
		if (argc < 7)
		{
			animTcl::OutputMessage("Usage: add point <x y z sx sy sz>");
			return TCL_ERROR;
		}
		double vertex[6] = { atof(argv[1]), atof(argv[2]), atof(argv[3]), atof(argv[4]), atof(argv[5]), atof(argv[6]) };
		setState(vertex);
		glutPostRedisplay();
		return TCL_OK;

	}

	else if (strcmp(argv[0], "load") == 0) {
		// system hermite load gradingTest3524.txt
		std::ifstream file(argv[1]);
		if (file.is_open()) {
			std::string line;
			std::getline(file, line); // skip the first line
			while (std::getline(file, line)) {
				float value;
				std::stringstream ss(line);
				std::vector<float> temps = std::vector<float>();
				while (ss >> value) {
					temps.push_back(value);
				}
				double vertex[6] = { temps.at(0), temps.at(1), temps.at(2), temps.at(3), temps.at(4), temps.at(5) };
				setState(vertex);
				
			}
			file.close();
			glutPostRedisplay();
		}
		else {
			animTcl::OutputMessage("Unable to open file.");
			return TCL_ERROR;
		}
		
	}

	else if (strcmp(argv[0], "export") == 0) {
		// system hermite export test.txt
		std::string line;
		std::ofstream  file(argv[1]);
		if (file.is_open())
		{
			file << this->retrieveName() << " " << std::to_string(vertices.size() / 2) << "\n";
			for (int i = 0; i < vertices.size() - 1; i += 2) {
				std::string x = std::to_string(vertices.at(i).x);
				std::string y = std::to_string(vertices.at(i).y);
				std::string z = std::to_string(vertices.at(i).z);
				std::string sx = std::to_string(vertices.at(i + 1).x);
				std::string sy = std::to_string(vertices.at(i + 1).y);
				std::string sz = std::to_string(vertices.at(i + 1).z);
				file << x << " " << y << " " << z << " " << sx << " " << sy << " " << sz << '\n';
			}
			
		}
		else {
			animTcl::OutputMessage("Unable to open file.");
			return TCL_ERROR;

		}
		file.close();
	}

	else if (strcmp(argv[0], "cr") == 0) 
	{
		if (cat_rom == false) {
			cat_rom = true;
		}
		else {
			cat_rom = false;
		}
		glutPostRedisplay();

	}

	else if (strcmp(argv[0], "getArcLength") == 0) 
	{
		if (argc < 2)
		{
			animTcl::OutputMessage("Usage: getArcLength <t>");
			return TCL_ERROR;
		}
		float arc_length = getArcLength(atof(argv[1]));
		std::string s = std::to_string(arc_length);
		char* char_type = new char[s.length()];
		strcpy(char_type, s.c_str());
		animTcl::OutputMessage(char_type);
	}

	else if (strcmp(argv[0], "reset") == 0)
	{
		vertices.clear();
		glutPostRedisplay();
	}

	glutPostRedisplay();
	return TCL_OK;

}	// HermiteSplineSystem::command

// system hermite "set point" 1 2 2 0
// system hermite "add point" 1 2 2 1 1 0

void HermiteSplineSystem::display(GLenum mode)
{
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	//glTranslated(m_pos[0], m_pos[1], m_pos[2]);
	//glScalef(m_sx, m_sy, m_sz);
	Vector r1;
	Vector r2;
	Vector pos1;
	Vector pos2;
	glBegin(GL_LINE_STRIP);
	int n = 500;
	if (vertices.size() > 0) {
		for (int j = 0; j < vertices.size() - 2; j+=2) {
			if (vertices.size() > 3) {
				
				setVector(pos1, vertices.at(j).x, vertices.at(j).y, vertices.at(j).z);
				setVector(pos2, vertices.at(j + 2).x, vertices.at(j + 2).y, vertices.at(j + 2).z);
				
				if (cat_rom) {
					double q1[3] = { pos1[0], pos1[1], pos1[2] };
					double q2[3] = { pos2[0], pos2[1], pos2[2] };
					if (vertices.size() / 2 == 2) {
						double* r1_temp = findTangentMidPoint(q1, q2);
						setVector(r1, r1_temp[0], r1_temp[1], r1_temp[2]);
						double* r2_temp = findTangentMidPoint(q1, q2);
						setVector(r2, r2_temp[0], r2_temp[1], r1_temp[2]);
					}
					else if (j == 0) {
						double q3[3] = { vertices.at(j + 4).x, vertices.at(j + 4).y , vertices.at(j + 4).z };
						double* r1_temp = findTangentStartPoint(q1, q3, q2);
						setVector(r1, r1_temp[0], r1_temp[1], r1_temp[2]);
						double* r2_temp = findTangentMidPoint(q1, q3);
						setVector(r2, r2_temp[0], r2_temp[1], r2_temp[2]);
					}
					else if (j == vertices.size() - 4) {
						double q3[3] = { vertices.at(j - 2).x, vertices.at(j - 2).y , vertices.at(j - 2).z };
						double* r1_temp = findTangentMidPoint(q3, q2);
						setVector(r1, r1_temp[0], r1_temp[1], r1_temp[2]);
						double* r2_temp = findTangentEndPoint(q2, q3, q1);
						setVector(r2, r2_temp[0], r2_temp[1], r2_temp[2]);
					}
					else {
						double q3[3] = { vertices.at(j - 2).x, vertices.at(j - 2).y , vertices.at(j - 2).z };
						double q4[3] = { vertices.at(j + 4).x, vertices.at(j + 4).y , vertices.at(j + 4).z };
						double* r1_temp = findTangentMidPoint(q3, q2);		
						setVector(r1, r1_temp[0], r1_temp[1], r1_temp[2]);
						double* r2_temp = findTangentMidPoint(q1, q4);
						setVector(r2, r2_temp[0], r2_temp[1], r2_temp[2]);
						
					}
				}
				else {
					setVector(r1, vertices.at(j + 1).x, vertices.at(j + 1).y, vertices.at(j + 1).z);
					setVector(r2, vertices.at(j + 3).x, vertices.at(j + 3).y, vertices.at(j + 3).z);
				}
			}
			//std::string s = std::to_string(r1[0]);
			//char* char_type = new char[s.length()];
			//strcpy(char_type, s.c_str());
			//animTcl::OutputMessage(char_type);
			for (int i = 0; i < n; i++) {

				float t = (float)i / (float)n;
				float x = HermiteSplineSystem::f1(t) * pos1[0] + HermiteSplineSystem::f2(t) * pos2[0] + HermiteSplineSystem::f3(t) * r1[0] + HermiteSplineSystem::f4(t) * r2[0];
				float y = HermiteSplineSystem::f1(t) * pos1[1] + HermiteSplineSystem::f2(t) * pos2[1] + HermiteSplineSystem::f3(t) * r1[1] + HermiteSplineSystem::f4(t) * r2[1];
				float z = HermiteSplineSystem::f1(t) * pos1[2] + HermiteSplineSystem::f2(t) * pos2[2] + HermiteSplineSystem::f3(t) * r1[2] + HermiteSplineSystem::f4(t) * r2[2];

				glVertex3f(x, y, z);

			}
		}

	}
	
	
	//glVertex2f(pos2[0], pos2[1]);
	glEnd();
	
	//glutSolidSphere(0.1, 20, 4);

	glPopMatrix();
	glPopAttrib();

}	// HermiteSplineSystem::display