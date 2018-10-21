#include <iostream>
#include <cstring>        // for strcpy(), strcat()
#include <io.h>
#include <vector>
#include <regex>
#include <string>
#include <fstream>

using namespace std;
struct Files
{
    string FullName; //全名=文件名.扩展名
    string FilesName; //文件名
    string ExtensionName; //扩展名
};

struct SupportName
{
    string SoftwareName;
    string ExtensionName;
};

vector<Files> FilesInfo;
vector<SupportName> SupportList;
string mintedSet;

void ListFiles();
void SetSupportList();
void LoadMintedSet();
void MakeTempDir();
void DealWithFile();

int main()
{
    cout<<"FuckCodePrint (Code to PDF By Latex)"<<endl;
    cout<<"Author:CHN-Student<chn-student@outlook.com>"<<endl;
    cout<<"Project:https://github.com/CHN-STUDENT/fuckcodeprint"<<endl;
    printf("Please wait.\n");
    cout<<"--------------------------------------------------------------------"<<endl;
    SetSupportList();//读取支持的代码设置文件
    ListFiles();//获取源码目录要处理的所有文件
    DealWithFile();
    system("pause");
    return 0;
}

void DealWithFile()
{
    vector<SupportName>::const_iterator SupportListIterator = SupportList.begin();
    SupportName S;
    static bool once = true;
    static bool normal = false;
    char srcpath[256]="";
    _getcwd(srcpath,256);//得到当前路径
    strcat(srcpath, "\\src");
    char tempsrcpath[256]="";
    _getcwd(tempsrcpath,256);//得到当前路径
    strcat(tempsrcpath, "\\temp\\src");
    char copy_command[600]="";
    fstream templatefile;
    fstream tempfile;
    char temp[1024];
    char templatefilepath[256]="";
    _getcwd(templatefilepath,256);//得到当前路径
    strcat(templatefilepath, "\\temp\\template.tex");
    char tempfilepath[256]="";
    _getcwd(tempfilepath,256);//得到当前路径
    strcat(tempfilepath, "\\temp\\temp.tex");
    while(!FilesInfo.empty())
    {//对每一个文件处理
        Files F;
        F=FilesInfo.back();
        while(SupportListIterator!=SupportList.end())
        {
            S=(*SupportListIterator);//取迭代器所指的内容
            if(S.ExtensionName==F.ExtensionName)
            {
                if(once)
                {
                    cout<<"--------------------------------------------------------------------"<<endl;
                    MakeTempDir(); //创建临时工作文件夹
                    LoadMintedSet();//读取minted设置
                    once=false;
                    templatefile.open(templatefilepath,ios::in);
                    tempfile.open(tempfilepath,ios::out);
                    //按行读取模板文件读取到标识符<%This is the locator to add your file, don't edit it.>那一行将读取文件重新定向写入到temp.tex
                     while(!templatefile.eof())
                    {
                        templatefile.getline(temp,1024,'\n');
                        //cout<<temp<<endl;
                        if(strcmp(temp,"%This is the locator to add your file, don't edit it.")==0)
                        {
                            normal=true;
                            break;
                        }
                        tempfile<<temp<<endl;
                    }
                    if(!normal)
                    {
                        cout<<"--------------------------------------------------------------------"<<endl;
                        cout<<"Error:you must the locator in your template file.Exit!"<<endl;
                        tempfile.close();
                        templatefile.close();
                        printf("Delete the temp dir.");
                        char remove_command[300]="";
                        char temppath[256]="";
                        _getcwd(temppath,256);//得到当前路径
                        strcat(temppath,"\\temp");
                        strcat(remove_command,"rd /S/Q ");
                        strcat(remove_command,temppath);
                        system(remove_command);
                        exit(0);
                    }
                }
                cout<<"Find '"<<F.FullName<<"' is support and add it. >> ";
                strcat(copy_command,"xcopy /Q ");
                strcat(copy_command,srcpath);
                strcat(copy_command,"\\");
                strcat(copy_command,F.FullName.c_str());
                strcat(copy_command," ");
                strcat(copy_command,tempsrcpath);
                //cout<<copy_command<<endl;
                system(copy_command);
                memset(copy_command,0,sizeof(copy_command));
                tempfile<<"\\section{"<<F.FullName<<"}"<<endl;
                tempfile<<"\\inputminted["<<mintedSet<<"]{"<<S.SoftwareName<<"}{src/"<<F.FullName<<"}"<<endl;

            }
            SupportListIterator++;//迭代器指针+1
        }
        SupportListIterator = SupportList.begin(); //重置迭代器指针
        FilesInfo.pop_back();//出栈
    }
    if(!once)
    {
         while(!templatefile.eof())
        {
            templatefile.getline(temp,1024,'\n');
            //cout<<temp<<endl;
            tempfile<<temp<<endl;
        }
        cout<<"--------------------------------------------------------------------"<<endl;
        cout<<"Start to use XeLatex to bulid."<<endl;
        cout<<"--------------------------------------------------------------------"<<endl;
        char temppath[256]="";
        char path[256]="";
        _getcwd(path,256);//得到当前路径
        strcat(temppath,path);
        strcat(temppath,"\\temp");
        char tex_command[300]="cd /D";
        strcat(tex_command,temppath);
        strcat(tex_command," && xelatex.exe -synctex=1 -interaction=nonstopmode  -8bit --shell-escape \"temp\".tex ");
        system(tex_command); //进入工作目录并且编译
        //复制编译好PDF出来
        cout<<"--------------------------------------------------------------------"<<endl;
        cout<<"Copy the output file."<<endl;
        char copy_command[600]="copy /y ";
        strcat(copy_command,temppath);
        strcat(copy_command,"\\temp.pdf");
        strcat(copy_command," ");
        strcat(copy_command,path);
        strcat(copy_command,"\\output.pdf");
       // cout<<copy_command<<endl;
        system(copy_command);
        memset(copy_command,0,sizeof(copy_command));
        strcat(copy_command,"copy /y ");
        strcat(copy_command,temppath);
        strcat(copy_command,"\\temp.tex");
        strcat(copy_command," ");
        strcat(copy_command,path);
        strcat(copy_command,"\\output.tex");
         system(copy_command);
        cout<<"--------------------------------------------------------------------"<<endl;
        tempfile.close();
        templatefile.close();
        printf("Delete the temp dir.");
        char remove_command[300]="";
        strcat(remove_command,"rd /S/Q ");
        strcat(remove_command,temppath);
        system(remove_command);
    }
}


void MakeTempDir()
{
    char path[256];
    char temppath[256];

    _getcwd(path,256);//得到当前路径
    _getcwd(temppath,256);//得到当前路径
    strcat(temppath, "\\temp");

    printf("Create the temp dir.");
    char create_command[300]="";
    strcat(create_command,"md ");
    strcat(create_command,temppath);
    system(create_command);
    strcat(create_command,"\\src");
    system(create_command);

    printf("\nCopy the latex file. >> "); //把模板文件复制进去
    char copy_command[600]="";
    strcat(copy_command,"xcopy /Q ");
    strcat(copy_command,path);
    strcat(copy_command,"\\latex\\* ");
    strcat(copy_command,temppath);
    //cout<<copy_command;
    system(copy_command);
    cout<<"--------------------------------------------------------------------"<<endl;
}

void LoadMintedSet()
{
    ifstream in("minted_set.txt");
    ostringstream tmp;
    tmp << in.rdbuf();
    mintedSet = tmp.str();
    in.close();
}

void SetSupportList()
{
    cout<<"Read the code set file and make the support list."<<endl;
    fstream setfile;
    char temp[1024];
    setfile.open("code_set.txt",ios::in);
    SupportName s;
    while(!setfile.eof())
    {
        setfile.getline(temp,1024,'\n');
        //正则表达式，用来处理文件
        cmatch reslut;
        regex FileRegex("([^/]*):(\\w+)$");
        regex_search(temp,reslut,FileRegex);
        s.SoftwareName=reslut.str(1);
        s.ExtensionName=reslut.str(2);
        //cout<<s.SoftwareName<<" "<<s.ExtensionName;
        SupportList.push_back(s);
    }
    setfile.close();
}

void ListFiles()
{
    cout<<"Read the your code files from src folder."<<endl;
    cout<<"--------------------------------------------------------------------"<<endl;
    char path[256];
    _getcwd(path,256);//得到当前路径
    strcat(path, "\\src\\*.*");    // 在目录后面加上"\\*.*"进行第一次搜索

    intptr_t handle;
    _finddata_t findData;

    handle = _findfirst(path, &findData);
    if (handle == -1)        // 检查是否成功
        return;
    //正则表达式，用来处理文件名
    cmatch reslut;
    regex FileRegex("([^/]*)\\.(\\w+)$");
    Files f;
    do
    {

        if (findData.attrib & _A_SUBDIR)
        {
            if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
                continue;
        }
        else
        {
            regex_search(findData.name,reslut,FileRegex);
            f.FullName=reslut.str();
            f.FilesName=reslut.str(1);
            f.ExtensionName=reslut.str(2);
            cout<<"Find '"<<f.FullName<<"'."<<endl;
            //cout<<f.ExtensionName;
            FilesInfo.push_back(f);
            //FilesName.push_back(findData.name);
        }
    } while (_findnext(handle, &findData) == 0);

    _findclose(handle);    // 关闭搜索句柄
}
