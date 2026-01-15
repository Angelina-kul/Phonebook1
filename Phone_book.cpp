#include "Phone_book.h"
#include "Validation.h"

using namespace std;

Contact_info::Contact_info() {
	this->Name = "";
	this->Last_Name = "";
	this->Email = "";
	this->Middle_Name = "";
	this->Address = "";
	this->Birth_day = "";
}


void Contact_info::set_Name(const string& Name) { this->Name = Name; }
void Contact_info::set_Last_Name(const string& L_name) { this->Last_Name = L_name; }
void Contact_info::set_Middle_Name(const string& M_name) { this->Middle_Name = M_name; }
void Contact_info::set_Adress(const string& Adress) { this->Address = Adress; }
void Contact_info::set_Birth_day(const string& B_day) { this->Birth_day = B_day; }
void Contact_info::set_Email(const string& email) { this->Email = email; }
void Contact_info::set_sPhone(const string& Pnone_num) { sPhone_nums.push_back(Pnone_num); }
void Contact_info::set_dPhone(const string& Pnone_num) { dPhone_nums.push_back(Pnone_num); }
void Contact_info::set_rPhone(const string& Pnone_num) { rPhone_nums.push_back(Pnone_num); }


string Contact_info::get_Name()const { return Name; }
string Contact_info::get_Last_Name()const { return Last_Name; }
string Contact_info::get_Middle_Name()const { return Middle_Name; }
string Contact_info::get_Adress()const { return Address; }
string Contact_info::get_Birth_day()const { return Birth_day; }
string Contact_info::get_Email() const { return Email; }
vector<string> Contact_info::get_sphones() const { return sPhone_nums; }
vector<string> Contact_info::get_rphones() const { return rPhone_nums; }
vector<string> Contact_info::get_dphones() const { return dPhone_nums; }

string Contact_info::get_phone(int type, int index) const {
	switch (type) {
	case 1:
		if (index >= 0 && index < sPhone_nums.size()) {
			return sPhone_nums[index];
		}
		break;
	case 2:
		if (index >= 0 && index < rPhone_nums.size()) {
			return rPhone_nums[index];
		}
		break;
	case 3:
		if (index >= 0 && index < dPhone_nums.size()) {
			return dPhone_nums[index];
		}
		break;
	}
	return "";
}

bool Contact_info::set_phone(int type, int index, const string& new_phone) {
	if (new_phone.empty()) return false;

	switch (type) {
	case 1:
		if (index >= 0 && index < sPhone_nums.size()) {
			sPhone_nums[index] = new_phone;
			return true;
		}
		break;
	case 2:
		if (index >= 0 && index < rPhone_nums.size()) {
			rPhone_nums[index] = new_phone;
			return true;
		}
		break;
	case 3:
		if (index >= 0 && index < dPhone_nums.size()) {
			dPhone_nums[index] = new_phone;
			return true;
		}
		break;
	}
	return false;
}

bool Contact_info::delete_phone(int type, int index) {
	switch (type) {
	case 1:
		if (index >= 0 && index < sPhone_nums.size()) {
			sPhone_nums.erase(sPhone_nums.begin() + index);
			return true;
		}
		break;
	case 2:
		if (index >= 0 && index < rPhone_nums.size()) {
			rPhone_nums.erase(rPhone_nums.begin() + index);
			return true;
		}
		break;
	case 3:
		if (index >= 0 && index < dPhone_nums.size()) {
			dPhone_nums.erase(dPhone_nums.begin() + index);
			return true;
		}
		break;
	}
	return false;
}

void Contact_info::phone_print(const vector<string>& anyPhone_nums) const {
	if (!anyPhone_nums.empty()) {
		for (int i = 0; i < anyPhone_nums.size(); ++i) {
			cout << "      " << i + 1 << ". " << anyPhone_nums[i] << endl;
		}
	}
}
string Contact_info::phone_serial(const vector<string>& anyPhone_nums)const {
	string result;
	for (int i = 0; i < anyPhone_nums.size(); i++) {
		if (i == 0) {
			result += anyPhone_nums[i];
		}
		else {
			result += ";" + anyPhone_nums[i];
		}
	}
	return result;
}
void Contact_info::phone_deserial(vector<string>& anyPhone_nums, vector<string>& polya, int i)const {
	string phones_str = polya[i];
	if (!phones_str.empty()) {
		istringstream phones_stream(phones_str);
		string phone;
		while (getline(phones_stream, phone, ';')) {
			if (!phone.empty()) {
				anyPhone_nums.push_back(phone);
			}
		}
	}
}

void Contact_info::print_tekush() const {
	cout << "1. Имя: " << Name << endl;
	cout << "2. Фамилия: " << Last_Name << endl;
	if (Middle_Name != "") {
		cout << "3. Отчество: " << Middle_Name << endl;
	}
	cout << "4. Номер телефона:" << endl;
	cout << " Cлужебные номера телефона: " << endl;
	phone_print(sPhone_nums);
	cout << " Рабочие номера телефона: " << endl;
	phone_print(rPhone_nums);
	cout << " Домашние номера телефона: " << endl;
	phone_print(dPhone_nums);
	cout << "5. Email: " << Email << endl;
	if (Address != "") {
		cout << "6. Адрес: " << Address << endl;
	}
	if (Birth_day != "") {
		cout << "7. Дата рождения: " << Birth_day << endl;
	}
	cout << endl;
}


string Contact_info::serialize() const {
	string result;
	result += Name + "|" + Last_Name + "|" + Email + "|";
	result += phone_serial(sPhone_nums);
	result += "|";
	result += phone_serial(rPhone_nums);
	result += "|";
	result += phone_serial(dPhone_nums);
	result += "|" + Middle_Name + "|" + Address + "|" + Birth_day;
	return result;
}
bool Contact_info::deserialize(const string& data) {
	Name.clear();
	Last_Name.clear();
	Email.clear();
	sPhone_nums.clear();
	rPhone_nums.clear();
	dPhone_nums.clear();
	Middle_Name.clear();
	Address.clear();
	Birth_day.clear();

	istringstream iss(data);
	string pole;
	vector<string> polya;

	while (getline(iss, pole, '|')) {
		polya.push_back(pole);
	}

	try {
		Name = polya[0];
		Last_Name = polya[1];
		Email = polya[2];
		phone_deserial(sPhone_nums, polya, 3);
		phone_deserial(rPhone_nums, polya, 4);
		phone_deserial(dPhone_nums, polya, 5);

		if (polya.size() > 6) Middle_Name = polya[6];
		if (polya.size() > 7) Address = polya[7];
		if (polya.size() > 8) Birth_day = polya[8];

		return true;

	}
	catch (const std::exception& e) {
		cerr << "? Исключение при парсинге: " << e.what() << endl;
		return false;
	}
}

bool Phonebook_app::save_file() {
	string filename = "contacts.txt";
	ofstream file(filename);

	if (!file.is_open()) {
		cerr << "Ошибка открытия файла " << filename << endl;
		return false;
	}

	for (const auto& contact : Contacts) {
		string serialized = contact.serialize();
		file << serialized << endl;
	}

	file.close();
	cout << "Контакты успешно сохранены: " << filename << endl;
	return true;
}
bool Phonebook_app::load_file() {
	string filename = "contacts.txt";
	ifstream file(filename);
	if (!file.is_open()) {
		cout << "Ошибка открытия файла" << endl;
		return true;
	}
	Contacts.clear();

	string line;
	int loaded_count = 0;
	int error_count = 0;

	while (getline(file, line)) {
		if (line.empty()) {
			continue;
		}
		Contact_info contact;
		if (contact.deserialize(line)) {
			Contacts.push_back(contact);
			loaded_count++;
		}
		else {
			cerr << "Ошибка парсинга строки: " << line.substr(0, 50) << "..." << endl;
			error_count++;
		}
	}
	file.close();

	cout << "Всего контактов: " << loaded_count << endl << endl;
	if (error_count > 0) {
		cout << "Пропущено строк с ошибками: " << error_count << endl;
	}

	return true;
}

bool Phonebook_app::print_all(){
	if (!Contacts.empty()) {
		for (int i = 0; i < Contacts.size(); i++) {
			cout << "Контакт " << i + 1 << endl;
			Contacts[i].print_tekush();
		}
		return true;
	}
	else {
		cout << "Вы ещё не создали ни одного контакта" << endl;
		return false;
	}
}

bool Phonebook_app::add_reduct_phones(string& phone, Contact_info& contact) {
	int n = 1;
	cout << "Какой номер телефона хотите добавить?" << endl << "1. Служебный" << endl << "2. Рабочий" << endl << "3.Домашний" << endl << "Введите номер:";
	string num_p;
	cin >> num_p;
	cin.ignore();
	if (num_p != "1" && num_p != "2" && num_p != "3") {
		cout << "Неверный номер команды, введите 1, 2, или 3 без точки" << endl << endl;
		return false;
	}

	do {
		cout << "Введите номер телефона: ";
		getline(cin, phone);
		probeli(phone); 
		if (phone.empty()) {
			cout << "Номер не может быть пустым. Попробуйте снова." << endl;
		}
	} while (!valid_Phone(phone));

	if (num_p == "1") {
		contact.set_sPhone(phone);
	}
	else if (num_p == "2") {
		contact.set_rPhone(phone);
	}
	else if (num_p == "3") {
		contact.set_dPhone(phone);
	}

	return true;
}

bool Phonebook_app::add_contact() {
	Contact_info contact;
	string f_name, l_name, email, phone, m_name, address, b_day;
	cout << "Заполните обязательные поля для создания контакта:" << endl;
	do {
		cout << "Введите имя: ";
		getline(cin, f_name);
	} while (!valid_Name(f_name));
	contact.set_Name(f_name);
	do {
		cout << "Введите фамилию: ";
		getline(cin, l_name);
	} while (!valid_Name(l_name));
	contact.set_Last_Name(l_name);
	do {
		cout << "Введите почту: ";
		getline(cin, email);
	} while (!valid_Email(email));
	contact.set_Email(email);
	
	bool phone_added = false;
	while (!phone_added) {
		phone_added = add_reduct_phones(phone, contact);
	}
	string otvet;
	do {
		cout << "Хотите добавить еще один номер?(введите да/нет)" << endl;
		getline(cin, otvet);
		if (otvet == "да" || otvet == "Да") {
			add_reduct_phones(phone, contact);
		}
		else if (otvet == "нет" || otvet == "Нет") {
			break;
		}
	} while (otvet != "да" || otvet != "Да" || otvet != "нет" || otvet != "Нет");
	
	cout << "Необязательные поля для создания контакта, нажмите enter, чтобы пропустить:" << endl;
	cout << "Введите отчество (нажмите Enter чтобы пропустить): ";
	getline(cin, m_name);
	if (!m_name.empty()) {
		while (!valid_Name(m_name)) {
			cout << "Введите отчество (нажмите Enter чтобы пропустить): ";
			getline(cin, m_name);
			if (m_name.empty()) break;
		}
	}
	contact.set_Middle_Name(m_name);
	cout << "Введите адрес (нажмите Enter чтобы пропустить): ";
	getline(cin, address);
	contact.set_Adress(address);

	cout << "Введите дату рождения (нажмите Enter чтобы пропустить): ";
	getline(cin, b_day);
	if (!b_day.empty()) {
		while (!valid_Bday(b_day)) {
			cout << "Введите дату рождения (нажмите Enter чтобы пропустить): ";
			getline(cin, b_day);
			if (b_day.empty()) break;
		}
	}
	contact.set_Birth_day(b_day);

	contact.print_tekush();
	
	do {
		cout << "Хотите сохранить контакт?(введите да/нет) ";
		getline(cin, otvet);
		if (otvet == "да" || otvet == "Да") {
			Contacts.push_back(contact);
			return save_file();
		}
		else if (otvet == "нет" || otvet == "Нет") {
			cout << "Контакт не сохранён" << endl;
			break;
		}
		else {
			cout << "Неверный ввод команды, введите 'да' или 'нет'" << endl;
		}
	} while (otvet != "да" || otvet != "Да" || otvet != "нет" || otvet != "Нет");
}

void Phonebook_app::delete_contact() {
	if (Contacts.empty()) {
		cout << "Нет контактов для удаления." << endl;
		return;
	}
	int num;
	cout << "          Контакты          " << endl;
	print_all();
	cout << "Введите порядковый номер контактa, который хотите удалить: ";
	cin >> num;
	cin.clear();
	if (num < 1 || num > Contacts.size()) {
		cout << "Неверный номер контакта!" << endl;
		return;
	}
	Contacts.erase(Contacts.begin() + num - 1);
	save_file();
	
}

void Phonebook_app::redact_contact() {
	if (Contacts.empty()) {
		cout << "Телефонная книга пуста." << endl;
		return;
	}
	string f_name, l_name, email, phone, m_name, address, b_day;

	int num;
	cout << "          Контакты          " << endl;
	print_all();
	cout << "Введите порядковый номер контактa, который хотите редактировать: ";
	cin >> num;
	cin.ignore();
	if (num < 1 || num > Contacts.size()) {
		cout << "Неверный номер контакта!" << endl;
		return;
	}

	Contact_info& contact = Contacts[num - 1];
	cout << "Введите порядковый номер поля, которое хотите редактировать: ";
	cin >> num;
	cin.ignore();
	string otvet;
	switch (num) {
	case 1:
		do {
			cout << "Введите новое имя: " << endl;
			getline(cin, f_name);
		} while (!valid_Name(f_name));
		contact.set_Name(f_name);
		break;
	case 2:
		do {
			cout << "Введите новую фамилию: " << endl;
			getline(cin, l_name);
		} while (!valid_Name(l_name));
		contact.set_Last_Name(l_name);
		break;
	case 3:
		do {
			cout << "Введите новое отчество (нажмите Enter чтобы пропустить): ";
			getline(cin, m_name);
		} while (!valid_Name(m_name));
		contact.set_Middle_Name(m_name);
		break;
	case 4:
	{
		bool editing_phones = true;
		while (editing_phones) {
			cout << "      Редактирование контактов      " << endl;
			cout << "Текущие телефоны контакта:" << endl;

			vector<string> sPhones = contact.get_sphones();
			vector<string> rPhones = contact.get_rphones();
			vector<string> dPhones = contact.get_dphones();

			int phone_counter = 1;
			vector<pair<int, int>> phone_map;

			for (int i = 0; i < sPhones.size(); i++) {
				cout << phone_counter++ << ". Служебный " << sPhones[i] << endl;
				phone_map.push_back({ 1, i });
			}

			for (int i = 0; i < rPhones.size(); i++) {
				cout << phone_counter++ << ". Рабочий " << rPhones[i] << endl;
				phone_map.push_back({ 2, i });
			}

			for (int i = 0; i < dPhones.size(); i++) {
				cout << phone_counter++ << ". Домашний " << dPhones[i] << endl;
				phone_map.push_back({ 3, i });
			}

			if (phone_counter == 1) {
				cout << "Телефонов нет" << endl;
			}

			cout << "Выберите действие:" << endl;
			cout << "1. Добавить новый телефон" << endl;
			cout << "2. Редактировать существующий" << endl;
			cout << "3. Удалить телефон" << endl;
			cout << "0. Завершить редактирование телефонов" << endl;
			cout << "Ваш выбор: ";

			char choice;
			cin >> choice;
			cin.ignore();

			if (choice == '1') {
				string phone;
				if (add_reduct_phones(phone, contact)) {
					cout << "Телефон добавлен успешно!" << endl;
				}
				else {
					cout << "Ошибка при добавлении телефона." << endl;
				}
				save_file();
				continue;
			}
			else if (choice == '2' && phone_counter > 1) {
				cout << "Введите номер телефона для редактирования (1-" << (phone_counter - 1) << "): ";
				string num_str;
				getline(cin, num_str);
				int target_num = -1;
				try {
					target_num = stoi(num_str);
				}
				catch (...) {
					cout << "Неверный ввод!" << endl;
					continue;
				}

				if (target_num < 1 || target_num >= phone_counter) {
					cout << "Неверный номер!" << endl;
					continue;
				}

				pair<int, int> phone_info = phone_map[target_num - 1];
				int type = phone_info.first;
				int index = phone_info.second;

				string current_phone = contact.get_phone(type, index);
				cout << "Текущий номер: " << current_phone << endl;

				cout << "Введите новый номер телефона: ";
				string new_phone;
				getline(cin, new_phone);

				if (new_phone.empty()) {
					cout << "Номер не изменен (пустая строка)." << endl;
					continue;
				}

				if (contact.set_phone(type, index, new_phone)) {
					cout << "Телефон успешно изменен!" << endl;
					save_file(); 
				}
				else {
					cout << "Ошибка при изменении телефона!" << endl;
				}
			}
			else if (choice == '3' && phone_counter > 1) {
				cout << "Введите номер телефона для удаления (1-" << (phone_counter - 1) << "): ";
				string num_str;
				getline(cin, num_str);
				int target_num = -1;
				try {
					target_num = stoi(num_str);
				}
				catch (...) {
					cout << "Неверный ввод!" << endl;
					continue;
				}

				if (target_num < 1 || target_num >= phone_counter) {
					cout << "Неверный номер!" << endl;
					continue;
				}

				pair<int, int> phone_info = phone_map[target_num - 1];
				int type = phone_info.first;
				int index = phone_info.second;

				string phone_to_delete = contact.get_phone(type, index);
				cout << "Вы действительно хотите удалить номер: " << phone_to_delete << "? (да/нет): ";
				string confirm;
				getline(cin, confirm);

				if (confirm == "да" || confirm == "Да") {
					if (contact.delete_phone(type, index)) {
						cout << "Телефон удален успешно!" << endl;
						save_file();
					}
					else {
						cout << "Ошибка при удалении телефона!" << endl;
					}
				}
				else {
					cout << "Удаление отменено." << endl;
				}
			}
			else if (choice == '0') {
				editing_phones = false;
				cout << "Редактирование телефонов завершено." << endl;
			}
			else {
				cout << "Неверный выбор!" << endl;
			}
		}
		break;
	}
	case 5:
		do {
			cout << "Введите новую почту: " << endl;
			getline(cin, email);
		} while (!valid_Email(email));
		contact.set_Email(email);
		break;
	case 6:
		cout << "Введите новый адрес (нажмите Enter чтобы пропустить): ";
		getline(cin, address);
		contact.set_Adress(address);
		break;
	case 7:
		do {
			cout << "Введите новую дату рождение(пример: 01.01.2001) (нажмите Enter чтобы пропустить): ";
			getline(cin, b_day);
		} while (!valid_Bday(b_day));
		contact.set_Birth_day(b_day);
		break;
	}

}

	bool searchInPhones(const vector<string>& phones, const string& search_term) {
		for (int i = 0; i < phones.size(); i++) {
			if (!phones[i].empty() && search_term == phones[i]) {
				return true;
			}
		}
		return false;
	}


	void Phonebook_app::search() {
		if (Contacts.empty()) {
			cout << "Телефонная книга пуста." << endl;
			return;
		}

		cout << "Введите информацию для поиска: ";
		string search_p;
		getline(cin, search_p);

		if (search_p.empty()) {
			cout << "Поисковый запрос не может быть пустым!" << endl;
			return;
		}

		bool found = false;

		for (int i = 0; i < Contacts.size(); i++) {
			string found_by = "";
			if (!Contacts[i].get_Name().empty() && search_p == Contacts[i].get_Name()) {
				found_by = "Имя";
			}
			else if (!Contacts[i].get_Last_Name().empty() && search_p == Contacts[i].get_Last_Name()) {
				found_by = "Фамилия";
			}
			else if (!Contacts[i].get_Middle_Name().empty() && search_p == Contacts[i].get_Middle_Name()) {
				found_by = "Отчество";
			}
			else if (!Contacts[i].get_Email().empty() && search_p == Contacts[i].get_Email()) {
				found_by = "Email";
			}
			else if (!Contacts[i].get_Adress().empty() && search_p == Contacts[i].get_Adress()) {
				found_by = "Адрес";
			}
			else if (!Contacts[i].get_Birth_day().empty() && search_p == Contacts[i].get_Birth_day()) {
				found_by = "Дата рождения";
			}
			else if (searchInPhones(Contacts[i].get_sphones(), search_p)) {
				found_by = "Служебный телефон";
			}
			else if (searchInPhones(Contacts[i].get_rphones(), search_p)) {
				found_by = "Рабочий телефон";
			}
			else if (searchInPhones(Contacts[i].get_dphones(), search_p)) {
				found_by = "Домашний телефон";
			}

			if (!found_by.empty()) {
				cout << "\nКонтакт найден по полю '" << found_by << "':" << endl;
				Contacts[i].print_tekush();
				found = true;
				cout << endl;
			}
		}

		if (!found) {
			cout << "Контакты не найдены." << endl;
		}
	}

void Phonebook_app::sorting() {
	if (Contacts.empty()) {
		cout << "Телефонная книга пуста." << endl;
		return;
	}

	cout << "      Сортировка контактов      " << endl;
	cout << "Выберите поле для сортировки:" << endl;
	cout << "1. По имени" << endl;
	cout << "2. По фамилии" << endl;
	cout << "3. По отчеству" << endl;
	cout << "4. По email" << endl;
	cout << "5. По адресу" << endl;
	cout << "6. По дате рождения" << endl;
	cout << "Ваш выбор: ";

	int choice;
	cin >> choice;
	cin.ignore();

	if (choice < 1 || choice > 6) {
		cout << "Неверный выбор!" << endl;
		return;
	}

	vector<Contact_info> sorted_contacts = Contacts;

	for (int i = 0; i < sorted_contacts.size() - 1; i++) {
		for (int j = i + 1; j < sorted_contacts.size(); j++) {
			bool need_swap = false;

			switch (choice) {
			case 1: 
				if (sorted_contacts[i].get_Name() > sorted_contacts[j].get_Name()) {
					need_swap = true;
				}
				break;
			case 2: 
				if (sorted_contacts[i].get_Last_Name() > sorted_contacts[j].get_Last_Name()) {
					need_swap = true;
				}
				break;
			case 3:
				if (sorted_contacts[i].get_Middle_Name() > sorted_contacts[j].get_Middle_Name()) {
					need_swap = true;
				}
				break;
			case 4:
				if (sorted_contacts[i].get_Email() > sorted_contacts[j].get_Email()) {
					need_swap = true;
				}
				break;
			case 5:
				if (sorted_contacts[i].get_Adress() > sorted_contacts[j].get_Adress()) {
					need_swap = true;
				}
				break;
			case 6:
				if (sorted_contacts[i].get_Birth_day() > sorted_contacts[j].get_Birth_day()) {
					need_swap = true;
				}
				break;
			}

			if (need_swap) {
				Contact_info temp = sorted_contacts[i];
				sorted_contacts[i] = sorted_contacts[j];
				sorted_contacts[j] = temp;
			}
		}
	}

	cout << "      Отсортированнные контакты      " << endl;
	for (int i = 0; i < sorted_contacts.size(); i++) {
		cout << "Контакт " << i + 1 << endl;
		sorted_contacts[i].print_tekush();
	}

	string answer;
	cout << "Хотите сохранить эту сортировку? (да/нет): ";
	getline(cin, answer);

	if (answer == "да" || answer == "Да") {
		Contacts = sorted_contacts;
		save_file();
		cout << "Сортировка сохранена." << endl;
	}
	else {
		cout << "Сортировка не сохранена." << endl;
	}
}