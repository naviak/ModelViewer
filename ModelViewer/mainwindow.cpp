#include <QDir>
#include <QAction>
#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
    delete view;
}
void MainWindow::on_pushButton_clicked()
{
    QString directory = (lastFilePath.length() > 0) ? lastFilePath : QDir::homePath();
    QString filename = QFileDialog::getOpenFileName(this, "Выбери модельку", directory, "Модельки (*.dae *.obj)");
    if (!filename.isNull()) {
        lastFilePath = filename;
        ui->widget->loadModel(filename);
    }
}
