#include "PhysicsWorld.h"
/// <summary>
/// Adding a function for cross that takes 2 vector2 
/// </summary>
namespace glm {
	float cross(const glm::vec2& a, const glm::vec2& b) {
		return a.x * b.y - a.y * b.x;
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
			if (!bodies[i]->isStatic) {
				bodies[i]->Step(deltaTime / iterations);
			}
		}
	}

	for (int iter = 0; iter < iterations; iter++)
	{

		for (int i = 0; i < bodies.size(); i++) {
			if (bodies[i]->isTrigger) {
				continue;
			}

			for (int l = i + 1; l < bodies.size(); l++) {
				if (bodies[l]->isTrigger) {
					continue;
				}

				Manifold manifold;

				if (CheckCollision(*bodies[i]->GetCollider(), *bodies[l]->GetCollider(), manifold)) {
					cout << "collision detected" << endl;
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

					SimpleResolution(bodyA, bodyB, manifold);
				}
			}	
		}
		
	}
	
}

void PhysicsWorld::SeparateBodies(PhysicsBody* bodyA, PhysicsBody * bodyB, Manifold manifold) {
	if (!bodyA->isStatic && !bodyB->isStatic) {
		*bodyA->position -= manifold.mtv / 2.0f;
		*bodyB->position += manifold.mtv / 2.0f;
	}
	if (!bodyA->isStatic && bodyB->isStatic) {
		*bodyA->position -= manifold.mtv;
	}
	if (bodyA->isStatic && !bodyB->isStatic) {
		*bodyB->position += manifold.mtv;
	}
}
void PhysicsWorld::RotationResolution(PhysicsBody* bodyA, PhysicsBody* bodyB,Manifold manifold) {

	vec2 normal = manifold.normal;
	vector<vec2> contactPoints = manifold.contactPoints;

	float e = std::min(bodyA->restitution, bodyB->restitution);
	
	vector<vec2> impulses = vector<vec2>();


	vector<vec2> raList = vector<vec2>();
	vector<vec2> rbList = vector<vec2>();


	for (size_t i = 0; i < contactPoints.size(); i++)
	{
		vec2 ra = contactPoints[i] - *bodyA->position;
		vec2 rb = contactPoints[i] - *bodyB->position;
		
		raList.push_back(ra);
		rbList.push_back(rb);

		vec2 raPerp = vec2(-ra.y, ra.x); // Im gonna kill myself
		vec2 rbPerp = vec2(-rb.y, rb.x);
		
		vec2 angularLinearVelocityA = raPerp * bodyA->angularVelocity;
		vec2 angularLinearVelocityB = rbPerp * bodyB->angularVelocity;
	
		//cout << "bV:" << glm::to_string(bodyB->getVelocity()) << "aV:" << glm::to_string(bodyA->getVelocity()) << endl;

		vec2 relativeVel =
			(bodyB->getVelocity() + angularLinearVelocityB) -
			(bodyA->getVelocity() + angularLinearVelocityA);

		//relativeVel is where b is heading
		//normal is where b should go based on collision
		
		float contactVelMag = glm::dot(relativeVel, normal);

		//cout << "relV:" << glm::to_string(relativeVel) << " normal:" << glm::to_string(normal) << endl;

		if (contactVelMag > 0) {
			continue;
		}

		float raPerpDotN = glm::dot(raPerp, normal);
		float rbPerpDotN = glm::dot(rbPerp, normal);

		float denom = (1 / bodyA->mass) + (1 / bodyB->mass) + 0.001f +
			(raPerpDotN * raPerpDotN) * (1 / bodyA->angularInertia) +
			(rbPerpDotN * rbPerpDotN) * (1 / bodyB->angularInertia);

		//cout << "mag:" << contactVelMag << endl;
		float j = -(1.0f + e) * contactVelMag;
		//cout << "j0:" << j << endl;
		j /= denom;
		//cout << "j1:" << j << endl;
		j /= contactPoints.size();
		//cout << "j2:" << j << endl;

		j = -(1 + e) * glm::dot(relativeVel, manifold.normal) / ((bodyA->isStatic ? 0 : 1 / bodyA->mass) + (bodyB->isStatic ? 0 : 1 / bodyB->mass) + 0.000000000001f);

		vec2 impulse = j * normal;

		//cout << "impulse:" << glm::to_string(impulse) << endl;

		impulses.push_back(impulse);
	}


	for (int i = 0; i < impulses.size(); i++)
	{
		vec2 impulse = impulses[i];
		vec2 ra = raList[i];
		vec2 rb = rbList[i];

		bodyA->SetVelocity(bodyA->getVelocity() - impulse / bodyA->mass);
		//bodyA->angularVelocity += -glm::cross(ra, impulse) / bodyA->angularInertia;
		bodyB->SetVelocity(bodyB->getVelocity() + impulse / bodyB->mass);
		//bodyB->angularVelocity += cross(rb, impulse) / bodyB->angularInertia;
	}
}
void PhysicsWorld::SimpleResolution(PhysicsBody* bodyA, PhysicsBody* bodyB, Manifold manifold) {
	vec2 relVel = bodyB->getVelocity() - bodyA->getVelocity();

	vector<vec2> contactPoints = manifold.contactPoints;

	float e = std::min(bodyA->restitution, bodyB->restitution); // coefficient of restitution
	float j = -(1 + e) * glm::dot(relVel, manifold.normal) / ((bodyA->isStatic ? 0 : 1 / bodyA->mass) + (bodyB->isStatic ? 0 : 1 / bodyB->mass) + 0.000000000001f);

	// friction stuff
	vec2 tangent = vec2(-manifold.normal.y, manifold.normal.x);
	vec2 relVelTangent = relVel - glm::dot(relVel, manifold.normal) * manifold.normal;

	float friction = std::min(bodyA->friction, bodyB->friction);
	float jt = -dot(relVelTangent, tangent) / ((bodyA->isStatic ? 0 : 1 / bodyA->mass) + (bodyB->isStatic ? 0 : 1 / bodyB->mass) + 0.000000000001f);
	jt /= contactPoints.size(); // average for multiple contact points

	if (std::abs(jt) > std::abs(j) * friction) {
		jt = std::copysign(std::abs(j) * friction, jt);
	}

	// apply impulse to the bodies
	if (!bodyA->isStatic && !bodyB->isStatic) {
		bodyA->SetVelocity(bodyA->getVelocity() - (j * manifold.normal + jt * tangent) / bodyA->mass);
		bodyB->SetVelocity(bodyB->getVelocity() + (j * manifold.normal + jt * tangent) / bodyB->mass);
	}
	if (!bodyA->isStatic && bodyB->isStatic) {
		bodyA->SetVelocity(bodyA->getVelocity() - (j * manifold.normal + jt * tangent) / bodyA->mass);
	}
	if (bodyA->isStatic && !bodyB->isStatic) {
		bodyB->SetVelocity(bodyB->getVelocity() + (j * manifold.normal + jt * tangent) / bodyB->mass);
	}
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

