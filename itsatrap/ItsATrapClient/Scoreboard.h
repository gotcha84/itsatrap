#pragma once
#ifndef SSCOREBOARD_H
#define SCOREBOARD_H

//Scoreboard.h
#include <string>
#include <utility>
#include <vector>
#include "MatrixTransform.h"
#include "MyPlayer.h"
#include "Texture.h"
#include <FTGL/ftgl.h>

class Scoreboard {
public:
	struct Entry {
		int kill;
		int death;
		int name;
		void setName(int str);
		void setKill(int num);
		void setDeath(int num);
	};

	FTGLPixmapFont *font;
	typedef std::vector<Entry> Entries;
	typedef std::pair<Entries::iterator, int> EntryLocation;

	Scoreboard();
	~Scoreboard();

	int  addEntry(Entry const& entry);
	bool existingPlayer(Entry const& entry);
	Entry getEntry(int index) const;
	Entries getEntries();
	void prune(int max_amt);
	int  size() const;
	void draw();
	void drawCube(float x, float y, float z);

private:
	Entries entries;
	static int const SizeOfEntry = sizeof(Entry);
	EntryLocation entryWorseThan(Entry const& entry);
};

#endif