#ifndef XML_SCHEMAWRITER_H
#define XML_SCHEMAWRITER_H

#include <xml_simpleschemawriter.h>
#include <attribute.h>
#include <ostream>
#include <string>
#include <stack>
#include <xml_writer.h>

using namespace std;

namespace XMLWriterAPI {

  // Base class for the XMLSimpleWriter classes
  class XMLSchemaWriter : private XMLSimpleSchemaWriter {
  public:
    XMLSchemaWriter(XMLSimpleWriter& _instance, 
		    XMLSimpleWriter& _schema) : XMLSimpleSchemaWriter(_instance, _schema) { 

    }

    void write(const string& name, const string& s)
    {
      writeSimpleElement(name, s);
    }

    void write(const string& name, const int &i) {
      writeSimpleElement(name, i);
    }

    void write(const string& name, const float &f) { 
      writeSimpleElement(name, f);
    }
    
    void write(const string& name, const double &d) { 
      writeSimpleElement(name, d);
    }

    void write(const string& name, const bool& b) {
      writeSimpleElement(name, b);
    }

						
  };
 
};


#endif



