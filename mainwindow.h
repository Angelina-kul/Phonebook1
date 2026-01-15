// mainwindow.h
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
#include <QLineEdit>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QHeaderView>
#include "Phone_book.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void onAddClicked();
    void onDeleteClicked();
    void onEditClicked();      // ← новое
    void onSearchTextChanged(const QString& text); // ← новое

private:
    void setupUI();
    void updateTable();
    void showContactDialog(Contact_info& contact, bool isNew = false); // ← общая функция

    Phonebook_app m_phonebook;
    QTableWidget* table;
    QPushButton* btnAdd;
    QPushButton* btnEdit;
    QPushButton* btnDelete;
    QLineEdit* searchBox; // ← новое
};

#endif // MAINWINDOW_H
