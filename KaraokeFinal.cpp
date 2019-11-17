// KaraokeSongs.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//testing the tutorial #1

#include <iostream>
#include "Song.h"
#include "MapManagement.h"
#include "FileManagement.h"
#include "CatalogEntry.h"
#include "QueueManagement_KJ.h"
#include "Singer.h"
#include "maps.h"
#include <map>

using namespace std;

//function prototypes
void test();
void startup();
void linkListTester();
bool displayMenu();
//char getInputReprompt(string, char, char);

int main()
{
	startup();
	//test(); this is currently under the catalogue management menu as a placeholder
	while (displayMenu()); //automatically repeats until a false (exit) is returned.

	return 0;

};

void startup() {
	cout << "open song fstream: " << openFileInOut(songFstream, songFileTXT) << endl;
	cout << "\nopen artist fstream: " << openFileInOut(artistFstream, artistFileTXT) << endl;
	primaryMapFromFile(songMap, songFstream);
	primaryMapFromFile(artistMap, artistFstream);
	cout << "\nDone importing map data";
	linkListTester();

};

// Function displays a menu for user selection of the submenu. Returns true if menu should continue.
bool displayMenu()
{
	char userSelection; //user choice within the top-of-house menu display

	string prompt = "\n----Karaoke Role Selection Menu----\n ";
	prompt += "1) Catalogue Management\n "; //this holds the menu options specific to management of the song/artist catalogues
	prompt += "2) KJ Queue Management\n "; //this menu holds options for the KJ to manage the queue of singers
	prompt += "3) Singer Menu\n "; //this menu holds singer options - histories, etc
	prompt += "4) Exit program\n ";
	prompt += "Please make a selection:\n ";
	//this is having some issues, need to take a look at the getInputReprompt function in FileManagement.h

	userSelection = getInputReprompt(prompt, 1, 4);//getInputPreprompt converts any entry to upper for comparison
	//cin >> userSelection;

	//call the MenuMember() menu
	if (toupper(userSelection) == 1)
	{
		linkListTester();
		//test();
		
		return true;//causes main menu to continue
	}

	else if (toupper(userSelection) == 2)
	{
		linkListTester();
		return true;//causes main menu to continue
	}
	//call the MenuArt() menu
	else if (toupper(userSelection) == 3)
	{
		//MenuArt();
		return true;//causes main menu to continue

	}
	//currently a stub with re-call to the displayMenu()
	else if (toupper(userSelection) == 4)
	{
		cout << "Thank you, program closing." << endl;
		return false;//causes main menu to stop
	}
	else {
		cout << "Main menu error. ";
		return true;
	}
};

//run this to test basic functions after changes
void test() {

	for (int i = 0; i < 5; i++) {
		cout << "\n------------------\nAdd a new Artist and Song (later will add select existing artist, function created but not tested yet.";

		Artist tempArtist = userInputArtist(); 
		Song tempSong = userInputSong(tempArtist.getKey());
		if (addSongToCatalogs(tempSong)) { cout << "Added to maps\n"; }
		else { cout << "Error\n"; };
		cout<<"Found in songMap? 1=true"<<to_string(songMap.count(tempSong.getKey()))<<"\n";
		cout << "Found in songCatalogByArtist? 1=true" << to_string(songCatalogByArtist.count(tempSong.getKey())) << "\n";
	}
	
	cout << "Check contents of songMap:\n";
	displayMap(songMap);
	cout << "Check contents of artistMap:\n";
	displayMap(artistMap);
	cout << "Check contents of songCatalogbyArtist:\n";
	displayMap(songCatalogByArtist);
	cout << "Write contents of songMap to File:\n";
	primaryMapToFile(songMap, songFstream);

	cout << "Now Reading file into a blank map and displaying that map to ensure that reading worked. You will only see contents below if they were read from the file and inserted into a map correctly\n";

	map<string, Song> testMap; //using a different map to test read since all the items are in
	primaryMapFromFile(testMap, songFstream);


	cout << "For Testing, display contents of map that were read in from file\n";
	displayMap(testMap);
}

void linkListTester()
{
	QueueManagement_KJ<string> list;
	
	//Singer singer1("MystiFreed");
	//Singer singer2("EthanFreed");
	//Singer singer3("JonFreed");
	//Singer singer4("AdrianVanderveer");
	//Singer singer5("AlexGaumer");

	//QueueNode<string>("MystiFreed");
	//string name1 = singer1.getDisplayName();
	//string name2 = singer2.getDisplayName();
	//string name3 = singer3.getDisplayName();
	//string name4 = singer4.getDisplayName();
	//string name5 = singer5.getDisplayName();

	list.appendNode("Mysti");
	list.appendNode("Jon");
	list.appendNode("Alex");
	list.appendNode("Adrian");
	list.appendNode("Ethan");

	list.displayList();
	//list.deleteNode("Alex");
	list.displayList();
	
}


