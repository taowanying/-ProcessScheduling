#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "pcb.h"
#include "table.h"
#include <QStandardItem>
#include <QStandardItemModel>
#include <QModelIndex>
#include <algorithm>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    int Ppriority;
    int Prunningtime;
    int Psize;
    int hourTemp;
    int minuteTemp;
    int secondTemp;
    int countTemp;
    QTimer *msTimer;
    QTimer *counttimer;
    void Display(QString,QString,QString);
    void SetStrLength(QString *str, int length);
    void StartStopwatch();


private slots:
    void TimeSlot();

    void Start();

    void on_add_clicked();

    void on_run_clicked();

    void finish();

    void on_guaqi_clicked(const QModelIndex &index);

    void on_jiuxu_clicked(const QModelIndex &index);


private:
    Ui::Widget *ui;
    vector<pcb> list;
    vector<pcb> exerciselist;
    vector<pcb> readylist;
    vector<pcb> finishlist;
    vector<pcb> suspendlist;
    vector<pcb>::iterator it;
    pcb run;
    vector<sfreetable> freetable;
    vector<stable> table;
    vector<sfreetable>::iterator it1;
    vector<sfreetable>::iterator it2;
    vector<stable>::iterator it3;
    QStandardItemModel  *pcbexercisemodel;
    QStandardItemModel  *pcbreadymodel;
    QStandardItemModel  *pcbrunningmodel;
    QStandardItemModel  *pcbfinishmodel;
    QStandardItemModel  *pcbsuspendmodel;
    QStandardItemModel  *tablemodel;
    QStandardItemModel  *freetablemodel;
};

#endif // WIDGET_H
