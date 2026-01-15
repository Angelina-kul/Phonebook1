#include <iostream>
#include <windows.h>
#include "Phone_book.h"
using namespace std;

void spicok_deystv() {
	cout << "Выберите номер действие:" << endl;
	cout << "1. Добавить контакт." << endl;
	cout << "2. Удалить контакт." << endl;
	cout << "3. Редактировать контакт." << endl;
	cout << "4. Поиск контакта." << endl;
	cout << "5. Сортировать контакты." << endl;
	cout << "6. Показать все контакты." << endl;
	cout << "7. Выход." << endl;
}


int main() {
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	int n = 0;
	Phonebook_app app;
	app.load_file();

	do {
		spicok_deystv();

		cin >> n;
		cin.ignore();
		switch (n) {
		case 1:
			app.add_contact();
			break;
		case 2:
			app.delete_contact();
			break;
		case 3:
			app.redact_contact();
			break;
		case 4:
			app.search();
			break;
		case 5:
			app.sorting();
		case 6:
			app.print_all();
			break;
		case 7:
			cout << "Завершение работы программы" << endl;
			break;
		default:
			cout << "Дейстие не найдено, попробуйте еще" << endl<< endl;
			break;
		}
	} while (n != 7);
}