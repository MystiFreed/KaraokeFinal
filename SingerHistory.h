#pragma once
#pragma warning(disable : 4996)//compiler can't do locatltime_s function so this stops the prompting for avoiding deprecated localtime function.
#include <set>
#include <string>
#include "CatalogEntry.h"
#include "FileManagement.h"
#include "MapManagement.h"
#include"Song.h"
#include <time.h>       /* time_t, struct tm, time, mktime */
#include <iostream>
#include <iterator> 
#include <utility>
using namespace std;

//date time functions
struct tm* setDate(int , int , int , time_t& );
struct tm* setDate(int , int , int );
string dateToString(tm*);
tm* userInputDate();

//singer history functions
void addToSingerHistory(string , tm* , string );
void addToSingerHistory(string , int , int , int , string );
void addToSingerHistory(string);

void viewSingerMenu();
void viewSingerHistory(multimap<string, string> );

//singer history definitions
multimap<string, string> allSingerHistoryMap; //<singerkey, dateAsString$songKey
string allSingerHistoryTXT = "allSingerHistory.txt";
fstream singerHistoryFstream;



//this version collects required information as parameters
void addToSingerHistory(string singerKey, int year, int month, int day, string songKey) {
	tm* performedDate = setDate(year, month, day);
	addToSingerHistory(singerKey, performedDate, songKey);
}
//this version collects required information as parameters

//this version gets user input for all required fields for this function, enter a singerKey if you know it, otherwise use "". It will check that the singer exists or ask the user to enter the singer.
void addToSingerHistory(string singerKey) {
	string storeInput;
	Singer tempSinger;
	Song tempSong;
	tm* tempDate;
	if (!SelectByKey(singerMap, singerKey, tempSinger)) //if the provided singerkey is not in the map, get input from the user to find one.
	{
		tempSinger = userInputSinger();
	}
	//will allow user to select song or add song
	tempSong = userInputSong();
	tempDate = userInputDate();
	addToSingerHistory(tempSinger.getKey(), tempDate, tempSong.getKey());
}
void addToSingerHistory(string singerKey, tm* performedDate, string songKey) {
	string dateString = dateToString(performedDate);
	string newValue = dateString + ELEMENT_DELIMITER + songKey+ELEMENT_DELIMITER ;
	addObjectToMap(&allSingerHistoryMap, singerKey, newValue);
	multiMapToFile(allSingerHistoryMap, singerHistoryFstream);
}

//filters to a single singer before viewing singer history
void viewSingerHistory(Singer selectedSinger) {
	multimap<string, string> singerCompositeMap;
	if (mapResultsByKey(allSingerHistoryMap, singerCompositeMap, selectedSinger.getKey())) {
		viewSingerHistory(singerCompositeMap);
	}
	else {
		cout << "No history for this singer.";
	}

}

//shows counts based on the multimap in the parameter. Should be <singerkery, "datestring | songkey |">
void viewSingerHistory(multimap<string, string> singerCompositeMap) {
	try {
		multimap<string, string> dateSongMap;//temporary map to be filled with all songs this singer has done, by date

		//split the value string into fields, map with date key and songkey value	
		for (auto& iter : singerCompositeMap) {
			string value = iter.second;//value is a string that includes delimiters
			vector<string> tempFields = SeparateLineByDelimiter(value, ELEMENT_DELIMITER);//split on the delimiters
			addObjectToMap(&dateSongMap, tempFields[0], tempFields[1]);//add to new map using first split field as the key and second as the value
		}//now dateSongMap includes only entries for the selected singer, with date as key and song as the value

		multimap<string, string> songDateMap;//temporary map to be filled with all songs this singer has done, by song
		multimap<string, string> artistSongMap;//temporary map to be filled with all songs this singer has done, by artist
		map<string, string> uniqueSongs;
		multimap<string, string> uniqueSongsByArtist;
		set<string> uniqueArtists;

		//fill the other maps that are indexed by songkey and artistkey
		for (auto& iter : dateSongMap) {
			string song = iter.second;//songkey
			Song tempSong;
			SelectByKey(songMap, song, tempSong);
			string artist = tempSong.getArtistKey();
			addObjectToMap(&songDateMap, song, iter.first);
			addObjectToMap(&artistSongMap, artist, song);
			uniqueSongs.emplace(song, artist);//only allows a single instance of each
			uniqueArtists.emplace(artist);
		}
		//save unique songs sorted by artist
		for (auto& e : uniqueSongs) {
			uniqueSongsByArtist.emplace(make_pair(e.second, e.first));
		}

		//do stuff with the results...
		//heading
		auto iter = dateSongMap.begin();
		cout << "\n\n----Results for " << iter->first << " - ";
		auto r_iter = dateSongMap.rbegin();
		cout << r_iter->first << "----" << endl;
		////done printing heading

		multimap<int, string, greater <int> > descendingMap;

		///////display song count in descending order///////
		cout << "\n---Number of times performing song----\n";
		for (auto& e : uniqueSongs) {
			string song = e.first;
			descendingMap.emplace(make_pair(songDateMap.count(song), song));
		}
		for (auto& e : descendingMap) {
			cout << e.first << ": " << e.second << endl;
		}
		//////////////
		cin.get();
		descendingMap.clear();

		///////display artist count in descending order///////
		cout << "\n---Number of times songs by artist were performed----\n";
		for (auto& e : uniqueArtists) {
			descendingMap.emplace(make_pair(artistSongMap.count(e), e));
		}
		for (auto& e : descendingMap) {
			cout << e.first << ": " << e.second << endl;
		}
		//////////////
		cin.get();
		descendingMap.clear();

		///////display artist unique song count in descending order///////
		cout << "\n---Number of songs by artist that were performed----\n";
		for (auto& e : uniqueArtists) {
			descendingMap.emplace(make_pair(uniqueSongsByArtist.count(e), e));
		}
		for (auto& e : descendingMap) {
			string artist = e.second;
			cout << e.first << ": " << artist << endl;
			multimap<string, string> resultMap;
			mapResultsByKey(uniqueSongsByArtist, resultMap, artist);
			for (auto& s : resultMap) {
				cout << "     " << " " << s.second << endl;
			}
		}
		//////////////
		cin.get();
		descendingMap.clear();

	}
	catch (...) { cout << "Error in viewSingerHistory(). Not able to display."; }
}


void menuSinger()
{
	while(true) 
	{
		int menuSingerSelection; //user choice within the top-of-house menu display
		enum roleOptions { BACK, DISPLAYALL, ADD, VIEW_SINGER };
		string prompt = "\n----Singer Selection Menu----\n ";
		prompt += "  0) Back\n ";
		prompt += "  1) Display all singers in system\n "; //this holds 
		prompt += "  2) Add Singer\n "; //this holds 
		prompt += "  3) View Singer\n "; //this menu h
		prompt += "  Please make a selection:\n ";
		menuSingerSelection = getInputReprompt(prompt, BACK, VIEW_SINGER);
		Singer tempSinger;
		Song tempSong;
		tm* tempDate;
		string storeInput;
		switch (menuSingerSelection)
		{
			case DISPLAYALL:
				displayMap(singerMap);
				break; 
							
			case ADD:
				do{
						tempSinger = userInputSinger();
						addObjectToMap(singerMap, tempSinger);
						primaryMapToFile(singerMap, singerFstream);
				} while (getInputReprompt("Add another singer?    0:No,  1:Yes   Enter a selection: \n", 0, 1));
			break;
			
			case VIEW_SINGER:
					viewSingerMenu();
				break;//case View break
			default: 
				cout << "Back";
				return;
		}//end menuSingerSelection switch
	}//menuSinger loop
};

void viewSingerMenu() {

	Singer tempSinger = userInputSinger();

	do
	{
		int singerOptionsSelection; //user choice within the top-of-house menu display
		enum singerOptions { BACK, ADD_HISTORY, VIEW_HISTORY }; //, REQUEST			};
		string prompt = "\n----View Singer Menu----\n ";
		prompt += "  " + to_string(BACK) + ") Back\n ";
		prompt += "  " + to_string(ADD_HISTORY) + ") Add a song to your history\n "; //this holds 
		prompt += "  " + to_string(VIEW_HISTORY) + ") View your song history\n "; //this holds 
		//prompt += "  "+to_string(REQUEST) + ") Put a song request in the queue\n "; //this menu h
		prompt += "  Please make a selection:\n ";
		singerOptionsSelection = getInputReprompt(prompt, BACK, VIEW_HISTORY);

		switch (singerOptionsSelection)
		{
		case ADD_HISTORY:
		{
			//ADD TO SONG HISTORY
			do
			{
				cout << "Add a song to your Singer History\n";
				addToSingerHistory(tempSinger.getKey());
			} while (getInputReprompt("Add a song to your song history? 0: No, 1: Yes, add a song. Enter a selection: \n", 0, 1));

		}
		break;
		case VIEW_HISTORY:
		{
			viewSingerHistory(tempSinger);

		}
		break;
		//case REQUEST:
		//{
			//ADD TO QUUEUE
					//do
					//{
					//	cout << "----Add your song request to the queue.----\n (If the song is not available, the KJ will let you know).\n";
					//	Song songRequest = userInputSong();
					//	//NEED TO DO ADD TO QUEUE
					//}while (getInputReprompt("Add a song request to the queue now? 0: No, 1: Yes, add a song. Enter a selection: \n", 0, 1));

			//break;
		//}
		default:
		{
			return;
		}
		break;
		}//end switch singerOptions
	} while (true);

};

//date time functions
struct tm* setDate(int year, int month, int day, time_t& storeTime) {
	struct tm* timeinfo;

	/* get current timeinfo and modify it to the user's choice */
	time(&storeTime);
	timeinfo = localtime(&storeTime);
	timeinfo->tm_year = year - 1900;
	timeinfo->tm_mon = month - 1;
	timeinfo->tm_mday = day;

	/* call mktime: timeinfo->tm_wday will be set */
	storeTime = mktime(timeinfo);
	return timeinfo;
};
//adapted from http://www.cplusplus.com/reference/ctime/mktime/
struct tm* setDate(int year, int month, int day)
{
	time_t rawtime;
	return setDate(year, month, day, rawtime);
};

string dateToString(tm* dateStruct) {
	char    time_buf[256];
	strftime(time_buf, sizeof(time_buf),
		"%F", dateStruct);
	return time_buf;
};

//Promput user for the components of a date, enforce ranges
tm* userInputDate() {
	const int MIN_YEAR = 1980;
	const int MAX_YEAR = 2300;
	const int MIN_MONTH = 1;
	const int MAX_MONTH = 12;
	const int MIN_DAY = 1;
	int max_day = 31;
	int month, day, year = 0;
	month = getInputReprompt("Enter the month:", MIN_MONTH, MAX_MONTH);
	switch (month) {
	case 1: max_day = 31; break;
	case 2: max_day = 29; break;
	case 3: max_day = 31; break;
	case 4: max_day = 30; break;
	case 5: max_day = 31; break;
	case 6: max_day = 30; break;
	case 7: max_day = 31; break;
	case 8: max_day = 31; break;
	case 9: max_day = 30; break;
	case 10: max_day = 31; break;
	case 11: max_day = 30; break;
	case 12: max_day = 31; break;
	default:cout << "Error, max day set to 31."; break;
	}
	day = getInputReprompt("Enter the day:", MIN_DAY, max_day);
	year = getInputReprompt("Enter the year:", MIN_YEAR, MAX_YEAR);
	return setDate(year, month, day);
};
