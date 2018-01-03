#pragma once

#include "ofMain.h"

class Boids
{
public:	

	bool state;
	int flag;
	int cpt;

	ofPoint coords;
	ofVec2f velocity;
	ofVec2f acceleration;

	ofColor color;
	ofPoint scale;

	float perceptionField;
	float separationDistance;
	float alignDistance;
	float cohesionDistance;
	float fov;
	float isoscale;

	float maxspeed;
	float maxforce;



	// Constructor / Destructor
	Boids(bool state = false, int flag = 0, int cpt = 0) :
		coords(ofRandom(0.f, ofGetWidth()), ofRandom(0.f, ofGetHeight())),
		color(ofColor::blueSteel),
		velocity(ofRandom(-2.5f, 2.5f), ofRandom(-2.5f, 2.5f)),
		acceleration(0.0f),
		isoscale(1.0f),
		scale(isoscale, isoscale)
	{
		perceptionField = 100.0f;
		separationDistance = 25.f;
		alignDistance = 50.f;
		cohesionDistance = 50.f;

		fov = 180;
		maxspeed = 2.0f;
		maxforce = 0.03f;
		this->state = state;
		this->flag = flag;
		this->cpt = cpt;

	};

	Boids(ofVec2f p) : coords(p), color(255) {};
	Boids(float x, float y) : coords(x, y), color(255) {};
	~Boids();

	//Fonctions
	void draw(ofParameter<int> scaler = ofParameter<int>(), ofParameter<ofColor> colorer = ofParameter<ofColor>());
	void update_V1(const vector<Boids>& neightbors);
	void update_V1_enhanced(const vector<Boids>& neightbors);
	void update_V2_processing(const vector<Boids>& neighbors, float weight1 = 1.5f, float weight2 = 1.0f, float weight3 = 1.0f);
	void update_V3_conradParker(vector<Boids> population);
	void update_Random(vector<Boids> population);
	void borders();
	void limit(ofVec2f& v);
	vector<Boids> getNeighbors(vector<Boids> population);
};

