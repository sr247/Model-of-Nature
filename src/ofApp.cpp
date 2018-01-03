#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	float t1 = ofGetElapsedTimeMillis();

	fr += to_string(ofGetFrameRate());
	gui.setup();
	group.setName("Control");
	//group.add(itemColorer.set("Color", ofColor::blueSteel, ofColor(0, 0, 0), ofColor(255, 255, 255)));
	group.add(backgroundColorer.set("Background", ofColor::black, ofColor(0, 0, 0), ofColor(255, 255, 255)));
	//group2.add(algorithmChooser.set("Style", Style::One, Style::One, Style::Four));
	group2.setName("Misc");
	group2.add(itemScaler.set("Scale", 2, 1, 10));
	group2.add(frameRater.set("Set rate", 60, 1, 80));
	group1.setName("Time");
	group1.add(elaspedTime.set("Time", t));
	group1.add(Framerate.set("Framrate", fr));
	group1.add(setupTimer.set("Setup time", st));
	group1.add(updateTimer.set("Update time", ut));
	group1.add(drawTimer.set("Draw time", dt));
	
	group.add(group1);
	group.add(group2);
	gui.setup(group);
	gui.minimizeAll();
	ofEnableSmoothing();
	ofSetFrameRate(frameRater);
	
	// Création des Boids
	/*
	for (unsigned int i = 0; i < 300; i++) {
			boids.push_back(Boids());
	}
	int rand = (int)ofRandom(boids.size());
	boids[rand].color = ofColor(200, 50, 50);
	boids[rand].state = true;
	move = true;
	*/
	
	isMoved = true;
	isDrawn = true;
	int w = 7;
	int h = 1;
	int max_degree = w*h - 1 < 5 ? w*h - 1 : 5;
	matrix<int> D(w*h, h*w, max_degree);
	C = vector<ofColor>(max_degree*(max_degree - 1) + 1, ofColor(128, 128, 128));
	for (int i = 0; i < C.size(); i ++) {
		C[i].setHsb(i * 360 / C.size(), 150, 160);
	}
	//cout << C.size() << "\n\n";
	for (unsigned int i = 0; i < w*h; i++) {
		population.push_back(Agent());
		population[i].flag = (int)ofRandom((float)C.size());
		population[i].F = vector<bool>(max_degree*(max_degree - 1) + 1, false);
		population[i].F[population[i].flag] = true;
		//cout << population[i].F.size() << '\n';
	}

	int size = population.size();
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
	for (Agent& a : population) {
		a.average();
	}

	float t2 = ofGetElapsedTimeMillis();

	st = to_string(t2 - t1);
	setupTimer.set(st);
	t = to_string(t2/1000.0f) + "/s";
	elaspedTime.set(t);
}

//--------------------------------------------------------------
void ofApp::update() {
	float t1 = ofGetElapsedTimeMillis();
	if (isMoved) {
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
		for (auto& a : population) {
			//population[i].update(population, itemScaler, itemColorer);
			a.update(population, itemScaler, C);
		}
	}	

	// Récupération de timing.
	fr = to_string(ofGetFrameRate());		
	Framerate.set(fr);
	ofSetFrameRate(frameRater);
	float t2 = ofGetElapsedTimeMillis();
	ut = to_string(t2 - t1);
	updateTimer.set(ut);
	t = to_string(t2/1000.0f) + "/s";
	elaspedTime.set(t);
}

//--------------------------------------------------------------
void ofApp::draw(){
	float t1 = ofGetElapsedTimeMillis();

	ofBackground(backgroundColorer);

	// Affichage des Boids
	//for (Boids b : boids) {
	//	b.draw(scaler, colorer);
	//}

	// Affichage du graphe 2Hop-Color
	transition(population);
	drawLink(population);
	ofSetLineWidth(4.0f);
	for (const Agent& a : population) {
		a.draw();		
	}

	// Affichage GUI
	gui.draw();

	// Récupération de timing.
	float t2 = ofGetElapsedTimeMillis();
	dt = to_string(t2 - t1);
	drawTimer.set(dt);
}

//--------------------------------------------------------------
void ofApp::drawLink(vector<Agent>& population) {
	if (isDrawn) {
		for (const Agent& u : population) {		
			ofSetColor(ofColor(ofColor::orange, 32.0f));
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

	int i = (int)ofRandom((float)population.size());
	int j = (int)ofRandom((float)population[i].nigh.size());

	Agent& u = population[i];
	Agent& v = **population[i].nigh[j];

	if (isMoved) {
		if (u.F[v.flag] != v.F[u.flag]) {
			glPushMatrix();
			ofSetColor(ofColor::red);
			ofNoFill();
			ofDrawCircle(u.coords, u.radius + 20);
			ofDrawCircle(v.coords, u.radius + 20);
			glPopMatrix();

			u.flag = (int)ofRandom((float)u.F.size());
			u.F[v.flag] = v.F[u.flag];
		}
		else{
			u.F[v.flag] = u.F[u.flag];
			v.F[u.flag] = v.F[v.flag];
		}

		ofSetLineWidth(2.0f);
		for (const shared_ptr<Agent*>& a : u.nigh) {
			glPushMatrix();
			if (**a == v) {
				ofSetColor(ofColor::red);
				ofDrawLine((*a)->coords, u.coords);
			}
			glPopMatrix();
		}
	
	}
	
	ofSetColor(255);
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


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == GLFW_KEY_SPACE) {
		isMoved = !isMoved;
	}
	if (key == GLFW_KEY_D) {
		isDrawn = !isDrawn;
	}
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
//
////--------------------------------------------------------------
//void ofApp::mousePressed(int x, int y, int button){
//
//}
//
////--------------------------------------------------------------
//void ofApp::mouseReleased(int x, int y, int button){
//
//}
//
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
