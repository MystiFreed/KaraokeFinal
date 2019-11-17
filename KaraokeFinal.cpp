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
void menuManageCatalogue();
void menuDisplayCatalogue();
bool DEBUGMAIN = true;
///deleted
char getInputReprompt(std::string promptMessage, char minRange, char maxRange) {
	if (DEBUGMAIN) cout << "Deleted the char version of getInputReprompt due to bugs, use int version for menus instead";
	return'A';
};

int main()
{
	//startup();
	//test(); this is currently under the catalogue management menu as a placeholder
	while (displayMenu()); //automatically repeats until a false (exit) is returned.

	return 0;

};

void startup() {
	bool check;
	if (DEBUGMAIN) {
		cout << "DEBUGMAIN is true. Displaying startup messages\n";
	}
	check = openFileInOut(songFstream, songFileTXT);
	if (DEBUGMAIN) { cout << "\nopen song fstream: " << check << endl; }
	check = openFileInOut(artistFstream, artistFileTXT);
	if (DEBUGMAIN) { cout << "\nopen artist fstream: " << check << endl; }
	primaryMapFromFile(songMap, songFstream);
	primaryMapFromFile(artistMap, artistFstream);
	cout << "\nDone importing map data";
	linkListTester();

	//NEED TO DO add import and saving for multimaps that are implemented
	if (DEBUGMAIN) {cout << "\nDone importing map data";}
};

// Function displays a menu for user selection of the submenu. Returns true if menu should continue.
bool displayMenu()
{
	int userSelection; //user choice within the top-of-house menu display

	string prompt = "\n----Karaoke Role Selection Menu----\n ";
	prompt += "1) Catalogue Management\n "; //this holds the menu options specific to management of the song/artist catalogues
	prompt += "2) KJ Queue Management\n "; //this menu holds options for the KJ to manage the queue of singers
	prompt += "3) Singer Menu\n "; //this menu holds singer options - histories, etc
	prompt += "4) Exit program\n ";
	prompt += "Please make a selection:\n ";

	//this is having some issues, need to take a look at the getInputReprompt function in FileManagement.h
	userSelection = getInputReprompt(prompt, 1, 4);//getInputPreprompt converts any entry to upper for comparison


	userSelection = 1;
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

void menuManageCatalogue() {
	bool continueMenu = true;

	while (continueMenu)
	{
		char userSelection;
		string prompt = "\n----Catalogue Management Menu----\n ";
		prompt += "1) Add Artist\n "; //this holds the menu options specific to management of the song/artist catalogues
		prompt += "2) Add Song\n "; //this menu holds options for the KJ to manage the queue of singers
		prompt += "3) View Catalogues\n "; //this menu holds singer options - histories, etc
		prompt += "4) Exit program\n ";
		prompt += "Please make a selection:\n ";
		userSelection = getInputReprompt(prompt, 1, 4);//getInputPreprompt converts any entry to upper for comparison

		Artist tempArtist;
		Song tempSong;
		switch (userSelection) {
		case 1:
			addObjectToMap(artistMap, userInputArtist());
			break;
		case 2:
			cout << "Enter the song's Artist and then the Song information.\n";
			addSongToCatalogs(userInputSong(userInputArtist().getKey()));
			
			break;
		case 3:
			menuDisplayCatalogue();
			break;
		case 4:
			continueMenu = false;
			return;
			break;
		default:
			continueMenu = false;
			return;
			break;
		}
	}
}
void menuDisplayCatalogue()
{
	bool continueMenu = true;

	while (continueMenu)
	{
		//GET USER INPUT ON VIEW METHOD //NEED TO DO finish this if desired
		enum viewOption { SCREEN_DISPLAY, PRINT_REPORT, BACK_MENU };
		int viewMethod = SCREEN_DISPLAY;
		//string prompt = "\n----View Catalogue Menu----\n ";
		//prompt += SCREEN_DISPLAY+") Display on screen\n "; 
		//prompt += PRINT_REPORT +") Print catalog\n "; 
		//prompt += BACK_MENU +") Back to previous menu\n ";
		//prompt += "Please make a selection:\n ";
		//viewMethod = getInputReprompt(prompt, SCREEN_DISPLAY, BACK_MENU);//getInputPreprompt converts any entry to upper for comparison
		//if (viewMethod == BACK_MENU) { return; };

		//GET USER INPUT WHICH CATALOG TO VIEW
		char userSelection;
		string promptb = "\n----View Catalogue Menu - SELECT CATALOG----\n ";
		promptb += "1) Song Catalogue\n ";
		promptb += "2) Song by Artist Catalogue\n ";
		promptb += "3) Artist List\n ";
		promptb += "4) Exit program\n ";
		promptb += "Please make a selection:\n ";
		userSelection = getInputReprompt(promptb, 'A', 'D');//getInputPreprompt converts any entry to upper for comparison

		switch (userSelection) {
		case 1:
			if (viewMethod == SCREEN_DISPLAY) { displayMap(songMap); }
			break;
		case 2:
			if (viewMethod == SCREEN_DISPLAY) { displayMap(songCatalogByArtist); }
			break;
		case 3:
			if (viewMethod == SCREEN_DISPLAY) { displayMap(artistMap); }
			break;
		case 4:
			return;
			break;
		}
	}//end while loop
}//end menuMDisplayCatalogue
