#pragma once
#pragma once
#include <set>
#include <string>
#include "CatalogEntry.h"
#include "FileManagement.h"
#include "MapManagement.h"
#include"Song.h"
#include "DateCalcLong.h"

//functions
void addToSingerHistory(string , tm* , string );
void addToSingerHistory(string , int , int , int , string );
void addToSingerHistory(string);
bool mapResultsByKey(multimap<string, string>& , multimap<string, string>& , string );
bool mapResultsByKey(multimap<string, string>& , multimap<string, string>& , string , string );
void viewSingerMenu();

void viewSingerHistory(multimap<string, string> );



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


void viewSingerHistory(Singer selectedSinger) {
	multimap<string, string> singerCompositeMap;
	if (mapResultsByKey(allSingerHistoryMap, singerCompositeMap, selectedSinger.getKey())) {
		viewSingerHistory(singerCompositeMap);
	}
	else {
		cout << "No history for this singer.";
	}

}

//can be used with individual singer history or allSingerHistory
void viewSingerHistory(multimap<string, string> singerCompositeMap) {
	
	multimap<string, string> dateSongMap;//temporary map to be filled with all songs this singer has done, by date

	//split the value string into fields, map with date key and songkey value	
	for (auto& iter : singerCompositeMap) {
		string value = iter.second;//value is a string that includes delimiters
		//cout << endl << "test" << value<<endl;
		vector<string> tempFields = SeparateLineByDelimiter(value, ELEMENT_DELIMITER);//split on the delimiters
		//cout << endl << "test tempFields[0]"<< tempFields[0]<<"  tempFields[1] "<<tempFields[1] << endl;
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

	//do stuff with the results
	//heading
	auto iter = dateSongMap.begin();
	cout << "\n\n----Results for " << iter->first<< " - ";
	auto r_iter = dateSongMap.rbegin();
	cout << r_iter->first << "----"<<endl;
	////done printing heading
	
	multimap<int, string, greater <int> > descendingMap;
	
	///////display song count in descending order///////
	cout << "\n---Number of times performing song----\n";
	for (auto& e : uniqueSongs) {
		string song = e.first;
		descendingMap.emplace(make_pair(songDateMap.count(song), song));
	}
	for (auto& e : descendingMap) {
		cout << e.first <<": "<<e.second << endl;
	}
	///////display song count in descending order///////
	cin.get();

	///////display artist count in descending order///////
	cout << "\n---Number of times songs by artist were performed----\n";
	descendingMap.clear();
	for (auto& e : uniqueArtists) {
		descendingMap.emplace(make_pair(artistSongMap.count(e), e));
	}
	for (auto& e : descendingMap) {
		cout << e.first << ": " << e.second << endl;
	}
	///////display artist count in descending order///////
	cin.get();

	///////display artist unique song count in descending order///////
	cout << "\n---Number of songs by artist that were performed----\n";
	descendingMap.clear();
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
	///////display artist unique song count in descending order///////
	cin.get();
}


//adapted from https://thispointer.com/finding-all-values-for-a-key-in-multimap-using-equals_range-example/
bool mapResultsByKey(multimap<string, string>& searchMap, multimap<string, string>& resultMap, string keyMatch) {
	return mapResultsByKey(searchMap, resultMap, keyMatch, keyMatch);
}

//adapted from https://www.geeksforgeeks.org/searching-map-using-stdmap-functions-c/
bool mapResultsByKey(multimap<string, string>& searchMap, multimap<string, string>& resultMap, string keyMin, string keyMax)
{
	std::pair<MMAPIterator, MMAPIterator> matchRange;
	matchRange.first = searchMap.lower_bound(keyMin);
	matchRange.second = searchMap.upper_bound(keyMax);

	if (distance(matchRange.first, matchRange.second) == 0) {
		return false; //none found 
	}
	for (MMAPIterator iter = matchRange.first; iter != matchRange.second; iter++)
	{
		addObjectToMap(&resultMap, iter->first, iter->second);	
	}
	return true;
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
				} while (getInputReprompt("Add another singer? 0: No, 1: Yes. Enter a selection: ", 0, 1));
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
			} while (getInputReprompt("Add a song to your song history? 0: No, 1: Yes, add a song. Enter a selection: ", 0, 1));

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
					//}while (getInputReprompt("Add a song request to the queue now? 0: No, 1: Yes, add a song. Enter a selection: ", 0, 1));

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