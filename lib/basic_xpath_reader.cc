/* ID: $Id: basic_xpath_reader.cc,v 1.11 2003-09-08 15:45:08 bjoo Exp $
 *
 * File: basic_xpath_reader.cc
 * 
 * Purpose: All the nasty libxml stuff should be hidden in here 
 * as well as a lot of the overloading/wrappint in BasicXPathReader
 */

#include <basic_xpath_reader.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <ios>
#include <iostream>
#include <sstream>

/* need this for iostream and string */
using namespace std;

/* This is our namespace */
using namespace XMLXPathReader;

// Your basic empty reader 
BasicXPathReader::BasicXPathReader(void)
{
  docref = 0x0;
  xpath_context = NULL;
  query_result = NULL;
}


BasicXPathReader::BasicXPathReader(istream& is)
{
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

BasicXPathReader::BasicXPathReader(const string& filename)
{
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


void BasicXPathReader::open(const string& filename)
{
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
    
    docref = new XMLDocument(filename);

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
    snarfNamespaces(xmlDocGetRootElement(doc), xpath_context);
  }
  catch ( const string& e ) { 
    throw;
  }
}


void BasicXPathReader::open(istream& is)
{
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
    
    docref = new XMLDocument(is);

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
    snarfNamespaces(xmlDocGetRootElement(doc), xpath_context);
  }
  catch ( const string& e ) { 
    throw;
  }
}

void BasicXPathReader::close(void) 
{

  if( docref != 0x0 ) { 
    // Reader is already open. 
    // We detach from the current document:

    // decrement its refcount
    docref->decreaseRefcount();

    // if decrementing the refcount means there are no more references
    // then delete the object -- this ought to call the destructor
    if ( docref->getRefcount() == 0 ) { 
      cout << "Reader: docrefs refcount reached 0. Deleting" << endl;
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

  }
  else {
    // Else we are not open so closing will do sod all
  }

}


BasicXPathReader::~BasicXPathReader(void)
{

  if( docref != 0x0) { 
    close();
  }

}

BasicXPathReader::BasicXPathReader(BasicXPathReader& old, const string& xpath)
{

  if( old.docref != 0x0) { 

    docref = old.docref;
   
    query_result = (xmlXPathObjectPtr) NULL;
    xmlDocPtr doc = docref->getDoc();
    xpath_context = xmlXPathNewContext(doc);
    snarfNamespaces(xmlDocGetRootElement(doc), xpath_context);

    /* Now execute the xpath query and set the context node */
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

    /* Check that the node set contains only 1 element */
    if( old.query_result->nodesetval->nodeNr != 1 ) {
      error_message << "XPath Query: " << xpath << " did not return unique node."
		    << " nodes returned = " << old.query_result->nodesetval->nodeNr 
		    << endl;
      throw error_message.str();
    }
    
  /* Check that the node returned is an element */
    xpath_context->node = old.query_result->nodesetval->nodeTab[0];
    docref->increaseRefcount();
  }
  else { 
    throw "Attempting to clone a closed Reader";
  }

}


/*! get for string. -- See header file for specifics */
void 
BasicXPathReader::get(const string& xpath, string& result)
{
  /* Try and get the string for the xpath */
  try {
    getPrimitiveString(xpath, result);
  }
  catch(const string& e) { 
    /* Pass up exception if it occurs */
    throw e;
  }
}

/*! get for int */
void
BasicXPathReader::get(const string& xpath, int& result)
{
  getPrimitive<int>(xpath, result, "int");
}

/*! get for unsigned int */
void 
BasicXPathReader::get(const string& xpath, unsigned int& result)
{
  getPrimitive<unsigned int>(xpath, result, "unsigned int");
}

/*! get for short */
void 
BasicXPathReader::get(const string& xpath, short int& result)
{
  getPrimitive<short int>(xpath, result,"short int");
}

/*! get for unsigned short */
void
BasicXPathReader::get(const string& xpath, unsigned short int& result)
{
  getPrimitive<unsigned short int>(xpath, result, "unsigned short int");
}

/*! get for long */
void
BasicXPathReader::get(const string& xpath, long int& result)
{
  getPrimitive<long int>(xpath, result, "long int");
}

/*! get for unsigned long */
void
BasicXPathReader::get(const string& xpath, unsigned long int& result)
{
  getPrimitive<unsigned long int>(xpath, result, "unsigned long int");
}

/*! get for float */
void
BasicXPathReader::get(const string& xpath, float& result)
{
  getPrimitive<float>(xpath, result, "float");
}

/*! get for double */
void
BasicXPathReader::get(const string& xpath, double& result)
{
  getPrimitive<double>(xpath, result, "double");
}

/*! get for bool */
void
BasicXPathReader::get(const string& xpath, bool& result)
{
  getPrimitive<bool>(xpath, result, "bool");
}

/*! get attribute for string */
void 
BasicXPathReader::getAttribute(const string& xpath_to_node,
			       const string& attrib_name,
			       string& result)
{
  getAttributeString(xpath_to_node, attrib_name, result);
}

/*! get attribute for int */
void 
BasicXPathReader::getAttribute(const string& xpath_to_node,
			       const string& attrib_name, 
			       int& result)
{
  getPrimitiveAttribute<int>(xpath_to_node, attrib_name, result, "int");
}

/*! get attribute for unsigned */
void 
BasicXPathReader::getAttribute(const string& xpath_to_node,
			       const string& attrib_name, 
			       unsigned int& result)
{
  getPrimitiveAttribute<unsigned int>(xpath_to_node, attrib_name, result, 
				     "unsigned int");
}

/* get attribute for short */
void 
BasicXPathReader::getAttribute(const string& xpath_to_node,
			       const string& attrib_name, 
			       short int& result)
{
  getPrimitiveAttribute<short int>(xpath_to_node, attrib_name, result, 
				  "short int");
}

/* get attribute for unsigned short */
void
BasicXPathReader::getAttribute(const string& xpath_to_node,
			       const string& attrib_name, 
			       unsigned short int& result)
{
   getPrimitiveAttribute<unsigned short int>(xpath_to_node, 
					    attrib_name, result, 
					    "unsigned short int");
}

/* get attribute for long */
void
BasicXPathReader::getAttribute(const string& xpath_to_node,
			       const string& attrib_name, 
			       long int& result)
{
   getPrimitiveAttribute<long int>(xpath_to_node, attrib_name, result, 
				   "long int");
}

/* get attribute for unsigned long */
void 
BasicXPathReader::getAttribute(const string& xpath_to_node,
			       const string& attrib_name, 
			       unsigned long int& result)
{
  getPrimitiveAttribute<unsigned long int>(xpath_to_node, attrib_name, result, 
					  "unsigned long int");
}

/* get attribute for float */
void
BasicXPathReader::getAttribute(const string& xpath_to_node,
			       const string& attrib_name, 
			       float& result)
{
  getPrimitiveAttribute<float>(xpath_to_node, attrib_name, result, 
			      "float");
}

/* get attribute for double */
void 
BasicXPathReader::getAttribute(const string& xpath_to_node,
			       const string& attrib_name, 
			       double& result)
{
 getPrimitiveAttribute<double>(xpath_to_node, attrib_name, result, 
				  "double");
}

/*! get attriute for bool */
void 
BasicXPathReader::getAttribute(const string& xpath_to_node,
			       const string& attrib_name, 
			       bool& result)
{
  getPrimitiveAttribute<bool>(xpath_to_node, attrib_name, result, 
				  "bool");
}


/*! count elements returned by xpath */
int
BasicXPathReader::count(const string& xpath)
{
  ostringstream error_message;
  int ret_val;
  
  // Construct query */
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

/*! basic routine for getting the string value of a desired attribute */
void 
BasicXPathReader::getAttributeString(const string& xpath_to_node,
				     const string& attrib_name,
				     string& result)

{
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


/* dump the entire tree out as stream */
void
BasicXPathReader::print(ostream& os)
{

  if( docref != 0x0 ) {
    xmlChar *buffer=(xmlChar *)NULL;
    int buflen;
    ostringstream error_stream;
    xmlDocPtr doc = docref->getDoc();
    
    if( doc != NULL ) { 
      xmlDocDumpFormatMemory(doc, &buffer, &buflen, 1);
      if( buffer ==(xmlChar *)NULL ) { 
	error_stream << "xmlDocDumpMemory produced NULL XML-char";
	throw error_stream.str();      
      }
      buffer[buflen]='\0';
      os << buffer << endl;
      xmlFree(buffer);
    }
  }
}      


void 
BasicXPathReader::printRoot(ostream& os)
{
  if( docref != 0x0 ) { 
    xmlDocPtr doc = docref->getDoc();
    printNode(os,xmlDocGetRootElement(doc) );
  }
}

void 
BasicXPathReader::printXPathNode(ostream& os, const string& xpath_to_node)
{
  ostringstream error_message;

  // Evaluate the Xpath to the node
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

  /* Check that the node returned is an element */
  xmlNodePtr res_node = query_result->nodesetval->nodeTab[0];
  
  if( res_node == (xmlNodePtr)NULL ) { 
    error_message << "XPath Query: " << xpath_to_node << " claims to have returned a single node, but nodeTab[0] is NULL" << endl;
    throw error_message.str();
  }

  if ( res_node->type != XML_ELEMENT_NODE ) {
    error_message << "XPath Query: " << xpath_to_node << " returned a non-element node"
                  << endl;
    throw error_message.str();
  }
  
  // print it 
  printNode(os, res_node);
  
  // clean up
  if ( query_result != NULL ) { 
    xmlXPathFreeObject(query_result);
    query_result = NULL;
  }    
}

void
BasicXPathReader::printNode(ostream& os, xmlNodePtr node)
{

  if( docref != 0x0 ) { 
    xmlDocPtr doc = docref->getDoc();
    xmlBufferPtr xmlBuf;
    ostringstream error_message;
    
    xmlBuf = xmlBufferCreate();
    if ( xmlBuf == (xmlBufferPtr)NULL ) { 
      error_message << "Failed to create buffer in printNode" << endl;
      throw error_message.str();
    }
    
#if 0
    // RGE: This was the original: updated below
    int size;
    size = xmlNodeDump(xmlBuf, doc, node, 2, 1);
    
    if ( size == -1 ) { 
      error_message << "xmlNodeDump failed most heinously" << endl;
      throw error_message.str();
    }
    
    os.write((char *)xmlBufferContent(xmlBuf), size);
#else
    xmlNodeDump(xmlBuf, doc, node, 2, 1);
    
    os << string((char *)xmlBufferContent(xmlBuf));
#endif
    
    xmlBufferFree(xmlBuf);
  }
}



/*! evaluate an xpath expression. */
void
BasicXPathReader::evaluateXPath(const string& xpath)
{

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

/*! check a query to make sure it returned a non-empty nodeset */
void 
BasicXPathReader::checkQuery(const string& xpath)
{
  ostringstream error_message;
  /* Check that the result of the XPath is a node set */
  /* Note: don't have to worry about query_result being NULL,
     that was already checked in evaluate XPath */
  if( query_result != NULL ) { 

    if( query_result->type != XPATH_NODESET ) {
      error_message << "XPath Query: " << xpath << " didn't return a node_set" 
		    << endl;
      throw error_message.str();
    }
    
    /* Check that the nodesetval is not NULL */
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

/*! check a query to make sure it returned a unique element
 *  with simple content */
void
BasicXPathReader::checkQueryPrimitive(const string& xpath)
{
  ostringstream error_message;

  /* first check that the query result is a nonnempty node set */
  try { 
    checkQuery(xpath);
  } catch(const string& e) {
    throw e;
  }

  /* Check that the node set contains only 1 element */
  if( query_result->nodesetval->nodeNr != 1 ) {
    error_message << "XPath Query: " << xpath << " did not return unique node."
		  << " nodes returned = " << query_result->nodesetval->nodeNr 
		  << endl;
    throw error_message.str();
  }

  /* Check that the node returned is an element */
  xmlNodePtr res_node = query_result->nodesetval->nodeTab[0];
  
  if ( res_node->type != XML_ELEMENT_NODE ) {
    error_message << "XPath Query: " << xpath << " returned a non-element node"
                  << endl;
    throw error_message.str();
  }

  /* Check that the element returned is simple (ie contains no more elements)*/
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


/*! Basic function to get a string from an xpath query */
void 
BasicXPathReader::getPrimitiveString(const string& xpath, string& result)
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

/*! the main templated routine to get back primitve type. 
 *  the string ptype is the name of the type and is for error reporting 
 */
template <class T>
void 
BasicXPathReader::getPrimitive(const string& xpath, 
			       T& result, 
			       const string& ptype)
{
  string pstring;

  /* Get the string value of the query */
  ostringstream error_message;
  try {
    getPrimitiveString(xpath, pstring);
  }
  catch(const string& e) { 
    throw e;
  }

  /* Make an istringstream from the string value */
  istringstream is(pstring);
  ios::iostate state;

  /* Save the exception mask of the istringstream */
  state = is.exceptions();

  /* Force the istringstream to throw an exception if it fails 
   * (to convert the string to our type) */
  is.exceptions(ios::failbit);
  
  /* Try to read the type from the istringstream. 
     bool-s should be "true" or "false" stirngs */
  try { 
    is >> boolalpha >> result;
  }
  catch(istringstream::failure e) { 
    error_message << "XPath Query: " << xpath 
		  << ": Error: Failed to convert query result string: " 
		  << pstring << " to " << ptype;
    throw error_message.str();
  }

  /* Turn off exceptions on failure */
  is.exceptions(state);

  /* Look for non whitespaces in whats left in the stream (ie in what's left
     from the string */
  if( findNonWhitespace(is) ) {
    error_message << "XPath Query: " << xpath << ": Error: query result "  << pstring << " does not contain a single " << ptype;
    throw error_message.str();
  }
}

/*! Main routine to get attributes and convert them into other types */
template< typename T > 
void
BasicXPathReader::getPrimitiveAttribute(const string& xpath_to_node,
					const string& attrib_name,
					T& result,
					const string& ptype)
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

/*! Dumb routine for looking for non-whitespaces */
bool
BasicXPathReader::findNonWhitespace(istringstream& s)
{
  char search;

  /* While the stream is not empty */
  while(!s.eof()) { 
    
    /* getchar */
    s.get(search);

    /* if its not a space return true -- (ie found non-whitespace) */
    if( !isspace(search) ) { 
      return true;
    }
  }

  /* Ok we reached eof without finding a non-whitespace. */
  return false;
}
     
/*! pretty print the query_result object onto an os */
void
BasicXPathReader::printQueryResult(ostream& os)
{
  if( docref != 0x0 ) { 
    int nset;
    int i;
    ostringstream error_message;
    
    if( query_result == NULL ) { 
      os << "Query Result is NULL" << endl;
    }
    else {
      xmlDocPtr doc = docref->getDoc();
      
      /* Print different stuff depending on the query result type */
      switch(query_result->type) {
      case XPATH_UNDEFINED:
	os << "======== QUERY RESULT IS UNDEFINED =========" << endl;
	break;
      case XPATH_NODESET:
	os << "======== QUERY RESULT IS A NODE_SET ========" << endl;
	os << "====== NODE DUMP START ========" << endl;
	os.flush();
	
	/* Dump a node set -- if it is not null */
	if( query_result->nodesetval == NULL  ) { 
	  os << "====== NODE SET IS NULL =======" << endl;
	}
	else {
	  /* Get the number of nodes in the node set */
	  nset = query_result->nodesetval->nodeNr;
	  os << "Nset is " << nset << endl;
	  if( nset > 0 ) { 
	    
	    /* If there is more than one, than go through the nodes
	     * and dump each one */
	    for(i=0; i < nset; i++) { 
	      os.flush();
	      switch(query_result->nodesetval->nodeTab[i]->type) { 
	      case XML_DOCUMENT_NODE:
		// if query was / we get back the whole document
		os << "NodeSet contains Document node" <<endl;
		
		xmlDocDump(stdout,doc );
		break;
	      case XML_ELEMENT_NODE:
		// otherwise use different function to dump
		xmlElemDump(stdout, doc, query_result->nodesetval->nodeTab[i]);
		break;
	      default:
		// We may get back other nodes, but I don't care about them.
		os << "Much Weirdness" << endl;
	      }
	    }
	  }
	}
	fflush(stdout);
	
	os << endl;
	os << "======= NODE DUMP END =========" << endl;
	os.flush();     
	break;
      case XPATH_BOOLEAN:
	os << "======== QUERY RESULT IS A BOOLEAN ========" << endl;
	os << "===== Value is: " << boolalpha << (bool)((query_result->boolval > 0) ? true : false)  << endl;
	break;
      case XPATH_NUMBER:
	os << "======== QUERY RESULT IS A NUMBER =========" << endl;
	os << "===== Value is: " << (query_result->floatval) << endl;
	break;
      case XPATH_STRING:
	os << "======== QUERY RESULT IS A STRING =========" << endl;
	os << "======== Value is: " << (query_result->stringval) <<endl;
	break;
      case XPATH_POINT:
	os << "======== QUERY RESULT IS XPATH_POINT ========" <<endl;
	break;
      case XPATH_RANGE:
	os << "======== QUERY RESULT IS XPATH_RANGE ========" << endl;
	break;
      case XPATH_LOCATIONSET:
	os << "======== QUERY RESULT IS XPATH_LOCATIONSET =====" << endl;
	break;
      case XPATH_USERS:
	os << "======== QUERY RESULT IS XPATH_USERS ==========" << endl;
	break;
      case XPATH_XSLT_TREE:
	os << "======== QUERY RESULT IS XPATH_XSLT_TREE ======" << endl;
	break;
      default:
	os << "======== DONT KNOW WHAT TO DO WITH QUERY_RESULT =======" <<endl;
	break;
      }
    }
  }
  else { 

    // No Document -- Nothing to print.
  }
}      

/*! This contentious piece of code goes through the whole "tree"
 *  and registers every single namespace it finds into the XPath context.
 *  This may or may not be standard compliant as is */
void 
BasicXPathReader::snarfNamespaces(xmlNodePtr current_node,
		     xmlXPathContextPtr xpath_context) {
  
  ostringstream error_message;
  
  if( current_node == (xmlNodePtr)NULL ) {
    /* End of recursion -- we are NULL */
    return;
  }
  else { 
    
    /* Do my own namespaces */
    xmlNsPtr nsdefptr = current_node->nsDef;
    while( nsdefptr != NULL ) { 
      
      // if the namespace prefix is not null it is a non-default
      // namespace -- register it.
      if( nsdefptr->prefix != NULL ) { 
	string prefix((char *)nsdefptr->prefix);
	string href((char *)nsdefptr->href);
#if 0
	cout << "Found Namespace Definition: " << endl;
	cout << "\t Prefix = " << prefix << " URI = " << href;
	cout << "... Registering" << endl; 
#endif
	xmlXPathRegisterNs(xpath_context, 
			   nsdefptr->prefix,
			   nsdefptr->href);
      }
      else { 
	// If the prefix is null, the namespace is 'a' default
	// namespace. Dunno what to do with it...
	string href((char *)nsdefptr->href);
#if 0
	cout << "Found Default Namespace Definition: " << endl;
	cout << "\t URI = " << href;
	cout << " ... Not Registering Default Namespace" << endl;
#endif
      }
      
      nsdefptr=nsdefptr->next;
    }
    
    /* Recurse down my siblings */
    snarfNamespaces(current_node->next, xpath_context);
    
    /* Recurse down my children */
    snarfNamespaces(current_node->children, xpath_context);
  }
}

