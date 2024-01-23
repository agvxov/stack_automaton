#ifndef AUTOMATONGUI_H
#define AUTOMATONGUI_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class AutomatonGUI; }
QT_END_NAMESPACE

class AutomatonGUI : public QMainWindow
{
    Q_OBJECT

public:
    AutomatonGUI(QWidget *parent = nullptr);
    ~AutomatonGUI();

    void display_state(const std::string &s);

private slots:
    void on_RunButton_clicked();

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_InputLine_textChanged(const QString &arg1);

    void on_ExtraStates_textChanged(const QString &arg1);

    void matrix_from_table();

private:
    Ui::AutomatonGUI *ui;

    void column_force(const std::string &s);
    void row_force(const std::string &s);
};
#endif // AUTOMATONGUI_H
