#pragma once

#include <utility>
#include <algorithm>

#include "ofMain.h"
#include "ofxGui.h"

#include "Boids.h"
#include "Agent.h"

typedef enum Style{One=1, Two, Three, Four} Style;

template <typename T>
class matrix {

	unsigned int width;
	unsigned int height;
	vector<vector<T>> data;

public:
	matrix(unsigned int height, unsigned int width, vector<T> const& v) noexcept :
		width(width),
		height(height)
	{
		unsigned int k = 0;
		for (unsigned int i = 0; i < height; i++) {
			data.push_back(vector<T>(width, T()));
			for (unsigned int j = 0; j < width; j++) {
				if (k < v.size()) {
					data[i][j] = v[k];
					k++;
				}
			}
		}
	}

	matrix(unsigned int height, unsigned int width, vector<T> &&v) :
		width(width),
		height(height)
	{
		vector<double> tmp(move(v));
		unsigned int k = 0;
		for (unsigned int i = 0; i < height; i++) {
			data.push_back(vector<T>(width, T()));
			for (unsigned int j = 0; j < width; j++) {
				if (k < tmp.size()) {
					data[i][j] = tmp[k];
					k++;
				}
			}
		}
	}

	/* Matrice diagonale avec diag valeur en diagonale et fill valeur pour le reste */
	matrix(unsigned int height, unsigned int width, T diag, T fill) noexcept :
		width(width),
		height(height)
	{
		for (unsigned int i = 0; i < height; i++) {
			data.push_back(vector<T>(width, T()));
			for (unsigned int j = 0; j < width; j++) {
				if (i == j) {
					data[i][j] = diag;
				}
				else {
					data[i][j] = fill;
				}
			}
		}
	}

	/* Matrice d'adjacence graphe connexe avec degre max */
	matrix(unsigned int height, unsigned int width, int max_degree = 5) :
		width(width),
		height(height)
	{
		for (unsigned int i = 0; i < height; i++) {
			data.push_back(vector<T>(width, T()));
		}

		vector<int> random(width, 0);
		std::iota(random.begin(), random.end(), 0);				
		for (unsigned int i = 0; i < height; i++) {
			std::random_shuffle(random.begin(), random.end());
			//cout << '\n' << random << '\n';
			int dm = max_degree;
			vector<int>::iterator u(random.begin());
			//cout << "line " << i << '\n';
			while (u != random.end()) {
				//cout << dm << '\n';
				if (*u != i) {					
					//cout << "iterator value : " << *u << '\n';
					int val1 = std::count(data[i].begin(), data[i].end(), T(1));
					int val2 = std::count(data[*u].begin(), data[*u].end(), T(1));
					if (val1 < max_degree && val2 < max_degree) {
						//cout << "Number of 1 in line" << i  <<":"<< val1 << '\n';
						//cout << "Number of 1 in line" << *u <<":"<< val2 << '\n';
						//cout << "Can set" << '\n';
						data[i][*u] = T(1);
						data[*u][i] = T(1);
					}
				}				
				u++;				
			}
		}
	}

	pair<unsigned int, unsigned int> size() const noexcept {
		return {height, width};
	}

	double get(unsigned int i, unsigned int j) const
	{
		if (i < 0 || i > height || j < 0 || j > width) {
			throw out_of_range("Out of range !");
		}
		return data[i][j];
	}

};

template <typename T>
ostream &operator<<(ostream &os, matrix<T> const &m)
{
	for (unsigned int i = 0; i < get<1>(m.size()); i++) {
		for (unsigned int j = 0; j < get<0>(m.size()); j++) {
			os << " " << m.get(i, j);
		}
		os << endl;
	}
	return os;
}

template <typename T>
ostream &operator<<(ostream &os, vector<T> const &v)
{
	for (unsigned int i = 0; i < v.size(); i++) {
			os << v[i] << " ";
	}
	os << endl;
	return os;
}


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();
		void transition(vector<Agent>& population);
		void transition(vector<Boids>& population);
		void generate(vector<Agent>& population, pair<int, int> number = {5, 5});
		void generate(vector<Boids>& population, int number = 5);
		void drawLink(vector<Agent>& population);

		void keyPressed(int key);
		void keyReleased(int key);
		//void mouseMoved(int x, int y );
		//void mouseDragged(int x, int y, int button);
		//void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseReleasedOnStep();
		void mouseReleasedOnPlay();
		void mouseReleasedOnReset();
		void mouseReleasedOnShow();
		//void mouseEntered(int x, int y);
		//void mouseExited(int x, int y);
		//void windowResized(int w, int h);
		//void dragEvent(ofDragInfo dragInfo);
		//void gotMessage(ofMessage msg);

		int a1, a2;
		int Speed;
		int number_of_transition;
		bool Moved;
		bool Drawn;
		bool Changed;
		bool Determinist;
		bool showTransition;
		string fr;
		string st;
		string ut;
		string dt;;
		string t;;

		Style style;
		vector<Boids> boids;
		vector<Agent> agents;
		vector<ofColor> C;
		vector<ofxLabel> labels;
		
		ofxPanel gui;
		ofxPanel info;
		ofxPanel colorArray;
		ofParameterGroup group;
		ofParameterGroup group1;
		ofParameterGroup group2;
		ofParameterGroup groupInfo1;
		ofParameterGroup groupInfo2;
		ofParameterGroup groupColorArray1;
		ofParameter<ofColor> colorInfo;
		ofParameter<string> flagInfo;
		ofParameter<ofColor> backgroundColorer;
		ofParameter<int> algorithmChooser;
		ofParameter<int> itemScaler;
		ofParameter<int> frameRater;
		ofParameter<int> number;
		ofParameter<string> num_transition;
		ofParameter<string> elaspedTime;
		ofParameter<string> Framerate;
		ofParameter<string> setupTimer;
		ofParameter<string> updateTimer;
		ofParameter<string> drawTimer;

		//// If ofParameter
		ofParameter<bool> step;
		ofParameter<bool> play;
		ofParameter<bool> reset;
		ofParameter<bool> show;

		ofxButton to_step;
		ofxButton to_play;
		ofxButton to_reset;
		ofxButton to_show;

		float timer;
		
};
