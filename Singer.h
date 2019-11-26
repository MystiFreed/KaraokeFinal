#pragma once
#include <set>
#include <string>
#include "CatalogEntry.h"
#include "FileManagement.h"
#include "MapManagement.h"
#include"Song.h"

class Singer : public CatalogEntry {
	string singerKey;//"unique username"
	std::string displayName = "";
public:
	Singer() :CatalogEntry() {};
	Singer(string username) :CatalogEntry()
	{
		singerKey = username;
		displayName = username;
	};

	Singer(string username, string displayName) :CatalogEntry()
	{
		singerKey = username;
		displayName = displayName;
	};

	//copy constructor
	Singer(const Singer& a2) {
		singerKey = a2.singerKey;
		displayName = a2.displayName;
	}
	//https://en.cppreference.com/w/cpp/language/copy_assignment
	Singer& operator=(const Singer& a2)
	{
		singerKey = a2.singerKey;
		displayName = a2.displayName;
		return *this;
	}

	void setDisplayName(string newName);
	string getDisplayName
	();

	virtual string	display();
	virtual string	toFile();
	virtual void fromFile(vector<string>::iterator);

	virtual string	getKey();
};

void Singer::setDisplayName(string newName) { displayName = newName; };
string Singer::getDisplayName(){return displayName;}

 string	Singer::display() { return displayName; }
 string	Singer::toFile()
 {
	return singerKey + FIELD_DELIMITER + displayName + FIELD_DELIMITER;
};

 string Singer::getKey() { return singerKey; }

 void Singer::fromFile(vector<string>::iterator iter)
 {
			 singerKey = *iter;//assign first field
			 displayName = *++iter;//assign next field
 }
 
 map<string, Singer> singerMap; 
 string singerTXT = "Singers.txt";
 fstream singerFstream;

