#include "Server.h"
#include "ConfigSettings.h"

using namespace std;

int main(int argc, char ** argv) {

	ConfigSettings::getConfig()->loadSettingsFile();
	Server::startServer();

	return 0;
}