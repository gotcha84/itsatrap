//Scoreboard.cpp
#include "Scoreboard.h"
#include <GL/glut.h>
#include <fstream>
#include <algorithm>
using namespace std;

Scoreboard::Scoreboard() {
	font = new FTGLPixmapFont("C:/Windows/Fonts/Arial.ttf");
	fonts = new FTGLExtrdFont("C:/Windows/Fonts/Arial.ttf");

}

Scoreboard::~Scoreboard() {
	delete font;
	font = nullptr;
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
	int name;

	for (auto it = entries.begin(); it != entries.end(); ++it, ++i) {
		int name = it->name;
		if (name == entry.name) {
			it->kill = entry.kill;
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

		if (it->name % 2 == 0) {
			return EntryLocation(it, i);
		}
	}
	return EntryLocation(entries.end(), i);
}
/*
void Scoreboard::Entry::setName(std::string str) {
	memcpy(&name[0], &str.c_str()[0], sizeof(char)* (str.length() + 1));
}
*/

void Scoreboard::Entry::setName(int str) {
	name = str;
}

void Scoreboard::Entry::setKill(int num) {
	kill = num;
}

void Scoreboard::Entry::setDeath(int num) {
	death = num;
}


void Scoreboard::draw() {

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);

		glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
				glLoadIdentity();
				bool switchColor = false;


				glColor3f(1.0f, 1.0f, 1.0f);
				font->FaceSize(50);
				font->CharMap(ft_encoding_symbol);
				glRasterPos2f(-0.12f, 0.9f);
				font->Render( (to_string(entries.size()) + " players").c_str() );
				glRasterPos2f(-0.10f, 0.7f);
				font->Render("Team 1");
				glRasterPos2f(-0.10f, -0.2f);
				//font->Render("Team 2");
				fonts->Render("TEAM 2", -1, FTPoint(), FTPoint(), 0x0002);
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

					Scoreboard::Entry e = entries[i];
					stringstream id;
					id << ((e.name) + 1);
					stringstream team;
					team << (((e.name) + 1) % 2) + 1 ;
					stringstream kills;
					kills << e.kill;
					stringstream deaths;
					deaths << e.death;
					string playerID = id.str();
					string teamID   = team.str();
					string numKill  = kills.str();
					string numDeath = deaths.str();
					

					//string name = "Player" + playerID + "                       " + "Kill: " + numKill + "                       " + "Death: " + numDeath;
					int temp = e.name;
					
					/*
					for (int t = 0; t< name.length(); t++) {
						glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, name[t]);
					}*/

					if (e.name == 0) { 
						glRasterPos2f(-0.6f, 0.5f);
						font->Render(("Player" + playerID).c_str());
						glRasterPos2f(-0.1f, 0.5f);
						font->Render(("Kill: " + numKill).c_str());
						glRasterPos2f(0.6f, 0.5f);
						font->Render(("Death: " + numDeath).c_str());
						if (e.name == currentPlayer) {
							glRasterPos2f(-0.8f, 0.5f);
							font->Render(">");
						}
					}
					if (e.name == 1) {
						glRasterPos2f(-0.6f, -0.4f);
						font->Render(("Player" + playerID).c_str());
						glRasterPos2f(-0.1f, -0.4f);
						font->Render(("Kill: " + numKill).c_str());
						glRasterPos2f(0.6f, -0.4f);
						font->Render(("Death: " + numDeath).c_str());
						if (e.name == currentPlayer) {
							glRasterPos2f(-0.8f, -0.4f);
							font->Render(">");
						}
					}
					if (e.name == 2) {
						glRasterPos2f(-0.6f, 0.2f);
						font->Render(("Player" + playerID).c_str());
						glRasterPos2f(-0.1f, 0.2f);
						font->Render(("Kill: " + numKill).c_str());
						glRasterPos2f(0.6f, 0.2f);
						font->Render(("Death: " + numDeath).c_str());
						if (e.name == currentPlayer) {
							glRasterPos2f(-0.8f, 0.2f);
							font->Render(">");
						}
					}
					if (e.name == 3) {
						glRasterPos2f(-0.6f, -0.7f);
						font->Render(("Player" + playerID).c_str());
						glRasterPos2f(-0.1f, -0.7f);
						font->Render(("Kill: " + numKill).c_str());
						glRasterPos2f(0.6f, -0.7f);
						font->Render(("Death: " + numDeath).c_str());
						if (e.name == currentPlayer) {
							glRasterPos2f(-0.8f, -0.7f);
							font->Render(">");
						}
					}

				}

				//drawCube(0.0f, 0.5f, 0.0f);
				//drawCube(0.0f, -0.5f, 0.0f);
			glPopMatrix();
		glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void Scoreboard::drawCube(float x, float y, float z) {
	glPushMatrix();
		glm::mat4 matrix;
		matrix = glm::translate(matrix, glm::vec3(x, y, z));
		matrix = glm::scale(matrix, glm::vec3(1.0f, 0.5f, 0.0f));
		glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
		glLoadMatrixf(glm::value_ptr(matrix));
		glutSolidCube(1);
	glPopMatrix();
}

void Scoreboard::setCurrentPlayer(int id) {
	currentPlayer = id;
}