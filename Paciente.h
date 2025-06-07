#pragma once
#define NOMINMAX // Prevents conflicts with the max macro in <windows.h>
#include "usuario.h"
#include <iostream>
#include <mysql.h>
#include "ConexionBD.h"
#include <string>
#include <limits> // Asegúrate de incluir este encabezado para std::numeric_limits

using namespace std;
class Paciente :public usuario{
    //atributos
private: string codigo;
       int idPaciente = 0;
    //constructor
public:
      Paciente() {}
        Paciente(float peso,float alt,string nom, string ape, int edad, string gen, string depto, string mun, string trab, string cod, int id_p) : usuario(nom,ape,edad,gen,depto,mun,trab,alt,peso){
            codigo = cod;
            idPaciente = id_p;
        }

        //metodos get y set
        //set (modificar un atributo)
        void setidPaciente(int p) { idPaciente = p; }
        void setcodigo(string c) { codigo = c; }
        void setnombre(string n) { nombrePaciente = n; }
        void setapellido(string a) { apellidoPaciente = a; }
        void setedad(int e) { edadPaciente = e; }
        void setgenero(string g) { generoPaciente = g; }
        void setmunicipio(string m) { municipioPaciente = m; }
        void setdepartamento(string d) { departamentoPaciente = d; }
        void settrabajo(string t) { trabajoPaciente = t; }
        void setpeso(float p) { pesoPaciente = p; }
        void setalt(float al) { alturaPaciente = al; }

        //get (mostrar el contenido del atributo)
        int getidPaciente() { return idPaciente; }
        string getcodigo() { return codigo; }
        string getnombre() { return nombrePaciente; }
        string getapellido() { return apellidoPaciente; }
        int getedad() { return edadPaciente; }
        string getgenero() { return generoPaciente; }
        string getmunicipio() { return municipioPaciente; }
        string getdepartamento() { return departamentoPaciente; }
        string gettrabajo() { return trabajoPaciente; }
        float getaltura() { return alturaPaciente; }
        float getpeso() { return pesoPaciente; }
        

    //metodos
        void crear() {
            int q_estado = 0;
            ConexionBD cn = ConexionBD();
            cn.abrir_conexion();

            // 1. Obtener el máximo idPaciente actual
            int nuevoId = 1;
            if (cn.getConector()) {
                string consultaMax = "SELECT IFNULL(MAX(idPaciente),0) + 1 FROM paciente";
                MYSQL_ROW fila;
                MYSQL_RES* resultado;
                q_estado = mysql_query(cn.getConector(), consultaMax.c_str());
                if (!q_estado) {
                    resultado = mysql_store_result(cn.getConector());
                    if ((fila = mysql_fetch_row(resultado))) {
                        nuevoId = atoi(fila[0]);
                    }
                    mysql_free_result(resultado);
                }
            }

            // 2. Insertar el nuevo paciente con el id generado
            string edad = to_string(edadPaciente);
            string altura = to_string(alturaPaciente);
            string peso = to_string(pesoPaciente);
            string id = to_string(nuevoId);
            string consulta = "INSERT INTO paciente(idPaciente, nombrePaciente, apellidoPaciente, edadPaciente, generoPaciente, departamentoPaciente, municipioPaciente, trabajoPaciente, alturaPaciente, pesoPaciente) VALUES ('"
                + id + "','" + nombrePaciente + "','" + apellidoPaciente + "','" + edad + "','" + generoPaciente + "','" + departamentoPaciente + "','" + municipioPaciente + "','" + trabajoPaciente + "','" + altura + "','" + peso + "')";
            const char* c = consulta.c_str();
            q_estado = mysql_query(cn.getConector(), c);
            if (!q_estado) {
                cout << "✓ Registro Exitoso ✓" << endl;
            }
            else {
                cout << "xxx Registro Fallido xxx" << endl;
                cout << mysql_error(cn.getConector()) << endl;
            }
            cn.cerrar_conexion();
        }

        void leer() {
            int q_estado = 0;
            ConexionBD cn = ConexionBD();
            MYSQL_ROW fila;
            MYSQL_RES* resultado;
            cn.abrir_conexion();
            if (cn.getConector()) {
                cout << "_____________ Datos de los Pacientes _____________" << endl;
                cout << "ID Nombre Apellido Edad Genero Departamento Municipio Trabajo Altura Peso" << endl;
                string consulta = "select * from paciente;";
                const char* c = consulta.c_str();
                q_estado = mysql_query(cn.getConector(), c);
                if (!q_estado) {
                    resultado = mysql_store_result(cn.getConector());
                    while (fila = mysql_fetch_row(resultado)) {
                        cout << fila[0] << "," << fila[1] << "," << fila[2] << "," << fila[3] << "," << fila[4] << "," << fila[5] << "," << fila[6] << "," << fila[7] << "," << fila[8] << "," << fila[9] << endl;
                    }
                }
                else {
                    cout << " xxx Consulta Fallida xxx" << endl;
                }
            }
            else {
                cout << "xxx Fallo de la conexion xxx" << endl;
            }
            cn.cerrar_conexion();
        }
        void actualizar() {
            int q_estado = 0;
            ConexionBD cn = ConexionBD();
            cn.abrir_conexion();
            if (cn.getConector()) {
                // Convertir idPaciente a string
                string id = to_string(idPaciente);
                string edad = to_string(edadPaciente);
                string altura = to_string(alturaPaciente);
                string peso = to_string(pesoPaciente);

                // Construir la consulta SQL correctamente
                string consulta = "UPDATE paciente SET nombrePaciente = '" + nombrePaciente +
                                  "', apellidoPaciente = '" + apellidoPaciente +
                                  "', edadPaciente = '" + edad +
                                  "', generoPaciente = '" + generoPaciente +
                                  "', departamentoPaciente = '" + departamentoPaciente +
                                  "', municipioPaciente = '" + municipioPaciente +
                                  "', trabajoPaciente = '" + trabajoPaciente +
                                  "', alturaPaciente = '" + altura +
                                  "', pesoPaciente = '" + peso +
                                  "' WHERE idPaciente = '" + id + "'";

                const char* c = consulta.c_str();
                q_estado = mysql_query(cn.getConector(), c);
                if (!q_estado) {
                    cout << "✓ Registro Actualizado Exitosamente ✓" << endl;
                } else {
                    cout << "xxx Actualización Fallida xxx" << endl;
                    cout << mysql_error(cn.getConector()) << endl;
                }
            } else {
                cout << "xxx Fallo de la conexión xxx" << endl;
            }
            cn.cerrar_conexion();
        }
        void borrar(){}

        // Método para cargar los datos del paciente desde la base de datos por ID
        bool cargarPorId(int id) {
            int q_estado = 0;
            ConexionBD cn = ConexionBD();
            MYSQL_ROW fila;
            MYSQL_RES* resultado;
            cn.abrir_conexion();
            if (cn.getConector()) {
                string consulta = "SELECT nombrePaciente, apellidoPaciente, edadPaciente, generoPaciente, departamentoPaciente, municipioPaciente, trabajoPaciente, alturaPaciente, pesoPaciente FROM paciente WHERE idPaciente = " + to_string(id) + ";";
                const char* c = consulta.c_str();
                q_estado = mysql_query(cn.getConector(), c);
                if (!q_estado) {
                    resultado = mysql_store_result(cn.getConector());
                    if ((fila = mysql_fetch_row(resultado))) {
                        nombrePaciente = fila[0];
                        apellidoPaciente = fila[1];
                        edadPaciente = atoi(fila[2]);
                        generoPaciente = fila[3];
                        departamentoPaciente = fila[4];
                        municipioPaciente = fila[5];
                        trabajoPaciente = fila[6];
                        alturaPaciente = atof(fila[7]);
                        pesoPaciente = atof(fila[8]);
                        idPaciente = id;
                        mysql_free_result(resultado);
                        cn.cerrar_conexion();
                        return true;
                    }
                    mysql_free_result(resultado);
                }
            }
            cn.cerrar_conexion();
            return false;
        }
}
;
