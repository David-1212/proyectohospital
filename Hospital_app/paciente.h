#ifndef PACIENTE_H
#define PACIENTE_H
#include <iostream>
#include <QString>

using namespace std;

class Paciente
{
public:
    Paciente();

    QString getId_Pac() const;
    void setId_Pac(const QString &value);

    QString getNombre() const;
    void setNombre(const QString &value);

    QString getApellidoP() const;
    void setApellidoP(const QString &value);

    QString getApellidoM() const;
    void setApellidoM(const QString &value);

    QString getCurp() const;
    void setCurp(const QString &value);

    QString getNss() const;
    void setNss(const QString &value);

    QString getTelefono() const;
    void setTelefono(const QString &value);

    QString getDomicilio() const;
    void setDomicilio(const QString &value);

    QString getDateNac() const;
    void setDateNac(const QString &value);

private:

    QString nombre;
    QString apellidoP;
    QString apellidoM;
    QString curp;
    QString nss;
    QString telefono;
    QString domicilio;
    QString dateNac;

};

#endif // PACIENTE_H
