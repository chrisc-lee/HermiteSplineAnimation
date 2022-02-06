////////////////////////////////////////////////////
// // Template code for  CS 174C
////////////////////////////////////////////////////

#ifdef WIN32
#include <windows.h>
#endif


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <shared/defs.h>

#include "shared/opengl.h"

#include <string.h>
#include <util/util.h>
#include <GLModel/GLModel.h>
#include "anim.h"
#include "animTcl.h"
#include "myScene.h"
#include "SampleParticle.h"
#include "SampleGravitySimulator.h"
#include "HermiteSpline.h"
#include "HermiteSplineSimulator.h"
//#include <util/jama/tnt_stopwatch.h>
//#include <util/jama/jama_lu.h>

// register a sample variable with the shell.
// Available types are:
// - TCL_LINK_INT 
// - TCL_LINK_FLOAT

int g_testVariable = 10;

std::list <HermiteSplineSystem*> control_points;
std::list <HermiteSplineSimulator*> simulators;

HermiteSplineSystem* hermite;


SETVAR myScriptVariables[] = {
	"testVariable", TCL_LINK_INT, (char *) &g_testVariable,
	"",0,(char *) NULL
};


//---------------------------------------------------------------------------------
//			Hooks that are called at appropriate places within anim.cpp
//---------------------------------------------------------------------------------

// start or end interaction
void myMouse(int button, int state, int x, int y)
{

	// let the global resource manager know about the new state of the mouse 
	// button
	GlobalResourceManager::use()->setMouseButtonInfo( button, state );

	if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
	{
		animTcl::OutputMessage(
			"My mouse received a mouse button press event\n");
		float normalX = (12) * ((x / (float)GLUT_SCREEN_WIDTH) / 2.5) - 6.0;
		float normalY = -1 * ((12) * ((y / (float)GLUT_SCREEN_HEIGHT) / 2.5) - 6.0);
		std::string s = std::to_string(normalY);

		char* char_type = new char[s.length()];
		strcpy(char_type, s.c_str());
		
		
		double p[6] = {normalX, normalY, 0, 1, 1, 0};

		double q[3] = { 6.0, 6.0, 0.0 };

		// register a system
		bool success;	

		BaseSystem* sampleSystemRetrieval;

		// retrieve the system
		sampleSystemRetrieval =
			GlobalResourceManager::use()->getSystem("hermite");

		sampleSystemRetrieval->setState(p);
		sampleSystemRetrieval->getState(p);

		// make sure you got it
		assert(sampleSystemRetrieval);

	}
	if( button == GLUT_LEFT_BUTTON && state == GLUT_UP )
	{
		animTcl::OutputMessage(
			"My mouse received a mouse button release event\n") ;
	}
}	// myMouse

// interaction (mouse motion)
void myMotion(int x, int y)
{

	GLMouseButtonInfo updatedMouseButtonInfo = 
		GlobalResourceManager::use()->getMouseButtonInfo();

	if( updatedMouseButtonInfo.button == GLUT_LEFT_BUTTON )
	{
		animTcl::OutputMessage(
			"My mouse motion callback received a mousemotion event\n") ;
	}

}	// myMotion


void MakeScene(void)
{

	/* 
	
	This is where you instantiate all objects, systems, and simulators and 
	register them with the global resource manager

	*/

	/* SAMPLE SCENE */

	//bool success;
	//HermiteSplineSystem* hermite = new HermiteSplineSystem("hermite");
	//success = GlobalResourceManager::use()->addSystem(hermite, true);

	// make sure it was registered successfully
	//assert(success);

	// register a system
	//SampleParticle* sphere1 = new SampleParticle( "sphere1" );

	//success = GlobalResourceManager::use()->addSystem( sphere1, true );

	// make sure it was registered successfully
	//assert( success );

	// register a simulator
	//SampleGravitySimulator* gravitySimulator = 
	//	new SampleGravitySimulator( "gravity", sphere1 );

	//success = GlobalResourceManager::use()->addSimulator( gravitySimulator );

	// make sure it was registered successfully
	//assert( success );

	/* END SAMPLE SCENE */

	// the following code shows you how to retrieve a system that was registered 
	// with the resource manager. 

	//BaseSystem* sampleSystemRetrieval;

	// retrieve the system
	//sampleSystemRetrieval = 
	//	GlobalResourceManager::use()->getSystem( "sphere1" );

	// make sure you got it
	//assert( sampleSystemRetrieval );

	//BaseSimulator* sampleSimulatorRetrieval;

	// retrieve the simulator
	//sampleSimulatorRetrieval = 
	//	GlobalResourceManager::use()->getSimulator( "gravity" );

	// make sure you got it
	//assert( sampleSimulatorRetrieval );

}	// MakeScene

// OpenGL initialization
void myOpenGLInit(void)
{
	animTcl::OutputMessage("Initialization routine was called.");

}	// myOpenGLInit

void myIdleCB(void)
{
	
	return;

}	// myIdleCB

void myKey(unsigned char key, int x, int y)
{
	 animTcl::OutputMessage("My key callback received a key press event\n");
	 if (key == 'r') {
		 animTcl::OutputMessage("R pressed\n");
		 
		 
		//retrieve the system	
		if (GlobalResourceManager::use()->getSystem("hermite")) {
			assert(GlobalResourceManager::use()->getSystem("hermite"));
			animTcl::OutputMessage("Reset hermite pressed\n");
			char* string = "reset";
			char** string_list = &string;
			GlobalResourceManager::use()->getSystem("hermite")->command(1, string_list);
		 }
	 }
	return;

}	// myKey

static int testGlobalCommand(ClientData clientData, Tcl_Interp *interp, int argc, myCONST_SPEC char **argv)
{
	 animTcl::OutputMessage("This is a test command!");
    animTcl::OutputResult("100") ;
	return TCL_OK;

}	// testGlobalCommand

static int intializePart1(ClientData clientData, Tcl_Interp* interp, int argc, myCONST_SPEC char** argv)
{
	bool success;
	HermiteSplineSystem* hermite = new HermiteSplineSystem("hermite");
	success = GlobalResourceManager::use()->addSystem(hermite, true);
	// make sure it was registered successfully
	assert(success);
	animTcl::OutputMessage("Part 1 intialized.");
	return TCL_OK;

}	// testGlobalCommand

static int intializePart2(ClientData clientData, Tcl_Interp* interp, int argc, myCONST_SPEC char** argv)
{
	bool success;
	//HermiteSplineSystem* hermite = new HermiteSplineSystem("hermite");
	//success = GlobalResourceManager::use()->addSystem(hermite, true);
	// make sure it was registered successfully
	//assert(success);
	animTcl::OutputMessage("Part 2 intialized.");
	return TCL_OK;

}	// testGlobalCommand

void mySetScriptCommands(Tcl_Interp *interp)
{

	// here you can register additional generic (they do not belong to any object) 
	// commands with the shell

	Tcl_CreateCommand(interp, "test", testGlobalCommand, (ClientData) NULL,
					  (Tcl_CmdDeleteProc *)	NULL);

	Tcl_CreateCommand(interp, "part1", intializePart1, (ClientData)NULL,
		(Tcl_CmdDeleteProc*)NULL);

	Tcl_CreateCommand(interp, "part2", intializePart2, (ClientData)NULL,
		(Tcl_CmdDeleteProc*)NULL);

}	// mySetScriptCommands
