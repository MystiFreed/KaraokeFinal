// KaraokeSongs.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//testing the tutorial #1

#include <iostream>
#include "Song.h"
#include "MapManagement.h"
#include "FileManagement.h"
#include "CatalogEntry.h"
#include "maps.h"
#include <map>

void test();
void startup();
bool displayMenu();

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


};

// Function displays a menu for user selection of the submenu. Returns true if menu should continue.
bool displayMenu()
{
	char userSelection; //user choice within the top-of-house menu display

	string prompt = "\n----Karaoke Role Selection Menu----\n ";
	prompt += "A) Catalogue Management\n "; //this holds the menu options specific to management of the song/artist catalogues
	prompt += "B) KJ Queue Management\n "; //this menu holds options for the KJ to manage the queue of singers
	prompt += "C) Singer Menu\n "; //this menu holds singer options - histories, etc
	prompt += "D) Exit program\n ";
	prompt += "Please make a selection:\n ";
	userSelection = getInputReprompt(prompt, 'A', 'D');//getInputPreprompt converts any entry to upper for comparison

	//call the MenuMember() menu
	if (toupper(userSelection) == 'A')
	{
		test();
		return true;//causes main menu to continue
	}

	else if (toupper(userSelection) == 'B')
	{
		cout << "Add MenuInventory()" << endl;
		return true;//causes main menu to continue
	}
	//call the MenuArt() menu
	else if (toupper(userSelection) == 'C')
	{
		//MenuArt();
		return true;//causes main menu to continue

	}
	//currently a stub with re-call to the displayMenu()
	else if (toupper(userSelection) == 'D')
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


