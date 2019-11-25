#include "IniFile.h"
#include <iostream>
using namespace std;

int main()
{
    cout << "Midapex IniFile API 0.2.0" << endl;
    string name_key = "name";
    string age_key = "age";

    IniFile ini("myconfig.ini");
    ini.setSection("student");
//    if(!ini.write(name_key,"Tony"))
//    {
//        cout << "write name to ini file fail"<<endl;
//        return -1;
//    }
    if(!ini.write(age_key,20))
    {
        cout << "write age to ini file fail"<<endl;
        return -1;
    }

    cout << "[" << ini.getSection()<< "]" <<endl;
    cout << name_key << "=" << ini.readStr(name_key,"") << endl;
    cout << age_key << "=" << ini.readInt(age_key,-1) << endl;

    ini.setSection("teacher");
    if(!ini.write(name_key,"Tom"))
    {
        cout << "write name to ini file fail"<<endl;
        return -1;
    }
    if(!ini.write(age_key,50))
    {
        cout << "write age to ini file fail"<<endl;
        return -1;
    }

    cout << "[" << ini.getSection()<< "]" <<endl;
    cout << name_key << "=" << ini.readStr(name_key,"") << endl;
    cout << age_key << "=" << ini.readInt(age_key,-1) << endl;

    return 0;
}
