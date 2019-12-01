#pragma once
#include <string>
#include "Artist.h"
#include "CatalogEntry.h"
#include "FileManagement.h"
#include <sstream>

using namespace std;
const bool DEBUGSONG = false;
class Song : public CatalogEntry
{
		string songKey=BLANK_FIELD;
		std::string title = BLANK_FIELD;
		string artistKey = BLANK_FIELD;
		void setKey() {
			songKey = title + " - " + artistKey;
			if(DEBUGSONG) cout<<songKey<<"1\n";
		}
	public:
		Song() :CatalogEntry() {  };
		Song(string newTitle, string newArtistKey) :CatalogEntry()
		{
			title = newTitle;
			artistKey = newArtistKey;
			setKey();
			if (DEBUGSONG) cout << songKey << "2\n";
		};
		//copy constructor
		Song(const Song& a2) {
			songKey = a2.songKey;
			title = a2.title;
			artistKey = a2.artistKey;
			if (DEBUGSONG) cout << songKey << "3\n";

		}

		bool operator==(const Song& a2)
		{
			if (songKey == a2.songKey)
			{
				return true;
			}
			return false;
		}

		void setTitle(string newTitle);
		string getTitle();
		void setArtistKey(string artistKey);//checks whether artist key exists in map?
		string getArtistKey();

		virtual string	display();
		virtual string	toFile();
		virtual void fromFile(vector<string>::iterator );

		virtual string	getKey();

		
		
};

inline bool operator==( Song& s1,  Song& s2) { return s1.getKey() == s2.getKey(); }

//Class implementation
//#include "Song.h"


void Song::setTitle(string newTitle) { title = newTitle; };
void Song::setArtistKey(string newArtistKey) { artistKey = newArtistKey; };

string Song::getTitle() { return title; };
string Song::getArtistKey() { return artistKey; };
string Song::getKey() { 
return songKey; };

string Song::display()
{ 
		return getKey(); 
}

string Song::toFile() { return songKey + FIELD_DELIMITER + title + FIELD_DELIMITER + artistKey; }

void Song::fromFile(std::vector<string>::iterator iter) {
	
	songKey = *iter;//assign first field

	title = *++iter;//assign next field
	if(DEBUGSONG) cout << "test " << display() << endl;
	artistKey = *++iter;//assign next field

}


map<string, Song> songMap;
string songFileTXT = "Songs.txt";
fstream songFstream;// (songFileTXT, ios::in | ios::out);






multimap<string, string> songCatalogByArtist; //key artistKey, value songKey ==will add using addSongToCatalogs
string songsByArtistFileTXT = "SongsByArtist.txt";
fstream songsByArtistFstream;


