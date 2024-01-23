#include "automaton.hpp"
#include "automaton_gui.h"
#include "ui_automaton_gui.h"
#include "util.h"

AutomatonGUI::AutomatonGUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AutomatonGUI)
{
    ui->setupUi(this);
    setCentralWidget(ui->horizontalLayoutWidget);
}

AutomatonGUI::~AutomatonGUI()
{
    delete ui;
}

void AutomatonGUI::matrix_from_table() {
    for (int i = 0; i < ui->InputTable->rowCount(); i++) {
        char n = ui->InputTable->verticalHeaderItem(i)->text().toStdString()[0];
        if (n == '#') { n = '\0'; }
        automaton::matrix.insert({ n, std::map<char,Rule>()});
        for (int h = 0; h < ui->InputTable->columnCount(); h++) {
            char m = ui->InputTable->horizontalHeaderItem(h)->text().toStdString()[0];
            if (m == '#') { m = '\0'; }
            auto item = ui->InputTable->item(i, h);
            std::string r = (item ? item->text().toStdString() : "");
            automaton::matrix[n].insert({m, Rule(r.c_str())});
        }
    }
}

void AutomatonGUI::on_RunButton_clicked()
{
    automaton::reset();

    const auto &i = ui->StatusIndicator;
    automaton::input_str = strdup(ui->InputLine->text().toStdString().c_str());

    if (not *automaton::input_str) {
         i->setText("No input");
         i->setProperty("status", "negative");
         i->style()->unpolish(i);
         i->style()->polish(i);
         i->update();
        return;
    }

    matrix_from_table();

    bool r = automaton::run();
    const char * t;
    const char * s;
    if (r) {
        t = "Accepted";
        s = "positive";
    } else {
        t = "Denied";
        s = "negative";
    }
    i->setProperty("status", s);
    i->setText(t);
    i->style()->unpolish(i);
    i->style()->polish(i);
	i->update();
}


void AutomatonGUI::on_InputLine_textChanged(const QString &arg1)
{
    QString r;
    for (const auto &c : arg1) {
        if (ui->lineEdit_2->text().contains(c)) {
            r.append(c);
        }
    }

    ui->InputLine->setText(r);
}


void AutomatonGUI::on_lineEdit_2_textChanged(const QString &arg1)
{
    std::string r = uniq(arg1.toStdString());
    std::string extra = ui->ExtraStates->text().toStdString();

    //ui->lineEdit_2->setText("");
    ui->InputLine->setText("");

    column_force(r);

    row_force(uniq(r + extra));
}

void AutomatonGUI::on_ExtraStates_textChanged(const QString &arg1)
{
    std::string r = arg1.toStdString();
    std::string base = (arg1 + ui->lineEdit_2->text()).toStdString();

    row_force(uniq(r + base));
}

void AutomatonGUI::column_force(const std::string &s) {
    // Delete horizontal headers
    {
        std::stack<int> buf;
        for (int i = 0; i < ui->InputTable->columnCount(); i++) {
         if (s.find(ui->InputTable->horizontalHeaderItem(i)->text().toStdString()[0]) == std::string::npos) {
             buf.push(i);
         }
        }
        while (not buf.empty()) {
         ui->InputTable->removeColumn(buf.top());
         buf.pop();
        }
    }
    // Append horizontal headers
    {
         int offset     = ui->InputTable->columnCount();
         int target_len = s.size();
         ui->InputTable->setColumnCount(target_len);
         for (int i = 0; i < target_len; i++) {
          for (int h = 0; h < offset; h++) {
              auto item = ui->InputTable->horizontalHeaderItem(h);
              if (not item
              || item->text().toStdString()[0] == s[i]) {
               goto long_continue;
              }
          }
          ui->InputTable->setHorizontalHeaderItem(
                offset,
                new QTableWidgetItem(QString(s[i]))
          );
          long_continue:
          ;
         }
    }
}

void AutomatonGUI::row_force(const std::string &s) {
    // Delete vertical headers
    {
        std::stack<int> buf;
        for (int i = 0; i < ui->InputTable->rowCount(); i++) {
          if (s.find(ui->InputTable->verticalHeaderItem(i)->text().toStdString()[0]) == std::string::npos) {
            buf.push(i);
          }
        }
        while (not buf.empty()) {
         ui->InputTable->removeRow(buf.top());
         buf.pop();
        }
    }
    // Append vertical headers
    {
        int offset     = ui->InputTable->rowCount();
        int target_len = s.size();
        ui->InputTable->setRowCount(target_len);
        for (int i = 0; i < target_len; i++) {
          for (int h = 0; h < offset; h++) {
              auto item = ui->InputTable->verticalHeaderItem(h);
              if (not item
              || item->text().toStdString()[0] == s[i]) {
                goto long_continue;
              }
          }
          ui->InputTable->setVerticalHeaderItem(
             offset,
             new QTableWidgetItem(QString(s[i]))
          );
          long_continue:
          ;
        }
    }

}

void AutomatonGUI::display_state(const std::string &s) {
    ui->log->append(QString::fromStdString(s));
}
