#include "paciente.h"

Paciente::Paciente()
{

}

QString Paciente::getNombre() const
{
    return nombre;
}

void Paciente::setNombre(const QString &value)
{
    nombre = value;
}

QString Paciente::getApellidoP() const
{
    return apellidoP;
}

void Paciente::setApellidoP(const QString &value)
{
    apellidoP = value;
}

QString Paciente::getApellidoM() const
{
    return apellidoM;
}

void Paciente::setApellidoM(const QString &value)
{
    apellidoM = value;
}

QString Paciente::getCurp() const
{
    return curp;
}

void Paciente::setCurp(const QString &value)
{
    curp = value;
}

QString Paciente::getNss() const
{
    return nss;
}

void Paciente::setNss(const QString &value)
{
    nss = value;
}

QString Paciente::getTelefono() const
{
    return telefono;
}

void Paciente::setTelefono(const QString &value)
{
    telefono = value;
}

QString Paciente::getDomicilio() const
{
    return domicilio;
}

void Paciente::setDomicilio(const QString &value)
{
    domicilio = value;
}

QString Paciente::getDateNac() const
{
    return dateNac;
}

void Paciente::setDateNac(const QString &value)
{
    dateNac = value;
}
