/**
* @file
* @brief initialization file read and write API
* @author Deng Yangjun
* @date 2007-12-9
* @version 0.2
*  (C)2007 Midapex
* This program is free software; you can redistribute it and/or modify it
*  under the terms of the GNU Library General Public License as published
*  by the Free Software Foundation; either version 2 of the License,
*  or (at your option) any later version.
*/
#ifndef INI_FILE_CPP_H_
#define INI_FILE_CPP_H_

#include <string>
using namespace std;

class IniFile
{
public:
    IniFile(const string & fileName);
public:
    virtual ~IniFile(void);

    const string & getFileName() const;

    const string &getSection() const;
    void setSection(const string §ion);

    bool write(const string &key, const string & value) const ;
    bool write(const string &key, int value) const ;

    string readStr(const string &key,const string &default_value) const;
    int readInt(const string &key, int default_value) const;

public:
    static int read_profile_string( const char *section, const char *key,char *value,
        int size, const char *default_value, const char *file);
    static int read_profile_int( const char *section, const char *key,int default_value,
        const char *file);
    static int write_profile_string(const char *section, const char *key,
        const char *value, const char *file);

private:
    static int load_ini_file(const char *file, char *buf,int *file_size);
    static int newline(char c);
    static int end_of_string(char c);
    static int left_barce(char c);
    static int right_brace(char c );
    static int parse_file(const char *section, const char *key, const char *buf,int *sec_s,int *sec_e,
        int *key_s,int *key_e, int *value_s, int *value_e);

private:
    string m_fileName;
    string m_section;
};

#endif
