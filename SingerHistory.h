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



void 			historyOptionsMenu(Singer);

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

//get individual singer from all history. Format is a map with the singerkey as key and the value is a string separated by delimiters.
bool getSingerHistory(multimap<string, string>& storeSingerMap, string singerKey) {
	multimap<string, string>tempSingerMap;
		if (mapResultsByKey(allSingerHistoryMap, storeSingerMap, singerKey)) {
			return true;
		}
	return false;
}



//can be used with individual singer history or allSingerHistory
void viewSingerHistory(Singer selectedSinger) {
	multimap<string, string> singerCompositeMap;
	getSingerHistory(singerCompositeMap, selectedSinger.getKey());

	multimap<string, string> dateSongMap;//temporary map to be filled with all songs this singer has done, by date
	multimap<string, string> songDateMap;//temporary map to be filled with all songs this singer has done, by song
	multimap<string, string> artistSongMap;//temporary map to be filled with all songs this singer has done, by artist
	set<string> uniqueSongs;
	set<string> uniqueArtists;

	//split the value string into fields, map with date key and songkey value	
	for (auto& iter : singerCompositeMap) {
		string value = iter.second;//value is a string that includes delimiters
		//cout << endl << "test" << value<<endl;
		vector<string> tempFields = SeparateLineByDelimiter(value, ELEMENT_DELIMITER);//split on the delimiters
		//cout << endl << "test tempFields[0]"<< tempFields[0]<<"  tempFields[1] "<<tempFields[1] << endl;
		addObjectToMap(&dateSongMap, tempFields[0], tempFields[1]);//add to new map using first split field as the key and second as the value
	}//now dateSongMap includes only entries for the selected singer, with date as key and song as the value
//	cout << endl << "test"; for (auto& e : dateSongMap) { cout << "first" << e.first << "second" << e.second << endl; };

	//fill the other maps that are indexed by songkey and artistkey
	for (auto& iter : dateSongMap) {
		string song = iter.second;//songkey
		Song tempSong;
		SelectByKey(songMap, song, tempSong);
		string artist = tempSong.getArtistKey();
		addObjectToMap(&songDateMap, song, iter.first);
		addObjectToMap(&artistSongMap, artist, song);
		uniqueSongs.emplace(song);
		uniqueArtists.emplace(artist);
	}

	//do stuff with the results
	//heading
	//auto iter = dateSongMap.begin();
	//cout << "Results for " << iter->first<< " - ";
	//iter = dateSongMap.end();
	//cout <<  iter->first << endl;
	////done printing heading
	
	map<int, string, greater <int> > descendingMap;
	
	///////display song count in descending order///////
	cout << "\n---Number of times performing song---\n";
	for (auto& e : uniqueSongs) {
		descendingMap.emplace(make_pair(songDateMap.count(e), e));
	}
	for (auto& e : descendingMap) {
		cout << e.first <<": "<<e.second << endl;
	}
	///////display song count in descending order///////


	///////display artist count in descending order///////
	cout << "\n---Number of times performing songs by artist---\n";
	for (auto& e : uniqueArtists) {
		descendingMap.emplace(make_pair(artistSongMap.count(e), e));
	}
	for (auto& e : descendingMap) {
		cout << e.first << ": " << e.second << endl;
	}
	///////display artist count in descending order///////
}

//can be used with individual singer history or allSingerHistory
void viewSingerHistory(multimap<string, string>& SingerMap, tm* startDate, tm* endDate) {
	string minDate = dateToString(startDate);
	string maxDate = dateToString(endDate);

	multimap<string, string> tempDateMap;
	multimap<string, string> tempDateRangeMap;
	multimap<string, string> tempSingerSongMap;
	multimap<string, string> tempSingerArtistMap;
	set<string> uniqueSongs;
	set<string> uniqueArtists;

	//split the value string into fields, map with date key and songkey value	
	for (auto& iter : SingerMap) {
		string value = iter.second;//value is a string that includes delimiters
		vector<string> tempFields = SeparateLineByDelimiter(value, FIELD_DELIMITER);//split on the delimiters
		addObjectToMap(&tempDateMap, tempFields[0], tempFields[1]);//add to new map using first split field as the key and second as the value
	}//now tempDateMap includes only entries for the selected singer, with date as key and song as the value

	//filter map down to date range
	mapResultsByKey(tempDateMap, tempDateRangeMap, minDate, maxDate);

	//fill the other maps that are indexed by songkey and artistkey
	for (auto& iter : tempDateRangeMap) {
		string song = iter.second;//songkey
		Song tempSong;
		SelectByKey(songMap, song, tempSong);
		string artist = tempSong.getArtistKey();
		addObjectToMap(&tempSingerSongMap, song, iter.first);
		addObjectToMap(&tempSingerArtistMap, artist, song);
		uniqueSongs.emplace(song);
		uniqueArtists.emplace(artist);
	}

	//do stuff with the results
	cout << "All";
	for (auto& e : uniqueSongs) {
		cout << e << "  count:" << tempSingerSongMap.count(e) << endl;
	}
	for (auto& e : uniqueArtists) {
		cout << e << "  count:" << tempSingerArtistMap.count(e) << endl;
	}
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
	do {
		int userSelection; //user choice within the top-of-house menu display
		enum roleOptions { BACK, DISPLAYALL, ADD, VIEW, SONGHISTORY, EXIT };
		string prompt = "\n----Singer Selection Menu----\n ";
		prompt += "0) Back\n ";
		prompt += "1) Display all singers in system\n "; //this holds 
		prompt += "2) Add Singer\n "; //this holds 
		prompt += "3) View Singer\n "; //this menu h
		prompt += "4) Song History\n "; //this menu 
		prompt += "Please make a selection:\n ";
		userSelection = getInputReprompt(prompt, BACK, SONGHISTORY);
		Singer tempSinger;
		Song tempSong;
		tm* tempDate;
		string storeInput;
		switch (userSelection)
		{
		case DISPLAYALL:
		{	displayMap(singerMap);
		break; }
		case ADD:
			
		{
			do 
			{
				tempSinger = userInputSinger();
				addObjectToMap(singerMap, tempSinger);
				primaryMapToFile(singerMap, singerFstream);
			} while (getInputReprompt("Add another singer? 0: No, 1: Yes. Enter a selection: ", 0, 1));
		break;
		}
		case VIEW:
		{	tempSinger = userInputSinger();
						//ADD TO QUUEUE
						//while (getInputReprompt("Add a song request to the queue now? 0: No, 1: Yes, add a song. Enter a selection: ", 0, 1))
						//{
						//	cout << "----Add your song request to the queue.----\n (If the song is not available, the KJ will let you know).\n";
						//	Song songRequest = userInputSong();
						//	//NEED TO DO ADD TO QUEUE
						//};

			historyOptionsMenu(tempSinger);


			//ADD TO SONG HISTORY
			while (getInputReprompt("Add a song to your song history? 0: No, 1: Yes, add a song. Enter a selection: ", 0, 1))
			{
				cout << "Add a song to your Singer History\n";
				addToSingerHistory(tempSinger.getKey());
			};
			break;//case View break
		}
		case SONGHISTORY:
		{	displayMap(allSingerHistoryMap);break; 
		}
		default: {
			return;
			break; }
		};
	
	} while (true);
};

void historyOptionsMenu(Singer selectedSinger) {

	//VIEW SINGER HISTORY
	//multimap<string, string> tempSingerMap;
	viewSingerHistory(selectedSinger);

	//enum historyOptions { BACK, LAST90, LAST30 };
	//string prompt = "\n----Singer History Menu----\n ";
	//prompt += to_string(historyOptions::BACK) + ") Back\n ";
	////prompt += to_string(historyOptions::ALL) + ") All history\n "; //this holds 
	//prompt += to_string(historyOptions::LAST90) + ") Last 90 days\n "; //this holds 
	//prompt += to_string(historyOptions::LAST30) + ") Last 30 days\n "; //this menu h
	//prompt += "Please make a selection:\n ";
	//int userSelection = getInputReprompt(prompt, BACK, LAST30);
	//time_t     now = time(0);
	//tm* startDate = localtime(&now);//https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
	//tm* endDate = localtime(&now);//https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
	//switch (userSelection)
	//{
	//	//case ALL:
	//	
	//case LAST90:
	//	addDays(startDate, -90);
	//	viewSingerHistory(tempSingerMap, startDate, endDate);
	//	break;
	//case LAST30:
	//	addDays(startDate, -30);
	//	viewSingerHistory(tempSingerMap, startDate, endDate);
	//	break;
	//default:
	//	return;
	//	break;
	//
	//};//end history options}
};