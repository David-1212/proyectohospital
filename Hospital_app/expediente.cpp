#include "expediente.h"
Expediente::Expediente()
{

}

QString Expediente::getId_Paciente() const
{
    return Id_Paciente;
}

void Expediente::setId_Paciente(const QString &value)
{
    Id_Paciente = value;
}

QString Expediente::getId_medico() const
{
    return Id_medico;
}

void Expediente::setId_medico(const QString &value)
{
    Id_medico = value;
}

QString Expediente::getFechacreacion() const
{
    return Fechacreacion;
}

void Expediente::setFechacreacion(const QString &value)
{
    Fechacreacion = value;
}

QString Expediente::getEstatura() const
{
    return Estatura;
}

void Expediente::setEstatura(const QString &value)
{
    Estatura = value;
}

QString Expediente::getPeso() const
{
    return Peso;
}

void Expediente::setPeso(const QString &value)
{
     Peso = value;
}
