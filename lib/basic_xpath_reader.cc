/* ID: $Id: basic_xpath_reader.cc,v 1.15 2003-09-09 20:29:49 bjoo Exp $
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

