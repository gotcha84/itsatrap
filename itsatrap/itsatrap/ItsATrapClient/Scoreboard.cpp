//Scoreboard.cpp
#include "Scoreboard.h"
#include <GL/glut.h>
#include <fstream>
#include <algorithm>
using namespace std;

Scoreboard::Scoreboard() {

}

Scoreboard::~Scoreboard() {

}

int Scoreboard::addEntry(Entry const& entry) {

	if (existingPlayer(entry)) {
		auto loc = entryWorseThan(entry);
		entries.insert(loc.first, entry);
		return loc.second;
	}
}

bool Scoreboard::existingPlayer(Entry const& entry) {
	int i = 0;
	string name;

	for (auto it = entries.begin(); it != entries.end(); ++it, ++i) {
		string name = it->name;
		if (name.compare(entry.name) == 0) {
			it->score = entry.score;
			return false;
		}
	}
	return true;
}
Scoreboard::Entry Scoreboard::getEntry(int index) const {
	return entries[index];
}

Scoreboard::Entries Scoreboard::getEntries() {
	return entries;
}
void Scoreboard::prune(int max_amt) {
	entries.resize(max_amt);
}

int Scoreboard::size() const { return entries.size(); }

Scoreboard::EntryLocation Scoreboard::entryWorseThan(Entry const& entry) {
	int i = 0;
	for (auto it = entries.begin(); it != entries.end(); ++it, ++i) {
		if (it->score <= entry.score) {
			return EntryLocation(it, i);
		}
	}
	return EntryLocation(entries.end(), i);
}

void Scoreboard::Entry::setName(std::string str) {
	memcpy(&name[0], &str.c_str()[0], sizeof(char)* (str.length() + 1));
}
void Scoreboard::Entry::setScore(int num) {
	score = num;
}

void Scoreboard::draw() {

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);

		glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
				glLoadIdentity();
				float incrementText = 0.0f;
				bool switchColor = false;

				glColor3f(0.0f, 0.0f, 0.0f);
				string title = "Score Board";
				glRasterPos2f(-0.22f, 0.35f);
				for (int t = 0; t< title.length(); t++) {
					glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, title[t]);
				}

				for (int i = 0; i<entries.size(); ++i) {

					// setup color
					if (switchColor) {
						glColor3f(1.0f, 0.0f, 0.0f);
						switchColor = false;
					}
					else{
						glColor3f(0.0f, 1.0f, 0.0f);
						switchColor = true;
					}

					// setup strings
					Scoreboard::Entry e = entries[i];
					string name = e.name;
					stringstream ss;
					ss << e.score;
					string score = ss.str();



					name = name + "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t" + score;
					glRasterPos2f(-0.25f, incrementText);

					for (int t = 0; t< name.length(); t++) {
						glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, name[t]);
					}

					incrementText += 0.08f;
				}
				drawCube(0.0f, 0.0f, 0.0f);

			glPopMatrix();
		glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void Scoreboard::drawCube(float x, float y, float z) {
	float increm = 0.0f;
	float color = 0.0f;
	glPushMatrix();
		// glEnable(GL_BLEND);
		// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glm::mat4 matrix;
		for (int i = 0; i< 1; i++) {
			matrix = glm::translate(matrix, glm::vec3(x, y, z));
			matrix = glm::scale(matrix, glm::vec3(1.0f, 0.9f, 0.0f));
			glColor4f(1.0f, color, 1.0f, 0.2f);
			glLoadMatrixf(glm::value_ptr(matrix));
			glutSolidCube(1);
			y = y + 0.06f;
			color = color + 0.3f;
		}
		// glDisable(GL_BLEND);
	glPopMatrix();
}