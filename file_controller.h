#ifndef FILE_CONTROLLER_H
#define FILE_CONTROLLER_H

#include <string>

using namespace std;

void write_to_file(string filename, string text, bool secure = true);
void write_to_file(string text);
bool file_exists(const std::string& filename);
string read_secure_filename();
void show_file(string filename);
string read_file(string filename);

#endif
