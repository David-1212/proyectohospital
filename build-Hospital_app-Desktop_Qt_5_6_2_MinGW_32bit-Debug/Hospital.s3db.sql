BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "Cita" (
	"Id_cita"	INTEGER,
	"Nss_Pac"	INTEGER NOT NULL,
	"Nss_med"	INTEGER NOT NULL,
	"Fecha"	DATE NOT NULL,
	"Hora"	TIME NOT NULL,
	FOREIGN KEY("Nss_med") REFERENCES "Medico"("Nss"),
	FOREIGN KEY("Nss_Pac") REFERENCES "Paciente"("Nss")
);
CREATE TABLE IF NOT EXISTS "Paciente" (
	"Nombre"	VARCHAR(30) NOT NULL,
	"Apellido_P"	VARCHAR(15) NOT NULL,
	"Apellido_M"	VARCHAR(15) NOT NULL,
	"Curp"	VARCHAR(20) NOT NULL UNIQUE,
	"Nss"	TEXT NOT NULL UNIQUE,
	"Telefono"	INTEGER NOT NULL,
	"Domicilio"	VARCHAR(30) NOT NULL,
	"fecha_nac"	date,
	PRIMARY KEY("Nss")
);
CREATE TABLE IF NOT EXISTS "Medico" (
	"Nombre"	VARCHAR(30) NOT NULL,
	"Apellido_P"	VARCHAR(15) NOT NULL,
	"Apellido_M"	VARCHAR(15),
	"Sexo"	VARCHAR(1) NOT NULL,
	"Fecha_nac"	DATE NOT NULL,
	"Curp"	VARCHAR(15) NOT NULL UNIQUE,
	"Nss"	TEXT NOT NULL UNIQUE,
	"Direccion"	VARCHAR(30) NOT NULL,
	"Especialidad"	VARCHAR(30) NOT NULL,
	"Fecha_ing"	DATE NOT NULL,
	PRIMARY KEY("Nss")
);
CREATE TABLE IF NOT EXISTS "Usuario" (
	"ID"	INTEGER NOT NULL,
	"Nombre_usuario"	VARCHAR(5) NOT NULL UNIQUE,
	"Contrasena"	VARCHAR(5) NOT NULL,
	"Rol"	VARCHAR(10),
	"NSS_user"	TEXT NOT NULL,
	PRIMARY KEY("ID" AUTOINCREMENT)
);
CREATE TABLE IF NOT EXISTS "Expediente" (
	"Id_exp"	INTEGER NOT NULL,
	"Nss_Pac"	INTEGER NOT NULL,
	"Nss_med"	TEXT NOT NULL,
	"Fecha_crea"	DATE NOT NULL,
	"Estatura"	REAL NOT NULL,
	"Peso"	REAL NOT NULL,
	"Tipo_Sangre"	TEXT,
	"Observaciones"	TEXT,
	FOREIGN KEY("Nss_Pac") REFERENCES "Paciente"("Nss"),
	PRIMARY KEY("Id_exp"),
	FOREIGN KEY("Nss_med") REFERENCES "Medico"("Nss")
);
INSERT INTO "Cita" ("Id_cita","Nss_Pac","Nss_med","Fecha","Hora") VALUES (1,99968220147,78945636925,'2021-06-24','14-00-00'),
 (2,57946813245,45678328776,'2021-06-25','18-00-00');
INSERT INTO "Paciente" ("Nombre","Apellido_P","Apellido_M","Curp","Nss","Telefono","Domicilio","fecha_nac") VALUES ('Gloria Andrea','Romo','Sandoval','ROSG991105MJCMNL01','95745524873',3321088173,'Aurelia Guevara 32','1999-11-05'),
 ('Jonathan David','Rojas','Piña','ROVI490617HSPTRE25','88965621421',3326540810,'2021 San Pablo','2000-12-01'),
 ('Victor Ricardo','Vera','Rojas','VERV000515HJCTJCA5','56849781326',3311524781,'Magnolia 490','2000-05-15'),
 ('David Guadalupe','Vargas','Lopez','VALD001212APRROLK0','59647126458',3358947763,'Ninos Heroes 560','2000-12-15'),
 ('Beatriz','Medina','Chicuate','MECB890611HJCDHT01','99968220147',3321900123,'Av. Experiencia 1200','1972-02-07'),
 ('Felipe','Juarez','Soto','JUSF870715HDYUT56','57946813245',3312468451,'Av Revolucion 805','1987-07-15');
INSERT INTO "Medico" ("Nombre","Apellido_P","Apellido_M","Sexo","Fecha_nac","Curp","Nss","Direccion","Especialidad","Fecha_ing") VALUES ('Abril','Gutierrez','Balcazar','F','1980-12-01','BAGA801201KJHMNB02','89895456565','Belisario Domínguez 219, interior A','Cardiologia','2020-12-01'),
 ('Franciso Javier','Chavolla','Gonzalez','M','1990-05-22','CHGF900522MJCMNL03','78945636925','Manuel Cambre 2010','Cardiologia','2021-01-01'),
 ('Teresa','Plascencia','Sanchez','F','1985-12-18','PLST851218LKJOKM45','10247859674','Circunvalacion 800','Alergologia','2020-08-08'),
 ('Ana Paola','Macias','Robles','F','1990-11-03','MARA901103UIOPOI03','96857412325','Tarascos 3432','Alergologia','2021-02-05'),
 ('Javier Arturo','Albarran','Arozarena','M','1970-07-24','ALAJ700724OIUYTRE10','96325112236','Tarascos 3235','Cirugia General','2021-03-03'),
 ('Bernardo Alejandro','Jones','Woog','M','1959-09-19','JOWB590919POIFTR02','20105596833','Avenida Lopez Mateos Nte 567','Cirugia General','2019-05-03'),
 ('Oscar ','Fernandez','Diaz','M','1989-09-01','FEDO890901GFDJKL03','87966541278','Guayaquil 2463','Cirugia Plastica','2019-05-03'),
 ('Rafael ','Velázquez','Razo','M','1987-10-04','VERR871004YTRIOP78','99856325458','Adolfo Lopez Mateos Norte 766','Cirugia Plastica','2020-01-31'),
 ('Paulina','Dominguez','Solis','F','1988-06-20','DOSP880620FGHHGF09','87965849321','Rio de Janeiro 2729','Cirugia Maxilofacial','2021-04-04'),
 ('Miguel Angel ','Moreno','Alba','M','1990-02-28','MOAM900228TREYUE01','45678328776','Avenida Americas 226','Cirugia Maxilofacial','2020-10-01');
INSERT INTO "Usuario" ("ID","Nombre_usuario","Contrasena","Rol","NSS_user") VALUES (0,'admin','123','admin','23021587991'),
 (1,'AbGB','12578','medico','89895456565'),
 (2,'JoRP','10974','paciente','88965621421'),
 (3,'GlRS','21492','paciente','95745524873'),
 (4,'ViVR','9157','paciente','56849781326'),
 (5,'DaVL','32594','paciente','59647126458'),
 (6,'FrCG','5368','medico','78945636925'),
 (7,'TePS','11269','medico','10247859674'),
 (8,'AnMR','28637','medico','96857412325'),
 (9,'JaAA','23754','medico','96325112236'),
 (10,'BeJW','12884','medico','20105596833'),
 (11,'OsFD','19386','medico','87966541278'),
 (12,'RaVR','9390','medico','99856325458'),
 (13,'PaDS','4299','medico','87965849321'),
 (14,'MiMA','29746','medico','45678328776'),
 (15,'BeMC','18255','paciente','99968220147'),
 (16,'FeJS','13111','paciente','57946813245');
INSERT INTO "Expediente" ("Id_exp","Nss_Pac","Nss_med","Fecha_crea","Estatura","Peso","Tipo_Sangre","Observaciones") VALUES (1,99968220147,'78945636925','2021-06-24',1.68,60.0,'A+','Cirugia del apendice, no presenta alergias.  2021-06-24 Se encontro que la paciente tiene problemas en sus arterias.'),
 (2,57946813245,'45678328776','2021-06-25',1.85,80.0,'AB+','No tiene alergias, 2021-06-25 dolencia de quijada');
COMMIT;
