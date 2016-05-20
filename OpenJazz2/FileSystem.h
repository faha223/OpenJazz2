#ifndef _FILE_SYS_H_
#define _FILE_SYS_H_

#include <vector>
#include <string>

class Directory
{
public:
	static std::vector<std::string> GetFiles(std::string path, std::string SearchString = "*");
};

class Path
{
public:
	static std::string Combine(std::string path, std::string relativePath);
};

class File
{
public:
	static void WriteAllText(std::string path, std::string text);
	static std::string ReadAllText(std::string path);
};

#endif