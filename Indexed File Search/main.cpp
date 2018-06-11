#include <iostream>
#include <fstream>
#include <strstream>
#include <sstream>
#include <cstring>
#include <vector>
#include <iterator>

using namespace std;
//************************************_______________******************************************//
//***********************************|Used file names|*****************************************//
//***********************************|_______________|*****************************************//
int MAX_BUFFER_SIZE=200;
char data_file[30]={"data_file.txt"},                                         prim_ID_index[30]={"prim_ID_index.txt"},
     sec_model_index[30]={"sec_model_index.txt"},                        sec_price_index[30]={"sec_price_index.txt"},
     data_comp_file[30]={"data_comp_file.txt"},                          prim_index_comp_file[30]={"prim_index_comp_file.txt"},
     sec_model_index_comp_file[30]={"sec_model_index_comp_file.txt"},    sec_price_index_comp_file[30]={"sec_price_index_comp_file.txt"};

//*******************************____________________________**********************************//
//******************************|string <-> integer converter|*********************************//
//******************************|____________________________|*********************************//
float str_to_float_converter(char* quantity1 )
{
    stringstream converter1;
    float quantity2;
    converter1<<quantity1;
    converter1>>quantity2;
    return quantity2;
}
char* float_to_str_converter(float quantity1)
{
    stringstream converter1;
    char* quantity2=new char[5];
    converter1<<quantity1;
    converter1>>quantity2;
    return quantity2 ;
}
//**************************______________________________***********************************//
//*************************|structure used in index arrays|**********************************//
//*************************|______________________________|**********************************//
class element
{
public:
    element():offset(-1){value=new char[10];}//strcpy(value," ");
    element(char* _value,int _offset=-1):offset(_offset){value=new char[10];strcpy(value,_value);}
    int get_offset(){return offset;}
    char* get_value(){return value;}
    void set_offset(int _offset){offset=_offset;}
    void set_value(char* _value){strcpy(value,_value);}
private:
    char* value;
    int offset;
};
//******************************_______________________******************************************//
//*****************************|electronic device class|****************************************//
//*****************************|_______________________|****************************************//
class electronic_device
{
public:
    electronic_device();
    electronic_device(char* _dev_ID,char* _dev_Model,float _dev_Price);

    friend  ostream& operator<< (ostream& out ,electronic_device &dev      );
    friend  istream& operator>> (istream& in  ,electronic_device &dev      );

    char* get_dev_Model     ();
    char* get_dev_ID        ();
    float get_dev_Price     ();

    void set_dev_Model     (char* _dev_Model);
    void set_dev_ID        (char* _dev_ID   );
    void set_dev_Price     (float _dev_Price);

private:
    char dev_ID[10];
    char dev_Model[10];
    float dev_Price;
};
electronic_device::electronic_device():dev_Price(0.0)
{
    //fill(dev_ID,dev_ID + 9,' ');   fill(dev_Model,dev_Model + 9,' ');
}
electronic_device::electronic_device(char* _dev_ID,char* _dev_Model,float _dev_Price):dev_Price(_dev_Price)
{
    //fill(dev_ID,dev_ID + 9,' ');   fill(dev_Model,dev_Model + 9,' ');
    strcpy(dev_ID   ,_dev_ID);      strcpy(dev_Model,_dev_Model);
}
ostream& operator<<(ostream& out,electronic_device &dev)
{
    out <<"\ndevice ID    : "<<dev.dev_ID
        <<"\ndevice Model : "<<dev.dev_Model
        <<"\ndevice price : "<<dev.dev_Price
        <<endl;
    return out;
}
istream& operator>>(istream& in ,electronic_device &dev)
{
    cout<<"enter device ID    : ";  in.getline(dev.dev_ID  ,sizeof(dev.dev_ID  ));
    cout<<"enter device Model : ";  in.getline(dev.dev_Model,sizeof(dev.dev_Model));
    cout<<"enter device price : ";  in>>dev.dev_Price;
    return in;
}
char* electronic_device::get_dev_Model  (){return dev_Model;}
char* electronic_device::get_dev_ID     (){return dev_ID;}
float electronic_device::get_dev_Price  (){return dev_Price;}

void electronic_device::set_dev_Model   (char* _dev_Model){strcpy(dev_Model,_dev_Model);}
void electronic_device::set_dev_ID      (char* _dev_ID   ){strcpy(dev_ID,_dev_ID);}
void electronic_device::set_dev_Price   (float _dev_Price){dev_Price=_dev_Price;}
//**************************************____________*******************************************//
//*************************************|System Class|******************************************//
//*************************************|____________|******************************************//
class system
{
    public:
        system();

        void interface();




        int     search_for_dev_ID         (char* _dev_ID   ,vector<element> &vec);//yes
        char*   search_for_dev_Model      (char* _dev_Model  );//yes
        char*   search_for_dev_Price      (float _dev_Price  );//yes

        void    read_all_devices          ();//yes##############
        int     add_device                (electronic_device &dev);//yes##############
        void    delete_device             (char*              _dev_ID  );//no##############
        void    update_dvice              (char*              _dev_ID  );//no##############

        void    compact_file              ();//no##############
        void    visualize_file            ();//no##############

        void    load_index                ();//yes
        void    save_index                ();//yes
        void    update_index              (electronic_device printer,int offset,char mode);//yes##############
        void    change_index_state        (bool state);//yes
        bool    get_index_state           ();//yes
        void    reconstruct_index         ();//yes##############

        int     binary_search             (vector<element> &vec,int left,int right,char* key);//yes
        void    quick_sort                (vector<element> &vec,int left,int right  );//yes

        bool    is_empty                  (std::fstream& pFile);//yes


    private:
        vector<element> prim_ID_index_vec,
                        sec_model_index1,
                        sec_model_index2,
                        sec_price_index1,
                        sec_price_index2;

        fstream file,comp_file;
        char*   buffer;
        bool    header;
};
void    system::quick_sort(vector<element> &vec,int left,int right)
{
    int i=left, j=right;
    char* pivot=new char[10];
    strcpy(pivot,vec[left].get_value());
    while(i<=j)
    {
        while (strcmp(vec[i].get_value(),pivot)<=-1){i++;}
        while (strcmp(vec[j].get_value(),pivot)>=1){j--;}
        if(i<=j)
        {
            element temp;   temp=vec[i];
            vec[i]=vec[j];  vec[j]=temp;
            i++; j--;
        }
    }
    if(left<j){quick_sort(vec,left,j);}
    if(i<right){quick_sort(vec,i,right);}
}
int     system::binary_search(vector<element> &vec,int left,int right,char* key)
{
    int mid_indx=(left+right)/2;
    if(right<left){return -1;}
    if(strcmp(vec[0].get_value(),key)>=1||strcmp(vec[vec.size()-1].get_value(),key)<=-1)
        {return -1;}
    if(strcmp(vec[mid_indx].get_value(),key)==0)
        {return mid_indx;}
    else if(strcmp(vec[mid_indx].get_value(),key)>=1)
        {binary_search(vec,left,mid_indx-1,key);}
    else if(strcmp(vec[mid_indx].get_value(),key)<=-1)
        {binary_search(vec,mid_indx+1,right,key);}
    else
        {return -1;}
    return -1;
}
bool    system::is_empty(std::fstream& pFile)
{
    return pFile.peek() == std::ifstream::traits_type::eof();
}
//*******************************________________________________******************************//
//******************************|system class default constructor|*****************************//
//******************************|________________________________|*****************************//
system::system():header(0)
{
    //buffer=new char[MAX_BUFFER_SIZE];

    file.open(data_file,ios::in|ios::out);
    comp_file.open(data_comp_file,ios::in|ios::out);
    if(file.is_open())
    {
        if(is_empty(file))
        {
            file.clear();
            file.seekp(0,ios::beg);
            file.write((char*)&header,sizeof(header));
        }
    }
    else
    {
        file.open(data_file,ios::out);
        file.close();
        file.open(data_file,ios::in|ios::out);
        file.seekp(0,ios::beg);
        file.write((char*)&header,sizeof(header));
    }
    if(!comp_file.is_open())
    {
        comp_file.open(data_comp_file,ios::out);
        comp_file.close();
    }
    //file.clear();   comp_file.clear();
    file.close();   comp_file.close();

    char* file_name_arr[6]={prim_ID_index,prim_index_comp_file,sec_model_index,sec_model_index_comp_file,
                            sec_price_index,sec_price_index_comp_file};
    for(int i=0;i<6;i+=2)
    {
        file.open(file_name_arr[i],ios::in|ios::out);
        comp_file.open(file_name_arr[i+1],ios::in|ios::out);
        if(!file.is_open())
        {
            file.open(file_name_arr[i],ios::out);
            //file.clear();
            file.close();
        }
        if(!comp_file.is_open())
        {
            comp_file.open(file_name_arr[i+1],ios::out);
            //file.clear();
            comp_file.close();
        }
    }
}
//**********************************________________*******************************************//
//*********************************|system interface|******************************************//
//*********************************|________________|*******************************************//
void system::interface()
{
    int check=0;
    while(true)
    {
        cout<<"\n\n";
        cout<<"choose what do you want to do: model1 \n\n"
            <<"1) Add New Printer\n"
            <<"2) Update Printer\n"
            <<"3) Delete Printer\n"
            <<"4) View all printers\n"
            <<"5) Search for Printer using ID\n"
            <<"6) Search for Printer using Model#\n"
            <<"7) Search for Printer using certain Model & Price\n"
            <<"8) Compact File\n"
            <<"9) Visualize File\n"
            <<"10) exit\n";
        cin>>check;
        cin.ignore();
        switch(check)
        {
        case 1://Add New Printer
        {
            electronic_device printer;
            cin>>printer;
            add_device(printer);
            break;
        }
        case 2://Update Printer
        {
            char* _ID=new char[10];
            cout<<"enter the printer ID you want to update: ";  cin.getline(_ID,10);
            //update_dvice(_ID);
            break;
        }
        case 3://Delete Printer
        {
            char* _ID=new char[10];
            cout<<"enter the printer ID you want to delete: ";  cin.getline(_ID,10);
            //delete_device(_ID);
            break;
        }
        case 4:{read_all_devices();break;}
        case 5://Search for Printer using ID
        {
            char* _ID=new char[10];
            cout<<"enter the printer ID you want to search for: ";  cin.getline(_ID,10);
            int index=search_for_dev_ID(_ID,prim_ID_index_vec);
            cout<<"\n\nthe printer : "<<prim_ID_index_vec[index].get_value()<<endl;

            break;
        }
        case 6://Search for Printer using Model#
        {
            char* _Model=new char[10];
            cout<<"enter the printer Model you want to search for: ";  cin.getline(_Model,10);
            cout<<"\n\nthe printer: "<<search_for_dev_Model(_Model)<<endl;
            break;
        }
        case 7://Search for Printer using certain Model & Price
        {
            float _Price;
            cout<<"enter the printer price you want to search for: ";   cin>>_Price;
            cout<<"\n\nthe printer: "<<search_for_dev_Price(_Price)<<endl;
            break;
        }
        case 8://Compact File
        {
            //compact_file();
            break;
        }
        case 9://Visualize File
        {
            //visualize_file();
            break;
        }
        case 10:
        {
            //save_index();
            return ;//exit
        }
        default:
            cout<<"\n\n****INVALID CHOICE****\n\n"<<"Enter '1' to go to main menu\n\n";
            cin>>check;
        }
    }
}
//******************************___________________********************************************//
//*****************************|Search for a device|*******************************************//
//*****************************|___________________|*******************************************//
int     system::search_for_dev_ID(char* _dev_ID,vector<element> &vec)
{
    return binary_search(vec,0,vec.size()-1,_dev_ID);
}

char*   system::search_for_dev_Model(char* _dev_Model)
{
    int index=binary_search(sec_model_index1,0,sec_model_index1.size()-1,_dev_Model);
    int offset= sec_model_index1[index].get_offset();
    if(index!=-1)
    {
        element i=sec_model_index2[offset];
        int _=1;    char* choice=new char[10];
        do
        {
            offset=i.get_offset();
            if(i.get_value()[0]!='*')
                cout<<_++<<")"<<i.get_value()<<endl;
            i=sec_model_index2[i.get_offset()];
        }while(offset!=-1);
        cout<<"\n\nchoose the device ID you want: ";    cin>>choice;
        return choice;
    }
    else
        return (char*)&"not found";
}
char*   system::search_for_dev_Price(float _dev_Price)
{
    char* str_price=new char[5];
    str_price=float_to_str_converter(_dev_Price);
    int index=binary_search(sec_price_index1,0,sec_price_index1.size()-1,str_price);
    int offset= sec_price_index1[index].get_offset();
    if(index!=1)
    {
        element i=sec_price_index2[offset];
        int _=1;    char* choice=new char[10];
        do
        {
            offset=i.get_offset();
            if(i.get_value()[0]!='*')
                cout<<_++<<")"<<i.get_value()<<endl;
            i=sec_price_index2[i.get_offset()];
        }while(offset!=-1);
        cout<<"\n\nchoose the device ID you want: ";   cin.ignore(); cin.getline(choice,10);
        return search_for_dev_Model(choice);
    }
    else
        return (char*)&"not found";
}
//************_________________________________________________________________****************//
//***********|Save/Load Index,get/change index state, binary search, quick sort|***************//
//***********|_________________________________________________________________|***************//
void    system::change_index_state(bool state)
{
    file.open(data_file,ios::in|ios::out);
    file.seekp(0,ios::beg);
    file.write((char*)&state,sizeof(state));
    file.clear();
    file.close();
}
bool    system::get_index_state()
{
    file.open(data_file,ios::in|ios::out);
    file.seekp(0,ios::beg);
    file.read((char*)&header,sizeof(header));
    file.clear();
    file.close();
    return header;
}
void    system::load_index()
{
    fstream file1;
    char* file_name_arr[2]={sec_model_index,sec_price_index};
    vector<element> *vec_name_arr[4]={&sec_model_index1,&sec_model_index2,&sec_price_index1,&sec_price_index2};

    int i=0,j=0;
    do
    {
        vector<element> *current1=vec_name_arr[j], *current2=vec_name_arr[j+1];
        file1.open(file_name_arr[i],ios::in|ios::out);
        element read_e1,read_e2;
        file1.read((char*)&read_e1,sizeof(read_e1));
        if(file1.eof()){break;}
        current1->push_back(read_e1);
        do
        {
            file1.read((char*)&read_e2,sizeof(read_e2));
            if(file1.eof()){break;}
            current2->push_back(read_e2);
        }
        while(read_e2.get_offset()!=-1);
        i++;j+=2;
    }
    while(i<2);
    file1.close();
    //*******loading the primary index*******
    file1.open(prim_ID_index,ios::in|ios::out);
    do
    {
        element read_e1;
        file1.read((char*)&read_e1,sizeof(read_e1));
        if(file1.eof()){break;}
        prim_ID_index_vec.push_back(read_e1);
    }
    while(!file1.eof());
    file1.close();
}
void    system::save_index()
{
    fstream file1;
    //*******saving the secondary indices*******
    char* file_name_arr[2]={sec_model_index,sec_price_index};
    vector<element> *vec_name_arr[]={&sec_model_index1,&sec_model_index2,&sec_price_index1,&sec_price_index2};
    for(int k=0,j=0;k<2;k++,j+=2)
    {
        file1.open(file_name_arr[k],ios::in|ios::out|ios::trunc);
        vector<element> *current1=vec_name_arr[j], *current2=vec_name_arr[j+1];
        for(unsigned int i=0; i<vec_name_arr[j]->size(); i++)
        {
            int indx=(*current1)[i].get_offset();
            element e1((*current1)[i].get_value(),(*current1)[i].get_offset());
            file1.write((char*)&e1,sizeof(e1));
            for(element c=(*current2)[indx]; c.get_offset()!=-1; c=(*current2)[c.get_offset()])
            {
                indx=c.get_offset();
                file1.write((char*)&c,sizeof(c));
            }
            file1.write((char*)&(*current2)[indx],sizeof((*current2)[indx]));
        }
        file1.close();
    }
    //*******saving the primary index*******
    file1.open(prim_ID_index,ios::in|ios::out|ios::trunc);
    for(unsigned int i=0; i<prim_ID_index_vec.size(); i++)
    {

        file1.write((char*)&prim_ID_index_vec[i],sizeof(prim_ID_index_vec[i]));
    }
    file1.close();
    change_index_state(false);
}


void    system::update_index(electronic_device printer,int _offset,char mode)
{
    int index1=0;   char* _value=new char[10];  //fill(_value,_value+9,' ');
    switch(mode)
    {
        case 'K':                  //   K : Primary key Index.
            {
                element e1(printer.get_dev_ID(),_offset);
                prim_ID_index_vec.push_back(e1);
                quick_sort(prim_ID_index_vec,0,prim_ID_index_vec.size()-1);
                cout<<"prim_ID_index_vec\n";
                for(vector<element>::iterator itr=prim_ID_index_vec.begin();itr!=prim_ID_index_vec.end();++itr)
                    cout<<itr->get_value()<<" "<<itr->get_offset()<<endl;
                return;
            }
        case 'M':{index1=0; strcpy(_value,printer.get_dev_Model()); break;}                            //   M : Secondary Model Index.
        case 'P':{index1=2; strcpy(_value,float_to_str_converter(printer.get_dev_Price())); break;}    //   P : Secondary Price Index.
        default:{cout<<"no update\n";return;}
    }
    vector<element> *vec_name_arr[4]={&sec_model_index1,&sec_model_index2,&sec_price_index1,&sec_price_index2};
    vector<element> *current1=vec_name_arr[index1], *current2=vec_name_arr[index1+1];
    cout<<_value<<endl;
    cout<<"binary search issue 1\n";
    int index2=binary_search((*current1),0,current1->size()-1,_value);
    cout<<"binary search issue 2\n";
    if(index2!=-1)
    {
        for(element i=(*current2)[index2]; i.get_offset()!=-1; i=(*current2)[i.get_offset()])
            index2=i.get_offset();
        (*current2)[index2].set_offset(current2->size());
        element e1(printer.get_dev_ID());
        current2->push_back(e1);

    }
    else
    {
        element e1(_value,current2->size()),
                e2(printer.get_dev_ID());
        current1->push_back(e1);    current2->push_back(e2);
        quick_sort(*current1,0,current1->size()-1);
    }
}
void system::reconstruct_index()
{
     fstream file1;

    cout<<"hello\n";
    vector<electronic_device> arr;
    vector<int> arr2;
    char _ID[10],_Model[10],str_Price[10];
    file1.open(data_file,ios::in|ios::out);
    file1.seekg(sizeof(header),ios::beg);
    do
    {
        char *buffer1=new char[MAX_BUFFER_SIZE];
        short offset=file1.tellg();
        short buff_len=0;
        file1.read((char*)&buff_len,sizeof(buff_len));
        if(file1.eof()){delete []buffer1;break;}
        file1.read(buffer1,buff_len);
        if(buffer1[0]=='*'){delete []buffer1;continue;}

        istrstream str(buffer1);
        str.getline(_ID,10,'|');
        str.getline(_Model,10,'|');
        str.getline(str_Price,10,'|');
        float float_Price=str_to_float_converter(str_Price);
        electronic_device printer(_ID,_Model,float_Price);
        arr.push_back(printer);
        arr2.push_back(offset);
        delete []buffer1;
    }while(!file1.eof());
    file1.clear();
    file1.close();
    for(int i=0;i<arr.size();i++)
    {
        update_index(arr[i],arr2[i],'K');
        update_index(arr[i],arr2[i],'M');
        update_index(arr[i],arr2[i],'P');
    }
}
//**************************__________________________*****************************************//
//*************************|(Add/Delete/Update) device|****************************************//
//*************************|__________________________|****************************************//

void    system::read_all_devices()
{
    buffer=new char[MAX_BUFFER_SIZE];
    file.open(data_file,ios::in|ios::out);
    char _ID[20],_Model[20],str_Price[20];
    short buff_len=0;
    for(unsigned int i=0;i<prim_ID_index_vec.size();i++)
    {
        if((prim_ID_index_vec[i].get_value())[0]=='*'){continue;}
        file.seekg(prim_ID_index_vec[i].get_offset(),ios::beg);
        file.read((char*)&buff_len,sizeof(buff_len));
        file.read(buffer,buff_len);

        istrstream str(buffer);
        str.getline(_ID,20,'|');
        str.getline(_Model,20,'|');
        str.getline(str_Price,20,'|');
        float float_price=str_to_float_converter(str_Price);

        electronic_device printer(_ID,_Model,float_price);
        cout<<printer<<endl;

    }
    file.clear();
    file.close();
    delete []buffer;
}
int     system::add_device(electronic_device &dev)
{
    reconstruct_index();

    buffer=new char[MAX_BUFFER_SIZE];
    change_index_state(true);
    file.open(data_file,ios::in|ios::out);
    file.seekp(0,ios::end);
    int offset=file.tellp();
    strcpy(buffer,dev.get_dev_ID()); strcat(buffer,"|");
    strcat(buffer,dev.get_dev_Model()); strcat(buffer,"|");
    strcat(buffer,float_to_str_converter(dev.get_dev_Price())); strcat(buffer,"|");
    short buff_len=strlen(buffer);
    file.write((char*)&buff_len,sizeof(buff_len));
    file.write(buffer,buff_len);
    update_index(dev,offset,'K');
    update_index(dev,offset,'M');
    update_index(dev,offset,'P');
    file.close();
    change_index_state(false);
    delete []buffer;
    cout<<"prim_ID_index_vec\n";
    for(vector<element>::iterator itr=prim_ID_index_vec.begin();itr!=prim_ID_index_vec.end();++itr)
        cout<<itr->get_value()<<" "<<itr->get_offset()<<endl;
    cout<<"sec_model_index1\n";
    for(vector<element>::iterator itr=sec_model_index1.begin();itr!=sec_model_index1.end();++itr)
        cout<<itr->get_value()<<" "<<itr->get_offset()<<endl;
    cout<<"sec_model_index2\n";
    for(vector<element>::iterator itr=sec_model_index2.begin();itr!=sec_model_index2.end();++itr)
        cout<<itr->get_value()<<" "<<itr->get_offset()<<endl;
    cout<<"sec_price_index1\n";
    for(vector<element>::iterator itr=sec_price_index1.begin();itr!=sec_price_index1.end();++itr)
        cout<<itr->get_value()<<" "<<itr->get_offset()<<endl;
    cout<<"sec_price_index2\n";
    for(vector<element>::iterator itr=sec_price_index2.begin();itr!=sec_price_index2.end();++itr)
        cout<<itr->get_value()<<" "<<itr->get_offset()<<endl;
}
void    system::delete_device(char*              _dev_ID  )
{
    int index=binary_search(prim_ID_index_vec,0,prim_ID_index_vec.size()-1,_dev_ID);
    int _offset=prim_ID_index_vec[index].get_offset();
    char* _value=new char[10];   strcpy(_value,prim_ID_index_vec[index].get_value());
    _value[0]='*';
    element e1(_value,_offset);
    prim_ID_index_vec[index]=e1;
    fstream file1;
    file1.open(data_file,ios::in|ios::out);
    file1.seekp(_offset+2,ios::beg);
    file<<'*';
    file1.close();
    delete []_value;
}
void    system::update_dvice(char*              _dev_ID  )
{
    buffer=new char[MAX_BUFFER_SIZE];   char* update_buffer=new char[MAX_BUFFER_SIZE];
    int index=binary_search(prim_ID_index_vec,0,prim_ID_index_vec.size()-1,_dev_ID);
    int _offset=prim_ID_index_vec[index].get_offset();
    fstream file1;
    file1.open(data_file,ios::in|ios::out);
    file1.seekp(_offset,ios::beg);
    short buff_len;
    file1.read((char*)&buff_len,sizeof(buff_len));
    file1.read(buffer,buff_len);
    file1.close();
    electronic_device printer;
    cin>>printer;
    strcpy(update_buffer,printer.get_dev_ID()); strcat(update_buffer,"|");
    strcat(update_buffer,printer.get_dev_Model()); strcat(update_buffer,"|");
    strcat(update_buffer,float_to_str_converter(printer.get_dev_Price())); strcat(update_buffer,"|");
    if(buff_len==strlen(update_buffer))
    {
        file1.open(data_file,ios::in|ios::out);
        file1.seekp(_offset+2,ios::beg);
        file1.write(update_buffer,strlen(update_buffer));
    }
    else
    {
        delete_device(printer.get_dev_ID());
        add_device(printer);
    }
}


int main()
{

    system user;
    user.interface();
    //cout<<sizeof(bool);

    /*char* buffer=new char[MAX_BUFFER_SIZE];
    char _ID[20],_Model[20],str_Price[20];
    short buff_len;
    fstream file1(data_file,ios::in|ios::out);
    file1.seekg(sizeof(bool),ios::beg);
    file1.read((char*)&buff_len,sizeof(buff_len));
    file1.read(buffer,buff_len);

    istrstream str(buffer);
    str.getline(_ID,20,'|');
    str.getline(_Model,20,'|');
    str.getline(str_Price,20,'|');
    float float_price=str_to_float_converter(str_Price);
    electronic_device e1(_ID,_Model,float_price);
    cout<<e1<<endl;
    file1.close();*/




    return 0;
}
