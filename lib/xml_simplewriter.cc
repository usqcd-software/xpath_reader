
#include <xml_simplewriter.h>
#include <sstream>


#define INDENT_SPACES ((unsigned int)2)

using namespace std;
using namespace XMLWriterAPI;

void XMLSimpleWriter::openTag(const string& tagname)
{
  string empty_string;
  AttributeList empty_list;

  dumpTag(empty_string, tagname, empty_list, false);
}

void XMLSimpleWriter::openTag(const string& nsprefix, const string& tagname)
{
  AttributeList empty_list;

  dumpTag(nsprefix, tagname, empty_list, false);
}

void XMLSimpleWriter::openTag(const string& tagname, AttributeList& al)
{
  string empty_string;
  dumpTag(empty_string, tagname, al, false);
}

void XMLSimpleWriter::openTag(const string& nsprefix, 
			    const string& tagname, 
			    AttributeList& al)
{
  dumpTag(nsprefix, tagname, al, false);
}

void XMLSimpleWriter::emptyTag(const string& tagname)
{
  string empty_string;
  AttributeList empty_list;

  dumpTag(empty_string, tagname, empty_list, true);
}

void XMLSimpleWriter::emptyTag(const string& tagname,  AttributeList& al)
{
  string empty_string;
  dumpTag(empty_string, tagname, al, true);
}

void XMLSimpleWriter::emptyTag(const string& nsprefix, 
			     const string& tagname, 
			     AttributeList& al)
{
  dumpTag(nsprefix, tagname, al, true);
}


void XMLSimpleWriter::dumpTag(const string& nsprefix, 
			    const string& tagname, 
			    AttributeList& al,
			    bool is_empty)
{
  string qualified_tagname;
  ostream& os=getOstream();

  // Check whether we are trying to write a second 
  // root element. I thought this was allowed but apparently not
  if( doctag_written == true && namestack.empty() ) {
    ostringstream error_message;
    error_message << "Attempt to write second root tag -- this is not XML compliant" << endl;
    throw error_message.str();
  }

  if( nsprefix.size() == 0 ) { 
    qualified_tagname = tagname;
  }
  else { 
    qualified_tagname = nsprefix + ":" + tagname;
  }
  
  string indent(indent_level*INDENT_SPACES, ' ');
  os << endl <<  indent << "<" << qualified_tagname;
  
  list<Attribute>::iterator the_iterator;
  for(the_iterator = al.begin(); the_iterator != al.end(); the_iterator++) {
    if( (*the_iterator).isEmpty() == false ) {
      os << "  " << the_iterator->getName() << "=\"" << the_iterator->getValue()
	 << "\"";
    }
  }
  
  if(is_empty == true) { 
    os << "/>";
  }
  else {
    os << ">" ;
    namestack.push(qualified_tagname);
    indent_level++;
  }
  
  os.flush();

  if( doctag_written == false ) { 
    doctag_written = true; 
  }

  primitive_last = false;
}
  
void XMLSimpleWriter::closeTag(void)
{
  string qualified_tagname;
  ostream& os=getOstream();

  if( namestack.empty() == false ) { 
    qualified_tagname = namestack.top();
    namestack.pop();
    indent_level--;

    if(primitive_last == false) {
      string indent(indent_level*INDENT_SPACES, ' ');
      os << endl << indent;
    }
    os << "</" << qualified_tagname << ">" ;
  }
  else {
    ostringstream error_message;
    error_message << "Attempt to close non existent tag";
    throw error_message.str();
  }

  primitive_last = false;

}


void 
XMLSimpleWriter::write(const string& output)
{
  writePrimitive<string>(output);
}

void
XMLSimpleWriter::write(const int& output) 
{
  writePrimitive<int>(output);
}

void
XMLSimpleWriter::write(const unsigned int& output)
{
  writePrimitive<unsigned int>(output);
}

void
XMLSimpleWriter::write(const short int& output)
{
  writePrimitive<short int>(output);
}


void 
XMLSimpleWriter::write(const unsigned short int& output)
{
  writePrimitive<unsigned short int>(output);
}

void
XMLSimpleWriter::write(const long int& output)
{
  writePrimitive<long int>(output);
}

void 
XMLSimpleWriter::write(const unsigned long int& output)
{
  writePrimitive<unsigned long int>(output);
}

void 
XMLSimpleWriter::write(const float& output)
{
  writePrimitive<float>(output);
}

void 
XMLSimpleWriter::write(const double& output)
{
  writePrimitive<double>(output);
}

void
XMLSimpleWriter::write(const bool& output)
{
  writePrimitive<bool>(output);
}


template < typename T > 
void
XMLSimpleWriter::writePrimitive(const T& output)
{
  ostream& os=getOstream();

  if( ! namestack.empty() ) { 

    os << boolalpha << output;
  }
  else { 
    ostringstream error_string;
    error_string << "Attempt to write before opening root tag" << endl;
    throw error_string.str();
  }

  primitive_last = true;
  os.flush();
}

void 
XMLSimpleWriter::writeXML(const string& output)
{
  ostream& os=getOstream();

  // Should this have more checking, e.g. of primitive_last or doctag?
  os << output << endl;
  os.flush();
}

void 
XMLSimpleWriter::writePrologue(ostream& os)
{
   os << (const string)"<?xml version=\"1.0\"?>" << endl;
   os << (const string)"<!-- Written by XMLSimpleWriter class by Balint Joo -->";
   os << endl;
   os.flush();
}







