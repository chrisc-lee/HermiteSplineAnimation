#include "HermiteSplineSimulator.h"
#include "HermiteSpline.h"
#include "GlobalResourceManager.h"

HermiteSplineSimulator::HermiteSplineSimulator(const std::string& name, BaseSystem* target, BaseSystem* previous) :
	BaseSimulator(name),
	m_target(target),
	m_previous(previous)
{
	curve_drawn = false;
	curr_iter = 0;
}	// SampleGravitySimulator


HermiteSplineSimulator::~HermiteSplineSimulator()
{
}	// SampleGravitySimulator::~SampleGravitySimulator

int HermiteSplineSimulator::step(double time)
{
	/*double g = -9.8;
	Vector pos;
	m_target->getState(pos);
	pos[0] = m_pos0[0] * cos(time) + m_vel0[0];
	pos[1] = 2 * sin(2 * 3.14 * time) + m_vel0[1];
	pos[2] = m_pos0[2];

	//   pos[1]= 2 * sin(2*3.14*time) ;

	m_target->setState(pos);*/
	
	bool success;
	float t = 0;
	//glBegin(GL_LINE_STRIP);
	
	while (curve_drawn == false){
		animTcl::OutputMessage(
			"Animating");
		t = (1 / double(NUM_ITERATIONS - 1.0)) * curr_iter;
		std::string s = std::to_string(curr_iter);
		char* char_type = new char[s.length()];
		strcpy(char_type, s.c_str());
		animTcl::OutputMessage(char_type);
		std::string name = this->retrieveName();
		name.append(char_type);
		// register new interpolation point
		animTcl::OutputMessage(char_type);
		HermiteSplineSystem* hermite = new HermiteSplineSystem(name);
		success = GlobalResourceManager::use()->addSystem(hermite, true);
		// make sure it was registered successfully
		assert(success);

		BaseSystem* sampleSystemRetrieval;

		Vector pos1;
		Vector pos2;

		m_target->getState(pos1);
		m_previous->getState(pos2);

		//Vector r1;
		//setVector(r1, 1, 3, 0);
		//Vector r2;
		//setVector(r2, 1, 2, 0);


		
		// retrieve the system
		//sampleSystemRetrieval =
		//	GlobalResourceManager::use()->getSystem(name);

		//float x = f1(t) * pos2[0] + f2(t) * pos1[0] + f3(t) * r1[0] + f4(t) * r2[0];
		//float y = f1(t) * pos2[1] + f2(t) * pos1[1] + f3(t) * r1[1] + f4(t) * r2[1];
		//glVertex2f(f1(t) * pos2[0] + f2(t) * pos1[0] + f3(t) * r1[0] + f4(t) * r2[0],
		//	f1(t) * pos2[1] + f2(t) * pos1[1] + f3(t) * r1[1] + f4(t) * r2[1]);
		//double p[3] = { x, y, 0 };
		//sampleSystemRetrieval->setState(p);
		//GLubyte* PixelBuffer = new GLubyte[(float)GLUT_SCREEN_WIDTH * (float)GLUT_SCREEN_HEIGHT * 3];
		//PixelBuffer[curr_iter] = x;
		//PixelBuffer[curr_iter + 1] = y;
		//PixelBuffer[curr_iter + 2] = 0;
		//glBegin(GL_LINE_STRIP);
		//glVertex2f(pos2[0], pos2[1]);
		//glVertex2f(x, y);
		//glEnd();
		//glutSwapBuffers();
		

		curr_iter++;
		if (curr_iter >= NUM_ITERATIONS - 1) {
			curve_drawn = true;
		}
	}
	
	return 0;

}	// SampleGravitySimulator::step

