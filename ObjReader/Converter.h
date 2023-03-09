#pragma once
#include <string>
#include <filesystem>
#include <vector>

struct Float3
{
	float x, y, z;
};

struct Int3
{
	int v1, v2, v3;
};

class Converter
{
public:
	static void ConvertToBOBJ(const std::string& objPath);

	static void ConvertToOBJ();

private:
	static void ReadOBJ(const std::filesystem::path& objPath, std::vector<Float3>& vertices, std::vector<Int3>& faces, std::vector<std::pair<unsigned int, std::string>>& comments);
	static void ReadBOBJ(std::vector<Float3>& vertices, std::vector<Int3>& faces, std::vector<std::pair<unsigned int, std::string>>& comments);
	static void WriteBOBJ(std::vector<Float3>& vertices, std::vector<Int3>& faces, std::vector<std::pair<unsigned int, std::string>>& comments);
	static void WriteOBJ(std::vector<Float3>& vertices, std::vector<Int3>& faces, std::vector<std::pair<unsigned int, std::string>>& comments);
};

