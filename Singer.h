#pragma once
#include <string>
#include "CatalogEntry.h"
#include "FileManagement.h"
#include "MapManagement.h"
#include"Song.h"
class Singer : public CatalogEntry {

	string singerKey;//"unique username"
	std::string displayName = "";
	//map<string, Song> SingerHistoryByDate;
	

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

	void setDisplayName(string newName);
	string getDisplayName
	();

	virtual string	display();
	virtual string	toFile();
	virtual void fromFile(vector<string>::iterator);

	virtual string	getKey();
//	virtual void	updateKey();
	multimap<string, string> SingerHistoryBySong;
};

void Singer::setDisplayName(string newName) { displayName = newName; };
string Singer::getDisplayName(){return displayName;}

 string	Singer::display() { return displayName; }
 string	Singer::toFile()
 {
	string temp = singerKey + FIELD_DELIMITER + displayName + FIELD_DELIMITER;
	////put elements of map 
	//	 for (auto& e : SingerHistoryByDate) 
	//	 { temp += ELEMENT_DELIMITER + FIELD_DELIMITER + e.first + FIELD_DELIMITER + e.second + FIELD_DELIMITER + ELEMENT_DELIMITER; }
	// temp += FIELD_DELIMITER;
	 //put elements of map 
		 for (auto& e : SingerHistoryBySong)
		 {
			 temp += ELEMENT_DELIMITER + FIELD_DELIMITER;
			 temp+=e.first;
			 temp+= FIELD_DELIMITER + e.second + FIELD_DELIMITER ;}
	 temp += ELEMENT_DELIMITER+ FIELD_DELIMITER;
	 temp += "\n";
	 if (DEBUGMAP)cout << temp;
	return temp;
};

 string Singer::getKey() { return singerKey; }

 //NEED to test, this one is complicated!!!
 void Singer::fromFile(vector<string>::iterator iter)
 {
		//whole object in one string from file, separate into four strings
			 singerKey = *iter;//assign first field
			 displayName = *++iter;//assign next field
						//	 string MapByDateAsLine = *++iter;//assign next field
			 string MapBySongAsLine = *++iter;//assign next field
							//whole map is in one string, separate into elements
								// vector < string >MapByDateAsvector = SeparateLineByDelimiter(MapByDateAsLine, ELEMENT_DELIMITER);
							//for each each element, spearate into fields and add to the map as a key and value
								 // for (auto& e : MapByDateAsvector) {
								//	 primaryMapSeparateLineByDelimiter(SingerHistoryByDate);
								 //}

			 //whole map is in one string, separate into elements
			 vector<string> MapBySongAsvector = SeparateLineByDelimiter(MapBySongAsLine, ELEMENT_DELIMITER);
			 //for each each element, spearate into fields and add to the map as a key and value
			 for (auto& e : MapBySongAsvector) {
				 cout << e;
				// vector<string> fields =SeparateLineByDelimiter(e, FIELD_DELIMITER);
				// SingerHistoryBySong.emplace(e[0], e[1]);
			 }
 }
 
 map<string, Singer> singerMap; 
 string singerTXT = "Singers.txt";
 fstream singerFstream;