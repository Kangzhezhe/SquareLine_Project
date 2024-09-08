#ifndef QT5_HPP
#define QT5_HPP

#include <QThread>
#include <QObject>
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QTableView>
#include <QMessageBox>
#include <QDebug>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QFormLayout>
#include <QMap>
#include <QLabel>
#include <QHeaderView>
#include <QTimer>
#include <QDialog>
#include <QHBoxLayout>
#include <QProgressBar>
#include <qobjectdefs.h>


// AddDataDialog ÀàÉùÃ÷
class AddDataDialog : public QDialog {
    Q_OBJECT

public:
    AddDataDialog(QWidget *parent = nullptr);

private slots:
    void addData();

private:
    QPushButton *addButton;
    QFormLayout *formLayout;
};

// DatabaseViewer class declaration
class DatabaseViewer : public QWidget {
    Q_OBJECT

public:
    DatabaseViewer(QWidget *parent = nullptr);
    friend class AddDataDialog;

public slots:
    void updateProgressBar(int value);  // New slot for updating progress bar
private slots:
    void switchTable();
    void addData();
    void deleteData();
    void refreshTable();
    void handleSubmitButtonClicked();
   
    
private:
    void populateComboBox();
    void updateInputFields(const QString &tableName);
    void openAddDataDialog();

    QTableView *tableView;
    QSqlTableModel *model;
    QComboBox *comboBox;
    QFormLayout *formLayout;
    QMap<QString, QLineEdit *> inputs;

    QProgressBar *progressBar;  // New progress bar
    QLineEdit* aEdit;
};


class ReceiveDataThread : public QThread {
    Q_OBJECT

public:
    explicit ReceiveDataThread(QObject *parent = nullptr);
    void run() override;
    void ReceiveData(void);

signals:
    void progressUpdated(int value);
};

class PingThread : public QThread {
    Q_OBJECT

public:
    explicit PingThread(QObject *parent = nullptr);
    void run() override;
};

class MiniProgramThread : public QThread {
    Q_OBJECT

public:
    explicit MiniProgramThread(QObject *parent = nullptr);
    void run() override;
};

#endif