#include "widget.h"
#include "ui_widget.h"
#include <algorithm>
#include <QTimer>
#include <pcb.h>
#include <table.h>
#include <QMessageBox>

bool cmp(pcb p1,pcb p2)
{

    return p1.priority>p2.priority;

}

bool cmp1(sfreetable p1,sfreetable p2)
{

    return p1.baddress<p2.baddress;

}

bool cmp2(stable p1,stable p2)
{


    return p1.baddress<p2.baddress;
}

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    countTemp=0;
    secondTemp=0;
    minuteTemp=0;
    hourTemp=0;
    msTimer= new QTimer;//时间
    counttimer=new QTimer;//计时器
    pcbexercisemodel = new QStandardItemModel(this);  //后备
    pcbreadymodel=new QStandardItemModel(this);   //就绪
    pcbrunningmodel=new QStandardItemModel(this);    //运行
    pcbfinishmodel=new QStandardItemModel(this);   //完成
    pcbsuspendmodel=new QStandardItemModel(this);   //挂起
    pcbsuspendmodel=new QStandardItemModel(this);   //挂起
    tablemodel=new QStandardItemModel(this);   //已分配
    freetablemodel=new QStandardItemModel(this);   //未分配

    sfreetable fft;
    fft.baddress=1000;//起始地址
    fft.size=100;//大小
    freetable.push_back(fft);

        QString str4 =QString::number(fft.baddress);
        QString str5 =QString::number(fft.size);
        QString str=str4+"               "+str5;
        QStandardItem *item = new QStandardItem(str);
        freetablemodel->appendRow(item);
        ui->houbei->setModel(pcbexercisemodel);
        ui->jiuxu->setModel(pcbreadymodel);
        ui->yunxing->setModel(pcbrunningmodel);
        ui->wancheng->setModel(pcbfinishmodel);
        ui->guaqi->setModel(pcbsuspendmodel);
        ui->table->setModel(tablemodel);
        ui->freetable->setModel(freetablemodel);

        connect(msTimer,SIGNAL(timeout()),this,SLOT(TimeSlot()));
        connect(ui->run,SIGNAL(clicked()),this,SLOT(Start()));
        //进程运行一次后触发以下两个函数
        connect(counttimer,SIGNAL(timeout()),this,SLOT(finish()));
        connect(counttimer,SIGNAL(timeout()),this,SLOT(on_run_clicked()));
}
void Widget::TimeSlot()
{
    countTemp+=1;
    if(countTemp==100)
    {
        countTemp=0;
        secondTemp+=1;
        if(secondTemp==60)
        {
            secondTemp=0;
            minuteTemp+=1;
            if(minuteTemp==60)
            {
                minuteTemp=0;
                hourTemp+=1;
                if(hourTemp==24)
                {
                    hourTemp=0;
                }
            }
        }
    }
    //把整数转换成字符串
    QString hourstr = QString::number(hourTemp);
    QString minutestr= QString::number(minuteTemp);
    QString secondstr = QString::number(secondTemp);
    //设置字符串的长度为2
    SetStrLength(&hourstr,2);
    SetStrLength(&minutestr,2);
    SetStrLength(&secondstr,2);
    Display(hourstr + ":",minutestr + ":",secondstr);
}

void Widget::Display(QString hour, QString minute, QString second)
{
    ui->runningtimer->setText(hour+minute+second);
}

void Widget::StartStopwatch()
{
    msTimer->start(10); //10ms
}

void Widget::SetStrLength(QString *str, int length)
{
    if(str->length()<length)
    {
        str->insert(0,"0");
    }
}

Widget::~Widget()
{
    delete ui;
}
void Widget::Start()
{
    StartStopwatch();
}


void Widget::on_add_clicked()//添加进程
{
    Ppriority=ui->priority->text().toInt();
    Prunningtime=ui->runningtime->text().toInt();
    Psize=ui->size->text().toInt();
    ui->priority->clear();
    ui->runningtime->clear();
    ui->size->clear();
    int listsize=list.size();
    int pid=listsize++;
    pcb p1;
    p1.pid=listsize++;
    p1.priority=Ppriority;
    p1.runtime=Prunningtime;
    p1.size=Psize;
    list.push_back(p1);
    exerciselist.push_back(p1);

    for(it=exerciselist.begin();it!=exerciselist.end();)
    {
            pcb p=*it;
            int f=0;
            for(it1=freetable.begin();it1!=freetable.end();it1++)
            {
                sfreetable ft=*it1;
                if(p.size<=ft.size)//可分配
                {
                    p.address=ft.baddress;
                    p.flag=1;
                    stable t;
                    t.pid=p.pid;
                    t.baddress=p.address;
                    t.size=p.size;
                    table.push_back(t);
                    readylist.push_back(p);
                    if(ft.size-p.size==0)
                    {
                        freetable.erase(it1);
                    }
                    else if(ft.size-p.size>0)
                    {
                        it1->baddress+=p.size;
                        it1->size-=p.size;
                    }
                    f=1;
                    break;
                }
            }
            if(f==1)
            {
                exerciselist.erase(it);
            }
            else
            {
                it++;
            }
    }
    int exercisesize=exerciselist.size();
    pcbexercisemodel->clear();
    for(int i=0;i<exercisesize;i++)
    {
           pcb p=exerciselist[i];
           QString str1 =QString::number(p.pid);
           QString str2 =QString::number(p.runtime);
           QString str3 =QString::number(p.priority);
           QString str=str1+"                      "+str2+"                   "+str3;
           QStandardItem *item = new QStandardItem(str);
           pcbexercisemodel->appendRow(item);
    }

    sort(readylist.begin(),readylist.end(),cmp);
    int readysize=readylist.size();
    pcbreadymodel->clear();
    for(int i=0;i<readysize;i++)
    {
           pcb p=readylist[i];
           QString str1 =QString::number(p.pid);
           QString str2 =QString::number(p.runtime);
           QString str3 =QString::number(p.priority);
           QString str=str1+"                      "+str2+"                   "+str3;
           QStandardItem *item = new QStandardItem(str);
           pcbreadymodel->appendRow(item);
    }

     sort(freetable.begin(),freetable.end(),cmp1);
     int freetablesize=freetable.size();
     freetablemodel->clear();
     for(int i=0;i<freetablesize;i++)
     {
           sfreetable t=freetable[i];
           QString str4 =QString::number(t.baddress);
           QString str5 =QString::number(t.size);
           QString str=str4+"               "+str5;
           QStandardItem *item = new QStandardItem(str);
           freetablemodel->appendRow(item);
     }

     sort(table.begin(),table.end(),cmp2);
     int tablesize=table.size();
     tablemodel->clear();
     for(int i=0;i<tablesize;i++)
     {
           stable t=table[i];
           QString str3 =QString::number(t.pid);
           QString str4 =QString::number(t.baddress);
           QString str5 =QString::number(t.size);
           QString str="    "+str3+"            "+str4+"               "+str5;
           QStandardItem *item = new QStandardItem(str);
           tablemodel->appendRow(item);
     }
}

void Widget::finish()
{
    if(run.priority>0)//运行一次进程优先级减一，直至减为0
        run.priority-=1;
    run.runtime-=1;
    pcbrunningmodel->clear();
    if(run.runtime<=0)
    {
        if(!finishlist.empty())
        {
            pcb p=finishlist.back();
            if(p.pid!=run.pid)
            {
                for(it1=freetable.begin();it1!=freetable.end();it1++)//it1指向进程所占内存空间的上一邻接空闲分区
                {
                    sfreetable t=*it1;
                    if(run.address==(t.size+t.baddress))
                    {
                        break;
                    }
                }
                for(it2=freetable.begin();it2!=freetable.end();it2++)//it2指向进程所占内存空间的下一邻接空闲分区
                {
                    sfreetable t=*it2;
                    if((run.address+run.size)==t.baddress)
                    {
                        break;
                    }
                }
                            //合并内存的四种情况
                            if(it1!=freetable.end()&&it2!=freetable.end())//若上下均有空闲分区
                            {
                                it1->size+=(it2->size+run.size);
                                freetable.erase(it2);
                                it2--;
                            }
                            if(it1!=freetable.end()&&it2==freetable.end())//若只有上空闲分区
                            {
                                it1->size+=run.size;
                            }
                            if(it1==freetable.end()&&it2!=freetable.end())//若只有下空闲分区
                            {
                                it2->baddress=run.address;
                                it2->size+=run.size;
                            }
                            if(it1==freetable.end()&&it2==freetable.end())//若上下均没有空闲分区
                            {
                                sfreetable t1;
                                t1.baddress=run.address;
                                t1.size=run.size;
                                freetable.push_back(t1);
                            }

                            int i=0;                           
                            for(it3=table.begin();it3!=table.end();it3++)
                            {
                                if(it3->pid==run.pid)
                                    break;
                                else i++;
                            }
                            tablemodel->removeRow(i);
                            table.erase(it3);
                            run.address=-1;
                            run.flag=0;

                            finishlist.push_back(run);
                            QString str1 =QString::number(run.pid);
                            QString str="       "+str1;
                            QStandardItem *item = new QStandardItem(str);
                            pcbfinishmodel->appendRow(item);

                            sort(freetable.begin(),freetable.end(),cmp1);
                            int freetablesize=freetable.size();
                            freetablemodel->clear();
                            for(int i=0;i<freetablesize;i++)
                            {
                                sfreetable t=freetable[i];
                                QString str4 =QString::number(t.baddress);
                                QString str5 =QString::number(t.size);
                                QString str=str4+"               "+str5;
                                QStandardItem *item = new QStandardItem(str);
                                freetablemodel->appendRow(item);
                            }
            }
            pcbrunningmodel->clear();
       }
      else
      {
            for(it1=freetable.begin();it1!=freetable.end();it1++)
            {
                sfreetable t=*it1;
                if(run.address==(t.size+t.baddress))
                {
                    break;
                }
            }
            for(it2=freetable.begin();it2!=freetable.end();it2++)
            {
                sfreetable t=*it2;
                if((run.address+run.size)==t.baddress)
                {
                    break;
                }
            }
                           //合并内存的四种情况
                           if(it1!=freetable.end()&&it2!=freetable.end())
                           {
                               it1->size+=(it2->size+run.size);
                               freetable.erase(it2);
                               it2--;
                           }
                           if(it1!=freetable.end()&&it2==freetable.end())
                           {
                               it1->size+=run.size;
                           }
                           if(it1==freetable.end()&&it2!=freetable.end())
                           {
                               it2->baddress=run.address;
                               it2->size+=run.size;
                           }
                           if(it1==freetable.end()&&it2==freetable.end())
                           {
                               sfreetable t1;
                               t1.baddress=run.address;
                               t1.size=run.size;
                               freetable.push_back(t1);
                           }

                           int i=0;
                           for(it3=table.begin();it3!=table.end();it3++)
                           {
                               if(it3->pid==run.pid)
                                   break;
                               else i++;
                           }
                           tablemodel->removeRow(i);
                           table.erase(it3);
                           run.address=-1;
                           run.flag=0;

                           finishlist.push_back(run);
                           QString str1 =QString::number(run.pid);
                           QString str="       "+str1;
                           QStandardItem *item = new QStandardItem(str);
                           pcbfinishmodel->appendRow(item);

                           sort(freetable.begin(),freetable.end(),cmp1);
                           int freetablesize=freetable.size();
                           freetablemodel->clear();
                           for(int i=0;i<freetablesize;i++)
                           {
                               sfreetable t=freetable[i];
                               QString str4 =QString::number(t.baddress);
                               QString str5 =QString::number(t.size);
                               QString str=str4+"               "+str5;
                               QStandardItem *item = new QStandardItem(str);
                               freetablemodel->appendRow(item);
                           }
                           pcbrunningmodel->clear();
       }
    }
    else//若进程没有运行完毕
    {
        readylist.push_back(run);
        sort(readylist.begin(),readylist.end(),cmp);
        int readysize=readylist.size();
        pcbreadymodel->clear();
        for(int i=0;i<readysize;i++)
        {
            pcb p=readylist[i];
            QString str1 =QString::number(p.pid);
            QString str2 =QString::number(p.runtime);
            QString str3 =QString::number(p.priority);
            QString str=str1+"                      "+str2+"                   "+str3;
            QStandardItem *item = new QStandardItem(str);
            pcbreadymodel->appendRow(item);
        }
    }
}

void Widget::on_run_clicked()
{
    if(!readylist.empty())
    {
        run=readylist[0];
        QString str1 =QString::number(run.pid);
        QString str2 =QString::number(run.runtime);
        QString str3 =QString::number(run.priority);
        QString str=str1+"                      "+str2+"                   "+str3;
        QStandardItem *item = new QStandardItem(str);
        pcbrunningmodel->appendRow(item);
        counttimer->start(1*1000);
        readylist.erase(readylist.begin());

        for(it=exerciselist.begin();it!=exerciselist.end();)
        {
            pcb p=*it;
            int f=0;
            for(it1=freetable.begin();it1!=freetable.end();it1++)
            {
                sfreetable ft=*it1;
                if(p.size<=ft.size)//可分配
                {
                    p.address=ft.baddress;
                    p.flag=1;
                    stable t;
                    //把进程所占内存放入已分配表
                    t.pid=p.pid;
                    t.baddress=p.address;
                    t.size=p.size;
                    table.push_back(t);
                    readylist.push_back(p);
                    if(ft.size-p.size==0)
                    {
                        freetable.erase(it1);
                    }
                    else if(ft.size-p.size>0)
                    {
                        it1->baddress+=p.size;
                        it1->size-=p.size;
                    }
                        f=1;
                        break;
                    }
                 }
                if(f==1)//如果当前分区内存足够放入进程，则后备队列进程减一
                {
                    exerciselist.erase(it);
                }
                else//当前分区内存不够，移至下一空闲分区
                {
                    it++;
                }
            }       
        int exercisesize=exerciselist.size();
        pcbexercisemodel->clear();

        for(int i=0;i<exercisesize;i++)
        {
               pcb p=exerciselist[i];
               QString str1 =QString::number(p.pid);
               QString str2 =QString::number(p.runtime);
               QString str3 =QString::number(p.priority);
               QString str=str1+"                      "+str2+"                   "+str3;
               QStandardItem *item = new QStandardItem(str);
               pcbexercisemodel->appendRow(item);
         }

         sort(readylist.begin(),readylist.end(),cmp);
         int readysize=readylist.size();
         pcbreadymodel->clear();
         for(int i=0;i<readysize;i++)
         {
                pcb p=readylist[i];
                QString str1 =QString::number(p.pid);
                QString str2 =QString::number(p.runtime);
                QString str3 =QString::number(p.priority);
                QString str=str1+"                      "+str2+"                   "+str3;
                QStandardItem *item = new QStandardItem(str);
                pcbreadymodel->appendRow(item);
         }

         sort(freetable.begin(),freetable.end(),cmp1);
         int freetablesize=freetable.size();
         freetablemodel->clear();
         for(int i=0;i<freetablesize;i++)
         {
                sfreetable t=freetable[i];
                QString str4 =QString::number(t.baddress);
                QString str5 =QString::number(t.size);
                QString str=str4+"               "+str5;
                QStandardItem *item = new QStandardItem(str);
                freetablemodel->appendRow(item);
         }

         sort(table.begin(),table.end(),cmp2);
         int tablesize=table.size();
         tablemodel->clear();
         for(int i=0;i<tablesize;i++)
         {
                stable t=table[i];
                QString str3 =QString::number(t.pid);
                QString str4 =QString::number(t.baddress);
                QString str5 =QString::number(t.size);
                QString str="    "+str3+"            "+str4+"               "+str5;
                QStandardItem *item = new QStandardItem(str);
                tablemodel->appendRow(item);
         }
     }
}


void Widget::on_jiuxu_clicked(const QModelIndex &index)//点击进程使之挂起
{
    QString str=index.data().toString();
    int k;
    for(int i=0;i<str.length();i++)
    {
        if(str[i]==' ')
        {
            k=i;
            break;
        }
    }
    int pid=str.mid(0,k).toInt();
    pcb p;
    for(it=readylist.begin();it!=readylist.end();it++)
    {
        pcb p1=*it;
        if(p1.pid==pid)
        {
            p=p1;
            break;
        }
    }
    readylist.erase(it);

    for(it1=freetable.begin();it1!=freetable.end();it1++)
    {
       sfreetable t=*it1;
       if(p.address==(t.size+t.baddress))
       {
           break;
       }
    }
    for(it2=freetable.begin();it2!=freetable.end();it2++)
    {
       sfreetable t=*it2;
       if((p.address+p.size)==t.baddress)
       {
           break;
       }
    }
   //合并内存的四种情况
   if(it1!=freetable.end()&&it2!=freetable.end())
   {
       it1->size+=(it2->size+p.size);
       freetable.erase(it2);
       it2--;
   }
   if(it1!=freetable.end()&&it2==freetable.end())
   {
       it1->size+=p.size;
   }
   if(it1==freetable.end()&&it2!=freetable.end())
   {
       it2->baddress=p.address;
       it2->size+=p.size;
   }
   if(it1==freetable.end()&&it2==freetable.end())
   {
       sfreetable t1;
       t1.baddress=p.address;
       t1.size=p.size;
       freetable.push_back(t1);
   }
   //挂起进程后再从后备队列中选取大小合适的进程放入就绪队列中
   for(it=exerciselist.begin();it!=exerciselist.end();)
   {
       pcb p1=*it;
       int f=0;
       for(it1=freetable.begin();it1!=freetable.end();it1++)
       {
           sfreetable ft=*it1;
           if(p1.size<=ft.size)//可分配
           {
               p1.address=ft.baddress;
               p1.flag=1;
               stable t;
               t.pid=p1.pid;
               t.baddress=p1.address;
               t.size=p1.size;
               table.push_back(t);
               readylist.push_back(p1);
               if(ft.size-p1.size==0)
               {
                   freetable.erase(it1);
               }
               else if(ft.size-p1.size>0)
               {
                   it1->baddress+=p1.size;
                   it1->size-=p1.size;
               }
               f=1;
               break;
               }
           }
           if(f==1)
           {
               exerciselist.erase(it);
           }
           else
           {
               it++;
           }
       }

    int exercisesize=exerciselist.size();
    pcbexercisemodel->clear();
    for(int i=0;i<exercisesize;i++)
    {
        pcb p=exerciselist[i];
        QString str1 =QString::number(p.pid);
        QString str2 =QString::number(p.runtime);
        QString str3 =QString::number(p.priority);
        QString str=str1+"                      "+str2+"                   "+str3;
        QStandardItem *item = new QStandardItem(str);
        pcbexercisemodel->appendRow(item);
    }

    int i=0;
    for(it3=table.begin();it3!=table.end();it3++)
    {
        if(it3->pid==p.pid)
            break;
        else i++;
    }
    tablemodel->removeRow(i);
    table.erase(it3);

    int readysize=readylist.size();
    sort(readylist.begin(),readylist.end(),cmp);
    pcbreadymodel->clear();
    for(int i=0;i<readysize;i++)
    {
        pcb p=readylist[i];
        QString str1 =QString::number(p.pid);
        QString str2 =QString::number(p.runtime);
        QString str3 =QString::number(p.priority);
        QString str4=str1+"                      "+str2+"                   "+str3;
        QStandardItem *item = new QStandardItem(str4);
        pcbreadymodel->appendRow(item);
    }

    sort(freetable.begin(),freetable.end(),cmp1);
    int freetablesize=freetable.size();
    freetablemodel->clear();
    for(int i=0;i<freetablesize;i++)
    {
         sfreetable t=freetable[i];
         QString str4 =QString::number(t.baddress);
         QString str5 =QString::number(t.size);
         QString str=str4+"               "+str5;
         QStandardItem *item = new QStandardItem(str);
         freetablemodel->appendRow(item);
    }
    p.address=-1;
    p.flag=0;
    suspendlist.push_back(p);
    QString str1 =QString::number(p.pid);
    QString str2 =QString::number(p.runtime);
    QString str3 =QString::number(p.priority);
    QString str4=str1+"                      "+str2+"                   "+str3;
    QStandardItem *item = new QStandardItem(str4);
    pcbsuspendmodel->appendRow(item);
}


void Widget::on_guaqi_clicked(const QModelIndex &index)//点击进程使之解挂
{
    QString str=index.data().toString();
    int k;
    for(int i=0;i<str.length();i++)
    {
        if(str[i]==' ')
        {
            k=i;
            break;
        }
    }
    int pid=str.mid(0,k).toInt();
    pcb p;
    int xu=0;
    for(it=suspendlist.begin();it!=suspendlist.end();it++)
    {
        pcb p1=*it;
        if(p1.pid==pid)
        {
               p=p1;
               break;
        }
           xu++;
    }

       int f=0;
       for(it1=freetable.begin();it1!=freetable.end();it1++)
       {
           sfreetable ft=*it1;
           if(p.size<=ft.size)//可分配
           {
               p.address=ft.baddress;
               p.flag=1;
               stable t;
               t.pid=p.pid;
               t.baddress=p.address;
               t.size=p.size;
               table.push_back(t);
               readylist.push_back(p);
               if(ft.size-p.size==0)
               {
                   freetable.erase(it1);
               }
               else if(ft.size-p.size>0)
               {
                   it1->baddress+=p.size;
                   it1->size-=p.size;
               }
               f=1;
               break;
            }
       }
           if(f==0)
           {
               QMessageBox::information(this, "警告","内存不够");
           }
           else
           {
               sort(readylist.begin(),readylist.end(),cmp);
               pcbreadymodel->clear();
               int readysize=readylist.size();
               for(int i=0;i<readysize;i++)
               {
                   pcb p1=readylist[i];
                   QString str1 =QString::number(p1.pid);
                   QString str2 =QString::number(p1.runtime);
                   QString str3 =QString::number(p1.priority);
                   QString str=str1+"                      "+str2+"                   "+str3;
                   QStandardItem *item = new QStandardItem(str);
                   pcbreadymodel->appendRow(item);
               }

       suspendlist.erase(it);
       pcbsuspendmodel->removeRow(xu);

       sort(freetable.begin(),freetable.end(),cmp1);
       int freetablesize=freetable.size();
       freetablemodel->clear();
       for(int i=0;i<freetablesize;i++)
       {
             sfreetable t=freetable[i];
             QString str4 =QString::number(t.baddress);
             QString str5 =QString::number(t.size);
             QString str=str4+"               "+str5;
             QStandardItem *item = new QStandardItem(str);
             freetablemodel->appendRow(item);
       }

       sort(table.begin(),table.end(),cmp2);
       int tablesize=table.size();
       tablemodel->clear();
       for(int i=0;i<tablesize;i++)
       {
             stable t=table[i];
             QString str3 =QString::number(t.pid);
             QString str4 =QString::number(t.baddress);
             QString str5 =QString::number(t.size);
             QString str="    "+str3+"            "+str4+"               "+str5;
             QStandardItem *item = new QStandardItem(str);
             tablemodel->appendRow(item);
       }
           }
}


