#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QSettings>
#include <QTimer>

enum HOTKEY_ID {
    ID_START=1,
    ID_RESET,
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug("init");

    QSettings settings;
    qlonglong millis = settings.value("timer", 0).toLongLong();
    tStop = QDateTime::currentDateTime();
    tStart = tStop.addMSecs(-1*millis);
    running = false;
    ended = false;

    running = true;
    update_timer();
    running = false;

    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update_timer()));
    //timer->start(50); // would be ideal for 1/10th second resolution
    timer->start(100); // will use less cpu time and is still good enough

    hotkeys = new UGlobalHotkeys(this);
    hotkeys->registerHotkey(settings.value("start", "Meta+F1").toString(), ID_START);
    hotkeys->registerHotkey(settings.value("reset", "Meta+F2").toString(), ID_RESET);
    connect(hotkeys, &UGlobalHotkeys::activated, [this](size_t id) {
        if (id == ID_START)
            on_pushButton_clicked();
        else if (id == ID_RESET)
            on_btnReset_clicked();
    });
}

MainWindow::~MainWindow()
{
    hotkeys->unregisterAllHotkeys();
    QSettings settings;
    if (running) tStop = QDateTime::currentDateTime();
    qint64 millis = tStart.msecsTo(tStop);
    settings.setValue("timer", millis);
    delete ui;
}


void MainWindow::update_timer()
{
    if (!running) return;
    auto now = QDateTime::currentDateTimeUtc();
    qint64 millis = tStart.msecsTo(now); // +50 for rounding
    const char* sign = "";
    if (millis < 0) {
        sign = "-";
        millis *= -1;
    }
    qint64 hrs = millis/1000/60/60;
    millis -= hrs*1000*60*60;
    qint64 mins = millis/1000/60;
    millis -= mins*1000*60;
    qint64 secs = millis/1000;
    millis -= secs*1000;
    qint64 tenths = millis/100;

    if (hrs == 0 && *sign) {
        // special case to display "- mm:ss.f", if possible, which aligns better
        QString fmt = "%1 %2:%3.%4";
        const QChar pad = '0';
        ui->pushButton->setText(fmt.arg(sign).arg(mins,2,10,pad).arg(secs,2,10,pad).arg(tenths,1,10,pad));
    } else {
        // default case, either "-h:mm:ss.f" or "h:mm:ss.f"
        QString fmt = "%1%2:%3:%4.%5";
        const QChar pad = '0';
        ui->pushButton->setText(fmt.arg(sign).arg(hrs,1,10,pad).arg(mins,2,10,pad).arg(secs,2,10,pad).arg(tenths,1,10,pad));
    }
}

void MainWindow::on_pushButton_clicked()
{
    auto now = QDateTime::currentDateTimeUtc();
    if (running) {
        qDebug("stop");
        tStop = now;
        update_timer(); // one last time
        running = false;
    } else {
        if (ended) {
            qDebug("start");
            tStart = now.addMSecs(startOffset);
            ended = false;
        } else {
            qDebug("continue");
            qint64 millis = tStart.msecsTo(tStop);
            tStart = now.addMSecs(-1 * millis);
        }
        running = true;
        update_timer(); // first time
    }
}

void MainWindow::on_btnReset_clicked()
{
    ended = true;
    tStop = QDateTime::currentDateTimeUtc();
    tStart = tStop;
    running = true;
    update_timer(); // clear display
    running = false;
}

void MainWindow::on_btnSet_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Enter time"), "h:mm:ss.f: ", QLineEdit::Normal, "0:00:00.0", &ok);
    if (ok) {
        qDebug("set %s\n", text.toStdString().c_str());
        auto parts = text.split(":");
        if (parts.length() > 3) {
            QMessageBox::critical(this, tr("Error"), tr("Invalid time format!"));
        }
        qint64 millis = 0;
        for (int i=0; i<parts.length(); i++) {
            millis *= 60;
            if (i == parts.length() - 1) {
                // seconds has comma
                millis += (qint64)(parts[i].toDouble(&ok) * 1000);
            } else {
                // others are ints
                millis += (qint64)parts[i].toInt(&ok) * 1000;
            }
            if (!ok) {
                QMessageBox::critical(this, tr("Error"), tr("Invalid time format!"));
                return;
            }
        }
        startOffset = -1 * millis;
        auto now = QDateTime::currentDateTimeUtc();
        tStart = now.addMSecs(startOffset);
        auto oldRunning = running;
        running = true;
        update_timer(); // show immediately
        running = oldRunning;
    }
}
