/* Id: $Id: xpath_reader.h,v 1.5 2003-09-08 15:45:08 bjoo Exp $
 *
 * File: xpath_reader.h
 *
 * Basic Header file for the XPathReader.
 *
 * This file provides the basic API for XPathReader.
 * including templated Complex (aka TComplex) and Array 
 * support. It relies (and inherits) from BasicXPathReader
 * (see basic_xpath_reader.h) but hides all its functions 
 * from you. This gives a uniform access to 'get' functions 
 * for primitive as well as templated types, and also Isolates
 * you from having to deal with libxml at all.
 *
 * Functions provided:
 *
 * A)   open, close -- same as BasicXPathReader 
 *                   allow you to open XML documents via FILE * or istream
 *
 * B)  countXPath(path_expression) -- -- runs the xpath query 
 *                                     "count(query string)"
 *                                      and returns its value.  
 *
 * C)  getXPath(path_expression, result) -- Executes XPath Query and 
 *                                      tries to put the result into result.
 *      Semantics:
 *         i) getXPath is templated and potentially recursive. 
 *        ii) for simple languge types the semantics are the same 
 *            as for the "get" functions in BasicXPathReader
 *
 *       iii) For complex types TComplex<T>
 *         the data must be marked up as:
 *              <name>
 *                 <cmpx><re> real part </re><im> imag part </im></cmpx>
 *              </name>
 * 
 *        and the XPath expression must uniquely identify the <name> tag.
 *        This will be suggested as standard ILDG markup at ILDG2.
 *        We will then change to whatever ILDG2 ends up standardising.
 *
 *        The getXPath function will construct xpaths:
 *             path_expression/cmpx/re
 *           & path_expression/cmpx/im 
 * 
 *           internally, and will call getXPath<T> recursively.
 *           for each one. This can allow you to have a real part and a 
 *           complex part of an array say. See also the tcomplex.h file
 *           where the TComplex class is defined.
 *
 *       iv) For array types Arra
 *
 *          Array data must be marked up as:
 * 
 *        <name>
 *            <array sizeName="foo" elemName="bar" indexName="idx" 
 *                   indexStart="imin">
 * 
 *              <foo>N</foo>
 *              <bar idx="imin+j"> T(j) </bar>
 *              ...
 *            </array>
 *        </name>
 *
 *      in other words. 
 *          a) The tag specifyin the length of the array is 
 *             specified by attribute sizeName of the <array> tag
 *
 *          b) The tag that distinguishes array elements is specified
 *            by attribute elemName of the <array> tag.
 *
 *          c) Each element carries an array index attribute. The name
 *             of the index attribute is specified by the "indexName" 
 *             attribute of the <array> tag.
 * 
 *          d) The minimal index value, is specified by the indexStart
 *             attribute  if the <array> tag.
 *      
 *          e) following the array declaration, there must be one unique
 *             occurrance of the tag whose name is specified in sizeName
 *             to denote the size of the array ( N in the above example)
 * 
 *          f) There must be N element tags, denoted by the tagname 
 *             elemName, each carrying an index specified by indexName
 *             which must take all the available values between (inclusive)
 *             indexStart and indexStart+N-1. The element tags need not
 *             be ordered, as explicit xpath queries will be made for each
 *             based on the index value.
 *
 *   This markup will be submitted to the ILDG2 for standardisation.
 *
 *   Like with complex numbers, the path given must uniquely identify
 *   the "name" tag.
 *
 *   In terms of operation, getXPathAttribute is used to determine the
 *   values of the attributes in the <array> tag. queries are constructed
 *   for the individual elements matching on the index attribute. 
 *   then getXPath is called RECURSIVELY on each element. 
 * 
 *   Hence getXPath can be used to also parse arrays of arrays.
 *
 *  D)  getXpathAttribute(xpath_to_node, attrib_name, result)
 * 
 *   attempts to get the value of attribute attrib_name into the 
 *   result. This is basically a wrapper for BasicXPathParser::getAttribute
 *   see the rules in basic_xpath_parser.h. Note that it is silly 
 *   to try and call this with say an array type result as attributes
 *   must be primitive types.
 *
 *   Due to the excessive templating, I couldn't separate the code off into
 *   a separate file yet... :(
 */

#ifndef XPATH_READER_H
#define XPATH_READER_H

#include <basic_xpath_reader.h>
#include <tcomplex.h>
#include <array.h>

using namespace std;

namespace XMLXPathReader {

  // Your basic Reader.
  class XPathReader : private BasicXPathReader {
  public:

    XPathReader(void) : BasicXPathReader() {} ;
    XPathReader(istream& is) : BasicXPathReader(is) {} ;
    XPathReader(const string& filename) : BasicXPathReader(filename) {} ;
    ~XPathReader(void) {};

    XPathReader(XPathReader& old, const string& xpath) : BasicXPathReader((BasicXPathReader&) old, xpath) {} ;


    void open(const string& filename) { 
      BasicXPathReader::open(filename);
    }

    void open(istream& is) { 
      BasicXPathReader::open(is);
    }

    void close(void) { 
      BasicXPathReader::close();
    }

    // evaluate "count(xpath)" and return an INTEGER
    int countXPath(const string& xpath) {
      return BasicXPathReader::count(xpath);
    }

    // print out xml document into ostream os 
    void printDocument(ostream& os) {
      BasicXPathReader::print(os);
    }

    // print out xml document from root element into ostream os
    void printRoot(ostream& os) { 
      BasicXPathReader::printRoot(os);
    }

    // print out xml from the node selected by the xpath_to_node
    // expression into ostream os
    void printXPathNode(ostream& os, const string& xpath_to_node) { 
      BasicXPathReader::printXPathNode(os, xpath_to_node);
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


    // Match TComplex's
    template< typename T >
      void
      getXPath(const string& xpath, TComplex<T>& result)
      {
	ostringstream error_message;


	XPathReader complex_top(*this, xpath);

	try { 
	  complex_top.getXPath("cmpx/re", result.real());
	}
	catch(const string &e) {
	  error_message << "XPath Query: " << xpath << " Error: "
			<< "Failed to match real part of TComplex Object with self constructed path: re" ;

	  complex_top.close();
	  throw error_message.str();
	}

	try { 
	  complex_top.getXPath("cmpx/im", result.imag());
	}
	catch(const string &e) {
	  error_message << "XPath Query: " << xpath << " Error: "
			<< "Failed to match real part of TComplex Object with self constructed path: ./cmpx/im" ;

	  complex_top.close();
	  throw error_message.str();
	}	

      }

    // getXPath for Arrays
    template< typename T > 
      void 
      getXPath(const string& xpath, Array<T>& result) 
      {
	ostringstream error_message;

	XPathReader arraytop(*this, xpath);
	
	string array_xpath = "array";

	// Values to be gleaned from attributes of <array>
	string sizeName="";
	string elemName="";
	string indexName="";
	int indexStart;
	
	// Try and get each attribute in turn with getXPathAttribute
	//
	// get sizeName attribute
	try {
	  arraytop.getXPathAttribute(array_xpath, "sizeName", sizeName);
	} 
	catch(const string& e) { 
	  error_message << "Couldn't match sizeName attribute for array"
			<< " starting at XPath: " << xpath
			<< endl
			<< "array_xpath is: " << array_xpath;

	  arraytop.close();
	  throw error_message.str();
	}

	// get elemName attribute
	try {
	  arraytop.getXPathAttribute(array_xpath, "elemName", elemName);
	} 
	catch(const string& e) { 
	  error_message << "Couldn't match elemName attribute for array"
			<< " starting at XPath: " << xpath
			<< endl
			<< "array_xpath is: " << array_xpath;

	  arraytop.close();
	  throw error_message.str();
	}

	// get indexName attribute
	try {
	  arraytop.getXPathAttribute(array_xpath, "indexName", indexName);
	} 
	catch(const string& e) { 
	  error_message << "Couldn't match indexName attribute for array"
			<< " starting at XPath: " << xpath
			<< endl
			<< "array_xpath is: " << array_xpath;
	  arraytop.close();
	  throw error_message.str();
	}

	// get index start attribute
	try {
	  arraytop.getXPathAttribute(array_xpath, "indexStart", indexStart);
	} 
	catch(const string& e) { 
	  error_message << "Couldn't match index attribute for array"
			<< " starting at XPath: " << xpath
			<< endl
			<< "array_xpath is: " << array_xpath;

	  arraytop.close();
	  throw error_message.str();
	}

	// Construct query to read size of array from <sizeName> 
	// tag
	XPathReader arrayelem(arraytop, array_xpath);


	string n_elem_query =  sizeName;
	int array_size; 

	// try and do the read
	try {
	  cout << "Getting no of array elements with query: " << n_elem_query << endl;

	  arrayelem.getXPath(n_elem_query, array_size) ;
	}
	catch( const string& e) {
	  error_message << "Couldn't match array size tag: " << sizeName
			<< "with XPath Query: " << n_elem_query
			<< endl;

	  arraytop.close();
	  arrayelem.close();
	  throw error_message.str();
	}
	
	// Count the number of elements
	string elem_base_query = elemName;
	
	int array_size_check;
	try {

	  cout << "Getting array size check with count of "<<elem_base_query<< endl;
	  array_size_check = arrayelem.countXPath(elem_base_query);
	}
	catch( const string& e) { 
	  error_message << "Exception occurred while counting " << elem_base_query 
			<< " during array read " << endl;

	  arraytop.close();
	  arrayelem.close();
	}
      
	// If there is disagreement, that is a problem
	if( array_size_check != array_size ) { 
	  error_message << "Array markup claims array has " << array_size 
			<< " elements but " << array_size_check 
			<< " were counted" << endl;

	  arraytop.close();
	  arrayelem.close();
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
	    
	    arrayelem.getXPath(element_xpath.str(), result[i]);
	   

	  } catch( const string& e ) {
	    error_message << "Failed to match element " << i
			  << " of array with query " << element_xpath.str()
			  << endl
			  << "Query returned error: " << e;

	    arraytop.close();
	    arrayelem.close();
	    throw error_message.str();
	  }

	}


	// Arraytop and Arrayelem should get disposed of here, and 
	// should decrease their refcount on 'the document'
	arraytop.close();
	arrayelem.close();
      }
  };


};

    






#endif
