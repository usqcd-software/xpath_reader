
#include <xml_writer.h>
#include <sstream>

using namespace std;
using namespace XMLWriterAPI;

void XMLWriterBase::openTag(const string& tagname)
{
  string empty_string;
  AttributeList empty_list;

  dumpTag(empty_string, tagname, empty_list, false);
}

void XMLWriterBase::openTag(const string& nsprefix, const string& tagname)
{
  AttributeList empty_list;

  dumpTag(nsprefix, tagname, empty_list, false);
}

void XMLWriterBase::openTag(const string& tagname, AttributeList& al)
{
  string empty_string;
  dumpTag(empty_string, tagname, al, false);
}

void XMLWriterBase::openTag(const string& nsprefix, 
			    const string& tagname, 
			    AttributeList& al)
{
  dumpTag(nsprefix, tagname, al, false);
}

void XMLWriterBase::emptyTag(const string& tagname)
{
  string empty_string;
  AttributeList empty_list;

  dumpTag(empty_string, tagname, empty_list, true);
}

void XMLWriterBase::emptyTag(const string& tagname,  AttributeList& al)
{
  string empty_string;
  dumpTag(empty_string, tagname, al, true);
}

void XMLWriterBase::emptyTag(const string& nsprefix, 
			     const string& tagname, 
			     AttributeList& al)
{
  dumpTag(nsprefix, tagname, al, true);
}


void XMLWriterBase::dumpTag(const string& nsprefix, 
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
  
  string indent(indent_level*4, ' ');
  os << indent << "<" << qualified_tagname;
  
  list<Attribute>::iterator the_iterator;
  for(the_iterator = al.begin(); the_iterator != al.end(); the_iterator++) {
    if( (*the_iterator).isEmpty() == false ) {
      os << "  " << the_iterator->getName() << "=\"" << the_iterator->getValue()
	 << "\"";
    }
  }
  
  if(is_empty == true) { 
    os << "/>" << endl;
  }
  else {
    os << ">" << endl;
    namestack.push(qualified_tagname);
    indent_level++;
  }
  
  os.flush();

  if( doctag_written == false ) { 
    doctag_written = true; 
  }

  

}
  
void XMLWriterBase::closeTag(void)
{
  string qualified_tagname;
  ostream& os=getOstream();

  if( namestack.empty() == false ) { 
    qualified_tagname = namestack.top();
    namestack.pop();
    indent_level--;
    string indent(indent_level*4, ' ');
   
    os << indent << "</" << qualified_tagname << ">" <<endl;
  }
  else {
    ostringstream error_message;
    error_message << "Attempt to close non existent tag";
    throw error_message.str();
  }


}


void 
XMLWriterBase::write(const string& output)
{
  writePrimitive<string>(output);
}

void
XMLWriterBase::write(const int& output) 
{
  writePrimitive<int>(output);
}

void
XMLWriterBase::write(const unsigned int& output)
{
  writePrimitive<unsigned int>(output);
}

void
XMLWriterBase::write(const short int& output)
{
  writePrimitive<short int>(output);
}


void 
XMLWriterBase::write(const unsigned short int& output)
{
  writePrimitive<unsigned short int>(output);
}

void
XMLWriterBase::write(const long int& output)
{
  writePrimitive<long int>(output);
}

void 
XMLWriterBase::write(const unsigned long int& output)
{
  writePrimitive<unsigned long int>(output);
}

void 
XMLWriterBase::write(const float& output)
{
  writePrimitive<float>(output);
}

void 
XMLWriterBase::write(const double& output)
{
  writePrimitive<double>(output);
}

void
XMLWriterBase::write(const bool& output)
{
  writePrimitive<bool>(output);
}


template < typename T > 
void
XMLWriterBase::writePrimitive(const T& output)
{
  ostream& os=getOstream();
  string indent(indent_level*4, ' ');
  if( ! namestack.empty() ) { 
    os << indent << boolalpha << output << endl ;
    // os << boolalpha << output;
  }
  else { 
    ostringstream error_string;
    error_string << "Attempt to write before opening root tag" << endl;
    throw error_string.str();
  }

  os.flush();
}

void 
XMLWriterBase::writePrologue(ostream& os)
{
   os << (const string)"<?xml version=\"1.0\"?>" << endl;
   os << (const string)"<!-- Written by XMLWriterBase class by Balint Joo -->" << endl;
   os.flush();
}







