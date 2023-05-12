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



void addMainCpp(const std::string& filename, const std::string& file)
{
	if (!is_sentence_in_file(filename, "#include\"" + file + ".h\"")
		&& !is_sentence_in_file(filename, file + " " + file + "scr" + ";")
		&& !is_sentence_in_file(filename, "				" + file + "scr" + ".Start(con, sceneObjects);")) {
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
			else if (line == "				script.Start(con, InputHandler, world, sceneObjects);") {
				found_line = true;

				lines.push_back(line);
				lines.push_back("				" + file + "scr" + ".Start(con, InputHandler, world, sceneObjects);");
			}
			else if (line == "				script.Update(con, InputHandler, world, sceneObjects);") {
				found_line = true;

				lines.push_back(line);
				lines.push_back("				" + file + "scr" + ".Update(con, InputHandler, world, sceneObjects);");
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
		|| is_sentence_in_file(filename, "				" + file + "scr" + ".Start(con, InputHandler, world, sceneObjects);")) {
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
			else if (line == "				" + file + "scr" + ".Start(con, InputHandler, world, sceneObjects);") {
				found_line = true;
			}
			else if (line == "				" + file + "scr" + ".Update(con, InputHandler, world, sceneObjects);") {
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

		ofstream createfile(file + ".h");

		std::string script =
			R"(
#pragma once
#include<iostream>
#include"Console.h"
#include"Object.h"
#include"InputSystem.h"
#include"OV.h"
class )" + file + R"( {
public:
    void Start(Console& ovcon, InputSystem Input, PhysicsWorld& world, std::vector<Object>& sceneObjects);
    void Update(Console& ovcon, InputSystem Input, PhysicsWorld& world, std::vector<Object>& sceneObjects);
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
void addCpp(const std::string& filename, const std::string& file, const std::string& lastFilename)
{
	if (!is_sentence_in_file(filename, "	<ClCompile Include=\"" + file + ".cpp\" />")) {
		std::ifstream infile(filename);

		ofstream createfile(file + ".cpp");

		std::string script =
			R"(#include")" + file + R"(.h"
void )" + file + R"(::Start(Console& ovcon, InputSystem Input, PhysicsWorld& world, std::vector<Object>& sceneObjects)
{											

}
void )" + file + R"(::Update(Console& ovcon, InputSystem Input, PhysicsWorld& world, std::vector<Object>& sceneObjects)
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
void addOVscript(const std::string& file)
{
	addCpp("Vision_engine.vcxproj", file, "Script");
	addH("Vision_engine.vcxproj", file, "Script");
	addMainCpp("main.cpp", file);
}

void removeOVscript(const std::string& file)
{
	removeCpp("Vision_engine.vcxproj", file);
	removeH("Vision_engine.vcxproj", file);
	removeMainCpp("main.cpp", file);
}