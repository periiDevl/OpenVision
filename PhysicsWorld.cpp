#include "PhysicsWorld.h"
/// <summary>
/// Adding a function for cross that takes 2 vector2 
/// </summary>
namespace glm {
	float cross(const glm::vec2& a, const glm::vec2& b) {
		return a.x * b.y - a.y * b.x;
	}
	vec2 cross(const float& a, const glm::vec2& b) {
		return vec2{ -a * b.y, a * b.x };
	}
}
PhysicsWorld::PhysicsWorld(glm::vec3 gravity, int iterations) {
	this->gravity = gravity;
	this->iterations = iterations;
}
void PhysicsWorld::Step(float deltaTime)
{
	for (int iter = 0; iter < iterations; iter++)
	{
		for (int i = 0; i < bodies.size(); i++) {
			if (!bodies[i]->IsStatic()) {
				bodies[i]->Step(deltaTime / iterations);
			}
		}
	}

	for (int iter = 0; iter < iterations; iter++)
	{

		for (int i = 0; i < bodies.size(); i++) {
			if (bodies[i]->IsTrigger()) {
				continue;
			}

			for (int l = i + 1; l < bodies.size(); l++) {
				if (bodies[l]->IsTrigger()) {
					continue;
				}

				Manifold manifold;

				if (CheckCollision(*bodies[i]->GetCollider(), *bodies[l]->GetCollider(), manifold)) {
					if (manifold.mtv == vec2(0.0f)) {
						continue;
					}

					cout << "mtv:" << glm::to_string(manifold.mtv) << endl;

					PhysicsBody* bodyA = bodies[i];
					PhysicsBody* bodyB = bodies[l];

					SeparateBodies(bodyA, bodyB, manifold);

					PolygonCollider& polyA = dynamic_cast<PolygonCollider&>(*bodyA->GetCollider());
					PolygonCollider& polyB = dynamic_cast<PolygonCollider&>(*bodyB->GetCollider());
					GetContactPointsPolyVPoly(polyA, polyB, manifold);
					vector<vec2> contactPoints = manifold.contactPoints;

					RotationResolution(bodyA, bodyB, manifold);
				}
			}	
		}
		
	}
	
}

void PhysicsWorld::SeparateBodies(PhysicsBody* bodyA, PhysicsBody * bodyB, Manifold manifold) {
	bool staticA = bodyA->IsStatic();
	bool staticB = bodyB->IsStatic();

	if (!staticA && !staticB) {
		*bodyA->position -= manifold.mtv / 2.0f;
		*bodyB->position += manifold.mtv / 2.0f;
	}
	if (!staticA && staticB) {
		*bodyA->position -= manifold.mtv;
	}
	if (staticA && !staticB) {
		*bodyB->position += manifold.mtv;
	}
}
void PhysicsWorld::RotationResolution(PhysicsBody* bodyA, PhysicsBody* bodyB, Manifold manifold) {

	vec2 normal = manifold.normal;
	vector<vec2> contactPoints = manifold.contactPoints;

	float e = std::min(bodyA->restitution, bodyB->restitution);


	for (size_t i = 0; i < contactPoints.size(); ++i) {
		vec2 ra = contactPoints[i] - *bodyA->position;
		vec2 rb = contactPoints[i] - *bodyB->position;

		vec2 relativeVel =
			bodyB->LinearVelocity() + glm::cross(bodyB->AngularVelocity(), rb)
			- bodyA->LinearVelocity() - glm::cross(bodyA->AngularVelocity(), ra);


		float velAlongNormal = dot(relativeVel, normal);

		if (velAlongNormal > 0.0f)
			return;

		float raCrossN = cross(ra, normal);
		float rbCrossN = cross(rb, normal);

		float invMassSum = bodyA->InvMass() + bodyB->InvMass()
			+ (raCrossN * raCrossN) * bodyA->InvInertia()
			+ (rbCrossN * rbCrossN) * bodyB->InvInertia();

		cout << "inv mass sum:" << invMassSum << endl;
		float j = -(1.0f + e) * velAlongNormal;
		j /= (invMassSum * (float)contactPoints.size());

		vec2 impulse = normal * j;

		bodyA->AddLinearVelocity(-impulse * bodyA->InvMass());
		bodyB->AddLinearVelocity(impulse * bodyB->InvMass());
		//bodyA->AddAngularVelocity(bodyA->InvInertia() * glm::cross(ra, -impulse));
		//bodyB->AddAngularVelocity(bodyB->InvInertia() * glm::cross(rb, impulse));

	}
}
void PhysicsWorld::SimpleResolution(PhysicsBody* bodyA, PhysicsBody* bodyB, Manifold manifold) {
	vec2 relVel = bodyB->LinearVelocity() - bodyA->LinearVelocity();

	vector<vec2> contactPoints = manifold.contactPoints;

	float e = std::min(bodyA->restitution, bodyB->restitution); 
	
	float j = -(1 + e) * glm::dot(relVel, manifold.normal) / (bodyA->InvMass() + bodyB->InvMass() + 0.000000000001f);

	// friction stuff
	vec2 tangent = vec2(-manifold.normal.y, manifold.normal.x);
	vec2 relVelTangent = relVel - glm::dot(relVel, manifold.normal) * manifold.normal;
	
	float friction = std::min(bodyA->friction, bodyB->friction);
	float jt = -dot(relVelTangent, tangent) / ((bodyA->InvMass()) + (bodyB->InvMass()) + 0.000000000001f);
	jt /= contactPoints.size(); // average for multiple contact points
	
	if (std::abs(jt) > std::abs(j) * friction) {
		jt = std::copysign(std::abs(j) * friction, jt);
	}
	// no friction for debugging
	jt = 0;
	         
	//      //apply impulse to the bodies
	//      if (!bodyA->IsStatic() && !bodyB->IsStatic()) {
	//      	bodyA->LinearVelocity(bodyA->LinearVelocity() - (j * manifold.normal + jt * tangent) / bodyA->Mass());
	//      	bodyB->LinearVelocity(bodyB->LinearVelocity() + (j * manifold.normal + jt * tangent) / bodyB->Mass());
	//      }
	//      if (!bodyA->IsStatic() && bodyB->IsStatic()) {
	//      	bodyA->LinearVelocity(bodyA->LinearVelocity() - (j * manifold.normal + jt * tangent) / bodyA->Mass());
	//      }
	//      if (bodyA->IsStatic() && !bodyB->IsStatic()) {
	//      	bodyB->LinearVelocity(bodyB->LinearVelocity() + (j * manifold.normal + jt * tangent) / bodyB->Mass());
	//      }
	// 
	bodyA->AddLinearVelocity(-(j * manifold.normal + jt * tangent * bodyA->InvMass()));
	bodyB->AddLinearVelocity( (j * manifold.normal + jt * tangent * bodyB->InvMass()));
}
bool PhysicsWorld::TouchingLayer(PhysicsBody* body, int layer) {

	UpdateLayerBodies();

	// There is no such layer or more accuraetly no bodies with this layer
	if (layeredBodies.find(layer) == layeredBodies.end()) {
		return false;
	}
	Manifold manifold;
	for (size_t l = 0; l < layeredBodies[layer].size(); l++) {

		if (CheckCollision(*layeredBodies[layer][l]->GetCollider(), *body->GetCollider(), manifold)) {
			return true;
		}
	}
	return false;
}
void PhysicsWorld::UpdateLayerBodies() {
	layeredBodies.clear();
	layers.clear();
	for (size_t i = 0; i < bodies.size(); i++) {
		int l = bodies[i]->layer;
		layeredBodies[l].push_back(bodies[i]);
		layers.push_back(l);
	}
}
void PhysicsWorld::SetGravity(vec3 gravity)
{
	this->gravity = gravity;
	for (PhysicsBody* body : bodies) {
		body->SetGravity(gravity);
	}
}

vec3 PhysicsWorld::GetGravity()
{
	return gravity;
}

void PhysicsWorld::AddBody(PhysicsBody* body) {
	for (PhysicsBody* existingBody : bodies) {
		if (existingBody == body) {
			// Body already exists, so return without adding it again
			return;
		}
	}

	body->SetGravity(gravity);
	body->LinearVelocity(vec2(0,0));
	body->AngularVelocity(0);
	bodies.push_back(body);
}

void PhysicsWorld::RemoveBody(PhysicsBody* body)
{
	for (auto it = bodies.begin(); it != bodies.end(); ++it) {
		if (*it == body) {
			bodies.erase(it);
			return;
		}
	}
}

vector<PhysicsBody*> PhysicsWorld::GetBodies() {
	return bodies;
}

