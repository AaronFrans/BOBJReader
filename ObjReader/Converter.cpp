#include "Converter.h"
#include <iostream>
#include <fstream>


void Converter::ConvertToBOBJ(const std::string& filepath)
{
	try
	{
		if (std::filesystem::path objPath(filepath);
			std::filesystem::exists(filepath) && !std::filesystem::is_directory(filepath))
		{
			std::vector<Float3> vertices{};
			std::vector<Int3> faces{};
			std::vector<std::pair<unsigned int, std::string>> comments{};
			ReadOBJ(objPath, vertices, faces, comments);
			WriteBOBJ(vertices, faces, comments);
			std::cout << "Converting to BOBJ done\n";
		}
		else
		{
			std::string message{};
			message += "Path: " + filepath + " does not exist\n";
			throw std::exception(message.c_str());
		}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
		throw std::exception("Failed to convert to BOBJ\n");
	}



}

void Converter::ConvertToOBJ()
{

	try
	{
		std::vector<Float3> vertices{};
		std::vector<Int3> faces{};
		std::vector<std::pair<unsigned int, std::string>> comments{};
		ReadBOBJ(vertices, faces, comments);
		WriteOBJ(vertices, faces, comments);
		std::cout << "Converting to OBJ done\n";
	}
	catch (const std::exception& e)
	{

		std::cout << e.what();
		throw std::exception("Failed to convert to OBJ\n");
	}



}

void Converter::ReadOBJ(const  std::filesystem::path& objPath, std::vector<Float3>& vertices, std::vector<Int3>& faces, std::vector<std::pair<unsigned int, std::string>>& comments)
{

	if (std::ifstream file(objPath.string());
		file.is_open())
	{
		int lineNum{ 0 };
		std::string line;
		while (std::getline(file, line))
		{
			std::istringstream iss(line);

			std::string token;
			iss >> token;
			if (token == "#") {
				std::string comment = iss.str();

				comments.push_back(std::pair(lineNum, comment));
			}
			else if (token == "v") {
				float x, y, z;
				iss >> x >> y >> z;

				vertices.push_back({ x, y, z });
			}
			else if (token == "f") {
				int v1, v2, v3;
				iss >> v1 >> v2 >> v3;

				faces.push_back({ v1, v2, v3 });
			}

			++lineNum;
		}
	}
	else
	{
		throw std::exception("Failed to open obj file\n");
	}
}

void Converter::ReadBOBJ(std::vector<Float3>& vertices, std::vector<Int3>& faces, std::vector<std::pair<unsigned int, std::string>>& comments)
{
	if (std::ifstream readFrom("Resources/bunny.BOBJ", std::ios::binary);
		readFrom.is_open())
	{
		unsigned int nrComments{};
		readFrom.read(reinterpret_cast<char*>(&nrComments), sizeof(nrComments));

		for (unsigned int i = 0; i < nrComments; i++)
		{
			int lineNumber{ 0 };
			unsigned int lineLength{ 0 };

			// Read the line number
			readFrom.read(reinterpret_cast<char*>(&lineNumber), sizeof(lineNumber));

			// Read the length of the comment
			readFrom.read(reinterpret_cast<char*>(&lineLength), sizeof(lineLength));

			// Read the comment
			std::string comment(lineLength, '\0');
			readFrom.read(reinterpret_cast<char*>(comment.data()), lineLength);

			comments.emplace_back(std::pair(lineNumber, comment));
		}

		unsigned int nrVertices{};
		readFrom.read(reinterpret_cast<char*>(&nrVertices), sizeof(nrVertices));
		vertices.resize(nrVertices);
		readFrom.read(reinterpret_cast<char*>(vertices.data()), sizeof(Float3) * nrVertices);
		unsigned int nrFaces{};
		readFrom.read(reinterpret_cast<char*>(&nrFaces), sizeof(nrFaces));
		faces.resize(nrFaces);
		readFrom.read(reinterpret_cast<char*>(faces.data()), sizeof(Int3) * nrFaces);

		readFrom.close();
	}
	else
	{
		throw std::exception("Failed to open BOBJ file\n");
	}
}

void Converter::WriteBOBJ(std::vector<Float3>& vertices, std::vector<Int3>& faces, std::vector<std::pair<unsigned int, std::string>>& comments)
{
	if (std::ofstream writeToFile{ "Resources/bunny.BOBJ", std::ios::binary };
		writeToFile.is_open())
	{


		//get sizes of vectors
		unsigned int nrVertices = static_cast<unsigned int>(vertices.size());
		unsigned int nrFaces = static_cast<unsigned int>(faces.size());
		unsigned int nrComments = static_cast<unsigned int>(comments.size());


		//write comments
		writeToFile.write(reinterpret_cast<const char*>(&nrComments), sizeof(nrComments));

		for (const auto& comment : comments)
		{
			//line number
			writeToFile.write(reinterpret_cast<const char*>(&comment.first), sizeof(comment.first));
			unsigned int lineLength = static_cast<unsigned int>(comment.second.size());

			writeToFile.write(reinterpret_cast<const char*>(&lineLength), sizeof(lineLength));
			writeToFile.write(reinterpret_cast<const char*>(comment.second.c_str()), comment.second.size());
		}


		//write vertices
		writeToFile.write(reinterpret_cast<const char*>(&nrVertices), sizeof(nrVertices));

		for (const auto& vertice : vertices)
		{
			writeToFile.write(reinterpret_cast<const char*>(&vertice), sizeof(vertice));
		}


		//write faces
		writeToFile.write(reinterpret_cast<const char*>(&nrFaces), sizeof(nrFaces));

		for (const auto& face : faces)
		{
			writeToFile.write(reinterpret_cast<const char*>(&face), sizeof(face));
		}


		writeToFile.close();
	}
	else
	{
		throw std::exception("failed to open BOBJ file to write to");
	}

}

void Converter::WriteOBJ(std::vector<Float3>& vertices, std::vector<Int3>& faces, std::vector<std::pair<unsigned int, std::string>>& comments)
{

	if (std::ofstream writeToFile{ "Resources/new_bunny.OBJ" };
		writeToFile.is_open())
	{
		unsigned int lineNum{ 0 };


		for (const auto& vertice : vertices)
		{
			while (!comments.empty() && lineNum == comments.front().first)
			{
				writeToFile << comments.front().second << "\n";
				comments.erase(comments.begin());
				++lineNum;
			}

			writeToFile << "v " << vertice.x << " " << vertice.y << " " << vertice.z << "\n";
			++lineNum;
		}

		for (const auto& face : faces)
		{
			while (!comments.empty() && lineNum == comments.front().first)
			{
				writeToFile << comments.front().second << "\n";
				comments.erase(comments.begin());
				++lineNum;
			}

			writeToFile << "f " << face.v1 << " " << face.v2 << " " << face.v3 << "\n";
			++lineNum;
		}
		writeToFile.close();
	}
	else
	{
		throw std::exception("failed to open OBJ file to write to");
	}
}

