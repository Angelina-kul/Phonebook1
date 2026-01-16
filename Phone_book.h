#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>


using namespace std;


class Contact_info {
private:
    string Name;
    string Last_Name;
    string Middle_Name;
    string Address;
    string Birth_day;
    string Email;
    vector<string> sPhone_nums;
    vector<string> dPhone_nums;
    vector<string> rPhone_nums;
public:
    Contact_info();

    void set_Name(const string& Name);
    void set_Last_Name(const string& L_name);
    void set_Middle_Name(const string& M_name);
    void set_Adress(const string& Address);
    void set_Birth_day(const string& B_day);
    void set_Email(const string& email);
    void set_sPhone(const string& Pnone_num);
    void set_rPhone(const string& Pnone_num);
    void set_dPhone(const string& Pnone_num);

    string get_Name()const;
    string get_Last_Name()const;
    string get_Middle_Name()const;
    string get_Adress()const;
    string get_Birth_day()const;
    string get_Email() const;
    vector<string> get_sphones() const;
    vector<string> get_rphones() const;
    vector<string> get_dphones() const;

    string get_phone(int type, int index) const;
    bool set_phone(int type, int index, const string& new_phone);
    bool delete_phone(int type, int index);
    void phone_print(const vector<string>& anyPhone_nums)const;
    string phone_serial(const vector<string>& anyPhone_nums)const;
    void phone_deserial(vector<string>& anyPhone_nums, vector<string>& polya, int i)const;

    void print_tekush() const;

    string serialize() const;
    bool deserialize(const std::string& data);
};


class Phonebook_app {
private:
    vector<Contact_info> Contacts;
public:
    size_t contactCount() const;
    Contact_info& getContact(size_t index);
    void addContact(const Contact_info& c);
    void removeContact(size_t index);
    void clearContacts();

    bool save_file();
    bool load_file();
    bool print_all();

    bool add_reduct_phones(string& phone, Contact_info& contact);

    bool add_contact();
    void delete_contact();
    void redact_contact();
    void search();
    void sorting();

};

