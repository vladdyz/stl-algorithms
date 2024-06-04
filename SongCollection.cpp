//Vladislav Zolotukhin
//Date: 2024-03-16
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <numeric>
#include <algorithm>
#include "SongCollection.h"

void printbar(std::ostream& out = std::cout); //forward declaration
namespace vz {

	//within the boundary of a function, you can use an algorithm api multiple times
	//outside the boundaries, you can only use each once!

	SongCollection::SongCollection(const char* filename) {

		std::ifstream file(filename);
		if (file) {
			//TITLE ARTIST ALBUM YEAR LENGTH PRICE
			std::string token;
			struct Song song;
			int seconds; 
			while (std::getline(file, token)) {
				//making the struct using substrings with defined lengths from the file
				//then using the trimWS function I made earlier
				song.m_title = token.substr(0, 25);
				song.m_title = trimWS(song.m_title);
				song.m_artist = token.substr(25, 25);
				song.m_artist = trimWS(song.m_artist);
				song.m_album = token.substr(50, 25);
				song.m_album = trimWS(song.m_album);
				song.m_year = token.substr(75, 4); //year should be a string because its not always provided! sometimes it's a blank field.
				seconds = std::stoi(token.substr(80, 5)); //seconds must be converted into a minutes:seconds string (MM:SS)
				int min = seconds / 60;
				int sec = seconds % 60;
				std::stringstream time; //have to convert it into a string stream to assign to the string struct member
				time << min << ":" << std::setfill('0') << std::setw(2) << sec; //also need to format it properly!
				song.m_length = time.str();
				song.m_price = std::stod(token.substr(85, 4));
				songs.push_back(song); //add the struct into the vector
			}
		}
		else {
			throw std::runtime_error("Invalid File!");
		}
	}

	void SongCollection::SongCollection::display(std::ostream& out) const {
		//accumulate
		std::for_each(songs.begin(), songs.end(), [&out](const Song& song) { out << song << std::endl; });
		printbar();
		//total length of all the songs in the vector is a bit tricky because the length has been formatted to a string
		//need to convert it back into an integer value representing seconds and then use accumulate from numeric library
		int totalTime = std::accumulate(songs.begin(), songs.end(), 0,
			[](int accumulator, const Song& song) {
				return accumulator + toSeconds(song.m_length);
			}
		);
		int hours = totalTime / 3600;
		totalTime %= 3600;
		int minutes = totalTime / 60;
		int seconds = totalTime % 60;
		std::stringstream accumulatedTime;
		accumulatedTime << hours << ":"
			<< std::setw(2) << std::setfill('0') << minutes << ":"
			<< std::setw(2) << std::setfill('0') << seconds;

		out << "| " << std::right << std::setw(77) << "Total Listening Time: " << accumulatedTime.str() << std::right << " |\n";

	}

	void SongCollection::sort(const char* sorter) { //using sort
		//I really wanted to use an enum for this
		enum sortType { title, album, length};
		sortType type;
		bool okay = true; //assuming the sorting parameter is correct
		std::string sorterStr(sorter);
		if (sorterStr == "title") type = sortType::title;
		else if (sorterStr == "album") type = sortType::album;
		else if (sorterStr == "length") type = sortType::length;
		else okay = false;

		if (okay) {
			switch (type) {
			case sortType::title:
				//sort by title
				std::sort(songs.begin(), songs.end(), [](const Song& a, const Song& b) {
					//the values being compared are strings here, so doing a < b won't work correctly
					//must use strcmp instead!
					return strcmp(a.m_title.c_str(), b.m_title.c_str()) < 0;
					});
				break;
			case sortType::album:
				//sort by album
				std::sort(songs.begin(), songs.end(), [](const Song& a, const Song& b) {
					return strcmp(a.m_album.c_str(), b.m_album.c_str()) < 0;
					});
				break;
			case sortType::length:
				//sort by length and convert strings back into seconds
				std::sort(songs.begin(), songs.end(), [](const Song& a, const Song& b) {
					int lengthA = toSeconds(a.m_length);
					int lengthB = toSeconds(b.m_length);
					return lengthA < lengthB;
					});
				break;
			}
		}
	}

	void SongCollection::cleanAlbum() { //forEach
		//removes the token [None] from the album field of the songs that do not have a valid album
		//use replace_if to only replace those Song objects in the vector that meet the above condition
		std::transform(songs.begin(), songs.end(), songs.begin(), [](Song& song) {
			if (song.m_album == "[None]")
				song.m_album = " ";
			return song;
			});
	}

	bool SongCollection::inCollection(const char* artist) const { //any_of
		//receives the name of an artist as a parameter, and returns true if the collection contains any song by that artist
		//use bool any_of since we only need to find at least one match in the vector
		if (std::any_of(songs.begin(), songs.end(), [artist](const Song& song) { return song.m_artist == artist; })) {
			return true;
		}
		else return false;
	}

	std::list<Song> SongCollection::getSongsForArtist(const char* artist) const { //copy_if
		//receives the name of an artist as a parameter, and returns the list of songs of that artist available in the collection.
		//we need to return a new collection (a list), so use copy_if because we need to meet a specific condition
		std::list<Song> artistSongs;
		std::copy_if(songs.begin(), songs.end(), std::back_inserter(artistSongs), [artist](const Song& song) { return song.m_artist == artist; });
		return artistSongs;
	}
	//Code written for STL algorithms workshop, must successfully complete without reusing an algorithm - Vladislav Zolotukhin Date: 2024-03-16

	//This function takes a reference to a string, will trim any leading or trailing whitespace from the string
	//Afterward, it will return the trimmed string
	//Borrowed from my Toy module
	std::string trimWS(std::string& tokenstr) {
		std::string trimmed;
		size_t first, last;
		if (tokenstr[0] != '\0') {
			//searching for the index of the first non-whitespace character
			first = tokenstr.find_first_not_of(" \t\n\r");
			last = tokenstr.find_last_not_of(" \t\n\r");
			//create a substring out of the token string using the first and last indexes
			trimmed = tokenstr.substr(first, last - first + 1);
			return trimmed; //return the trimmed string so it could be assigned to the name attribute
		}
		else return "Invalid string!"; //if the string has a length of 0 it hasn't been initialized

	}

	std::ostream& operator<<(std::ostream& out, const Song& theSong) {
		out << "| ";
		out.unsetf(std::ios::right);
		out.setf(std::ios::left);
		out.width(20);
		out << theSong.m_title;
		out << " | ";
		out.width(15);
		out << theSong.m_artist;
		out << " | ";
		out.width(20);
		out << theSong.m_album;
		out << " | ";
		out.width(6);
		out.unsetf(std::ios::left);
		out.setf(std::ios::right);
		out << theSong.m_year << " | " << theSong.m_length << " | " << theSong.m_price << " |"; //no endline, handled outside
		return out;
	}

	int toSeconds(const std::string& length) {
		int min, sec; //parse the M:SS values from the string and assign them to these
		char col; //for the colon ":"
		std::istringstream ss(length); //change it to an input stream to assign the variables
		ss >> min >> col >> sec;
		return min * 60 + sec;
	}
}