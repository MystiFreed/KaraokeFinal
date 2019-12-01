#pragma once
#include "Singer.h"
#include "Song.h"
#include <iostream>
class SongRequest { 
public:
	Singer reqSinger; 
	Song reqSong; 


	bool operator==(const SongRequest& a2)
	{

		if (reqSinger == a2.reqSinger && reqSong == a2.reqSong)
		{
			return true;
		}
		return false;
	}
	string display()
	{
		return reqSinger.display() + " / " + reqSong.display();
	}
};

inline bool operator==(SongRequest& s1, SongRequest& s2) { return (s1.reqSong == s2.reqSong && s1.reqSinger == s2.reqSinger); }