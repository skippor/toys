#include <iostream>
#include <string>
#include "tinyxml.h"

  using std::string;

  int main()
{
  TiXmlDocument* myDocument = new TiXmlDocument();
  myDocument->LoadFile("Students.xml");
  TiXmlElement* rootElement = myDocument->RootElement();  //Class
  TiXmlElement* studentsElement = rootElement->FirstChildElement();  //Students
  TiXmlElement* studentElement = studentsElement->FirstChildElement();  //Students
  while ( studentElement ) {
    TiXmlAttribute* attributeOfStudent = studentElement->FirstAttribute();  //获得student的name属性
    while ( attributeOfStudent ) {
      std::cout << attributeOfStudent->Name() << " : " << attributeOfStudent->Value() << std::endl;
      attributeOfStudent = attributeOfStudent->Next();
    }
    TiXmlElement* phoneElement = studentElement->FirstChildElement();//获得student的phone元素
    std::cout << "phone" << " : " << phoneElement->GetText() << std::endl;
    TiXmlElement* addressElement = phoneElement->NextSiblingElement();
    std::cout << "address" << " : " << phoneElement->GetText() << std::endl;
    studentElement = studentElement->NextSiblingElement();
  }
  return 0;
}
