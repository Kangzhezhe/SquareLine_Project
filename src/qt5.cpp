#include "qt5.hpp"
#include <cstdio>
#include <QFrame>
#include <qlabel.h>
#include <iostream>
#include <QFile>
using namespace std;

extern int max_num;
// LoginDialog class declaration
class LoginDialog : public QDialog {
    Q_OBJECT

public:
    LoginDialog(QWidget *parent = nullptr);

private slots:
    void attemptLogin();

private:
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QLineEdit *databaseLineEdit;

    bool createConnection(const QString &username, const QString &password, const QString &databaseName);
};

DatabaseViewer *viewer;

// main function
extern int main_mqtt();
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QFile qss("qt5.qss");
    qss.open(QFile::ReadOnly);
    app.setStyleSheet(qss.readAll());
    qss.close();

    LoginDialog loginDialog;
    
    
    if (loginDialog.exec() == QDialog::Accepted) {
        viewer = new DatabaseViewer();
        main_mqtt();
        viewer->setWindowTitle("医疗先锋MySQL服务器后台");
        viewer->resize(800, 600);
        viewer->show();
        return app.exec();
    } else {
        return 0;
    }
}

AddDataDialog::AddDataDialog(QWidget *parent)
    : QDialog(parent) {
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    formLayout = new QFormLayout;
    mainLayout->addLayout(formLayout);

    while (QLayoutItem *item = formLayout->takeAt(0)) {
        delete item->widget();
        delete item;
    }
    viewer->inputs.clear();
    QString tableName = viewer->comboBox->currentText();

    QSqlQuery query(QString("DESCRIBE %1").arg(tableName));
    while (query.next()) {
        QString columnName = query.value(0).toString();
        QLineEdit *lineEdit = new QLineEdit(this);
        formLayout->addRow(new QLabel(columnName), lineEdit);
        viewer->inputs[columnName] = lineEdit;
    }

    // 创建添加数据按钮
    addButton = new QPushButton("添加数据", this);
    mainLayout->addWidget(addButton);

    connect(addButton, &QPushButton::clicked, this, &AddDataDialog::addData);

    setLayout(mainLayout);
    setWindowTitle("添加数据");
}

void AddDataDialog::addData() {
    // 在这里实现添加数据的逻辑，例如通过表单字段的输入生成 SQL 插入语句
    // 然后将数据添加到数据库中

    QString tableName = viewer->comboBox->currentText();
    QSqlQuery query;
    QString queryString = QString("INSERT INTO %1 (").arg(tableName);

    QStringList columns, values;
    for (const QString &column : viewer->inputs.keys()) {
        columns << column;
        values << QString("'%1'").arg(viewer->inputs[column]->text());
    }
    queryString += columns.join(", ") + ") VALUES (" + values.join(", ") + ")";

    if (!query.exec(queryString)) {
        qDebug() << "Insert Error: " << query.lastError().text();
        QMessageBox::critical(this, QObject::tr("Insert Error"), query.lastError().text());
        return;
    }

    viewer->model->select();
}

// DatabaseViewer class definition
DatabaseViewer::DatabaseViewer(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    tableView = new QTableView(this);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    model = new QSqlTableModel(this);
    comboBox = new QComboBox(this);
    QPushButton *switchButton = new QPushButton("切换表", this);

    connect(switchButton, &QPushButton::clicked, this, &DatabaseViewer::switchTable);

    mainLayout->addWidget(comboBox);
    mainLayout->addWidget(switchButton);
    mainLayout->addWidget(tableView);

    formLayout = new QFormLayout;
    mainLayout->addLayout(formLayout);

    // QPushButton *addButton = new QPushButton("添加数据", this);
    // connect(addButton, &QPushButton::clicked, this, &DatabaseViewer::addData);
    // mainLayout->addWidget(addButton);


    QPushButton *openAddDataDialogButton = new QPushButton("添加数据", this);
    connect(openAddDataDialogButton, &QPushButton::clicked, this, &DatabaseViewer::openAddDataDialog);
    mainLayout->addWidget(openAddDataDialogButton);


    QPushButton *deleteButton = new QPushButton("删除选中行", this);
    connect(deleteButton, &QPushButton::clicked, this, &DatabaseViewer::deleteData);

    mainLayout->addWidget(deleteButton);

    // 创建水平分割�?
    QFrame *line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(line);

    QLabel *remoteLabel = new QLabel("远程录入功能:");
    mainLayout->addWidget(remoteLabel);

    QLabel *maxNumLabel = new QLabel("录入数量:");
    maxNumLabel->setFixedWidth(100);


    aEdit =new QLineEdit;
    QIntValidator* aIntValidator = new QIntValidator;
    aIntValidator->setRange(0, 100);
    aEdit->setValidator(aIntValidator);
    aEdit->setText(QString::number(max_num));

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(maxNumLabel);
    hLayout->addWidget(aEdit);
    QPushButton *submitButton = new QPushButton("提交");
    hLayout->addWidget(submitButton);
    mainLayout->addLayout(hLayout);
    connect(submitButton, &QPushButton::clicked, this, &DatabaseViewer::handleSubmitButtonClicked);

    // New progress bar
    progressBar = new QProgressBar(this);
    progressBar->setRange(0, 100);
    progressBar->setValue(0);

    mainLayout->addWidget(progressBar);

    setLayout(mainLayout);

    populateComboBox();

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &DatabaseViewer::refreshTable);
    timer->start(5000);
}


void DatabaseViewer::openAddDataDialog() {
    AddDataDialog dialog(this);
    dialog.exec();
}

void DatabaseViewer::handleSubmitButtonClicked() {
    bool ok;
    int value = aEdit->text().toInt(&ok);
    if (ok) {
        printf("max_num before: %d\n", max_num);
        max_num = value;
        printf("max_num: %d\n", max_num);
    } else {
    }
}

void DatabaseViewer::switchTable() {
    QString tableName = comboBox->currentText();
    model->setTable(tableName);
    if (!model->select()) {
        qDebug() << "Model Select Error: " << model->lastError().text();
        QMessageBox::critical(this, QObject::tr("Model Error"), model->lastError().text());
        return;
    }
    tableView->setModel(model);
    // updateInputFields(tableName);
}

void DatabaseViewer::addData() {
    QString tableName = comboBox->currentText();
    QSqlQuery query;
    QString queryString = QString("INSERT INTO %1 (").arg(tableName);

    QStringList columns, values;
    for (const QString &column : inputs.keys()) {
        columns << column;
        values << QString("'%1'").arg(inputs[column]->text());
    }
    queryString += columns.join(", ") + ") VALUES (" + values.join(", ") + ")";

    if (!query.exec(queryString)) {
        qDebug() << "Insert Error: " << query.lastError().text();
        QMessageBox::critical(this, QObject::tr("Insert Error"), query.lastError().text());
        return;
    }

    model->select();
}

void DatabaseViewer::deleteData() {
    QModelIndexList selectedRows = tableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "Delete Error", "No row selected.");
        return;
    }

    int row = selectedRows.first().row();
    model->removeRow(row);
    if (!model->submitAll()) {
        qDebug() << "Delete Error: " << model->lastError().text();
        QMessageBox::critical(this, QObject::tr("Delete Error"), model->lastError().text());
        return;
    }
    model->select();
}

void DatabaseViewer::refreshTable() {
    // model->select();
}

void DatabaseViewer::populateComboBox() {
    QSqlQuery query("SHOW TABLES");
    while (query.next()) {
        comboBox->addItem(query.value(0).toString());
    }
}

void DatabaseViewer::updateInputFields(const QString &tableName) {
    while (QLayoutItem *item = formLayout->takeAt(0)) {
        delete item->widget();
        delete item;
    }
    inputs.clear();

    QSqlQuery query(QString("DESCRIBE %1").arg(tableName));
    while (query.next()) {
        QString columnName = query.value(0).toString();
        QLineEdit *lineEdit = new QLineEdit(this);
        formLayout->addRow(new QLabel(columnName), lineEdit);
        inputs[columnName] = lineEdit;
    }
}

void DatabaseViewer::updateProgressBar(int value) {
    progressBar->setValue(value);
}

// LoginDialog class definition
LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    usernameLineEdit = new QLineEdit(this);
    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    databaseLineEdit = new QLineEdit(this);

    usernameLineEdit->setText("root");
    databaseLineEdit->setText("testc");
    passwordLineEdit->setText("123456");

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("数据库名:", databaseLineEdit);
    formLayout->addRow("用户名:", usernameLineEdit);
    formLayout->addRow("密码:", passwordLineEdit);

    mainLayout->addLayout(formLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    QPushButton *loginButton = new QPushButton("Login", this);
    QPushButton *cancelButton = new QPushButton("Cancel", this);
    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(buttonLayout);

    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::attemptLogin);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    setLayout(mainLayout);
    setWindowTitle("Login");
    setFixedSize(300, 150);
}

void LoginDialog::attemptLogin() {
    QString username = usernameLineEdit->text();
    QString password = passwordLineEdit->text();
    QString databaseName = databaseLineEdit->text();

    if (createConnection(username, password, databaseName)) {
        accept();
    } else {
        QMessageBox::critical(this, "Login Failed", "Could not connect to the database.");
    }
}

bool LoginDialog::createConnection(const QString &username, const QString &password, const QString &databaseName) {
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName(databaseName);
    db.setUserName(username);
    db.setPassword(password);

    if (!db.open()) {
        qDebug() << "Database Error: " << db.lastError().text();
        return false;
    }
    return true;
}


ReceiveDataThread::ReceiveDataThread(QObject *parent) : QThread(parent) {}

void ReceiveDataThread::run() {
    ReceiveData();
}


PingThread::PingThread(QObject *parent) : QThread(parent) {}

extern void MQTT_Ping();
void PingThread::run() {
    MQTT_Ping();
}

MiniProgramThread::MiniProgramThread(QObject *parent) : QThread(parent) {}

extern int main_miniprogram();
void MiniProgramThread::run() {
    main_miniprogram();
}

void thread_start(){
    ReceiveDataThread *receiveThread = new ReceiveDataThread();
    QObject::connect(receiveThread, &ReceiveDataThread::progressUpdated,viewer, &DatabaseViewer::updateProgressBar);

    PingThread *pingThread = new PingThread();
    MiniProgramThread *miniProgramThread = new MiniProgramThread();

    receiveThread->start();
    pingThread->start();
    miniProgramThread->start();
}

#include "qt5.moc"
