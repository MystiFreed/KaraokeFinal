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
template <typename T> bool SelectByKey(map<string, T>, string, T&);
template <typename T> bool UserInputSelectByKey(map<string, T>, string, string&, T&);

template <typename T>  void displayMap(map<string, T>& );
template <typename T>  bool addObjectToMap(typename map<string, T>&, T);
template<typename T> void primaryMapSeparateLineByDelimiter(map<string, T>&, string);
template<typename T> void primaryMapToFile(map<string, T>& , fstream& );
template<typename T> void primaryMapFromFile(map<string, T>& , fstream& );

typedef std::multimap<string, string>::iterator MMAPIterator;
void addObjectToMap(typename multimap<string, string>*, string, string);
void displayMap(multimap<string, string>&);
void multiMapSeparateLineByDelimiter(multimap<string, string>& , string );
void multiMapToFile(multimap<string, string>& , fstream& );
void multiMapFromFile(multimap<string, string>& , fstream& );

string inputMapString(const string);
string inputMapString();

//Use to collect user input if it is for one of the maps, confirms that no delimiters are part of the user input, converts to uppercase to enable comparisons later
string inputMapString() {
	string storeInput;
	getline(cin, storeInput);
	return  inputMapString(storeInput);
}
//Use to convert string for use with one of the maps, confirms that no delimiters are part of the user input, converts to uppercase to enable comparisons later
string inputMapString(const string existingInput) {
	string storeInput= existingInput;
	for (auto& c : storeInput) { 
		c = toupper((unsigned char)c);
		if (c == ELEMENT_DELIMITER || c == FIELD_DELIMITER) { c = SAFE_CHAR; }//use this char in place of the delimiter characters if one of them is in user input
	};
	return storeInput;
}

void clearCin() {
	//std::cin.clear();
	//std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
//use with any ordered map that has string as the key and an object of a class that has a key field, and updateKey() and getKey() functions in the class.
//http://www.cplusplus.com/reference/map/map/emplace/ returns the bool that emplace returns (second part of pair that emplace returns)
template <typename T>  bool addObjectToMap(typename map<string, T>& existingMap, T newObject)
{
	string newKey = newObject.getKey(); //uses the key field in the object as the key for the map								
	return existingMap.emplace(newKey, newObject).second;

};
	
//use with any ordered map that has string as the key, for testing only - replace with save to file
template <typename T>  void displayMap(map<string, T>& existingMap) {
	for (auto& element : existingMap) {
		cout << "Key:" <<element.first << " Object: " << element.second.display() << endl;
	}
};

//include instructions to show the user for entering a useful search string.
template <typename T> bool UserInputSelectByKey(map<string, T> myMap, string userInputInstructions, string& storeInput, T& storeObject)

{
	if (DEBUGMAP) cout << "\nstarting UserInputSelectByKey\n";

	////menu management
	string sectionTitle = "";//gives a header to next menu section so it is easier to read
	string sectionPrompt = "";//defines the prompt with options for this section
	int menuSelected = -1; //stores the user input

	
	bool isKeepLooking = true;

	cout << userInputInstructions;
	string searchString = inputMapString();
	storeInput = searchString;//saves user input in address provided as parameter for use outside the function
	if (DEBUGMAP) cout << "\n searchString: " << searchString <<" storeInput: "<< storeInput << endl;
	if (SelectByKey(myMap, searchString, storeObject))//if it is not map.end then the search found a match
	{		return true;	}
	if (DEBUGMAP) cout << "\n no selectbykey found, continuing with substring search\n";

	//full key not found, try substring search and ask user if that is what they were looking for
	typename	map<string, T>::iterator iter = myMap.begin();//start searching from beginning


	//while (isKeepLooking)
	//{
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
					storeObject = iter->second;
			
					return true;
					break;
				case AGAIN:
					//keep looking not returning
					//isKeepLooking = true;
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
	return false;
};//end func

//include instructions to show the user for entering a useful search string.
template <typename T> bool SelectByKey(map<string, T> myMap, string searchString, T& storeObject)
{
	if (DEBUGMAP) cout << "\nstarting SelectByKey\n";
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
	for (auto& e : myMap)
	{
		myFstream <<e.first << FIELD_DELIMITER << e.second.toFile() << FIELD_DELIMITER << ELEMENT_DELIMITER << endl;//put key and class output all on a line with field delimiter between key and value
	}
	cout << "\nDone writing map to file.\n";
	GoBeginningOfFile(myFstream);
};
template<typename T> void primaryMapFromFile(map<string, T>& myMap, fstream& inputFile)
{
	GoBeginningOfFile(inputFile);
	while (inputFile)//keep going until end of file
	{
		string line = "";
		getline(inputFile, line, '\n');

		if (line == "")
		{//skip blank line
		}
		else
		{
			primaryMapSeparateLineByDelimiter(myMap, line);
		}

	}
	if(DEBUGMAP) cout << "\nDone reading map from file.\n";
	GoBeginningOfFile(inputFile);

};
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

///MULTIMAP

//overwrites current contents of the file
void multiMapToFile(multimap<string, string>& myMap, fstream& myFstream)
{
	for (auto& e : myMap)
	{
		myFstream << e.first << FIELD_DELIMITER << e.second << FIELD_DELIMITER << ELEMENT_DELIMITER << endl;//put key and class output all on a line with field delimiter between key and value
	}
	cout << "\nDone writing map to file.\n";
	GoBeginningOfFile(myFstream);
};
 void multiMapFromFile(multimap<string, string>& myMap, fstream& inputFile)
{
	GoBeginningOfFile(inputFile);
	while (inputFile)//keep going until end of file
	{
		string line = "";
		getline(inputFile, line, '\n');

		if (line == "")
		{//skip blank line
		}
		else
		{
			multiMapSeparateLineByDelimiter(myMap, line);
		}
	}
	cout << "\nDone reading map from file.\n";
	GoBeginningOfFile(inputFile);
};
void multiMapSeparateLineByDelimiter(multimap<string, string>& myMap, string line)
{
	vector<string> fields = SeparateLineByDelimiter(line, FIELD_DELIMITER);

	vector<string>::iterator iter = fields.begin();
	string tempKey = *iter;//stores contents of first item in vector as the key
	
	iter++;//iterator now points to the first class field that is saved as a string in the vector
	string tempValue = *iter;
//	myMap.emplace(make_pair(tempKey, tempValue)); //add to map;
}
 void displayMap(multimap<string, string>& existingMap) {

	for (auto& element : existingMap) {
		cout << "Key:" << element.first << " Object: " << element.second << endl;
	}
};
//multimap version
//http://www.cplusplus.com/reference/map/map/emplace/ returns the bool that emplace returns (second part of pair that emplace returns)
void addObjectToMap(typename multimap<string, string>* existingMap, string newKey, string newValue)
{
	existingMap->emplace(newKey, newValue);
};

///from maps.h NEED to figure out the best loacation for these.

/////functions for maps
Artist userInputArtist();
Song userInputSong();
bool addSongToCatalogs(Song);

/////////functions and user menus specific to individual maps we are using (generic map functions are in MapManagement.h)/////////////////

Artist userInputArtist() {
	if (DEBUGMAP) cout << "\nstarting userInputArtist\n";

	string alphaName = "";
	string displayName = "";
	string instructions= "\nArtist name, alphabetical (Move \", The \" to the end of the artist name if applicable) :";
	Artist tempArtist;
	if (UserInputSelectByKey(artistMap, instructions, alphaName, tempArtist)) { return tempArtist; 
	}//if found, return existing 
	else {
		string prompt = "Not found, add new Artist? \n1: Use \"" + alphaName + "\" as display name for artist. \n2: Enter a different display name. \n3. Cancel new artist. Enter a selection: ";
		int userSelection = getInputReprompt(prompt, 1, 3);
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

Song userInputSong() {
	if (DEBUGMAP) cout << "\nstarting userInputSong\n";
	string songTitle = "";
	Song tempSong;
	string instructions = "\nSong Title:";
	if (UserInputSelectByKey(songMap, instructions, songTitle, tempSong)) { return tempSong; };//if found, return existing 
	cout << songTitle<<" was not found. Enter artist and song information to add the song record:\n";
	Artist tempArtist = userInputArtist();
	string tempArtistKey = tempArtist.getKey();
	tempSong = Song(songTitle, tempArtistKey);
	addSongToCatalogs(tempSong);
	return tempSong;
};

Singer userInputSinger() {
	if (DEBUGMAP) cout << "\nstarting userInputSinger\n";
	bool uniqueUsername = false;
	string singerKey = "";
	string instructions = "\nEnter a username:";
	Singer tempSinger;
	do {
		if (UserInputSelectByKey(singerMap, instructions, singerKey, tempSinger)) {
			cout << "This profile already exists, username: " << tempSinger.getKey() << " display name: " << tempSinger.getDisplayName() << endl;
			int userSelection = getInputReprompt(" Is this you? 1) Yes, use this profile 2) No, create new profile", 1, 2);
			if (userSelection == 1) { 
				return tempSinger; };//if found, return existing 
		}
		else {
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

//new songs must be inserted in all relevant maps
bool addSongToCatalogs(Song newSong) {
	if (DEBUGMAP)cout <<endl<< "addSongToCatalogs: " << newSong.display()<<endl;
	if (!addObjectToMap(songMap, newSong))
	{
		cout << "Already exists in catalog, not added."; return false;
	}
	addObjectToMap(&songCatalogByArtist, newSong.getKey(), newSong.getArtistKey());
	return true;
};



/////////////////////////////////////////////////////////////////
///////////////////////may not need this/////////////////////////
///////////////////////////////////////////////////////////////////
//use with any ordered map that has int as the key, will return next higher int that is available to use as a new key
//adapted from https://stackoverflow.com/questions/1660195/c-how-to-find-the-biggest-key-in-a-stdmap
//use crbegin to get pointer to last element which will have highest key http://www.cplusplus.com/reference/map/map/crbegin/
template <typename T> int createNextKey(map<int, T>& existingMap)
{
	const int STARTING_KEY = 100;

	if (existingMap.empty()) return STARTING_KEY;
	int largestKey = existingMap.crbegin()->first;
	return (++largestKey);
}
