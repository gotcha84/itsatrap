#include "InfoMessage.h"

string InfoMessage::getMessage()
{
	int tmp = 0;
	ConfigSettings::getConfig()->getValue("InfoMessageDuration", tmp);

	DWORD thres = (DWORD)tmp;

	if (GetTickCount() - lastChanged > thres)
		return "";
	else
		return msg;
}

void InfoMessage::setMessage(string msg)
{
	this->msg = msg;
	this->lastChanged = GetTickCount();
}