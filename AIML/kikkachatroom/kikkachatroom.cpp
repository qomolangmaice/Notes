
#include "kikkachatroom.h"

main()
{
	string answer;
	string txt;
	//s+="ddddd����";
	//s+="\nssss";
	//s="���ã���\n����\n�ֽУ�����ָ��";
	//	strncpy(t,cut(str,1,"����"),500);
	//sprintf(t,"%s");
	//strncpy(t,cut(t,0,"��"),500);
	//	s=TextBetween(s,"����","��",0);
	//	s=cut(s,1,"����");
	//s=cut(s,0,"��");
	//string num="123";
	//long int n=0;
	//n=strtol(num.c_str(),NULL,10);
	//cout<<n;
	//itoa(a,txt,10);
	//txt=TOSTR(a);
	//txt="<Q></Q>\n";
	//txt=TextBetween("<Q></Q>\n","<Q>","</Q>",0);
	//txt=txt.substr(3,3);
	//txt=cut("<Q></Q>\n",1,"<Q>");
	//txt=cut(txt,0,"</Q>");
	//cout<<"->"<<"'"<<txt<<"'";
	//txt="<topic name=\"�ļ�����\">";
	//cout<<"'"<<TextToEnd(txt,">",1)<<"'";
	struct tm *local; 
	time_t t; 
	t=time(NULL); 
	local=localtime(&t); 
	cout<<"Andy > "<<local->tm_hour<<":"<<local->tm_min<<":"<<local->tm_sec<<endl;
	answer=RandAiTalk();
	cout<<answer<<endl;
	LastTalkArr(answer);
	while (1)
	{
		cin>>txt;
		t=time(NULL); 
		local=localtime(&t);  
		cout<<local->tm_hour<<":"<<local->tm_min<<":"<<local->tm_sec<<endl;
		answer=AiGet(txt);
		//string file="myai\\meal.ai";
		//answer=AiLoad_AiFile(file,txt,0,txt);
		t=time(NULL); 
		local=localtime(&t); 
		cout<<"localhost > "<<local->tm_hour<<":"<<local->tm_min<<":"<<local->tm_sec<<endl;
		cout<<answer<<endl;
		LastTalkArr(answer);
		//cout<<"lasttalk="<<lastTalkHistory[0];
		//system("pause");
	}
}

/*
int   main3()   
{           
	string   path; 
	vector<string> file;
	//cout<<"������Ŀ¼"<<endl;   
	//cin>>path;
	path="myai";
	string s=TOLOWER(path.c_str());
	//SPLITPATH(path.c_str(),file); 
	cout<<s;
	cout<<file[0]<<"0\n";
	cout<<file[1]<<"1\n";
	cout<<file[2]<<"2\n";
	system("PAUSE");   
	return   0;     
}   


int main4()  
{  	
using namespace rapidxml;  
xml_document<> doc;  	
xml_node<> *node = doc.allocate_node(node_element,"a","Google��˾");  	
doc.append_node(node);  	
xml_attribute<> *attr = doc.allocate_attribute("href","google.com");  	
node->append_attribute(attr);  	
//ֱ������  	
cout<<"print:doc"<<doc<<endl;  	
//�ñ��浽string  
std::string strxml;  
print(std::back_inserter(strxml),doc,0);  
cout<<"print:strxml"<<strxml<<endl;  
//֧��c���������ﲻ��ʾ,�Լ����ĵ���  	
return 0;  	
}  

  void main()
  {
  using namespace rapidxml;  
  xml_document<> doc;
  xml_node<>* decl = doc.allocate_node(node_declaration);
  xml_attribute<>* decl_ver =doc.allocate_attribute("version", "1.0");
  decl->append_attribute(decl_ver);
  doc.append_node(decl);
  xml_node<> *node =doc.allocate_node(node_element,"Hello","World");
  doc.append_node(node);
  string text;
  print(back_inserter(text),doc,0);
  // write text to file by yourself
  }
  
	
*/



char *TOLOWER(const char* str)
{
	
	char *p;
	p=(char *)malloc(strlen(str)+1);
	strcpy(p,str);
	for(int i = 0; i < sizeof(p); i++)
		if (p[i]>= 'A' && p[i]<= 'Z')
			p[i]+= ('a' - 'A');
		
		return p;  
}  

char *TOUPPER(const char* str)
{
	
	char *p;
	p=(char *)malloc(strlen(str)+1);
	strcpy(p,str);
	for(int i = 0; i < sizeof(p); i++)
		if (p[i]>= 'a' && p[i]<= 'z')
			p[i]+= ('A' - 'a');
		
		return p;  
}  

string TOSTR(long str)
{
	stringstream sout; 
	sout<<str; 
	return sout.str(); 
}

void LastTalkArr(string talk)
{
	int i = 0;
	vector<string> lasttalk;
	
	lastTalkHistory.push_back(talk);
	//cout<<lastTalkHistory[0];
	int n=lastTalkHistory.size();
	//cout<<n;
	for (i=n-1;i>=0;i--)
	{
		lasttalk.push_back(lastTalkHistory[i]);
	}
	//cout<<lasttalk[0];
	n=lasttalk.size();
	lastTalkHistory.clear();
	for (i=0;i<n;i++)
	{
		lastTalkHistory.push_back(lasttalk[i]);
	}	
}
string AiGet(string str)
{
	int i = 0;
	string found;
	found=AiLoad(str);
	string txt;
	txt="";
	vector<string> temp;
	//return found;
	SPLIT(found,temp,"#");
	
	int n=temp.size()-1;
	//return temp[0];
	//cout<<n;
	//return temp[0] ;
	for (i=0;i<n;i++)
	{
		found=temp[i];
		vector<string> tfound;
		SPLIT(found,tfound,"|");
		int count=tfound.size()-1;
		if (count>0)
		{
			int id=RAND(count);
			found=tfound[id];
			if (strstr(found.c_str(),"<think>"))
			{
				AiLoad_Think(found);
				found=cut(found,0,"<think>")+cut(found,1,"</think>");
				//found=RE_REPLACE(_found,'<think>.*</think>','')
			}
			
			
			txt+=found;
		}
	}
	
	
	return txt;
}

long RAND(long num)
{
	
	srand(time(0));  
	long id;
	if (num==0)
		id=num;
	else if(num<0)
		id=0;
	else
		id=rand() %num ;   
	return id;
	
	
}

//static int e;
string AiLoad(string str)
{
	string path="myai";
	string found="";
	int snum=0;
	qnum=0;
	int anum=0;
	int length=0;
	int num=0;
	string txt="";	
	vector<string> files;
	FENUM(path,files);
	int e=0;
	int E=e;
	//aimode="";
	int lastm=0;
	int strnum;
	int a = 0;
	String.clear();
	String.push_back(str);
	strnum=String.size();
	for (a=0;a<strnum;a++)
	{
		str=String[a];
		aimode="";
		for(int i=0;i<files.size();i++)
		{
			string file=files[i];
			vector<string> tfile;
			SPLITPATH(file.c_str(),tfile);
			
			//string filename=TOLOWER(tfile[2].c_str());
			//return filename;
			//file = path +"/"+ file;
			string fileext = TOLOWER(tfile[3].c_str());
			//return file ;
			if(fileext== ".ai")
			{
				string t=AiLoad_AiFile(file,str,e,txt);
				if (aimode=="break")
				{
					txt="";
					break;
				}
				else if (t!="-1")
					txt=t;
			}
			
		}
		strnum=String.size();
	}
	if (strstr(str.c_str(),"�Ի�"))
		txt="�ٻ�ѧ���˳���"+TOSTR(qnum)+"���Ի�Ŷ"+"|";
	
	found+=txt+"#";
	return found;
	
	
}



void AiLoad_InfFile(string file,string str,int e)
{
	vector<string> tfile;
	SPLITPATH(file.c_str(),tfile);
	string path=tfile[1];
	string filename=TOLOWER(tfile[2].c_str());
	string inf=path+filename+".inf";
	
	int open=0;
	//string md5=MD5(ifstream(file.c_str())).toString();
	FILE* fp1;
	char *buff;
	char text[10240];
   	fp1=fopen(inf.c_str(),"r");
	//	cout<<(fp1!=NULL);
	//return "";
	//if (infile.is_open())
	//{
	int fileisascii=1;
	if (GetTextType(file.c_str()))
		fileisascii=0;
	int isascii=1;
	if (GetTextType(inf.c_str()))
		isascii=0;
	char szBOM[4]={(char)0xEF,(char)0xBB,(char)0xBF,0};
	
	
	
	
	
	if (fp1!=NULL)
	{
		
		buff=fgets(text,1000,fp1);
		//tring newmd5=cut(text,1,"md5=");
		//newmd5=REPLACE(newmd5,"\n","");
		//cout<<"'"+md5+"'"+"->"+"'"+newmd5+"'"<<endl;
		buff=fgets(text,1000,fp1);
		string strcha=str;
		string Ttemp="";
		Tflag=0;
		string TflagTxt="";
		int lastm=0;
		int laststar=0;
		int ql=0;
		int E=0;
		int l = 0;
		//cout<<text;
		while (buff!=NULL)
		{
			string temp=text;
			if (!isascii)
				temp=Utf8ToAnsi(temp.c_str());
			string dextemp=cut(temp,1,"|");
			dextemp=REPLACE(dextemp,"\n","");
			string Q=cut(temp,0,"|");
			//cout<<isascii<<fileisascii<<inf;return;
			if (filename=="meal")
			{		
				if (strstr(str.c_str(),Q.c_str()))
				{
					dexori.push_back(dextemp);
					dexnum++;
				}
			}
			
			else if (filename!="user")
			{			
				if (strstr(text,"|")&&filename!="meal"&&filename!="food")
					qnum++;
				
				if (strstr(text,"<topic"))
				{ 
					string t=temp;
					string temp=AiLoad_TopicMatch(t);
					if (temp!="")
					{
						Ttemp=dextemp;
						Tflag=1;//cout<<"'"<<Ttemp<<"'";
					}
				}
				
				
				if (strstr(Q.c_str(),"&")||strstr(Q.c_str(),"*" ))
				{
					vector<string> Qs;
					unsigned int k=0;
					unsigned int n=0;
					
					if (strstr(Q.c_str(),"&"))
					{
						SPLIT(Q,Qs,"&");
						n=Qs.size();
						//cout<<"n is"<<n;
						k+=n-1;
					}
					else if (strstr(Q.c_str(),"*"))
					{
						SPLIT(Q,Qs,"&");
						n=Qs.size();
					}
					
					int m = 0;
					for (l=0, m=1;l<n && m==1;l++)
					{
						if(strstr(str.c_str(),Qs[l].c_str()))
							m=1;
						else
							m=0;
						k+=strlen(Qs[l].c_str());		
					} 
					
					if  (m==1 && k==e && lastm==1)
					{
						dexori.push_back(dextemp);
						dexnum++;     
						//anum+=Anum-1;
					}
					else if (m==1 && k>e)
					{
						lastm=1;
						e=k;
						dexnum=1;
						dexori.clear();
						dexori.push_back(dextemp);
						//anum=Anum-1;
					}
				}
				int len=strlen(Q.c_str());
				if (strstr(str.c_str(),Q.c_str())&&len==e && lastm==0 )
				{
					dexori.push_back(dextemp);
					dexnum++; 
				}
				else if (strstr(str.c_str(),Q.c_str())&& len>=e)
				{//���ϴ����󳤶�ƥ�䣾����,�����Ǻ��ֱ���������
					lastm=0;
					e=len;
					dexnum=1;
					dexori.clear();
					dexori.push_back(dextemp);
				}	
				
			}
			buff=fgets(text,1000,fp1);
		}
		
		fclose(fp1);
		//if (md5!=newmd5&&filename!="user")
		if (filename!="user")
		{ 
			dexnum=0;
			infmode=1;
		}
		
		else if (dexnum==0)
		{
			//if (filename!="food")
			if (Tflag==1)
			{
				dexori.push_back(Ttemp);
				dexnum++;     
			}
			else
			{
				dexori.clear();
				dexori.push_back("-1");		
			}
		}
		else if (dexnum!=0&&dexori.size()==0)
		{	//if filename=="ZLOGIC"
			dexnum=0;
			infmode=1;	
		}
	}
	
	else if (filename!="user")
	{	//if filename=="ZLOGIC"
		infmode=1;
	}
	
	/*
	if (infmode==1)
	{
		
		string temp="md5="+md5+"\n";
		fp1=fopen(inf.c_str(),"w");
		if (fp1!=NULL)
		{
			if (!fileisascii)
				fprintf(fp1,"%s",szBOM);
			fputs(temp.c_str(),fp1);	
			fclose(fp1);
		}
	}
	*/
	//	cout<<dexnum;
	//	cout<<dexori.size()<<endl;
	
	
}



string AiLoad_AiFile(string file,string str,int &e,string &txt)
{
	FILE* fp1;
	FILE* fp2;
	char *buff;
	char text[1024];
	vector<string> tfile;
	SPLITPATH(file.c_str(),tfile);
	string path=tfile[1];
	string filename=TOLOWER(tfile[2].c_str());
	string inf=path+filename+".inf";
	//return file;
	//fstream infile;
	//infile.open(file.c_str());          
	//fp1 =infile.is_open();
	int found=0;
	infmode=0;
	dexnum=0;
	dexori.clear();
	AiLoad_InfFile(file,str,e);	
	//return TOSTR(dexori.size());
	if (dexori.size())
		if (dexori[0]=="-1")
		{
			return "-1";
			
		}
		fp1=fopen(file.c_str(),"r");
		//	cout<<(fp1!=NULL);
		//return "";
		//if (infile.is_open())
		//{
		if (fp1!=NULL)
		{
			buff="1";
			int dexid=0;
			int Lastm=0;
			int anum=0;
			int N=0;
			int laststar=0;
			int isascii=1;
			long offset;
			if (GetTextType(file.c_str()))
				isascii=0;
			//return dexori[0];
			//cout<<file<<"->"<<dexnum;
			//	return TOSTR(dexnum);
			if(infmode)
				fp2=fopen(inf.c_str(),"a");
			while (buff!=NULL)
			{
				
				
				if (dexnum>0&&dexid<dexnum)
				{
					offset=strtol(dexori[dexid].c_str(),NULL,10)-11;//cout<<dexori[dexid]<<endl;
					fseek(fp1,offset,0);
					buff=fgets(text,500,fp1);
					dexid++;
					//dexmode=1;
				}
				else if (dexnum>0&&dexid==dexnum)
				{
					buff=NULL;
					break;
				}
				else
					buff=fgets(text,500,fp1);
				
				
				int topicmode=0;	
				string topicdex="";
				string topic="";
				string temp=text;
				temp=REPLACE(temp,"\n","");
				//if (strstr(text,"<topic"))
				//cout<<text;
				//cout<<"'"<<TextToEnd(temp,">",1)<<"'";
				if (strstr(text,"<topic")&&TextToEnd(temp,">",1)==">")
				{
					topicdex=TOSTR(ftell(fp1));
					topicmode=1;
					topic=temp;
					//cout<<text<<endl;
				}
				
				
				string dex="";
				
				if (strstr(text,"<SESSION>"))
				{
					dex=TOSTR(ftell(fp1));
					topicmode=2;
				}
				string session="";
				string Q;
				
				if (topicmode==1)
				{
					if (infmode)
					{
						string temp=topic+"|"+topicdex+"\n";
						if (fp2!=NULL)
							fputs(temp.c_str(),fp2);
					}
					string t=text;
					string temp=AiLoad_TopicMatch(t);
					if (temp!="")
					{
						while (strstr(text,"<SESSION>")==NULL&&buff!=NULL)
						{
							buff=fgets(text,500,fp1);
						}
						
						session=text;
						while (strstr(text,"</SESSION>")==NULL&&buff!=NULL)
						{
							buff=fgets(text,500,fp1);
							session+=text; 
						}
						
					}	
					else
					{
						while (!strstr(text,"</topic>")&&buff!=NULL)
						{
							buff=fgets(text,500,fp1);
						}
					}
					
				}
				
				else if (topicmode==2||Tflag==1)
				{
					while (strstr(text,"<SESSION>")==NULL&&buff!=NULL)
					{
						buff=fgets(text,500,fp1);
					}
					if (strstr(text,"<SESSION>"))
					{
						dex=TOSTR(ftell(fp1));
					}
					session=text;
					while (strstr(text,"</SESSION>")==NULL&&buff!=NULL)
					{
						buff=fgets(text,500,fp1);
						session+=text;
						if (infmode)
						{
							Q=TextBetween(text,"<Q>","</Q>",0);
							if (Q!="")
							{
								string temp=Q+"|"+dex+"\n";
								if (fp2!=NULL)
									fputs(temp.c_str(),fp2);
								
							}
						} 
					}
				}
				
				if (!isascii)
					session=Utf8ToAnsi(session.c_str());
				//return session;
				
				session=TextBetween(session,"<SESSION>","</SESSION>",0);
				//session=cut(session,1,"<SESSION>");
				//	session=cut(session,0,"</SESSION>");
				//return session;
				//cout<<Tflag<<session;
				int Qnum=0;
				string A;
				int r=0;
				int j = 0;
				int thinkmode=0;
				if (strstr(session.c_str(),"<Q>"))
				{
					while (cut(session,Qnum,"<Q>")!="")
					{
						Qnum++;
					}
				}
				for (j=0;j<Qnum-1;j++)
				{
					Q=TextBetween(session,"<Q>","</Q>",j);
					string A="";
					int Anum=0;
					string Atemp="";
					int k = 0;
					if (strstr(session.c_str(),"<A>"))
					{
						
						while (cut(session,Anum,"<A>")!="")
						{
							Anum++;
						}
						
						for (k=0;k<Anum-1;k++)
						{
							int Aflag=0;
							string temp="";
							string t=TextBetween(session,"<A>","</A>",k);
							
							if (strstr(t.c_str(),"<think>"))
								thinkmode=1;
							if (strstr(t.c_str(),"<topic"))
							{
								temp=AiLoad_TopicMatch(t);
								if (temp!="")
								{
									Atemp=temp;
								}
								t=TextToEnd(t,">",1);
								t=cut(t,0,"</topic>");
								Aflag=1;//cout<<topic<<temp<<endl;
							}
							//cout<<temp<<Aflag<<"-"<<endl;
							if (strstr(t.c_str(),"<that>"))
							{
								//cout<<temp<<Aflag<<"-"<<endl;
								if (temp==""&&Aflag)
									continue;
								//cout<<t<<"-";
								temp=AiLoad_ThatMatch(t);//cout<<temp<<"-";
								if (temp!="")
									Atemp=temp;	
								else if (Aflag==1)
									Atemp="";
								t=cut(t,1,"</that>");		
								Aflag=1;
							}
							if (temp==""&&Aflag)
								continue;
							A+=t+"|";	
						}
						if (Atemp!="")
							A=Atemp+"|";
						
					}
					
					int m=0;
					if (A=="|" )
						continue;
					//cout<<Q;
					
					if (Q=="*"&&Tflag)
					{
						Q=str;//cout<<Tflag<<endl;
					}

					if (strstr(Q.c_str(),"&")||strstr(Q.c_str(),"*" ))
					{
						vector<string> Qs;
						unsigned int k=0;
						unsigned int n=0;
						if (strstr(Q.c_str(),"&"))
						{
							SPLIT(Q,Qs,"&");
							n=Qs.size();
							k+=n-1;
							//cout<<"n is"<<n;
						}
						else if (strstr(Q.c_str(),"*"))
						{
							SPLIT(Q,Qs,"&");
							n=Qs.size();
						}
						
						int l = 0, m = 0;
						for (l=0, m=1;l<n && m==1;l++)
						{
							if(strstr(str.c_str(),Qs[l].c_str()))
								m=1;
							else
								m=0;
							k+=strlen(Qs[l].c_str());
						} 
						//cout<<e;
						if (m==1 && k==e && Lastm==1)
						{
							txt+=A;               
							anum+=Anum-1;
							r=1;
						}
						else if (m==1 && k>e)
						{
							Lastm=1;
							e=k;
							txt=A;               
							anum=Anum-1;//cout<<txt;
							r=1;
						}
					}
					int len=strlen(Q.c_str());
					//cout<< file<<"-"<<e<<len<<"'"<<Q<<"'"<<str<<Q<<(strstr(str.c_str(),Q.c_str())&&len==e && Lastm==0 )<<(len==e)<<(Lastm==0) ;
					if (strstr(str.c_str(),Q.c_str())&&len==e && Lastm==0 )
					{
						txt+=A;               
						anum+=Anum-1;	
						r=1;	
					}
					else if  (strstr(str.c_str(),Q.c_str())&& len>=e)
					{//���ϴ����󳤶�ƥ�䣾����,�����Ǻ��ֱ���������
						Lastm=0;
						e=len;
						txt=A;
						r=1;          
						anum=Anum-1;		
					}
					//cout<< file<<"-"<<e<<len<<"'"<<txt<<"'";
					
			}
			if (r)
			{
				found=r;
				if (strstr(session.c_str(),"<think>")&&!thinkmode)
				{
					AiLoad_Think(session);
				}
			}
			
		}
		if(infmode)
			if (fp2!=NULL)
				fclose(fp2);
	}
	fclose(fp1);
	static int i=e;
	
	if (found)
	{
		if (strstr(txt.c_str(),"<srai>"))
		{
			if (strstr(txt.c_str(),"<think>"))
				AiLoad_Think(txt);
			String.push_back(TextBetween(txt,"<srai>","</srai>",0));
			aimode="break";
			//cout<<String.size();
		}  
	}	
	//infile.close();
	//cout<<e<<file;
	return txt;
}

string AiLoad_TopicMatch(string str)
{
	//	cout<<"topic="<<topic;
	string t=str;
	string Atemp="";
	string value=TextBetween(t,"name=",">",0);
	value=REPLACE(value,"\"","");
	if (topic==value)
	{
		t=TextToEnd(t,">",1);
		t=cut(t,0,"</topic>");
		Atemp=t;	//cout<<"topic="<<topic;
	}
	
	return Atemp;	
}



string AiLoad_ThatMatch(string str)
{
	string Atemp="";
	string that=TextBetween(str,"<that>","</that>",0);
	//cout<<that;
	string startxt="";
	int n=lastTalkHistory.size();
	//cout<<n;
	int i = 0;
	for (i=0;i<n;i++)
	{
		string t=lastTalkHistory[i];
		//cout<<t<<"-"<<that<<endl;
		if (AiLoad_KeyMatch(t,that,0))
		{
			//cout<<t<<endl;
			t=cut(str,1,"</that>");
			Atemp=t;
			startxt=t;//cout<<Atemp<<endl;
		}
								;
	}
	//	if (strstr(Atemp,"<thatstar/>")
	//	Atemp=ReplaceStarTxt(startxt,that,Atemp,'thatstar');
	return Atemp;
	
}


int AiLoad_KeyMatch(string str,string Q,int e)
{
	int match=0;
	
	if (strstr(str.c_str(),Q.c_str())&& strlen(Q.c_str())>=e)
	{
		match=1;
	}	
	return	match;
	
}


void AiLoad_Think(string txt)
{
	string think=TextBetween(txt,"<think>","</think>",0);
	int i=0;
	while (cut(think,i,"</set>")!="")
	{
		string set=cut(think,i,"</set>");
		string name=cut(set,0,">");
		name=cut(name,1,"name=");
		name=REPLACE(name,"\"","");
		string value=cut(set,1,">");
		EVAL(name+"="+value);
		i++;
	}
}

void EVAL(string str)
{
	string t=str;
	string name=cut(t,0,"=");
	string value=cut(t,1,"=");
	if (name=="topic")
		topic=value;
	//cout<<"topic="<<topic;
}


string RandAiTalk(void)
{
	
	string talk[]={
		//"���ð��� �һᱳʫŶ�����������롰���߲����������Ҿͻ���������һ�� ��ʲô��",
		"%username���ã��ٻ�����ǧ�����й��˵��������İٶ��ֳ���ʳ������ζ����Ч˵��Ŷ�������롰���Ŷ������򡰺��ҡ����ٻ��ͻ�����������Ŷ",
			"%username��ϲ�����ˣ�Ҫ��%selfname��ʲô�أ�",
			"%username����ҲҪ�úü����ء�",
			//"���롰\teach��ָ�����Խ�����ѧģʽ�����롰\talk��ָ��ضԻ�ģʽ";
			//"��Ҫ�ٻ�Ϊ�����������������롰������������˵�������򡰱�����100������";
			//Greetings;
	};
	int n=sizeof(talk)/sizeof(talk[0]);
	//cout<<n;
	string txt=talk[RAND(n)];
	return txt;
}



string TextToEnd(string str,string begin,int idx)         //TextToEnd("a=b=1234",'=',0)=="b=1234"�õ��ַ���"a=b=1234"�дӵ�1��"="���ַ����������ı�"b=1234"
{
	string s=str;
	//s=REPLACE(s,"\"","");
	string d;
	d = cut(s,idx,begin);
	while (cut(s,idx,begin)!="")
	{
        d+=begin+cut(s,idx+1,begin);
		idx++;
	}
	if (s.substr(strlen(s.c_str())-1,1)==begin)
		d+=begin;
	return d;
}


wstring UTF8ToUnicode(string str )
{
	int len = 0;
	len = str.length();
	int unicodeLen = ::MultiByteToWideChar( CP_UTF8,
		0,
		str.c_str(),
		-1,
		NULL,
		0 ); 
	wchar_t * pUnicode; 
	pUnicode = new wchar_t[unicodeLen+1]; 
	memset(pUnicode,0,(unicodeLen+1)*sizeof(wchar_t)); 
	::MultiByteToWideChar( CP_UTF8,
		0,
		str.c_str(),
        -1,
		(string)pUnicode,
		unicodeLen ); 
	wstring rt; 
	rt = ( wchar_t* )pUnicode;
	delete pUnicode; 
	return rt; 
}

string UnicodeToANSI( wstring str )
{
	char*     pElementText;
	int    iTextLen;
	// wide char to multi char
	iTextLen = WideCharToMultiByte( CP_ACP,
		0,
		str.c_str(),
		-1,
		NULL,
        0,
		NULL,
		NULL );
	pElementText = new char[iTextLen + 1];
	memset( ( void* )pElementText, 0, sizeof( char ) * ( iTextLen + 1 ) );
	::WideCharToMultiByte( CP_ACP,
		0,
		str.c_str(),
		-1,
		pElementText,
		iTextLen,
		NULL,
		NULL );
	string strText;
	strText = pElementText;
	delete[] pElementText;
	return strText;
}

string Utf8ToAnsi(const char *pstrUTF8)
{
	int i = 0;
	int j = 0;
	//   unsigned char strUnicode[200] = {0};
	char *pstrAnsi;
	wchar_t *strUnicode;
    i = MultiByteToWideChar(CP_UTF8, 0, pstrUTF8, -1, NULL, 0);
	
	//memset(strUnicode, 0, i);
	strUnicode=new wchar_t[i + 1];
	memset( ( void* )strUnicode, 0, sizeof( wchar_t ) * ( i + 1 ) );
	::MultiByteToWideChar(CP_UTF8, 0, pstrUTF8, -1, (string)strUnicode, i);
	j = WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, NULL, 0, NULL, NULL);
	pstrAnsi=new char[j+1];
	memset( ( void* )pstrAnsi, 0, sizeof( char ) * (j+ 1 ) );
	WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, pstrAnsi, j, NULL, NULL);
	string strText;
	strText=pstrAnsi;
	delete[] strUnicode;
	delete[] pstrAnsi;
	return strText;
}


string ToUTF8(const wchar_t* buffer, int len)
{
	int size = ::WideCharToMultiByte(CP_UTF8, 0, buffer, len, NULL, 0, NULL,
		NULL);
	if (size == 0)
		return "";
	
	std::string newbuffer;
	newbuffer.resize(size);
	::WideCharToMultiByte(CP_UTF8, 0, buffer, len,
		const_cast<char*>(newbuffer.c_str()), size, NULL, NULL);
	
	return newbuffer;
}

string ToUTF8(const std::wstring& str)
{
	return ToUTF8(str.c_str(), (int) str.size());
}


int GetTextType(const char *fPath)//��ȡָ��·���ļ��ı�����ʽ
{
	int nReturn = -1;
	unsigned char uniTxt[] = {0xFF, 0xFE};// Unicode file header
	unsigned char endianTxt[] = {0xFE, 0xFF};// Unicode big endian file header
	unsigned char utf8Txt[] = {0xEF, 0xBB};// UTF_8 file header
	DWORD dwBytesRead = 0;
	HANDLE hFile = CreateFile(fPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		hFile = NULL;
		CloseHandle(hFile);
		return -1;
	}
	BYTE *lpHeader = new BYTE[2];
	ReadFile(hFile, lpHeader, 2, &dwBytesRead, NULL);
	CloseHandle(hFile);
	if (lpHeader[0] == uniTxt[0] && lpHeader[1] == uniTxt[1])// Unicode file
		nReturn = 1;
	else if (lpHeader[0] == endianTxt[0] && lpHeader[1] == endianTxt[1])//  Unicode big endian file
		nReturn = 2;
	else if (lpHeader[0] == utf8Txt[0] && lpHeader[1] == utf8Txt[1])// UTF-8 file
		nReturn = 3;
	else
		nReturn = 0;   //Ascii
	delete []lpHeader;
	return nReturn;
}

LPSTR strcut(LPSTR old, LPSTR start, LPSTR end) 
{ 
    LPSTR strtmp1 = strstr(old, start); 
    LPSTR strtmp2 = strstr(old, end); 
    LPSTR newstr = new char[strtmp2-strtmp1+1]; 
    memset(newstr,0,(strtmp2-strtmp1+1)*sizeof(char));  
    return strncpy(newstr,strtmp1,strtmp2-strtmp1-1); 
} 



string TextBetween(string str,string start,string end,int idx)
{
	string dest;
	dest=cut(str,idx+1,start);
	dest=cut(dest,0,end);
	return dest;
}


string TextBetween2(string str,string start,string end)
{
	int len=str.length();
	string dest;
	LPSTR newstr =new char[len+1]; 
	memset(newstr,0,len*sizeof(char)); 
	strcpy(newstr,str.c_str()); 
	dest=strcut(newstr,"<SESSION>","</SESSION>");
	return dest;
}

void SPLIT2(const string& src ,vector<string> &tokens,const string& separator)
{
	string str=src;
	string::size_type start=0,index;
	str=REPLACE(str,"<","[");
	str=REPLACE(str," >","]");
	string delimiters;
	delimiters=REPLACE(separator,"<","[");
	delimiters=REPLACE(delimiters," >","]");
	
	do
	{
		index=str.find(delimiters,start);
		
		if (index!=string::npos)
		{
			tokens.push_back(str.substr(start,index-start));
			start = str.find(delimiters, index);
			if(start==string::npos) return;
			
			//cout<<str.substr(start,index-start)<<"-";
		}
	}
	while (index!=string::npos);
	
	//cout<<str;
	// Found a token, add it to the vector.
	tokens.push_back(str.substr(start));
	//cout<<str.substr(start)<<"-";
    
}


void SPLIT(const string &src, vector<string> &tokens,const string& separator)
{
	string pSeparator;char *pStart, *pEnd;
	char *pTemp;
	unsigned int sep_len;
	int count = 0;
	int len;
	len=strlen(src.c_str());
	//if (src=="")
	//tokens.push_back("");
	
	pSeparator =separator;
	if (pSeparator == "") return;
	
	if (separator == "" || strlen(separator.c_str()) == 0)pSeparator=" ";/* one blank by default */	
	sep_len = strlen(pSeparator.c_str());
	pTemp=new char[len+1];
	strcpy(pTemp,src.c_str());
	pStart = pTemp;
	
	while(1)
	{
		pEnd = strstr(pStart, pSeparator.c_str());
		if (pEnd != NULL)
		{
			memset(pEnd,'\0',sep_len);
			string temp=pStart;
			tokens.push_back(temp);
			pEnd = pEnd + sep_len;
			pStart = pEnd;
		}
		else
		{
			string temp=pStart;
			tokens.push_back(temp);
			break;
		}
	}
	delete []pTemp;
}

void SPLIT3(const string& str ,vector<string> &tokens,const string& delimiters)
{
    // Skip delimiters at beginning.
    int lastPos = str.find_first_not_of(delimiters, 0);
	cout<<lastPos;
    // Find first "non-delimiter".
    int pos= str.find(delimiters,0);
	cout<<pos;
	if (str=="")
		tokens.push_back("");
	if (lastPos!=string::npos||pos!=string::npos)
	{
		tokens.push_back(str.substr(0,pos));
		lastPos = str.find_first_not_of(delimiters, pos);
		
		pos =str.find(delimiters, lastPos);
		//	cout<<lastPos<<"-"<<pos;
	}
    while (string::npos != pos || string::npos != lastPos)
    {
		//cout<<str;
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
		//	cout<<str.substr(lastPos, pos - lastPos);
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find(delimiters, lastPos);
    }
	
	
	
	
	
}

string cut(string str,int n,string delimiters)
{
	vector<string> tokens;
	int num=0;
    SPLIT(str,tokens,delimiters);
	num=tokens.size();
	if (n+1>num)
		return "";
	else
		return tokens[n];
	
}

void FENUM(string path,vector<string> &file)   
{   
	struct _finddata_t   filefind;   
	string  curr=path+"\\*.*";   
	int   done=0,i,handle;   
	if((handle=_findfirst(curr.c_str(),&filefind))==-1)return;     
	while(!(done=_findnext(handle,&filefind)))   
	{   	
		//printf("%s\n",filefind.name);    
		if(!strcmp(filefind.name,"..")){
			continue;
		}   	    	
		if(_A_SUBDIR==filefind.attrib) //��Ŀ¼
		{           
			//printf("----------%s\n",filefind.name);    
			//cout<<filefind.name<<"(dir)"<<endl; 
			string dirend="\\";
			//filefind.name=filefind.name+"\";
			file.push_back(filefind.name+dirend);
			//curr=path+"\\"+filefind.name;   
		}   
		else//����Ŀ¼�����ļ�     
		{   
			//cout<<path+"\\"+filefind.name<<endl; 
			file.push_back(path+"\\"+filefind.name);
		}   
	}           
	_findclose(handle); 
}   

void SPLITPATH(const char *path, vector<string> &array)
{
	char drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
	_splitpath(path, drive, dir, fname, ext);
	array.push_back(drive);
	array.push_back(dir);
	array.push_back(fname);
	array.push_back(ext);
}


string REPLACE(string str, const char *before, const char *after)
{
	if (!after) 	{ after = ""; }
	
	size_t sz_bef = strlen(before);
	size_t sz_aft = strlen(after);
	for(size_t rp_pos = 0; ; rp_pos += sz_aft) {
		rp_pos = str.find(before, rp_pos);
		if (rp_pos == string::npos)
			break;
		str.replace(rp_pos, sz_bef, after);
	}
	return str;
}





