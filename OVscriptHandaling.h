#pragma once
bool is_sentence_in_file(const std::string& filename, const std::string& sentence_to_find) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cout << "Failed to open file: " << filename << std::endl;
		return false;
	}

	std::string line;
	while (std::getline(file, line)) {
		if (line.find(sentence_to_find) != std::string::npos) {
			return true;
		}
	}

	return false;
}
void removeH(const std::string& filename, const std::string& fileToRemove)
{
	std::ifstream infile(filename);
	std::vector<std::string> lines;

	std::string line;
	while (std::getline(infile, line))
	{
		if (line.find("<ClInclude Include=\"" + fileToRemove + ".h\" />") != std::string::npos)
		{
			continue;
		}
		else
		{
			lines.push_back(line);
		}
	}
	infile.close();

	std::ofstream outfile(filename);
	for (const auto& line : lines)
	{
		outfile << line << std::endl;
	}
	outfile.close();

	std::remove((fileToRemove + ".h").c_str());
}
/*
void removeCpp(const std::string& filename, const std::string& fileToRemove)
{
	std::ifstream infile(filename);
	std::vector<std::string> lines;

	std::string line;
	while (std::getline(infile, line))
	{
		if (line.find("<ClCompile Include=\"" + fileToRemove + ".cpp\" />") != std::string::npos)
		{
			continue;
		}
		else
		{
			lines.push_back(line);
		}
	}
	infile.close();

	std::ofstream outfile(filename);
	for (const auto& line : lines)
	{
		outfile << line << std::endl;
	}
	outfile.close();

	std::remove((fileToRemove + ".cpp").c_str());
}

*/

void addMainCpp(const std::string& filename, const std::string& file)
{
	if (!is_sentence_in_file(filename, "#include\"" + file + ".h\"")
		&& !is_sentence_in_file(filename, file + " " + file + "scr" + ";")
		&& !is_sentence_in_file(filename, "    " + file + "scr" + ".Start();")) {
		std::ifstream infile(filename);

		std::string line;
		std::vector<std::string> lines;
		bool found_line = false;
		while (std::getline(infile, line)) {
			if (line == "#include\"Script.h\"") {
				found_line = true;
				lines.push_back(line);
				lines.push_back("#include\"" + file + ".h\"");
			}
			else if (line == "Script script;") {
				found_line = true;

				lines.push_back(line);
				lines.push_back(file + " " + file + "scr" + ";");
			}
			else if (line == "    script.Start();") {
				found_line = true;

				lines.push_back(line);
				lines.push_back("    " + file + "scr" + ".Start();");
			}
			else if (line == "    script.Update();") {
				found_line = true;

				lines.push_back(line);
				lines.push_back("    " + file + "scr" + ".Update();");
			}
			else if (line == "    script.Exit();") {
				found_line = true;

				lines.push_back(line);
				lines.push_back("    " + file + "scr" + ".Exit();");
			}
			else {
				lines.push_back(line);
			}
		}
		infile.close();
		std::ofstream outfile(filename);
		for (const auto& line : lines) {
			outfile << line << std::endl;
		}
		outfile.close();
	}
}

void removeMainCpp(const std::string& filename, const std::string& file)
{
	if (is_sentence_in_file(filename, "#include\"" + file + ".h\"")
		|| is_sentence_in_file(filename, file + " " + file + "scr" + ";")
		|| is_sentence_in_file(filename, "    " + file + "scr" + ".Start();")) {
		std::ifstream infile(filename);

		std::string line;
		std::vector<std::string> lines;
		bool found_line = false;
		while (std::getline(infile, line)) {
			if (line == "#include\"" + file + ".h\"") {
				found_line = true;
			}
			else if (line == file + " " + file + "scr" + ";") {
				found_line = true;
			}
			else if (line == "    " + file + "scr" + ".Start();") {
				found_line = true;
			}
			else if (line == "    " + file + "scr" + ".Update();") {
				found_line = true;
			}
			else if (line == "    " + file + "scr" + ".Exit();") {
				found_line = true;
			}
			else {
				lines.push_back(line);
			}
		}
		infile.close();
		std::ofstream outfile(filename);
		for (const auto& line : lines) {
			outfile << line << std::endl;
		}
		outfile.close();
	}
}


void addH(const std::string& filename, const std::string& file, const std::string& lastFilename)
{
	if (!is_sentence_in_file(filename, "	<ClInclude Include=\"" + file + ".h\" />")) {
		std::ifstream infile(filename);

		ofstream createfile("DynaLL/" + file + ".h");

		std::string script =
			R"(
#pragma once
#include<iostream>
#include"OVLIB.h"
#include"pch.h"

class )" + file + R"( {
public:
	void Start() {

	}
	void Update() {
	
	}
	void Exit() {

	}
};
    )";
		createfile << script;


		std::string line;
		std::vector<std::string> lines;
		bool found_line = false;
		while (std::getline(infile, line)) {
			if (line.find("<ClInclude Include=\"" + lastFilename + ".h\" />") != std::string::npos) {
				found_line = true;
				lines.push_back(line);
				lines.push_back("	<ClInclude Include=\"" + file + ".h\" />");
			}
			else {
				lines.push_back(line);
			}
		}
		infile.close();
		std::ofstream outfile(filename);
		for (const auto& line : lines) {
			outfile << line << std::endl;
		}
		outfile.close();



	}
}
/*
void addCpp(const std::string& filename, const std::string& file, const std::string& lastFilename)
{
	if (!is_sentence_in_file(filename, "	<ClCompile Include=\"" + file + ".cpp\" />")) {
		std::ifstream infile(filename);

		ofstream createfile("DynaLL/"+file + ".cpp");

		std::string script =
			R"(#include")" + file + R"(.h"
void )" + file + R"(::Start()
{											

}
void )" + file + R"(::Update()
{

}
void )" + file + R"(::Exit()
{

}
    )";
		createfile << script;


		std::string line;
		std::vector<std::string> lines;
		bool found_line = false;
		while (std::getline(infile, line)) {

			if (line.find("<ClCompile Include=\"" + lastFilename + ".cpp\" />") != std::string::npos) {
				found_line = true;
				lines.push_back(line);
				lines.push_back("	<ClCompile Include=\"" + file + ".cpp\" />");
			}
			else {
				lines.push_back(line);
			}
		}
		infile.close();
		std::ofstream outfile(filename);
		for (const auto& line : lines) {
			outfile << line << std::endl;
		}
		outfile.close();
	}
}
*/
void addOVscript(const std::string& file)
{
	//addCpp("DynaLL/Dll1.vcxproj", file, "Script");
	addH("DynaLL/Dll1.vcxproj", file, "Script");
	addMainCpp("DynaLL/OVLIB.cpp", file);
}

void removeOVscript(const std::string& file)
{
	//removeCpp("DynaLL/Dll1.vcxproj", file);
	removeH("DynaLL/Dll1.vcxproj", file);
	removeMainCpp("DynaLL/OVLIB.cpp", file);
}