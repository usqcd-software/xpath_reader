#include <fstream>
#include <ostream>
#include <istream>
#include <string>

#include <array_type.h>
#include <xml_struct_writer.h>
#include <xml_array_writer.h>

using namespace std;

namespace XMLStructWriterAPI {

XMLFileArrayWriter* XMLFileStructWriter::arrayChild(const string& tagname, 
						    const string& elem_name, 
						    ArrayType t)
{
  XMLFileArrayWriter* child = new XMLFileArrayWriter(output_stream, 
		  				    tagname, elem_name, 
						    t, indent_level+1, false);

  return child;
}

XMLFileStructWriter* XMLFileArrayWriter::elementStruct(void) 
{
  XMLFileStructWriter* child=new XMLFileStructWriter(output_stream,
		 				    elem_qname, indent_level+1, false); 
  elems_written++; 
  return child;
}

};
