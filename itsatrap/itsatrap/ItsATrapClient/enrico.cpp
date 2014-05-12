
#include "enrico.h"
#include <map>

using namespace std;

extern ClientInstance client; // 'client' is a global var in main.cpp

static map<int, sg::MatrixTransform *> objects;


void testAddCube(int id, float x, float y, float z)
{
	// MEMORY LEAK POSSIBILITY!
	sg::MatrixTransform *obj1 = new sg::MatrixTransform();
	client.root->addChild(obj1);
	sg::Cube *obj1Shape = new sg::Cube();
	obj1->addChild(obj1Shape);
	obj1->setMatrix(glm::translate(glm::vec3(x,y,z)) * glm::scale(glm::vec3(10,10,10)));

	objects[id] = obj1;
}

// Returns 0 if successful
int testUpdate(int id, float x, float y, float z)
{
	sg::MatrixTransform *obj1 = objects[id];

	if (obj1 == NULL)
		return 1;

	obj1->setMatrix(glm::translate(glm::vec3(x,y,z)) * glm::scale(glm::vec3(10,10,10)));
	return 0;
}

// The argument 'world' contains objects information, such as id, x, y, z.
// This function works like this:
// If the object's id exists, update its location
// If the object's id doesn't exist, create a new one object with that id
void testUpdateWorld(WorldState *world)
{
	for (int i = 0; i < world->getSize(); i++)
	{
		int id = world->getEntryAt(i).objectId;
		float x = world->getEntryAt(i).x;
		float y = world->getEntryAt(i).y;
		float z = world->getEntryAt(i).z;

		if (testUpdate(id, x, y, z))
		{
			// If the id is playerId, do not render because we don't want to render our own player.
			// This is just temporary. playerId should not be used as objectId!
			if (id != Client::getPlayerId()) 
				testAddCube(id, x, y, z);
		}
	}
}