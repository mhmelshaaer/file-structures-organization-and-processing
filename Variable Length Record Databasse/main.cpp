#include <iostream>
#include <string.h>
#include <fstream>
#include <strstream>
using namespace std;


//*********************************************************************************************//
//*********************************************************************************************//
//*********************************************************************************************//
class electronic_device
{
    public:
        electronic_device();
        electronic_device(char* _dev_ID,char* _dev_Name);
        friend  ostream& operator<< (ostream& out ,electronic_device &dev      );
        friend  istream& operator>> (istream& in  ,electronic_device &dev      );

        char* get_dev_Name   ();
        char* get_dev_ID     ();

    private:
        char dev_ID[20];
        char dev_Name[30];
};
electronic_device::electronic_device()
{
    for(int i=0;i<20;i++)
        dev_ID  [i]=' ';
    for(int i=0;i<30;i++)
        dev_Name[i]=' ';
}
electronic_device::electronic_device(char* _dev_ID,char* _dev_Name)
{
    strcpy(dev_ID  ,_dev_ID  );
    strcpy(dev_Name,_dev_Name);
}
ostream& operator<<(ostream& out,electronic_device &dev)
{
    out <<"\ndevice ID   : "<<dev.dev_ID
        <<"\ndevice Name : "<<dev.dev_Name
        <<endl;
    return out;
}

istream& operator>>(istream& in ,electronic_device &dev)
{
    cout<<"enter device ID  : ";    in.getline(dev.dev_ID  ,sizeof(dev.dev_ID  ));
    cout<<"enter device Name: ";    in.getline(dev.dev_Name,sizeof(dev.dev_Name));
    return in;
}
char* electronic_device::get_dev_Name   ()
{
    char* _dev_Name=new char[strlen(dev_Name)];
    strcpy(_dev_Name,dev_Name);
    return _dev_Name   ;
}
char* electronic_device::get_dev_ID     ()
{
    char* _dev_ID=new char[strlen(dev_ID)];
    strcpy(_dev_ID,dev_ID);
    return _dev_ID     ;
}
//*********************************************************************************************//
//*********************************************************************************************//
//*********************************************************************************************//
class system
{
    public:
        system();

        void interface();

        void    add_device          (electronic_device &dev       );
        void    read_all_devices    (                             );
        void    search_for_dev      (string              _dev_ID  );
        void    delete_device       (string              _dev_ID  );
        void    update_dvice        (string              _dev_ID  );
        void    compact_file        (                             );

        bool    is_empty            (std::fstream& pFile          );


    private:
        fstream file,comp_file;
        char*   buffer;
        const int MAX_BUFFER_SIZE;
        string main_file_name;
        string compact_file_name;
        short header,compacted_file_size,main_file_size;
};
system::system():MAX_BUFFER_SIZE(200),main_file_name("file1.txt"),compact_file_name("file2.txt"),header(-1)
{
    file.open("file1.txt",ios::in|ios::out);
    comp_file.open("file2.txt",ios::in|ios::out);
    if(file.is_open())
    {
        if(is_empty(file))
        {
            file.clear();
            file.seekp(ios::beg);
            file.write((char*)&header,sizeof(header));
        }
    }
    else
    {

        file.open("file1.txt",ios::out);
        file.close();
        file.open("file1.txt",ios::in|ios::out);
        if(is_empty(file))
        {
            file.clear();
            file.seekp(ios::beg);
            file.write((char*)&header,sizeof(header));
            //file.seekg(0,ios::end);
            //main_file_size=file.tellg();
        }
    }
    if(!comp_file.is_open())
    {
        comp_file.open("file2.txt",ios::out);
        comp_file.close();
        comp_file.open("file2.txt",ios::in|ios::out);

    }
    file.seekg(0,ios::end);
    main_file_size=file.tellg();
    comp_file.seekg(0,ios::end);
    compacted_file_size=comp_file.tellg();
    file.close();
    comp_file.close();
    buffer=new char[MAX_BUFFER_SIZE];
}

void system::interface()
{
    int choice=0;
    while(true)
    {
        file.open("file1.txt",ios::in|ios::out);
        short head;
        file.read((char*)&head,sizeof(head));

        cout<<"\n\n\nwelcome to the store management system\n\n\n"
            <<"main_file_size: "<<main_file_size<<endl
            <<"compacted_file_size: "<<compacted_file_size<<endl
            <<"head: "<<head<<"\n\n"
            <<"1) Add New Device\n"
            <<"2) List All Devices\n"
            <<"3) Update Device\n"
            <<"4) Delete Device\n"
            <<"5) Search for a Device By ID\n"
            <<"6) Compact File\n"
            <<"7) Exit\n"
            <<"\n\n\n"
            <<"\n\n\n\nPlease Enter Your Choice : ";
        cin>>choice;
        file.close();
        switch(choice)
        {
            case 1:
                {
                    electronic_device dev;
                    cout<<"\n\nenter the device information : \n";
                    cin.ignore();
                    cin >>dev;
                    add_device(dev);
                    break;
                }
            case 2:
                {
                    file.open("file1.txt",ios::in|ios::out);
                    read_all_devices();
                    file.close();
                    break;
                }
            case 3:
                {
                    file.open("file1.txt",ios::in|ios::out);
                    string _ID;
                    cout<<"\n\nEnter the device ID that you want to Update: ";  cin>>_ID;
                    cin.ignore();
                    update_dvice(_ID);
                    file.close();
                    break;
                }
            case 4:
                {
                    file.open("file1.txt",ios::in|ios::out);
                    string _ID;
                    cout<<"\n\nEnter the device ID that you want to delete: ";  cin>>_ID;
                    delete_device(_ID);
                    file.close();
                    break;
                }
            case 5:
                 {
                     file.open("file1.txt",ios::in|ios::out);
                     string _ID;
                     cout<<"\n\nEnter the device ID you want to search for: ";  cin>>_ID;
                     search_for_dev(_ID);
                     file.close();
                     break;
                 }
            case 6:
                {
                    file.open("file1.txt",ios::in|ios::out);
                    compact_file();
                    file.close();
                    break;
                }
            case 7: return;
            default:cout<<"\n\n*****invalid choice*****\n\n";
        }
    }
}

void    system::add_device          (electronic_device &dev       )
{
    file.open("file1.txt",ios::in|ios::out|ios::app);
    short recd_len;
    buffer=new char[MAX_BUFFER_SIZE];
    strcpy(buffer,dev.get_dev_ID()      );
    strcat(buffer,"#"                   );
    strcat(buffer,dev.get_dev_Name()    );
    strcat(buffer,"#"                   );

    short buff_len=strlen(buffer);
    file.seekg(ios::beg);
    file.read((char*)&header,sizeof(header));

    if(-1==header)
    {
        cout<<"the avail list is empty\n";
        file.write((char*)&buff_len,sizeof(buff_len));
        file.write(buffer,buff_len);
        file.close();
    }
    else
    {
        file.close();
        file.open("file1.txt",ios::in|ios::out);
        cout<<"we are here\n";
        while(true)
        {
            if(header==-1)
            {
                cout<<"there's no empty slot that fit the new record\n";
                file.close();
                return;
            }
            file.seekg(header,ios::beg);
            file.read((char*)&recd_len,sizeof(recd_len));
            file.seekg(sizeof(char),ios::cur);
            file.read((char*)&header,sizeof(header));
            if(buff_len<=recd_len)
            {
                short indx=sizeof(char)+sizeof(short);
                file.seekp(-indx,ios::cur);
                file.write(buffer,buff_len);
                file.seekp(0,ios::beg);
                file.write((char*)&header,sizeof(header));
                file.close();
                return;
            }
        }
    }
}
void    system::read_all_devices    (                             )
{
    char temp_ID[30],temp_Name[30];
    for(int i=0;i<sizeof(temp_ID);i++)
        temp_ID  [i]=' ';
    for(int i=0;i<sizeof(temp_Name);i++)
        temp_Name[i]=' ';
    short recd_len;
    file.seekg(sizeof(header),ios::beg);
    file.read((char*)&recd_len,sizeof(recd_len));
    buffer=new char[recd_len];

    do
    {
        file.read(buffer,recd_len);

        istrstream str_buff(buffer);

        str_buff.getline(temp_ID     ,sizeof(temp_ID  )   ,'#');
        str_buff.getline(temp_Name   ,sizeof(temp_Name)   ,'#');
        electronic_device dev(temp_ID,temp_Name);

        if(dev.get_dev_ID()[0]!='*')
            cout<<dev<<endl;

        file.read((char*)&recd_len,sizeof(recd_len));
    }while(!file.eof());
    file.clear();
    delete buffer;
}
void    system::search_for_dev      (string              _dev_ID  )
{
    char temp_ID[30],temp_Name[30];
    for(int i=0;i<sizeof(temp_ID);i++)
        temp_ID  [i]=' ';
    for(int i=0;i<sizeof(temp_Name);i++)
        temp_Name[i]=' ';
    short recd_len;
    file.seekg(sizeof(header),ios::beg);
    file.read((char*)&recd_len,sizeof(recd_len));
    char* buffer=new char[MAX_BUFFER_SIZE];

    do
    {
        file.read(buffer,recd_len);
        istrstream str_buff(buffer);
        str_buff.getline(temp_ID     ,sizeof(temp_ID)     ,'#');
        str_buff.getline(temp_Name   ,sizeof(temp_Name)   ,'#');
        electronic_device dev(temp_ID,temp_Name);
        short _counter=0;
        for(int i=0;i<_dev_ID.length();i++)
        {
            if(dev.get_dev_ID()[i]==_dev_ID[i])
                _counter++;
        }
        if(_counter==_dev_ID.length())
            {
                cout<<"the device you are looking for is :\n"<<dev<<endl;
                break;
            }
        file.read((char*)&recd_len,sizeof(recd_len));
        if(file.eof())
        {
            cout<<"\ndevice not found\n";
            break;
        }
    }while(true);

    file.clear();
    delete buffer;
}
void    system::delete_device       (string              _dev_ID  )
{
    char temp_ID[30],temp_Name[30];
    for(int i=0;i<sizeof(temp_ID);i++)
        temp_ID  [i]=' ';
    for(int i=0;i<sizeof(temp_Name);i++)
        temp_Name[i]=' ';
    short recd_len;
    file.read((char*)&header,sizeof(header));
    file.read((char*)&recd_len,sizeof(recd_len));
    buffer=new char[MAX_BUFFER_SIZE];

    do
    {
        file.read(buffer,recd_len);
        istrstream str_buff(buffer);
        str_buff.getline(temp_ID     ,sizeof(temp_ID)     ,'#');
        str_buff.getline(temp_Name   ,sizeof(temp_Name)   ,'#');

        electronic_device dev(temp_ID,temp_Name);
        short _counter=0;
        for(int i=0;i<_dev_ID.length();i++)
        {
            if(dev.get_dev_ID()[i]==_dev_ID[i])
                _counter++;
        }
        if(_counter==_dev_ID.length())
            {
                char ch='*';
                cout<<"the device that you want to delete:\n"<<dev<<endl;

                int check;
                cout<<"enter (1) to confirm or (2) to cancel: ";    cin>>check;
                if (2==check){return;}
                file.seekp(-recd_len,ios::cur);
                cout<<"tellp: "<<file.tellp()<<endl;
                short offset=file.tellp()-sizeof(recd_len);
                cout<<"offset: "<<offset<<endl;
                file.write((char*)&ch,sizeof(ch));
                file.write((char*)&header,sizeof(header));
                file.seekp(ios::beg);
                file.write((char*)&offset,sizeof(offset));
                break;
            }
        file.read((char*)&recd_len,sizeof(recd_len));
        if(file.eof())
        {
            cout<<"\ndevice not found\n";
            break;
        }
    }while(true);

    file.clear();
    delete buffer;

}
void    system::compact_file        (                             )
{
    comp_file.open("file2.txt",ios::in|ios::out|ios::trunc);//|ios::app
    if(comp_file.is_open())
    {
        comp_file.clear();
        file.seekp(ios::beg);
        file.read((char*)&header,sizeof(header));
        comp_file.seekp(ios::beg);
        comp_file.write((char*)&header,sizeof(header));
    }
    else
    {

        comp_file.open("file2.txt",ios::out);
        comp_file.close();
        comp_file.open("file2.txt",ios::in|ios::out|ios::trunc);

        comp_file.clear();
        file.seekp(ios::beg);
        file.read((char*)&header,sizeof(header));
        comp_file.seekp(ios::beg);
        comp_file.write((char*)&header,sizeof(header));
    }
    char temp_ID[30],temp_Name[30];
    for(int i=0;i<sizeof(temp_ID);i++)
        temp_ID  [i]=' ';
    for(int i=0;i<sizeof(temp_Name);i++)
        temp_Name[i]=' ';
    short recd_len;
    file.seekp(sizeof(header),ios::beg);
    file.read((char*)&recd_len,sizeof(recd_len));
    buffer=new char[MAX_BUFFER_SIZE];
    char* buffer1=new char[MAX_BUFFER_SIZE];
    do
    {
        file.read(buffer,recd_len);
        istrstream str_buff(buffer);
        str_buff.getline(temp_ID     ,sizeof(temp_ID  )   ,'#');
        str_buff.getline(temp_Name   ,sizeof(temp_Name)   ,'#');
        electronic_device dev(temp_ID,temp_Name);
        if(dev.get_dev_ID()[0]!='*')
            {
                strcpy(buffer1,dev.get_dev_ID()      );
                strcat(buffer1,"#"                   );
                strcat(buffer1,dev.get_dev_Name()    );
                strcat(buffer1,"#"                   );
                short buff_len=strlen(buffer1);
                comp_file.write((char*)&buff_len,sizeof(buff_len));
                comp_file.write(buffer1,buff_len);
            }

        file.read((char*)&recd_len,sizeof(recd_len));
    }while(!file.eof());
    file.clear();
    comp_file.seekg(0,ios::end);
    compacted_file_size=comp_file.tellg();
    comp_file.close();
    delete buffer;
    delete buffer1;
}
void    system::update_dvice        (string              _dev_ID  )
{
    char temp_ID[30],temp_Name[30];
    for(int i=0;i<sizeof(temp_ID);i++)
        temp_ID  [i]=' ';
    for(int i=0;i<sizeof(temp_Name);i++)
        temp_Name[i]=' ';
    short recd_len;
    file.seekg(sizeof(header),ios::beg);
    file.read((char*)&recd_len,sizeof(recd_len));
    char* buffer=new char[recd_len];

    do
    {
        file.read(buffer,recd_len);
        istrstream str_buff(buffer);
        str_buff.getline(temp_ID     ,sizeof(temp_ID)     ,'#');
        str_buff.getline(temp_Name   ,sizeof(temp_Name)   ,'#');

        electronic_device dev(temp_ID,temp_Name);
        short _counter=0;
        for(int i=0;i<_dev_ID.length();i++)
        {
            if(dev.get_dev_ID()[i]==_dev_ID[i])
                _counter++;
        }
        if(_counter==_dev_ID.length())
            {

                cout<<"the device that you want to update is :\n"<<dev<<endl;
                file.seekp(-recd_len,ios::cur);
                electronic_device updated_dev;
                cin>>updated_dev;
                buffer=new char[MAX_BUFFER_SIZE];

                strcpy(buffer,updated_dev.get_dev_ID()    );
                strcat(buffer,"#"                         );
                strcat(buffer,updated_dev.get_dev_Name()  );
                strcat(buffer,"#"                         );

                short buff_len=strlen(buffer);
                if(buff_len<=recd_len)
                {
                    file.write(buffer,buff_len);
                }
                else
                {
                    file.close();
                    file.open("file1.txt",ios::in|ios::out);
                    delete_device(_dev_ID);
                    file.close();
                    add_device(updated_dev);
                }
                break;
            }
        file.read((char*)&recd_len,sizeof(recd_len));
        if(file.eof())
        {
            cout<<"\ndevice not found\n";
            break;
        }
    }while(true);

    file.clear();
    delete buffer;
}
bool    system::is_empty            (std::fstream& pFile          )
{
    return pFile.peek() == std::ifstream::traits_type::eof();
}
//*********************************************************************************************//
//*********************************************************************************************//
//*********************************************************************************************//
int main()
{
    system user1;
    user1.interface();

    return 0;
}
