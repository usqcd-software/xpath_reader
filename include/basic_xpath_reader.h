/* $Id: basic_xpath_reader.h,v 1.14 2005-02-28 16:46:38 bjoo Exp $
 *
 * File: basic_xpath_reader.h
 *
 * Description: 
 *
 * This file specifies the basic functionality of the reader.
 * 
 * These consist of 
 *   i) Opening files
 *  ii) Getting at primitive types (non-templated types)
 * iii) Getting at attributes
 *  iv) Executing arbitrary XPATH queries
 *   v) Displaying the results of the Xpath Queries
 *  vi) Cleaning up
 *
 * Despite the length of this file there are only a few functions 
 * you need to worry about here, since most of the file deals with 
 * overloading things.
 *
 * Functionality:
 *
 *   open -- from file or istream: opens a document
 *   close -- cleans up 
 * 
 *   evaluateXPath(query string) -- runs the XPATH query
 *     the query result is returned in the internal variable
 *     query result which is an xmlXPathObjectPtr. If you want to
 *     mess with this you have to get down to the level of libxml2
 *
 *   printQueryResult(ostream) -- print the results of the query to 
 *                                the ostream supplied. Useful if you
 *                                need the result tagset of the query as
 *                                a string, or for debugging or whatever.
 *
 *   count(query_string)      -- runs the xpath query "count(query string)"
 *                               and returns its value.  
 * 
 *   AND THE MOST IMPORTANT FOR LAST:
 *
 *   get(xpath_query_string, <result>) -- Executes the XPath Query
 *                                and stores the result in the variable
 *                                result. This is a highly overloaded function
 *                                and can deal with most language basic types
 *                                (int, unsigned int,
 *                                 short, unsigned short,
 *                                 long,  unsigned long,
 *                                 float, double, 
 *                                 bool, std::string)
 *
 *   The semantics are: 
 *       - the query string must identify a unique element node with simple 
 *         content
 *
 *       - only 1 occurrance of the desired object can occur in the 
 *         identified tag. I.e: sequences of object within type eg <foo> 2 3
 *         </foo> are not allowed. In the case of string, the tag can contain
 *         and arbitrary string. 
 *
 *       - internalisation is LOST. xmlChar -s are converted to char * and 
 *         then to std:string
 *
 *       - boolean values have to be "true" or "false" rather than 0 or 1.
 *
 *       Given that the above are satisfied failure can still occur if 
 *       the >> operator for the type cannot convert from the string of the 
 *       type to the desired type.
 *
 * 
 *   getAttribute(xpath_query_to_node, attribute_name, <result>)
 * 
 *      This is much like get, except it tries to get attributes attached
 *      to a given element node. The same amount of overloading is done
 *      as for get.
 *
 *   Semantics are:
 *    
 *       - the xpath_query_to_node must identify a unique element node
 *         which has the desired attribute.
 *          
 *       - the semantics of the form of the attribute are the same as for
 *         get()
 * 
 * 
 * ERRORS ETC:
 * ===========
 *
 *  If the semantics are not met, or queries fail, C++ exceptions will be 
 *  thrown. These need to be caught to stop your program from "Abort" ing.
 *  The type of exceptions thrown in this library is always const string&
 *  so that you can immediately dump the error message to wherever you like
 *
 *  Have fun,
 *     Balint
 */

#ifndef BASIC_XPATH_READER_H
#define BASIC_XPATH_READER_H


#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <ostream>
#include <istream>
#include <iostream>
#include <sstream>
#include <string>
#include <document.h>
#include <list>

using namespace std;

namespace XMLXPathReader {

  static bool xpath_is_initialised = false;

  static void initXPath(void) {
    xmlXPathInit();
    xpath_is_initialised = true;
  }

  class NameSpace { 
  public: 
    NameSpace(const string& _prefix, const string& _uri) {
      prefix = _prefix;
      uri = _uri;
    }
    
    const string& getPrefix() {
      return prefix;
    }

    const string& getURI() {
      return uri;
    }
      
    private:
      string prefix;
      string uri;
  };

  class BasicXPathReader {
  public:

    /* The meaning of these should be clear to you */
    // BasicXPathReader(XMLDocument& doc);

    BasicXPathReader(void) {
      if( xpath_is_initialised != true ) { 
	initXPath();
      }

      docref = 0x0;
      xpath_context = NULL;
      query_result = NULL;
    }

    BasicXPathReader(istream& is) { 
     if( xpath_is_initialised != true ) { 
	initXPath();
      }

      docref = 0x0;
      xpath_context = NULL;
      query_result = NULL; 
      try {
	open(is);
      }
      catch( const string& e) {
	throw;
      }
    }
    
    BasicXPathReader(const string& filename)  { 
     if( xpath_is_initialised != true ) { 
	initXPath();
      }

      docref = 0x0;
      xpath_context = NULL;
      query_result = NULL;
      try { 
	open(filename);
      }
      catch( const string& e) {
	throw;
      }
    }
    
    ~BasicXPathReader(void) {
      close();
    }
    
    /* This should let you clone an XPath reader */
    BasicXPathReader(BasicXPathReader& old, const string& xpath) {

      docref=0x0;
      xpath_context = NULL;
      query_result = NULL;

      open(old, xpath);
    }
    

    void open(const string& filename) { 
      // There are two possibilities to opening
      //
      // i) Reader is already open
      // ii) The reader is not yet open.
      //
      //
      // case i) Reader is already open.
      close();
      
      // Ok, open the document
      try { 
	
	docref = new(nothrow) XMLDocument(filename);
	
	if( docref == 0x0 ) {
	  ostringstream error_stream;
	  error_stream << "Unable to create new XML Document using open(filename) filename=" << filename << ")";
	  throw error_stream.str();
	}
	
	// Make sure the document feels referred to.
	docref->increaseRefcount();
	query_result = (xmlXPathObjectPtr) NULL;
	xmlDocPtr doc = docref->getDoc();
	xpath_context = xmlXPathNewContext(doc);
	// snarfNamespaces(xmlDocGetRootElement(doc), xpath_context);
      }
      catch ( const string& e ) { 
	throw;
      }
    }
    
    void open(istream &is) {
      // There are two possibilities to opening
      //
      // i) Reader is already open
      // ii) The reader is not yet open.
      //
      //
      // case i) Reader is already open.
      close();
      
      // Ok, open the document
      try { 
	
	docref = new(nothrow) XMLDocument(is);
	
	if( docref == 0x0 ) {
	  ostringstream error_stream;
	  error_stream << "Unable to create new XML Document using open(istream& is)";
	  throw error_stream.str();
	}
	
	// Make sure the document feels referred to.
	docref->increaseRefcount();
	
	query_result = (xmlXPathObjectPtr) NULL;
	xmlDocPtr doc = docref->getDoc();
	xpath_context = xmlXPathNewContext(doc);
        // snarfNamespaces(xmlDocGetRootElement(doc), xpath_context);
      }
      catch ( const string& e ) { 
	throw;
      }
    }

    /* This should let you clone an XPath reader */
    void open(BasicXPathReader& old, const string& xpath) {
      close();

      if( old.docref != 0x0) { 
	
	docref = old.docref;
	
	query_result = (xmlXPathObjectPtr) NULL;
	xmlDocPtr doc = docref->getDoc();
	xpath_context = xmlXPathNewContext(doc);

	// Now execute the xpath query and set the context node
	ostringstream error_message;
	
	try { 
	  old.evaluateXPath(xpath);
	}
	catch ( const string& e ) { 
	  throw e;
	}
	
	// Check that the query returned non-empty result
	try { 
	  old.checkQuery(xpath);
	}
	catch( const string& e) { 
	  throw;
	}
	
	//Check that the node set contains only 1 element
	if( old.query_result->nodesetval->nodeNr != 1 ) {
	  error_message << "XPath Query: " << xpath << " did not return unique node."
			<< " nodes returned = " << old.query_result->nodesetval->nodeNr 
			<< endl;
	  throw error_message.str();
	}
	
	// Check that the node returned is an element
	xpath_context->node = old.query_result->nodesetval->nodeTab[0];

	list<NameSpace>::iterator iter;
	for( iter = old.nslist.begin(); iter != old.nslist.end(); iter++) {
	  registerNamespace(iter->getPrefix(), iter->getURI());
	}
	
	docref->increaseRefcount();
      }
      else { 
	throw "Attempting to clone a closed Reader";
      }
      
    }
      
    void close(void) { 
      if( docref != 0x0 ) { 
	// Reader is already open. 
	// We detach from the current document:
	
	// decrement its refcount
	docref->decreaseRefcount();
	
	// if decrementing the refcount means there are no more references
	// then delete the object -- this ought to call the destructor
	if ( docref->getRefcount() == 0 ) { 
#ifdef DEBUG_XML_REFCOUNT
	  cout << "Reader: docrefs refcount reached 0. Deleting" << endl;
#endif
	  delete docref;
	}
	
	// We are now officially not open 
	docref = 0x0;
	
	// Clean up any left-over query result
	if ( query_result != NULL ) { 
	  xmlXPathFreeObject(query_result);
	}
	
	// Clean up the XPath content
	if( xpath_context != NULL ) { 
	  xmlXPathFreeContext(xpath_context);
	}

	nslist.clear();
      }
      else {
	// Else we are not open so closing will do sod all
      }      
    }


    /* So should these, there is just a lot of overloading */
    void get(const string& xpath, string& result) {
      
      // Try and get the string for the xpath
      try {
	getPrimitiveString(xpath, result);
      }
      catch(const string& e) { 
	// Pass up exception if it occurs
	throw e;
      }
    }

    void get(const string& xpath, int& result) { 
      getPrimitive<int>(xpath, result, "int");
    }


    void get(const string& xpath, unsigned int& result) {
      getPrimitive<unsigned int>(xpath, result, "unsigned int");
    }
    
    void get(const string& xpath, short int& result) { 
      getPrimitive<short int>(xpath, result,"short int");
    }

    void get(const string& xpath, unsigned short int& result) {
      getPrimitive<unsigned short int>(xpath, result, "unsigned short int");
    }

    void get(const string& xpath, long int& result) { 
        getPrimitive<long int>(xpath, result, "long int");
    }
    
    void get(const string& xpath, unsigned long int& result) {
      getPrimitive<unsigned long int>(xpath, result, "unsigned long int");
    }
    
    void get(const string& xpath, float& result) {
      getPrimitive<float>(xpath, result, "float");
    }

    void get(const string& xpath, double& result) {
      getPrimitive<double>(xpath, result, "double");
    }

    void get(const string& xpath, bool& result) { 
      getPrimitive<bool>(xpath, result, "bool");
    }

    /* So should these, especially if you read the introductory comments */
    void getAttribute(const string& xpath_to_node,
		      const string& attrib_name, 
		      string& result) {
      getAttributeString(xpath_to_node, attrib_name, result);
    }
    
    void getAttribute(const string& xpath_to_node,
		      const string& attrib_name, 
		      int& result) {
      getPrimitiveAttribute<int>(xpath_to_node, attrib_name, result, "int");
    }
    
    void getAttribute(const string& xpath_to_node,
		      const string& attrib_name, 
		      unsigned int& result) {
      getPrimitiveAttribute<unsigned int>(xpath_to_node, attrib_name, result, 
					  "unsigned int");
    }
    
    void getAttribute(const string& xpath_to_node,
		      const string& attrib_name, 
		      short int& result) {
      getPrimitiveAttribute<short int>(xpath_to_node, attrib_name, result, 
				       "short int");
    }
    
    void getAttribute(const string& xpath_to_node,
		      const string& attrib_name, 
		      unsigned short int& result) { 
      getPrimitiveAttribute<unsigned short int>(xpath_to_node, 
						attrib_name, result, 
						"unsigned short int");
    }
    
    void getAttribute(const string& xpath_to_node,
		      const string& attrib_name, 
		      long int& result) {
      getPrimitiveAttribute<long int>(xpath_to_node, attrib_name, result, 
				      "long int");
    }
    
    void getAttribute(const string& xpath_to_node,
		      const string& attrib_name, 
		      unsigned long int& result) { 
      getPrimitiveAttribute<unsigned long int>(xpath_to_node, attrib_name, result, 
					       "unsigned long int");
    }

    void getAttribute(const string& xpath_to_node,
		      const string& attrib_name, 
		      float& result) { 
      getPrimitiveAttribute<float>(xpath_to_node, attrib_name, result, 
				   "float");
    }
    
    void getAttribute(const string& xpath_to_node,
		      const string& attrib_name, 
		      double& result) {
      
      getPrimitiveAttribute<double>(xpath_to_node, attrib_name, result, 
				    "double");
    }

    void getAttribute(const string& xpath_to_node,
		      const string& attrib_name, 
		      bool& result) {
      getPrimitiveAttribute<bool>(xpath_to_node, attrib_name, result, 
				  "bool");
    }

    int count(const string& xpath) { 
      ostringstream error_message;
      int ret_val;
      
      // Construct query 
      string query="count("+xpath+")";
      
      // Run query 
      // This is the bit that checks that what we are doing is legit
      
      try { 
	evaluateXPath(query);
      }
      catch(const string& e) {
	throw e;
      }
      
      
      // Check that the result is a number otherwise something is very wrong! 
      if( query_result->type != XPATH_NUMBER ) {
	error_message << "XPath Query: " << query
		      << " did not succeed" << endl;
	throw error_message.str();
      }
      else {
	// It is  a number -- goodie. Cast to int. This should be OK, 
	// as you cant count a non-integral no of things.
	ret_val = (int)(query_result->floatval);
      }
      
      // Post query cleanup
      if ( query_result != NULL ) { 
	xmlXPathFreeObject(query_result);
	query_result = NULL;
      }  
      
      // return the number 
      return ret_val;      
    }

    void evaluateXPath(const string& xpath) {
      
      ostringstream error_message;
      
      // Guard against queries of empty readers
      if ( docref == 0 ) { 
	error_message << "Attempting to execute a query on an empty Reader";
	throw error_message.str();
      }
      
      // Make sure previos queries didn't leave anything kicking about 
      if( query_result != NULL ) { 
	xmlXPathFreeObject(query_result);
	query_result = NULL;
      }
      
      // Call libxml2 to run the xpath query 
      query_result = xmlXPathEval((const xmlChar *)xpath.c_str(),
				  xpath_context);
      
      // if the query is null throw up our hands in despair 
      if( query_result == NULL ) { 
	error_message << "XPath Query " << xpath << " Failed! " << endl;
	throw error_message.str();
      }
    }
    
    


    void printQueryResult(ostream& os);

    /*    void setCurrentXPath(const string& xpath);
    xmlNodePtr getCurrentContextNode(void);
    void setCurrentContextNode(xmlNodePtr new_context_node);
    */

    //! Print self from context node down -- include context node
    void print(ostream& os);

    //! Print the root element as a stream
    void printRoot(ostream& os);

    //! Print the children of the context node -- dont include context node
    void printChildren(ostream& os);

    void printDoc(ostream& os);


    //! Print an element selected by XPath
    void printXPathNode(ostream& os, const string& xpath_to_node);

    void registerNamespace(const string& prefix, const string& uri) {
      if( docref != 0x0 ) { 
	if( xpath_context != 0x0 ) { 
	  xmlXPathRegisterNs(xpath_context, (xmlChar *)prefix.c_str(),
			     (xmlChar *)uri.c_str());
	  nslist.push_back(NameSpace(prefix, uri));
	}
      }
    }

  private:
    /* Stuff needed by libxml */
    XMLDocument* docref;
    xmlXPathContextPtr xpath_context;
    xmlXPathObjectPtr  query_result;
    list<NameSpace> nslist;


    /*! This contentious piece of code goes through the whole "tree"
     *  and registers every single namespace it finds into the XPath context.
     *  This may or may not be standard compliant as is */
#if 0
    void 
      snarfNamespaces(xmlNodePtr current_node,
		      xmlXPathContextPtr xpath_context) {
      
      ostringstream error_message;
      xmlNsPtr *ns_ptr_array;
      xmlNsPtr *shadow;
      xmlDocPtr doc;
      int i;

      doc = docref->getDoc();

      // Get the namespaces applying to the current node
      ns_ptr_array = xmlGetNsList(doc,current_node);
    
      // Walk list and register xpath
      if(  ns_ptr_array != NULL ) {
	shadow = ns_ptr_array;
	while( *shadow != NULL ) {
	  if( (*shadow)->prefix != NULL ) { 
	    xmlXPathRegisterNs(xpath_context, (*shadow)->prefix, (*shadow)->href);
	  }
	  shadow++; // Next
	}
   
	/* xmlFreeNsList(*ns_ptr_array); */
      }
      
    }
#endif

    /* Ensure that the query returned something non-null */
    void checkQuery(const string& xpath) {
      ostringstream error_message;
      // Check that the result of the XPath is a node set
      // Note: don't have to worry about query_result being NULL,
      // that was already checked in evaluate XPath
	if( query_result != NULL ) { 
	  
	  if( query_result->type != XPATH_NODESET ) {
	    error_message << "XPath Query: " << xpath << " didn't return a node_set" 
			  << endl;
	    throw error_message.str();
	  }
	  
	  // Check that the nodesetval is not NULL
	  if( query_result->nodesetval == 0x0 ) { 
	    error_message << "XPath Query: " << xpath << " returned a NULL node set"
			  << endl;
	    throw error_message.str();
	  }
	}
	else { 
	  throw "query_result is NULL in checkQuery";
	}
      
    }
 

    /* Ensure that the query has returned something non-null, 
       and that the query result consists of a unique node with
       simple content */
    void checkQueryPrimitive(const string& xpath) {
      ostringstream error_message;
      
      // first check that the query result is a nonnempty node set
      try { 
	checkQuery(xpath);
      } catch(const string& e) {
	throw e;
      }
      
      // Check that the node set contains only 1 element
      if( query_result->nodesetval->nodeNr != 1 ) {
	error_message << "XPath Query: " << xpath << " did not return unique node."
		      << " nodes returned = " << query_result->nodesetval->nodeNr 
		      << endl;
	throw error_message.str();
      }
      
      // Check that the node returned is an element 
      xmlNodePtr res_node = query_result->nodesetval->nodeTab[0];
      
      if ( res_node->type != XML_ELEMENT_NODE ) {
	error_message << "XPath Query: " << xpath << " returned a non-element node"
		      << endl;
	throw error_message.str();
      }
      
      // Check that the element returned is simple (ie contains no more elements)
      if( res_node->children != NULL) { 
	for( res_node=res_node->children; res_node != NULL; 
	     res_node=res_node->next ) {
	  if( res_node->type == XML_ELEMENT_NODE ) { 
	    error_message << "XPath Query: " << xpath << " returned non-simple node" << endl;
	    throw error_message.str();
	  }
	}
      }
    }
    

    /* Get a string from a path expression that matches checkQueryPrimitive.
     * this string is copied into result, after which the query result
     * is freed (hopefully properly).
     */
    void getPrimitiveString(const string& xpath, string& result)
    {
      ostringstream error_message;


      // Run the query 
      try { 
	evaluateXPath(xpath);
      }
      catch(const string& e) {
	cerr << e;
	throw e;
      }
      
      try { 
	checkQueryPrimitive(xpath);
      }
      catch(const string& e) {
	throw e;
      }
      
      
      // get the "simple" content as a string.
      xmlChar *element_content = xmlNodeGetContent(query_result->nodesetval->nodeTab[0]);
      
      // check it for non-nullness */
      if(element_content != NULL ) {
	// COPY it into result 
	result = (string)((const char *)element_content);
	// Free memory malloced by libxml2
	xmlFree(element_content); 
      }
      else {
	// otherwise the query ran, but the node returned is empty
	result = "";
      }
      
      // Clean up post query
      if ( query_result != NULL ) { 
	xmlXPathFreeObject(query_result);
	query_result = NULL;
      }
      
    }
    
    /* Get the attribute string from a query, that satisfies 
       checkQuery, and is a unique element node ( but doesn't have to have 
       simple content). It copies the attribute into result after which 
       the query result is freed (hopefully properly).
    */
    void getAttributeString(const string& xpath_to_node,
			    const string& attrib_name,
			    string& result) {
      ostringstream error_message;

      // First check that the xpath to node identifies a unique node
      
      // run the query 
      // This is the bit that checks that we have a document to query 
      try {
	evaluateXPath(xpath_to_node);
      }
      catch (const string& e) {
	throw e;
      }
      
      // check for general non nullness
      try { 
	checkQuery(xpath_to_node);
      }
      catch (const string& e) {
	throw e;
      }
      
      // check node for uniqueness
      if( query_result->nodesetval->nodeNr != 1 ) { 
	error_message << "XPath Query: " << xpath_to_node
		      << " does not identify a unique node" ;
	throw error_message.str();
      }
      
      // OK, so at this point a unique node is identified by xpath_to_node
      //
      // construct query to get the attribute
      string query = "string(" + xpath_to_node + "/@" + attrib_name + ")";
      
      // run the query 
      try { 
	evaluateXPath(query);
      }
      catch(const string& e) {
	error_message << "Failed to find attribute: " << attrib_name 
		      << " on node selected by XPath Query: " << xpath_to_node;
	throw error_message.str();
      }
      
      // check that we have a result
      if( query_result == NULL ) {
	error_message << "Query for attribute: " << attrib_name 
		      << " on node selected by XPath Query: " << xpath_to_node
		      << " returned NULL query result";
	throw error_message.str();
      }
      
      // check that the result is a string
      if( query_result->type != XPATH_STRING ) {
	error_message << "Query for attribute: " << attrib_name
		      << " on node selected by XPath Query: " << xpath_to_node
		      << " didn't return a string value";
	throw error_message.str();
      }
      
      // if the string is null then it presumably is an empty string(?)
      if( query_result->stringval == NULL ) {
	result = "";
      }
      else {
	// otherwise COPY the result string into result.
	result = string((const char *)(query_result->stringval));
      }
      
      // Post query cleanup
      if ( query_result != NULL ) { 
	xmlXPathFreeObject(query_result);
	query_result = NULL;
      }
    }


    /* Once you snarfed your data out of s, call this function to check
       there are no further whitespaces (ie another string) following 
       -- inelegant, but I don't know an elegant way without casting 
       -- doubles etc. */
    bool  findNonWhitespace(istringstream& s) {
      char search;
	
      // While the stream is not empty 
      while(!s.eof()) { 
	
	//getchar 
	s.get(search);
	
	// if its not a space return true -- (ie found non-whitespace) 
	if( !isspace(search) ) { 
	  return true;
	}
      }
	
      // Ok we reached eof without finding a non-whitespace.
      return false;
    }
    
    /* Templated getPrimitive function. Called by all the overloaded
       get wrappers. This level of indirection also allows that the 
       wrappers submit a string "ptype" containing their type, so that
       you can check what type fails to be matched. I may eliminated 
       this and simply have a templated get --- Dunno for now.
          -- For any given T, it does the following:
	     i) calls getPrimitiveString to get the element content
	     ii) dumps the content string into an istringstream
	     iii) tries to read back from the istringstream into T,
                  converting it if possible. Failure throws exception.
	     iv)  checks that the istringstream contains no more non
	          whitespaces after T, ie T is unique in the tag.
    */

    template< typename T >
      void
      getPrimitive(const string& xpath, T& result, const char* ptype) {
        string pstring;

	// Get the string value of the query 
	ostringstream error_message;
	try {
	  getPrimitiveString(xpath, pstring);
	}
	catch(const string& e) { 
	  throw e;
	}
	
	// Make an istringstream from the string value 
	istringstream is(pstring);
	ios::iostate state;
	
	// Save the exception mask of the istringstream 
	state = is.exceptions();
	
	// Force the istringstream to throw an exception if it fails 
	// (to convert the string to our type)
	is.exceptions(ios::failbit);
	
	// Try to read the type from the istringstream. 
	//   bool-s should be "true" or "false" stirngs 
	try { 
	  is >> boolalpha >> result;
	}
	catch(istringstream::failure e) { 
	  error_message << "XPath Query: " << xpath 
			<< ": Error: Failed to convert query result string: " 
			<< pstring << " to " << ptype;
	  throw error_message.str();
	}
	
	// Turn off exceptions on failure
	is.exceptions(state);
	
	// Look for non whitespaces in whats left in the stream 
	// (ie in what's left from the string
	  if( findNonWhitespace(is) ) {
	    error_message << "XPath Query: " << xpath << ": Error: query result "  << pstring << " does not contain a single " << ptype;
	    throw error_message.str();
	  }
    }
    

    /* Templated getAttribute function, much like getPrimitive<T>. 
     * apart from the case of string, all the overloaded getAttribute
     * functions wrap this. It does the following:
     * 
     *    i) call getAttributeString to match the attribute
     *   ii) converts to desired type in the same way as getPrimitive
     */
    template< typename T >
      void
      getPrimitiveAttribute(const string& xpath_to_node,
			    const string& attrib_name,
			    T& result,
			    const char* ptype)
      {
	string stringval;
	ostringstream error_message;
	
	// Get the string value of the attribute
	try {
	  getAttributeString(xpath_to_node, attrib_name, stringval);
	}
	catch (const string& e) {
	  throw e;
	}
	
	// Put it into an istringstream
	istringstream is(stringval);
	
	// Save the default exception mask (throw no exceptions)
	ios::iostate state;
	state = is.exceptions();
	
	// make the istringstream throw a wobbly on conversion failures 
	is.exceptions(ios::failbit);
	
	// Do the conversion, booleans must be "true" or "false"
	try { 
	  is >> boolalpha >> result;
	}
	catch(istringstream::failure e) { 
	  error_message << "Failed to convert attribute string: " << stringval
			<< " to type " << ptype << " for attribute " << attrib_name
			<< " on node identified by XPath: " << xpath_to_node;
	  throw error_message.str();
	}
	
	// Restore excpetion mask (no exceptions)
	is.exceptions(state);
	
	// look for non-whitespaces following.
	if( findNonWhitespace(is) ) {
	  error_message << "Attribute string: " << stringval
			<< " for attribute " << attrib_name
			<< " on node identified by XPath: "
			<< " doesn't contain a single " << ptype;
	  throw error_message.str();
	}
      }
    
    /* Generic function to print a node to an ostream */
    void printNode(ostream& os, xmlNodePtr node);


  };

};
#endif  
  
