#include "mainwindow.h"
#include "Validation.h"
#include <QDebug>
#include <QTextEdit>
#include <QLabel>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>

// SQL headers
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setPort(5432);
    db.setDatabaseName("phonebook");
    db.setUserName("postgres");
    db.setPassword("Angelk228");

    if (!db.open()) {
        QMessageBox::critical(this, "Connection Error",
            "Failed to connect to database:\n" + db.lastError().text());
        return;
    }

    loadContactsFromDB();
    setupUI();
    updateTable();
}

void MainWindow::loadContactsFromDB() {
    m_phonebook.clearContacts();

    QSqlQuery query;
    query.prepare("SELECT last_name, first_name, middle_name, email, address, birth_date, s_phone, r_phone, d_phone FROM contacts ORDER BY last_name");

    if (!query.exec()) {
        qDebug() << "Load error:" << query.lastError();
        return;
    }

    while (query.next()) {
        Contact_info c;
        c.set_Last_Name(query.value(0).toString().toStdString());
        c.set_Name(query.value(1).toString().toStdString());
        c.set_Middle_Name(query.value(2).toString().toStdString());
        c.set_Email(query.value(3).toString().toStdString());
        c.set_Adress(query.value(4).toString().toStdString());
        c.set_Birth_day(query.value(5).toString().toStdString());
        c.set_sPhone(query.value(6).toString().toStdString());
        c.set_rPhone(query.value(7).toString().toStdString());
        c.set_dPhone(query.value(8).toString().toStdString());

        m_phonebook.addContact(c);
    }
    qDebug() << "Loaded contacts:" << m_phonebook.contactCount();
}

void MainWindow::saveContactToDB(const Contact_info& c) {
    QSqlQuery query;
    query.prepare("INSERT INTO contacts (last_name, first_name, middle_name, email, address, birth_date, s_phone, r_phone, d_phone) "
                  "VALUES (:last, :first, :middle, :email, :addr, :birth, :s_phone, :r_phone, :d_phone)");
    query.bindValue(":last", QString::fromStdString(c.get_Last_Name()));
    query.bindValue(":first", QString::fromStdString(c.get_Name()));
    query.bindValue(":middle", QString::fromStdString(c.get_Middle_Name()));
    query.bindValue(":email", QString::fromStdString(c.get_Email()));
    query.bindValue(":addr", QString::fromStdString(c.get_Adress()));
    QString birthDate = QString::fromStdString(c.get_Birth_day());
    query.bindValue(":birth", birthDate.isEmpty() ? QVariant() : birthDate);
    query.bindValue(":s_phone", QString::fromStdString(c.get_sphones().empty() ? "" : c.get_sphones().front()));
    query.bindValue(":r_phone", QString::fromStdString(c.get_rphones().empty() ? "" : c.get_rphones().front()));
    query.bindValue(":d_phone", QString::fromStdString(c.get_dphones().empty() ? "" : c.get_dphones().front()));

    if (!query.exec()) {
        qDebug() << "Save error:" << query.lastError();
        QMessageBox::warning(this, "Error", "Failed to save contact");
    } else {
        qDebug() << "Contact saved successfully";
    }
}

void MainWindow::deleteAllContactsFromDB() {
    QSqlQuery query;
    query.exec("DELETE FROM contacts");
}

void MainWindow::setupUI() {
    setWindowTitle("Phonebook (Task 3)");
    resize(1000, 600);

    auto* central = new QWidget(this);
    setCentralWidget(central);

    auto* layout = new QVBoxLayout(central);

    auto* topLayout = new QHBoxLayout();
    searchBox = new QLineEdit();
    searchBox->setPlaceholderText("Search all fields...");
    btnAdd = new QPushButton("Add");
    btnEdit = new QPushButton("Edit");
    btnDelete = new QPushButton("Delete");

    topLayout->addWidget(searchBox);
    topLayout->addWidget(btnAdd);
    topLayout->addWidget(btnEdit);
    topLayout->addWidget(btnDelete);
    layout->addLayout(topLayout);

    table = new QTableWidget(this);
    table->setColumnCount(7);
    table->setHorizontalHeaderLabels({
        "Last Name", "First Name", "Middle Name", "Email", "Phones", "Address", "Birth Date"
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
            (c.get_sphones().empty() ? "" : "Work: " + c.phone_serial(c.get_sphones()) + "\n") +
            (c.get_rphones().empty() ? "" : "Mobile: " + c.phone_serial(c.get_rphones()) + "\n") +
            (c.get_dphones().empty() ? "" : "Home: " + c.phone_serial(c.get_dphones()));
        setItem(i, 4, allPhones);

        setItem(i, 5, c.get_Adress());
        setItem(i, 6, c.get_Birth_day());
    }
}

void MainWindow::showContactDialog(Contact_info& contact, bool isNew) {
    QDialog dialog(this);
    dialog.setWindowTitle(isNew ? "New Contact" : "Edit Contact");
    dialog.resize(300, 450);

    auto* layout = new QFormLayout(&dialog);
    auto* leLastName = new QLineEdit(QString::fromStdString(contact.get_Last_Name()));
    auto* leName = new QLineEdit(QString::fromStdString(contact.get_Name()));
    auto* leEmail = new QLineEdit(QString::fromStdString(contact.get_Email()));
    auto* leMiddleName = new QLineEdit(QString::fromStdString(contact.get_Middle_Name()));
    auto* leAddress = new QLineEdit(QString::fromStdString(contact.get_Adress()));
    auto* leBirthDate = new QLineEdit(QString::fromStdString(contact.get_Birth_day()));
    auto* lePhones = new QTextEdit();

    std::string allPhones;
    for (const auto& p : contact.get_sphones()) allPhones += "Work:" + p + "\n";
    for (const auto& p : contact.get_rphones()) allPhones += "Mobile:" + p + "\n";
    for (const auto& p : contact.get_dphones()) allPhones += "Home:" + p + "\n";
    lePhones->setPlainText(QString::fromStdString(allPhones));
    lePhones->setPlaceholderText("Format:\nWork:+7999...\nMobile:+7888...\nHome:+7777...");

    layout->addRow("Last Name*", leLastName);
    layout->addRow("First Name*", leName);
    layout->addRow("Email*", leEmail);
    layout->addRow("Middle Name", leMiddleName);
    layout->addRow("Address", leAddress);
    layout->addRow("Birth Date", leBirthDate);
    layout->addRow("Phones", lePhones);

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

    if (!valid_Name(lastName)) { QMessageBox::warning(this, "Error", "Invalid last name"); return; }
    if (!valid_Name(name)) { QMessageBox::warning(this, "Error", "Invalid first name"); return; }
    if (!valid_Email(email)) { QMessageBox::warning(this, "Error", "Invalid email"); return; }
    if (!middleName.empty() && !valid_Name(middleName)) { QMessageBox::warning(this, "Error", "Invalid middle name"); return; }
    if (!birthDate.empty() && !valid_Bday(birthDate)) { QMessageBox::warning(this, "Error", "Invalid birth date"); return; }

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
        if (trimmed.startsWith("Work:")) {
            std::string phone = trimmed.mid(5).toStdString();
            if (valid_Phone(phone)) contact.set_sPhone(phone);
        } else if (trimmed.startsWith("Mobile:")) {
            std::string phone = trimmed.mid(7).toStdString();
            if (valid_Phone(phone)) contact.set_rPhone(phone);
        } else if (trimmed.startsWith("Home:")) {
            std::string phone = trimmed.mid(5).toStdString();
            if (valid_Phone(phone)) contact.set_dPhone(phone);
        }
    }

    if (isNew) {
        m_phonebook.addContact(contact);
        saveContactToDB(contact);
    } else {
        deleteAllContactsFromDB();
        for (size_t i = 0; i < m_phonebook.contactCount(); ++i) {
            saveContactToDB(m_phonebook.getContact(i));
        }
    }
    updateTable();
}

void MainWindow::onAddClicked() {
    Contact_info contact;
    showContactDialog(contact, true);
}

void MainWindow::onEditClicked() {
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Select a contact to edit");
        return;
    }
    Contact_info& contact = m_phonebook.getContact(row);
    showContactDialog(contact, false);
}

void MainWindow::onDeleteClicked() {
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Select a contact to delete");
        return;
    }
    if (QMessageBox::question(this, "Confirm", "Delete this contact?") == QMessageBox::Yes) {
        m_phonebook.removeContact(row);
        deleteAllContactsFromDB();
        for (size_t i = 0; i < m_phonebook.contactCount(); ++i) {
            saveContactToDB(m_phonebook.getContact(i));
        }
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
