#include <wx/xml/xml.h>

#include "xml_config.h"
#include "../common/convert.h"

namespace ygopro
{

	bool CommonConfig::LoadConfig(const std::wstring& name) {
		wxXmlDocument doc;
		if(!doc.Load(name, "UTF-8", wxXMLDOC_KEEP_WHITESPACE_NODES))
			return false;
		wxXmlNode* root = doc.GetRoot();
		wxXmlNode* child = root->GetChildren();
		while (child) {
            if(child->GetType() == wxXmlNodeType::wxXML_ELEMENT_NODE) {
                if (child->GetName() == "integer") {
                    std::wstring name = child->GetAttribute("name").ToStdWstring();
                    long value = To<long>(child->GetAttribute("value").ToUTF8().data());
                    config_map[name] = value;
                } if (child->GetName() == "float") {
                    std::wstring name = child->GetAttribute("name").ToStdWstring();
                    double value = To<long>(child->GetAttribute("value").ToUTF8().data());
                    config_map[name] = value;
                } else {
                    std::wstring name = child->GetAttribute("name").ToStdWstring();
                    std::wstring value = child->GetAttribute("value").ToStdWstring();
                    config_map[name] = value;
                }
            }
			child = child->GetNext();
		}
        return true;
	}

	void CommonConfig::SaveConfig(const std::wstring& name) {
		if(config_map.size() == 0)
			return;
		wxXmlDocument doc;
		doc.SetVersion(wxT("1.0"));
		doc.SetFileEncoding(wxT("UTF-8"));
		wxXmlNode* root = new wxXmlNode(wxXML_ELEMENT_NODE, "Common");
		for(auto iter : config_map) {
            if(iter.second.val_type == 0) {
                wxXmlNode* element = new wxXmlNode(wxXML_ELEMENT_NODE, "integer");
				char buf[32];
				sprintf(buf, "%ld", iter.second.ivalue);
				element->AddAttribute("name", iter.first);
				element->AddAttribute("value", buf);
                root->AddChild(element);
			} else if(iter.second.val_type == 1) {
                wxXmlNode* element = new wxXmlNode(wxXML_ELEMENT_NODE, "float");
				char buf[32];
				sprintf(buf, "%lf", iter.second.fvalue);
				element->AddAttribute("name", iter.first);
				element->AddAttribute("value", buf);
                root->AddChild(element);
            } else {
                wxXmlNode* element = new wxXmlNode(wxXML_ELEMENT_NODE, "string");
				element->AddAttribute("name", iter.first);
				element->AddAttribute("value", iter.second.svalue);
                root->AddChild(element);
            }
		}
		doc.SetRoot(root);
		doc.Save(name);
	}

}
