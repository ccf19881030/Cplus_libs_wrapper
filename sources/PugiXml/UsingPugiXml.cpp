C++ Example for using PugiXml
## [PugiXml - reading, writing, searching XML data](http://www.gerald-fahrnholz.eu/sw/DocGenerated/HowToUse/html/group___grp_pugi_xml.html)
```cpp
// UsingPugiXml.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "MyTestCommands.h"
#include <pugixml.hpp>
#include <TestToolBox\TestToolBox.h>
#include <boost/test/unit_test.hpp>
namespace TTB = TestToolBox;
std::string const XML_FILE_PATH = "C://MyTemp/PugiXml/MyDemo.xml";
void CreateNewXmlFileWithEmptyRootNode()
{
    /// [create new xml file]
    // Generate new XML document within memory
    pugi::xml_document doc;
    // Alternatively store as shared pointer if tree shall be used for longer
    // time or multiple client calls:
    // std::shared_ptr<pugi::xml_document> spDoc = std::make_shared<pugi::xml_document>();
    // Generate XML declaration
    auto declarationNode = doc.append_child(pugi::node_declaration);
    declarationNode.append_attribute("version")    = "1.0";
    declarationNode.append_attribute("encoding")   = "ISO-8859-1";
    declarationNode.append_attribute("standalone") = "yes";
    // A valid XML doc must contain a single root node of any name
    auto root = doc.append_child("MyRoot");
    // Save XML tree to file.
    // Remark: second optional param is indent string to be used;
    // default indentation is tab character.
   bool saveSucceeded = doc.save_file(XML_FILE_PATH.c_str(), PUGIXML_TEXT("  "));
   assert(saveSucceeded);
   /// [create new xml file]
}
//-----------------------------------------------------------------------------
void AddSomeChildNodesWithVariousAttributeTypesToExistingXmlFile(
    int    in_intVal,
    double in_doubleVal,
    bool   in_boolVal)
{
    /// [load xml file]
    // Create empty XML document within memory
    pugi::xml_document doc;
    // Load XML file into memory
    // Remark: to fully read declaration entries you have to specify
    // "pugi::parse_declaration"
    pugi::xml_parse_result result = doc.load_file(XML_FILE_PATH.c_str(),
        pugi::parse_default|pugi::parse_declaration);
    if (!result)
    {
        std::cout << "Parse error: " << result.description()
            << ", character pos= " << result.offset;
    }
    // A valid XML document must have a single root node
    pugi::xml_node root = doc.document_element();
    /// [load xml file]
    /// [add child elements]
    // Append some child elements below root
    // Add as last element
    pugi::xml_node nodeChild = root.append_child("MyChild");
    nodeChild.append_attribute("hint") = "inserted as last child";
    nodeChild.append_attribute("intVal") = in_intVal;
    // Add as last element
    nodeChild = root.append_child("MyChild");
    nodeChild.append_attribute("hint") = "also inserted as last child";
    nodeChild.append_attribute("doubleVal") = in_doubleVal;
    // Add as first element
    nodeChild = root.prepend_child("MyChild");
    nodeChild.append_attribute("hint") = "inserted at front";
    nodeChild.append_attribute("boolVal") = in_boolVal;
    /// [add child elements]
    // Save XML tree to file.
    bool saveSucceeded = doc.save_file(XML_FILE_PATH.c_str(), PUGIXML_TEXT("  "));
    assert(saveSucceeded);
}
//-----------------------------------------------------------------------------
/// [ToString helper function]
// Helper functions to uniformly convert data types to string representation
template <typename T>
std::string ToString(T const & in_val)
{
    return std::to_string(in_val);
}
// Specialization for boolean type to force "true"/"false"
template<>
std::string ToString(bool const & in_val)
{
    std::ostringstream oss;
    oss << std::boolalpha << in_val;
    return oss.str();
}
/// [ToString helper function]
//-----------------------------------------------------------------------------
void AddSomeChildNodesWithDirectValuesToExistingXmlFile()
{
    // Create empty XML document within  memory
    pugi::xml_document doc;
    // Load XML file into memory
    // Remark: to fully read declaration entries you have to specify
    // "pugi::parse_declaration"
    pugi::xml_parse_result result = doc.load_file(XML_FILE_PATH.c_str(),
        pugi::parse_default|pugi::parse_declaration);
    if (!result)
    {
        std::cout << "Parse error: " << result.description()
            << ", character pos= " << result.offset;
    }
    // A valid XML document must have a single root node
    pugi::xml_node root = doc.document_element();
    /// [add child wiith node value]
    pugi::xml_node childrenWithValues = root.append_child("ChildrenWithValue");
    // Add child of type integer
    pugi::xml_node nodeChild = childrenWithValues.append_child("MyChildWithIntValue");
    nodeChild.append_child(pugi::node_pcdata).set_value(ToString(4712).c_str());
    // Add child of type double
    nodeChild = childrenWithValues.append_child("MyChildWithDoubleValue");
    nodeChild.append_child(pugi::node_pcdata).set_value(ToString(3.18).c_str());
    // Add child of type bool
    nodeChild = childrenWithValues.append_child("MyChildWithBoolValue");
    nodeChild.append_child(pugi::node_pcdata).set_value(ToString(false).c_str());
    /// [add child wiith node value]
    // Save XML tree to file.
    bool saveSucceeded = doc.save_file(XML_FILE_PATH.c_str(), PUGIXML_TEXT("  "));
    assert(saveSucceeded);
}
//-----------------------------------------------------------------------------
void AddXmlSubtreeToExistingXmlFile()
{
    // Create empty XML document within  memory
    pugi::xml_document doc;
    // Load XML file into memory
    // Remark: to fully read declaration entries you have to specify
    // "pugi::parse_declaration"
    pugi::xml_parse_result result = doc.load_file(XML_FILE_PATH.c_str(),
        pugi::parse_default|pugi::parse_declaration);
    if (!result)
    {
        std::cout << "Parse error: " << result.description()
            << ", character pos= " << result.offset;
    }
    // A valid XML document must have a single root node
    pugi::xml_node root = doc.document_element();
    /// [add xml string to existing xml file]
    std::string externalXmlString = "<ExternalData>"
        "<X>-1.5</X>"
        "<NumPoints>23</NumPoints>"
        "<exists>true</exists>"
        "<SomeChild intVal=\"1508\" doubleVal=\"4.5\" boolVal=\"false\"/>"
        "</ExternalData>";
    // Read XML string into temporary document
    pugi::xml_document tmpDoc;
    if (tmpDoc.load(externalXmlString.c_str()))
    {
        // Create child node to hold external XML data
        pugi::xml_node childWithExternalSubtree = root.append_child("ChildWithExternalXmlSubtree");
        // Copy subtree from temporary document to target node
        childWithExternalSubtree.append_copy(tmpDoc.document_element());
    }
    /// [add xml string to existing xml file]
    // Save XML tree to file.
    bool saveSucceeded = doc.save_file(XML_FILE_PATH.c_str(), PUGIXML_TEXT("  "));
    assert(saveSucceeded);
}
//-----------------------------------------------------------------------------
void SearchForNodesUsingXpath(bool in_searchFirst)
{
    pugi::xml_document doc;
    doc.load_file(XML_FILE_PATH.c_str());
    pugi::xml_node root = doc.document_element();
    /// [xpath search node with attribute value]
    // Search for the first / last child entry with the given hint attribute
    std::string searchStr = in_searchFirst ? "MyChild[@hint='inserted as last child']"
                                           : "MyChild[@hint='inserted as last child'][last()]";
    pugi::xpath_node xpathNode = root.select_single_node(searchStr.c_str());
    if (xpathNode)
    {
        pugi::xml_node selectedNode = xpathNode.node();
        // now access found node
        // ...
        /// [xpath search node with attribute value]
        /// [check for attribute and read value]
        // Read attribute value
        pugi::xml_attribute attr;
        if (attr = selectedNode.attribute("intVal")) // attribute really exists
        {
             // Read value as string
             std::cout << "read as string: intVal=" << attr.value() << std::endl;
             // Read value as int
             int intVal = attr.as_int();
             std::cout << "read as int   : intVal=" << intVal << std::endl;
             // for other types use as_double, as_bool, as_uint, ...
        }
        /// [check for attribute and read value]
    }
}
//-----------------------------------------------------------------------------
void ReadVariousAttributeAndNodeTypes()
{
    pugi::xml_document doc;
    doc.load_file(XML_FILE_PATH.c_str());
    pugi::xml_node root = doc.document_element();
    /// [xpath search node]
    // Search for the first matching entry with the given hint attribute
    std::string searchStr = "ChildWithExternalXmlSubtree/ExternalData";
    pugi::xpath_node xpathNode = root.select_single_node(searchStr.c_str());
    if (xpathNode)
    {
        pugi::xml_node selectedNode = xpathNode.node();
        // continue with accessing node
        // ...
        /// [xpath search node]
        /// [read node values]
        // Read several child node values according to their expected types
        double x      = selectedNode.child("X")        .text().as_double();
        int numPoints = selectedNode.child("NumPoints").text().as_int();
        bool exists   = selectedNode.child("exists")   .text().as_bool();
        /// [read node values]
        /// [read attribute values]
        // Read various attribute types of "SomeChild" according to their expected types
        double doubleVal = selectedNode.child("SomeChild").attribute("doubleVal").as_double();
        int    intVal    = selectedNode.child("SomeChild").attribute("intVal").as_int();
        bool   boolVal   = selectedNode.child("SomeChild").attribute("boolVal").as_bool();
        /// [read attribute values]
        std::cout << "\nnode values read: X=" << x
            << " NumPoints=" << numPoints
            << " exists=" << std::boolalpha << exists << std::endl;
        std::cout << "attribute values read: doubleVal=" << doubleVal
            << " intVal=" << intVal
            << " boolVal=" << std::boolalpha << boolVal << std::endl;
    }
}
//-----------------------------------------------------------------------------
void WriteXmlDocumentToStream()
{
    pugi::xml_document doc;
    doc.load_file(XML_FILE_PATH.c_str(), pugi::parse_default|pugi::parse_declaration);
    /// [write xml doc to stream]
    // Write complete xml document to stdout
    std::cout << "\nWrite xml doc to stdout with indent of 1 char:" << std::endl;
    doc.save(std::cout," ");
    // Write complete xml document to string stream
    std::cout << "\nWrite xml doc to stringstream with indent of 2 chars:" << std::endl;
    std::stringstream ss;
    doc.save(ss,"  ");
    std::cout << "stream contents are:\n" << ss.str() << std::endl;
    /// [write xml doc to stream]
    std::string generatedXmlContents = ss.str();
    std::string expectedOutput =
/// [generated xml file]
R"(<?xml version="1.0" encoding="ISO-8859-1" standalone="yes"?>
<MyRoot>
  <MyChild hint="inserted at front" boolVal="false" />
  <MyChild hint="inserted at front" boolVal="true" />
  <MyChild hint="inserted as last child" intVal="4711" />
  <MyChild hint="also inserted as last child" doubleVal="3.14" />
  <MyChild hint="inserted as last child" intVal="4712" />
  <MyChild hint="also inserted as last child" doubleVal="3.15" />
  <ChildrenWithValue>
    <MyChildWithIntValue>4712</MyChildWithIntValue>
    <MyChildWithDoubleValue>3.180000</MyChildWithDoubleValue>
    <MyChildWithBoolValue>false</MyChildWithBoolValue>
  </ChildrenWithValue>
  <ChildWithExternalXmlSubtree>
    <ExternalData>
      <X>-1.5</X>
      <NumPoints>23</NumPoints>
      <exists>true</exists>
      <SomeChild intVal="1508" doubleVal="4.5" boolVal="false" />
    </ExternalData>
  </ChildWithExternalXmlSubtree>
</MyRoot>
)";
/// [generated xml file]
    // Ensure that generated xml contents are identical to the string above
    assert(generatedXmlContents == expectedOutput);
}
//-----------------------------------------------------------------------------
void WriteXmlSubTreeToStream()
{
    pugi::xml_document doc;
    doc.load_file(XML_FILE_PATH.c_str(), pugi::parse_default | pugi::parse_declaration);
    pugi::xml_node root = doc.document_element();
    /// [write xml subtree to stream]
    // Write xml subtree to stdout
    std::cout << "\nWrite subtree to stdout with indent of 1 char:" << std::endl;
    root.child("ChildWithExternalXmlSubtree").print(std::cout, " ");
    // Write xml subtree to string stream
    std::cout << "\nWrite subtree to stringstream with indent of 2 chars:" << std::endl;
    std::stringstream ss;
    root.child("ChildWithExternalXmlSubtree").print(ss, "  ");
    std::cout << "stream contents are:\n" << ss.str() << std::endl;
    /// [write xml subtree to stream]
    std::string generatedXmlContents = ss.str();
    std::string expectedOutput =
        R"(<ChildWithExternalXmlSubtree>
  <ExternalData>
    <X>-1.5</X>
    <NumPoints>23</NumPoints>
    <exists>true</exists>
    <SomeChild intVal="1508" doubleVal="4.5" boolVal="false" />
  </ExternalData>
</ChildWithExternalXmlSubtree>
)";
    // Ensure that generated xml contents are identical to the string above
    assert(generatedXmlContents == expectedOutput);
}
//-----------------------------------------------------------------------------
void RemoveAttributes()
{
    // Read simple XML string into document
    std::string xmlString = R"(<SomeNode x="0.5" y="2.18" z="-3.5" phi="20">)";
    pugi::xml_document doc;
    doc.load(xmlString.c_str());
    pugi::xml_node someNode = doc.document_element();
    {
        std::cout << "\nWrite tree before deletion of attributes" << std::endl;
        std::stringstream ss;
        someNode.print(ss, "  ");
        std::cout << ss.str() << std::endl;
        std::string generatedXmlContents = ss.str();
        std::string expectedOutput = R"(<SomeNode x="0.5" y="2.18" z="-3.5" phi="20" />)" "\n";
        assert(generatedXmlContents == expectedOutput);
    }
    /// [remove xml attribute while iterating]
    for (pugi::xml_attribute attr = someNode.first_attribute(); attr;)
    {
        // Get next attribute before possibly deleting current attribute
        pugi::xml_attribute nextAttr = attr.next_attribute();
        // Check wether attribute shall be deleted
        if ((std::string(attr.name())=="phi") || (attr.as_double() < 0))
        {
            someNode.remove_attribute(attr);
        }
        attr = nextAttr;
    }
    /// [remove xml attribute while iterating]
    {
        std::cout << "\nWrite tree after deletion of attributes" << std::endl;
        std::stringstream ss;
        someNode.print(ss, "  ");
        std::cout << ss.str() << std::endl;
        std::string generatedXmlContents = ss.str();
        std::string expectedOutput = R"(<SomeNode x="0.5" y="2.18" />)" "\n";
        assert(generatedXmlContents == expectedOutput);
    }
    /// [remove xml attribute by name]
    someNode.remove_attribute("x");
    /// [remove xml attribute by name]
    {
        std::cout << "\nWrite tree after second deletion of attributes" << std::endl;
        std::stringstream ss;
        someNode.print(ss, "  ");
        std::cout << ss.str() << std::endl;
        std::string generatedXmlContents = ss.str();
        std::string expectedOutput = R"(<SomeNode y="2.18" />)" "\n";
        assert(generatedXmlContents == expectedOutput);
    }
}
//-----------------------------------------------------------------------------
void RemoveNodes()
{
    // Read simple XML string into document
    std::string xmlString = R"(<Subtree>
  <SomeNode>some content</SomeNode>
  <OtherNode x="0.5" y="2.18" z="-3.5" phi="20" />
  <AnOtherNode x="4.7" z="-3.5" />
  <AnOtherNode z="22.3" />
  <OtherNode x="0.5" y="2.18" z="-3.5" phi="20" />
</Subtree>)";
    pugi::xml_document doc;
    doc.load(xmlString.c_str());
    pugi::xml_node someParentNode = doc.document_element();
    {
        std::cout << "\nWrite tree before deletion of child nodes" << std::endl;
        std::stringstream ss;
        someParentNode.print(ss, "  ");
        std::cout << ss.str() << std::endl;
        std::string generatedXmlContents = ss.str();
        std::string expectedOutput = xmlString + "\n";
        assert(generatedXmlContents == expectedOutput);
    }
    /// [remove xml child nodes while iterating]
    for (pugi::xml_node child = someParentNode.first_child(); child; )
    {
        // Get next child node before possibly deleting current child
        pugi::xml_node next = child.next_sibling();
        // Check wether child node shall be deleted
        if (std::string(child.name()) != "AnOtherNode")
        {
            child.parent().remove_child(child);
        }
        child = next;
    }
    /// [remove xml child nodes while iterating]
    {
        std::cout << "\nWrite tree after deletion of child nodes" << std::endl;
        std::stringstream ss;
        someParentNode.print(ss, "  ");
        std::cout << ss.str() << std::endl;
        std::string generatedXmlContents = ss.str();
        std::string expectedOutput = R"(<Subtree>
  <AnOtherNode x="4.7" z="-3.5" />
  <AnOtherNode z="22.3" />
</Subtree>)" "\n";
        assert(generatedXmlContents == expectedOutput);
    }
    /// [remove xml child node by name]
    // "remove_child" only removes the first found child
    // the return value signals if a child was found
    // removing all childs with a given name:
    while(someParentNode.remove_child("AnOtherNode"));
    /// [remove xml child node by name]
    {
        std::cout << "\nWrite tree after second deletion of child nodes" << std::endl;
        std::stringstream ss;
        someParentNode.print(ss, "  ");
        std::cout << ss.str() << std::endl;
        std::string generatedXmlContents = ss.str();
        std::string expectedOutput = R"(<Subtree />)" "\n";
        assert(generatedXmlContents == expectedOutput);
    }
}
void DemonstrateUsageOfPugiXml()
{
    std::cout << "\n----------------------------" << std::endl;
    std::cout << "\n\nHow to use PugiXml (no checks, simply write to stdout)" << std::endl;
    CreateNewXmlFileWithEmptyRootNode();
    AddSomeChildNodesWithVariousAttributeTypesToExistingXmlFile(4711,3.14,true);
    AddSomeChildNodesWithVariousAttributeTypesToExistingXmlFile(4712,3.15,false);
    AddSomeChildNodesWithDirectValuesToExistingXmlFile();
    AddXmlSubtreeToExistingXmlFile();
    SearchForNodesUsingXpath(true);
    SearchForNodesUsingXpath(false);
    ReadVariousAttributeAndNodeTypes();
    WriteXmlDocumentToStream();
    WriteXmlSubTreeToStream();
    RemoveAttributes();
    RemoveNodes();
    std::cout << "\n----------------------------" << std::endl;
}
//-----------------------------------------------------------------------------
TTB_BOOST_TEST_CASE(Demo_HowToUsePugiXmlWithinProductionCode)
{
    /// [boost.test optionally skip execution]
    // Optionaly skip execution
    if (!TTB::TheEnvironment()->IsExistingCommandLineOption("-suppressDemoOutput"))
    {
        DemonstrateUsageOfPugiXml();
    }
    /// [boost.test optionally skip execution]
    /// [boost.test read param value from command line]
    // For demonstration purpose read some value from command line.
    // A not existing value will be default constructed (here 0.0)
    double valX = TTB::TheEnvironment()->GetCommandLineOptionVal<double>("-valX");
    // Write value as informational message to test protocol
    TTB_INFO_S("command line param valX=" << valX);
    /// [boost.test read param value from command line]
}}
//-----------------------------------------------------------------------------
/// \endcond
```
