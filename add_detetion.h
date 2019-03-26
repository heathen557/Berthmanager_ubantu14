#ifndef ADD_DETETION_H
#define ADD_DETETION_H

#include <QDialog>

namespace Ui {
class add_detetion;
}

class add_detetion : public QDialog
{
    Q_OBJECT

public:
    explicit add_detetion(QWidget *parent = 0);
    ~add_detetion();

private slots:
    void on_ok_pushButton_clicked();

    void on_cancel_pushButton_clicked();

    void updateModel_slot(QStringList);

private:
    Ui::add_detetion *ui;

    QStringList allModel_list;

signals:
    void addDetectionPlane_signal(QString);
    void cancel_signal();
};

#endif // ADD_DETETION_H
