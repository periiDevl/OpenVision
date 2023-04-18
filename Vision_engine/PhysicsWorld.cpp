#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld(glm::vec3 gravity, int iterations) {
	this->gravity = gravity;
	this->iterations = iterations;
}
void PhysicsWorld::Step(float deltaTime)
{
	for (int iter = 0; iter < iterations; iter++)
	{
		for (int i = 0; i < bodies.size(); i++) {
			if (!bodies[i]->GetIsStatic())
				bodies[i]->Step(deltaTime/iterations);
		}
	}
	for (int iter = 0; iter < iterations; iter++)
	{
		for (int i = 0; i < bodies.size(); i++) {
			for (int l = i + 1; l < bodies.size(); l++) {
				vec2 mtv;
				const PolygonCollider& colA = dynamic_cast<const PolygonCollider&>(*bodies[i]->GetCollider()) ;
				const PolygonCollider& colB = dynamic_cast<const PolygonCollider&>(*bodies[l]->GetCollider()) ;
				if (PolygonVPolygon(colA, colB, mtv)) {
					if (mtv == vec2(0.0f)) {
						cout << "bruh" << " mtv failed for some reason" << endl;
						return;
					}
					vec2 normal = normalize(mtv);
					float depth = length(mtv);


					PhysicsBody* bodyA = bodies[i];
					PhysicsBody* bodyB = bodies[l];

					// Get what bodies are static
					bool isAStatic = bodyA->GetIsStatic();
					bool isBStatic = bodyB->GetIsStatic();

					// Change their position based on MTV
					if (!isAStatic && !isBStatic) {
						bodyA->SetPosition(bodyA->GetPosition() + mtv / 2.0f);
						bodyB->SetPosition(bodyB->GetPosition() - mtv / 2.0f);
					}
					if (!isAStatic && isBStatic) {
						bodyA->SetPosition(bodyA->GetPosition() + mtv);
					}
					if (isAStatic && !isBStatic) {
						bodyB->SetPosition(bodyB->GetPosition() - mtv);
					}

					

					vec2 point1 = bodyA->GetCollider()->GetSupportPoint(-normal);
					vec2 point2 = bodyB->GetCollider()->GetSupportPoint(normal);
					int sizeContactPoints = point1 != point2 ? 2 : 1;
					cout << "Point 1 " << to_string(point1) << "Point 2 " << to_string(point2) << endl;

					// calculate relative velocity
					vec2 relVel = bodyB->GetVelocity() - bodyA->GetVelocity();

					// calculate impulse magnitude
					float e = std::min(bodyA->GetRestituion(), bodyB->GetRestituion()); // coefficient of restitution
					float j = -(1 + e) * dot(relVel, normal) / ((bodyA->GetIsStatic() ? 0 : 1 / bodyA->GetMass()) + (bodyB->GetIsStatic() ? 0 : 1 / bodyB->GetMass()) + 0.000000000001f);


					// apply impulse to the bodies
					if (!isAStatic && !isBStatic) {
						bodyA->SetVelocity(bodyA->GetVelocity() - j * normal / bodyA->GetMass());
						bodyB->SetVelocity(bodyB->GetVelocity() + j * normal / bodyB->GetMass());
					}
					if (!isAStatic && isBStatic) {
						cout << "body A vel:" << to_string(bodyA->GetVelocity()) << endl;
						bodyA->SetVelocity(bodyA->GetVelocity() - j * normal / bodyA->GetMass());
						cout << "body A updated vel:" << to_string(bodyA->GetVelocity()) << endl;
					}																		   
					if (isAStatic && !isBStatic) {											   
						bodyB->SetVelocity(bodyB->GetVelocity() + j * normal / bodyB->GetMass());
					}
				}
			}
		}
		
	}
	
}
void PhysicsWorld::AddBody(PhysicsBody* body) {
	body->SetGravity(gravity);
	bodies.push_back(body);
}

