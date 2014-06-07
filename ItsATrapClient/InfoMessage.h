#ifndef INFO_MESSAGE_H
#define INFO_MESSAGE_H

using namespace std;

#include <Windows.h>
#include <string>
#include <iostream>
#include "ConfigSettings.h"

class InfoMessage
{
private:
	DWORD		lastChanged;
	string		msg;
public:
	string		getMessage();
	void		setMessage(string msg);
};

#endif