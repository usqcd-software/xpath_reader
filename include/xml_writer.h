#ifndef XML_WRITER_H
#define XML_WRITER_H

#include <attribute.h>
#include <tcomplex.h>
#include <array.h>
#include <ostream>
#include <sstream>
#include <fstream>
#include <string>
#include <stack>


#define INDENT_SPACES ((unsigned int)3)

using namespace std;

namespace XMLWriterAPI {

  // Base class for the XMLWriter classes
  class XMLWriterBase {
  public:
    XMLWriterBase(void) {
      doctag_written = false;
      primitive_last = false;
    }

    ~XMLWriterBase(void) { 
      while ( namestack.empty() == false ) {
	closeTag();
      }
    }

    void openTag(const string& tagname);
    void openTag(const string& nsprefix, const string& tagname);
    void openTag(const string& tagname, AttributeList& al);

    void openTag(const string& nsprefix,
		 const string& tagname, 
		 AttributeList& al);

    void closeTag();

    void emptyTag(const string& tagname);
    void emptyTag(const string& nsprefix, const string& tagname);
    void emptyTag(const string& tagname, AttributeList& al);

    void emptyTag(const string& nsprefix,
		  const string& tagname, 
		  AttributeList& al);
    

   

    // Overloaded Writer Functions
    void write(const string& output);
    void write(const int& output);
    void write(const unsigned int& output);
    void write(const short int& output);
    void write(const unsigned short int& output);
    void write(const long int& output);
    void write(const unsigned long int& output);
    void write(const float& output);
    void write(const double& output);
    void write(const bool& output);


    template <typename T>
    void write(TComplex<T>& output) {
      openTag((string)"cmpx");
      openTag((string)"re");
      write(output.real());
      closeTag();
      openTag("im");
      write(output.imag());
      closeTag();
      closeTag();
    }

    template <typename T> 
      void write(const string& sizeName, 
		 const string& elemName, 
		 const string& indexName,
		 const unsigned int& indexStart,
		 Array<T>& a) {
      
      AttributeList alist;
      alist.push_back(Attribute((string)"sizeName",  sizeName));
      alist.push_back(Attribute((string)"elemName",  elemName));
      alist.push_back(Attribute((string)"indexName", indexName));
      alist.push_back(Attribute((string)"indexStart", indexStart));
      
      // Write the array - tag
      openTag((string)"array", alist);

      openTag(sizeName);
      write(a.size());
      closeTag();

      unsigned int index;
      for(index=0; index < a.size(); index++) {
	alist.clear();
	alist.push_back(Attribute(indexName, index + indexStart));
	openTag(elemName, alist);
	write(a[index]);
	closeTag();
      }

      closeTag(); // Array
    }

    template < typename T > 
    void write(Array<T>& a) {
      write((string)"size",
	    (string)"element",
	    (string)"index",
	    0, a);
    }

  protected:
    // Protected functions -- for overloading access by derived classes
    
    // Get the internal ostream
    virtual ostream& getOstream(void) = 0;

    // Write the prologue
    void writePrologue(ostream& os);
    unsigned int indent_level;
  private:
    
    // Check we have written a document tag...
    // apparently we need this before writing anything else
    bool doctag_written;
    bool primitive_last;

    // A stack to hold names.
    stack<string> namestack;

    // The universal tag-write function. Copes with empty tags too
    // all the openTag, emptyTag functions call this basically
     void dumpTag(const string& nsprefix,
		  const string& tagname, 
		  AttributeList& al,
		  bool is_empty=false);
     
     // The universal data-write. All the write functions call this
     template< typename T>
       void
       writePrimitive(const T& output);
  };

  // A Class to write XML to a string
  // Supplies an ostringstream as the outstream for the BaseClass
  class XMLSimpleStringWriter : public XMLWriterBase {
  public:

    // Constructor
    XMLSimpleStringWriter(bool write_prologue=true) { 
      if( write_prologue ) { 
	writePrologue(output_stream);
      }
      indent_level=0;
    }  
  
    // Get the string -- not the stream...
    string str(void) const { 
      return output_stream.str();
    }
        
  private:

    // The output stream...
    ostringstream output_stream;

    // The function that supplies the stream to the parent...
    ostream& getOstream(void) { 
      return output_stream;
    }
  };

 
  class XMLSimpleFileWriter : public XMLWriterBase {
  public:
    XMLSimpleFileWriter(const string& filename, bool write_prologue=true) {
    
      output_stream.open(filename.c_str(), ofstream::out);
      if( write_prologue ) {
	writePrologue(output_stream);
      }
      indent_level=0;
    }
        
    void close() 
      {
	output_stream.close();
      }

    ~XMLSimpleFileWriter(void) {
      output_stream.close();
    }

  private:
    ofstream output_stream;
    ostream& getOstream(void) { 
      return output_stream;
    }
  };
  
};

#endif



