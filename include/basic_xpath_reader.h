/* $Id: basic_xpath_reader.h,v 1.5 2003-08-25 10:48:59 bjoo Exp $
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
#include <sstream>
#include <string>


using namespace std;

namespace XMLXPathReader {

  class BasicXPathReader {
  public:

    /* The meaning of these should be clear to you */
    void open(const string& filename);
    void open(istream& is);
    void close(void);
    
    /* So should these, there is just a lot of overloading */
    void get(const string& xpath, string& result);
    void get(const string& xpath, int& result);
    void get(const string& xpath, unsigned int& result);
    void get(const string& xpath, short int& result);
    void get(const string& xpath, unsigned short int& result);
    void get(const string& xpath, long int& result);
    void get(const string& xpath, unsigned long int& result);
    void get(const string& xpath, float& result);
    void get(const string& xpath, double& result);
    void get(const string& xpath, bool& result);

    /* So should these, especially if you read the introductory comments */
    void getAttribute(const string& xpath_to_node,
		      const string& attrib_name, 
		      string& result);

    void getAttribute(const string& xpath_to_node,
		      const string& attrib_name, 
		      int& result);

    void getAttribute(const string& xpath_to_node,
		      const string& attrib_name, 
		      unsigned int& result);

    void getAttribute(const string& xpath_to_node,
		      const string& attrib_name, 
		      short int& result);

    void getAttribute(const string& xpath_to_node,
		      const string& attrib_name, 
		      unsigned short int& result);

    void getAttribute(const string& xpath_to_node,
		      const string& attrib_name, 
		      long int& result);

    void getAttribute(const string& xpath_to_node,
		      const string& attrib_name, 
		      unsigned long int& result);

    void getAttribute(const string& xpath_to_node,
		      const string& attrib_name, 
		      float& result);

    void getAttribute(const string& xpath_to_node,
		      const string& attrib_name, 
		      double& result);

    void getAttribute(const string& xpath_to_node,
		      const string& attrib_name, 
		      bool& result);

    int count(const string& xpath);

    void evaluateXPath(const string& xpath);
    void printQueryResult(ostream& os);

    void setCurrentXPath(const string& xpath);
    xmlNodePtr getCurrentContextNode(void);
    void setCurrentContextNode(xmlNodePtr new_context_node);

    //! Print the entire contents as a stream
    void print(ostream& os);

    //! Print the root element as a stream
    void printRoot(ostream& os);

    //! Print an element selected by XPath
    void printXPathNode(ostream& os, const string& xpath_to_node);

  private:

    /* Stuff needed by libxml */
    xmlDocPtr doc;
    xmlNodePtr cur;
    xmlNodePtr root_node;
    xmlXPathContextPtr xpath_context;
    xmlXPathObjectPtr  query_result;


    /* Init function for the xpath system */
    void setupXPath(void);

    /* Recursively trawl the document for namespace definitions
     * and register them in the XPath context */
    void snarfNamespaces(xmlNodePtr current_node, 
			 xmlXPathContextPtr xpath_context);
    
    /* Ensure that the query returned something non-null */
    void checkQuery(const string& xpath);

    /* Ensure that the query has returned something non-null, 
       and that the query result consists of a unique node with
       simple content */
    void checkQueryPrimitive(const string& xpath);

    /* Get a string from a path expression that matches checkQueryPrimitive.
     * this string is copied into result, after which the query result
     * is freed (hopefully properly).
     */
    void getPrimitiveString(const string& xpath, string& result);

    /* Get the attribute string from a query, that satisfies 
       checkQuery, and is a unique element node ( but doesn't have to have 
       simple content). It copies the attribute into result after which 
       the query result is freed (hopefully properly).
    */
    void getAttributeString(const string& xpath_to_node,
			    const string& attrib_name,
			    string& result);

    /* Once you snarfed your data out of s, call this function to check
       there are no further whitespaces (ie another string) following 
       -- inelegant, but I don't know an elegant way without casting 
       -- doubles etc. */
    bool findNonWhitespace(istringstream& s);

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
      getPrimitive(const string& xpath,
		   T& result,
		   const string& ptype);

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
			    const string& ptype);


    /* Generic function to print a node to an ostream */
    void printNode(ostream& os, xmlNodePtr node);
  };

};
#endif  
  
