#ifndef XML_ARRAY_WRITER_H
#define XML_ARRAY_WRITER_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <array_type.h>



#define INDENT_SPACES 2

using namespace std;

namespace XMLStructWriterAPI { 

  class XMLFileArrayWriter;
  class XMLFileStructWriter;
 
  // Base class for struct writers
  class XMLArrayWriterBase {
  public:
    XMLArrayWriterBase(const string& tagname, 
		       const string& elem_name, 
		       ArrayType atype, 
		       int _indent_level=0) 
    { 
      indent_level = _indent_level;
      qname=tagname;
      elem_qname=elem_name;
      elems_written=0;
      type=atype;
    }

    virtual ~XMLArrayWriterBase() { };

    void elementSimple(const int& value) {
      if (type == SIMPLE_INT) { 
	writeSimpleTag(value);
      }
      else {
	throw "Array member type mismatch error: wanted SIMPLE_INT";
      }
    }

    void elementSimple(const float& value) {
      if (type == SIMPLE_FLOAT) { 
	writeSimpleTag(value);
      }
      else {
	throw "Array member type mismatch error: wanted SIMPLE_FLOAT";
      }
    }

    void elementSimple(const string& value) {
      if (type == SIMPLE_STRING) { 
	writeSimpleTag(value);
      }
      else {
	throw "Array member type mismatch error: wanted SIMPLE_STRING";
      }
    }

    void elementSimple(const bool& value) {
      if (type == SIMPLE_BOOL) { 
	writeSimpleTag(value);
      }
      else {
	throw "Array member type mismatch error: wanted SIMPLE_BOOL";

      }
    }

    template <typename T>
      void writeSimpleTag(T& value)
      {
	ostream &os=getOstream();
	string indent((indent_level+1)*INDENT_SPACES, ' ');
	os << endl << indent << "<" << elem_qname << ">";
	os << boolalpha <<  value << "</" << elem_qname << ">";
	elems_written++;
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
    string elem_qname;
    ArrayType type;
    int elems_written;
  };

  class XMLFileArrayWriter : public XMLArrayWriterBase {
  public:
    XMLFileArrayWriter(ofstream& _os, 
		       const string& tagname, 
		       const string& elem_name,
		       ArrayType atype,
		       int indent_level=0, 
		       bool write_prologue=false) : 
      XMLArrayWriterBase(tagname,elem_name, atype, indent_level), output_stream(_os) {
      
      if ( write_prologue == true ) { 
	writePrologue(_os);
      }
      
      string indent(indent_level*INDENT_SPACES, ' ');
      output_stream  << endl << indent << "<" << qname << ">";
      
    }
    
    ~XMLFileArrayWriter(void) {
      string indent(indent_level*INDENT_SPACES, ' ');
      output_stream  << endl << indent << "</" << qname << ">";
    }
    
    
    XMLFileArrayWriter* elementArray(ArrayType t) {
      XMLFileArrayWriter* child=new(nothrow) XMLFileArrayWriter(
						       output_stream,
						       elem_qname,
						       "element",
						       t,
						       indent_level+1,
						       false);
      if( child == 0x0 ) { 
	std::cerr << " Failed to allocate child " << endl << flush;
	exit(-1);
      }

      elems_written++;
      return child;
    }
    XMLFileArrayWriter* elementArray(const string& elem_name, ArrayType t) {
      XMLFileArrayWriter* child=new(nothrow) XMLFileArrayWriter(
						       output_stream,
						       elem_qname,
						       elem_name,
						       t,
						       indent_level+1,
						       false);
      if( child == 0x0 ) { 
	std::cerr << " Failed to allocate child " << endl << flush;
	exit(-1);
      }

      elems_written++;
      return child;
    }

    XMLFileStructWriter* elementStruct(void); 
    
  private:
    ofstream& output_stream;
    std::ostream& getOstream(void) { 
      return output_stream;
    }
    
  };

  /*
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
      XMLBufferStructWriter* child=new(nothrow) XMLBufferStructWriter(output_stream, tagname, indent_level+1, false);

      if( child == 0x0 ) { 
        std::cerr << "Failed to new child " << endl << flush;
	exit(-1);
      }
      return child;
    }
  
  private:
    ostringstream& output_stream;
    std::ostream& getOstream(void) { 
      return output_stream;
    }
    
  };
  */
};



#endif
