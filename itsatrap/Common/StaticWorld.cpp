#include "StaticWorld.h"


/*
 * StaticWorld::updateObject()
 *
 */
void StaticWorld::addObject(struct staticObject e)
{
	// Entry not found, create a new one.
	objects.push_back(e);	
}

/*
 * StaticWorld::getsize()
 *
 */
int StaticWorld::getSize()
{
	return objects.size();
}




/*
 * StaticWorld::printWorld()
 *
 */
void StaticWorld::printWorld()
{
	printf("[COMMON]: Printing static world:\n");
	for (int i = 0; i < getSize(); i++)
	{
		printf("[COMMON]: Object:   minX:%4.1f   minY:%4.1f   minZ:%4.1f\n maxX:%4.1f   maxY:%4.1f   maxZ:%4.1f\n",
			objects[i].minX, objects[i].minY, objects[i].minZ, objects[i].maxX, objects[i].maxY, objects[i].maxZ);
	}
}

struct staticObject StaticWorld::getObjectAt(int i)
{
	return objects[i];
}