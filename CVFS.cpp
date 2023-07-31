#include "CVFS.h"

/////////////////////////////////////////////////////////////////
// Name of function : man
// Input Parameter  : Address 
// Return value     : void
// Description      : used to display user manual of any command 
/////////////////////////////////////////////////////////////////


void man(char *name)
{
    if(name==NULL) return;

    if(strcmp(name,"create")==0)
    {
        cout<<"Description : Used to create new regular file"<<endl;
        cout<<"Usage : Create File_name Permission"<<endl;
    }
    else if(strcmp(name,"read")==0)
    {
        cout<<"Description : Used to read data from regular file"<<endl;
        cout<<"Usage : read File_name No_Of_Bytes_To_Read"<<endl;
    }
    else if(strcmp(name,"write")==0)
    {
        cout<<"Description : Used to  write into regular file"<<endl;
        cout<<"Usage : write File_name\n After this Enter the data that we want to write"<<endl;
    }
    else if(strcmp(name,"ls")==0)
    {
        cout<<"Description : used to list all information of files"<<endl;
        cout<<"Usages : ls"<<endl;
    }
    else if(strcmp(name,"stat")==0)
    {
        cout<<"Description : Used to display information of file"<<endl;
        cout<<"Usage : stat File_name"<<endl;
    }
    else if(strcmp(name,"fstat")==0)
    {
        cout<<"Description : Used to display information of file"<<endl;
        cout<<"Usage : stat File_Descriptor"<<endl;
    }
    else if(strcmp(name,"truncate")==0)
    {
        cout<<"Description : Used to remove data from file"<<endl;
        cout<<"Usage : truncate File_name"<<endl;
    }
    else if(strcmp(name,"open")==0)
    {
        cout<<"Description : Used to open existing file"<<endl;
        cout<<"Usage : open File_name"<<endl;
    }
    else if(strcmp(name,"close")==0)
    {
        cout<<"Description : Used to close opened file"<<endl;
        cout<<"Usage : close File_name"<<endl;
    }
    else if(strcmp(name,"closeall")==0)
    {
        cout<<"Description : Used to close all opened file"<<endl;
        cout<<"Usage : closeall"<<endl;
    }
    else if(strcmp(name,"lseek")==0)
    {
        cout<<"Description : Used to change file offset"<<endl;
        cout<<"Usage : lseek File_Name ChangeInOffset StartPoint"<<endl;
    }
    else if(strcmp(name,"rm")==0)
    {
        cout<<"Description : Used to delete the file"<<endl;
        cout<<"Usage : rm File_name"<<endl;
    }
    else
    {
        cout<<"ERROR : No manual entry available"<<endl;
    }
}


/////////////////////////////////////////////////////////////////
// Name of function : DisplayHelp
// Input Parameter  : no parameter
// Return value     : void
// Description      : used to display commands with description 
/////////////////////////////////////////////////////////////////

void DisplayHelp()
{
    cout<<"ls       : To List out all files"<<endl;
    cout<<"clear    : To Clear console"<<endl;
    cout<<"create   : To create a new file"<<endl;
    cout<<"open     : To Open the files"<<endl;
    cout<<"close    : To Close the files"<<endl;
    cout<<"closeall : To Close all opened files"<<endl;
    cout<<"read     : To Read the contents from file"<<endl;
    cout<<"write    : To Write contents into file"<<endl;
    cout<<"exit     : To Terminate file system"<<endl;
    cout<<"stat     : To Display information of file using name"<<endl;
    cout<<"fstat    : To Display information of file using file descriptor"<<endl;
    cout<<"truncate : To Remove all data from file"<<endl;
    cout<<"rm       : To Delete the file"<<endl;
}

int GetFDFromName(char *name)
{
    int i=0;

    while(i<50)
    {
        if(UFDTArr[i].ptrfiletable!=NULL)
            if(strcmp((UFDTArr[i].ptrfiletable->ptrinode->FileName),name)==0)
                break;
        i++;
    }

    if(i==50)   return -1;
    else        return i;
}

PINODE Get_Inode(char *name)
{
    PINODE temp=head;
    int i=0;

    if(name==NULL)
        return NULL;

    while(temp!=NULL)
    {
        if(strcmp(name,temp->FileName)==0)
            break;
        temp=temp->next;
    }
    return temp;
}


/////////////////////////////////////////////////////////////////
// Name of function : CreateDILB
// Input Parameter  : no parameter 
// Return value     : void
// Description      : used to create Disk inode list block 
/////////////////////////////////////////////////////////////////
void CreateDILB()
{
    int i=1;
    PINODE newn=NULL;
    PINODE temp=head;

    while(i<=MAXINODE)
    {
        newn=(PINODE)malloc(sizeof(INODE));

        newn->LinkCount=0;
        newn->ReferenceCount=0;
        newn->FileType=0;
        newn->FileSize=0;

        newn->Buffer=NULL;
        newn->next=NULL;

        newn->InodeNumber=i;

        if(temp==NULL)
        {
            head=newn;
            temp=head;
        }
        else
        {
            temp->next=newn;
            temp=temp->next;
        }
        i++;
    }
    cout<<"DILB created successfully"<<endl;
}

/////////////////////////////////////////////////////////////////
// Name of function : InitialiseSuperBlock
// Input Parameter  : no parameter 
// Return value     : void
// Description      : used to Initialise super block 
/////////////////////////////////////////////////////////////////
void InitialiseSuperBlock()
{
    int i=0;
    while(i<MAXINODE)
    {
        UFDTArr[i].ptrfiletable=NULL;
        i++;
    }

    SUPERBLOCKobj.TotalInodes=MAXINODE;
    SUPERBLOCKobj.FreeInode=MAXINODE;
}


/////////////////////////////////////////////////////////////////
// Name of function : CreateFile
// Input Parameter  : address,int variable
// Return value     : int
// Description      : used to Create new file
/////////////////////////////////////////////////////////////////

int CreateFile(char *name,int permission)
{
    int i=0;
    PINODE temp=head;

    if((name==NULL)||(permission==0)||(permission>3))
        return -1;

    if(SUPERBLOCKobj.FreeInode==0)
        return -2;
    
    (SUPERBLOCKobj.FreeInode)--;

    if(Get_Inode(name)!=NULL)
        return -3;

    while(temp!=NULL)
    {
        if(temp->FileType==0)
            break;
        temp=temp->next;
    }

    while(i<50)
    {
        if(UFDTArr[i].ptrfiletable==NULL)
            break;
        i++;
    }

    UFDTArr[i].ptrfiletable=(PFILETABLE)malloc(sizeof(FILETABLE));

    UFDTArr[i].ptrfiletable->count          = 1;
    UFDTArr[i].ptrfiletable->mode           = permission;
    UFDTArr[i].ptrfiletable->readoffset     = 0;
    UFDTArr[i].ptrfiletable->writeoffset    = 0;

    UFDTArr[i].ptrfiletable->ptrinode       = temp;

    strcpy(UFDTArr[i].ptrfiletable->ptrinode->FileName,name);
    UFDTArr[i].ptrfiletable->ptrinode->FileType         = REGULAR;
    UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount   = 1;
    UFDTArr[i].ptrfiletable->ptrinode->LinkCount        = 1;
    UFDTArr[i].ptrfiletable->ptrinode->FileSize         = MAXFILESIZE;
    UFDTArr[i].ptrfiletable->ptrinode->FileActualSize   = 0;
    UFDTArr[i].ptrfiletable->ptrinode->permission       = permission;
    UFDTArr[i].ptrfiletable->ptrinode->Buffer=(char *)malloc(MAXFILESIZE);

    return i;
}


/////////////////////////////////////////////////////////////////
// Name of function : rm
// Input Parameter  : Address 
// Return value     : int
// Description      : used to remove file 
/////////////////////////////////////////////////////////////////

int rm_File(char *name)
{
    int fd=0;

    fd=GetFDFromName(name);
    if(fd==-1)
        return -1;

    (UFDTArr[fd].ptrfiletable->ptrinode->LinkCount)--;

    if(UFDTArr[fd].ptrfiletable->ptrinode->LinkCount==0)
    {
        UFDTArr[fd].ptrfiletable->ptrinode->FileType=0;
        //free(UFDTArr[fd].ptrfiletable->ptrinode->Buffer);
        free(UFDTArr[fd].ptrfiletable);
    }

    UFDTArr[fd].ptrfiletable=NULL;
    (SUPERBLOCKobj.FreeInode)++;
}


/////////////////////////////////////////////////////////////////
// Name of function : ReadFile
// Input Parameter  : int ,Address,int 
// Return value     : int
// Description      : used to Read the file 
/////////////////////////////////////////////////////////////////

int ReadFile(int fd,char *arr,int isize)
{
    int read_size=0;

    if(UFDTArr[fd].ptrfiletable==NULL)  return -1;

    if(UFDTArr[fd].ptrfiletable->mode!=READ && UFDTArr[fd].ptrfiletable->mode!=READ+WRITE)  return -2;

    if(UFDTArr[fd].ptrfiletable->ptrinode->permission!=READ && UFDTArr[fd].ptrfiletable->ptrinode->permission!=READ+WRITE)  return -2;

    if(UFDTArr[fd].ptrfiletable->readoffset==UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)    return -3;

    if(UFDTArr[fd].ptrfiletable->ptrinode->FileType!=REGULAR)   return -4;

    read_size=(UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)-(UFDTArr[fd].ptrfiletable->readoffset);
    if(read_size<0) return -3;
    
    if(read_size<isize)
    {
        strncpy(arr,(UFDTArr[fd].ptrfiletable->ptrinode->Buffer)+(UFDTArr[fd].ptrfiletable->readoffset),read_size);

        UFDTArr[fd].ptrfiletable->readoffset=UFDTArr[fd].ptrfiletable->readoffset+read_size;
    }
    else
    {
        strncpy(arr,(UFDTArr[fd].ptrfiletable->ptrinode->Buffer)+(UFDTArr[fd].ptrfiletable->readoffset),isize);

        (UFDTArr[fd].ptrfiletable->readoffset)=(UFDTArr[fd].ptrfiletable->readoffset)+isize;
    }

    return isize;
}
/////////////////////////////////////////////////////////////////
// Name of function : WriteFile
// Input Parameter  : int ,Address,int 
// Return value     : int
// Description      : used to write in the file 
/////////////////////////////////////////////////////////////////

int WriteFile(int fd,char *arr,int isize)
{
    if(((UFDTArr[fd].ptrfiletable->mode)!=WRITE) && ((UFDTArr[fd].ptrfiletable->mode)!=READ+WRITE))     return -1;

    if(((UFDTArr[fd].ptrfiletable->ptrinode->permission)!=WRITE) && ((UFDTArr[fd].ptrfiletable->ptrinode->permission)!=READ+WRITE))  return -1;

    if((UFDTArr[fd].ptrfiletable->writeoffset)==MAXFILESIZE)    return -2;

    if((UFDTArr[fd].ptrfiletable->ptrinode->FileType)!=REGULAR)     return -2;

    strncpy((UFDTArr[fd].ptrfiletable->ptrinode->Buffer)+(UFDTArr[fd].ptrfiletable->writeoffset),arr,isize);

    (UFDTArr[fd].ptrfiletable->writeoffset)=(UFDTArr[fd].ptrfiletable->writeoffset)+isize;

    (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)=(UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)+isize;

    return isize;
}

/////////////////////////////////////////////////////////////////
// Name of function : OpenFile
// Input Parameter  : Address,int 
// Return value     : int
// Description      : used to open the file 
/////////////////////////////////////////////////////////////////


int OpenFile(char *name,int mode)
{
    int i=0;
    PINODE temp=NULL;

    if(name==NULL || mode<=0)
        return -1;
    
    temp=Get_Inode(name);
    if(temp==NULL)
        return -2;

    if(temp->permission<mode)
        return -3;

    while(i<50)
    {
        if(UFDTArr[i].ptrfiletable==NULL)
            break;
        i++;
    }

    UFDTArr[i].ptrfiletable=(PFILETABLE)malloc(sizeof(FILETABLE));
    if(UFDTArr[i].ptrfiletable==NULL)   return -1;
    UFDTArr[i].ptrfiletable->count=1;
    UFDTArr[i].ptrfiletable->mode=mode;
    if(mode==READ+WRITE)
    {
        UFDTArr[i].ptrfiletable->readoffset=0;
        UFDTArr[i].ptrfiletable->writeoffset=0;
    }
    else if(mode==READ)
    {
        UFDTArr[i].ptrfiletable->readoffset=0;
    }
    else if(mode==WRITE)
    {
        UFDTArr[i].ptrfiletable->writeoffset=0;
    }
    UFDTArr[i].ptrfiletable->ptrinode=temp;
    (UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)++;

    return i;
}

/////////////////////////////////////////////////////////////////
// Name of function : CloseFileByName
// Input Parameter  : int  
// Return value     : void
// Description      : used to close the file by name 
/////////////////////////////////////////////////////////////////

void CloseFileByName(int fd)
{
    UFDTArr[fd].ptrfiletable->readoffset=0;
    UFDTArr[fd].ptrfiletable->writeoffset=0;
    (UFDTArr[fd].ptrfiletable->ptrinode->ReferenceCount)--;
}

/////////////////////////////////////////////////////////////////
// Name of function : CloseFileByName
// Input Parameter  : int  
// Return value     : int
// Description      : used to close the file by name 
/////////////////////////////////////////////////////////////////


int CloseFileByName(char *name)
{
    int i=0;
    i=GetFDFromName(name);
    if(i==-1)
        return -1;
    
    UFDTArr[i].ptrfiletable->readoffset=0;
    UFDTArr[i].ptrfiletable->writeoffset=0;
    (UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)--;

    return 0;
}

/////////////////////////////////////////////////////////////////
// Name of function : CloseAllFile
// Input Parameter  : none  
// Return value     : void
// Description      : used to close the all files 
/////////////////////////////////////////////////////////////////

void CloseAllFile()
{
    int i=0;
    while(i<50)
    {
        if(UFDTArr[i].ptrfiletable!=NULL)
        {
            UFDTArr[i].ptrfiletable->readoffset=0;
            UFDTArr[i].ptrfiletable->writeoffset=0;
            (UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)--;
            break;
        }
        i++;
    }
}

/////////////////////////////////////////////////////////////////
// Name of function : LseekFile
// Input Parameter  : int,int,int  
// Return value     : int
// Description      : used to repositions read/write file offset 
/////////////////////////////////////////////////////////////////


int LseekFile(int fd,int size,int from)
{
    if((fd<0)||(from>2))    return -1;
    if(UFDTArr[fd].ptrfiletable==NULL)  return -1;

    if((UFDTArr[fd].ptrfiletable->mode==READ)||(UFDTArr[fd].ptrfiletable->mode==READ+WRITE))
    {
        if(from==CURRENT)
        {
            if(((UFDTArr[fd].ptrfiletable->readoffset)+size)>UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)       return -1;
            cout<<((UFDTArr[fd].ptrfiletable->readoffset)+size)<<endl;
            if(((UFDTArr[fd].ptrfiletable->readoffset)+size)<0) return -1;
            (UFDTArr[fd].ptrfiletable->readoffset)=(UFDTArr[fd].ptrfiletable->readoffset)+size;
        }
        else if(from==START)
        {
            if(size>(UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize))   return -1;
            if(size<0)  return -1;
            (UFDTArr[fd].ptrfiletable->readoffset)=size;
        }
        else if(from==END)
        {
            if((UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)+size>MAXFILESIZE)       return -1;
            if(((UFDTArr[fd].ptrfiletable->readoffset)+size)<0) return -1;
            (UFDTArr[fd].ptrfiletable->readoffset)=(UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)+size;
        }
    }
    else if(UFDTArr[fd].ptrfiletable->mode==WRITE)
    {
        if(from==CURRENT)
        {
            if(((UFDTArr[fd].ptrfiletable->writeoffset)+size)>MAXFILESIZE)  return -1;
            if(((UFDTArr[fd].ptrfiletable->writeoffset)+size)<0)    return -1;
            if(((UFDTArr[fd].ptrfiletable->writeoffset)+size)>(UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize))
                (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)=(UFDTArr[fd].ptrfiletable->writeoffset)+size;
            (UFDTArr[fd].ptrfiletable->writeoffset)=(UFDTArr[fd].ptrfiletable->writeoffset)+size;
        }
        else if(from==START)
        {
            if(size>MAXFILESIZE)    return -1;
            if(size<0)  return -1;
            if(size>(UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize))
                (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)=size;
            (UFDTArr[fd].ptrfiletable->writeoffset)=size;
        }
        else if(from==END)
        {
            if((UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)+size>MAXFILESIZE)   return -1;
            if(((UFDTArr[fd].ptrfiletable->ptrinode)+size)<0)   return -1;
            (UFDTArr[fd].ptrfiletable->writeoffset)=(UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)+size;
        }
    }
}

/////////////////////////////////////////////////////////////////
// Name of function : ls_file
// Input Parameter  : none  
// Return value     : void
// Description      : used to list files
/////////////////////////////////////////////////////////////////


void ls_file()
{
    int i=0;
    PINODE temp=head;

    if(SUPERBLOCKobj.FreeInode==MAXINODE)
    {
        cout<<"ERROR : There are no files"<<endl;
        return;
    }

    cout<<"File Name\tInode Number\tFile size\tLink Count"<<endl;
    cout<<"---------------------------------------------------------------"<<endl;
    while(temp!=NULL)
    {
        if(temp->FileType!=0)
        {
            cout<<temp->FileName<<"\t\t"<<temp->InodeNumber<<"\t\t"<<temp->FileActualSize<<"\t\t"<<temp->LinkCount<<endl;
        }
        temp=temp->next;
    }
    cout<<"----------------------------------------------------------------"<<endl;
}
/////////////////////////////////////////////////////////////////
// Name of function : fstat_file
// Input Parameter  : int  
// Return value     : int
// Description      : used to obtain information about open file 
/////////////////////////////////////////////////////////////////

int fstat_file(int fd)
{
    PINODE temp=head;
    int i=0;

    if(fd<0)    return -1;

    if(UFDTArr[fd].ptrfiletable==NULL)  return -2;

    temp=UFDTArr[fd].ptrfiletable->ptrinode;

    cout<<".............Statistics Information about file............."<<endl;
    cout<<"File name : "<<temp->FileName<<endl;
    cout<<"Inode Number : "<<temp->InodeNumber<<endl;
    cout<<"File size : "<<temp->FileSize<<endl;
    cout<<"Actual File size : "<<temp->FileActualSize<<endl;
    cout<<"Link count : "<<temp->LinkCount<<endl;
    cout<<"Reference Count : "<<temp->ReferenceCount<<endl;

    if(temp->permission==1)
        cout<<"File permission : Read only"<<endl;
    else if(temp->permission==2)
        cout<<"File permission : Write"<<endl;
    else if(temp->permission==3)
        cout<<"File permission : Read & Write"<<endl;
    cout<<"-----------------------------------------------------------------"<<endl;

    return 0;
}

/////////////////////////////////////////////////////////////////
// Name of function : stat_file
// Input Parameter  : int  
// Return value     : int
// Description      : used to display file attributes about an inode
/////////////////////////////////////////////////////////////////

int stat_file(char *name)
{
    PINODE temp=head;
    int i=0;

    if(name==NULL)  return -1;

    while(temp!=NULL)
    {
        if(strcmp(name,temp->FileName)==0)
            break;
        temp=temp->next;
    }

    if(temp==NULL) return -2;

    cout<<".............Statistics Information about file............."<<endl;
    cout<<"File name : "<<temp->FileName<<endl;
    cout<<"Inode Number : "<<temp->InodeNumber<<endl;
    cout<<"File size : "<<temp->FileSize<<endl;
    cout<<"Actual File size : "<<temp->FileActualSize<<endl;
    cout<<"Link count : "<<temp->LinkCount<<endl;
    cout<<"Reference Count : "<<temp->ReferenceCount<<endl;

    if(temp->permission==1)
        cout<<"File permission : Read only"<<endl;
    else if(temp->permission==2)
        cout<<"File permission : Write"<<endl;
    else if(temp->permission==3)
        cout<<"File permission : Read & Write"<<endl;
    cout<<"-----------------------------------------------------------------"<<endl;

    return 0;
}

/////////////////////////////////////////////////////////////////
// Name of function : truncate_File
// Input Parameter  : address  
// Return value     : int
// Description      : used to extend the size of file to given size 
/////////////////////////////////////////////////////////////////

int truncate_File(char *name)
{
    int fd=GetFDFromName(name);
    if(fd==-1)
        return -1;
    
    memset(UFDTArr[fd].ptrfiletable->ptrinode->Buffer,0,1024);
    UFDTArr[fd].ptrfiletable->readoffset=0;
    UFDTArr[fd].ptrfiletable->writeoffset=0;
    UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize=0;
}

/////////////////////////////////////////////////////////////////
// Name of function : SaveFiles
// Input Parameter  : NULL 
// Return value     : NULL
// Description      : used to save the files created in ram 
/////////////////////////////////////////////////////////////////
void SaveFiles()
{
    PINODE temp     = head;
    int    fd       = 0;
    char fileName[50]   = {'\0'}; 
    if(SUPERBLOCKobj.FreeInode==MAXINODE)  return;
   // char Buffer[MAXFILESIZE]={'\0'};
    if(temp==NULL) return;

    int result = mkdir("CVFSData", 0777);
    if (result == -1) {
        printf("Error creating directory");
        return;
    }

    while(temp!=NULL)
    {
        if(temp->FileType!=0)
        {
            sprintf(fileName,"CVFSData/%s",temp->FileName);
           fd=creat(fileName,0777);
           if(fd<0)
           {
            continue;
           }
          // strcpy(Buffer,temp->Buffer);
           write(fd,temp->Buffer,strlen(temp->Buffer));
           memset(temp->Buffer,'\0',sizeof(temp->Buffer));

        }
        temp=temp->next;
    }
    cout<<"Successfully Saved file"<<endl;
}

int main()
{
    char *ptr=NULL;
    int ret=0,fd=0,count=0;
    char command[4][80],str[80],arr[1024];

    InitialiseSuperBlock();
    CreateDILB();

    while(1)
    {
        fflush(stdin);
        strcpy(str,"");

        cout<<"Marvellous VFS : >";

        fgets(str,80,stdin);

        count=sscanf(str,"%s%s%s%s",command[0],command[1],command[2],command[3]);

        if(count==1)
        {
            if(strcmp(command[0],"ls")==0)
            {
                ls_file();
            }
            else if(strcmp(command[0],"closeall")==0)
            {
                CloseAllFile();
                cout<<"All files closed successfully"<<endl;
                continue;
            }
            else if(strcmp(command[0],"clear")==0)
            {
                system("cls");
                continue;
            }
            else if(strcmp(command[0],"help")==0)
            {
                DisplayHelp();
                continue;
            }
            else if(strcmp(command[0],"exit")==0)
            {
                SaveFiles();
                cout<<"Terminating the marvellous virtual file system"<<endl;
                break;
            }
            else
            {
                cout<<"ERROR : Command not found!!!"<<endl;
                continue;
            }
        }
        else if(count==2)
        {
            if(strcmp(command[0],"stat")==0)
            {
                ret=stat_file(command[1]);
                if(ret==-1)
                    cout<<"ERROR : Incorrect parameter"<<endl;
                if(ret==-2)
                    cout<<"ERROR : There is no such file"<<endl;
                continue;
            }
            else if(strcmp(command[0],"fstat")==0)
            {
                ret=fstat_file(atoi(command[1]));
                if(ret==-1)
                    cout<<"ERROR : Incorrect parameter"<<endl;
                if(ret==-2)
                    cout<<"ERROR : There is no such file"<<endl;
                continue;
            }
            else if(strcmp(command[0],"close")==0)
            {
                ret=CloseFileByName(command[1]);
                if(ret==-1)
                    cout<<"ERROR : There is no such file"<<endl;
                continue;
            }
            else if(strcmp(command[0],"rm")==0)
            {
                ret=rm_File(command[1]);
                if(ret==-1)
                    cout<<"ERROR : There is no such file"<<endl;
                continue;
            }
            else if(strcmp(command[0],"man")==0)
            {
                man(command[1]);
            }
            else if(strcmp(command[0],"write")==0)
            {
                fd=GetFDFromName(command[1]);
                if(fd==-1)
                {
                    cout<<"ERROR : Incorrect parameter"<<endl;
                    continue;
                }
                cout<<"Enter the data : "<<endl;
                scanf("%[^'\n']s",arr);

                ret=strlen(arr);
                if(ret==0)
                {
                    cout<<"Error : Incorrect parameter"<<endl;
                    continue;
                }
                ret=WriteFile(fd,arr,ret);
                if(ret==-1)
                    cout<<"ERROR : Permission Denied"<<endl;
                if(ret==-2)
                    cout<<"ERROR : There is no sufficient memory to write"<<endl;
                if(ret==-3)
                    cout<<"ERROR : It is not regular file"<<endl;
            }
            else if(strcmp(command[0],"truncate")==0)
            {
                ret=truncate_File(command[1]);
                if(ret==-1)
                    cout<<"ERROR : Incorrect parameter"<<endl;
            }
            else
            {
                cout<<"ERROR : Command not found"<<endl;
                continue;
            }
        }
        else if(count==3)
        {
            if(strcmp(command[0],"create")==0)
            {
                ret=CreateFile(command[1],atoi(command[2]));
                if(ret>=0)
                    cout<<"File is successfully created with file descriptor "<<ret<<endl;
                if(ret==-1)
                    cout<<"ERROR : Incorrect parameter"<<endl;
                if(ret==-2)
                    cout<<"ERROR : There is no inodes"<<endl;
                if(ret==-3)
                    cout<<"ERROR : File already exists"<<endl;
                if(ret==-4)
                    cout<<"ERROR : Memory alllocation failure"<<endl;
                continue;
            }
            else if(strcmp(command[0],"open")==0)
            {
                ret=OpenFile(command[1],atoi(command[2]));
                if(ret>=0)
                    cout<<"File is successfully open with file descriptor "<<ret<<endl;
                if(ret==-1)
                    cout<<"ERROR : Incorrect parameter"<<endl;
                if(ret==-2)
                    cout<<"ERROR : File not present"<<endl;
                if(ret==-3)
                    cout<<"ERROR : permission denied"<<endl;
                continue;
            }
            else if(strcmp(command[0],"read")==0)
            {
                fd=GetFDFromName(command[1]);
                if(fd==-1)
                {
                    cout<<"ERROR : Incorrect parameter"<<endl;
                    continue;
                }
                ptr=(char *)malloc(sizeof(atoi(command[2]))+1);
                if(ptr==NULL)
                {
                    cout<<"ERROR : Memory allocation failure"<<endl;
                    continue;
                }
                ret=ReadFile(fd,ptr,atoi(command[2]));
                if(ret==-1)
                    cout<<"ERROR : File does not existing"<<endl;
                if(ret==-2)
                    cout<<"ERROR : Permission denied"<<endl;
                if(ret==-3)
                    cout<<"ERROR : Reached at end of file"<<endl;
                if(ret==-4)
                    cout<<"ERROR : It is not regular file"<<endl;
                if(ret==0)
                    cout<<"ERROR : File empty"<<endl;
                if(ret>0)
                    {
                        write(2,ptr,ret);
                    }
                continue;
            }
            else
            {
                cout<<"ERROR : Command not found !!!"<<endl;
                continue;
            }
        }
        else if(count==4)
        {
            if(strcmp(command[0],"lseek")==0)
            {
                fd=GetFDFromName(command[1]);
                if(fd==-1)
                {
                    cout<<"ERROR : Incorrect parameter"<<endl;
                    continue;
                }
                ret=LseekFile(fd,atoi(command[2]),atoi(command[3]));
                if(ret==-1)
                {
                    cout<<"ERROR : Unable to perform lseek"<<endl;
                }
            }
            else
            {
                cout<<"ERROR : Command not found !!!"<<endl;
                continue;
            }
        }
        else
        {
            cout<<"ERROR : Command not found !!!"<<endl;
            continue;
        }
    }
    return 0;
}