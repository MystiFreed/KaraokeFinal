/////////PRIMARY Maps/////save unique objects of a class with a unique key. Maps are in the .h file of the class that the map contains. This is the official record of all objects of the class. Map will be loaded at the start of program, new objects inserted into map will be saved to file.
/////////SECONDARY Multimaps/////////////save only keys to file, all the map functions like find and emplace only return iterators which are not useful to save. Lookups will be needed to use the contents of these maps
/////This file contains functions that can be used with maps

#pragma once
#include <map>
#include <iterator>
#include "Artist.h"
#include "Song.h"
#include "Singer.h"
#include <set>
const bool DEBUGMAP = false;

//map functions
template <typename T> bool SelectByKey(map<string, T>, string, T&);
template <typename T> bool UserInputSelectByKey(map<string, T>, string, string&, T&);
template <typename T>  void displayMap(map<string, T>& );
template <typename T>  bool addObjectToMap(typename map<string, T>&, T);
template<typename T> void primaryMapSeparateLineByDelimiter(map<string, T>&, string);
template<typename T> void primaryMapToFile(map<string, T>& , fstream& );
template<typename T> void primaryMapFromFile(map<string, T>& , fstream& );

//multimap functions
typedef std::multimap<string, string>::iterator MMAPIterator;//can declare iterator of this type
void addObjectToMap(typename multimap<string, string>*, string, string);
void displayMap(multimap<string, string>&);
void multiMapSeparateLineByDelimiter(multimap<string, string>& , string );
void multiMapToFile(multimap<string, string>& , fstream& );
void multiMapFromFile(multimap<string, string>& , fstream& );
bool mapResultsByKey(multimap<string, string>&, multimap<string, string>&, string);
bool mapResultsByKey(multimap<string, string>&, multimap<string, string>&, string, string);

//map management functions
bool addSongToCatalogs(Song);
string inputMapString();
Artist userInputArtist();
Song userInputSong();
Singer userInputSinger();


///////////////

//Use to collect user input if it is for one of the maps, confirms that no delimiters are part of the user input, converts to uppercase to enable comparisons later
string inputMapString() {
	string storeInput;
	getline(cin, storeInput);
	for (auto& c : storeInput) { 
		c = toupper((unsigned char)c);
		if ( c == FIELD_DELIMITER||c==ELEMENT_DELIMITER) { c = SAFE_CHAR; }//use this char in place of the delimiter characters if one of them is in user input
	};
	return storeInput;
}

//use with any ordered map that has string as the key and an object of a class that has a key field, and updateKey() and getKey() functions in the class.
//http://www.cplusplus.com/reference/map/map/emplace/ returns the bool that emplace returns (second part of pair that emplace returns)
template <typename T>  bool addObjectToMap(typename map<string, T>& existingMap, T newObject)
{
	string newKey = newObject.getKey(); //uses the key field in the object as the key for the map								
	return existingMap.emplace(newKey, newObject).second;
};
	
//use with any ordered map that has string as the key
template <typename T>  void displayMap(map<string, T>& existingMap) {
	stringstream report;
	const int WIDTH = 30;
	for (auto& element : existingMap) {
			report << setw(WIDTH) <<left<< element.second.display() << FIELD_DELIMITER << endl;
	}
	cout << report.str();
}

//search map, include instructions to show the user for entering a useful search string.
template <typename T> bool UserInputSelectByKey(map<string, T> myMap, string userInputInstructions, string& storeInput, T& storeObject)
{
	////menu management
	string sectionTitle = "";//gives a header to next menu section so it is easier to read
	string sectionPrompt = "";//defines the prompt with options for this section
	int menuSelected = -1; //stores the user input
	bool isKeepLooking = true;

	cout << userInputInstructions;
	
	string searchString = inputMapString();
	storeInput = searchString;//saves user input in address provided as parameter for use outside the function
	if (SelectByKey(myMap, searchString, storeObject))//if it is not map.end then the search found a match
	{		return true;	}

	//full key not found, try substring search and ask user if that is what they were looking for
	typename	map<string, T>::iterator iter = myMap.begin();//start searching from beginning

		while (iter != myMap.end())//stop search after end reached without match
		{
			if (iter->first.find(searchString) != std::string::npos)//possible match
			{//check if possible match is what user wants
				string sectionTitle = "\n----Select This One?---\n " + iter->first+" "+iter->second.display() + "\n";
				enum ConfirmMenu { CANCEL, SELECT, AGAIN };
				string sectionPrompt = sectionTitle + "\n  0:Cancel, go back without selection\n  1:Confirm, select this \n  2: Keep looking, see next match \nSelect an option: "; //define the prompt string.
				int menuSelected = getInputReprompt(sectionPrompt, CANCEL, AGAIN); //get input within menu option range.

				switch (menuSelected)
				{
				case CANCEL:
					return false;
					break;
				case SELECT:
					storeObject = iter->second;//found a match
					return true;
					break;
				case AGAIN:
					//keep looking not returning
					break;
				default:
					cerr << "Error in select object\n";
					return false;
					break;
				}//end switch
			}//end if

			advance(iter, 1);
		}//end while
	//}//end while
	return false;//no match after all search methods
};//end func

//search map for exact match key
template <typename T> bool SelectByKey(map<string, T> myMap, string searchString, T& storeObject)
{
	//attempt finding exact match
	typename map<string, T>::iterator iter;
	iter = myMap.find(searchString);
	if (iter != myMap.end())//if it is not map.end then the search found something and it is in the iterator
	{
		storeObject = iter->second ; //NEED copy constructor for this to work - Artist, Song, Singer
		return true;//iterator has been updated and can be used back where the function was called from.
	}
	return false;
};

//overwrites current contents of the file
template<typename T> void primaryMapToFile(map<string, T>& myMap, fstream& myFstream)
{
	GoBeginningOfFile(myFstream);
	for (auto& e : myMap)
	{
		myFstream <<e.first << FIELD_DELIMITER << e.second.toFile() << FIELD_DELIMITER << endl;//put key and class output all on a line with field delimiter between key and value
	}
	GoBeginningOfFile(myFstream);
};
//parses file into map
template<typename T> void primaryMapFromFile(map<string, T>& myMap, fstream& inputFile)
{
	GoBeginningOfFile(inputFile);
	while (inputFile)//keep going until end of file
	{
		string line = "";
		getline(inputFile, line, '\n');
		if (line == "") {}//skip blank line
		else		{			primaryMapSeparateLineByDelimiter(myMap, line);		}
	}
	GoBeginningOfFile(inputFile);

};
//separates text line with delimiters into expected fields
template<typename T> void primaryMapSeparateLineByDelimiter(map<string, T>& myMap, string line)
{
	vector<string> fields = SeparateLineByDelimiter(line, FIELD_DELIMITER);
	vector<string>::iterator iter = fields.begin();
	string tempKey = *iter;//stores contents of first item in vector as the key
	T tempObject = T();
	iter++;//iterator now points to the first class field that is saved as a string in the vector
	tempObject.fromFile(iter);//pass iterator to class's FromFile() to update the class fields of the object (it will be different for each class)
	bool check = myMap.emplace(make_pair(tempKey, tempObject)).second; //add to map; .second collects the bool portion of what emplace returns
}


///MULTIMAP FUNCTIONS

//overwrites current contents of the file
void multiMapToFile(multimap<string, string>& myMap, fstream& myFstream)
{
	GoBeginningOfFile(myFstream);
	for (auto& e : myMap)
	{
		myFstream << e.first << FIELD_DELIMITER << e.second << FIELD_DELIMITER << endl;//put key and class output all on a line with field delimiter between key and value
	}
	GoBeginningOfFile(myFstream);
};
 void multiMapFromFile(multimap<string, string>& myMap, fstream& inputFile)
{
	GoBeginningOfFile(inputFile);
	while (inputFile)//keep going until end of file
	{
		string line = "";
		getline(inputFile, line, '\n');

		if (line == "") {}//skip blank line
		else
		{
			multiMapSeparateLineByDelimiter(myMap, line);
		}
	}
	GoBeginningOfFile(inputFile);
};
void multiMapSeparateLineByDelimiter(multimap<string, string>& myMap, string line)
{
	vector<string> fields = SeparateLineByDelimiter(line, FIELD_DELIMITER);
	vector<string>::iterator iter = fields.begin();
	string tempKey = *iter;//stores contents of first item in vector as the key
	iter++;//iterator now points to the first class field that is saved as a string in the vector
	string tempValue = *iter;
	myMap.emplace(make_pair(tempKey, tempValue)); //add to map;
}

//multimap, shows all contents on screen
 void displayMap(multimap<string, string>& existingMap) {
	 stringstream report;
	 const int WIDTH = 30;
	 for (auto& element : existingMap) {
		 report << setw(WIDTH) <<left<< element.first << FIELD_DELIMITER << setw(WIDTH) <<left<<element.second<< FIELD_DELIMITER <<endl;
	 }
	 cout << report.str();
 }
 
 //multimap version, adds string, string pair to multimap
//http://www.cplusplus.com/reference/map/map/emplace/ returns the bool that emplace returns (second part of pair that emplace returns)
void addObjectToMap(typename multimap<string, string>* existingMap, string newKey, string newValue)
{
	existingMap->emplace(newKey, newValue);
};


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



/////////functions and user menus specific to individual maps we are using /////////////////
//create artist object, add to map
Artist userInputArtist() {
	if (DEBUGMAP) cout << "\nstarting userInputArtist\n";

	string alphaName = "";
	string displayName = "";
	string instructions= "\nArtist name, alphabetical (Move \", The \" to the end of the artist name if applicable) :";
	Artist tempArtist;
	if (UserInputSelectByKey(artistMap, instructions, alphaName, tempArtist)) { return tempArtist; 
	}//if found, return existing 
	else {
		string prompt = "Not found, add new Artist?\n  0. Cancel new artist. \n  1: Use \"" + alphaName + "\" as display name for artist. \n  2: Enter a different display name. \n Enter a selection: \n";
		int userSelection = getInputReprompt(prompt, 0, 2);
		switch (userSelection) {
		case 1:
			tempArtist = Artist(alphaName);
			addObjectToMap(artistMap, tempArtist);
			break;
		case 2:
			cout << "Enter the display name for the artist (not alphabetical sorting):";
			displayName = inputMapString();
			tempArtist = Artist(alphaName, displayName);//not found, return created one
			addObjectToMap(artistMap, tempArtist);
			break;
		default:
			tempArtist = Artist(); //no entry, use blank
			break;
		}
	}
return tempArtist;
};

//create song object, add to map
Song userInputSong() {
	string songTitle = "";
	Song tempSong;
	string instructions = "\nSong Title:";
	if (UserInputSelectByKey(songMap, instructions, songTitle, tempSong)) { return tempSong; };//if found, return existing 
	cout << songTitle<<" was not found. Enter artist to add the song record:\n";
	Artist tempArtist = userInputArtist();
	string tempArtistKey = tempArtist.getKey();
	tempSong = Song(songTitle, tempArtistKey);
	if (getInputReprompt("Add new Song ( " + tempSong.getKey() + ")?    0:No,   1:Yes", 0, 1)) {
		addSongToCatalogs(tempSong);
	}
	else {
		tempSong = Song();//cancel, return blank instead
	}
	return tempSong;
};
Song userInputSong(Artist tempArtist) {
	string songTitle = "";
	string tempArtistKey = tempArtist.getKey();
	cout<< "\nSong Title:";
	songTitle = inputMapString();
	Song tempSong = Song(songTitle, tempArtistKey);
	if (SelectByKey(songMap, tempSong.getKey(), tempSong)) { return tempSong; };//if found, return existing 
	if (getInputReprompt("Add new Song ( " + tempSong.getKey() + ")?    0:No,   1:Yes", 0, 1)) {
		addSongToCatalogs(tempSong);
	}
	else {
		tempSong = Song();//cancel, return blank instead
	}
	return tempSong;
};


//create singer object, add to map
Singer userInputSinger() {
	if (DEBUGMAP) cout << "\nstarting userInputSinger\n";
	bool uniqueUsername = false;
	string singerKey = "";
	string instructions = "\nEnter a username:";
	Singer tempSinger;
	do {
		if (UserInputSelectByKey(singerMap, instructions, singerKey, tempSinger)) {
			//cout << "This profile already exists, username: " << tempSinger.getKey() << " display name: " << tempSinger.getDisplayName() << endl;
			//int userSelection = getInputReprompt(" Is this you? 1) Yes, use this profile 2) No, create new profile", 1, 2);
			//if (userSelection == 1) { 
				return tempSinger; //if found, return existing 
			//};
		}
		else {
			cout << singerKey<<"\n";
			uniqueUsername = true;
			tempSinger = Singer(singerKey);
		}
	} while (!uniqueUsername);//continue promting and checking if exists until username is unique
	
	cout << "Enter a display name to be shown on screen. This may be different from the username.";
	string tempDisplayName="";
	tempDisplayName = inputMapString();
	tempSinger.setDisplayName(tempDisplayName);
	return tempSinger;//not found, return created one
};

//new songs get inserted in all relevant maps
bool addSongToCatalogs(Song newSong) {
	if (DEBUGMAP)cout <<endl<< "addSongToCatalogs: " << newSong.display()<<endl;
	if (!addObjectToMap(songMap, newSong))
	{
		cout << "Already exists in catalog, not added."; return false;
	}
	addObjectToMap(&songCatalogByArtist, newSong.getArtistKey(), newSong.getTitle());
	return true;
};
