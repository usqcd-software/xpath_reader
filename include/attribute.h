#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <string>
#include <list>
using namespace std;

namespace XMLWriterAPI {

  class Attribute {
  public:
    // Construct with empty constructor;
    Attribute(void);

    // Destructor -- members will just nicely 
    // go out of scope and die...
    ~Attribute(void);

      // Construct with initialiser 
    Attribute(const string& _name, const string& _value);
    Attribute(const string& _name, const int& _value);
    Attribute(const string& _name, const unsigned int& _value);
    Attribute(const string& _name, const long int& _value);
    Attribute(const string& _name, const unsigned long int& _value);
    Attribute(const string& _name, const short int& _value);
    Attribute(const string& _name, const unsigned short int& _value);
    Attribute(const string& _name, const float& _value);
    Attribute(const string& _name, const double& _value);
    Attribute(const string& _name, const bool& _value);
    
    Attribute(const string& nsprefix, const string& _name, const string& _value);
    Attribute(const string& nsprefix, const string& _name, const int& _value);
    Attribute(const string& nsprefix, const string& _name, const unsigned int& _value);
    Attribute(const string& nsprefix, const string& _name, const long int& _value);
    Attribute(const string& nsprefix, const string& _name, const unsigned long int& _value);
    Attribute(const string& nsprefix, const string& _name, const short int& _value);
    Attribute(const string& nsprefix, const string& _name, const unsigned short int& _value);
    Attribute(const string& nsprefix, const string& _name, const float& _value);
    Attribute(const string& nsprefix, const string& _name, const double& _value);
    Attribute(const string& nsprefix, const string& _name, const bool& _value);
    // Copy
    Attribute(const Attribute& a);

    // Assign through copy.
    const Attribute& operator=(const Attribute& a);

    // Get at name
    string& getName(void);

    // Get at value
    string& getValue(void);

    // Check that the attribute has both a nonzero
    // name and a nonzero value.
    bool isEmpty(void);

  private:
    string name;
    string value;
    template < class T > void init(const string& _name, const T& _value);
    template < class T > void init(const string& nsprefix, const string& _name, const T& _value);
    void initString(const string& _name, const string& _value);
    void initString(const string& nsprefix, const string& _name, const string& _value);
    
  };

  // Define attribute list...
  typedef list<Attribute> AttributeList;

};
#endif
