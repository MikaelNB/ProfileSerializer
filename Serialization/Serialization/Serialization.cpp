// Serialization.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "Profile.h"

using namespace std;
using namespace rapidjson;


void save(string file, string data) {
	ofstream outFile;
	outFile.open(file+".json");
	cout << endl << data << endl;
	outFile << data;
	outFile.close();
}
const char *load(string file) {
	ifstream inFile;
	inFile.open(file + ".json");

	//Check For Error
	if (inFile.fail()) {
		cerr << "Error Opening " << file << endl;
		exit(1);
	}

	string item;
	int count = 0;
	//Read a file until you've reached the end
	string data = "";
	while (!inFile.eof()) {
		inFile >> item;
		count++;
		data = data + item;
	}
	cout << data << endl;
	inFile.close();

	int size = data.length() + 1;
	char* result = new char[size];
	strcpy_s(result, size,data.c_str());
	
	return result;
}
//Setup profile from referenced DOM "d"(previously loaded)
Profile SetupProfile(Document* d) {
	Profile profile(
		((Value&)(*d)["elapsedTime"]).GetDouble(),
		((Value&)(*d)["currentHp"]).GetInt(),
		((Value&)(*d)["totalHp"]).GetInt(),
		((Value&)(*d)["currentMp"]).GetInt(),
		((Value&)(*d)["totalMp"]).GetInt(),
		((Value&)(*d)["historyAdvance"]).GetInt(),
		((Value&)(*d)["mapIndex"]).GetInt(),
		((Value&)(*d)["location"]["i"]).GetInt(),
		((Value&)(*d)["location"]["j"]).GetInt(),
		((Value&)(*d)["combatStatus"]).GetBool()
	);
	return profile;
}

//Obtain data from profile and save to referenced DOM "d"
void SaveProfileToDOM(Profile profile, Document* d) {
	((Value&)(*d)["elapsedTime"]).SetDouble(profile.GetElapsedTime());
	((Value&)(*d)["currentHp"]).SetInt(profile.GetCurrenHp());
	((Value&)(*d)["totalHp"]).SetInt(profile.GetTotalHp());
	((Value&)(*d)["currentMp"]).SetInt(profile.GetCurrentMp());
	((Value&)(*d)["totalMp"]).SetInt(profile.GetTotalMp());
	((Value&)(*d)["historyAdvance"]).SetInt(profile.GetHistoryAdvance());
	((Value&)(*d)["mapIndex"]).SetInt(profile.GetMapIndex());
	((Value&)(*d)["location"]["i"]).SetInt(profile.GetLocationI());
	((Value&)(*d)["location"]["j"]).SetInt(profile.GetLocationJ());
	((Value&)(*d)["combatStatus"]).SetBool(profile.GetCombatStatus());
}

int main()
{
	//1. Parse json DOM "d" from loaded file named "profile".json
	Document d;
	const char *json { load("profile") };	
	d.Parse(json);

	//Printed examples.
	cout << "Current HP: " << to_string(((Value&)d["currentHp"]).GetInt()) << endl;
	cout << "Current i Location: " << to_string(((Value&)d["location"]["i"]).GetInt()) << endl;
		
	//2. Setup profile from (previously loaded) DOM.	
	Profile profile;
	//op 0, perfil nuevo
	//op 1, cargar perfil desde DOM 
	int op = 1;
	switch (op)
	{
	case 1:profile = SetupProfile(&d); break;
	default:profile = Profile();
		break;
	}

	//3. Update profile values at will
	profile.SetCurrenHp(profile.GetCurrenHp() + 1);

	//4. Update DOM from profile info.
	SaveProfileToDOM(profile, &d);

	//5. Stringify the DOM.
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	d.Accept(writer);
	
	//6. Save buffered DOM string to file "profile" 
	save("profile", buffer.GetString());
	system("pause");
	return 0;	
}