#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pid.h"
#include "transfunc.h"
#include <QtCharts/QLegendMarker>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_chart = new QChart;
    ui->chartView->setChart(m_chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);

    addSeries("Set Value");
    addSeries("Control Results");
    addSeries("PID Output");

    connectMarkers();

    m_chart->createDefaultAxes();
    m_chart->setMargins(QMargins(0, 0, 0, 0)); // set margins
    m_chart->setContentsMargins(-9, -9, -8, -8);
    m_chart->setBackgroundRoundness(4);

    connect(ui->spinBoxKp, SIGNAL(valueChanged(double)), this, SLOT(update()));
    connect(ui->spinBoxKi, SIGNAL(valueChanged(double)), this, SLOT(update()));
    connect(ui->spinBoxKd, SIGNAL(valueChanged(double)), this, SLOT(update()));
    connect(ui->spinBoxKp, SIGNAL(editingFinished()), this, SLOT(onSpinBoxEdited()));
    connect(ui->spinBoxKi, SIGNAL(editingFinished()), this, SLOT(onSpinBoxEdited()));
    connect(ui->spinBoxKd, SIGNAL(editingFinished()), this, SLOT(onSpinBoxEdited()));
    connect(ui->silderKp, SIGNAL(valueChanged(int)), this, SLOT(onSilderKpChanged(int)));
    connect(ui->silderKi, SIGNAL(valueChanged(int)), this, SLOT(onSilderKiChanged(int)));
    connect(ui->silderKd, SIGNAL(valueChanged(int)), this, SLOT(onSilderKdChanged(int)));

    update();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_chart;
}

void MainWindow::onSilderKpChanged(int value)
{
    ui->spinBoxKp->setValue(value / 200.0);
}

void MainWindow::onSilderKiChanged(int value)
{
    ui->spinBoxKi->setValue(value / 1000.0);
}

void MainWindow::onSilderKdChanged(int value)
{
    ui->spinBoxKd->setValue(value / 200.0);
}

void MainWindow::onSpinBoxEdited()
{
    ui->silderKp->setValue((int)(ui->spinBoxKp->value() * 200));
    ui->silderKi->setValue((int)(ui->spinBoxKi->value() * 1000));
    ui->silderKd->setValue((int)(ui->spinBoxKd->value() * 200));
}

void MainWindow::onMarkerClicked()
{
    QLegendMarker* marker = qobject_cast<QLegendMarker*> (sender());

    if (marker->type() == QLegendMarker::LegendMarkerTypeXY) {
        marker->series()->setVisible(!marker->series()->isVisible());
        marker->setVisible(true);

        // Dim the marker, if series is not visible
        qreal alpha = marker->series()->isVisible() ? 1.0 : 0.5;

        QColor color;
        QBrush brush = marker->labelBrush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setLabelBrush(brush);

        brush = marker->brush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setBrush(brush);

        QPen pen = marker->pen();
        color = pen.color();
        color.setAlphaF(alpha);
        pen.setColor(color);
        marker->setPen(pen);
    }
}

void MainWindow::addSeries(const QString &name)
{
    QLineSeries *series = new QLineSeries();
    m_series.append(series);

    series->setName(name);
    m_chart->addSeries(series);
    series->setPen(QPen(series->pen().color(), 1.1));

    m_data.append(QVector<QPointF>());
}

void MainWindow::connectMarkers()
{
    // Connect all markers to handler
    const auto markers = m_chart->legend()->markers();
    for (QLegendMarker *marker : markers) {
        QObject::connect(marker, &QLegendMarker::clicked, this, &MainWindow::onMarkerClicked);
    }
}

void MainWindow::plotPoint(int index, double x, double y)
{
    m_data[index].append(QPointF(x, y));
}

void MainWindow::update()
{
    int count = m_series.count();
    for (int i = 0; i < count; ++i) {
        m_data[i].clear();
    }

    // run simulater
    simulater();

    for (int i = 0; i < count; ++i) {
        m_series[i]->replace(m_data[i]);
    }
}

void MainWindow::simulater()
{
    double value = 0.0;
    PID pid(ui->spinBoxKp->value(),
            ui->spinBoxKi->value(),
            ui->spinBoxKd->value());

    TransFunc tf; // init transfer function
    for (double t = 0; t < 1; t += 0.001) {

        double input = tf.drivingFunction(t);
        pid.setValue(input);

        double pidOut = pid.iteration(value);
        value = tf.iteration(pidOut);

        plotPoint(0, t, input);
        plotPoint(1, t, value);
        plotPoint(2, t, pidOut);
    }

    m_chart->axisX()->setRange(0, 1);
    m_chart->axisY()->setRange(-20, 100);
}
