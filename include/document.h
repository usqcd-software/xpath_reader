#ifndef XML_DOCUMENT_H
#define XML_DOCUMENT_H

#include <libxml/parser.h>
#include <istream>
#include <string>

namespace XMLXPathReader {

  class XMLDocument {
    friend class BasicXPathReader;
  public:
                                                                                
    XMLDocument(const std::string& filename);
    XMLDocument(std::istream& is);

  protected:
    void increaseRefcount();
    void decreaseRefcount();
                                                                                
    int getRefcount() {
      return refcount;
    }
                                                                                
    xmlDocPtr getDoc(void) {
      return doc;
    }
                                                                                
  private:
    int refcount;
    xmlDocPtr doc;
  };

}

#endif
