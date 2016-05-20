#include "FileSystem.h"
#include "FileIO.h"
#include <stdint.h>
#include <stack>
using namespace std;

#ifdef _WIN32
#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#pragma comment(lib, "User32.lib")
const string PathDelimiter = "\\";
#else
#include <boost/filesystem.hpp>
namespace fs = ::boost::filesystem;
const string PathDelimiter = "/";
#endif


vector<string> Directory::GetFiles(string path, string extension)
{
	vector<string> files;
#ifdef _WIN32
	WIN32_FIND_DATA ffd;
	HANDLE firstFile = FindFirstFileA(Path::Combine(path, string("*") + extension).c_str(), &ffd);
	if (INVALID_HANDLE_VALUE != firstFile)
	{
		do
		{
			if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				files.push_back(Path::Combine(path, ffd.cFileName));
			}
		} while (FindNextFile(firstFile, &ffd) != 0);
	}
#else
	fs::path p(path);
	if(fs::exists(p) && fs::is_directory(p))
	{
		fs::recursive_directory_iterator it(p);
		fs::recursive_directory_iterator endit;
		while(it != endit)
		{
			if(fs::is_regular_file(*it) && it->path().extension().native() == extension)
				files.push_back(it->path().c_str());
			it++;
		}
	}
#endif
	return files;
}

string Path::Combine(string path, string relativePath)
{
	vector<string> pathTokens;
	vector<string> relativePathTokens;
	while (path.length() > 0)
	{
		int tokenLength = path.find_first_of('\\');
		if (tokenLength == -1)
			tokenLength = path.find_first_of('/');
		if (tokenLength == -1)
		{
			pathTokens.push_back(path);
			path = "";
		}
		else if (tokenLength >= 0)
		{
			string token = path.substr(0, tokenLength);
			path = path.substr(tokenLength + 1, path.length() - tokenLength - 1);
			pathTokens.push_back(token);
		}
	}

	while (relativePath.length() > 0)
	{
		int tokenLength = relativePath.find_first_of('\\');
		if (tokenLength == -1)
			tokenLength = relativePath.find_first_of('/');
		if (tokenLength == -1)
		{
			relativePathTokens.push_back(relativePath);
			relativePath = "";
		}
		else if (tokenLength > 0)
		{
			string token = relativePath.substr(0, tokenLength);
			relativePath = relativePath.substr(tokenLength + 1, relativePath.length() - tokenLength - 1);
			relativePathTokens.push_back(token);
		}
	}

	stack<string> combinedTokens;
	for (uint32_t i = 0; i < pathTokens.size(); i++)
	{
		if (pathTokens[i] == string("."))
			continue;
		if (pathTokens[i] == string(".."))
		{
			combinedTokens.pop();
			continue;
		}
		combinedTokens.push(pathTokens[i]);
	}
	for (uint32_t i = 0; i < relativePathTokens.size(); i++)
	{
		if (relativePathTokens[i] == string("."))
			continue;
		if (relativePathTokens[i] == string(".."))
		{
			combinedTokens.pop();
			continue;
		}
		combinedTokens.push(relativePathTokens[i]);
	}
	if (combinedTokens.size() == 0)
		return "";
	stack<string> reverse;
	do 
	{
		reverse.push(combinedTokens.top());
		combinedTokens.pop();
	} while (combinedTokens.size() > 0);

	path = reverse.top();
	reverse.pop();
	while (reverse.size() > 0)
	{
		path += PathDelimiter + reverse.top();
		reverse.pop();
	}

	return path;
}

void File::WriteAllText(string path, string text)
{
	FILE *fi = openFile(path.c_str(), "w");
	if (fi != nullptr)
	{
		fwrite(text.c_str(), 1, text.length(), fi);
		fclose(fi);
	}
}

string File::ReadAllText(string path)
{
	FILE *fi = openFile(path.c_str(), "r");
	if (fi == nullptr)
	{
		return string();
	}

	fseek(fi, 0, SEEK_END);
	uint32_t length = ftell(fi);
	fseek(fi, 0, SEEK_SET);

	char *out = new char[length + 1];
	memset(out, 0, length + 1);
	out[length] = '\0';
	fread(out, 1, length, fi);
	fclose(fi);
	string strout(out);
	delete[] out;

	return strout;
}