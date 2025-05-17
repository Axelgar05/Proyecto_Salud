#include <iostream>
#include <string>
#include <memory>
#include <algorithm>

// MySQL Connector headers
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>

using namespace std;
using namespace sql;

string toLowerCase(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

bool esGeneroValido(const string& genero) {
    string generoLower = toLowerCase(genero);
    return (generoLower == "hombre" || generoLower == "mujer");
}

bool confirmarRegistro() {
    string contrasena;
    cout << "\nPara confirmar el registro, introduzca la contrase�a: ";
    getline(cin, contrasena);
    return contrasena == "1234";
}

void insertarPaciente() {
    try {
        // Conexi�n a la base de datos
        mysql::MySQL_Driver* driver = mysql::get_mysql_driver_instance();
        unique_ptr<Connection> con(driver->connect("tcp://127.0.0.1:3306", "tu_usuario", "tu_contrase�a"));
        con->setSchema("tu_base_de_datos");

        // Captura de datos
        string nombre, apellido, genero, departamento, municipio, trabajo;
        int edad;

        cout << "=== Registro de Paciente ===" << endl;
        cout << "Nombre: ";
        getline(cin, nombre);

        cout << "Apellido: ";
        getline(cin, apellido);

        cout << "Edad: ";
        cin >> edad;
        cin.ignore();

        do {
            cout << "G�nero (Hombre/Mujer): ";
            getline(cin, genero);
            if (!esGeneroValido(genero)) {
                cout << "G�nero inv�lido. Intente nuevamente." << endl;
            }
        } while (!esGeneroValido(genero));

        cout << "Departamento: ";
        getline(cin, departamento);

        cout << "Municipio: ";
        getline(cin, municipio);

        cout << "Trabajo: ";
        getline(cin, trabajo);

        // Confirmaci�n por contrase�a
        if (!confirmarRegistro()) {
            cout << "Contrase�a incorrecta. Registro cancelado." << endl;
            return;
        }

        // Insertar en la tabla paciente
        unique_ptr<PreparedStatement> pstmt(
            con->prepareStatement("INSERT INTO paciente (nombrePaciente, apellidoPaciente, edadPaciente, generoPaciente, departamentoPaciente, municipioPaciente, trabajoPaciente) VALUES (?, ?, ?, ?, ?, ?, ?)")
        );

        pstmt->setString(1, nombre);
        pstmt->setString(2, apellido);
        pstmt->setInt(3, edad);
        pstmt->setString(4, genero);
        pstmt->setString(5, departamento);
        pstmt->setString(6, municipio);
        pstmt->setString(7, trabajo);

        pstmt->execute();
        cout << "\nPaciente insertado exitosamente en la base de datos." << endl;

    }
    catch (SQLException& e) {
        cerr << "Error de base de datos: " << e.what() << endl;
    }
}

void calcularIMC() {
    float peso, altura;
    cout << "\n=== C�lculo de IMC ===" << endl;
    cout << "Ingrese peso (kg): ";
    cin >> peso;
    cout << "Ingrese altura (m): ";
    cin >> altura;

    if (altura <= 0) {
        cout << "Altura no v�lida." << endl;
        return;
    }

    float imc = peso / (altura * altura);
    cout << "El IMC es: " << imc << endl;

    // Clasificaci�n b�sica
    if (imc < 18.5)
        cout << "Clasificaci�n: Bajo peso" << endl;
    else if (imc < 24.9)
        cout << "Clasificaci�n: Normal" << endl;
    else if (imc < 29.9)
        cout << "Clasificaci�n: Sobrepeso" << endl;
    else
        cout << "Clasificaci�n: Obesidad" << endl;
}

int main() {
    insertarPaciente();

    int opcion;
    do {
        cout << "\n=== Men� ===" << endl;
        cout << "1. Calcular IMC" << endl;
        cout << "0. Salir" << endl;
        cout << "Seleccione una opci�n: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
        case 1:
            calcularIMC();
            break;
        case 0:
            cout << "Saliendo del programa..." << endl;
            break;
        default:
            cout << "Opci�n no v�lida." << endl;
        }
    } while (opcion != 0);

    return 0;
}
