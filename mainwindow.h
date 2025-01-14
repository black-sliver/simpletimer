#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include "UGlobalHotkey/uglobalhotkeys.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void update_timer();

    void on_btnReset_clicked();

    void on_btnSet_clicked();

private:
    Ui::MainWindow *ui;
    QDateTime tStart;
    QDateTime tStop;
    bool running = false;
    bool ended = false;
    qint64 startOffset = 0;
    UGlobalHotkeys* hotkeys;
};
#endif // MAINWINDOW_H
