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
void addToSingerHistory(string singerKey, tm* performedDate, string songKey);
void addToSingerHistory(string singerKey, int year, int month, int day, string songKey);
bool mapResultsByKey(multimap<string, string>& , multimap<string, string>& , string );
bool mapResultsByKey(multimap<string, string>& , multimap<string, string>& , string , string );




multimap<string, string> allSingerHistoryMap; //<singerkey, dateAsString$songKey

string allSingerHistoryTXT = "allSingerHistory.txt";
fstream singerHistoryFstream;


void addToSingerHistory(string singerKey, int year, int month, int day, string songKey) {
	tm* performedDate = setDate(year, month, day);
	addToSingerHistory(singerKey, performedDate, songKey);
}

void addToSingerHistory(string singerKey, tm* performedDate, string songKey) {
	string dateString = dateToString(performedDate);
	string newValue = dateString + FIELD_DELIMITER + songKey + FIELD_DELIMITER;
	addObjectToMap(&allSingerHistoryMap, singerKey, newValue);

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
		return true;
	}
}