// $Id: qdp_xpath_reader.h,v 1.1 2003-04-28 03:02:13 edwards Exp $

#ifndef XPATH_READER_H
#define XPATH_READER_H

#include <basic_xpath_reader.h>

using namespace std;

namespace XMLXPathReader {

  // Your basic Reader.
  class QDPXPathReader : private BasicXPathReader {
  public:

    // Open file. filename contains name of file.
    void open(const string& filename) {
      BasicXPathReader::open(filename);
    }

    // Open from stream. 
    void open(istream& is) {
      BasicXPathReader::open(is);
    }

    // Close & cleanup
    void close(void) {
      BasicXPathReader::close();
    }

    // evaluate "count(xpath)" and return an INTEGER
    int countXPath(const string& xpath) {
      return BasicXPathReader::count(xpath);
    }

    // Basic getXPathAttribute function -- calls 
    //  BasicXPath parsers getAttribute functions
    template <typename T>
      void
      getXPathAttribute(const string& xpath_to_node,
			const string& attrib_name, 
			T& value)
      {
	getAttribute(xpath_to_node, attrib_name, value);
      }

		     
    // Match primtive types (+ general tempate declaration)
    template< typename T > 
      void
      getXPath(const string& xpath, T& result)
      {
	get(xpath, result);
      }


    // Match OScalar
    template< typename T >
      void
      getXPath(const string& xpath, OScalar<T>& result)
      {
	ostringstream error_message;
	
	// Nothing to match, so continue search
	try { 
	  getXPath(xpath, result.elem());
	}
	catch(const string &e) {
	  error_message << "XPath Query: " << xpath << " Error: "
			<< "Failed to match underneath OScalar Object with self constructed path: " << path_real;
	  
	  throw error_message.str();
	}

      }

    // Match PScalar
    template< typename T >
      void
      getXPath(const string& xpath, PScalar<T>& result)
      {
	ostringstream error_message;
	
	// Nothing to match, so continue search
	try { 
	  getXPath(xpath, result.elem());
	}
	catch(const string &e) {
	  error_message << "XPath Query: " << xpath << " Error: "
			<< "Failed to match underneath PScalar Object with self constructed path: " << path_real;
	  
	  throw error_message.str();
	}

      }

    // Match RScalar
    template< typename T >
      void
      getXPath(const string& xpath, RScalar<T>& result)
      {
	ostringstream error_message;
	
	// Nothing to match, so continue search
	try { 
	  getXPath(xpath, result.elem());
	}
	catch(const string &e) {
	  error_message << "XPath Query: " << xpath << " Error: "
			<< "Failed to match underneath RScalar Object with self constructed path: " << path_real;
	  
	  throw error_message.str();
	}

      }

    // Match RComplex
    template< typename T >
      void
      getXPath(const string& xpath, RComplex<T>& result)
      {
	ostringstream error_message;
	
	// XPath for the real part 
	string path_real = xpath + "/cmpx/re";
	
	// XPath for the imaginary part.
	string path_imag = xpath + "/cmpx/im";
	
	// Try and recursively get the real part
	try { 
	  getXPath(path_real, result.real());
	}
	catch(const string &e) {
	  error_message << "XPath Query: " << xpath << " Error: "
			<< "Failed to match real part of RComplex Object with self constructed path: " << path_real;
	  
	  throw error_message.str();
	}
	
	// Try and recursively get the imaginary part
	try {
	  getXPath(path_imag, result.imag());
	}
	catch(const string &e) {
	  error_message << "XPath Query: " << xpath <<" Error:"
			<<"Failed to match imaginary part of RComplex Object with self constructed path: " << path_imag;
	  
	  throw error_message.str();
	}
      }

    // getXPath for Arrays
    template< typename T > 
      void 
      getXPath(const string& xpath, multi1d<T>& result) 
      {
	ostringstream error_message;

	// XPath to array tag
	string array_xpath = xpath + "/array";

	// Values to be gleaned from attributes of <array>
	string sizeName="";
	string elemName="";
	string indexName="";
	int indexStart;
	
	// Try and get each attribute in turn with getXPathAttribute
	//
	// get sizeName attribute
	try {
	  getXPathAttribute(array_xpath, "sizeName", sizeName);
	} 
	catch(const string& e) { 
	  error_message << "Couldn't match sizeName attribute for array"
			<< " starting at XPath: " << xpath
			<< endl
			<< "array_xpath is: " << array_xpath;
	  throw error_message.str();
	}

	// get elemName attribute
	try {
	  getXPathAttribute(array_xpath, "elemName", elemName);
	} 
	catch(const string& e) { 
	  error_message << "Couldn't match elemName attribute for array"
			<< " starting at XPath: " << xpath
			<< endl
			<< "array_xpath is: " << array_xpath;
	  throw error_message.str();
	}

	// get indexName attribute
	try {
	  getXPathAttribute(array_xpath, "indexName", indexName);
	} 
	catch(const string& e) { 
	  error_message << "Couldn't match indexName attribute for array"
			<< " starting at XPath: " << xpath
			<< endl
			<< "array_xpath is: " << array_xpath;
	  throw error_message.str();
	}

	// get index start attribute
	try {
	  getXPathAttribute(array_xpath, "indexStart", indexStart);
	} 
	catch(const string& e) { 
	  error_message << "Couldn't match index attribute for array"
			<< " starting at XPath: " << xpath
			<< endl
			<< "array_xpath is: " << array_xpath;
	  throw error_message.str();
	}

	// Construct query to read size of array from <sizeName> 
	// tag
	string n_elem_query = array_xpath + "/" + sizeName;
	int array_size; 

	// try and do the read
	try {
	  getXPath(n_elem_query, array_size) ;
	}
	catch( const string& e) {
	  error_message << "Couldn't match array size tag: " << sizeName
			<< "with XPath Query: " << n_elem_query
			<< endl;
	  throw error_message.str();
	}
	
	// Count the number of elements
	string elem_base_query = array_xpath + "/" + elemName;
	
	int array_size_check;
	try {
	  array_size_check = countXPath(elem_base_query);
	}
	catch( const string& e) { 
	  error_message << "Exception occurred while counting " << elem_base_query 
			<< " during array read " << endl;
	}
      
	// If there is disagreement, that is a problem
	if( array_size_check != array_size ) { 
	  error_message << "multi1d markup claims array has " << array_size 
			<< " elements but " << array_size_check 
			<< " were counted" << endl;

	  throw error_message.str();
	}

	// Now resize the array to hold the no of elements.
	// simple thanks to Robert Edwards' fantastic array class.
	result.resize(array_size);

	// Get the elements one by one
	int i;
	for(i = 0; i < array_size; i++) { 
	  ostringstream element_xpath;

	  // Create the query for the element 
	  element_xpath << elem_base_query  
			<< "[number(@" + indexName +")=" 
			<< (i+indexStart) << "]";

	  // recursively try and get the element.
	  try {
	    getXPath(element_xpath.str(), result[i]);
	  } catch( const string& e ) {
	    error_message << "Failed to match element " << i
			  << " of array with query " << element_xpath.str()
			  << endl
			  << "Query returned error: " << e;
	    throw error_message.str();
	  }
	}
      }
    
  };


};

    






#endif
