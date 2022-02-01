#include "SampleGravitySimulator.h"

SampleGravitySimulator::SampleGravitySimulator( const std::string& name, BaseSystem* target ):
	BaseSimulator( name ),
	m_object( target )
{
}	// SampleGravitySimulator

SampleGravitySimulator::~SampleGravitySimulator()
{
}	// SampleGravitySimulator::~SampleGravitySimulator

int SampleGravitySimulator::step(double time)
{
	double g = -9.8;
	Vector pos;
	m_object->getState(pos);
	pos[0] = m_pos0[0] * cos(time) + m_vel0[0];
	pos[1] = 2 * sin(2 * 3.14 * time) + m_vel0[1];
	pos[2] = m_pos0[2];
    
 //   pos[1]= 2 * sin(2*3.14*time) ;
    
	m_object->setState(pos);

	return 0;

}	// SampleGravitySimulator::step
