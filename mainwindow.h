#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDialog>
#include <QFormLayout>
#include <QTextEdit>
#include "Phone_book.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void onAddClicked();
    void onDeleteClicked();
    void onEditClicked();
    void onSearchTextChanged(const QString& text);

private:
    void setupUI();
    void updateTable();
    void showContactDialog(Contact_info& contact, bool isNew = false);
    void loadContactsFromDB();
    void saveContactToDB(const Contact_info& c);
    void deleteAllContactsFromDB();

    Phonebook_app m_phonebook;
    QTableWidget* table;
    QPushButton* btnAdd;
    QPushButton* btnEdit;
    QPushButton* btnDelete;
    QLineEdit* searchBox;
};

#endif // MAINWINDOW_H
