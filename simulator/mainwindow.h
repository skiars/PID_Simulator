#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onSilderKpChanged(int value);
    void onSilderKiChanged(int value);
    void onSilderKdChanged(int value);
    void onSpinBoxEdited();
    void onMarkerClicked();
    void update();

private:
    void addSeries(const QString &name);
    void connectMarkers();
    void plotPoint(int index, double x, double y);
    void simulater();

private:
    Ui::MainWindow *ui;
    QChart *m_chart;
    QList<QLineSeries *> m_series;
    QList<QVector<QPointF>> m_data;
};

#endif // MAINWINDOW_H
