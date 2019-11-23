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
#include <map>
#include "DateCalcLong.h"
#include "SingerHistory.h"

using namespace std;
//testing functions
void testIntInput();
void test();
void linkListTester();
const bool DEBUGMAIN = false;

//function prototypes
void startup();
bool displayMenu();
void menuManageCatalogue();
void menuDisplayCatalogue();
void exitSaving();
void menuSinger();

int main()
{
	startup();
	while (displayMenu()); //automatically repeats until a false (exit) is returned.
	exitSaving();
	return 0;
	
};

void startup() {
	cout << "Startup...";
	bool check;
	if (DEBUGMAIN) {
		cout << "DEBUGMAIN is true. Displaying startup messages\n";
	}
	check = openFileInOut(songFstream, songFileTXT);
	if (DEBUGMAIN) { cout << "\nopen song fstream: " << check << endl; }
	check = openFileInOut(artistFstream, artistFileTXT);
	if (DEBUGMAIN) { cout << "\nopen artist fstream: " << check << endl; }

	check = openFileInOut(singerFstream, singerTXT);
	if (DEBUGMAIN) { cout << "\nopen allSingerHistory fstream: " << check << endl; }


	check = openFileInOut(singerHistoryFstream, allSingerHistoryTXT);
	if (DEBUGMAIN) { cout << "\nopen allSingerHistory fstream: " << check << endl; }


	primaryMapFromFile(songMap, songFstream);
	primaryMapFromFile(artistMap, artistFstream);
	primaryMapFromFile(singerMap, singerFstream);
	multiMapFromFile(allSingerHistoryMap, singerHistoryFstream);
	cout << "Done importing map data";
	if (DEBUGMAIN) { linkListTester(); }

	//NEED TO DO add import and saving for multimaps that are implemented
	if (DEBUGMAIN) {cout << "\nDone importing map data";}
};

// Function displays a menu for user selection of the submenu. Returns true if menu should continue.
bool displayMenu()
{
	int userSelection; //user choice within the top-of-house menu display
	enum roleOptions {NONE, CATALOG, KJ,SINGER,EXIT};
	string prompt = "\n----Karaoke Role Selection Menu----\n ";
	prompt += "1) Catalogue Management\n "; //this holds the menu options specific to management of the song/artist catalogues
	prompt += "2) KJ Queue Management\n "; //this menu holds options for the KJ to manage the queue of singers
	prompt += "3) Singer Menu\n "; //this menu holds singer options - histories, etc
	prompt += "4) Exit program\n ";
	prompt += "Please make a selection:\n ";

	//this is having some issues, need to take a look at the getInputReprompt function in FileManagement.h
	userSelection = getInputReprompt(prompt, CATALOG, EXIT);//getInputPreprompt converts any entry to upper for comparison

	//call the MenuMember() menu
	if (toupper(userSelection) == CATALOG)
	{
		menuManageCatalogue();

		return true;//causes main menu to continue
	}

	else if (toupper(userSelection) == KJ)
	{
		linkListTester();
		return true;//causes main menu to continue
	}
	else if (toupper(userSelection) == SINGER)
	{
		menuSinger();
		return true;//causes main menu to continue

	}
	else if (toupper(userSelection) == EXIT)
	{
		cout << "Thank you, program closing." << endl;
		return false;//causes main menu to stop
	}
	else {
		cout << "Main menu error. ";
		return true;
	}
};



void menuSinger() 
{
	do	{

		int userSelection; //user choice within the top-of-house menu display
		enum roleOptions { DISPLAYALL, ADD, VIEW, SONGHISTORY, EXIT };
		string prompt = "\n----Singer Selection Menu----\n ";
		prompt += "1) Add Singer\n "; //this holds the menu options specific to management of the song/artist catalogues
		prompt += "2) View Singer\n "; //this menu holds options for the KJ to manage the queue of singers
		prompt += "3) Song History\n "; //this menu holds singer options - histories, etc
		prompt += "4) Back\n ";
		prompt += "Please make a selection:\n ";

		userSelection = getInputReprompt(prompt, DISPLAYALL, EXIT);//getInputPreprompt converts any entry to upper for comparison
		Singer tempSinger;
		Song tempSong;
		tm* tempDate;
		string storeInput;
		switch (userSelection)
		{
			case DISPLAYALL:
				displayMap(singerMap);
			case ADD:
				 tempSinger = userInputSinger();
				addObjectToMap(singerMap, tempSinger);
				cout << endl << "temp object: " << tempSinger.display() << endl;
				displayMap(singerMap);
				break;
			case VIEW:
				 tempSinger;
				if (UserInputSelectByKey(singerMap, "Enter the username of the singer:", storeInput, tempSinger)) { cout << tempSinger.display(); }
				else { cout << "\nNot found\n"; }
				break;
			case SONGHISTORY:
				cout << "\nAll Singer History Map\n";
				UserInputSelectByKey(singerMap, "singerkey", storeInput, tempSinger);
				UserInputSelectByKey(songMap, "songkey", storeInput, tempSong);
				tempDate = userInputDate();
				//addToSingerHistory("billy",2019,11,20, "Hey Jude - Beatles, The");
				addToSingerHistory(tempSinger.getKey(),tempDate, tempSong.getKey());
				displayMap(allSingerHistoryMap);
				break;
			case EXIT:
				return;
				break;
			default:
				return;
				break;
		}
	} while (true);
};
//run this to test basic functions after changes
void test() {
	multimap<string, string> testMultiMap;
	fstream testFstream("testMap.txt", ios::in | ios::out);
	addObjectToMap(&testMultiMap, "key1", "value1");
	addObjectToMap(&testMultiMap, "key2", "value2");
	multiMapToFile(testMultiMap, testFstream);
	multimap<string, string> placedMap;
	multiMapFromFile(placedMap, testFstream);

	Singer amy("Amy1", "Amy");
	cout << "add " << endl;

	

	addObjectToMap(singerMap, amy);
	displayMap(singerMap);


	for (int i = 0; i < 5; i++) {
		cout << "\n------------------\nAdd a new Artist and Song (later will add select existing artist, function created but not tested yet.";

		Song tempSong = userInputSong();
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
void exitSaving() {
	primaryMapToFile(songMap, songFstream);
	primaryMapToFile(artistMap, artistFstream);
	primaryMapToFile(singerMap, singerFstream);
	multiMapToFile(allSingerHistoryMap, singerHistoryFstream);
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
		int userSelection;
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
			 userInputArtist();
			break;
		case 2:
			cout << "Enter the song's Artist and then the Song information.\n";
			userInputSong();
			
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
		int userSelection;
		string promptb = "\n----View Catalogue Menu - SELECT CATALOG----\n ";
		promptb += "1) Song Catalogue\n ";
		promptb += "2) Song by Artist Catalogue\n ";
		promptb += "3) Artist List\n ";
		promptb += "4) Exit program\n ";
		promptb += "Please make a selection:\n ";
		userSelection = getInputReprompt(promptb, 1, 4);//getInputPreprompt converts any entry to upper for comparison

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

void testIntInput()
{ for (int x = 0; x < 6; x++) {
		cout << "\nAttempt " << x << endl;
		cout << "Selected:" << getInputReprompt("Prompt:", 1, 4);
	} 
}