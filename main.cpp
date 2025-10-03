#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

//Include propios
#include "Vector3D.h"
#include "Particle.h"

#include <iostream>

//std::string display_text = "This is a test";


using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;


PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;

//Variables propias
RenderItem* esferaPrueba = nullptr;	
RenderItem* ejeX = nullptr;
RenderItem* ejeY = nullptr;
RenderItem* ejeZ = nullptr;
Particle* esfera = nullptr;


// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),true,gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);

	// ----------------------------------------------------- PRÁCTICA 1 --------------------------------------------//
	// 
	// -----------------------------------------------------  Creacion objetos
	// En tu escena (por ejemplo dentro de init() o similar)
	physx::PxTransform* transform = new physx::PxTransform(PxVec3(0, 0, 0));

	RenderItem* esferaPrueba = new RenderItem(
		CreateShape(PxSphereGeometry(1.0f)),   // radio = 1.0
		transform,                      // posición en el mundo
		Vector4(0, 0, 1, 1)                    // color RGBA (azul en este caso)
	);

	physx::PxTransform* transformEjeX = new physx::PxTransform(PxVec3(2.5f, 0, 0));
	// Eje X (rojo)
	RenderItem* ejeX = new RenderItem(
		CreateShape(PxBoxGeometry(5, 0.05f, 0.05f)),  // caja larga en X
		transformEjeX,                         // desplazada en +X
		Vector4(1, 0, 0, 1)                           // rojo
	);

	physx::PxTransform* transformEjeY = new physx::PxTransform(PxVec3(0, 2.5f, 0));
	// Eje Y (verde)
	RenderItem* ejeY = new RenderItem(
		CreateShape(PxBoxGeometry(0.05f, 5, 0.05f)),
		transformEjeY,
		Vector4(0, 1, 0, 1)
	);

	physx::PxTransform* transformEjeZ = new physx::PxTransform(PxVec3(0, 2.5f, 0));
	// Eje Z (azul)
	RenderItem* ejeZ = new RenderItem(
		CreateShape(PxBoxGeometry(0.05f, 0.05f, 5)),
		transformEjeZ,
		Vector4(0, 0, 1, 1)
	);

	// ----------------------------------------------------- PRÁCTICA 2 --------------------------------------------//

	// -----------------------------------------------------  Creacion objetos 

	Particle* esfera = new Particle(
		PxSphereGeometry(1.0f),               // radio = 1.0
		physx::PxVec3(0, 0, 0),               // posición inicial
		Vector3(1, 0, 0),                     // velocidad constante en X
		Vector3(0, 0, 0),                     // sin aceleración
		1.0f,                                 // damping = 1 (sin efecto)
		Vector4(0, 0, 1, 1)                   // color azul
	);

	//Partícula con aceleracion
	Vector3 accel(0, -9.8f, 0); // gravedad
	Particle* esfera = new Particle(PxSphereGeometry(1.0f), PxVec3(0, 10, 0),
		Vector3(0, 0, 0), accel, 0.99f, Vector4(1, 0, 0, 1));


	}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	float dt = 0.016f; // ejemplo: 60 FPS
	esfera->integrate(dt);

	gScene->simulate(t);
	gScene->fetchResults(true);
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	//Borrado de items propios
	delete esferaPrueba; esferaPrueba = nullptr;
	delete ejeX; ejeX = nullptr;
	delete ejeY; ejeY = nullptr;
	delete ejeZ; ejeZ = nullptr;
	delete esfera; esfera = nullptr;

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	
	gFoundation->release();
	}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	switch(toupper(key))
	{
	//case 'B': break;
	//case ' ':	break;
	case ' ':
	{
		break;
	}
	default:
		break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}


int main(int, const char*const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for(PxU32 i=0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}