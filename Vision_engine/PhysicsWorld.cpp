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
			bodies[i]->Step(deltaTime / iterations);
		}

		for (int i = 0; i < bodies.size(); i++) {
			for (int l = i + 1; l < bodies.size(); l++) {
				vec2 mtv;
				if (((BoxCollider*)bodies[i]->GetCollider())->CollidesWithBox((BoxCollider*)(bodies[l]->GetCollider()), mtv)) {

					vec2 normal = normalize(mtv);
					float depth = length(mtv);


					PhysicsBody* bodyA = bodies[i];
					PhysicsBody* bodyB = bodies[l];

					bodyA->SetPosition(vec2(bodyA->GetPosition().x + mtv.x / 2, bodyA->GetPosition().y + mtv.y / 2));
					bodyB->SetPosition(vec2(bodyB->GetPosition().x - mtv.x / 2, bodyB->GetPosition().y - mtv.y / 2));

					vec2 point1 = bodyA->GetCollider()->GetSupportPoint(normal);
					vec2 point2 = bodyB->GetCollider()->GetSupportPoint(-normal);
					int sizeContactPoints = point1 != point2 ? 2 : 1;
					cout << "point 1:" << to_string(point1) << " point 2:" << to_string(point2) << endl;
					

					// calculate relative velocity
					vec2 relVel = bodyB->GetVelocity() - bodyA->GetVelocity();

					// calculate impulse magnitude
					float e = 0.5; // coefficient of restitution
					float impulseMag = -(1 + e) * dot(relVel, normal) / (1 / bodyA->GetMass() + 1 / bodyB->GetMass());

					// apply impulse to the bodies
					bodyA->SetVelocity(bodyA->GetVelocity() - impulseMag * normal / bodyA->GetMass());
					bodyB->SetVelocity(bodyB->GetVelocity() + impulseMag * normal / bodyB->GetMass());
				}
			}
		}
		
	}
	
}
void PhysicsWorld::AddBody(PhysicsBody* body) {
	body->SetGravity(gravity);
	bodies.push_back(body);
}

