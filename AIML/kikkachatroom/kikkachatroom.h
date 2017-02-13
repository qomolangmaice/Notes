
#pragma once

//#include <windows.h>
#include <vector>
#include <map>


#include <iostream>//std::cout
#include <sstream> 
#include <cstdlib>  
#include <cstring> //std::string
using namespace std;
#include <stdio.h>
#include <fstream>   
//#include <io.h> 

typedef wchar_t char_t;
typedef std::basic_string<char_t> string_t;

#include <time.h>
#include <algorithm>
//#include <rapidxml/rapidxml.hpp>    
//#include <rapidxml/rapidxml_utils.hpp> 
//#include <rapidxml/rapidxml_print.hpp>  


#include "md5.h"
#include "logic.h"
using namespace std;






string AiLoad_AiFile(string file,string str,int &e,string &txt);
string AiLoad(string str);
wstring UTF8ToUnicode(string str );
string UnicodeToANSI(wstring str );
string Utf8ToAnsi(const char *pstrUTF8);
string AnsiToUtf8(string str);
int GetTextType(const char*fPath);
void LastTalkArr(string);
string strcut(string old, string start, string end) ;
string TextBetween(string str,string start,string end,int idx);
string TextBetween2(string str,string start,string end);
void SPLIT2(const string&str ,vector<string> &tokens,const string& separator);
void SPLIT3(const string& src ,vector<string> &tokens,const string& delimiters);
void SPLIT(const string &src, vector<string> &tokens,const string& separator);
string cut(string str,int n,string delimiters);
//SETDELIM(string str,string delimiters);
void FENUM(string path,vector<string> &file);
void SPLITPATH(const char *arg, vector<string> &array);
void AiLoad_InfFile(string file,string str,int e);
char *TOLOWER(const char* str);
char *TOUPPER(const char* str);
string AiGet(string str);
string TOSTR(long str);
long RAND(long num);
string REPLACE(string str, const char *before, const char *after);


string ToUTF8(const std::wstring& str);
string ToUTF8(const wchar_t* buffer, int len);
string AiLoad_TopicMatch(string str);
void AiLoad_Think(string txt);
void EVAL(string str);
string TextToEnd(string str,string begin,int idx);
int AiLoad_KeyMatch(string str,string Q,int e);
string AiLoad_ThatMatch(string str);
string topic;
vector<string> lastTalkHistory;
int qnum;
vector<string> String;
string aimode;
int dexnum;
vector<string> dexori;
int infmode;
string RandAiTalk(void);
int Tflag;


