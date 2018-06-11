#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <iterator>
#include <cstring>
#include <sstream>
#include <strstream>
#include <cmath>
#include <initializer_list>

using namespace std;

#define _GOLDEN_RATIO       (sqrt(5)-1)/2
#define _TABLE_SIZE         1994
#define _RECORD_NUMBER      500
#define _MAX_BUFFER_SIZE    100

float _L_SEARCH_TIME=     0;
float _TOTAL_L_SEARCH_TIME=0;
float _TOTAL_L_DELETION_TIME=0;
float _Q_SEARCH_TIME=     0;
float _TOTAL_Q_SEARCH_TIME=0;
float _TOTAL_Q_DELETION_TIME=0;
float _D_SEARCH_TIME=     0;
float _TOTAL_D_SEARCH_TIME=0;
float _TOTAL_D_DELETION_TIME=0;
float _C_SEARCH_TIME=     0;
float _TOTAL_C_SEARCH_TIME=0;
float _TOTAL_C_DELETION_TIME=0;

float _L_PRINTING_TIME=   0;
float _Q_PRINTING_TIME=   0;
float _D_PRINTING_TIME=   0;
float _C_PRINTING_TIME=   0;


//************************************_______________******************************************//
//***********************************|Used file names|*****************************************//
//***********************************|_______________|*****************************************//
char    L_data_file         []={"L_data_file.txt"},
        Q_data_file         []={"Q_data_file.txt"},
        D_data_file         []={"D_data_file.txt"},
        C_data_file         []={"C_data_file.txt"},
        L_hash_index_file   []={"L_hash_index_file.txt"},
        Q_hash_index_file   []={"Q_hash_index_file.txt"},
        D_hash_index_file   []={"D_hash_index_file.txt"},
        C_hash_index_file   []={"C_hash_index_file.txt"};

//**************************______________________________***********************************//
//*************************|structure used in index arrays|**********************************//
//*************************|______________________________|**********************************//
struct record
{
    long ID;
    char Name[51];
    record():ID(0){}
    record(long _ID,char _Name[]):ID(_ID){strcpy(Name,_Name);}
};
struct element
{
    char PK[6];
    long offset;
    int next;
    element():offset(-1),next(-1){}
    element(char _PK[],long _offset, int _next=-1):offset(_offset),next(_next){strcpy(PK,_PK);}
};

//*******************************____________________________**********************************//
//******************************|string <-> integer converter|*********************************//
//******************************|____________________________|*********************************//
long long str_to_int_converter(char* quantity1 )
{
    stringstream converter1;
    long long quantity2;
    converter1<<quantity1;
    converter1>>quantity2;
    return quantity2;
}
char* int_to_str_converter(long long quantity1)
{
    stringstream converter1;
    char* quantity2=new char;
    converter1<<quantity1;
    converter1>>quantity2;
    quantity2[strlen(quantity2)]='\0';
    return quantity2 ;
}
//*******************************____________________________**********************************//
//******************************|       Tools functions      |*********************************//
//******************************|____________________________|*********************************//
bool int_exists_in(vector<int>& vec,int value)
{
    for(unsigned int i=0;i<vec.size();i++)
    {
        if(vec[i]==value){return true;}
    }
    return false;
}
char get_rand_char()
{
    char alphabets[]={"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
    int size=strlen(alphabets);
    int n= rand()%(size);
    return alphabets[n];
}
vector<record> generate_rand_records()
{
    vector<int>     vec_IDs;
    //vector<char*>   vec_names;
    vector<record>  vec_records;
    int i=0;
    while(i<500)
    {
        record rec;
        int n= 10000+rand()%10000;
        char *name=new char[6];
        for(int j=0;j<5;j++)
            name[j]=get_rand_char();
        name[5]='\0';
        if(!int_exists_in(vec_IDs,n))//&&!str_exists_in(vec_names,name)
        {
            rec.ID=n;
            strcpy(rec.Name,name);
            vec_records.push_back(rec);
            vec_IDs.push_back(n);
            //vec_names.push_back(name);
            cout<<"ID: "<<rec.ID<<"  "<<"Name: "<<rec.Name<<endl;
            i++;
        }
    }
    return vec_records;
}
long long mid_square(long long _ID)
{
    _ID=pow(_ID,2);
    int ID_len=strlen(int_to_str_converter(_ID));

    char*str_id =new char[ID_len];      strcpy(str_id,int_to_str_converter(_ID));
    char*num    =new char[(ID_len+1)/2];

    for(int i=(ID_len/2+1)/2,j=0; i <ID_len-(ID_len/2+1)/2; i++,j++)
        num[j]=str_id[i];
    num[strlen(num)]='\0';

    return str_to_int_converter(num);
}

long long folding(long long _ID, char mode)
{
    long long key=0,i,j;
    char*str_id =new char,*num=new char[4];     str_id=int_to_str_converter(_ID);   //str_id[strlen(str_id)]='\0';
    if(strlen(str_id)%3!=0)
    {
        while(strlen(str_id)%3!=0)
        {
            str_id[strlen(str_id)]='0';
            str_id[strlen(str_id)]='\0';
        }

    }
    switch(mode)
    {
    case 'S':{i=0, j=1;    break;}
    case 'B':
        {
            i=3, j=3;
            num[0]=str_id[2];   num[1]=str_id[1];   num[2]=str_id[0];   num[3]='\0';
            key+=str_to_int_converter(num);
            num[0]=str_id[strlen(str_id)-1];   num[1]=str_id[strlen(str_id)-2];   num[2]=str_id[strlen(str_id)-3];  num[3]='\0';
            key+=str_to_int_converter(num);
            break;
        }
    }
    for(i;i<strlen(str_id)-j;i+=3)
    {
        num[0]=str_id[i];   num[1]=str_id[i+1];   num[2]=str_id[i+2];   num[3]='\0';
        key+=str_to_int_converter(num);
    }
    return key;
}
long double  mod(long double num1, long double num2)
{
    return (num1*num2)-floor(num1*num2);
}
long long  int_mod(long long  num1,long long  num2)
{
    return num1 - num2*(num1/num2);
}
//**************************************____________*******************************************//
//*************************************|System Class|******************************************//
//*************************************|____________|******************************************//
class my_system
{
public:
    my_system();

    void interface();

    void load_hash_table();
    void save_hash_table();

    void add_record     (record rec, char mode  );      //Modes{L,Q,D,C}
    void delete_record  (record rec, char mode  );      //Mode L:linear probing, Mode Q:quadratic probing,
    int  search_record  (int   _key, char mode  );      //Mode D:double Hashing, Mode C:Chaining

    long long  linear_probing     (record rec, char mode, long long hash_value,int i  );  //Modes{S,A}
    long long  quadratic_probing  (record rec, char mode, long long hash_value,int i  );  //Mode S:Search for record,    Mode A:Add record
    long long  double_Hashing     (record rec, char mode, long long hash_value,int i  );
    long long  Chaining           (record rec, char mode, long long hash_value,int i  );

    bool is_empty_slot  (element slot);                 //the offset is -1
    bool is_deleted_slot(element slot);                 //the PK array filled with '#'


private:
    vector<record>  vec_records;            //vector of unique 500 records.

    vector<element> L_hash_index_vec,       //loading the linear probing    hash index file in the hash index vector
                    Q_hash_index_vec,       //loading the quadratic probing hash index file in the hash index vector
                    D_hash_index_vec,       //loading the double Hashing index  file in the hash index vector
                    C_hash_index_vec_1,     //loading the Chaining hash index   file in the hash index vector
                    C_hash_index_vec_2;     //loading the separate overflow area


    //unsigned int time_length;               //search time and deletion time

};
bool my_system::is_empty_slot  (element slot){if(slot.offset==-1){return true;}return false;}
bool my_system::is_deleted_slot(element slot){if(strcmp(slot.PK,"#####")==0){return true;}return false;}
//*******************************________________________________******************************//
//******************************|my_system class default constructor|*****************************//
//******************************|________________________________|*****************************//
my_system::my_system():L_hash_index_vec(_TABLE_SIZE),Q_hash_index_vec(_TABLE_SIZE),D_hash_index_vec(_TABLE_SIZE),
                        C_hash_index_vec_1(_TABLE_SIZE)//,time_length(0)
{
    fstream file;
    char* file_name_arr[8]={L_data_file,Q_data_file,D_data_file,C_data_file,L_hash_index_file,Q_hash_index_file,D_hash_index_file,C_hash_index_file};
    for(int i=0;i<8;i++)
    {
        file.open(file_name_arr[i],ios::in|ios::out);
        if(!file.is_open())
        {
            file.open(file_name_arr[i],ios::out);
            file.close();
        }
    }
    vector<element> *vec_name_arr[]={&L_hash_index_vec,&Q_hash_index_vec,&D_hash_index_vec,&C_hash_index_vec_1};
    for(int i=0;i<4;i++)
    {
        element e1;
        fill(vec_name_arr[i]->begin(),vec_name_arr[i]->end(),e1);
    }
}

//**********************************________________*******************************************//
//*********************************|my_system interface|******************************************//
//*********************************|________________|******************************************//
void my_system::interface()
{
    vec_records=generate_rand_records();
    int choice;
    while(true)
    {
    cout<<"\n\n";
    cout<<"choose what do you want to do: \n\n"
        <<"1) Add record\n"
        <<"2) Search for record\n"
        <<"3) Delete record\n"
        <<"4) Exit\n";
    cin>>choice;
    cin.ignore();
    switch(choice)
    {
        case 1://Add record
        {
            for(vector<record>::iterator itr=vec_records.begin();itr!=vec_records.end();++itr)
                add_record(*itr,'L');
            for(vector<record>::iterator itr=vec_records.begin();itr!=vec_records.end();++itr)
                add_record(*itr,'Q');
            for(vector<record>::iterator itr=vec_records.begin();itr!=vec_records.end();++itr)
                add_record(*itr,'D');
            for(vector<record>::iterator itr=vec_records.begin();itr!=vec_records.end();++itr)
                add_record(*itr,'C');

            break;
        }
        case 2://Search for record
        {
            float total_search_time=0;
            for(vector<record>::iterator itr=vec_records.begin();itr!=vec_records.end();++itr)
            {
                _L_SEARCH_TIME = 0;
                search_record(itr->ID,'L');
                total_search_time+=_L_SEARCH_TIME;
            }
            _TOTAL_L_SEARCH_TIME=total_search_time;
            total_search_time=0;
            for(vector<record>::iterator itr=vec_records.begin();itr!=vec_records.end();++itr)
            {
                _Q_SEARCH_TIME = 0;
                search_record(itr->ID,'Q');
                total_search_time+=_Q_SEARCH_TIME;
            }
            _TOTAL_Q_SEARCH_TIME=total_search_time;
            total_search_time=0;
            for(vector<record>::iterator itr=vec_records.begin();itr!=vec_records.end();++itr)
            {
                _D_SEARCH_TIME = 0;
                search_record(itr->ID,'D');
                total_search_time+=_D_SEARCH_TIME;
            }
            _TOTAL_D_SEARCH_TIME=total_search_time;
            total_search_time=0;
            for(vector<record>::iterator itr=vec_records.begin();itr!=vec_records.end();++itr)
            {
                _C_SEARCH_TIME = 0;
                search_record(itr->ID,'C');
                total_search_time+=_C_SEARCH_TIME;
            }
            _TOTAL_C_SEARCH_TIME=total_search_time;

            break;
        }
        case 3://Delete record
        {
            float total_deletion_time=0;
            for(int i=0;i<50;i++)
            {
                _L_SEARCH_TIME=0;
                delete_record(vec_records[i],'L');
                total_deletion_time+=_L_SEARCH_TIME;
            }
            _TOTAL_L_DELETION_TIME=total_deletion_time;
            total_deletion_time=0;
            for(int i=0;i<50;i++)
            {
                _Q_SEARCH_TIME=0;
                delete_record(vec_records[i],'Q');
                total_deletion_time+=_Q_SEARCH_TIME;
            }
            _TOTAL_Q_DELETION_TIME=total_deletion_time;
            total_deletion_time=0;
            for(int i=0;i<50;i++)
            {
                _D_SEARCH_TIME=0;
                delete_record(vec_records[i],'D');
                total_deletion_time+=_D_SEARCH_TIME;
            }
            _TOTAL_D_DELETION_TIME=total_deletion_time;
            total_deletion_time=0;
            for(int i=0;i<50;i++)
            {
                _C_SEARCH_TIME=0;
                delete_record(vec_records[i],'C');
                total_deletion_time+=_C_SEARCH_TIME;
            }
            _TOTAL_C_DELETION_TIME=total_deletion_time;
            break;
        }
        case 4://Exit
        {
            float total_printing_time=0;
            for(int i=50;i<vec_records.size();i++)
            {
                _L_SEARCH_TIME=0;
                search_record(vec_records[i].ID,'L');
                total_printing_time+=_L_SEARCH_TIME;
            }
            _L_PRINTING_TIME=total_printing_time;
            total_printing_time=0;
            for(int i=50;i<vec_records.size();i++)
            {
                _Q_SEARCH_TIME=0;
                search_record(vec_records[i].ID,'Q');
                total_printing_time+=_Q_SEARCH_TIME;
            }
            _Q_PRINTING_TIME=total_printing_time;
            total_printing_time=0;
            for(int i=50;i<vec_records.size();i++)
            {
                _D_SEARCH_TIME=0;
                search_record(vec_records[i].ID,'D');
                total_printing_time+=_D_SEARCH_TIME;
            }
            _D_PRINTING_TIME=total_printing_time;
            total_printing_time=0;
            for(int i=50;i<vec_records.size();i++)
            {
                _C_SEARCH_TIME=0;
                search_record(vec_records[i].ID,'C');
                total_printing_time+=_C_SEARCH_TIME;
            }
            _C_PRINTING_TIME=total_printing_time;
            cout<<"\n\n\n*********note that one search cost 1 time unit*********\n\n\n";
            cout<<"\n\n***Linear Probing Technique***\n\n";
            cout<<"average search length of all records                 : "<<_TOTAL_L_SEARCH_TIME/500<<endl;
            cout<<"average search length in deletion of the 50 records  : "<<_TOTAL_L_DELETION_TIME/50<<endl;
            cout<<"average search length to print the 450 records       : "<<_L_PRINTING_TIME/450<<endl;
            cout<<"____________________________________________________________________\n";
            cout<<"\n\n***Quadratic Probing Technique***\n\n";
            cout<<"average search length of all records                 : "<<_TOTAL_Q_SEARCH_TIME/500<<endl;
            cout<<"average search length in deletion of the 50 records  : "<<_TOTAL_Q_DELETION_TIME/50<<endl;
            cout<<"average search length to print the 450 records       : "<<_Q_PRINTING_TIME/450<<endl;
            cout<<"____________________________________________________________________\n";
            cout<<"\n\n***Double Hashing Technique***\n\n";
            cout<<"average search length of all records                 : "<<_TOTAL_D_SEARCH_TIME/500<<endl;
            cout<<"average search length in deletion of the 50 records  : "<<_TOTAL_D_DELETION_TIME/50<<endl;
            cout<<"average search length to print the 450 records       : "<<_D_PRINTING_TIME/450<<endl;
            cout<<"____________________________________________________________________\n";
            cout<<"\n\n***Chaining with separate overflow area Technique***\n\n";
            cout<<"average search length of all records                 : "<<_TOTAL_C_SEARCH_TIME/500<<endl;
            cout<<"average search length in deletion of the 50 records  : "<<_TOTAL_C_DELETION_TIME/50<<endl;
            cout<<"average search length to print the 450 records       : "<<_C_PRINTING_TIME/450<<endl;
            return;
        }
        default:
            cout<<"\n\n****INVALID CHOICE****\n\n"<<"Enter '1' to go to main menu\n\n";
            cin>>choice;
    }
    }

}

//**************************_________________________________________************************//
//*************************|              Save/Load Index,           |***********************//
//*************************|_________________________________________|***********************//


//**************************__________________________________________*****************************************//
//*************************|    (Add/Delete) record, search for key   |****************************************//
//*************************|__________________________________________|****************************************//
void my_system::add_record      (record rec, char mode  )
{
    switch(mode)
    {
    case 'L':
        {
            long long hash_value=_TABLE_SIZE*mod(rec.ID,_GOLDEN_RATIO);
            if(hash_value%2!=0)
                hash_value-=1;
            linear_probing(rec,'A',hash_value,2);
            break;
        }
    case 'Q':
        {

            long long hash_value=_TABLE_SIZE*mod(rec.ID,_GOLDEN_RATIO);
            if(hash_value%2!=0)
                hash_value-=1;
            quadratic_probing(rec,'A',hash_value,2);
            break;
        }
    case 'D':
        {
            long long hash_value=folding(rec.ID,'S');
            if(hash_value%2!=0)
                hash_value-=1;
            double_Hashing(rec,'A',hash_value,2);
            break;
        }
    case 'C':
        {
            long long hash_value=_TABLE_SIZE*mod(rec.ID,_GOLDEN_RATIO);
            if(hash_value%2!=0)
                hash_value-=1;
            Chaining(rec,'A',hash_value,2);
            break;
        }
    default: cout<<"\n\n****INVALID MODE IN ADD RECORD****\n\n"<<"Enter '1' to go to main menu\n\n";
             int check;  cin>>check;
    }
}
void my_system::delete_record   (record rec, char mode  )
{
    switch(mode)
    {
    case 'L':
        {
            long long hash_value=_TABLE_SIZE*mod(rec.ID,_GOLDEN_RATIO);
            if(hash_value%2!=0)
                hash_value-=1;
            int index=linear_probing(rec,'S',hash_value,2);
            if(index!=-1)
            {
                strcpy(L_hash_index_vec[index].PK,"#####");
                cout<<"\n\nsuccessfully deleted\n\n";
            }
            else
            {
                cout<<"\n\nitem already not exist\n\n";
            }
            break;
        }
    case 'Q':
        {
            long long hash_value=_TABLE_SIZE*mod(rec.ID,_GOLDEN_RATIO);
            if(hash_value%2!=0)
                hash_value-=1;
            int index=quadratic_probing(rec,'S',hash_value,2);
            if(index!=-1)
            {
                strcpy(Q_hash_index_vec[index].PK,"#####");
                cout<<"\n\nsuccessfully deleted\n\n";
            }
            else
            {
                cout<<"\n\nitem already not exist\n\n";
            }
            break;
        }
    case 'D':
        {
            long long hash_value=folding(rec.ID,'S');
            if(hash_value%2!=0)
                hash_value-=1;
            int index=double_Hashing(rec,'S',hash_value,2);
            if(index!=-1)
            {
                strcpy(D_hash_index_vec[index].PK,"#####");
                cout<<"\n\nsuccessfully deleted\n\n";
            }
            else
            {
                cout<<"\n\nitem already not exist\n\n";
            }
            break;
        }
    case 'C':
        {
            long long hash_value=_TABLE_SIZE*mod(rec.ID,_GOLDEN_RATIO);
            if(hash_value%2!=0)
                hash_value-=1;
            int index=Chaining(rec,'S',hash_value,2);
            if(index!=-1)
            {
                if(strcpy(C_hash_index_vec_1[index].PK,int_to_str_converter(rec.ID))==0)
                    strcpy(C_hash_index_vec_1[index].PK,"#####");
                else
                    strcpy(C_hash_index_vec_2[index].PK,"#####");
                cout<<"\n\nsuccessfully deleted\n\n";
            }
            else
            {
                cout<<"\n\nitem already not exist\n\n";
            }
        }
        break;
    }
}
int  my_system::search_record   (int _ID, char mode    )
{
    long index;
    switch(mode)
    {
    case 'L':
        {
            char name[]="Untitled";
            record rec(_ID,name);
            long long hash_value=_TABLE_SIZE*mod(_ID,_GOLDEN_RATIO);
            if(hash_value%2!=0)
                hash_value-=1;
            index=linear_probing(rec,'S',hash_value,2);
            break;
        }
    case 'Q':
        {
            char name[]="Untitled";
            record rec(_ID,name);
            long long hash_value=_TABLE_SIZE*mod(_ID,_GOLDEN_RATIO);
            if(hash_value%2!=0)
                hash_value-=1;
            index=quadratic_probing(rec,'S',hash_value,2);
            break;
        }
    case 'D':
        {
            char name[]="Untitled";
            record rec(_ID,name);
            long long hash_value  =int_mod(folding(_ID,'S'),_TABLE_SIZE);
            if(hash_value%2!=0)
                hash_value-=1;
            index=double_Hashing(rec,'S',hash_value,2);
            break;
        }
    case 'C':
        {
            char name[]="Untitled";
            record rec(_ID,name);
            long long hash_value=_TABLE_SIZE*mod(_ID,_GOLDEN_RATIO);
            if(hash_value%2!=0)
                hash_value-=1;
            index=Chaining(rec,'S',hash_value,2);
            break;
        }
    default: cout<<"\n\n****INVALID MODE IN SEARCH FOR RECORD****\n\n"<<"Enter '1' to go to main menu\n\n";
             int check;  cin>>check;
    }
    return index;
}


//**************************__________________________*****************************************//
//*************************|    Hashing techniques    |****************************************//
//*************************|__________________________|****************************************//
long long  my_system::linear_probing     (record rec, char mode, long long hash_value, int i  )
{
    if(hash_value>=_TABLE_SIZE-1)
        hash_value=_TABLE_SIZE-hash_value;
    if(hash_value%2!=0)
        hash_value-=1;
    switch(mode)
    {
    case 'A':
        {
            if( is_empty_slot(L_hash_index_vec[hash_value]) || is_empty_slot(L_hash_index_vec[hash_value+1]))
            {
                fstream file;
                file.open(L_data_file,ios::out|ios::in|ios::app);
                file.seekg(0,ios::end);
                long _offset=file.tellg();
                char* BUFFER=new char[_MAX_BUFFER_SIZE];
                strcpy(BUFFER,int_to_str_converter(rec.ID) );   strcat(BUFFER,"|");
                strcat(BUFFER,rec.Name);                        strcat(BUFFER,"|");
                short buff_len=strlen(BUFFER);
                file.write((char*)&buff_len,sizeof(buff_len));
                file.write(BUFFER,buff_len);
                file.close();
                element e1(int_to_str_converter(rec.ID),_offset);
                L_hash_index_vec[( is_empty_slot(L_hash_index_vec[hash_value]))?hash_value:hash_value+1]=e1;
                delete BUFFER;
                return -1;
            }
            else
            {
                linear_probing(rec,'A',hash_value+i,i);
            }
            break;
        }
    case 'S':
        {
            _L_SEARCH_TIME+=1;
           if( !is_empty_slot(L_hash_index_vec[hash_value]) || !is_empty_slot(L_hash_index_vec[hash_value+1]))
           {
                if(strcmp( (L_hash_index_vec[hash_value]).PK,int_to_str_converter(rec.ID) )==0 ||
                   strcmp( (L_hash_index_vec[hash_value+1]).PK,int_to_str_converter(rec.ID) )==0 )
                    return (strcmp( (L_hash_index_vec[hash_value]).PK,int_to_str_converter(rec.ID) )==0)?hash_value:hash_value+1;
                else
                    linear_probing(rec,'S',hash_value+i,i);
           }
           else
           {
               return -1;
           }
           break;
        }
    default :   cout<<"\n\n****INVALID MODE IN LENIAR PROBING****\n\n"<<"Enter '1' to go to main menu\n\n";
                int check;  cin>>check;
    }
    return -1;
}
long long  my_system::quadratic_probing  (record rec, char mode,long long hash_value, int i  )
{
    if(hash_value>=_TABLE_SIZE-1)
        hash_value=_TABLE_SIZE-hash_value;
    if(hash_value%2!=0)
        hash_value-=1;
    switch(mode)
    {
    case 'A':
        {
            if( is_empty_slot(Q_hash_index_vec[hash_value]) || is_empty_slot(Q_hash_index_vec[hash_value+1]))
            {
                fstream file;
                file.open(Q_data_file,ios::out|ios::in|ios::app);
                file.seekg(0,ios::end);
                long _offset=file.tellg();
                char* BUFFER=new char[_MAX_BUFFER_SIZE];
                strcpy(BUFFER,int_to_str_converter(rec.ID) );   strcat(BUFFER,"|");
                strcat(BUFFER,rec.Name);                        strcat(BUFFER,"|");
                short buff_len=strlen(BUFFER);
                file.write((char*)&buff_len,sizeof(buff_len));
                file.write(BUFFER,buff_len);
                file.close();
                element e1(int_to_str_converter(rec.ID),_offset);
                Q_hash_index_vec[( is_empty_slot(Q_hash_index_vec[hash_value]))?hash_value:hash_value+1]=e1;
                delete BUFFER;
                return -1;
            }
            else
            {
                quadratic_probing(rec,'A',hash_value+pow(i,2),i+2);
            }
            break;
        }
    case 'S':
        {
            _Q_SEARCH_TIME+=1;
           if( !is_empty_slot(Q_hash_index_vec[hash_value]) || !is_empty_slot(Q_hash_index_vec[hash_value+1]))
           {
                if(strcmp( (Q_hash_index_vec[hash_value]).PK,int_to_str_converter(rec.ID) )==0 ||
                   strcmp( (Q_hash_index_vec[hash_value+1]).PK,int_to_str_converter(rec.ID) )==0 )
                    return (strcmp( (Q_hash_index_vec[hash_value]).PK,int_to_str_converter(rec.ID) )==0)?hash_value:hash_value+1;
                else
                    quadratic_probing(rec,'S',hash_value+pow(i,2),i+1);
           }
           else
           {
               return -1;
           }
           break;
        }
    default :   cout<<"\n\n****INVALID MODE IN QUADRATIC PROBING****\n\n"<<"Enter '1' to go to main menu\n\n";
                int check;  cin>>check;
    }
    return -1;
}

long long  my_system::double_Hashing     (record rec, char mode,long long hash_value, int i  )
{
    if(hash_value>=_TABLE_SIZE-1)
        hash_value=_TABLE_SIZE-hash_value;
    if(hash_value%2!=0)
        hash_value-=1;
    switch(mode)
    {
    case 'A':
        {
            if( is_empty_slot(D_hash_index_vec[hash_value]) || is_empty_slot(D_hash_index_vec[hash_value+1]))
            {
                fstream file;
                file.open(D_data_file,ios::out|ios::in|ios::app);
                file.seekg(0,ios::end);
                long _offset=file.tellg();
                char* BUFFER=new char[_MAX_BUFFER_SIZE];
                strcpy(BUFFER,int_to_str_converter(rec.ID) );   strcat(BUFFER,"|");
                strcat(BUFFER,rec.Name);                        strcat(BUFFER,"|");
                short buff_len=strlen(BUFFER);
                file.write((char*)&buff_len,sizeof(buff_len));
                file.write(BUFFER,buff_len);
                file.flush();
                file.close();
                element e1(int_to_str_converter(rec.ID),_offset);
                D_hash_index_vec[( is_empty_slot(D_hash_index_vec[hash_value]))?hash_value:hash_value+1]=e1;
                delete BUFFER;
                return -1;
            }
            else
            {
                double_Hashing(rec,'A',int_mod(hash_value+mid_square(rec.ID),_TABLE_SIZE),i);
            }
            break;
        }
    case 'S':
        {
            _D_SEARCH_TIME+=1;
            if( !is_empty_slot(D_hash_index_vec[hash_value]) || !is_empty_slot(D_hash_index_vec[hash_value+1]))
            {
                if(strcmp( (D_hash_index_vec[hash_value]).PK,int_to_str_converter(rec.ID) )==0 ||
                   strcmp( (D_hash_index_vec[hash_value+1]).PK,int_to_str_converter(rec.ID) )==0 )
                    return (strcmp( (D_hash_index_vec[hash_value]).PK,int_to_str_converter(rec.ID) )==0)?hash_value:hash_value+1;
                else
                    {
                        double_Hashing(rec,'S',int_mod(hash_value+mid_square(rec.ID),_TABLE_SIZE),i);
                    }
            }
            else
            {
                return -1;
            }
           break;
        }
    default :   cout<<"\n\n****INVALID MODE IN DOUBLE HASHING****\n\n"<<"Enter '1' to go to main menu\n\n";
                int check;  cin>>check;
    }
    return -1;
}

long long  my_system::Chaining           (record rec, char mode,long long hash_value, int i  )
{
    switch(mode)
    {
    case 'A':
        {
            if( is_empty_slot(C_hash_index_vec_1[hash_value]) || is_empty_slot(C_hash_index_vec_1[hash_value+1]))
            {
                fstream file;
                file.open(C_data_file,ios::out|ios::in|ios::app);
                file.seekg(0,ios::end);
                long _offset=file.tellg();
                char* BUFFER=new char[_MAX_BUFFER_SIZE];
                strcpy(BUFFER,int_to_str_converter(rec.ID) );   strcat(BUFFER,"|");
                strcat(BUFFER,rec.Name);                        strcat(BUFFER,"|");
                short buff_len=strlen(BUFFER);
                file.write((char*)&buff_len,sizeof(buff_len));
                file.write(BUFFER,buff_len);
                file.close();
                element e1(int_to_str_converter(rec.ID),_offset);
                C_hash_index_vec_1[( is_empty_slot(C_hash_index_vec_1[hash_value]))?hash_value:hash_value+1]=e1;
                delete BUFFER;
                return -1;
            }
            else
            {
                int index1=C_hash_index_vec_1[hash_value+1].next;
                fstream file;
                file.open(C_data_file,ios::out|ios::in|ios::app);
                file.seekg(0,ios::end);
                long _offset=file.tellg();
                char* BUFFER=new char[_MAX_BUFFER_SIZE];
                strcpy(BUFFER,int_to_str_converter(rec.ID) );   strcat(BUFFER,"|");
                strcat(BUFFER,rec.Name);                        strcat(BUFFER,"|");
                short buff_len=strlen(BUFFER);
                file.write((char*)&buff_len,sizeof(buff_len));
                file.write(BUFFER,buff_len);
                file.close();
                if(index1!=-1)
                {
                    while(C_hash_index_vec_2[index1].next!=-1)
                        index1=C_hash_index_vec_2[index1].next;
                    C_hash_index_vec_2[index1].next=C_hash_index_vec_2.size();
                    element e1(int_to_str_converter(rec.ID),_offset);
                    C_hash_index_vec_2.push_back(e1);
                    return -1;
                }
                else
                {
                   C_hash_index_vec_1[hash_value+1].next= C_hash_index_vec_2.size();
                   element e1(int_to_str_converter(rec.ID),_offset);
                   C_hash_index_vec_2.push_back(e1);
                }
                return -1;
            }
            break;
        }
    case 'S':
        {
            _C_SEARCH_TIME+=1;
           if( !is_empty_slot(C_hash_index_vec_1[hash_value]) || !is_empty_slot(C_hash_index_vec_1[hash_value+1]))
           {
                if(strcmp( (C_hash_index_vec_1[hash_value]).PK,int_to_str_converter(rec.ID) )==0 ||
                   strcmp( (C_hash_index_vec_1[hash_value+1]).PK,int_to_str_converter(rec.ID) )==0 )
                    return (strcmp( (C_hash_index_vec_1[hash_value]).PK,int_to_str_converter(rec.ID) )==0)?hash_value:hash_value+1;
                else
                {
                    int index1=C_hash_index_vec_1[hash_value+1].next;
                    if(index1!=-1){
                    do
                    {
                        index1=C_hash_index_vec_2[index1].next;
                        if( strcmp( C_hash_index_vec_2[index1].PK,int_to_str_converter(rec.ID) )==0 )
                            return index1;
                    }while(index1!=-1);
                    return -1;
                    }
                    else
                        return -1;
                }
           }
           break;
        }
    default :   cout<<"\n\n****INVALID MODE IN CHAINING****\n\n"<<"Enter '1' to go to main menu\n\n";
                int check;  cin>>check;
    }
    return -1;
}

//**************************__________________________*****************************************//
//*************************|       main function      |****************************************//
//*************************|__________________________|****************************************//
int main()
{
    srand(time(0));
    my_system user;
    user.interface();

    return 0;
}

