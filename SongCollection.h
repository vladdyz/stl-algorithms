//Vladislav Zolotukhin
//Date: 2024-03-16
#ifndef VZ_SONGCOLLECTION_H
#define VZ_SONGCOLLECTION_H
#include <iostream>
#include <vector>
#include <list>
#include <string>

namespace vz {

	struct Song {
		std::string m_artist{ "" };
		std::string m_title{ "" };
		std::string m_album{ "" };
		double m_price{ 0.0 };
		std::string m_year{ ""};
		std::string m_length{ ""};
	};


	class SongCollection {
		std::vector<Song> songs;
	public:
		SongCollection() {};
		SongCollection(const char* filename);
		void display(std::ostream& out) const;
		void sort(const char*);
		void cleanAlbum();
		bool inCollection(const char* artist) const;
		std::list<Song> getSongsForArtist(const char* artist) const;

	};
	std::string trimWS(std::string& tokenstr); //my custom trim whitespace function
	std::ostream& operator<<(std::ostream& out, const Song& theSong);
	int toSeconds(const std::string&); //helper function for the numeric library template

}

#endif

