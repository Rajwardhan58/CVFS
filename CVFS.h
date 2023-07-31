#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>




using namespace std;

#define MAXINODE 50

#define READ 1
#define WRITE 2

#define MAXFILESIZE 2048

#define REGULAR 1
#define SPECIAL 2

#define START 0
#define CURRENT 1
#define END 2

typedef struct superblock
{
    int TotalInodes;
    int FreeInode;
}SUPERBLOCK,*PSUPERBLOCK;

typedef struct inode
{
    char FileName[50];
    int InodeNumber;
    int FileSize;
    int FileActualSize;
    int FileType;
    char *Buffer;
    int LinkCount;
    int ReferenceCount;
    int permission;
    struct inode *next;
}INODE,*PINODE,**PPINODE;

typedef struct FileTable
{
    int readoffset;
    int writeoffset;
    int count;
    int mode;
    PINODE ptrinode;
}FILETABLE,*PFILETABLE;

typedef struct ufdt
{
    PFILETABLE ptrfiletable;
}UFDT;

UFDT UFDTArr[50];
SUPERBLOCK SUPERBLOCKobj;
PINODE head=NULL;

void man(char *);
void DisplayHelp();
int GetFDFromName(char *);
PINODE Get_Inode(char *);
void CreateDILB();
void InitialiseSuperBlock();
int CreateFile(char *,int);
int rm_File(char *);
int ReadFile(int,char *,int);
int WriteFile(int,char *,int);
int OpenFile(char *,int);
void CloseFileByName(int);
int CloseFileByName(char *);
void CloseAllFile();
int LseekFile(int,int,int);
void ls_file();
int fstat_file(int);
int stat_file(char *);
int truncate_File(char *);
void SaveFiles();







