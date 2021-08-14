#ifndef CONTROL_INTERFACE_H
#define CONTROL_INTERFACE_H

#include <QWidget>
#include <QTimer>
#include <QFile>
#include <stdio.h>
#include <iostream>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class control_interface; }
QT_END_NAMESPACE

class control_interface : public QWidget
{
    Q_OBJECT

public:
    control_interface(QWidget *parent = nullptr);
    ~control_interface();

    QTimer *clock;

    QVector<double> data_setPoint;
    QVector<double> data_varControl;
    QVector<double> ref_plot;





private slots:

    void rotina();

private:
    Ui::control_interface *ui;

    //Plot Definitions
    void DefplotVR();

    //Desenhos do plot
    int curr_flag; //bandeira com o numero de plots adicionados
    void drawBall(double curr_x, double curr_y, double r);
    void drawRobot(double curr_x, double curr_y, double theta, double r, int team);
    void drawPath(QVector<double> caminho);



    //Parâmetros de interesse para a "response_analysis
    QVector<double> *x_data;
    QVector<double> *y_data;



};
#endif // CONTROL_INTERFACE_H
