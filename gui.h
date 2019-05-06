#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include "QProcess"
#include "QString"
#include "QQueue"

namespace Ui {
class GUI;
}

class GUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit GUI(QWidget *parent = 0);
    ~GUI();
    bool stopPressed=false;
    QString lastDir;
    QProcess *process;
    QQueue<QString> commandList;
    QString logText;


public slots:
    void updateText();

    void launchProcess();

private slots:
    void on_pushButton_Insertar_clicked();

    void on_pushButton_Eliminar_clicked();

    void on_pushButton_Remove_clicked();

    void on_pushButton_Add_clicked();

    void on_pushButton_Run_clicked();

    void on_pushButton_Exit_clicked();

    void on_pushButton_Limpiar_clicked();

    void on_pushButton_Freesurfer_clicked();

    void on_pushButton_outDir_clicked();

    void on_pushButton_shScript_clicked();

private:
    Ui::GUI *ui;
};

#endif // GUI_H
