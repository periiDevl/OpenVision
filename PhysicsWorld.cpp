#include "PhysicsWorld.h"
/// <summary>
/// Adding a function for cross that takes 2 vector2 
/// </summary>
namespace glm {
	float cross(const vec2& a, const vec2& b) {
		return a.x * b.y - a.y * b.x;
	}
	vec2 cross(const vec2& a, float s) {
		return vec2(s * a.y, -s * a.x);

	}
	vec2 cross(float s, const vec2& a)
	{
		return vec2(-s * a.y, s * a.x);

	}
	bool nearEqual(const vec2& a, const vec2& b) {
		return glm::all(glm::epsilonEqual(a, b, 0.000001f));
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

				if (bodies[i]->IsStatic() && bodies[l]->IsStatic())
					continue;

				Manifold manifold;

				if (CollisionManager::CheckCollision(*bodies[i]->GetCollider(), *bodies[l]->GetCollider(), manifold)) 
				{
					if (nearEqual(manifold.mtv, vec2(0))) 
					{
						continue;
					}

					//cout << "mtv:" << glm::to_string(manifold.mtv) << endl;
					//cout << "vel A:" << glm::to_string(bodies[i]->LinearVelocity()) << " vel B:" << glm::to_string(bodies[l]->LinearVelocity()) << endl;

					PhysicsBody* bodyA = bodies[i];
					PhysicsBody* bodyB = bodies[l];

					//cout << "rotation \nA:" << *bodyA->rotation << " \nB:" << *bodyB->rotation << endl;
					SeparateBodies(bodyA, bodyB, manifold);

					PolygonCollider& polyA = dynamic_cast<PolygonCollider&>(*bodyA->GetCollider());
					PolygonCollider& polyB = dynamic_cast<PolygonCollider&>(*bodyB->GetCollider());
					CollisionManager::GetContactPointsPolyVPoly(polyA, polyB, manifold);
					vector<vec2> contactPoints = manifold.contactPoints;

					//cout << "start friction" << endl;

					FrictionResolution(bodyA, bodyB, manifold);
				}
			}	
		}
		
	}
	
}

void PhysicsWorld::SeparateBodies(PhysicsBody* bodyA, PhysicsBody * bodyB, Manifold manifold) {
	bool staticA = bodyA->IsStatic();
	bool staticB = bodyB->IsStatic();

	if (!staticA && !staticB) {
		*bodyA->position -= manifold.mtv * 0.5f;
		*bodyB->position += manifold.mtv * 0.5f;
	}
	else if (!staticA && staticB) {
		*bodyA->position -= manifold.mtv;
	}
	else if (staticA && !staticB) {
		*bodyB->position += manifold.mtv;
	}
}
void PhysicsWorld::FrictionResolution(PhysicsBody* bodyA, PhysicsBody* bodyB, Manifold manifold) {

	vec2 normal = manifold.normal;
	vector<vec2> contactPoints = manifold.contactPoints;

	float e = std::min(bodyA->restitution, bodyB->restitution);

	vector<vec2> raList = vector<vec2>();
	vector<vec2> rbList = vector<vec2>();

	float sf = (bodyA->staticFric + bodyB->staticFric) * 0.5f;
	float df = (bodyA->dynamicFric + bodyB->dynamicFric) * 0.5f;

	vector<vec2> impulses = vector<vec2>();
	vector<vec2> frictionImpulses = vector<vec2>();
	vector<float> jList = vector<float>();

	for (size_t i = 0; i < contactPoints.size(); i++) {
		vec2 ra = contactPoints[i] - *bodyA->position;
		raList.push_back(ra);
		vec2 rb = contactPoints[i] - *bodyB->position;
		rbList.push_back(rb);


		vec2 raPerp = vec2(-ra.y, ra.x);
		vec2 rbPerp = vec2(-rb.y, rb.x);

		vec2 angularLinearVelA = raPerp * bodyA->AngularVelocity();
		vec2 angularLinearVelB = rbPerp * bodyB->AngularVelocity();

		vec2 relativeVel =
			(bodyB->LinearVelocity() + angularLinearVelB)
			- (bodyA->LinearVelocity() + angularLinearVelA);


		float velAlongNormal = dot(relativeVel, normal);

		if (velAlongNormal > 0.0f)
			continue;

		float raPerpDotN = dot(raPerp, normal);
		float rbPerpDotN = dot(rbPerp, normal);

		float invMassSum = (bodyA->InvMass() + bodyB->InvMass()) +
			((raPerpDotN * raPerpDotN) * bodyA->InvInertia()) +
			((rbPerpDotN * rbPerpDotN) * bodyB->InvInertia());
		

		float j = -(1.0f + e) * velAlongNormal;
		j /= invMassSum;
		j /= (float)contactPoints.size();

		jList.push_back(j);

		vec2 impulse = normal * j;

		impulses.push_back(impulse);
	}

	for (size_t i = 0; i < impulses.size(); i++)
	{
		vec2 impulse = impulses[i];
		vec2 ra = raList[i];
		vec2 rb = rbList[i];

		if (!bodyA->IsStatic()) {
			bodyA->AddLinearVelocity(-impulse * bodyA->InvMass());
			bodyA->AddAngularVelocity(-glm::cross(ra, impulse) * bodyA->InvInertia());
		}

		if (!bodyB->IsStatic()) {
			bodyB->AddLinearVelocity(impulse * bodyB->InvMass());
			bodyB->AddAngularVelocity(glm::cross(rb, impulse) * bodyB->InvInertia());
		}
	}

	// if there is no impulse there is nothing to fricitonize
	if (impulses.size() == 0)
	{
		return;
	}

	raList.clear();
	rbList.clear();
	
	for (size_t i = 0; i < contactPoints.size(); i++) {
		vec2 ra = contactPoints[i] - *bodyA->position;
		raList.push_back(ra);
		vec2 rb = contactPoints[i] - *bodyB->position;
		rbList.push_back(rb);


		vec2 raPerp = vec2(-ra.y, ra.x);
		vec2 rbPerp = vec2(-rb.y, rb.x);

		vec2 angularLinearVelA = raPerp * bodyA->AngularVelocity();
		vec2 angularLinearVelB = rbPerp * bodyB->AngularVelocity();

		vec2 relativeVel =
			(bodyB->LinearVelocity() + angularLinearVelB)
			- (bodyA->LinearVelocity() + angularLinearVelA);


		vec2 tangent = relativeVel - dot(relativeVel, normal) * normal;

		// nearly equal
		if (abs(tangent.x) < 0.005 && abs(tangent.y) < 0.005)
		{
			continue;
		}
		else
		{
			tangent = normalize(tangent);
		}

		float raPerpDotT = dot(raPerp, tangent);
		float rbPerpDotT = dot(rbPerp, tangent);

		float invMassSum = (bodyA->InvMass() + bodyB->InvMass()) +
			((raPerpDotT * raPerpDotT) * bodyA->InvInertia()) +
			((rbPerpDotT * rbPerpDotT) * bodyB->InvInertia());


		float jt = -dot(relativeVel, tangent);
		jt /= invMassSum;
		jt /= (float)contactPoints.size();

		vec2 frictionImpulse = vec2(0);

		if (abs(jt) <= jList[i] * sf)
		{
			frictionImpulse = jt * tangent;
		}
		else
		{
			frictionImpulse = -jList[i] * tangent * df;
		}

		frictionImpulses.push_back(frictionImpulse);
	}

	for (size_t i = 0; i < frictionImpulses.size(); i++)
	{
		vec2 frictionImpulse = frictionImpulses[i];
		vec2 ra = raList[i];
		vec2 rb = rbList[i];

		if (!bodyA->IsStatic()) {
			bodyA->AddLinearVelocity(-frictionImpulse * bodyA->InvMass());
			bodyA->AddAngularVelocity(-glm::cross(ra, frictionImpulse) * bodyA->InvInertia());
		}

		if (!bodyB->IsStatic()) {
			bodyB->AddLinearVelocity(frictionImpulse * bodyB->InvMass());
			bodyB->AddAngularVelocity(glm::cross(rb, frictionImpulse) * bodyB->InvInertia());
		}
	}
}
void PhysicsWorld::RotationResolution(PhysicsBody* bodyA, PhysicsBody* bodyB, Manifold manifold) {

	vec2 normal = manifold.normal;
	vector<vec2> contactPoints = manifold.contactPoints;

	float e = std::min(bodyA->restitution, bodyB->restitution);
	
	vector<vec2> raList = vector<vec2>();
	vector<vec2> rbList = vector<vec2>();
	vector<vec2> impulses = vector<vec2>();
	vector<vec2> frictionImpulses = vector<vec2>();
	vector<float> jList = vector<float>();

	cout << "bodyA pos:" << glm::to_string(*bodyA->position) << endl
		<< "bodyB pos:" << glm::to_string(*bodyB->position) << endl;
	for (size_t i = 0; i < contactPoints.size(); i++) {
		vec2 ra = contactPoints[i] - *bodyA->position;
		raList.push_back(ra);
		vec2 rb = contactPoints[i] - *bodyB->position;
		rbList.push_back(rb);


		vec2 raPerp = vec2(-ra.y, ra.x);
		vec2 rbPerp = vec2(-rb.y, rb.x);

		vec2 angularLinearVelA = raPerp * bodyA->AngularVelocity();
		vec2 angularLinearVelB = rbPerp * bodyB->AngularVelocity();

		vec2 relativeVel =
			(bodyB->LinearVelocity() + angularLinearVelB)
			- (bodyA->LinearVelocity() + angularLinearVelA);


		float velAlongNormal = dot(relativeVel, normal);

		if (velAlongNormal > 0.0f)
			continue;

		float raPerpDotN = dot(raPerp, normal);
		float rbPerpDotN = dot(rbPerp, normal);

		float invMassSum = (bodyA->InvMass() + bodyB->InvMass()) +
			((raPerpDotN * raPerpDotN) * bodyA->InvInertia()) +
			((rbPerpDotN * rbPerpDotN) * bodyB->InvInertia());

		cout << "contact point:" << glm::to_string(contactPoints[i]) << endl
			<< "bodyA + bodyB:" << (bodyA->InvMass() + bodyB->InvMass()) << endl
			<< "first part:" << ((raPerpDotN * raPerpDotN) * bodyA->InvInertia()) << endl
			<< "second part:" << ((rbPerpDotN * rbPerpDotN) * bodyB->InvInertia()) << endl
			<< "raPerp:" << glm::to_string(raPerp) << endl
			<< "rbPerp:" << glm::to_string(rbPerp) << endl
			<< "normal:" << glm::to_string(normal) << endl
			<< "inv mass A:" << bodyA->InvMass() << " normal mass:" << bodyA->Mass() << endl
			<< "inv mass B:" << bodyB->InvMass() << " normal mass:" << bodyB->Mass() << endl
			<< "inv inertia A:" << bodyA->InvInertia() << " inertia A:" << bodyA->Inertia() << endl
			<< "inv inertia B:" << bodyB->InvInertia() << " inertia B:" << bodyB->Inertia() << endl
			<< "raCrossN sqr:" << (raPerpDotN * raPerpDotN) << endl
			<< "inv mass sum:" << invMassSum << endl;



		float j = -(1.0f + e) * velAlongNormal;
		j /= invMassSum;
		j /= (float)contactPoints.size();

		jList.push_back(j);

		vec2 impulse = normal * j;
		cout << "impulse:" << glm::to_string(-impulse) << endl;

		impulses.push_back(impulse);
	}

	for (size_t i = 0; i < impulses.size(); i++)
	{
		vec2 impulse = impulses[i];
		vec2 ra = raList[i];
		vec2 rb = rbList[i];

		if (!bodyA->IsStatic()) {
			cout << "BEFORE" << endl << "body a, vel:" << glm::to_string(bodyA->LinearVelocity()) << " angular vel:" << bodyA->AngularVelocity() << endl;
			bodyA->AddLinearVelocity(-impulse * bodyA->InvMass());
			bodyA->AddAngularVelocity(-glm::cross(ra, impulse) * bodyA->InvInertia());
			cout << "AFTER" << endl << "body a, vel:" << glm::to_string(bodyA->LinearVelocity()) << " angular vel:" << bodyA->AngularVelocity() << endl;
		}

		if (!bodyB->IsStatic()) {
			cout << "BEFORE" << endl << "body b, vel:" << glm::to_string(bodyB->LinearVelocity()) << " angular vel:" << bodyB->AngularVelocity() << endl;
			bodyB->AddLinearVelocity(impulse * bodyB->InvMass());
			bodyB->AddAngularVelocity(glm::cross(rb, impulse) * bodyB->InvInertia());
			cout << "AFTER" << endl << "body b, vel:" << glm::to_string(bodyB->LinearVelocity()) << " angular vel:" << bodyB->AngularVelocity() << endl;
		}
	}


}
void PhysicsWorld::SimpleResolution(PhysicsBody* bodyA, PhysicsBody* bodyB, Manifold manifold) {
	vec2 relVel = bodyB->LinearVelocity() - bodyA->LinearVelocity();

	vector<vec2> contactPoints = manifold.contactPoints;

	float e = std::min(bodyA->restitution, bodyB->restitution); 
	
	float j = -(1 + e) * glm::dot(relVel, manifold.normal) / (bodyA->InvMass() + bodyB->InvMass() + 0.0001f);

	// friction stuff
	vec2 tangent = vec2(-manifold.normal.y, manifold.normal.x);
	vec2 relVelTangent = relVel - glm::dot(relVel, manifold.normal) * manifold.normal;
	
	float friction = std::min(bodyA->dynamicFric, bodyB->dynamicFric);
	float jt = -dot(relVelTangent, tangent) / ((bodyA->InvMass()) + (bodyB->InvMass()) + 0.000000000001f);
	jt /= contactPoints.size(); // average for multiple contact points
	
	if (std::abs(jt) > std::abs(j) * friction) {
		jt = std::copysign(std::abs(j) * friction, jt);
	}
	// no friction for debugging
	jt = 0;
	         
	
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

		if (CollisionManager::CheckCollision(*layeredBodies[layer][l]->GetCollider(), *body->GetCollider(), manifold)) {
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
	body->area = body->scale->x * body->scale->y;
	body->Mass(body->area * body->density);

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

