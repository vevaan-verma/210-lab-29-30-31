// COMSC-210 | Lab 29, 30, 31 | Vevaan Verma
/*
Project Overview:
	This project simulates the atmosphere of different stages at a very large 25 hour music festival. It aims to reveal which features of concerts are appealing to audiences, which can be used by other artists when planning their own concerts.

Implementation:
	An std::map will contain the data of each stage at the festival. The map's key will be the name of each stage, while the value will be an�std::array that contains three�std::lists. The lists will contain the genres of music being played (eg. pop, rock, EDM, hip-hop), the artists performing on that stage (eg. The Weeknd, Dua Lipa, Travis Scott), and the weather (eg. sunny, rainy, cloudy), respectively. One map entry would be for the main stage (named "Main Stage"), with the lists containing the genre(s) of music being performed on that stage, the artists performing, and the weather. Another entry would be the "DJ Set", which would contain a different set of genres and artists, and possibly different weather as well.

Simulated Events:
	The simulation will focus on three events: artist changes, song genre changes, and weather effects that push crowds to different stages. During a specific hour, the crowd may be completely pushed to a different stage due to bad weather conditions, an artist change, or a genre change.
*/
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <list>
#include <array>
#include <vector>
#include <chrono>
#include <thread>
using namespace std;
using namespace std::this_thread; // for sleep_for
using namespace std::chrono_literals; // for the 1s literal

// function prototype for the atmosphere simulation function
void simulateFestivalAtmosphere(map<string, array<list<string>, 3>>& festivalData, string stageName, int hour);
void outputFestivalInfo(map<string, array<list<string>, 3>>& festivalData);

/* CONSTANTS */
const string DATA_FILE_NAME = "festival_data.txt"; // the name of the file that contains the data of the festival (this file contains the initial data of the festival)
const vector<string> STAGE_NAMES = { "Main Stage", "DJ Set", "Acoustic Stage", "Rock Stage", "EDM Stage", "Hip-Hop Stage" }; // the names of the stages at the festival
const vector<string> GENRES = { "Pop", "Rock", "EDM", "Hip-Hop", "R&B", "Country" }; // the genres of music that can be played at the festival
const vector<string> ARTISTS = { "The Weeknd", "Dua Lipa", "Travis Scott", "Ariana Grande", "Drake", "Taylor Swift" }; // the artists that can perform at the festival
const vector<string> WEATHER = { "Sunny", "Rainy", "Cloudy", "Windy", "Snowy", "Foggy" }; // the weather conditions that can occur at the festival

int main() {

	// create a map that will hold the data of each stage at the festival; each value will be an array that contains three lists
	// the lists will contain the genres of music being played (string), the artists performing on that stage (string), and the weather (string)
	map<string, array<list<string>, 3>> festivalData;

	// open the file that contains the data of the festival
	ifstream fin(DATA_FILE_NAME);

	// make sure the file is open and read the data from the file, otherwise print an error message and exit the program
	if (!fin) {

		cout << "Error: File " << DATA_FILE_NAME << " not found." << endl; // output error message
		return 1; // return error code

	}

	// read the data from the file and store it in the map (for now, actual file reading is not implemented because sample data is used)
		// each line in the file will contain the name of the stage and the atmosphere data of the stage
		// insert each part of the atmosphere data into its respective list in the map
		// each stage starts with one artist, one genre, and one weather condition, but this changes as the simulation progresses
	while (!fin.eof()) {

		string stageName;
		string genre;
		string artist;
		string weather;

		getline(fin, stageName, '\n');
		getline(fin, genre, '\n');
		getline(fin, artist, '\n');
		getline(fin, weather, '\n');

		array<list<string>, 3> stageData;
		stageData[0].push_back(genre);
		stageData[1].push_back(artist);
		stageData[2].push_back(weather);

		festivalData[stageName] = stageData;

	}

	// close the file to prevent memory leaks
	fin.close();

	// output the initial atmosphere of the festival
	cout << "Initial Festival Atmosphere:" << endl;
	outputFestivalInfo(festivalData);

	// start simulating the atmosphere of the festival (25 hours, 1 hour intervals)
	for (int hour = 1; hour <= 25; hour++) {

		// for each stage in the map, call the function that simulates the atmosphere of the festival
		// pass the map and the current hour of the festival to the function
		for (auto stage : festivalData)
			simulateFestivalAtmosphere(festivalData, stage.first, hour);

		cout << "-----------------------------------------------------------------------------------" << endl; // output a separator between each hour
		sleep_for(1s); // sleep for 1 seconds to simulate the passage of time

	}

	cout << endl << "Post Festival Atmosphere:" << endl;
	outputFestivalInfo(festivalData); // output the final atmosphere of the festival
	return 0;

}

// simulateFestivalAtmosphere() will take in the map and the current hour of the festival and randomly choose between the three events: artist changes, song genre changes, and weather effects
// arguments: map<string, array<list<string>, 3>>& festivalData - the map that contains the data of each stage at the festival, int hour - the current hour of the festival
// returns: none
void simulateFestivalAtmosphere(map<string, array<list<string>, 3>>& festivalData, string stageName, int hour) {

	// bug found through unit testing: if the stage name is not found in the map, the program will crash

	// the function will take in the map and the current hour of the festival
	// randomly choose between the three events: artist changes, song genre changes, and weather effects
	int event = rand() % 3;

	if (event == 0) { // artist changes

		festivalData[stageName][0].clear(); // clear the list of genres

		// stage can have up to 3 genres of music being played at the same time
		int genreCount = rand() % 3 + 1; // randomly select the number of genres that will be played on that stage

		for (int i = 0; i < genreCount; i++) { // for each genre that will be played on that stage

			string newGenre = GENRES[rand() % GENRES.size()]; // randomly select a new genre to replace the current genre(s) on that stage
			festivalData[stageName][0].push_back(newGenre); // add the new genre to the list

		}

		cout << "Hour " << hour << ": Song genre changes at " << stageName << endl; // output the event change

	} else if (event == 1) { // song genre changes

		// stage can have up to 3 artists performing at the same time
		int artistCount = rand() % 3 + 1; // randomly select the number of artists that will perform on that stage
		festivalData[stageName][1].clear(); // clear the list of artists

		for (int i = 0; i < artistCount; i++) { // for each artist that will perform on that stage

			string newArtist = ARTISTS[rand() % ARTISTS.size()]; // randomly select an artist to replace the current artist(s) on that stage
			festivalData[stageName][1].push_back(newArtist); // add the new artist to the list

		}

		cout << "Hour " << hour << ": Artists on stage change at " << stageName << endl; // output the event change

	} else if (event == 2) { // weather effect changes

		festivalData[stageName][2].clear(); // clear the list of weather

		// stage can have up to 3 weather conditions at the same time (due to the existence of partly cloudy, etc.)
		int weatherCount = rand() % 3 + 1; // randomly select the number of weather conditions that will affect that stage

		for (int i = 0; i < weatherCount; i++) { // for each weather condition that will affect that stage

			string newWeather = WEATHER[rand() % WEATHER.size()]; // randomly select a new weather condition to replace the current weather on that stage
			festivalData[stageName][2].push_back(newWeather); // add the new weather to the list

		}

		cout << "Hour " << hour << ": Weather effect changes at " << stageName << endl; // output the event change

	}
}

// outputFestivalInfo() will take in the map and output the current atmosphere of the festival
// arguments: map<string, array<list<string>, 3>>& festivalData - the map that contains the data of each stage at the festival
// returns: none
void outputFestivalInfo(map<string, array<list<string>, 3>>& festivalData) {

	// output the current atmosphere of the festival using the following format:
	/*
	Stage: <stage name>
		Artist(s): <artist name>
		Genre(s): <genre name>
		Weather: <weather condition>
	*/

	// this whole section below outputs the current atmosphere of the festival using the format above
	for (auto stage : festivalData) {

		cout << "Stage: " << stage.first << endl;
		cout << "\tGenre(s): ";
		string genres = "";

		for (auto genre : stage.second[0])
			genres += genre + ", ";

		cout << genres.substr(0, genres.size() - 2) << endl; // remove the last comma and space from the list of genres

		cout << "\tArtist(s): ";
		string artists = "";

		for (auto artist : stage.second[1])
			artists += artist + ", ";

		cout << artists.substr(0, artists.size() - 2) << endl; // remove the last comma and space from the list of artists

		cout << "\tWeather: ";
		string weatherConditions = "";

		for (auto weather : stage.second[2])
			weatherConditions += weather + ", ";

		cout << weatherConditions.substr(0, weatherConditions.size() - 2) << endl; // remove the last comma and space from the list of weather

	}

	cout << endl; // output a blank line for formatting purposes

}