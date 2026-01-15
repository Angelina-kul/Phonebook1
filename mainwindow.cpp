#include "mainwindow.h"
#include "Validation.h"
#include <QDebug>
#include <QTextEdit>
#include <QComboBox>
#include <QLabel>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_phonebook.load_file();
    setupUI();
    updateTable();
}

void MainWindow::setupUI() {
    setWindowTitle("Телефонный справочник (Задача 2)");
    resize(1000, 600);

    auto* central = new QWidget(this);
    setCentralWidget(central);

    auto* layout = new QVBoxLayout(central);

    auto* topLayout = new QHBoxLayout();

    searchBox = new QLineEdit();
    searchBox->setPlaceholderText("Поиск по всем полям...");
    topLayout->addWidget(searchBox);

    btnAdd = new QPushButton("Добавить");
    btnEdit = new QPushButton("Редактировать");
    btnDelete = new QPushButton("Удалить");
    topLayout->addWidget(btnAdd);
    topLayout->addWidget(btnEdit);
    topLayout->addWidget(btnDelete);
    layout->addLayout(topLayout);

    table = new QTableWidget(this);
    table->setColumnCount(7);
    table->setHorizontalHeaderLabels({
        "Фамилия", "Имя", "Отчество", "Email", "Телефоны", "Адрес", "Дата рождения"
    });
    table->setSortingEnabled(true);
    table->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(table);

    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::onAddClicked);
    connect(btnEdit, &QPushButton::clicked, this, &MainWindow::onEditClicked);
    connect(btnDelete, &QPushButton::clicked, this, &MainWindow::onDeleteClicked);
    connect(searchBox, &QLineEdit::textChanged, this, &MainWindow::onSearchTextChanged);
}

void MainWindow::updateTable() {
    table->setRowCount(static_cast<int>(m_phonebook.contactCount()));
    for (size_t i = 0; i < m_phonebook.contactCount(); ++i) {
        const Contact_info& c = m_phonebook.getContact(i);

        auto setItem = [&](int row, int col, const std::string& s) {
            if (!s.empty())
                table->setItem(row, col, new QTableWidgetItem(QString::fromStdString(s)));
        };

        setItem(i, 0, c.get_Last_Name());
        setItem(i, 1, c.get_Name());
        setItem(i, 2, c.get_Middle_Name());
        setItem(i, 3, c.get_Email());

        std::string allPhones =
            (c.get_sphones().empty() ? "" : "С: " + c.phone_serial(c.get_sphones()) + "\n") +
            (c.get_rphones().empty() ? "" : "Р: " + c.phone_serial(c.get_rphones()) + "\n") +
            (c.get_dphones().empty() ? "" : "Д: " + c.phone_serial(c.get_dphones()));
        setItem(i, 4, allPhones);

        setItem(i, 5, c.get_Adress());
        setItem(i, 6, c.get_Birth_day());
    }
}

void MainWindow::showContactDialog(Contact_info& contact, bool isNew) {
    QDialog dialog(this);
    dialog.setWindowTitle(isNew ? "Новый контакт" : "Редактировать контакт");
    dialog.resize(300, 450);

    auto* layout = new QFormLayout(&dialog);

    auto* leLastName = new QLineEdit(QString::fromStdString(contact.get_Last_Name()));
    auto* leName = new QLineEdit(QString::fromStdString(contact.get_Name()));
    auto* leEmail = new QLineEdit(QString::fromStdString(contact.get_Email()));
    auto* leMiddleName = new QLineEdit(QString::fromStdString(contact.get_Middle_Name()));
    auto* leAddress = new QLineEdit(QString::fromStdString(contact.get_Adress()));
    auto* leBirthDate = new QLineEdit(QString::fromStdString(contact.get_Birth_day()));

    auto* lePhones = new QTextEdit(); // ✅ Теперь известен тип!
    std::string allPhones;
    for (const auto& p : contact.get_sphones()) allPhones += "С:" + p + "\n";
    for (const auto& p : contact.get_rphones()) allPhones += "Р:" + p + "\n";
    for (const auto& p : contact.get_dphones()) allPhones += "Д:" + p + "\n";
    lePhones->setPlainText(QString::fromStdString(allPhones));
    lePhones->setPlaceholderText("Формат:\nС:+7999...\nР:+7888...\nД:+7777...");

    layout->addRow("Фамилия*", leLastName);
    layout->addRow("Имя*", leName);
    layout->addRow("Email*", leEmail);
    layout->addRow("Отчество", leMiddleName);
    layout->addRow("Адрес", leAddress);
    layout->addRow("Дата рождения", leBirthDate);
    layout->addRow("Телефоны", lePhones);

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttons);
    QObject::connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) return;

    std::string lastName = leLastName->text().toStdString();
    std::string name = leName->text().toStdString();
    std::string email = leEmail->text().toStdString();
    std::string middleName = leMiddleName->text().toStdString();
    std::string address = leAddress->text().toStdString();
    std::string birthDate = leBirthDate->text().toStdString();

    if (!valid_Name(lastName)) { QMessageBox::warning(this, "Ошибка", "Некорректная фамилия"); return; }
    if (!valid_Name(name)) { QMessageBox::warning(this, "Ошибка", "Некорректное имя"); return; }
    if (!valid_Email(email)) { QMessageBox::warning(this, "Ошибка", "Некорректный email"); return; }
    if (!middleName.empty() && !valid_Name(middleName)) { QMessageBox::warning(this, "Ошибка", "Некорректное отчество"); return; }
    if (!birthDate.empty() && !valid_Bday(birthDate)) { QMessageBox::warning(this, "Ошибка", "Некорректная дата рождения"); return; }

    contact.set_Last_Name(lastName);
    contact.set_Name(name);
    contact.set_Email(email);
    contact.set_Middle_Name(middleName);
    contact.set_Adress(address);
    contact.set_Birth_day(birthDate);

    contact.get_sphones().clear();
    contact.get_rphones().clear();
    contact.get_dphones().clear();

    QString phonesText = lePhones->toPlainText();
    QStringList lines = phonesText.split('\n', Qt::SkipEmptyParts);
    for (const QString& line : lines) {
        QString trimmed = line.trimmed();
        if (trimmed.isEmpty()) continue;
        if (trimmed.startsWith("С:")) {
            std::string phone = trimmed.mid(2).toStdString();
            if (valid_Phone(phone)) contact.set_sPhone(phone);
        } else if (trimmed.startsWith("Р:")) {
            std::string phone = trimmed.mid(2).toStdString();
            if (valid_Phone(phone)) contact.set_rPhone(phone);
        } else if (trimmed.startsWith("Д:")) {
            std::string phone = trimmed.mid(2).toStdString();
            if (valid_Phone(phone)) contact.set_dPhone(phone);
        }
    }

    if (isNew) {
        m_phonebook.addContact(contact);
    }
    m_phonebook.save_file();
    updateTable();
}

void MainWindow::onAddClicked() {
    Contact_info contact;
    showContactDialog(contact, true);
}

void MainWindow::onEditClicked() {
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите контакт для редактирования");
        return;
    }
    Contact_info& contact = m_phonebook.getContact(row);
    showContactDialog(contact, false);
}

void MainWindow::onDeleteClicked() {
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите контакт для удаления");
        return;
    }
    if (QMessageBox::question(this, "Подтверждение", "Удалить контакт?") == QMessageBox::Yes) {
        m_phonebook.removeContact(row);
        m_phonebook.save_file();
        updateTable();
    }
}

void MainWindow::onSearchTextChanged(const QString& text) {
    QString searchTerm = text.trimmed().toLower();
    for (int i = 0; i < table->rowCount(); ++i) {
        bool match = false;
        for (int j = 0; j < table->columnCount(); ++j) {
            QTableWidgetItem* item = table->item(i, j);
            if (item && !item->text().isEmpty()) {
                if (item->text().toLower().contains(searchTerm)) {
                    match = true;
                    break;
                }
            }
        }
        table->setRowHidden(i, !match);
    }
}
