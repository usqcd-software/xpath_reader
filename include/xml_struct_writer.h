#ifndef XML_STRUCT_WRITER_H
#define XML_STRUCT_WRITER_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include <array_type.h>

#define INDENT_SPACES 2


using namespace std;

namespace XMLStructWriterAPI { 

  class XMLFileArrayWriter;

  // Base class for struct writers
  class XMLStructWriterBase {
  public:
    XMLStructWriterBase(const string& tagname, int _indent_level=0) 
    { 
      indent_level = _indent_level;
      qname=tagname;
    }

    void writeSimple(const string& tagname, const int& value) {
      writeSimpleTag(tagname, value);
    }

    void writeSimple(const string& tagname, const float& value) {
      writeSimpleTag(tagname, value);
    }
    
    void writeSimple(const string& tagname, const string& value) {
      writeSimpleTag(tagname, value);
    }

    void writeSimple(const string& tagname, const bool& value) {
      writeSimpleTag(tagname, value);
    }  

    template <typename T>
      void writeSimpleTag(const string& tagname, T& value)
      {
	ostream &os=getOstream();
	string indent((indent_level+1)*INDENT_SPACES, ' ');
	os << endl << indent << "<" << tagname << ">";
	os << boolalpha <<  value << "</" << tagname << ">";
      }  

  protected:
    
    virtual std::ostream& getOstream() = 0;

    void writePrologue(ostream& os) const {
      os << "<?xml version=\"1.0\"?>" << endl;
      os << "<!-- Written by XMLSimpleWriter class by Balint Joo -->";
      os << endl;
      os.flush();
    }

    bool simple_array;
    int indent_level;
    string qname;
    int elems_written;
  };

  class XMLFileStructWriter : public XMLStructWriterBase {
  public:
    XMLFileStructWriter(ofstream& _os, 
			const string& tagname, 
			int indent_level=0, 
			bool write_prologue=false) : 
      XMLStructWriterBase(tagname, indent_level), output_stream(_os) {
      
      if ( write_prologue == true ) { 
	writePrologue(_os);
      }

      string indent(indent_level*INDENT_SPACES, ' ');
      output_stream  << endl << indent << "<" << qname << ">";

    }

    ~XMLFileStructWriter(void) {
      string indent(indent_level*INDENT_SPACES, ' ');
      output_stream  << endl << indent << "</" << qname << ">";
    }

    XMLFileStructWriter* structChild(const string& tagname) {
      XMLFileStructWriter* child=new XMLFileStructWriter(output_stream, tagname, indent_level+1, false);
      return child;
    }

    
    XMLFileArrayWriter* arrayChild(const string& tagname, const string& elem_name, ArrayType t);
    
  private:
    ofstream& output_stream;
    std::ostream& getOstream(void) { 
      return output_stream;
    }
    
  };

  class XMLBufferStructWriter : public XMLStructWriterBase {
  public:
    XMLBufferStructWriter(ostringstream& _os, 
			  const string& tagname, 
			  int indent_level=0, 
			  bool write_prologue=false) : 
      XMLStructWriterBase(tagname, indent_level), output_stream(_os) {
      
      if ( write_prologue == true ) { 
	writePrologue(_os);
      }

      string indent(indent_level*INDENT_SPACES, ' ');
      output_stream  << endl << indent << "<" << qname << ">";

    }

    ~XMLBufferStructWriter(void) {
      string indent(indent_level*INDENT_SPACES, ' ');
      output_stream  << endl << indent << "</" << qname << ">";
    }

    XMLBufferStructWriter* structChild(const string& tagname) {
      XMLBufferStructWriter* child=new XMLBufferStructWriter(output_stream, tagname, indent_level+1, false);
      return child;
    }
  
  private:
    ostringstream& output_stream;
    std::ostream& getOstream(void) { 
      return output_stream;
    }
    
  };

};



#endif
