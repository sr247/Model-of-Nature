#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	float t1 = ofGetElapsedTimeMillis();
	fr += to_string(ofGetFrameRate());
	number_of_transition = 0;
	generate(agents, {1, 2});

	timer = 0.0f;
	gui.setup();
	gui.setWidthElements(200.0f);
	group.setName("Control");
	//group.add(itemColorer.set("Color", ofColor::blueSteel, ofColor(0, 0, 0), ofColor(255, 255, 255)));
	group.add(backgroundColorer.set("Background", ofColor::black, ofColor(0, 0, 0), ofColor(255, 255, 255)));
	//group2.add(algorithmChooser.set("Style", Style::One, Style::One, Style::Four));
	group2.setName("Misc");
	group2.add(itemScaler.set("Scale", 3, 1, 10));
	group2.add(frameRater.set("Set rate", 60, 1, 80));
	group2.add(number.set("Number of Agents", 6, 2, 200));
	group1.setName("Time");
	//group1.add(elaspedTime.set("Time", t));
	group1.add(Framerate.set("Framrate", fr));
	group1.add(setupTimer.set("Setup time", st));
	group1.add(updateTimer.set("Update time", ut));
	group1.add(drawTimer.set("Draw time", dt));
	group1.add(num_transition.set("Total transition", to_string(number_of_transition)));
	
	group.add(group1);
	group.add(group2);
	gui.setup(group);
	gui.setPosition(ofPoint(ofGetWidth() / 10, ofGetHeight() / 4));
	gui.minimizeAll();
	
	info.setup();
	info.setName("Info");
	groupInfo1.setName("GI1");
	groupInfo2.setName("GI2");
	groupInfo1.add(colorInfo.set("Color", ofColor::black, ofColor(0, 0, 0), ofColor(255, 255, 255)));
	groupInfo1.add(flagInfo.set("N° Color", to_string(C.size())));

	//// If ofParameter
	//groupInfo1.add(step.set("Step", false));
	//groupInfo1.add(play.set("Play", false));
	//groupInfo1.add(reset.set("Reset", false));
	
	// If ofxButton
	to_step.addListener(this, &ofApp::mouseReleasedOnStep);
	to_play.addListener(this, &ofApp::mouseReleasedOnPlay);
	to_reset.addListener(this, &ofApp::mouseReleasedOnReset);
	to_show.addListener(this, &ofApp::mouseReleasedOnShow);
	info.add(to_step.setup("Step", false));
	info.add(to_play.setup("Play", false));
	info.add(to_reset.setup("Reset", false));
	info.add(to_show.setup("Show", true));

	step.set(false);
	play.set(false);
	reset.set(false);
	show.set(true);
	info.add(groupInfo1);
	info.setPosition(ofPoint(ofGetWidth()/10, 3*ofGetHeight()/6));
	
	//colorArray.setup();
	//colorArray.setName("Range of Color");
	//labels = vector<ofxLabel>(C.size(), ofxLabel());
	//for (unsigned int i = 0; i < C.size(); i++) {
	//	//ofParameter<string> label;
	//	labels[i].setBackgroundColor(C[i]);
	//	//label.setup("color " + to_string(i), "");
	//	//groupInfo2.add(label.set("color " + to_string(i)));
	//	colorArray.add(labels[i].setup("color " + to_string(i), ""));
	//}

	ofEnableSmoothing();
	ofSetFrameRate(frameRater);
	
	// Création des Boids
	//generate(boids);
	/*
	for (unsigned int i = 0; i < 300; i++) {
		boids.push_back(Boids());
	}
	int rand = (int)ofRandom(boids.size());
	boids[rand].color = ofColor(200, 50, 50);
	boids[rand].state = true;
	move = true;
	*/
	
	Moved = true;
	Drawn = true;
	Determinist = false;
	Speed = 1;
	float t2 = ofGetElapsedTimeMillis();

	a1 = (int)ofRandom((float)agents.size());
	a2 = (int)ofRandom((float)agents[a1].nigh.size());
	num_transition.set(to_string(number_of_transition));

	st = to_string(t2 - t1) + "/ms";
	setupTimer.set(st);
	t = to_string((t2 - timer)/1000.0f) + "/s";
	elaspedTime.set(t);
}

//--------------------------------------------------------------
void ofApp::update() {
	float t1 = ofGetElapsedTimeMillis();
	if (Moved) {
		// La partie Boids en stand by car ne faisant pas 
		// partie du projet en premier lieu
		// Amélioration++: Trouver comment amélioré les déplacements.
		// --V1 --V3 et --V4
		/*for (unsigned int i = 0; i < boids.size(); i++) {

			if (algorithmChooser == One)
				boids[i].update_V1(boids[i].getNeighbors(boids));
			if (algorithmChooser == Two)
				boids[i].update_V1_enhanced(boids[i].getNeighbors(boids));
			if (algorithmChooser == Three)
				boids[i].update_V2_processing(boids[i].getNeighbors(boids));
			if (algorithmChooser == Four)
				boids[i].update_V3_conradParker(boids[i].getNeighbors(boids));

			// L'algorithme du OU avec distance de com = 50
			// Tansition aléatoire selon équité globale
			transition(boids);
		}*/

		// Partie Protocole de population
		// Agents.
		// Fonction de transition de la class ofApp
		// 2-Hop Coloring 
		try {
			for (auto& a : agents) {
				a.update(agents, itemScaler, C, colorInfo, flagInfo);
			}
		}
		catch (exception e) {
			fstream f("error.txt");
			f << "ofApp::update" << e.what() << '\n';
		}

	}	

	num_transition.set(to_string(number_of_transition));

	// Récupération de timing.
	fr = to_string(ofGetFrameRate());		
	Framerate.set(fr);
	ofSetFrameRate(frameRater);
	float t2 = ofGetElapsedTimeMillis();
	ut = to_string(t2 - t1) + "/ms";
	updateTimer.set(ut);
	t = to_string((t2 - timer)/1000.0f) + "/s";
	elaspedTime.set(t);
}

//--------------------------------------------------------------
void ofApp::draw(){
	float t1 = ofGetElapsedTimeMillis();

	//ofBackground(backgroundColorer);
	ofBackgroundGradient(ofColor::lightGray, ofColor::darkGray);
	// Affichage des Boids
	//for (Boids b : boids) {
	//	b.draw(scaler, colorer);
	//}

	// Affichage du graphe 2Hop-Color
	try {
		transition(agents);
	}
	catch (exception e) {
		fstream f("error.txt");
		f << "ofApp::draw::" << e.what() << '\n';
	}
	drawLink(agents);
	ofSetLineWidth(4.0f);
	for (const Agent& a : agents) {
		a.draw();		
	}

	// Affichage GUI
	gui.draw();
	info.draw();
	colorArray.draw();

	// Récupération de timing.
	float t2 = ofGetElapsedTimeMillis();
	dt = to_string(t2 - t1) + "/ms";
	drawTimer.set(dt);
}

//--------------------------------------------------------------
void ofApp::drawLink(vector<Agent>& population) {
	if (Drawn) {
		for (const Agent& u : population) {		
			ofSetColor(ofColor(ofColor::orange, 64.0f));
			ofSetLineWidth(2.0f);
			for (const shared_ptr<Agent*>& v : u.nigh) {
				glPushMatrix();
				ofDrawLine((*v)->coords, u.coords);
				glPopMatrix();
			}
		}
	}
	
}

//--------------------------------------------------------------
void ofApp::transition(vector<Agent>& population)
{
	int tn = (int)ofGetElapsedTimef();
	if (play) {
		if (tn % Speed == 0) {
			step.set(true);
		}
	}
	if (step) {
		a1 = (int)ofRandom((float)population.size());
		a2 = (int)ofRandom((float)population[a1].nigh.size());
	}

	Agent& u = population[a1];
	Agent& v = **population[a1].nigh[a2];

	if (step) {
		number_of_transition++;
		if (u.F[v.flag] != v.F[u.flag]) {
			if (!Determinist) {
				u.flag = (int)ofRandom((float)C.size());
			}else {

			}
			u.F[v.flag] = v.F[u.flag];
			Changed = true;
		}
		else{
			u.F[v.flag] = !u.F[v.flag];
			v.F[u.flag] = !v.F[u.flag];
			Changed = false;
		}
	}
	step.set(false);
	u.color.set(C[u.flag]);
	v.color.set(C[v.flag]);

	if(show){
		if (Changed) {
			glPushMatrix();
			ofNoFill();
			ofSetColor(ofColor::green);
			ofDrawCircle(u.coords, u.radius + 20);
			ofSetColor(ofColor::red);
			ofDrawCircle(v.coords, v.radius + 20);
			glPopMatrix();
		}

		ofSetLineWidth(2.0f);
		glPushMatrix();
		ofSetColor(ofColor::red);
		ofDrawLine(u.coords, v.coords);
		glPopMatrix();
		ofSetColor(255);
	}
	
}

//--------------------------------------------------------------
void ofApp::transition(vector<Boids>& population)
{
	int j = (int)ofRandom((float)boids.size());
	int k = (int)ofRandom((float)boids.size());
	if (boids[j].coords.distance(boids[k].coords) < 50) {
		if (boids[j].state == true) {
			boids[k].color = boids[j].color;
			boids[k].state = boids[k].state || boids[k].state;
		}
		if (boids[k].state == true) {
			boids[j].color = boids[k].color;
			boids[j].state = boids[j].state || boids[k].state;
		}
	}
}

void ofApp::generate(vector<Agent>& population, pair<int, int> number)
{
	try {
		int dval = 3;
		population = vector<Agent>();
		// Memory optimisation
		//population.shrink_to_fit();
		// Calcul of Max degree and Adjacency matrix
		int max_degree = number.first*number.second - 1 < dval ? number.first*number.second - 1 : dval;
		matrix<int> D(number.first*number.second, number.first*number.second, max_degree);
		// Calcul of Color vector of size g = d * (d-1) + 1
		C = vector<ofColor>(max_degree*(max_degree - 1) + 1, ofColor(128, 128, 128));
		// Setting g arbitrary color with Hue coloring system
		for (int i = 0; i < C.size(); i++) {
			C[i].setHsb(i * 360 / C.size(), 150, 160);
		}

		//cout << C.size() << "\n\n";
		// Pushing new agents with their flag and F vector
		for (unsigned int i = 0; i < number.first*number.second; i++) {
			population.push_back(Agent());
			population[i].flag = (int)ofRandom((float)C.size());
			population[i].F = vector<bool>(max_degree*(max_degree - 1) + 1, false);
			//population[i].F[agents[i].flag] = true;
			//cout << population[i].F.size() << '\n';
		}

		// Setting the neighbourhood of each agents
		int size = agents.size();
		int k = 0, l = 0;
		for (unsigned int i = 0; i < D.size().first; i++) {
			for (unsigned int j = 0; j < D.size().second; j++) {
				if (i != j) {
					if (D.get(i, j) == 1) {
						population[l].nigh
							.push_back(make_shared<Agent*>(&population[k]));
					}
				}
				k++;
			}
			l++;
			k = 0;
		}
		cout << D;

		// Le barycentre n'est pas la solution rechercher:
		// Trouvé un algo d'équidistance générale
		// Trying to set the position to agents in an equidistant way
		for (Agent& a : population) {
			a.average();
		}
	
		play.set(false);
		step.set(false);

		a1 = (int)ofRandom((float)population.size());
		a2 = (int)ofRandom((float)population[a1].nigh.size());
		number_of_transition = 0;
	}catch (exception e){
		fstream f("error.txt");
		f << "ofApp::generate" << e.what() << '\n';
	}
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == GLFW_KEY_SPACE) {
		Moved = !Moved;
	}
	if (key == GLFW_KEY_D) {
		Drawn = !Drawn;
	}
	if (key == GLFW_KEY_R) {
		//generate(agents, {(int)ofRandom(4) + 1, (int)ofRandom(4) + 1 });
		generate(agents, {1, number});
	}
	if (key == OF_KEY_RIGHT) {
		step.set(true);
	}

}

void ofApp::mouseReleasedOnStep() {
	step.set(!step);
}

void ofApp::mouseReleasedOnPlay() {
	play.set(!play);
	if (play.getName() == "Play") {
		to_play.setName("Pause");
	}
	else {
		to_play.setName("Play");
	}
}

void ofApp::mouseReleasedOnReset() {
	generate(agents, { 1, number });
	timer = ofGetElapsedTimeMillis();
	elaspedTime.set(to_string(timer));
}

void ofApp::mouseReleasedOnShow() {
	show.set(!show);
}

//--------------------------------------------------------------
void ofApp::exit() {
	to_step.removeListener(this, &ofApp::mouseReleasedOnStep);
	to_play.removeListener(this, &ofApp::mouseReleasedOnPlay);
	to_reset.removeListener(this, &ofApp::mouseReleasedOnReset);
}
////--------------------------------------------------------------
//void ofApp::mouseMoved(int x, int y ){
//
//}
//
////--------------------------------------------------------------
//void ofApp::mouseDragged(int x, int y, int button){
//
//}


////--------------------------------------------------------------
//void ofApp::mousePressed(int x, int y, int button){
//
//}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

////--------------------------------------------------------------
//void ofApp::mouseEntered(int x, int y){
//
//}
//
////--------------------------------------------------------------
//void ofApp::mouseExited(int x, int y){
//
//}
//
////--------------------------------------------------------------
//void ofApp::windowResized(int w, int h){
//
//}
//
////--------------------------------------------------------------
//void ofApp::gotMessage(ofMessage msg){
//
//}
//
////--------------------------------------------------------------
//void ofApp::dragEvent(ofDragInfo dragInfo){ 
//
//}
