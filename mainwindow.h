#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QVector>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void selectMatrixSize(int size);
    void calculateDeterminant();
    void clearMatrix();

private:
    void setupUi();
    void setupStyleSheet();
    void buildMatrixGrid(int size);
    int det2(const QVector<int>& v, int a, int b, int c, int d);
    int det3(const QVector<int>& v, int a, int b, int c, int d, int e, int f, int g, int h, int i);
    int det4(const QVector<int>& v);

    QWidget *centralW, *headerWidget, *selectorWidget, *matrixContainer, *resultWidget;
    QVBoxLayout *mainLayout;
    QGridLayout *matrixLayout;
    QLabel *titleLabel, *subtitleLabel, *matrixSizeLabel, *resultLabel, *resultValue;
    QPushButton *btn2x2, *btn3x3, *btn4x4, *clearButton, *calcButton;
    QVector<QLineEdit*> cells;
    int currentSize;
};

#endif
