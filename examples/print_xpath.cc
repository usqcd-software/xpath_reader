#include <basic_xpath_reader.h>
#include <xpath_reader.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <array.h>

using namespace std;
using namespace XMLXPathReader;

int main(int argc, char *argv[])
{
  // Instantiate a reader
  XPathReader reader;
 
  if (argc != 3)
  {
    cerr << "Usage: print_xpath  <xml file>  <xpath query>" << endl;
    exit(1);
  }

  // Try and open the reader
  try { 
    reader.open(argv[1]);
  } 
  catch (string &error_mesg) {
    cerr << error_mesg << endl;
    throw;
  }

  // Try and get a string 
  string sresult ="";  
  try {
    reader.getXPath<string>(argv[2], sresult);
    cout << sresult << endl;
  }
  catch (const string& e) {
    cout << e << endl;
  }

  /* This bit kills the last refcount */
  reader.close();

  return EXIT_SUCCESS;
}

