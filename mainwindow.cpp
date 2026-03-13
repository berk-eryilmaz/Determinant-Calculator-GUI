#include "MainWindow.h"
#include <QApplication>
#include <QFont>
#include <QColor>
#include <QPalette>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QIntValidator>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include <QFrame>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUi();
    setupStyleSheet();
    buildMatrixGrid(2);
}

void MainWindow::setupUi() {
    setWindowTitle("Determinant Calculator");
    setMinimumSize(680, 700);
    resize(680, 760);

    centralW = new QWidget(this);
    setCentralWidget(centralW);

    mainLayout = new QVBoxLayout(centralW);
    mainLayout->setContentsMargins(32, 28, 32, 28);
    mainLayout->setSpacing(20);

    // ── Header ──────────────────────────────────────────────────────────
    headerWidget = new QWidget();
    headerWidget->setObjectName("headerWidget");
    QVBoxLayout* headerLayout = new QVBoxLayout(headerWidget);
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->setSpacing(4);

    titleLabel = new QLabel("MATRIX");
    titleLabel->setObjectName("titleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);

    subtitleLabel = new QLabel("DETERMINANT CALCULATOR");
    subtitleLabel->setObjectName("subtitleLabel");
    subtitleLabel->setAlignment(Qt::AlignCenter);

    QFrame* headerLine = new QFrame();
    headerLine->setObjectName("headerLine");
    headerLine->setFrameShape(QFrame::HLine);
    headerLine->setFixedHeight(2);

    headerLayout->addWidget(titleLabel);
    headerLayout->addWidget(subtitleLabel);
    headerLayout->addSpacing(10);
    headerLayout->addWidget(headerLine);

    // ── Size Selector ────────────────────────────────────────────────────
    selectorWidget = new QWidget();
    selectorWidget->setObjectName("selectorWidget");
    QHBoxLayout* selectorLayout = new QHBoxLayout(selectorWidget);
    selectorLayout->setContentsMargins(0, 0, 0, 0);
    selectorLayout->setSpacing(12);

    QLabel* selectorLabel = new QLabel("MATRIX SIZE");
    selectorLabel->setObjectName("sectionLabel");

    btn2x2 = new QPushButton("2 × 2");
    btn3x3 = new QPushButton("3 × 3");
    btn4x4 = new QPushButton("4 × 4");
    btn2x2->setObjectName("sizeBtn");
    btn3x3->setObjectName("sizeBtn");
    btn4x4->setObjectName("sizeBtn");
    btn2x2->setCheckable(true);
    btn3x3->setCheckable(true);
    btn4x4->setCheckable(true);
    btn2x2->setChecked(true);
    btn2x2->setMinimumHeight(40);
    btn3x3->setMinimumHeight(40);
    btn4x4->setMinimumHeight(40);
    btn2x2->setMinimumWidth(90);
    btn3x3->setMinimumWidth(90);
    btn4x4->setMinimumWidth(90);

    selectorLayout->addWidget(selectorLabel);
    selectorLayout->addStretch();
    selectorLayout->addWidget(btn2x2);
    selectorLayout->addWidget(btn3x3);
    selectorLayout->addWidget(btn4x4);

    connect(btn2x2, &QPushButton::clicked, this, [this]{ selectMatrixSize(2); });
    connect(btn3x3, &QPushButton::clicked, this, [this]{ selectMatrixSize(3); });
    connect(btn4x4, &QPushButton::clicked, this, [this]{ selectMatrixSize(4); });

    // ── Matrix Grid Container ─────────────────────────────────────────────
    matrixContainer = new QWidget();
    matrixContainer->setObjectName("matrixContainer");
    matrixContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    matrixLayout = new QGridLayout(matrixContainer);
    matrixLayout->setSpacing(10);
    matrixLayout->setContentsMargins(24, 24, 24, 24);
    matrixLayout->setAlignment(Qt::AlignCenter);

    matrixSizeLabel = new QLabel("[ 2 × 2 ]");
    matrixSizeLabel->setObjectName("matrixSizeLabel");
    matrixSizeLabel->setAlignment(Qt::AlignCenter);

    // ── Buttons Row ───────────────────────────────────────────────────────
    QWidget* btnRow = new QWidget();
    QHBoxLayout* btnLayout = new QHBoxLayout(btnRow);
    btnLayout->setContentsMargins(0, 0, 0, 0);
    btnLayout->setSpacing(14);

    clearButton = new QPushButton("CLEAR");
    clearButton->setObjectName("clearBtn");
    clearButton->setMinimumHeight(48);

    calcButton = new QPushButton("CALCULATE  det(A)");
    calcButton->setObjectName("calcBtn");
    calcButton->setMinimumHeight(48);

    btnLayout->addWidget(clearButton, 1);
    btnLayout->addWidget(calcButton, 3);

    connect(calcButton, &QPushButton::clicked, this, &MainWindow::calculateDeterminant);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearMatrix);

    // ── Result Panel ──────────────────────────────────────────────────────
    resultWidget = new QWidget();
    resultWidget->setObjectName("resultWidget");
    QHBoxLayout* resultLayout = new QHBoxLayout(resultWidget);
    resultLayout->setContentsMargins(20, 16, 20, 16);
    resultLayout->setSpacing(12);

    resultLabel = new QLabel("det ( A )  =");
    resultLabel->setObjectName("resultLabel");

    resultValue = new QLabel("—");
    resultValue->setObjectName("resultValue");

    resultLayout->addWidget(resultLabel);
    resultLayout->addStretch();
    resultLayout->addWidget(resultValue);

    // ── Assemble ──────────────────────────────────────────────────────────
    mainLayout->addWidget(headerWidget);
    mainLayout->addWidget(selectorWidget);
    mainLayout->addWidget(matrixSizeLabel);
    mainLayout->addWidget(matrixContainer, 1);
    mainLayout->addWidget(btnRow);
    mainLayout->addWidget(resultWidget);
}

void MainWindow::setupStyleSheet() {
    QString style = R"(
        QMainWindow, QWidget {
            background-color: #0d0f14;
            color: #e8e8e8;
        }

        #headerWidget {
            background: transparent;
        }

        #titleLabel {
            font-family: "Courier New", monospace;
            font-size: 42px;
            font-weight: bold;
            letter-spacing: 18px;
            color: #00e5ff;
            text-shadow: 0 0 24px rgba(0,229,255,0.4);
        }

        #subtitleLabel {
            font-family: "Courier New", monospace;
            font-size: 11px;
            letter-spacing: 10px;
            color: #4a6070;
        }

        #headerLine {
            background-color: #1e3040;
            border: none;
        }

        #sectionLabel {
            font-family: "Courier New", monospace;
            font-size: 11px;
            letter-spacing: 5px;
            color: #4a6070;
        }

        #sizeBtn {
            font-family: "Courier New", monospace;
            font-size: 13px;
            letter-spacing: 2px;
            color: #7a9ab0;
            background-color: #111620;
            border: 1px solid #1e3040;
            border-radius: 6px;
            padding: 6px 14px;
        }

        #sizeBtn:hover {
            background-color: #162030;
            border-color: #00b8d4;
            color: #00e5ff;
        }

        #sizeBtn:checked {
            background-color: #00294d;
            border: 1.5px solid #00e5ff;
            color: #00e5ff;
        }

        #matrixContainer {
            background-color: #111620;
            border: 1px solid #1e3040;
            border-radius: 12px;
        }

        #matrixSizeLabel {
            font-family: "Courier New", monospace;
            font-size: 12px;
            letter-spacing: 4px;
            color: #2a4a60;
        }

        QLineEdit {
            font-family: "Courier New", monospace;
            font-size: 22px;
            font-weight: bold;
            color: #00e5ff;
            background-color: #0d1520;
            border: 1.5px solid #1e3040;
            border-radius: 8px;
            padding: 8px;
            text-align: center;
            selection-background-color: #00294d;
        }

        QLineEdit:focus {
            border: 1.5px solid #00e5ff;
            background-color: #0a1a2a;
        }

        QLineEdit:hover {
            border-color: #00b8d4;
        }

        #calcBtn {
            font-family: "Courier New", monospace;
            font-size: 13px;
            font-weight: bold;
            letter-spacing: 3px;
            color: #0d0f14;
            background-color: #00e5ff;
            border: none;
            border-radius: 8px;
            padding: 10px 24px;
        }

        #calcBtn:hover {
            background-color: #33eeff;
        }

        #calcBtn:pressed {
            background-color: #00b8cc;
        }

        #clearBtn {
            font-family: "Courier New", monospace;
            font-size: 12px;
            letter-spacing: 3px;
            color: #4a6070;
            background-color: #111620;
            border: 1px solid #1e3040;
            border-radius: 8px;
            padding: 10px 14px;
        }

        #clearBtn:hover {
            color: #ff4466;
            border-color: #ff4466;
            background-color: #1a0d14;
        }

        #clearBtn:pressed {
            background-color: #2a0d14;
        }

        #resultWidget {
            background-color: #111620;
            border: 1px solid #1e3040;
            border-radius: 10px;
        }

        #resultLabel {
            font-family: "Courier New", monospace;
            font-size: 15px;
            letter-spacing: 2px;
            color: #4a6070;
        }

        #resultValue {
            font-family: "Courier New", monospace;
            font-size: 32px;
            font-weight: bold;
            color: #00e5ff;
            letter-spacing: 2px;
        }
    )";
    qApp->setStyleSheet(style);
}

void MainWindow::buildMatrixGrid(int size) {
    // Clear existing cells
    for (auto* cell : cells) { delete cell; }
    cells.clear();
    QLayoutItem* item;
    while ((item = matrixLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    currentSize = size;
    int cellSize = (size == 2) ? 110 : (size == 3) ? 88 : 72;
    QValidator* validator = new QIntValidator(-9999, 9999, this);

    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            QLineEdit* cell = new QLineEdit("0");
            cell->setAlignment(Qt::AlignCenter);
            cell->setValidator(validator);
            cell->setFixedSize(cellSize, cellSize);
            matrixLayout->addWidget(cell, r, c);
            cells.append(cell);
        }
    }

    matrixSizeLabel->setText(QString("[ %1 × %1 ]").arg(size));
    resultValue->setText("—");
}

void MainWindow::selectMatrixSize(int size) {
    btn2x2->setChecked(size == 2);
    btn3x3->setChecked(size == 3);
    btn4x4->setChecked(size == 4);
    buildMatrixGrid(size);
}

void MainWindow::calculateDeterminant() {
    int n = currentSize * currentSize;
    QVector<int> v(n);
    for (int i = 0; i < n; i++) {
        bool ok;
        v[i] = cells[i]->text().toInt(&ok);
        if (!ok) v[i] = 0;
    }

    int result = 0;
    if (currentSize == 2) result = det2(v, 0,1,2,3);
    else if (currentSize == 3) result = det3(v, 0,1,2,3,4,5,6,7,8);
    else result = det4(v);

    resultValue->setText(QString::number(result));
}

void MainWindow::clearMatrix() {
    for (auto* cell : cells) { cell->setText("0"); }
    resultValue->setText("—");
}

int MainWindow::det2(const QVector<int>& v, int a, int b, int c, int d) {
    return (v[a] * v[d]) - (v[b] * v[c]);
}

int MainWindow::det3(const QVector<int>& v, int a, int b, int c, int d, int e, int f, int g, int h, int i) {
    return v[a] * det2(v, e, f, h, i)
    - v[b] * det2(v, d, f, g, i)
        + v[c] * det2(v, d, e, g, h);
}

int MainWindow::det4(const QVector<int>& v) {
    return v[0] * det3(v, 5,6,7,9,10,11,13,14,15)
    - v[1] * det3(v, 4,6,7,8,10,11,12,14,15)
        + v[2] * det3(v, 4,5,7,8,9,11,12,13,15)
        - v[3] * det3(v, 4,5,6,8,9,10,12,13,14);
}
