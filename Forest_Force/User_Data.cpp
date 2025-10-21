/******************************************************************************/
/*!
\file		User_Data.cpp
\author 	Lim Zhen Eu Damon
\par    	Email: l.zheneudamon@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		This is the source file for functions that deal with user data /
			saved play data.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "pch.hpp"
#include "User_Data.hpp"

static const char* USER_DATA_FILE_PATH = "Data/User/user_data.txt";

bool tutorial_done = { false };

/**************************************************************************/
/*!
	This function checks if user data exists. If not, it creates it. Also,
	updates tutorial information.
*/
/**************************************************************************/
bool Check_User_Data() {
	// Check Data
	std::ifstream user_data{ USER_DATA_FILE_PATH };
	if (!user_data) {
		Create_User_Data();
	}
	else {
		bool tutorial{}; // 0 if tutorial is not done, 1 if tutorial is done
		user_data >> tutorial;
		user_data.close();
		tutorial_done = tutorial;
		return tutorial;
	}
	user_data.close();
	return false;
}

/**************************************************************************/
/*!
	This function creates the file for user data.
*/
/**************************************************************************/
bool Create_User_Data() {
	std::ofstream new_user_data{ USER_DATA_FILE_PATH };
	if (new_user_data) {
		new_user_data << "0" << "\n";									// Tutorial not done, by default
		for (u8 i{}; i <= LAST_LEVEL - GS_LEVEL1; ++i) {
			new_user_data << std::to_string(i + 1) << ": 0" << "\n";	// the file, by default will have 0 stars (not cleared)
		}
		new_user_data.close();
		return true;
	}
	else {
		new_user_data.close();
		return false;
	}
}

/**************************************************************************/
/*!
	This function resets user data. It deletes the file then recreates it.
*/
/**************************************************************************/
void Clear_User_Data() {
	std::remove(USER_DATA_FILE_PATH);
	Create_User_Data();
}

/**************************************************************************/
/*!
	This function updates the vector given with the amount of stars in
	each level.
*/
/**************************************************************************/
void Get_Level_Data(std::vector<std::string>& level_data) {
	// 0. Prepare the Vector of string to store
	std::vector<std::string> file_level_data{};

	// 1. Get the whole file data as a string
	std::ifstream user_data{ USER_DATA_FILE_PATH };
	if (user_data) {
		for (int i{}; !user_data.eof(); ++i) {
			std::string line;
			std::getline(user_data, line);
			// Avoids the tutorial line (which is i == 0)
			if (i) file_level_data.push_back(line);
		}
	}
	user_data.close();

	// 2. Update level_data
	level_data = file_level_data;
}

/**************************************************************************/
/*!
	This function updates the user data file to update that tutorial is
	completed.
*/
/**************************************************************************/
void Complete_Tutorial() {
	// We edit a file here by rewriting the whole file

	// 0. Prepare the Vector of string to store
	std::vector<std::string> the_whole_file{};

	// 1. Get the whole file data as a string
	std::ifstream user_data{ USER_DATA_FILE_PATH };
	if (user_data) {
		for (int i{}; !user_data.eof(); ++i) {
			std::string line;
			std::getline(user_data, line);
			the_whole_file.push_back(line);
		}
	}
	user_data.close();

	// 2. Update Tutorial Status
	if (the_whole_file[0].back() == '0') {
		the_whole_file[0].back() = '1';

		// 3. Write the whole file again
		std::ofstream new_user_data{ USER_DATA_FILE_PATH };
		for (int i{}; i < the_whole_file.size(); ++i) {
			new_user_data << the_whole_file[i] << "\n";
		}
		new_user_data.close();
	}
}

/**************************************************************************/
/*!
	This function updates the user data file with a new star count for the
	level cleared.
*/
/**************************************************************************/
void Complete_Level(int level, int stars) {
	// We edit a file here by rewriting the whole file

	// 0. Prepare the Vector of string to store
	std::vector<std::string> the_whole_file{};

	// 1. Get the whole file data as a string
	std::ifstream user_data{ USER_DATA_FILE_PATH };
	if (user_data) {
		for (int i{}; !user_data.eof(); ++i) {
			std::string line;
			std::getline(user_data, line);
			the_whole_file.push_back(line);
		}
	}
	user_data.close();

	// 2. Update Star Count
	if (static_cast<int>(the_whole_file[level].back()-'0') < stars) {
		the_whole_file[level].back() = static_cast<char>(stars + '0');

		// 3. Write the whole file again
		std::ofstream new_user_data{ USER_DATA_FILE_PATH };
		for (int i{}; i < the_whole_file.size(); ++i) {
			new_user_data << the_whole_file[i] << "\n";
		}
		new_user_data.close();
	}
}