#include "Agent.h"




Agent::~Agent()
{
}

void Agent::draw() const
{
	ofSetColor(255);

	glPushMatrix();
	if (onClick) {
		ofSetColor(clickColor);
	}
	else {
		ofSetColor(color);
	}
	ofFill();
	ofDrawCircle(coords, radius);
	ofNoFill();
	if (onHoover) {
		ofSetColor(hooverColor);
	}

	ofDrawCircle(coords, radius + 5);
	glPopMatrix();
}

// Update Couleur paramètrée
void Agent::update(const vector<Agent>& population, const ofParameter<int>& scaler, const ofParameter<ofColor>& colorer)
{

	radius = scaler.get() * 5.0f;
	color.set(colorer);

	float val = 0.01f;
	float epsilon = 2.0f;
	//coords.x += (ofRandom(-val, val)); // cos ?
	//coords.y += (ofRandom(-val, val)); // sin ?		
	velocity.x = (ofRandom(-val, val)); // cos ?
	velocity.y = (ofRandom(-val, val)); // sin ?

	for (const Agent& a : population) {
		if (a.coords.distance(coords) <= (a.radius + radius) * epsilon) {
			coords += (coords - a.coords).getLimited(1.0f);
		}
		else {
			coords += (a.coords - coords).getLimited(0.0001f);
		}
	}

	borders();
	coords += velocity.getNormalized();
}

// Update Couleur déterminé par l'algo
void Agent::update(const vector<Agent>& population, const ofParameter<int>& scaler, const vector<ofColor>& C, ofParameter<ofColor>& info, ofParameter<string>& infoColor)
{
	ofPoint mouse(ofGetMouseX(), ofGetMouseY());
	if (coords.distance(mouse) <= radius + 5.0f) {
		onHoover = true;
		onClick = ofGetMousePressed();
	}
	else
	{
		onClick = false;
		onHoover = false;
	}
	
	radius = scaler.get() * 5.0f;
	color.set(C[flag]);

	float val = 0.01f;
	float epsilon = 6.0f;
	//coords.x += (ofRandom(-val, val)); // cos ?
	//coords.y += (ofRandom(-val, val)); // sin ?		
	velocity.x = (ofRandom(-val, val)); // cos ?
	velocity.y = (ofRandom(-val, val)); // sin ?
	

	for (const Agent& a : population) {
		if (a.coords.distance(coords) <= (a.radius + radius) * epsilon) {
			coords += (coords - a.coords).getLimited(1.0f);
		}
		else {
			coords += (a.coords - coords).getLimited(0.0001f);
		}
	}	

	borders();
	if (!onClick) {
		coords += velocity.getNormalized();
	}
	else {
		coords = mouse;
		info.set(color);
		infoColor.set(to_string(flag));
	}
	if (isActive) {
		info.set(color);
	}
}

void Agent::borders() {
	float lim = 2.0f * radius;
	float k = 2.0f;
	if (coords.x >= ofGetWidth() - lim) {
		coords.x -= k;
		//velocity.x = -velocity.x*k;
	}
	if (coords.x <= 0 + lim) {
		coords.x += k;
		//velocity.x = -velocity.x*k;
	}
	if (coords.y >= ofGetHeight() - lim) {
		coords.y -= k;
		//velocity.y = -velocity.y*k;
	}
	if (coords.y <= 0 + lim) {
		coords.y += k;
		//velocity.y = -velocity.y*k;
	}
}

void Agent::average()
{	
	// Methode 1:
	/*
	ofVec3f* points = new ofVec3f[nigh.size()+1];
	for (unsigned int i = 0; i < nigh.size(); i++) {
		points[i] = ofVec3f((*nigh[i])->coords);
	}
	points[nigh.size()] = coords;
	ofPoint p(ofPoint::average(points, nigh.size()));
	coords = p;
	delete points;
	*/

	// Methode 2:
	//float average_distance(0.0f);
	//ofVec3f v(0.0f);
	//for (const auto& a : nigh) {
	//	average_distance += coords.distance((*a)->coords);
	//	v += coords - (*a)->coords;
	//}
	//average_distance /= nigh.size();
	//v /= average_distance;
	//coords.set(v);

	coords = barycentre(nigh);
}


ofPoint Agent::barycentre(vector<shared_ptr<Agent*>> agents, float threshold)
{
	ofPoint medion;
	for (const auto& a : agents) {
		medion.x += /*cos(coords.distance((*a)->coords)*threshold) * */(*a)->coords.x;
		medion.y += /*sin(coords.distance((*a)->coords)*threshold) * */(*a)->coords.y;
	}
	medion /= agents.size();
	return medion;
}

ofPoint Agent::barycentre(vector<Agent> agents, float threshold)
{
	ofPoint medion;
	for (const auto& a : agents) {
		medion.x += /*cos(coords.distance((*a)->coords)*threshold) * */a.coords.x;
		medion.y += /*sin(coords.distance((*a)->coords)*threshold) * */a.coords.y;
	}
	medion /= agents.size();
	return medion;
}

bool operator==(Agent const &a, Agent const &b) {
	if (&a == &b)
		return true;
	return false;
}