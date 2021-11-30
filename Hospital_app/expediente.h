#ifndef EXPEDIENTE_H
#define EXPEDIENTE_H
#include <iostream>
#include <QString>

using namespace std;


class Expediente
{
public:
    Expediente();

    QString getId_Paciente() const;
    void setId_Paciente(const QString &value);

    QString getId_medico() const;
    void setId_medico(const QString &value);

    QString getFechacreacion() const;
    void setFechacreacion(const QString &value);

    QString getEstatura() const;
    void setEstatura(const QString &value);

    QString getPeso() const;
    void setPeso(const QString &value);


private:
    QString Id_Paciente;
    QString Id_medico;
    QString Fechacreacion;
    QString Estatura;
    QString Peso;


};

#endif // EXPEDIENTE_H
