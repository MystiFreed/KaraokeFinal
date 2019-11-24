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
#include <algorithm>

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
void menuQueueManagement();
void menuSinger();
//void verifyNameExists();

int main()
{startup();
	
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
		menuQueueManagement();
		//linkListTester();
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
	/*QueueManagement_KJ<string> list;
	
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
	list.displayList();*/
	
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
	while (true)//always continue, use returns to exit function
	{
		//GET USER INPUT ON VIEW METHOD //NEED TO DO finish this if desired
		enum viewOption { BACK_MENU, SCREEN_DISPLAY, PRINT_REPORT};
		

		//string prompt = "\n----View Catalogue Menu----\n ";
		//prompt += BACK_MENU +") Back to main menu\n ";
		//prompt += SCREEN_DISPLAY+") Display on screen\n ";
		////prompt += PRINT_REPORT +") Print catalog\n "; //NEED TO DO - either create this or remove this menu.
		//prompt += "Please make a selection:\n ";
		//viewMethod = getInputReprompt(prompt, BACK_MENU, PRINT_REPORT);//getInputPreprompt converts any entry to upper for comparison
		// if (viewMethod == BACK_MENU) { return; };
		
		int viewMethod = SCREEN_DISPLAY;//NEED TO DO - delete this if above menu gets finished

		//GET USER INPUT WHICH CATALOG TO VIEW
		int userSelection;
		enum catalogSelection { BACK, SONG, SONG_ARTIST, ARTIST };
		string promptb = "\n----View Catalogue Menu - SELECT CATALOG----\n ";
		promptb += BACK +") Back to previous menu\n ";
		promptb += SONG +") Song Catalogue\n ";
		promptb += SONG_ARTIST +") Song by Artist Catalogue\n ";
		promptb += ARTIST +") Artist List\n ";
		
		promptb += "Please make a selection:\n ";
		userSelection = getInputReprompt(promptb, 1, 4);//getInputPreprompt converts any entry to upper for comparison

		switch (userSelection) {
		case BACK:
			//allow while loop to continue;
			break;
		case SONG:
			if (viewMethod == SCREEN_DISPLAY) { displayMap(songMap); }
			break;
		case SONG_ARTIST:
			if (viewMethod == SCREEN_DISPLAY) { displayMap(songCatalogByArtist); }
			break;
		case ARTIST:
			if (viewMethod == SCREEN_DISPLAY) { displayMap(artistMap); }
			break;
		default:
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

void menuQueueManagement()
{
	bool continueMenu = true;

	QueueManagement_KJ<string> list;
	QueueManagement_KJ<string> songList;

	Singer singer1("MystiFreed");
	Singer singer2("EthanFreed");
	Singer singer3("JonFreed");
	Singer singer4("AdrianVanderveer");
	Singer singer5("AlexGaumer");
	Singer singer6("RoseMendlik");
	Singer singer7("TomMendlik");
	Singer singer8("SharonFreed");
	Singer singer9("DavidFreed");
	Singer singer10("PauleenDofner");
	Singer singer11("TobyDofner");
	//select by key or use the new function to find/create

	list.appendNode(singer1.getDisplayName());
	list.appendNode(singer2.getDisplayName());
	list.appendNode(singer3.getDisplayName());
	list.appendNode(singer4.getDisplayName());
	list.appendNode(singer5.getDisplayName());
	list.appendNode(singer6.getDisplayName());
	list.appendNode(singer7.getDisplayName());
	list.appendNode(singer8.getDisplayName());
	list.appendNode(singer9.getDisplayName());
	list.appendNode(singer10.getDisplayName());
	list.appendNode(singer11.getDisplayName());


	while (continueMenu)
	{
		int userSelection;
		string prompt = "\n----Queue Management Menu----\n ";
		prompt += "1) Add Singer and Selection\n "; //add a new singer to the queue - this adds them to the end
		prompt += "2) Remove Singer\n "; //remove a singer from the queue 
		prompt += "3) Move Singer in Queue\n "; //move a singer from their current place in the queue to another selected place
		prompt += "4) Display Pending Singers\n "; //display the next 10 singers pending
		prompt += "5) Display Pending Songs\n "; //display the pending songs (and their corresponding singer)
		prompt += "6) EOD Clear Queue\n "; //clears out any remaining in the queue at the end of the day
		prompt += "7) Exit to main\n "; //exit to main menu
		prompt += "Please make a selection:\n ";
		userSelection = getInputReprompt(prompt, 1, 7);//getInputPreprompt converts any entry to upper for comparison

		Singer newSinger;
		Song newSong;
		Singer toRemove;
		string firstName;
		string lastName;
		string displayname;
		string songTitle;
		string singerToMove;
		string singerAfter;
		bool verifyExists;
		char songComplete;
		//Song tempSong;
		switch (userSelection) {
		case 1:
			/*cout << "Please enter the singer's first name:" << endl;
			cin >> firstName;
			cout << "Please enter the singer's last name:" << endl;
			cin >> lastName;
			displayname = firstName+lastName;
			//credit to geeksforgeeks: https://www.geeksforgeeks.org/conversion-whole-string-uppercase-lowercase-using-stl-c/
			transform(displayname.begin(), displayname.end(), displayname.begin(), ::toupper); //convert the whole combined string to all caps*/

			newSinger = userInputSinger();
			displayname = newSinger.getDisplayName();
			list.appendNode(displayname);

			cout << "Please enter the song you that the singer has chosen:\n";
			cin >> songTitle;
			//credit to geeksforgeeks: https://www.geeksforgeeks.org/conversion-whole-string-uppercase-lowercase-using-stl-c/
			transform(songTitle.begin(), songTitle.end(), songTitle.begin(), ::toupper); //convert the whole combined string to all caps
			cout << songTitle;
			songList.appendNode(songTitle);
			cin.ignore();
			break;
		case 2:
			cout << endl;
			cout << "Current queue:" << endl;
			list.displayFullList();
			cout << endl;
			cout << "Please select the singer to remove by entering the display name.\n";
			reenter1:cin >> displayname;
			verifyExists = list.verifyNameExists(displayname);
			if (verifyExists == false)
			{
				cout << "Please enter a valid username:\n"; 
				goto reenter1;
			}
			list.deleteNode(displayname);
			cout << endl;
			cout << "Did the singer complete the song? Y yes, N no.\n";
			cin >> songComplete;
			if (toupper(songComplete) == 'Y')
			{
				//addToSingerHistory(displayname, 2019, 11, 10, "DREAMS-FLEETWOOD MAC");
				//addObjectToMap(&allSingerHistoryMap, displayname, "DREAMS-FLEETWOOD MAC");
				//make sure to update both the singer list and the song list
			}
			if (toupper(songComplete) == 'N')
			{
				//addToSingerHistory(displayname, 2019, 11, 10, "DREAMS-FLEETWOOD MAC");
				//addObjectToMap(&allSingerHistoryMap, displayname, "DREAMS-FLEETWOOD MAC");
				//make sure to update both the singer list and the song list
			}
			cout << displayname << " removed from the queue. Remaining queue:\n";
			cout << endl;
			list.displayFullList();
			cin.ignore();
			break;
		case 3:
			list.displayFullList();
			cout << "Please select the singer to move by entering the display name.\n";
			cin >> singerToMove;
			cout << "Move this singer to which position (please enter the singer display name that will follow the moved singer).\n";
			cin >> singerAfter;
			list.insertNode(singerToMove, singerAfter);
			break;
		case 4:
			cout << "On deck: \n";
			cout << endl; //add a break between the on deck and singer list
			list.displayList(); //display the top 10 displaynames for singers
			break;
		case 5:
			//this needs to pull in the songs tied to the singers in the queue
			songList.displayFullList();
			break;
		case 6:
			cout << "Clearing queue.\n";
			list.~QueueManagement_KJ(); //call the destructor to remove all pending singers, send back to the main to avoid duplicate delete[] calls
			main();
		default:
			continueMenu = false; //change the boolean from true to false and exit the menu
			return;
			break;
		}
	}
}