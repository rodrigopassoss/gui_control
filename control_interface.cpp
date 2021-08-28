#include "control_interface.h"
#include "ui_control_interface.h"

control_interface::control_interface(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::control_interface)
{
    ui->setupUi(this);
    DefplotVR();
    this->curr_flag = 1;

    clock = new QTimer(this);

    data_setPoint.clear();
    data_varControl.clear();
    ref_plot.clear();

    connect(clock,SIGNAL(timeout()),this,SLOT(rotina()));
    clock->start(0);

    //Definições para o plot da analise da resposta
    ui->response_analysis->addGraph();
    ui->response_analysis->addGraph();

    //setPoint
    QPen bluePen;
    bluePen.setColor(QColor(30, 40, 255, 150));
    bluePen.setStyle(Qt::DotLine);
    bluePen.setWidthF(2);
    ui->response_analysis->graph(0)->setPen(bluePen);
    ui->response_analysis->graph(0)->setName("SetPoint");
    //varControl
    QPen redPen;
    redPen.setColor(QColor(255, 10, 20, 150));
    redPen.setStyle(Qt::SolidLine);
    redPen.setWidthF(3);
    ui->response_analysis->graph(1)->setPen(redPen);
    ui->response_analysis->graph(1)->setName("varControl");
    //range
    ui->response_analysis->xAxis->setRange(1, 100);
    ui->response_analysis->yAxis->setRange(-1.6, 1.6);


    ui->plot_virtual->replot();
    ui->response_analysis->replot();


}

control_interface::~control_interface()
{
    delete ui;
}

void control_interface::rotina()
{
    QVector<double> data_robot;
    QVector<double> data_ball;
    QVector<double> data_path;
    QVector<double> data_control;


    //QVector<double> ydata_path;


   // File reading
   QFile file_1("../lambesujoVSSS_2021/robot_data.txt");
    if(file_1.open(QIODevice::ReadOnly))
        {

            QTextStream stream(&file_1);
            while(!file_1.atEnd())
            {
                double buf;
                QStringList list = stream.readAll().split(" ");
                QListIterator<QString> li(list);

                while(li.hasNext())
                {
                    buf=li.next().toDouble();
                    data_robot.append(buf);
                }
            }
        }
    // File reading
    QFile file_2("../lambesujoVSSS_2021/ball_data.txt");
    if(file_2.open(QIODevice::ReadOnly))
        {

            QTextStream stream(&file_2);
            while(!file_2.atEnd())
            {
                double buf;
                QStringList list = stream.readAll().split(" ");
                QListIterator<QString> li(list);

                while(li.hasNext())
                {
                    buf=li.next().toDouble();
                    data_ball.append(buf);
                }
            }
        }
    //File reading
    QFile file_3("../lambesujoVSSS_2021/path_data.txt");
    if(file_3.open(QIODevice::ReadOnly))
        {

            QTextStream stream(&file_3);
            while(!file_3.atEnd())
            {
                double buf;
                QStringList list = stream.readAll().split(" ");
                QListIterator<QString> li(list);

                while(li.hasNext())
                {
                    buf=li.next().toDouble();
                    data_path.append(buf);
                }
            }
        }
    //File reading
    QFile file_4("../lambesujoVSSS_2021/control_data.txt");
    if(file_4.open(QIODevice::ReadOnly))
        {

            QTextStream stream(&file_4);
            while(!file_4.atEnd())
            {
                double buf;
                QStringList list = stream.readAll().split(" ");
                QListIterator<QString> li(list);

                while(li.hasNext())
                {
                    buf=li.next().toDouble();
                    data_control.append(buf);
                }
            }
        }

    if((data_setPoint.size()<100)&&(data_setPoint.size()<100))
        {
            data_setPoint.push_back(data_control.at(0));
            data_varControl.push_back(data_control.at(1));
            ref_plot.push_back(data_setPoint.size());
        }
    else
        {
            data_setPoint.pop_front();
            data_varControl.pop_front();
            data_setPoint.push_back(data_control.at(0));
            data_varControl.push_back(data_control.at(1));
        }

    //instruções para o plot para analise da resposta
    ui->response_analysis->graph(0)->setData(ref_plot,data_setPoint);
    ui->response_analysis->graph(1)->setData(ref_plot,data_varControl);
    ui->set_point->display(data_control.at(0));
    ui->var_control->display(data_control.at(1));


    //Chamadas de função para o plot virtual
    drawBall(data_ball.at(0),data_ball.at(1),8);
    drawRobot(data_robot.at(4),data_robot.at(5),data_robot.at(6)-M_PI/2,0.05,0);
    drawPath(data_path);
    curr_flag=1;

    ui->plot_virtual->replot();
    ui->response_analysis->replot();
    for(int i=0;i<curr_flag;i++)
        ui->plot_virtual->removeGraph(i);
}


void control_interface::DefplotVR()
{

    //https://www.qcustomplot.com/index.php/demos/styleddemo

    /*ui->plot_virtual->addGraph();  //ball
    ui->plot_virtual->graph(0)->setPen(QPen(QColor(255, 100, 0)));
    ui->plot_virtual->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
    ui->plot_virtual->addGraph();  //robot
    ui->plot_virtual->graph(1)->setPen(QPen(QColor(255, 100, 0)));
    ui->plot_virtual->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
    ui->plot_virtual->addGraph(); // Caminho
    ui->plot_virtual->graph(2)->setPen(QPen(Qt::black));*/

    //customPlot->graph(0)->setBrush(QBrush(QPixmap("./balboa.jpg"))); // fill with texture of specified image
    //ui->plot_virtual->graph(0)->setLineStyle(QCPGraph::lsLine);
    //ui->plot_virtual->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 1));
    // ranges of axis
    ui->plot_virtual->xAxis->setRange(-0.75, 0.75);
    ui->plot_virtual->yAxis->setRange(-0.75, 0.75);
    ui->plot_virtual->replot();
}

void control_interface::drawBall(double curr_x, double curr_y, double r)
{
    QVector<double> x(1),y(1);

        x[0]=curr_x;
        y[0]=curr_y;

    ui->plot_virtual->addGraph();  //ball
    ui->plot_virtual->graph(0)->setPen(QPen(QColor(255, 100, 0)));
    ui->plot_virtual->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, r));
    ui->plot_virtual->graph(0)->setData(x, y);

}

void control_interface::drawRobot(double curr_x, double curr_y, double theta, double r, int team)
{
    //team = 0 -> azul
    //team = 1 -> amarelo



    QVector<double> x_point(7);
    QVector<double> y_point(7);
    x_point.clear();
    y_point.clear();
    //Vétices no sistema de coordenadas do robô
    for(int i = 0; i < 5; i++)
    {
        x_point[i] = r*cos((M_PI/4)+i*(M_PI/2));
        y_point[i] = r*sin((M_PI/4)+i*(M_PI/2));
    }
    x_point[5] =  x_point[0];
    y_point[5] = r*sin(M_PI/8);
    x_point[6] =  x_point[1];
    y_point[6] = r*sin(M_PI - M_PI/8);

    //Conversão para sistema inercial
    for(int i = 0; i < 7; i++)
    {
        double x_aux;
        double y_aux;
        x_aux = curr_x + (x_point[i]*cos(theta) - y_point[i]*sin(theta));
        y_aux = curr_y + (x_point[i]*sin(theta) + y_point[i]*cos(theta));
        x_point[i] = x_aux;
        y_point[i] = y_aux;
    }

    //addPlots
    for (int i=0;i<6;i++)
    {
        ui->plot_virtual->addGraph();
        ui->plot_virtual->graph(curr_flag + i)->setPen(QPen(QColor(255*(team), 0, 255)));
        QVector<double> tmpx(2);
        QVector<double> tmpy(2);
        tmpx[0]=x_point.at(i);
        tmpx[1]=x_point.at(i+1);
        tmpy[0]=y_point.at(i);
        tmpy[1]=y_point.at(i+1);
        ui->plot_virtual->graph(curr_flag + i)->setData(tmpx, tmpy);

    }
    curr_flag = curr_flag + 6;
}

void control_interface::drawPath(QVector<double> caminho)
{
    int p = ((caminho.size() - 1)/2)-1;
    cout << p << endl;
    bool flag=true;
    int i=0;
    QPen blackPen;
    blackPen.setColor(QColor(0, 0, 0, 150));
    blackPen.setStyle(Qt::SolidLine);
    blackPen.setWidthF(2);
    for (i=0;i<p-1;i++)
    {
        ui->plot_virtual->addGraph();
        ui->plot_virtual->graph(curr_flag + i)->setPen(blackPen);
        ui->plot_virtual->graph(curr_flag + i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
        QVector<double> tmpx(2);
        QVector<double> tmpy(2);
        tmpx[0]=caminho.at((2*i)+1);
        tmpx[1]=caminho.at((2*(i+1))+1);
        tmpy[0]=caminho.at((2*i)+2);
        tmpy[1]=caminho.at((2*(i+1))+2);
        ui->plot_virtual->graph(curr_flag + i)->setData(tmpx, tmpy);

        cout << "("<< tmpx.at(0) <<", " << tmpx.at(0) <<") ";
        cout << "-> "<< "("<< tmpx.at(1) <<", " << tmpx.at(2) <<")" << endl;
        flag=false;
    }
    if(flag==false)
    {
        //i=i+1;
        ui->plot_virtual->addGraph();
        ui->plot_virtual->graph(curr_flag + i)->setPen(blackPen);
        ui->plot_virtual->graph(curr_flag + i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
        QVector<double> tmpx(2);
        QVector<double> tmpy(2);
        tmpx[0]=caminho.at((2*i)+1);
        tmpx[1]=caminho.at((2*(i+1))+1);
        tmpy[0]=caminho.at((2*i)+2);
        tmpy[1]=caminho.at((2*(i+1))+2);
        ui->plot_virtual->graph(curr_flag + i)->setData(tmpx, tmpy);
    }
   /* else
    {
        ui->plot_virtual->addGraph();
        ui->plot_virtual->graph(curr_flag)->setPen(QPen(QColor(0, 0, 0)));
        ui->plot_virtual->graph(curr_flag)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
        QVector<double> tmpx(2);
        QVector<double> tmpy(2);
        tmpx[0]=caminho.at(1);
        tmpy[0]=caminho.at(2);
        tmpx[1]=caminho.at(3);
        tmpy[1]=caminho.at(4);
        ui->plot_virtual->graph(curr_flag)->setData(tmpx, tmpy);
    }*/


}
