#ifndef ADD_MODEL_H
#define ADD_MODEL_H

#include <QDialog>

namespace Ui {
class add_model;
}

class add_model : public QDialog
{
    Q_OBJECT

public:
    explicit add_model(QWidget *parent = 0);
    ~add_model();


private:
    Ui::add_model *ui;

signals:
    void addPlane_signal(QString,float,float,float,float,float);

    void cancel_signal();

private slots:

    void on_ok_pushButton_clicked();

    void on_cancel_pushButton_clicked();
};

#endif // ADD_MODEL_H
