/* ID: $Id: basic_xpath_reader_test2.cc,v 1.3 2003-09-05 15:43:52 bjoo Exp $
 *
 * file: basic_xpath_reader2.cc
 *
 * a second example of using BasicXPathReader -- now to bind 
 * primitive types 
 */

#include <basic_xpath_reader.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>


using namespace std;
using namespace XMLXPathReader;

int main(int argc, char *argv[])
{

  // Get the reader
  XMLDocument the_doc;

  // Open the file
  try { 
    the_doc.open("foo.xml");
  } 
  catch (string &error_mesg) {
    cerr << error_mesg << endl;
    throw;
  }
  cout << "Document Open Complete" << endl;

  // Attach a reader
  BasicXPathReader reader(the_doc);

  // try to read a string 
  string sresult ="";
  try {
    reader.get("/foo/funky", sresult);
    cout << "Query: /foo/funky returned string: " << sresult << endl;
  }
  catch (const string& e) {
    cout << e << endl;
  }

  // try to read an int 
  int iresult;
  try {
    reader.get("//bj:armadillo", iresult);
    cout << "Query: //bj:armadillo returned int: " << iresult << endl;
  }
  catch (const string& e) {
    cout << e << endl;
  }

  // fail to read an int (armadillo2 contains a non-integer string)
  try {
    reader.get("//armadillo2", iresult);
    cout << "Query: //armadillo2 returned int: " << iresult << endl;
  }
  catch (const string& e) {
    cout << e << endl;
  }

  // fail to read an integer (funky contains a sequence of 2 integers)
  try {
    reader.get("//funky", iresult);
    cout << "Query: //funky  returned int: " << iresult << endl;
  }
  catch (const string& e) {
    cout << e << endl;
  } 

  // read a float (and succeed)
  float fresult;
  try { 
    reader.get("/foo/bar[3]", fresult);
    cout << "Query: /foo/bar[3] returned float: " << fresult <<endl;
  }
  catch( const string& e) { 
    cout << e << endl;
  }

  // read another float and succeed -- use namespace in the xpath
  try { 
    reader.get("/foo/bar/fred:kappa", fresult);
    cout << "Query: /foo/bar/fred:kappa returned float: " << fresult <<endl;
  }
  catch( const string& e) { 
    cout << e << endl;
  }

  // try and read a string into a float and fail
  try { 
    reader.get("//armadillo2", fresult);
    cout << "Query: //armadillo2 returned float: " << fresult <<endl;
  }
  catch( const string& e) { 
    cout << e << endl;
  }
  
  // try and read a float from a list of 2 and fail
  try { 
    reader.get("//funky", fresult);
    cout << "Query: //funky returned float: " << fresult <<endl;
  }
  catch( const string& e) { 
    cout << e << endl;
  }

  // try and read a double and succeed
  double dresult;
  try { 
    reader.get("/foo/bar[3]", dresult);
    cout << "Query: /foo/bar[3] returned double: " << dresult <<endl;
  }
  catch( const string& e) { 
    cout << e << endl;
  }

  // try and read a double with a namespace and succeed
  try { 
    reader.get("/foo/bar/fred:kappa", dresult);
    cout << "Query: /foo/bar/fred:kappa returned double: " << dresult <<endl;
  }
  catch( const string& e) { 
    cout << e << endl;
  }

  // try and read a double and fail -- armadillo2 contains unsuitable string
  try { 
    reader.get("//armadillo2", dresult);
    cout << "Query: //armadillo2 returned double: " << dresult <<endl;
  }
  catch( const string& e) { 
    cout << e << endl;
  }
  
  // try and read a double and fial -- //funky returns a list 
  try { 
    reader.get("//funky", dresult);
    cout << "Query: //funky returned double: " << dresult <<endl;
  }
  catch( const string& e) { 
    cout << e << endl;
  }

  // try and read a boolean and succeed
  bool bresult;
  try { 
    reader.get("//multi_quarkP", bresult);
    cout << "Query: //multi_quarkP returned boolean: " << boolalpha << bresult <<endl;
  }
  catch( const string& e) { 
    cout << e << endl;
  }
  
  // try and read a boolean with a namespace
  try { 
    reader.get("//bj:mybool", bresult);
    cout << "Query: //bj:mybool returned boolean: " << boolalpha << bresult <<endl;
  }
  catch( const string& e) { 
    cout << e << endl;
  }
  
  // try and read a boolean from a string and fail as the string is unsuitable
  try { 
    reader.get("//armadillo2", bresult);
    cout << "Query: //armadillo2 returned boolean: " << boolalpha << bresult <<endl;
  }
  catch( const string& e) { 
    cout << e << endl;
  }

  // try and read a boolean from a list thereof and fail
  try { 
    reader.get("//funkybool", bresult);
    cout << "Query: //funkybool returned boolean: " << boolalpha << bresult <<endl;
  }
  catch( const string& e) { 
    cout << e << endl;
  }
  
  // Try out some attributes
  string sattrib;
  int iattrib;
  
  // try and read the index attribute of /foo/bar[1] into a string and succeed
  try {
    reader.getAttribute("/foo/bar[1]", "index", sattrib);
  }
  catch( const string& e) { 
    cout << e << endl;
  }
  cout << "String attribute index on node identified by" 
       << " /foo/bar[1]" << " has value " << sattrib << endl;

  // try and read the index attribute of /foo/bar[2] into an int and succeed
  try {
    reader.getAttribute("/foo/bar[2]", "index", iattrib);
  }
  catch( const string& e) { 
    cout << e << endl;
  }
  cout << "int attribute \"index\" on node identified by" 
       << " /foo/bar[2]" << " has value " << iattrib << endl;

  // try and get the attribute from a query that doesn't return a unique 
  // node -- and fail
  try {
    reader.getAttribute("/foo/bar", "index", iattrib);
  }
  catch( const string& e) { 
    cout << e << endl;
  } 
  
  // try the count function
  int occursFooBar;
  try { 
    occursFooBar = reader.count("/foo/bar");
  }
  catch( const string& e) {
    cout << e << endl;
  }

  cout << "Query /foo/bar occurs " << occursFooBar << " times" << endl;

  cout << "Testing dump functions:" <<endl;
 
  ostringstream document;
  ostringstream root;
  ostringstream node;

  try {
    reader.print(document);
  }
  catch(const string& e) {
    cout << e << endl;
  }
  cout << "Document: " << endl << document.str() <<endl;

  try {
    reader.printRoot(root);
  }
  catch( const string& e) { 
    cout << e << endl;
  }
  cout << "Root Node: " << endl << root.str() << endl;
  
  try {
    reader.printXPathNode(node, "/foo/bar[1]");
  }
  catch ( const string& e) { 
    cout << e << endl;
  }
  cout << "/foo/bar[1]: " << endl << node.str() << endl;

  try { 
    reader.printXPathNode(node, "/foo/bar");
  }
  catch( const string& e) { 
    cout << e << endl;
  }

  /* cout << "Closing reader." << endl;
  reader.close();
  */

  cout << "Trying to reopen reader on document" << endl;
  istringstream foo(document.str());
  XMLDocument doc2;
  doc2.open(foo);

  BasicXPathReader reader2(doc2);

  reader2.print(cout);
  
  cout << endl;
  cout << "Trying to reopen reader on root node" << endl;
  
  istringstream foo2(root.str());
  

  doc2.close();
  doc2.open(foo2);
  BasicXPathReader reader3(doc2);
  reader3.printRoot(cout);
  
  
  cout << endl;
  cout << "Trying to reopenm reader on node selection" << endl;
  
  istringstream foo3(node.str());
  doc2.close();

  doc2.open(foo3);
  BasicXPathReader reader4(doc2);
  reader4.printRoot(cout);
 
  cout << endl;
  return EXIT_SUCCESS;
}

