#include "Boids.h"

static float alignmentWeight = 0.6f;
static float cohesionWeight = 0.1f;
static float separationWeight = 2.0f;
static float vlim = 2.f;

static float norm(const ofVec2f& A, int n = 2) {
	return sqrt(pow(A.x, n) + pow(A.y, n));
}

static ofVec2f max(const ofVec2f& g, const ofVec2f& d) {
	if (norm(g) >= norm(d)) {
		return g;
	}
	else {
		return d;
	}
}

Boids::~Boids()
{
}

void Boids::draw(ofParameter<int> scaler, ofParameter<ofColor> colorer)
{
	glPushMatrix();

	ofPoint p1(4.f, 0.f);
	ofPoint p2(-1.f, -2.f);
	ofPoint p3(-1.f, 2.f);
	scale.set(scaler);
	ofTranslate(coords);
	ofRotate(ofRadToDeg(-(float)atan2(-velocity.y, velocity.x)), 0.f, 0.f, 1.f);
	ofScale(scale);
	//ofSetColor(colorer);
	ofSetColor(color);
	ofDrawTriangle(p1, p2, p3);

	glPopMatrix();

}

void Boids::update_V1(const vector<Boids>& neighbors)
{
	ofVec2f coordsDifference(0);
	ofVec2f velocityDifference(0);

	ofVec2f alignment(0);
	ofVec2f cohesion(0);
	ofVec2f separation(0);

	for (Boids b : neighbors) {

		coordsDifference = b.coords - coords;
		velocityDifference = b.velocity - velocity;

		alignment += velocityDifference;
		cohesion += coordsDifference;

		if (b.coords.distance(coords) < separationDistance) {
			separation += coordsDifference;
		}
	}

	alignment.normalize();
	cohesion.normalize();
	separation.normalize();
	velocity.normalize();

	alignment *= alignmentWeight;
	cohesion *= cohesionWeight;
	separation *= separationWeight;

	acceleration = alignment + cohesion - separation;
	velocity += acceleration;
	velocity *= vlim;
	coords += velocity;
	borders();
}

void Boids::update_V1_enhanced(const vector<Boids>& neighbors)
{
	ofVec2f coordsDifference(0.f);
	ofVec2f velocityDifference(0.f);

	ofVec2f alignment(0.f);
	ofVec2f cohesion(0.f);
	ofVec2f separation(0.f);

	for (Boids b : neighbors) {
		ofVec2f acc(0.0f);

		float distance = b.coords.distance(coords);
		coordsDifference = b.coords - coords;
		velocityDifference = b.velocity - velocity;		

		if (0 < distance < separationDistance) {
			acc += coordsDifference;
			acc.normalize();
			acc /= distance;
			separation += acc;
		}

		if (0 < distance < alignDistance) {
			alignment += velocityDifference;
		}

		if (0 < distance < cohesionDistance) {
			cohesion += coordsDifference;
		}
	}

	// Align
	if (neighbors.size() > 0) {
		alignment /= neighbors.size();
		alignment.normalize();
		alignment *= maxspeed;
		alignment -= velocity;
		alignment.limit(maxforce);

		// Separation
		separation /= neighbors.size();
	}
	else {
		alignment = ofVec2f(0.f);
		separation = ofVec2f(0.f);
	}

	// Cohesion
	cohesion -= coords;
	cohesion.normalize();
	cohesion *= maxspeed;
	cohesion -= velocity;
	cohesion.limit(maxforce);
	

	// Separation
	if (separation.length() > 0) {
		separation.normalize();
		separation *= maxspeed;
		separation -= velocity;
		separation.limit(maxforce);
	}

	velocity.normalize();

	alignment *= alignmentWeight;
	cohesion *= cohesionWeight;
	separation *= separationWeight;

	acceleration = alignment + cohesion - separation;
	velocity += acceleration;
	velocity *= vlim;
	coords += velocity;
	borders();
}

void Boids::update_V2_processing(const vector<Boids>& neighbors, float weight1, float weight2, float weight3)
{
	ofVec2f coordsSum(0.f);
	ofVec2f velocitySum(0.f);

	ofVec2f alignment(0.f);
	ofVec2f cohesion(0.f);
	ofVec2f separation(0.f);
	int count = neighbors.size();

	// ------- SEPA ----------------------------------------------------
	ofVec2f steer1 =  ofVec2f(0.f);
	ofVec2f steer2 = ofVec2f(0.f);
	ofVec2f steer3 = ofVec2f(0.f);
	// For every boid in the system, check if it's too close
	for (Boids other : neighbors) {
		float d = coords.distance(other.coords);
		// If the distance is greater than 0 and less than an arbitrary amount (0 when you are yourself)
		if ((d > 0) && (d < separationDistance)) {
			// Calculate vector pointing away from neighbor
			ofVec2f diff(0.f);
			diff = coords - other.coords;
			diff.normalize();
			diff /= d;        // Weight by distance
			steer1 += diff;
		}

		if ((d > 0) && (d < alignDistance)) {
			velocitySum += other.velocity;
		}

		if ((d > 0) && (d < cohesionDistance)) {
			coordsSum += other.coords;
		}
	}
	// Average -- divide by how many
	if (count > 0) {
		// Separation part
		steer1 /= (float)count;

		// Align part
		velocitySum /= (float)count;
		// Implement Reynolds: Steering = Desired - Velocity
		velocitySum.normalize();
		velocitySum *= maxspeed;
		steer2 = velocitySum - velocity;
		steer2.limit(maxforce);
		alignment = steer2;

		// Cohesion part
		coordsSum /= count;
		ofVec2f desired = coordsSum - coords;  // A vector pointing from the position to the target
														  // Scale to maximum speed
		desired.normalize();
		desired *= maxspeed;

		// Above two lines of code below could be condensed with new PVector setMag() method
		// Not using this method until Processing.js catches up
		// desired.setMag(maxspeed);

		// Steering = Desired minus Velocity
		steer3 = desired - velocity;
		steer3.limit(maxforce);  // Limit to maximum steering force
		cohesion =  steer3;
	}
	else {
		alignment = ofVec2f(0.f);
		cohesion = ofVec2f(0.f);
	}

	// As long as the vector is greater than 0
	if (steer1.length() > 0) {
		// First two lines of code below could be condensed with new PVector setMag() method
		// Not using this method until Processing.js catches up
		// steer.setMag(maxspeed);

		// Implement Reynolds: Steering = Desired - Velocity
		steer1.normalize();
		steer1 *= maxspeed;
		steer1 -= velocity;
		steer1.limit(maxforce);
	}
	separation = steer1;
	// --------------------------------------------------------------------

	separation *= weight1;
	alignment *= weight2;
	cohesion *= weight3;

	// We could add mass here if we want A = F / M
	acceleration = separation + alignment + cohesion;

	// Update velocity
	velocity += acceleration;
	// Limit speed
	velocity.limit(maxspeed);
	coords += velocity;
	// Reset accelertion to 0 each cycle
	acceleration *= 0.f;;
	borders();
}

void Boids::update_V3_conradParker(vector<Boids> population)
{
	ofVec2f coordsMedium(0);
	ofVec2f velocityMedium(0);

	ofVec2f alignment(0);
	ofVec2f cohesion(0);
	ofVec2f separation(0);

	for (Boids b : population) {
		if (&b != this) {
			coordsMedium += b.coords;

			if (b.coords.distance(coords) < 100.f) {
				separation -= b.coords - coords;
			}

			velocityMedium += b.velocity;
		}
	}

	coordsMedium /= population.size() - 1;
	velocityMedium /= population.size() - 1;

	cohesion = coordsMedium *0.01f;
	alignment = velocityMedium * 0.00008f;
	
	alignment.normalize();
	cohesion.normalize();
	separation.normalize();
	velocity.normalize();

	acceleration = alignment + cohesion + separation;
	velocity += acceleration;
	velocity *= vlim;
	coords += velocity;
	borders();
}

void Boids::update_Random(vector<Boids> population)
{
	ofVec2f coordsDifference(0);
	ofVec2f velocityDifference(0);

	ofVec2f alignment(0);
	ofVec2f cohesion(0);
	ofVec2f separation(0);

	for (Boids b : population) {

		coordsDifference = b.coords - coords;
		velocityDifference = b.velocity - velocity;

		alignment += velocityDifference;
		cohesion += coordsDifference;

		if (b.coords.distance(coords) < separationDistance) {
			separation += coordsDifference;
		}
	}

	alignment.normalize();
	cohesion.normalize();
	separation.normalize();
	velocity.normalize();

	alignment *= alignmentWeight;
	cohesion *= cohesionWeight;
	separation *= separationWeight;

	acceleration = ofVec2f(ofRandom(-2.5, 2.5), ofRandom(-2.5, 2.5));
	velocity += acceleration;
	velocity *= vlim;
	coords += velocity;
	borders();
}


vector<Boids> Boids::getNeighbors(vector<Boids> population)
{
	vector<Boids> neighbors;
	
	for (Boids b : population) {
		if (&b != this) {
			float distance = b.coords.distance(coords);
			ofVec2f difference = b.coords - coords;
			if (distance < perceptionField) {
				if (velocity.angle(difference) <= fov) {
					neighbors.push_back(b);
				}
			}
		}
	}
	return neighbors;
}


void Boids::borders() {
	if (coords.x > ofGetWidth()) coords.x = 1;
	if (coords.x <= 0) coords.x = ofGetWidth()-1;
	if (coords.y > ofGetHeight()) coords.y = 1;
	if (coords.y <= 0) coords.y = ofGetHeight()-1;
}


void Boids::limit(ofVec2f& v) {
	
}