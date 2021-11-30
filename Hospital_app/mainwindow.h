#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>
#include <QString>
#include <windows.h>
#include <stdio.h>
#include <QMessageBox>
#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <paciente.h>
using namespace std;

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

    void on_cancelar_PB_clicked();

    void on_aceptar_PB_clicked();

    void on_backRD_clicked();

    void on_AddDoc_clicked();

    void on_logOutAdmin_clicked();

    void on_backRP_clicked();

    void on_AddPatient_clicked();

    void on_saveD_clicked();

    void on_saveP_clicked();

    void on_backCA_clicked();

    void on_SearchNssPB_clicked();

    void on_SearchNss_textChanged(const QString &arg1);

    void on_tablaDoctores_itemSelectionChanged();

    void on_save_appo_clicked();

    void on_AddAppoPB_clicked();

    void on_botonvagenda_clicked();

    void on_perfil_PB_clicked();

    void on_agendar_PB_clicked();

    void on_backCA_2_clicked();

    void on_backCA_3_clicked();

    void on_save_appo_2_clicked();

    void setPaciente();

    void consultarDocs2(QString especialidad);

    void on_backRD_2_clicked();

    void on_logOutAdmin_2_clicked();

    void on_botonvagenda_2_clicked();

    void on_backCA_4_clicked();

    void on_pushButton_clicked();

    void on_backRP_3_clicked();

    void on_botonvagenda_3_clicked();

    void on_saveP_2_clicked();

    void on_logOutAdmin_3_clicked();

    void on_tablaDoctores_2_itemSelectionChanged();




    void on_comboSD_3_currentTextChanged(const QString &arg1);

    void on_comboSD_2_currentTextChanged(const QString &arg1);

    void on_botonvagenda_4_clicked();

    void on_backRP_4_clicked();


    void on_calendarWidget_clicked(const QDate &date);

    void on_CalendarApp_clicked(const QDate &date);

    void on_SearchMedLE_textChanged(const QString &arg1);

    void on_SearchMedPB_clicked();

    void on_UpdateMedPB_clicked();

    void on_CancelarMedPB_clicked();

    void on_SearchPatLE_textChanged(const QString &arg1);

    void on_UpdatePatPB_clicked();

    void on_CancelarPatPB_clicked();

    void on_SearchPatPB_clicked();

    void on_showMeds_clicked();

    void on_ShowPats_clicked();

    void on_updateMed_2_clicked();


    void on_backVerMed_clicked();

    void on_update_med2_clicked();

    void on_backVerPat_clicked();

    void on_addAppo_2_clicked();

    void on_buscarexp_clicked();

    void on_botonvagenda_5_clicked();

    void on_backRP_5_clicked();

    void on_buscarexp_2_clicked();

    void on_botonvagenda_6_clicked();

    void on_ModBuscar_clicked();

    void on_buscarexp_3_clicked();

    void on_backVerPat_2_clicked();

    void on_SearchNssPB_2_clicked();

    void on_buscarnssReceta_PB_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase hospitalDb;
    int contM = 0;
    int userid;
    QString patid;
    QString idpat;
    QString idmed;
    QString fecha;
    void InsertMed();
    void InsertPat();
    void InsertApp();
    void DeleteLEPat();
    void DeleteLEMed();
    void DeleteLEApp();
    void DeleteLEApp2();
    void consultarDocs(QString especialidad);
    void consultarAgenda();
    void consultarpacientes();
    void createUser(QString name, QString last1, QString last2, QString rol, QString ID_user);
    void LimpiarExp();
    void LimpiarBusExp();
    void LimpiarModExp();
    void reestablecerCB();
    void reestablecerCB2();
    void searchMed(QString nss);
    void searchPat(QString nss);
    void searchPatAgenda(QString nss);
    void searchPatExp(QString nss);
    void searchPatModExp(QString nss);
    void on_UpdateExp_clicked(QString nss);
    QString rol;
    QString ID_user;
    Paciente paciente;

};

#endif // MAINWINDOW_H
