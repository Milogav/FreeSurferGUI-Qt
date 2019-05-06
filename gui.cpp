#include "gui.h"
#include "ui_gui.h"
#include "QFileDialog"
#include "QProcess"
#include "QQueue"

GUI::GUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GUI)
{
    ui->setupUi(this);
    int w1=ui->scrollArea->width();
    ui->tableWidget->setColumnWidth(0,w1);
    ui->tableWidget->setColumnWidth(1,w1*2);
    ui->tableWidget->setColumnWidth(2,w1*0.75);
    this->lastDir="/home"; //default initial input directory: /home/{user}
}

GUI::~GUI()
{
    this->process->close();
    delete ui;
}

void GUI::on_pushButton_Add_clicked()
{
    int row=ui->tableWidget->currentRow();
    int col=ui->tableWidget->currentColumn();
    int crow=ui->tableWidget->rowCount();
    if (crow==0){
        ui->tableWidget->insertRow(crow);
        row=0;
        col=1;
    }
    QString dir = QFileDialog::getOpenFileName(this, tr("Seleccionar archivo..."),this->lastDir);
    int pos=dir.lastIndexOf('/');
    this->lastDir=dir.left(pos);
    ui->tableWidget->setItem(row,col,new QTableWidgetItem(dir));
}

void GUI::on_pushButton_Remove_clicked()
{
    QModelIndexList idx = ui->tableWidget->selectionModel()->selectedIndexes();
    for(auto i = idx.begin();i!=idx.constEnd();i++){
        ui->tableWidget->setItem(i->row(),i->column(),NULL);
    }
}

void GUI::on_pushButton_Insertar_clicked()
{
    int row=ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);
}

void GUI::on_pushButton_Eliminar_clicked()
{
    int row=ui->tableWidget->currentRow();
    ui->tableWidget->removeRow(row);
}

void GUI::updateText(){
    ui->textlog->insertHtml(this->process->readAll());
    ui->textlog->insertHtml("<br>");/////////NEWLINE IN HTML
    ui->textlog->moveCursor(QTextCursor::End);
}

void GUI::launchProcess() {
   int val=ui->progressBar->value()+1;
   this->commandList.dequeue();        
   if(!this->commandList.isEmpty() && !this->stopPressed){
        ui->textlog->append("~~~~~~~~~~~~~~~PROCESO TERMINADO~~~~~~~~~~~~~~~\n");
        ui->textlog->append("~~~~~~~~~~~~~~~INICIANDO SIGUIENTE PROCESO~~~~~~~~~~~~~~~\n");
        ui->progressBar->setValue(val);
        this->process->start(this->commandList.head());
   }
   else{
       ui->progressBar->setFormat("Terminado");
       ui->pushButton_Exit->setEnabled(false);
       ui->pushButton_Run->setEnabled(true);
       ui->textlog->append("~~~~~~~~~~~~~~~TODOS LOS PROCESOS TERMINADOS~~~~~~~~~~~~~~~\n");
   }
}

void GUI::on_pushButton_Run_clicked()
{
    this->process=new QProcess();
    this->stopPressed=false;
    this->logText.clear();
    ui->textlog->setText("");
    ui->progressBar->setValue(0);
    ui->progressBar->setFormat("");
    this->commandList.clear();
    int val;
    QString command,t1Dir,t2Dir,sujName,newname;
    QString outDir=ui->lineEdit_outDir->text();
    QString freesurferDir=ui->lineEdit_Freesurfer->text();
    if(freesurferDir.isEmpty()){
         ui->textlog->setText("ERROR: ¡directorio a FreeSurfer no especificado!");
    }
    else if(outDir.isEmpty()){
         ui->textlog->setText("ERROR: ¡directorio de salida no especificado!");
    }
    else{
        bool ok=false;
        connect(this->process, SIGNAL(readyRead()), this, SLOT(updateText()));
        connect(this->process, SIGNAL(finished(int, QProcess::ExitStatus)),this, SLOT(launchProcess()));
        int row=ui->tableWidget->rowCount();
        ui->progressBar->setMaximum(row);
        for(int i=0;i<row;i++){
            if(ui->tableWidget->item(i,1) && !ui->tableWidget->item(i,1)->text().isEmpty() ){
                ok=true;
                newname="SUJ";
                newname=newname.append(QString::number(i));
                t1Dir=ui->tableWidget->item(i,1)->text();

                if(!ui->tableWidget->item(i,0)){
                    ui->tableWidget->setItem(i,0,new QTableWidgetItem(newname));
                }
                else if(ui->tableWidget->item(i,0)->text().isEmpty()){
                    ui->tableWidget->item(i,0)->setText(newname);
                }
                sujName=ui->tableWidget->item(i,0)->text();

                if(!ui->tableWidget->item(i,2)){
                    ui->tableWidget->setItem(i,2,new QTableWidgetItem("NO_T2_INPUT"));
                }
                else if(ui->tableWidget->item(i,2)->text().isEmpty()){
                    ui->tableWidget->item(i,2)->setText("NO_T2_INPUT");
                }
                t2Dir=ui->tableWidget->item(i,2)->text();

                command=ui->lineEdit_shScript->text();
                command.append(" \"");
                command.append(t1Dir);
                command.append("\" \"");
                command.append(t2Dir);
                command.append("\" \"");
                command.append(freesurferDir);
                command.append("\" \"");
                command.append(outDir);
                command.append("\" \"");
                command.append(sujName);
                command.append("\"");
                commandList.enqueue(command);
            }
          else{
              val=ui->progressBar->value()+1;
              ui->progressBar->setValue(val);
              if(val==ui->progressBar->maximum()){
                  ui->progressBar->setFormat("Terminado");
              }
          }
        }
        if(ok){
            ui->textlog->append("~~~~~~~~~~~~~~~INICIANDO NUEVO PROCESO~~~~~~~~~~~~~~~\n");
            ui->pushButton_Exit->setEnabled(true);
            ui->pushButton_Run->setEnabled(false);
            val=ui->progressBar->value()+1;
            ui->progressBar->setValue(val);
            ui->progressBar->setFormat("Procesando: %v/%m");
            this->process->start(commandList.head());
        }
        else{
             ui->textlog->setText("ERROR: ¡ningun archivo T1 ha sido introducido!");
             ui->progressBar->setValue(0);
             ui->progressBar->setFormat("");
        }
    }
}

void GUI::on_pushButton_Exit_clicked()
{
    this->process->kill();
    this->process->deleteLater();
    ui->textlog->append("~~~~~~~~~~~~~~~PROCESO ACTUAL DETENIDO POR EL USUARIO~~~~~~~~~~~~~~~\n");
    ui->textlog->moveCursor(QTextCursor::End);
    ui->progressBar->setValue(ui->progressBar->maximum());
    ui->progressBar->setFormat("Detenido");
    this->stopPressed=true;
    this->process=new QProcess();
    ui->pushButton_Exit->setEnabled(false);
    ui->pushButton_Run->setEnabled(true);
}

void GUI::on_pushButton_Limpiar_clicked()
{
    ui->tableWidget->clearContents();
}

void GUI::on_pushButton_Freesurfer_clicked()
{
    QString freesurferDir = QFileDialog::getExistingDirectory(this, tr("Directorio de Freesurfer..."),this->lastDir,QFileDialog::ShowDirsOnly);
    ui->lineEdit_Freesurfer->setText(freesurferDir);
}

void GUI::on_pushButton_outDir_clicked()
{
    QString outDir = QFileDialog::getExistingDirectory(this, tr("Guardar resultados en..."),this->lastDir,QFileDialog::ShowDirsOnly);
    ui->lineEdit_outDir->setText(outDir);
}

void GUI::on_pushButton_shScript_clicked()
{
    QString scriptPath = QFileDialog::getOpenFileName(this, tr("Seleccionar script ../batchFreesurfer.sh"),"/home");
    ui->lineEdit_shScript->setText(scriptPath);
}
