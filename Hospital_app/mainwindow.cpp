#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login.h"
#include <windows.h>
#include <stdio.h>
#include <QPdfWriter>
#include <QPainter>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QWidget>
#include <QDateTime>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    hospitalDb = QSqlDatabase:: addDatabase("QSQLITE");
    QString ubicacion= ("Hospital.s3db");
    hospitalDb.setDatabaseName(ubicacion);
    hospitalDb.open();

    if(!hospitalDb.open()){
        qDebug()<<"No se abrio esta wea";
    }else{
        qDebug()<<"Si se abrió la base de Datos";
    }

    ui->SW->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_cancelar_PB_clicked()
{
    this->close();
}

void MainWindow::on_aceptar_PB_clicked()
{
    QMessageBox message;
    QString usuario, contrasena;
    usuario= ui->usuario_LE->text();
    contrasena=ui->contrasena_LE->text();

//    if(!hospitalDb.isOpen()){
//        qDebug()<<"Failed to open database";
//        return;
//    }

    QSqlQuery qry;
    if(qry.exec("select * from Usuario where Nombre_usuario = '"+usuario + "' and Contrasena= '"+contrasena+"'")){
        int count=0;
        while(qry.next()&&count==0){
            count++;
            rol=(qry.value(3).toString());

            ID_user=(qry.value(4).toString());
        }if(count==1){
            message.setText("Bienvenido :)");
            //ui->SW->setCurrentIndex(1);
            message.exec();
            if(rol == "admin"){
                   ui->SW->setCurrentIndex(2);
             }
            else if(rol == "medico"){
                QString consultar;
                consultar.append("select Nombre,Apellido_P, Apellido_M from Medico where Nss = '"+ID_user+"'");
                qry.prepare(consultar);qry.exec();

                qry.next();
                consultar = qry.value(0).toByteArray().constData();
                consultar+=" ";
                consultar.append(qry.value(1).toByteArray().constData());
                consultar+=" ";
                consultar.append(qry.value(2).toByteArray().constData());

                   ui->SW->setCurrentIndex(6);
                   ui->Bienvenida_med_LB->setText("Bienvenido(a) Dr(a) "+consultar+"!");
             }
            else if(rol == "paciente"){
                setPaciente();
                ui->SW->setCurrentIndex(7);
                ui->bienvenida_pac_LB->setText("Bienvenid@ "+paciente.getNombre()+" "+paciente.getApellidoP()+" "+paciente.getApellidoM()+"!");

             }


        }else{
            message.setText("Los datos son incorrectos");
            message.setIcon(QMessageBox::Warning);
            message.exec();
        }

        ui->usuario_LE->clear();
        ui->contrasena_LE->clear();
    }
}

void MainWindow::on_backRD_clicked()
{
    ui->SW->setCurrentIndex(2);
}

void MainWindow::on_AddDoc_clicked()
{
    ui->SW->setCurrentIndex(3);
}

void MainWindow::on_logOutAdmin_clicked()
{
    QMessageBox message;
    message.setText("Sesión cerrada, Vuelva pronto :)");
    message.exec();
    ui->SW->setCurrentIndex(0);
}

void MainWindow::on_backRP_clicked()
{
    ui->SW->setCurrentIndex(2);
}

void MainWindow::on_AddPatient_clicked()
{
    ui->SW->setCurrentIndex(4);
}

void MainWindow::on_saveD_clicked()
{
     QString rol = "medico";
      InsertMed();
      createUser(ui->namesDLE->text(),ui->lastnameDLE1->text(),ui->lastnameDLE2->text(),rol,ui->nssDLE->text());
      DeleteLEMed();
}

void MainWindow::InsertMed()
{   QSqlQuery qry;
    QString name = ui->namesDLE->text(),lastname1 = ui->lastnameDLE1->text(),
            lastname2 = ui->lastnameDLE2->text(), sex = ui->sexDLE->text(), dateOfbirth = ui->dobDLE->text(),
            curp = ui->curpDLE->text(), nss = ui->nssDLE->text(), address = ui->addressDLE->text(),
            Spec = ui->comboSD->currentText(), dateHired = ui->hiredDLE->text(),sentencia;
    sentencia.append("INSERT INTO Medico VALUES('"+name+"','"+lastname1+"','"+lastname2+"','"
                     +sex+"','"+dateOfbirth+"','"+curp+"','"+nss+"','"+address+"','"+Spec+"','"
                     +dateHired+"')");
    qry.prepare(sentencia);
    if(qry.exec()){
                   qDebug()<<"Se ha insertado corectamente.";
        }else{
                qDebug()<<"No se ha insertado correctamente.";
                qDebug()<<"ERROR! "<<qry.lastError();
            }
}

void MainWindow::InsertPat()
{
    QSqlQuery qry;


        QString  name = ui->namesPLE->text(), lastname1 = ui->lastnamePLE1->text(),
                lastname2 = ui->lastnamePLE2->text(), curp = ui->curpPLE->text(), nss = ui->nssPLE->text(),
                telephoneN = ui->telPLE->text(), address = ui->addressPLE->text(),sentencia,
                dateNac =ui->fecha_->text();
        sentencia.append("INSERT INTO Paciente VALUES('"+name+"','"+lastname1+"','"+lastname2+"','"+curp+"','"+nss+"',"+telephoneN+",'"+address+"','"+dateNac+"')");
        qry.prepare(sentencia);
        if(qry.exec()){
                       qDebug()<<"Se ha insertado corectamente.";
            }else{
                    qDebug()<<"No se ha insertado correctamente.";
                    qDebug()<<"ERROR! "<<qry.lastError();
                }

}

void MainWindow::InsertApp()
{
    QSqlQuery qry,consul;
    QMessageBox message;
    QString consultar;
       consultar.append("select Id_cita from Cita order by Id_cita desc limit 1");
       consul.prepare(consultar);

       if(consul.exec()){
                  qDebug()<<"Se ha consultado correctamente.";
       }else{
               qDebug()<<"No se ha consultado correctamente.";
               qDebug()<<"ERROR! "<<qry.lastError();
           }
        consul.next();
      QString idmax = consul.value(0).toByteArray().constData();
      int aux = idmax.toInt() + 1;
    QString hora=ui->horaApp->currentText().replace(":","-")+"-00";
    QString sentencia;
    idmax = QString::number(aux);
    sentencia.append("insert into Cita values("+idmax+","+idpat+","+idmed+",'"+fecha+"','"+hora+"')");
    qry.prepare(sentencia);
    if(qry.exec()){
                   qDebug()<<"Se ha insertado corectamente.";
                   message.setText("Cita agendada el: "+fecha+" a las "+hora);
                   message.exec();
        }else{
                qDebug()<<"No se ha insertado correctamente.";
                qDebug()<<"ERROR! "<<qry.lastError();
            }

}
void MainWindow::DeleteLEPat()
{
    ui->namesPLE->clear();
    ui->lastnamePLE1->clear();
    ui->lastnamePLE2->clear();
    ui->curpPLE->clear();
    ui->nssPLE->clear();
    ui->telPLE->clear();
    ui->addressPLE->clear();
    ui->fecha_->clear();
}

void MainWindow::DeleteLEMed()
{
    ui->namesDLE->setText("");
    ui->lastnameDLE1->setText("");
    ui->lastnameDLE2->setText("");
    ui->sexDLE->setText("");
    ui->dobDLE->setText("");
    ui->curpDLE->setText("");
    ui->nssDLE->setText("");
    ui->comboSD->setCurrentIndex(0);
    ui->addressDLE->setText("");
    ui->hiredDLE->setText("");
}

void MainWindow::DeleteLEApp()
{
    ui->SearchNss->setText("");
    ui->NamePLabel->setText("");
    ui->LastP1L->setText("");
    ui->LastP2L->setText("");
    reestablecerCB2();
    ui->comboSD_2->setCurrentIndex(0);
}

void MainWindow::DeleteLEApp2()
{
    reestablecerCB();

    ///ui->horacita_2->clear();
}

void MainWindow::consultarDocs(QString especialidad)
{
    ui->CalendarApp->setDisabled(true);
    ui->tablaDoctores->clear();
    QSqlQuery qry;
    QString consulta;
    consulta.append("select Nombre,Apellido_P from Medico where Especialidad = '"+especialidad+"'");
    qry.prepare(consulta);

    if(qry.exec()){
           qDebug()<<"Se ha consultado correctamente.";
}else{
        qDebug()<<"No se ha consultado correctamente.";
        qDebug()<<"ERROR! "<<qry.lastError();
    }

    int fila = 0;
    ui->tablaDoctores->setRowCount(0);
    while(qry.next()){
        ui->tablaDoctores->insertRow(fila);
        ui->tablaDoctores->setItem(fila,0,new QTableWidgetItem(qry.value(0).toByteArray().constData()));
        ui->tablaDoctores->setItem(fila,1,new QTableWidgetItem(qry.value(1).toByteArray().constData()));
        fila++;
    }
}

void MainWindow::consultarDocs2(QString especialidad)
{
    // ui->calendarWidget->acceptDrops();
    ui->calendarWidget->setDisabled(true);
    ui->tablaDoctores_2->clear();

    QSqlQuery qry;
    QString consulta;
    consulta.append("select Nombre,Apellido_P from Medico where Especialidad = '"+especialidad+"'");
    qry.prepare(consulta);

    if(qry.exec()){
           qDebug()<<"Se ha consultado correctamente.";
}else{
        qDebug()<<"No se ha consultado correctamente.";
        qDebug()<<"ERROR! "<<qry.lastError();
    }

    int fila = 0;
    ui->tablaDoctores_2->setRowCount(0);
    while(qry.next()){
        ui->tablaDoctores_2->insertRow(fila);
        ui->tablaDoctores_2->setItem(fila,0,new QTableWidgetItem(qry.value(0).toByteArray().constData()));
        ui->tablaDoctores_2->setItem(fila,1,new QTableWidgetItem(qry.value(1).toByteArray().constData()));
        ui->tablaDoctores_2->setItem(fila,2,new QTableWidgetItem(qry.value(2).toByteArray().constData()));
        fila++;
    }
}

void MainWindow::consultarAgenda()
{
   QSqlQuery qry;
   QString consulta;
   consulta.append("select Paciente.Nombre, Paciente.Apellido_P,Paciente.Apellido_M,Cita.Nss_Pac,Cita.Fecha,Cita.Hora from Paciente inner join Cita on Cita.Nss_Pac where Cita.Nss_Pac=Paciente.Nss and Nss_med ='"+ID_user+"'ORDER BY Cita.Fecha ASC");
   qry.prepare(consulta);

   if(qry.exec()){
       qDebug()<<"Se ha consultado correctamente";
   }
   else{
       qDebug()<<"No se ha consultado correctamente";
       qDebug()<<"ERROR! "<<qry.lastError();
   }

   int fila=0;
   ui->ver_citas->setRowCount(0);
    while(qry.next()){
       ui->ver_citas->insertRow(fila);
       ui->ver_citas->setItem(fila,0,new QTableWidgetItem(qry.value(0).toByteArray().constData()));
       ui->ver_citas->setItem(fila,1,new QTableWidgetItem(qry.value(1).toByteArray().constData()));
       ui->ver_citas->setItem(fila,2,new QTableWidgetItem(qry.value(2).toByteArray().constData()));
       ui->ver_citas->setItem(fila,3,new QTableWidgetItem(qry.value(3).toByteArray().constData()));
       ui->ver_citas->setItem(fila,4,new QTableWidgetItem(qry.value(4).toByteArray().constData()));
       ui->ver_citas->setItem(fila,5,new QTableWidgetItem(qry.value(5).toByteArray().constData()));
       fila++;
    }
}


void MainWindow::createUser(QString name, QString last1, QString last2, QString rol, QString ID_user)
{
    QSqlQuery qry,consul;
            QMessageBox message;
            string ss = name.toStdString().substr(0,2)+last1.toStdString().substr(0,1)
                        +last2.toStdString().substr(0,1);
            QString user = QString::fromStdString(ss);

            int al = rand()%(100000-10000);

            QString consultar;
            consultar.append("select ID from Usuario order by ID desc limit 1");
            consul.prepare(consultar);

            if(consul.exec()){
                       qDebug()<<"Se ha consultado correctamente.";
            }else{
                    qDebug()<<"No se ha consultado correctamente.";
                    qDebug()<<"ERROR! "<<qry.lastError();
                }
          consul.next();
           QString idmax = consul.value(0).toByteArray().constData();
           userid = idmax.toInt() + 1;
            QString pass = QString::number(al), id = QString::number(userid),sentencia;

            sentencia.append("INSERT INTO Usuario VALUES("+id+",'"+user+"','"+pass+"','"+rol+"','"+ID_user+"')");
            qry.prepare(sentencia);
            if(qry.exec()){
                           qDebug()<<"Se ha insertado corectamente.";
                           message.setText("Usuario: "+user+" Contraseña: "+pass);

                }else{
                        qDebug()<<"No se ha insertado correctamente.";
                        qDebug()<<"ERROR! "<<qry.lastError();
                        message.setText("No se ha podido crear Usuario");
                    }
            message.exec();
}

void MainWindow::on_saveP_clicked()
{
    QString rol = "paciente";
    InsertPat();
    createUser(ui->namesPLE->text(),ui->lastnamePLE1->text(),ui->lastnamePLE2->text(),rol,ui->nssPLE->text());
    DeleteLEPat();


}

void MainWindow::on_backCA_clicked()
{
    ui->SW->setCurrentIndex(2);
}

void MainWindow::on_SearchNssPB_clicked()
{
   searchPatAgenda(ui->SearchNss->text());
}

void MainWindow::on_SearchNss_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    if(ui->SearchNss->text().length()>0){
        ui->SearchNssPB->setEnabled(true);
    }
}



void MainWindow::on_tablaDoctores_itemSelectionChanged()
{
    reestablecerCB2();
    ui->CalendarApp->setEnabled(true);
    int row = ui->tablaDoctores->currentRow();
    QTableWidgetItem nombre = *ui->tablaDoctores->item(row,0);
    QTableWidgetItem apellido = *ui->tablaDoctores->item(row,1);
    QString n = nombre.text(); QString a = apellido.text();

    QSqlQuery qry;
    QString consul;
    consul.append("select Nss from Medico where Nombre = '"+n+"' and Apellido_P = '"+a+"'");
    qry.prepare(consul);

    if(qry.exec()){
           qDebug()<<"Se ha consultado correctamente.";
}else{
        qDebug()<<"No se ha consultado correctamente.";
        qDebug()<<"ERROR! "<<qry.lastError();
    }
    qry.next();
    idmed = qry.value(0).toByteArray().constData();

}

void MainWindow::on_save_appo_clicked()
{
    InsertApp();
    DeleteLEApp();
}

void MainWindow::on_AddAppoPB_clicked()
{
        ui->SW->setCurrentIndex(5);
}

void MainWindow::on_botonvagenda_clicked()
{
    QSqlQuery qry;
    consultarAgenda();
    ui->SW->setCurrentIndex(10);
    //etiqueta

    QString consultar;
                    consultar.append("select Nombre,Apellido_P, Apellido_M from Medico where Nss = '"+ID_user+"'");
                    qry.prepare(consultar);qry.exec();

                    qry.next();
                    consultar = qry.value(0).toByteArray().constData();
                    consultar+=" ";
                    consultar.append(qry.value(1).toByteArray().constData());
                    consultar+=" ";
                    consultar.append(qry.value(2).toByteArray().constData());


                       ui->doc_LB->setText(consultar);
    //fin*etiqueta

}


void MainWindow::on_perfil_PB_clicked()
{
    ui->SW->setCurrentIndex(8);
    ui->nombre_LB_2->setText(paciente.getNombre());
    ui->apP_LB_2->setText(paciente.getApellidoP());
    ui->apM_LB_2->setText(paciente.getApellidoM());
    ui->curp_LB_2->setText(paciente.getCurp());
    ui->nss_LB_2->setText(paciente.getNss());
    ui->telefono_LB_2->setText(paciente.getTelefono());
    ui->dom_LB_2->setText(paciente.getDomicilio());
    ui->dom_LB_3->setText(paciente.getDateNac());
}

void MainWindow::on_agendar_PB_clicked()
{
    ui->SW->setCurrentIndex(9);
    ui->nombrepac_LB->setText(paciente.getNombre()+" "+paciente.getApellidoP()+" "+paciente.getApellidoM());

}

void MainWindow::on_backCA_2_clicked()
{
    ui->SW->setCurrentIndex(7);
}

void MainWindow::on_backCA_3_clicked()
{
    ui->SW->setCurrentIndex(7);
}



void MainWindow::on_save_appo_2_clicked()
{
    QSqlQuery qry,consul;
    QMessageBox message;
    QString consultar;
       consultar.append("select Id_cita from Cita order by Id_cita desc limit 1");
       consul.prepare(consultar);

       if(consul.exec()){
                  qDebug()<<"Se ha consultado correctamente.";
       }else{
               qDebug()<<"No se ha consultado correctamente.";
               qDebug()<<"ERROR! "<<qry.lastError();
           }
        consul.next();
      QString idmax = consul.value(0).toByteArray().constData();
      int aux = idmax.toInt() + 1;
     QString hora=ui->hora_CB->currentText().replace(":","-")+"-00";
    idmax = QString::number(aux);
    QString sentencia;
    sentencia.append("insert into Cita values("+idmax+",'"+paciente.getNss()+"','"+idmed+"','"+fecha+"','"+hora+"')");
    qry.prepare(sentencia);
    if(qry.exec()){
                   qDebug()<<"Se ha insertado corectamente.";
                   message.setText("Cita agendada el: "+fecha+" a las "+hora);
                   message.exec();
        }else{
                qDebug()<<"No se ha insertado correctamente.";
                qDebug()<<"ERROR! "<<qry.lastError();
            }
    DeleteLEApp2();
}

void MainWindow::setPaciente()
{
        QSqlQuery qry;
        QString consulta="select * from Paciente where Nss = ";
        consulta+=ID_user;
        qry.prepare(consulta);

        if(qry.exec()){
               qDebug()<<"Se ha consultado correctamente.";
    }else{

            qDebug()<<"No se ha consultado correctamente.";
            qDebug()<<"ERROR! "<<qry.lastError();
        }


        while(qry.next()){

            paciente.setNombre(qry.value(0).toByteArray().constData());
            paciente.setApellidoP(qry.value(1).toByteArray().constData());
            paciente.setApellidoM(qry.value(2).toByteArray().constData());
            paciente.setCurp(qry.value(3).toByteArray().constData());
            paciente.setNss(qry.value(4).toByteArray().constData());
            paciente.setTelefono(qry.value(5).toByteArray().constData());
            paciente.setDomicilio(qry.value(6).toByteArray().constData());
            paciente.setDateNac(qry.value(7).toByteArray().constData());
        }
}



void MainWindow::on_backRD_2_clicked()
{
    ui->SW->setCurrentIndex(6);
}

void MainWindow::on_logOutAdmin_2_clicked()
{

    ui->SW->setCurrentIndex(0);

}

void MainWindow::on_botonvagenda_2_clicked()
{

    ui->SW->setCurrentIndex(11);
    //etiqueta
    QString consultar;
     QSqlQuery qry;
                    consultar.append("select Nombre,Apellido_P, Apellido_M from Medico where Nss = '"+ID_user+"'");
                    qry.prepare(consultar);qry.exec();

                    qry.next();
                    consultar = qry.value(0).toByteArray().constData();
                    consultar+=" ";
                    consultar.append(qry.value(1).toByteArray().constData());
                    consultar+=" ";
                    consultar.append(qry.value(2).toByteArray().constData());

                       ui->doc_nombre_LB->setText(consultar);
    //fin*etiqueta

}

void MainWindow::on_backCA_4_clicked()
{
    ui->SW->setCurrentIndex(6);
}

void MainWindow::on_pushButton_clicked()
{
    QString nombre=ui->Nombre->text();
    QString nombreM=ui->doc_nombre_LB->text();
        QString enfermedad=ui->Enfermedad->text();
        QString medicamentos=ui->Medicamentos->text();
        QString Descripcion=ui->Descripcion->toPlainText();
        std::string cadenaStd=Descripcion.toStdString();
        int romper=Descripcion.length()+1;
        QString fecha = QDateTime :: currentDateTime (). toString ("dd-MM-yyyy");
        QString Definitivo;
        int a=0;
        QPdfWriter pdf("Recetas/Receta"+nombre+"("+fecha+").pdf");
        QPainter painter (&pdf);
        painter.drawPixmap(QRect(100,1500,9000,9000),QPixmap("hospi.PNG"));
        painter.drawPixmap(QRect(100,100,1500,1500),QPixmap("logo.PNG"));
        painter.setPen(Qt::black);
        painter.setFont(QFont("Arial",12));
        painter.drawText(500,2500,"Nombre del Medico:");
        painter.drawText(3000,2500,nombreM);
        painter.drawText(500,2800,"Nombre del Paciente:");
        painter.drawText(3000,2800,nombre);
        painter.drawText(500,3100,"Diagnóstico:");
        painter.drawText(3000,3100,enfermedad);
        painter.drawText(500,3400,"Medicamentos:");
        painter.drawText(3000,3400,medicamentos);
        painter.setFont(QFont("Aharoni",12));
        painter.drawText(500,3900,"Indicaciones: ");
        for(int i=0;i<3500;i++)
        {
            painter.drawText(500,4100+a,Definitivo);
            if(i==romper)
                break;
            if(i%70==0){
                a=a+300;
             Definitivo='\0';
            }
            Definitivo=Definitivo+cadenaStd[i];
        }

        painter.setPen(QPen(Qt::black, 20, Qt::SolidLine, Qt::RoundCap));
        painter.drawLine(7000, 12000, 2000, 12000);
        painter.drawText(4000,12500,"Firma del doctor");
        painter.drawText(4000,13000,fecha);
        painter.end();

        QMessageBox message;
        message.setText("la receta ha sido guardada exitosamente");
        message.exec();
}

void MainWindow::on_backRP_3_clicked()
{
    ui->SW->setCurrentIndex(6);
}

void MainWindow::on_botonvagenda_3_clicked()
{
    QSqlQuery qry;
    ui->SW->setCurrentIndex(12);
    ui->NSSMedico_2->setText(ID_user);
    //etiqueta
    QString consultar;
                    consultar.append("select Nombre,Apellido_P, Apellido_M from Medico where Nss = '"+ID_user+"'");
                    qry.prepare(consultar);qry.exec();

                    qry.next();
                    consultar = qry.value(0).toByteArray().constData();
                    consultar+=" ";
                    consultar.append(qry.value(1).toByteArray().constData());
                    consultar+=" ";
                    consultar.append(qry.value(2).toByteArray().constData());

                       ui->nombre_DOC->setText(consultar);
    //fin*etiqueta
}

void MainWindow::on_saveP_2_clicked()
{
    int id_EXP;
    QSqlQuery consul;
    QString consultar;
    consultar.append("select id_exp from Expediente order by id_exp desc limit 1");
    consul.prepare(consultar);

    if(consul.exec()){
               qDebug()<<"Se ha consultado correctamente.";
    }else{
            qDebug()<<"No se ha consultado correctamente.";
            qDebug()<<"ERROR! "<<consul.lastError();
        }
    consul.next();
    QString idmax = consul.value(0).toByteArray().constData();
    id_EXP=idmax.toInt()+1;
    idmax=QString::number(id_EXP);
    QSqlQuery qry;
    QMessageBox message;
    QString pac1 = ui->pac->text(), fetch1 = ui->fetch->text(),est1 = ui->est->text(),pes1 = ui->pes->text(),Sangre = ui->Sangre->text(),observaciones=ui->observaciones->text();
    qry.exec("insert into Expediente values("+idmax+","+pac1+","+ID_user+",'"+fetch1+"',"+est1+","+pes1+",'"+Sangre+"','"+observaciones+"')");
    message.setText("El expediente fue guardado exitosamente");
    message.exec();
    LimpiarExp();
}

void MainWindow::LimpiarExp()
{
    ui->pac->clear();
    ui->fetch->clear();
    ui->est->clear();
    ui->pes->clear();
    ui->Sangre->clear();
    ui->observaciones->clear();
}

void MainWindow::reestablecerCB()
{
    ui->hora_CB->clear();

        ui->hora_CB->addItem("09:00");
        ui->hora_CB->addItem("09:30");
        ui->hora_CB->addItem("10:00");
        ui->hora_CB->addItem("10:30");
        ui->hora_CB->addItem("11:00");
        ui->hora_CB->addItem("11:30");
        ui->hora_CB->addItem("12:00");
        ui->hora_CB->addItem("12:30");
        ui->hora_CB->addItem("13:00");
        ui->hora_CB->addItem("13:30");
        ui->hora_CB->addItem("14:00");
        ui->hora_CB->addItem("15:00");
        ui->hora_CB->addItem("15:30");
        ui->hora_CB->addItem("16:00");
        ui->hora_CB->addItem("16:30");
        ui->hora_CB->addItem("17:00");
        ui->hora_CB->addItem("17:30");
        ui->hora_CB->addItem("18:00");
        ui->hora_CB->addItem("18:30");
        ui->hora_CB->addItem("19:00");
        ui->hora_CB->addItem("19:30");
        ui->hora_CB->addItem("20:00");
        ui->hora_CB->addItem("20:30");
}

void MainWindow::reestablecerCB2()
{
    ui->horaApp->clear();

        ui->horaApp->addItem("09:00");
        ui->horaApp->addItem("09:30");
        ui->horaApp->addItem("10:00");
        ui->horaApp->addItem("10:30");
        ui->horaApp->addItem("11:00");
        ui->horaApp->addItem("11:30");
        ui->horaApp->addItem("12:00");
        ui->horaApp->addItem("12:30");
        ui->horaApp->addItem("13:00");
        ui->horaApp->addItem("13:30");
        ui->horaApp->addItem("14:00");
        ui->horaApp->addItem("15:00");
        ui->horaApp->addItem("15:30");
        ui->horaApp->addItem("16:00");
        ui->horaApp->addItem("16:30");
        ui->horaApp->addItem("17:00");
        ui->horaApp->addItem("17:30");
        ui->horaApp->addItem("18:00");
        ui->horaApp->addItem("18:30");
        ui->horaApp->addItem("19:00");
        ui->horaApp->addItem("19:30");
        ui->horaApp->addItem("20:00");
        ui->horaApp->addItem("20:30");
}

void MainWindow::searchMed(QString nss)
{


    QSqlQuery qry;
    QString name;
    name.append("select * from Medico where Nss = "+nss);
    qry.prepare(name);

    if(qry.exec()){
           qDebug()<<"Se ha consultado correctamente.";
}else{
        qDebug()<<"No se ha consultado correctamente.";
        qDebug()<<"ERROR! "<<qry.lastError();
    }
    qry.next();
    ui->namesDLE->setText(qry.value(0).toByteArray().constData());
    ui->lastnameDLE1->setText(qry.value(1).toByteArray().constData());
    ui->lastnameDLE2->setText(qry.value(2).toByteArray().constData());
    ui->sexDLE->setText(qry.value(3).toByteArray().constData());
    ui->dobDLE->setText(qry.value(4).toByteArray().constData());
    ui->curpDLE->setText(qry.value(5).toByteArray().constData());
    ui->nssDLE->setText(qry.value(6).toByteArray().constData());
    ui->addressDLE->setText(qry.value(7).toByteArray().constData());

    for (int i(0);i<=ui->comboSD->count();i++) {
        if(ui->comboSD->currentText() != qry.value(8).toByteArray().constData()){
            ui->comboSD->setCurrentIndex(i+1);
        }else{
            break;
        }
    }
   ui->hiredDLE->setText(qry.value(9).toByteArray().constData());
   ui->saveD->setEnabled(false);
   ui->UpdateMedPB->setEnabled(true);
   ui->CancelarMedPB->setEnabled(true);
   ui->nssDLE->setEnabled(false);

}

void MainWindow::searchPat(QString nss)
{
    QSqlQuery qry;
    QString name;
    name.append("select * from Paciente where Nss = "+nss);
    qry.prepare(name);

    if(qry.exec()){
           qDebug()<<"Se ha consultado correctamente.";
}else{
        qDebug()<<"No se ha consultado correctamente.";
        qDebug()<<"ERROR! "<<qry.lastError();
    }
    qry.next();
    ui->namesPLE->setText(qry.value(0).toByteArray().constData());
    ui->lastnamePLE1->setText(qry.value(1).toByteArray().constData());
    ui->lastnamePLE2->setText(qry.value(2).toByteArray().constData());
    ui->curpPLE->setText(qry.value(3).toByteArray().constData());
    ui->nssPLE->setText(qry.value(4).toByteArray().constData());
    ui->telPLE->setText(qry.value(5).toByteArray().constData());
    ui->addressPLE->setText(qry.value(6).toByteArray().constData());
    ui->fecha_->setText(qry.value(7).toByteArray().constData());

    ui->nssPLE->setEnabled(false);
    ui->saveP->setEnabled(false);
    ui->UpdatePatPB->setEnabled(true);
    ui->CancelarPatPB->setEnabled(true);
}

void MainWindow::searchPatAgenda(QString nss)
{
    QSqlQuery qry;
    QString name;
    name.append("select Nombre,Apellido_p,Apellido_M,Nss from Paciente where Nss = "+nss);
    qry.prepare(name);

    if(qry.exec()){
           qDebug()<<"Se ha consultado correctamente.";
}else{
        qDebug()<<"No se ha consultado correctamente.";
        qDebug()<<"ERROR! "<<qry.lastError();
    }
    qry.next();
    idpat = qry.value(3).toByteArray().constData();
    ui->NamePLabel->setText(qry.value(0).toByteArray().constData());
    ui->LastP1L->setText(qry.value(1).toByteArray().constData());
    ui->LastP2L->setText(qry.value(2).toByteArray().constData());
}

void MainWindow::on_logOutAdmin_3_clicked()
{
    ui->SW->setCurrentIndex(0);
}


void MainWindow::on_tablaDoctores_2_itemSelectionChanged()
{
    reestablecerCB();
    ui->calendarWidget->setEnabled(true);
    int row = ui->tablaDoctores_2->currentRow();
    QTableWidgetItem nombre = *ui->tablaDoctores_2->item(row,0);
    QTableWidgetItem apellido = *ui->tablaDoctores_2->item(row,1);
    QString n = nombre.text(); QString a = apellido.text();

    QSqlQuery qry;
    QString consul;
    consul.append("select Nss from Medico where Nombre = '"+n+"' and Apellido_P = '"+a+"'");
    qry.prepare(consul);

    if(qry.exec()){
           qDebug()<<"Se ha consultado correctamente.";
}else{
        qDebug()<<"No se ha consultado correctamente.";
        qDebug()<<"ERROR! "<<qry.lastError();
    }
    qry.next();
    idmed = qry.value(0).toByteArray().constData();

}



void MainWindow::on_comboSD_3_currentTextChanged(const QString &arg1)
{
    consultarDocs2(arg1);
}


void MainWindow::on_comboSD_2_currentTextChanged(const QString &arg1)
{
    consultarDocs(arg1);
}


void MainWindow::on_botonvagenda_4_clicked()
{
    QSqlQuery qry;
    consultarpacientes();
    ui->SW->setCurrentIndex(14);
    //etiqueta
    QString consultar;
                    consultar.append("select Nombre,Apellido_P, Apellido_M from Medico where Nss = '"+ID_user+"'");
                    qry.prepare(consultar);
                    qry.exec();
                    qry.next();
                    consultar = qry.value(0).toByteArray().constData();
                    consultar+=" ";
                    consultar.append(qry.value(1).toByteArray().constData());
                    consultar+=" ";
                    consultar.append(qry.value(2).toByteArray().constData());

                       ui->doc_LB_2->setText(consultar);
    //fin*etiqueta
}


void MainWindow::consultarpacientes()
{
   QSqlQuery qry;
   QString consulta;
   consulta.append("select distinct Paciente.Nombre,Paciente.Apellido_P,Paciente.Apellido_M,Paciente.Curp,Paciente.Nss,Paciente.Telefono,Paciente.Domicilio from Paciente inner join Cita on Cita.Nss_med where Cita.Nss_med='"+ID_user+"' and Paciente.Nss=Cita.Nss_Pac");
   qry.prepare(consulta);

   if(qry.exec()){
       qDebug()<<"Se ha consultado correctamente";
   }
   else{
       qDebug()<<"No se ha consultado correctamente";
       qDebug()<<"ERROR! "<<qry.lastError();
   }

   int fila=0;
   ui->ver_pacientes->setRowCount(0);
    while(qry.next()){
       ui->ver_pacientes->insertRow(fila);
       ui->ver_pacientes->setItem(fila,0,new QTableWidgetItem(qry.value(0).toByteArray().constData()));
       ui->ver_pacientes->setItem(fila,1,new QTableWidgetItem(qry.value(1).toByteArray().constData()));
       ui->ver_pacientes->setItem(fila,2,new QTableWidgetItem(qry.value(2).toByteArray().constData()));
       ui->ver_pacientes->setItem(fila,3,new QTableWidgetItem(qry.value(3).toByteArray().constData()));
       ui->ver_pacientes->setItem(fila,4,new QTableWidgetItem(qry.value(4).toByteArray().constData()));
       ui->ver_pacientes->setItem(fila,5,new QTableWidgetItem(qry.value(5).toByteArray().constData()));
       ui->ver_pacientes->setItem(fila,6,new QTableWidgetItem(qry.value(6).toByteArray().constData()));
       fila++;
    }
}

void MainWindow::on_backRP_4_clicked()
{
    ui->SW->setCurrentIndex(6);
}




void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    QSqlQuery qry,consul;
    QMessageBox message;
    QString consultar;
    fecha=date.toString("yyyy.MM.dd").replace(".","-");
       consultar.append("select Hora from Cita WHERE Fecha='");
       consultar+=fecha+"'";
       consultar+=(" AND Nss_med='"+idmed+"'");
       consul.prepare(consultar);

       if(consul.exec()){
                  qDebug()<<"Se ha consultado correctamente.";
       }else{
               qDebug()<<"No se ha consultado correctamente.";
               qDebug()<<"ERROR! "<<qry.lastError();
           }

        while(consul.next()){
            for(int i= ui->hora_CB->count();i>=0;i--){
                QString cadenaH=ui->hora_CB->itemText(i).replace(":","-")+"-00";

                if(cadenaH==consul.value(0).toByteArray().constData()){
                ui->hora_CB->removeItem(i);
                i=-1;
            }
            }

        }


}


void MainWindow::on_CalendarApp_clicked(const QDate &date)
{
    QSqlQuery qry,consul;
    QMessageBox message;
    QString consultar;
    fecha=date.toString("yyyy.MM.dd").replace(".","-");
       consultar.append("select Hora from Cita WHERE Fecha='");
       consultar+=fecha+"'";
       consultar+=(" AND Nss_med='"+idmed+"'");
       consul.prepare(consultar);

       if(consul.exec()){
                  qDebug()<<"Se ha consultado correctamente.";
       }else{
               qDebug()<<"No se ha consultado correctamente.";
               qDebug()<<"ERROR! "<<qry.lastError();
           }

        while(consul.next()){
            for(int i= ui->horaApp->count();i>=0;i--){
                QString cadenaH=ui->horaApp->itemText(i).replace(":","-")+"-00";
                if(cadenaH==consul.value(0).toByteArray().constData()){
                ui->horaApp->removeItem(i);
                i=-1;
            }
            }

        }
}

void MainWindow::on_SearchMedLE_textChanged(const QString &arg1)
{
    if(arg1.length() > 0){
    ui->SearchMedPB->setEnabled(true);
    }
}

void MainWindow::on_SearchMedPB_clicked()
{
    DeleteLEMed();
    searchMed(ui->SearchMedLE->text());
}

void MainWindow::on_UpdateMedPB_clicked()
{
    QSqlQuery qry;
        QString name = ui->namesDLE->text(),lastname1 = ui->lastnameDLE1->text(),
                lastname2 = ui->lastnameDLE2->text(), sex = ui->sexDLE->text(), dateOfbirth = ui->dobDLE->text(),
                curp = ui->curpDLE->text(), nss = ui->nssDLE->text(), address = ui->addressDLE->text(),
                Spec = ui->comboSD->currentText(), dateHired = ui->hiredDLE->text(),sentencia;
        sentencia.append("UPDATE Medico SET Nombre ='"+name+"',Apellido_P = '"+lastname1+"',Apellido_M = '"+lastname2+"', Sexo ='"
                         +sex+"',Fecha_nac ='"+dateOfbirth+"',Curp = '"+curp+"',Direccion = '"+address+"',Especialidad = '"+Spec+"',Fecha_ing ='"
                         +dateHired+"' WHERE Nss = "+nss);
        qry.prepare(sentencia);
        if(qry.exec()){
                       qDebug()<<"Se ha Modificado corectamente.";
            }else{
                    qDebug()<<"No se ha Modificado correctamente.";
                    qDebug()<<"ERROR! "<<qry.lastError();
                }
        DeleteLEMed();
        ui->nssDLE->setEnabled(true);
        ui->SearchMedLE->setEnabled(true);
        ui->UpdateMedPB->setEnabled(false);
        ui->saveD->setEnabled(true);
        ui->SearchMedPB->setEnabled(false);
        ui->CancelarMedPB->setEnabled(false);
        ui->SearchMedLE->clear();
}

void MainWindow::on_CancelarMedPB_clicked()
{
    DeleteLEMed();
    ui->nssDLE->setEnabled(true);
    ui->SearchMedLE->setEnabled(true);
    ui->UpdateMedPB->setEnabled(false);
    ui->saveD->setEnabled(true);
    ui->SearchMedPB->setEnabled(false);
    ui->CancelarMedPB->setEnabled(false);
    ui->SearchMedLE->clear();
}

void MainWindow::on_SearchPatLE_textChanged(const QString &arg1)
{
    if(arg1.length() > 0){
    ui->SearchPatPB->setEnabled(true);
    }
}

void MainWindow::on_SearchPatPB_clicked()
{
    DeleteLEPat();
    searchPat(ui->SearchPatLE->text());
}
void MainWindow::on_UpdatePatPB_clicked()
{
    QSqlQuery qry;
        QString  name = ui->namesPLE->text(), lastname1 = ui->lastnamePLE1->text(),
                lastname2 = ui->lastnamePLE2->text(), curp = ui->curpPLE->text(), nss = ui->nssPLE->text(),
                telephoneN = ui->telPLE->text(), address = ui->addressPLE->text(),sentencia,
                fechaNacc = ui->fecha_->text();
        sentencia.append("UPDATE Paciente SET Nombre ='"+name+"',Apellido_P = '"+lastname1+"',Apellido_M = '"+lastname2+"',Curp = '"+curp+"',Telefono = "+telephoneN+",Domicilio = '"+address+"', fecha_nac ='"+fechaNacc+"' WHERE Nss = "+nss);
        qry.prepare(sentencia);
        if(qry.exec()){
                       qDebug()<<"Se ha insertado corectamente.";
            }else{
                    qDebug()<<"No se ha insertado correctamente.";
                    qDebug()<<"ERROR! "<<qry.lastError();
                }
        DeleteLEPat();
        ui->nssPLE->setEnabled(true);
        ui->SearchPatLE->setEnabled(true);
        ui->SearchPatPB->setEnabled(false);
        ui->UpdatePatPB->setEnabled(false);
        ui->saveP->setEnabled(true);
        ui->CancelarPatPB->setEnabled(false);
        ui->SearchPatLE->clear();
}

void MainWindow::on_CancelarPatPB_clicked()
{
    DeleteLEPat();
    ui->nssPLE->setEnabled(true);
    ui->SearchPatLE->setEnabled(true);
    ui->SearchPatPB->setEnabled(false);
    ui->UpdatePatPB->setEnabled(false);
    ui->saveP->setEnabled(true);
    ui->CancelarPatPB->setEnabled(false);
    ui->SearchPatLE->clear();
}


void MainWindow::on_showMeds_clicked()
{

    ui->SW->setCurrentIndex(15);
    QSqlQuery qry;
    QString consulta;
    consulta.append("select * from Medico");
    qry.prepare(consulta);

    if(qry.exec()){
           qDebug()<<"Se ha consultado correctamente.";
}else{
        qDebug()<<"No se ha consultado correctamente.";
        qDebug()<<"ERROR! "<<qry.lastError();
    }
    QStringList headers = { "Nombre/s", "Apellido P.", "Apellido M.", "Sexo", "Fecha Nac.","CURP","NSS","Direccion","Especialidad","Fecha Ing." };
    ui->verMed->setHorizontalHeaderLabels(headers);
    int fila = 0;
    ui->verMed->setRowCount(0);
    while(qry.next()){
        ui->verMed->insertRow(fila);
        ui->verMed->setItem(fila,0,new QTableWidgetItem(qry.value(0).toByteArray().constData()));
        ui->verMed->setItem(fila,1,new QTableWidgetItem(qry.value(1).toByteArray().constData()));
        ui->verMed->setItem(fila,2,new QTableWidgetItem(qry.value(2).toByteArray().constData()));
        ui->verMed->setItem(fila,3,new QTableWidgetItem(qry.value(3).toByteArray().constData()));
        ui->verMed->setItem(fila,4,new QTableWidgetItem(qry.value(4).toByteArray().constData()));
        ui->verMed->setItem(fila,5,new QTableWidgetItem(qry.value(5).toByteArray().constData()));
        ui->verMed->setItem(fila,6,new QTableWidgetItem(qry.value(6).toByteArray().constData()));
        ui->verMed->setItem(fila,7,new QTableWidgetItem(qry.value(7).toByteArray().constData()));
        ui->verMed->setItem(fila,8,new QTableWidgetItem(qry.value(8).toByteArray().constData()));
        ui->verMed->setItem(fila,9,new QTableWidgetItem(qry.value(9).toByteArray().constData()));
        fila++;
    }
}

void MainWindow::on_ShowPats_clicked()
{
    ui->SW->setCurrentIndex(16);
    QSqlQuery qry;
    QString consulta;
    consulta.append("select * from Paciente");
    qry.prepare(consulta);

    if(qry.exec()){
           qDebug()<<"Se ha consultado correctamente.";
}else{
        qDebug()<<"No se ha consultado correctamente.";
        qDebug()<<"ERROR! "<<qry.lastError();
    }
    QStringList headers = { "Nombre", "Apelldo Paterno", "Apelldo Materno", "CURP", "NSS", "Telefono","Domicilio" };
    ui->verPat->setHorizontalHeaderLabels(headers);
    int fila = 0;
    ui->verPat->setRowCount(0);
    while(qry.next()){
        ui->verPat->insertRow(fila);
        ui->verPat->setItem(fila,0,new QTableWidgetItem(qry.value(0).toByteArray().constData()));
        ui->verPat->setItem(fila,1,new QTableWidgetItem(qry.value(1).toByteArray().constData()));
        ui->verPat->setItem(fila,2,new QTableWidgetItem(qry.value(2).toByteArray().constData()));
        ui->verPat->setItem(fila,3,new QTableWidgetItem(qry.value(3).toByteArray().constData()));
        ui->verPat->setItem(fila,4,new QTableWidgetItem(qry.value(4).toByteArray().constData()));
        ui->verPat->setItem(fila,5,new QTableWidgetItem(qry.value(5).toByteArray().constData()));
        ui->verPat->setItem(fila,6,new QTableWidgetItem(qry.value(6).toByteArray().constData()));
        fila++;
    }
}

void MainWindow::on_updateMed_2_clicked()
{
     ui->SW->setCurrentIndex(3);
     int row = ui->verMed->currentRow();
     QTableWidgetItem n = *ui->verMed->item(row,6);
     QString nss = n.text();

     searchMed(nss);
     ui->verMed->clear();
}

void MainWindow::on_backVerMed_clicked()
{
    ui->verMed->clear();
    ui->SW->setCurrentIndex(2);
}

void MainWindow::on_update_med2_clicked()
{
    ui->SW->setCurrentIndex(4);
    int row = ui->verPat->currentRow();
    QTableWidgetItem n = *ui->verPat->item(row,4);
    QString nss = n.text();

    searchPat(nss);
    ui->verPat->clear();
}

void MainWindow::on_backVerPat_clicked()
{
    ui->verPat->clear();
    ui->SW->setCurrentIndex(2);
}

void MainWindow::on_addAppo_2_clicked()
{
    ui->SW->setCurrentIndex(5);
    int row = ui->verPat->currentRow();
    QTableWidgetItem n = *ui->verPat->item(row,4);
    QString nss = n.text();

    searchPatAgenda(nss);
    ui->verPat->clear();
}

void MainWindow::on_buscarexp_clicked()
{
    searchPatExp(ui->exp->text());
}

void MainWindow::searchPatExp(QString nss)
{
    QSqlQuery qry;
    QString name;
    name.append("select Id_exp,Nss_pac,Nss_med,Fecha_crea,Estatura,Peso,Tipo_Sangre,Observaciones from Expediente where Nss_pac = "+nss);
    qry.prepare(name);

    if(qry.exec()){
           qDebug()<<"Se ha consultado correctamente.";
}else{
        qDebug()<<"No se ha consultado correctamente.";
        qDebug()<<"ERROR! "<<qry.lastError();
    }
    qry.next();
    idpat = qry.value(8).toByteArray().constData();
    ui->idexpe->setText(qry.value(0).toByteArray().constData());
    ui->nsspaciente->setText(qry.value(1).toByteArray().constData());
    ui->nssmedico->setText(qry.value(2).toByteArray().constData());
    ui->fech->setText(qry.value(3).toByteArray().constData());
    ui->Estatura->setText(qry.value(4).toByteArray().constData());
    ui->peso->setText(qry.value(5).toByteArray().constData());
    ui->San_pac->setText(qry.value(6).toByteArray().constData());
    ui->Observacion->setText(qry.value(7).toByteArray().constData());
    //etiqueta
    QString consultar;
                    consultar.append("select Nombre,Apellido_P, Apellido_M from Paciente where Nss = '"+ui->nsspaciente->text()+"'");
                    qry.prepare(consultar);qry.exec();

                    qry.next();
                    consultar = qry.value(0).toByteArray().constData();
                    consultar+=" ";
                    consultar.append(qry.value(1).toByteArray().constData());
                    consultar+=" ";
                    consultar.append(qry.value(2).toByteArray().constData());

                       ui->nsspaciente_2->setText(consultar);
    //fin*etiqueta


}

void MainWindow::on_botonvagenda_5_clicked()
{
    QSqlQuery qry;
    ui->SW->setCurrentIndex(13);
    //etiqueta
    QString consultar;
                    consultar.append("select Nombre,Apellido_P, Apellido_M from Medico where Nss = '"+ID_user+"'");
                    qry.prepare(consultar);qry.exec();

                    qry.next();
                    consultar = qry.value(0).toByteArray().constData();
                    consultar+=" ";
                    consultar.append(qry.value(1).toByteArray().constData());
                    consultar+=" ";
                    consultar.append(qry.value(2).toByteArray().constData());

                       ui->nDoc_LB->setText(consultar);
    //fin*etiqueta
}


void MainWindow::on_backRP_5_clicked()
{
    ui->SW->setCurrentIndex(6);
    LimpiarBusExp();

}


void MainWindow::on_buscarexp_2_clicked()
{

}

void MainWindow::searchPatModExp(QString nss)
{
    QSqlQuery qry;
    QString name;
    name.append("select Id_exp,Nss_pac,Nss_med,Fecha_crea,Estatura,Peso,Tipo_Sangre,Observaciones from Expediente where Nss_pac = "+nss);
    qry.prepare(name);

    if(qry.exec()){
           qDebug()<<"Se ha consultado correctamente.";
}else{
        qDebug()<<"No se ha consultado correctamente.";
        qDebug()<<"ERROR! "<<qry.lastError();
    }
    qry.next();
    idpat = qry.value(8).toByteArray().constData();
    ui->Mod_id->setText(qry.value(0).toByteArray().constData());
    ui->Mos_nssPac->setText(qry.value(1).toByteArray().constData());
    ui->Mod_nssMed->setText(qry.value(2).toByteArray().constData());
    ui->ModFecha->setText(qry.value(3).toByteArray().constData());
    ui->ModEstatura->setText(qry.value(4).toByteArray().constData());
    ui->ModPeso->setText(qry.value(5).toByteArray().constData());
    ui->Sangre_3->setText(qry.value(6).toByteArray().constData());
    ui->Observaciones->setText(qry.value(7).toByteArray().constData());
    //etiqueta
    QString consultar;
                    consultar.append("select Nombre,Apellido_P, Apellido_M from Paciente where Nss = '"+ ui->Mos_nssPac->text()+"'");
                    qry.prepare(consultar);qry.exec();

                    qry.next();
                    consultar = qry.value(0).toByteArray().constData();
                    consultar+=" ";
                    consultar.append(qry.value(1).toByteArray().constData());
                    consultar+=" ";
                    consultar.append(qry.value(2).toByteArray().constData());

                       ui->Mos_nssPac->setText(consultar);
    //fin*etiqueta
}


void MainWindow::on_botonvagenda_6_clicked()
{
    ui->SW->setCurrentIndex(17);
}


void MainWindow::on_ModBuscar_clicked()
{
    searchPatModExp(ui->EditarExp->text());
}


void MainWindow::on_buscarexp_3_clicked()
{
    on_UpdateExp_clicked(ui->EditarExp->text());
}

void MainWindow::on_UpdateExp_clicked(QString nss)
{
    QSqlQuery qry;
        QString  id = ui->Mod_id->text(), Nsspaciente =nss,
                NssMedico = ui->Mod_nssMed->text(), fecha = ui->ModFecha->text(), Estatura = ui->ModEstatura->text(),
                peso = ui->ModPeso->text(),sangre=ui->Sangre_3->text(),observaciones=ui->Observaciones->text(),sentencia;
        QMessageBox message;
        sentencia.append("UPDATE Expediente SET Id_exp ='"+id+"',Nss_Pac = '"+Nsspaciente+"',Nss_med = '"+NssMedico+"',Fecha_crea = '"+fecha+"',Estatura = "+Estatura+",Peso = '"+peso+"',Tipo_Sangre='"+sangre+"',Observaciones='"+observaciones+"' WHERE Nss_Pac = "+nss);
        qry.prepare(sentencia);
        if(qry.exec()){
                       qDebug()<<"Se ha insertado corectamente.";
            }else{
                    qDebug()<<"No se ha insertado correctamente.";
                    qDebug()<<"ERROR! "<<qry.lastError();
                }
        message.setText("Se modifico el exepediente correctamente");
        message.exec();
        LimpiarModExp();
}

void MainWindow::LimpiarModExp()
{
    ui->Mod_id->clear();
    ui->Mos_nssPac->clear();
    ui->Mod_nssMed->clear();
    ui->ModFecha->clear();
    ui->ModEstatura->clear();
    ui->ModPeso->clear();
    ui->Sangre_3->clear();
    ui->Observaciones->clear();
    ui->EditarExp->clear();
}

void MainWindow::LimpiarBusExp()
{
    ui->idexpe->clear();
    ui->nsspaciente->clear();
    ui->nssmedico->clear();
    ui->fech->clear();
    ui->Estatura->clear();
    ui->peso->clear();
    ui->San_pac->clear();
    ui->Observacion->clear();
    ui->exp->clear();
}


void MainWindow::on_backVerPat_2_clicked()
{
    ui->SW->setCurrentIndex(6);
    LimpiarModExp();
}


void MainWindow::on_SearchNssPB_2_clicked()
{
    QSqlQuery qry;
    QString pacienteNss= ui->pac->text();
    QString consultar;
                    consultar.append("select Nombre,Apellido_P, Apellido_M from Paciente where Nss = '"+pacienteNss+"'");
                    qry.prepare(consultar);qry.exec();

                    qry.next();
                    consultar = qry.value(0).toByteArray().constData();
                    consultar+=" ";
                    consultar.append(qry.value(1).toByteArray().constData());
                    consultar+=" ";
                    consultar.append(qry.value(2).toByteArray().constData());

                       ui->nombre_LB_22->setText(consultar);

}

void MainWindow::on_buscarnssReceta_PB_clicked()
{
    //etiqueta
    QSqlQuery qry;
    QString consultar;
                    consultar.append("select Nombre,Apellido_P, Apellido_M from Paciente where Nss = '"+ui->NSS_LE->text()+"'");
                    qry.prepare(consultar);qry.exec();

                    qry.next();
                    consultar = qry.value(0).toByteArray().constData();
                    consultar+=" ";
                    consultar.append(qry.value(1).toByteArray().constData());
                    consultar+=" ";
                    consultar.append(qry.value(2).toByteArray().constData());

                       ui->Nombre->setText(consultar);
    //fin*etiqueta
}
