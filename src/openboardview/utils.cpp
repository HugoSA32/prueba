#include "utils.h"
#include <algorithm>
#include <assert.h>
#include <cctype>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdint.h>

#ifndef _WIN32
#include <dirent.h>
#endif
#include <sys/stat.h>
#include <sys/types.h>

// Loads an entire file in to memory
std::vector<char> file_as_buffer(const filesystem::path &filepath) {
	std::vector<char> data;

	if (!filesystem::is_regular_file(filepath)) {
		std::cerr << "Error opening " << filepath.string() << ": not a regular file " << std::endl;
		return data;
	}

	ifstream file;
	file.open(filepath, std::ios::in | std::ios::binary | std::ios::ate);

	if (!file.is_open()) {
		std::cerr << "Error opening " << filepath.string() << ": " << strerror(errno) << std::endl;
		return data;
	}

	file.seekg(0, std::ios_base::end);
	std::streampos sz = file.tellg();
	assert(sz >= 0);
	data.reserve(sz);
	file.seekg(0, std::ios_base::beg);
	data.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

	assert(data.size() == static_cast<unsigned int>(sz));
	file.close();

	return data;
}

// Extract extension from filename and check against given fileext
// fileext must be lowercase
bool check_fileext(const filesystem::path &filepath, const std::string fileext) {
	std::string ext{filepath.extension().string()}; // extract file ext
	std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);                 // make ext lowercase
	return ext == fileext;
}

// Returns true if the given str was found in buf
bool find_str_in_buf(const std::string str, const std::vector<char> &buf) {
	return std::search(buf.begin(), buf.end(), str.begin(), str.end()) != buf.end();
}

// Case insensitive comparison of std::string
bool compare_string_insensitive(const std::string &str1, const std::string &str2) {
	return str1.size() == str2.size() && std::equal(str2.begin(), str2.end(), str1.begin(), [](const char &a, const char &b) {
		       return std::tolower(a) == std::tolower(b);
		   });
}

// Case insensitive lookup of a filename at the given path
filesystem::path lookup_file_insensitive(const filesystem::path &path, const std::string &filename) {
	for(auto& p: filesystem::directory_iterator(path)) {
		if (compare_string_insensitive(p.path().filename().string(), filename)) {
			return p.path();
		}
	}
	return {};
}

// Split a string in a vector, delimiter is a space (stringstream iterator)
std::vector<std::string> split_string(const std::string str) {
	std::istringstream iss(str);
	return {std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};
}

// Split a string in a vector with given delimiter
std::vector<std::string> split_string(const std::string &str, char delimeter) {
	std::istringstream iss(str);
	std::string item;
	std::vector<std::string> strs;
	while (std::getline(iss, item, delimeter)) {
		strs.push_back(item);
	}
	return strs;
}
