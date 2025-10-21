#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class LoginWindow; }
QT_END_NAMESPACE

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

    QString getUsername() const;
    QString getPassword() const;

private slots:
    void onLoginClicked();
    void onCancelClicked();
    void onCreateAccountClicked();

private:
    Ui::LoginWindow *ui;
    void setupConnections();
    void setupStyling();
};

#endif // LOGINWINDOW_H
