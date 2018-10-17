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

int main()
{
    printf("Please wait.\n");
    MakeTempDir();
    SetSupportList();//读取支持的代码设置文件
    ListFiles();//获取源码目录要处理的所有文件
    LoadMintedSet();//读取minted设置
    return 0;
}

void MakeTempDir()
{
    char path[256];
    char temppath[256];

    _getcwd(path,256);//得到当前路径
    _getcwd(temppath,256);//得到当前路径
    strcat(temppath, "\\temp");

    printf("Delete the temp dir.");
    char remove_command[300]="";
    strcat(remove_command,"rd /S/Q ");
    strcat(remove_command,temppath);
    system(remove_command);

    printf("\nCreate the temp dir.");
    char create_command[300]="";
    strcat(create_command,"md ");
    strcat(create_command,temppath);
    system(create_command);

    printf("\nCopy the latex template."); //把模板文件复制进去
    char copy_command[600]="";
    strcat(copy_command,"xcopy ");
    strcat(copy_command,path);
    strcat(copy_command,"\\template.tex ");
    strcat(copy_command,temppath);
    //cout<<copy_command;
    system(copy_command);
    print("\n");
}

void LoadMintedSet()
{
    ifstream in("minted_set.txt");
    ostringstream tmp;
    tmp << in.rdbuf();
    mintedSet = tmp.str();
}

void SetSupportList()
{
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
            //cout<<f.ExtensionName;
            FilesInfo.push_back(f);
            //FilesName.push_back(findData.name);
        }
    } while (_findnext(handle, &findData) == 0);

    _findclose(handle);    // 关闭搜索句柄
}
