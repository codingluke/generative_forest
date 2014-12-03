#include <iostream>
#include <string>
#include <fstream>
#include "file_controller.h"

using namespace std;


/**
 * Reads a file according a given filename and gives its content out
 * as a string.
 *
 * @param filename  Name of the file to read.
 *
 * @return content of the file as a string.
 */
string read_file(string filename)
{
  string line = "";
  string file = "";
  ifstream infile(filename.c_str());
  if (!infile)
    cout << "File '" << filename << "' existiert nicht!" << endl;
  while (getline(infile, line))
    file += line + "\n";
  return file;
}
