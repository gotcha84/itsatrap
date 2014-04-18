#ifndef WORLDSTATE_H
#define WORLDSTATE_H

#include <vector>
#include "StateEntry.h"

using namespace std;

class WorldState {

public:
    vector<StateEntry>		entries;
	
	void addEntry(StateEntry entry) 
	{
		entries.push_back(entry);
	}
};

#endif
