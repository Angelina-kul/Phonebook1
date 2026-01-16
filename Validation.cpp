#include "Validation.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>
using namespace std;


void  probeli(string& s) {
    if (s.empty()) return;

    int start = 0;
    while (start < (int)s.length() && (s[start] == ' ' || s[start] == '\t' || s[start] == '\r' || s[start] == '\n')) {
        start++;
    }

    int end = s.length() - 1;
    while (end >= 0 && (s[end] == ' ' || s[end] == '\t' || s[end] == '\r' || s[end] == '\n')) {
        end--;
    }

    if (start > end) {
        s = "";
    }
    else {
        s = s.substr(start, end - start + 1);
    }
}

bool isAlphaUnicode(unsigned char c) {
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) return true;
    if (c >= 128) return true;
    return false;
}

bool isLatinAlnum(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

bool isAlnumUnicode(unsigned char c) {
    return isdigit(c) || isAlphaUnicode(c);
}


bool valid_Name(const string& name) {
    if (name.empty()) {
        cout << "Поле не может быть пустым.\n";
        return false;
    }

    string new_name = name;
    probeli(new_name);
    if (new_name.empty()) {
        cout << "Поле не может состоять только из пробелов.\n";
        return false;
    }

    if (!isAlphaUnicode((unsigned char)new_name[0])) {
        cout << "Поле должно начинаться с буквы.\n";
        return false;
    }

    if (new_name[new_name.length() - 1] == '-') {
        cout << "Поле не может заканчиваться на дефис.\n";
        return false;
    }

    for (int i = 0; i < (int)new_name.length(); i++) {
        unsigned char c = (unsigned char)new_name[i];
        if (isAlnumUnicode(c) || new_name[i] == '-' || new_name[i] == ' ') {
            continue;
        }
        cout << "Разрешены только буквы, цифры, дефис и пробел.\n";
        return false;
    }

    return true;
}

bool valid_Phone(const string& phone) {
    if (phone.empty()) {
        cout << "Номер не может быть пустым.\n";
        return false;
    }

    string normalized = "";
    for (int i = 0; i < (int)phone.length(); i++) {
        if (isdigit(phone[i])) {
            normalized += phone[i];
        }
    }

    if (normalized.length() != 11) {
        cout << "Номер должен содержать ровно 11 цифр (например: 88121234567).\n";
        return false;
    }

    if (normalized[0] != '7' && normalized[0] != '8') {
        cout << "Номер должен начинаться с 7 или 8.\n";
        return false;
    }

    return true;
}

bool valid_Email(const string& email) {
    if (email.empty()) {
        cout << "Email не может быть пустым.\n";
        return false;
    }

    string new_email = email;
    string noSpaces = "";
    for (int i = 0; i < (int)new_email.length(); i++) {
        if (email[i] != ' ') {
            noSpaces += email[i];
        }
    }
    new_email = noSpaces;

    int atCount = 0;
    for (int i = 0; i < (int)new_email.length(); i++) {
        if (new_email[i] == '@') atCount++;
    }

    if (atCount != 1) {
        cout << "Email должен содержать ровно один символ '@'.\n";
        return false;
    }

    int atPos = -1;
    for (int i = 0; i < (int)new_email.length(); i++) {
        if (new_email[i] == '@') {
            atPos = i;
            break;
        }
    }

    string username = "";
    string domain = "";
    for (int i = 0; i < atPos; i++) username += new_email[i];
    for (int i = atPos + 1; i < (int)new_email.length(); i++) domain += new_email[i];

    if (username.empty()) {
        cout << "Имя пользователя (до '@') не может быть пустым.\n";
        return false;
    }
    if (domain.empty()) {
        cout << "Домен (после '@') не может быть пустым.\n";
        return false;
    }

    for (int i = 0; i < (int)username.length(); i++) {
        if (!isLatinAlnum(username[i])) {
            cout << "? Имя пользователя может содержать только латинские буквы и цифры.\n";
            return false;
        }
    }

    if (domain[0] == '.' || domain[domain.length() - 1] == '.') {
        cout << "Домен не может начинаться или заканчиваться на точку.\n";
        return false;
    }

    for (int i = 0; i < (int)domain.length(); i++) {
        char c = domain[i];
        if (isLatinAlnum(c)) {
            continue;
        }
        else if (c == '.') {
            if (i + 1 < (int)domain.length() && domain[i + 1] == '.') {
                cout << "В домене не может быть двух точек подряд.\n";
                return false;
            }
        }
        else {
            cout << "Домен может содержать только латинские буквы, цифры и точки.\n";
            return false;
        }
    }
    return true;
}

bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int daysInMonth(int month, int year) {
    static const int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (month == 2 && isLeapYear(year)) return 29;
    return days[month - 1];
}

bool valid_Bday(const string& date) {
    string dateStr = date;
    probeli(dateStr);
    if (dateStr.empty()) {
        return true;
    }

    if (dateStr.length() != 10 || dateStr[2] != '.' || dateStr[5] != '.') {
        cout << "Формат даты: ДД.ММ.ГГГГ (например, 01.01.2001).\n";
        return false;
    }

    string dd = "", mm = "", yyyy = "";
    dd += dateStr[0]; dd += dateStr[1];
    mm += dateStr[3]; mm += dateStr[4];
    yyyy += dateStr[6]; yyyy += dateStr[7]; yyyy += dateStr[8]; yyyy += dateStr[9];

    bool allDigits = true;
    string parts[3] = { dd, mm, yyyy };
    for (int p = 0; p < 3; p++) {
        string part = parts[p];
        for (int i = 0; i < (int)part.length(); i++) {
            if (!isdigit(part[i])) {
                allDigits = false;
                break;
            }
        }
        if (!allDigits) break;
    }

    if (!allDigits) {
        cout << "Дата должна содержать только цифры в формате ДД.ММ.ГГГГ.\n";
        return false;
    }

    int day = 0, month = 0, year = 0;
    for (int i = 0; i < 2; i++) day = day * 10 + (dd[i] - '0');
    for (int i = 0; i < 2; i++) month = month * 10 + (mm[i] - '0');
    for (int i = 0; i < 4; i++) year = year * 10 + (yyyy[i] - '0');

    if (month < 1 || month > 12) {
        cout << "Месяц должен быть от 1 до 12.\n";
        return false;
    }

    int maxDay = daysInMonth(month, year);
    if (day < 1 || day > maxDay) {
        cout << "Некорректный день для указанного месяца.\n";
        return false;
    }

    const int CURR_YEAR = 2026;
    const int CURR_MONTH = 1;
    const int CURR_DAY = 16;

    if (year > CURR_YEAR) {
        cout << "Дата рождения не может быть в будущем.\n";
        return false;
    }

    if (year == CURR_YEAR && month > CURR_MONTH) {
        cout << "Дата рождения не может быть в будущем.\n";
        return false;
    }
    if (year == CURR_YEAR && month == CURR_MONTH && day >= CURR_DAY) {
        cout << "Дата рождения должна быть строго раньше 15.01.2026.\n";
        return false;
    }

    return true;
}
