

#include<bullet/btBulletDynamicsCommon.h>

#include <iostream>
#include"pch.h"
#include"OVLIB.h"

class Script {

public:

	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

	// Create the dispatcher
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	// Create the broadphase
	btBroadphaseInterface* broadphase = new btDbvtBroadphase();

	// Create the constraint solver
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	// Create the world
	btDiscreteDynamicsWorld* world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	btRigidBody* boxBody;
	void Start() {
		world->setGravity(btVector3(0, -10, 0)); // Set gravity

		// Create a ground plane
		btCollisionShape* groundShape = new btBoxShape(btVector3(5, 1, 5));
		btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform::getIdentity());
		btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
		btRigidBody* groundBody = new btRigidBody(groundRigidBodyCI);
		world->addRigidBody(groundBody);

		// Create a dynamic object (replace with your desired shape and properties)
		btCollisionShape* boxShape = new btBoxShape(btVector3(1, 1, 1));
		btDefaultMotionState* boxMotionState = new btDefaultMotionState(btTransform(btQuaternion::getIdentity(), btVector3(0, 5, 0)));
		btScalar mass = 1.0f;
		btVector3 inertia(0, 0, 0);
		boxShape->calculateLocalInertia(mass, inertia);
		btRigidBody::btRigidBodyConstructionInfo boxRigidBodyCI(mass, boxMotionState, boxShape, inertia);
		boxBody = new btRigidBody(boxRigidBodyCI);
		world->addRigidBody(boxBody);

		std::cout << "HI!" << std::endl;
	}
	void Update() {

		// Main simulation loop
				// Create the collision configuration

		world->stepSimulation(1.0f / 60.0f, 10); // Simulate for 1/60th of a second, 10 iterations
		// Print the Y position of the dynamic object
		btTransform transform = boxBody->getWorldTransform();
		btVector3 position = transform.getOrigin();
		std::cout << "Dynamic object Y position: " << position.getY() << std::endl;
		
	}
	void Exit() {

	}


};

