#ifndef LX_OBJECT_H
#define LX_OBJECT_H

#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/writer.h"       // for stringify JSON
#include "type_name.h"

using lxWriter  = rapidjson::Writer<rapidjson::StringBuffer>;
using lxParse   = rapidjson::Value;

#define lxCLASS_NAME(name)  \
public:\
    virtual wxString getClassName() override {return type_name<name>();}

class lxObject{
    public:
        virtual wxString getClassName() = 0;
        virtual wxString getData(){
            rapidjson::StringBuffer s;
            lxWriter writer(s);
            writeData(writer);
            return s.GetString();
        }
        virtual void setData(const wxString &data){
            rapidjson::Document doc;
            doc.Parse(data.c_str());
            if(!doc.IsObject()) return;
            parseData(doc);
        }
        virtual void writeData(lxWriter &writer){
            writer.StartObject();
            writeMembers(writer);
            writer.EndObject();
        }
        virtual void parseData(lxParse &parser){
            if(!parser.IsObject()) return;
            parseMembers(parser);
        }
    protected:
        virtual void writeMembers(lxWriter &writer) = 0;
        virtual void parseMembers(lxParse &parser) = 0;

};

#endif // LX_OBJECT_H
