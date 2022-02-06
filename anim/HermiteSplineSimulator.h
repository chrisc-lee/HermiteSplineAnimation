#ifndef MY_HERMITE_SIMULATOR_H
#define MY_HERMITE_SIMULATOR_H

#include "animTcl.h"

#include "BaseSimulator.h"
#include "BaseSystem.h"

#include <string>

// a sample simulator

class HermiteSplineSimulator : public BaseSimulator
{
public:

	HermiteSplineSimulator(const std::string& name, BaseSystem* target, BaseSystem* previous);
	~HermiteSplineSimulator();

	int step(double time);
	int init(double time)
	{
		m_target->getState(m_pos0);
		setVector(m_vel0, 0, 0, 0);
		return 0;
	};

	int command(int argc, myCONST_SPEC char** argv) { return TCL_OK; }


protected:

	Vector m_pos0; // initial position
	Vector m_vel0; // initial velocity
	Vector m_pos;
	Vector m_vel;

	BaseSystem* m_previous;
	BaseSystem* m_target;

	float f1(float t);
	float f2(float t);
	float f3(float t);
	float f4(float t);

	const int NUM_ITERATIONS = 100;

	bool curve_drawn;
	int curr_iter;

};


#endif