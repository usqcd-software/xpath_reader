#include <xml_writer.h>
#include <xml_schemawriter.h>

#include <xpath_reader.h>
#include <stdlib.h>
#include <iostream>
#include <string>
using namespace XMLWriterAPI;
using namespace XMLXPathReader;
using namespace std; 

int main(int argc, char *argv[]) 
{

  XMLSimpleFileWriter schema("testout.xsd");
  XMLSimpleFileWriter instance("testout.xml");

  XMLFileSchemaWriter writer(instance,schema);

  writer.openComplexElement("jim");
  writer.writeSimpleElement("foostring", (string)"Hello Schema World!");
  writer.writeSimpleElement("jive", (string)"Goodbye Cruel World!");
  writer.writeSimpleElement("booltest", true);
  writer.writeSimpleElement("inttest", (int)5);

  /* Castor doesn't support unsigneds 
   *
   * writer.writeSimpleElement("uintTest", (unsigned int)6);
   */
  writer.writeSimpleElement("floattest", (float)5.0432);

  /* writer.openComplexElement("arraythingie");
     writer.writeSimpleElement("foo", 1);
     writer.writeSimpleElement("foo", 2);
     writer.writeSimpleElement("foo", 3);
     writer.closeComplexElement();
  */
  writer.closeComplexElement();



  return(EXIT_SUCCESS);
}

