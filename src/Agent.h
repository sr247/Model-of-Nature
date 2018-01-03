#pragma once
#include "ofMain.h"

class Agent
{
public:
	int state;
	int flag;
	int cpt;

	bool onHoover;
	bool onClick;

	ofPoint coords;
	ofVec2f velocity;
	ofVec2f acceleration;
	float radius;

	ofColor color;
	ofColor hooverColor;
	ofColor clickColor;
	ofPoint scale;

	vector<bool> F;
	vector<shared_ptr<Agent*>> nigh;

public:
	Agent() : coords(ofRandom(0.f, ofGetWidth()), ofRandom(0.f, ofGetHeight())),
		color(ofColor::blueSteel),
		hooverColor(ofColor::greenYellow),
		clickColor(ofColor::yellowGreen),
		velocity(ofRandom(-1.0f, 1.0f), ofRandom(-1.0f, 1.0f)),
		acceleration(0.0f),
		radius(5.0f),
		onHoover(false),
		onClick(false)
	{
	}
	~Agent();

	void draw() const;
	void update(const vector<Agent>& population, const ofParameter<int>& scaler, const ofParameter<ofColor>& colorer = ofParameter<ofColor>());
	void update(const vector<Agent>& population, const ofParameter<int>& scaler, const vector<ofColor>& C);
	void borders();
	void average();
	ofPoint barycentre(vector<shared_ptr<Agent*>> agents, float threshold = 2.0f);
	ofPoint barycentre(vector<Agent> agents, float threshold = 2.0f);
};


bool operator==(Agent const &a, Agent const &b);
