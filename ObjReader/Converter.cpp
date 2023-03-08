#include "Converter.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

using std::filesystem::path;

void Converter::ConvertToBOBJ(const std::string& filepath)
{

	std::vector<float> vertices;
	std::vector<int> faces;





	if (std::filesystem::path objPath(filepath); std::filesystem::exists(filepath) && !std::filesystem::is_directory(filepath))
	{


		if (std::ifstream file(objPath.string()); file.is_open())
		{
			std::string line;
			while (std::getline(file, line)) {
				std::istringstream iss(line);

				std::string token;
				iss >> token;

				if (token == "v") {
					float x, y, z;
					iss >> x >> y >> z;
					vertices.push_back(x);
					vertices.push_back(y);
					vertices.push_back(z);
				}
				else if (token == "f") {
					int v1, v2, v3;
					iss >> v1 >> v2 >> v3;
					faces.push_back(v1);
					faces.push_back(v2);
					faces.push_back(v3);
				}
			}
		}
	}
	std::cout << "test";

}
