#define NOMINMAX // Esto debe ir antes de windows.h
#include <iostream>
#include "ConexionBD.h"
#include <vector>
#include <string>

#include <cstdlib>
#include "Paciente.h"
#include <windows.h>
#include "Usuario.h" // Incluir el archivo que contiene la definición de Usuario
#include <hpdf.h>
#include <direct.h> // Para _mkdir si prefieres multiplataforma

using namespace std;


// Limpiar el buffer de entrada
void limpiarBuffer() {
    while (cin.get() != '\n' && !cin.eof());
}

// Añade esta función para limpiar la pantalla (Windows)
void limpiarPantalla() {
    system("cls");
}

// Añade esta función para pausar la consola
void pausarConsola() {
    cout << "\nPresione ENTER para continuar...";
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Estructura mejorada con comentarios descriptivos
struct Usuario {
    string nombre;          // Nombre completo (mínimo 2 palabras)
    int edad;               // Edad en años (rango 1-120)
    string genero;          // Género (masculino/femenino/otro)
    string departamento;    // Departamento de residencia
    string municipio;       // Municipio de residencia (debe coincidir con departamento)
    float peso;             // Peso en kilogramos (rango 20-300 kg)
    float altura;           // Altura en metros (rango 0.5-2.5 m)
    string nivel_actividad; // Sedentario/ligero/moderado/intenso
    string ocupacion;       // Ocupación principal
};

// Función mejorada con validaciones y especificación de unidades
void pedirDatosUsuario(Usuario& usuario) {
    do {
        cout << "Ingrese su nombre completo (ej. Juan Pérez): ";
        getline(cin, usuario.nombre);
        // Validar que contenga al menos un espacio
        if (usuario.nombre.find(' ') == string::npos) {
            cout << "Error: Debe ingresar al menos nombre y apellido.\n";
        }
    } while (usuario.nombre.find(' ') == string::npos);

    do {
        cout << "Ingrese su edad (en años, entre 1 y 120): ";
        cin >> usuario.edad;
        if (usuario.edad < 1 || usuario.edad > 120) {
            cout << "Error: La edad debe estar entre 1 y 120 años.\n";
            limpiarBuffer();
        }
    } while (usuario.edad < 1 || usuario.edad > 120);
    limpiarBuffer();

    do {
        cout << "Ingrese su género (masculino/femenino/otro): ";
        getline(cin, usuario.genero);
    } while (usuario.genero.empty());

    do {
        cout << "Ingrese su departamento de residencia: ";
        getline(cin, usuario.departamento);
    } while (usuario.departamento.empty());

    do {
        cout << "Ingrese su municipio de residencia: ";
        getline(cin, usuario.municipio);
    } while (usuario.municipio.empty());

    do {
        cout << "Ingrese su peso en kilogramos (ej. 70.5, rango 20-300 kg): ";
        cin >> usuario.peso;
        if (usuario.peso < 20 || usuario.peso > 300) {
            cout << "Error: El peso debe estar entre 20 y 300 kg.\n";
            limpiarBuffer();
        }
    } while (usuario.peso < 20 || usuario.peso > 300);

    do {
        cout << "Ingrese su altura en metros (ej. 1.75, rango 0.5-2.5 m): ";
        cin >> usuario.altura;
        if (usuario.altura < 0.5 || usuario.altura > 2.5) {
            cout << "Error: La altura debe estar entre 0.5 y 2.5 metros.\n";
            limpiarBuffer();
        }
    } while (usuario.altura < 0.5 || usuario.altura > 2.5);
    limpiarBuffer();

    do {
        cout << "Ingrese su nivel de actividad física:\n"
            << "  - sedentario (poco o ningún ejercicio)\n"
            << "  - ligero (ejercicio 1-3 días/semana)\n"
            << "  - moderado (ejercicio 3-5 días/semana)\n"
            << "  - intenso (ejercicio 6-7 días/semana)\n"
            << "Seleccione: ";
        getline(cin, usuario.nivel_actividad);
    } while (usuario.nivel_actividad != "sedentario" &&
        usuario.nivel_actividad != "ligero" &&
        usuario.nivel_actividad != "moderado" &&
        usuario.nivel_actividad != "intenso");

    do {
        cout << "Ingrese su ocupación principal: ";
        getline(cin, usuario.ocupacion);
    } while (usuario.ocupacion.empty());
}

// [Las funciones calcularIMC, calcularTMB, obtenerFactorActividad y calcularCaloriasDiarias 
// permanecen igual que en el código original, pero con comentarios mejorados]

// Función para calcular el Índice de Masa Corporal (IMC)
float calcularIMC(const Usuario& usuario) {
    if (usuario.altura > 0) {
        return usuario.peso / (usuario.altura * usuario.altura);
    }
    else {
        cout << "Error: La altura debe ser mayor que 0 para calcular el IMC.\n";
        return 0.0f; // Valor por defecto en caso de error
    }
}

// Función para calcular el peso ideal (fórmula de Broca modificada)
float calcularPesoIdeal(const Usuario& usuario) {
    // Para hombres: altura en cm - 100 - ((altura en cm - 150) / 4)
    // Para mujeres: altura en cm - 100 - ((altura en cm - 150) / 2.5)
    float altura_cm = usuario.altura * 100;
    if (usuario.genero == "masculino")
        return altura_cm - 100 - ((altura_cm - 150) / 4.0f);
    else
        return altura_cm - 100 - ((altura_cm - 150) / 2.5f);
}

// Función para clasificar el IMC
string clasificarIMC(float imc) {
    if (imc < 18.5f) return "Desnutrición";
    else if (imc < 25.0f) return "Normal";
    else if (imc < 30.0f) return "Sobrepeso";
    else if (imc < 35.0f) return "Obesidad I";
    else if (imc < 40.0f) return "Obesidad II";
    else return "Obesidad III (mórbida)";
}

// Función para calcular las calorías diarias requeridas
float calcularCaloriasDiarias(const Usuario& usuario) {
    // Calcular el TMB (Tasa Metabólica Basal) usando la fórmula de Harris-Benedict
    float tmb = 0.0f;
    if (usuario.genero == "masculino") {
        tmb = 10 * usuario.peso + 6.25 * (usuario.altura * 100) - 5 * usuario.edad + 5;
    } else if (usuario.genero == "femenino") {
        tmb = 10 * usuario.peso + 6.25 * (usuario.altura * 100) - 5 * usuario.edad - 161;
    } else {
        cout << "Advertencia: Género no reconocido. Usando valores por defecto.\n";
        tmb = 10 * usuario.peso + 6.25 * (usuario.altura * 100) - 5 * usuario.edad;
    }

    // Obtener el factor de actividad física
    float factorActividad = 1.2f; // Por defecto: sedentario
    if (usuario.nivel_actividad == "ligero") {
        factorActividad = 1.375f;
    } else if (usuario.nivel_actividad == "moderado") {
        factorActividad = 1.55f;
    } else if (usuario.nivel_actividad == "intenso") {
        factorActividad = 1.725f;
    }

    // Calorías diarias = TMB * factor de actividad
    return tmb * factorActividad;
}

// Función para mostrar información del paciente en la base de datos
void manejarBaseDatos(ConexionBD& conexion) {
    int opcionBD = 0; // Inicializar la variable
    do {
        cout << "\n----- Gestión de Base de Datos -----\n";
        cout << "1. Mostrar pacientes\n";
        cout << "2. Crear nuevo paciente\n";
        cout << "3. Buscar paciente existente\n";
        cout << "4. Actualizar información de paciente\n";
        cout << "5. Volver al menú principal\n";
        cout << "Seleccione una opción: ";
        cin >> opcionBD;
        limpiarBuffer();

        switch (opcionBD) { // Asegurarse de que el case esté dentro de un switch
        case 1: {
            // Mostrar tabla
            Paciente p = Paciente(); // Asegúrate de inicializar el objeto
            p.leer(); // Llama al método leer de la clase Paciente para mostrar los datos
            break;
        }
        case 2: {
            // Crear nuevo paciente
            string nombrePaciente, apellidoPaciente, generoPaciente, departamentoPaciente, municipioPaciente, trabajoPaciente;
            int edad = 0;
            float altura, peso;

            cout << "Ingrese el nombre del paciente: " << endl;
            getline(cin, nombrePaciente);
            cout << "Ingrese el apellido del paciente: " << endl;
            getline(cin, apellidoPaciente);
            cout << "Ingrese la edad del paciente: " << endl;
            cin >> edad;
            limpiarBuffer();
            cout << "Ingrese el género del paciente (masculino/femenino): " << endl;
            getline(cin, generoPaciente);
            cout << "Ingrese el departamento del paciente: " << endl;
            getline(cin, departamentoPaciente);
            cout << "Ingrese el municipio del paciente: " << endl;
            getline(cin, municipioPaciente);
            cout << "Ingrese el trabajo del paciente: " << endl;
            getline(cin, trabajoPaciente);
            cout << "Ingrese la altura del paciente (en metros): " << endl;
            cin >> altura;
            cout << "Ingrese el peso del paciente (en kilogramos): " << endl;
            cin >> peso;

            Paciente p = Paciente(peso, altura, nombrePaciente, apellidoPaciente, edad, generoPaciente, departamentoPaciente, municipioPaciente, trabajoPaciente, "", 0);

            p.crear();
            p.leer();
            break;
        }
        case 3: {
            // Buscar paciente existente
            cout << "Función para buscar un paciente aún no implementada.\n";
            break;
        }
        case 4: {
            // Actualizar información de paciente
            int idActualizar;
            string nombrePaciente, apellidoPaciente, generoPaciente, departamentoPaciente, municipioPaciente, trabajoPaciente;
            int edad = 0;
            float altura, peso;

            cout << "Ingrese el ID del paciente a actualizar: ";
            cin >> idActualizar;
            limpiarBuffer();

            cout << "Ingrese el nuevo nombre del paciente: ";
            getline(cin, nombrePaciente);
            cout << "Ingrese el nuevo apellido del paciente: ";
            getline(cin, apellidoPaciente);
            cout << "Ingrese la nueva edad del paciente: ";
            cin >> edad;
            limpiarBuffer();
            cout << "Ingrese el nuevo género del paciente: ";
            getline(cin, generoPaciente);
            cout << "Ingrese el nuevo departamento del paciente: ";
            getline(cin, departamentoPaciente);
            cout << "Ingrese el nuevo municipio del paciente: ";
            getline(cin, municipioPaciente);
            cout << "Ingrese el nuevo trabajo del paciente: ";
            getline(cin, trabajoPaciente);
            cout << "Ingrese la nueva altura del paciente (en metros): ";
            cin >> altura;
            cout << "Ingrese la nueva peso del paciente (en kilogramos): ";
            cin >> peso;
            limpiarBuffer();

            Paciente p = Paciente(peso, altura, nombrePaciente, apellidoPaciente, edad, generoPaciente, departamentoPaciente, municipioPaciente, trabajoPaciente, "", idActualizar);
            p.actualizar();
            break;
        }
        case 5:
            // Volver al menú principal
            cout << "Volviendo al menú principal...\n";
            break;
        default:
            // Opción no válida
            cout << "Opción no válida. Intente nuevamente.\n";
        }
    } while (opcionBD != 5);
}

// Prototipos de las funciones que faltan en el codigo
float calcularTMB(const Usuario& usuario);
float obtenerFactorActividad(const Usuario& usuario);
void generarPDFCalculosSalud(const Usuario& usuario, float imc, float tmb, float factor, float calorias);

// Submenú para cálculos de salud SOLO con datos de la base de datos
void menuCalculosSalud(const Usuario&) {
    int idPaciente;
    cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    cout << "║      CÁLCULOS DE SALUD DEL USUARIO (BASE DE DATOS)        ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n";
    cout << "Ingrese el ID del paciente para realizar los cálculos: ";
    cin >> idPaciente;
    limpiarBuffer();

    Paciente p;
    if (!p.cargarPorId(idPaciente)) {
        cout << "\n╔══════════════════════════════╗\n";
        cout << "║  No se encontró un paciente  ║\n";
        cout << "║        con ese ID.           ║\n";
        cout << "╚══════════════════════════════╝\n";
        return;
    }

    // Mostrar datos del paciente con marco
    cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    cout << "║                  DATOS DEL PACIENTE                        ║\n";
    cout << "╠════════════════════════════════════════════════════════════╣\n";
    cout << "║ Nombre:       " << p.getnombre() << " " << p.getapellido() << endl;
    cout << "║ Edad:         " << p.getedad() << endl;
    cout << "║ Género:       " << p.getgenero() << endl;
    cout << "║ Departamento: " << p.getdepartamento() << endl;
    cout << "║ Municipio:    " << p.getmunicipio() << endl;
    cout << "║ Trabajo:      " << p.gettrabajo() << endl;
    cout << "║ Altura:       " << p.getaltura() << " m" << endl;
    cout << "║ Peso:         " << p.getpeso() << " kg" << endl;
    cout << "╚════════════════════════════════════════════════════════════╝\n";

    // Convertir a struct Usuario para usar las funciones existentes
    Usuario usuarioBD;
    usuarioBD.nombre = p.getnombre() + " " + p.getapellido();
    usuarioBD.edad = p.getedad();
    usuarioBD.genero = p.getgenero();
    usuarioBD.departamento = p.getdepartamento();
    usuarioBD.municipio = p.getmunicipio();
    usuarioBD.peso = p.getpeso();
    usuarioBD.altura = p.getaltura();
    usuarioBD.nivel_actividad = "sedentario"; // Puedes ajustar si tienes este dato
    usuarioBD.ocupacion = p.gettrabajo();

    // Realizar cálculos
    float imc = calcularIMC(usuarioBD);
    float tmb = calcularTMB(usuarioBD);
    float factor = obtenerFactorActividad(usuarioBD);
    float calorias = calcularCaloriasDiarias(usuarioBD);

    cout << "\n--- Resultados de Cálculos de Salud ---\n";
    cout << "IMC: " << imc << " (" << clasificarIMC(imc) << ")\n";
    cout << "TMB: " << tmb << " kcal/día\n";
    cout << "Factor de actividad: " << factor << endl;
    cout << "Calorías diarias requeridas: " << calorias << " kcal\n";

    // Generar PDF con los cálculos
    generarPDFCalculosSalud(usuarioBD, imc, tmb, factor, calorias);
}

// Ejemplo de implementación de las funciones faltantes
float calcularTMB(const Usuario& usuario) {
    if (usuario.genero == "masculino")
        return 10 * usuario.peso + 6.25 * (usuario.altura * 100) - 5 * usuario.edad + 5;
    else if (usuario.genero == "femenino")
        return 10 * usuario.peso + 6.25 * (usuario.altura * 100) - 5 * usuario.edad - 161;
    else
        return 10 * usuario.peso + 6.25 * (usuario.altura * 100) - 5 * usuario.edad;
}

float obtenerFactorActividad(const Usuario& usuario) {
    if (usuario.nivel_actividad == "sedentario") return 1.2f;
    if (usuario.nivel_actividad == "ligero") return 1.375f;
    if (usuario.nivel_actividad == "moderado") return 1.55f;
    if (usuario.nivel_actividad == "intenso") return 1.725f;
    return 1.2f;
}

void generarPDFCalculosSalud(const Usuario& usuario, float imc, float tmb, float factor, float calorias) {
    // Crear carpeta "reportes" si no existe
    CreateDirectoryA("reportes", NULL);

    HPDF_Doc pdf = HPDF_New(nullptr, nullptr);
    if (!pdf) {
        std::cerr << "No se pudo crear el documento PDF.\n";
        return;
    }

    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

    // Insertar el logo de la universidad (debe estar en la carpeta del ejecutable)
    HPDF_Image logo = HPDF_LoadPngImageFromFile(pdf, "logo_umg.png");
    if (logo) {
        // Dibuja el logo en la parte superior izquierda (ajusta tamaño/posición si lo deseas)
        HPDF_Page_DrawImage(page, logo, 50, 810, 80, 80);
    }

    // Título centrado y grande
    HPDF_Page_BeginText(page);
    HPDF_Page_SetFontAndSize(page, HPDF_GetFont(pdf, "Helvetica-Bold", nullptr), 18);
    HPDF_Page_TextOut(page, 180, 800, "Reporte de Salud Personal");
    HPDF_Page_EndText(page);

    // Fecha y hora
    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now);
    char fecha[64];
    strftime(fecha, sizeof(fecha), "Fecha de generación: %d/%m/%Y %H:%M", &ltm);

    // Crear nombre de archivo único con fecha y hora
    char nombreArchivo[256];
    strftime(nombreArchivo, sizeof(nombreArchivo), "reportes\\reporte_salud_%Y%m%d_%H%M%S.pdf", &ltm);

    HPDF_Page_BeginText(page);
    HPDF_Page_SetFontAndSize(page, HPDF_GetFont(pdf, "Helvetica", nullptr), 10);
    HPDF_Page_TextOut(page, 50, 780, fecha);
    HPDF_Page_EndText(page);

    // Línea separadora
    HPDF_Page_MoveTo(page, 50, 770);
    HPDF_Page_LineTo(page, 550, 770);
    HPDF_Page_Stroke(page);

    // Datos personales
    float y = 750;
    float lineHeight = 18;
    HPDF_Page_BeginText(page);
    HPDF_Page_SetFontAndSize(page, HPDF_GetFont(pdf, "Helvetica-Bold", nullptr), 12);
    HPDF_Page_TextOut(page, 50, y, "Datos personales:");
    HPDF_Page_SetFontAndSize(page, HPDF_GetFont(pdf, "Helvetica", nullptr), 12);
    y -= lineHeight;
    HPDF_Page_TextOut(page, 60, y, ("Nombre: " + usuario.nombre).c_str());
    y -= lineHeight;
    HPDF_Page_TextOut(page, 60, y, ("Edad: " + std::to_string(usuario.edad)).c_str());
    y -= lineHeight;
    HPDF_Page_TextOut(page, 60, y, ("Género: " + usuario.genero).c_str());
    y -= lineHeight;
    HPDF_Page_TextOut(page, 60, y, ("Departamento: " + usuario.departamento).c_str());
    y -= lineHeight;
    HPDF_Page_TextOut(page, 60, y, ("Municipio: " + usuario.municipio).c_str());
    y -= lineHeight;
    HPDF_Page_TextOut(page, 60, y, ("Ocupación: " + usuario.ocupacion).c_str());
    y -= lineHeight;
    HPDF_Page_TextOut(page, 60, y, ("Peso: " + std::to_string(usuario.peso) + " kg").c_str());
    y -= lineHeight;
    HPDF_Page_TextOut(page, 60, y, ("Altura: " + std::to_string(usuario.altura) + " m").c_str());
    y -= lineHeight;
    HPDF_Page_TextOut(page, 60, y, ("Nivel de actividad: " + usuario.nivel_actividad).c_str());
    HPDF_Page_EndText(page);

    // Línea separadora
    y -= 10;
    HPDF_Page_MoveTo(page, 50, y);
    HPDF_Page_LineTo(page, 550, y);
    HPDF_Page_Stroke(page);

    // Resultados
    y -= 25;
    HPDF_Page_BeginText(page);
    HPDF_Page_SetFontAndSize(page, HPDF_GetFont(pdf, "Helvetica-Bold", nullptr), 12);
    HPDF_Page_TextOut(page, 50, y, "Resultados:");
    HPDF_Page_SetFontAndSize(page, HPDF_GetFont(pdf, "Helvetica", nullptr), 12);
    y -= lineHeight;
    HPDF_Page_TextOut(page, 60, y, ("IMC: " + std::to_string(imc)).c_str());
    y -= lineHeight;
    std::string clasificacion = "Clasificación IMC: " + clasificarIMC(imc);
    HPDF_Page_TextOut(page, 60, y, clasificacion.c_str());
    y -= lineHeight;
    float pesoIdeal = calcularPesoIdeal(usuario);
    HPDF_Page_TextOut(page, 60, y, ("Peso ideal: " + std::to_string(pesoIdeal) + " kg").c_str());
    y -= lineHeight;
    HPDF_Page_TextOut(page, 60, y, ("TMB: " + std::to_string(tmb) + " kcal/día").c_str());
    y -= lineHeight;
    HPDF_Page_TextOut(page, 60, y, ("Factor de actividad: " + std::to_string(factor)).c_str());
    y -= lineHeight;
    HPDF_Page_TextOut(page, 60, y, ("Calorías diarias requeridas: " + std::to_string(calorias) + " kcal").c_str());
    y -= lineHeight;
    HPDF_Page_EndText(page);

    // Datos de la universidad
    y -= 30;
    HPDF_Page_BeginText(page);
    HPDF_Page_SetFontAndSize(page, HPDF_GetFont(pdf, "Helvetica-Bold", nullptr), 11);
    HPDF_Page_TextOut(page, 50, y, "Universidad Mariano Gálvez de Guatemala");
    y -= 16;
    HPDF_Page_SetFontAndSize(page, HPDF_GetFont(pdf, "Helvetica", nullptr), 11);
    HPDF_Page_TextOut(page, 50, y, "Facultad de Ingeniería de Sistemas");
    y -= 16;
    HPDF_Page_TextOut(page, 50, y, "Curso: Programación 1");
    HPDF_Page_EndText(page);

    // Recomendaciones básicas
    y -= 30;
    HPDF_Page_BeginText(page);
    HPDF_Page_SetFontAndSize(page, HPDF_GetFont(pdf, "Helvetica-Bold", nullptr), 12);
    HPDF_Page_TextOut(page, 50, y, "Recomendaciones:");
    HPDF_Page_SetFontAndSize(page, HPDF_GetFont(pdf, "Helvetica", nullptr), 12);
    y -= lineHeight;
    std::string recomendacion;
    if (imc < 18.5f)
        recomendacion = "Aumente su ingesta calórica y consulte a un nutricionista.";
    else if (imc < 25.0f)
        recomendacion = "¡Felicidades! Mantenga su estilo de vida saludable.";
    else if (imc < 30.0f)
        recomendacion = "Reduzca el consumo de calorías y aumente la actividad física.";
    else
        recomendacion = "Consulte a un profesional de la salud para un plan personalizado.";
    HPDF_Page_TextOut(page, 60, y, recomendacion.c_str());
    HPDF_Page_EndText(page);

    // Pie de página
    HPDF_Page_BeginText(page);
    HPDF_Page_SetFontAndSize(page, HPDF_GetFont(pdf, "Helvetica-Oblique", nullptr), 9);
    HPDF_Page_TextOut(page, 50, 40, "Reporte generado por Proyecto Salud - Universidad Mariano Galvez");
    HPDF_Page_EndText(page);

    HPDF_SaveToFile(pdf, nombreArchivo);
    HPDF_Free(pdf);

    std::cout << "PDF generado: " << nombreArchivo << "\n";
}

Usuario usuario; // Ahora el compilador reconocerá 'Usuario'
int opcion;

//--------------------------------------------INT MAIN PROYECTO SALUD--------------------------------------------//
int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::ios_base::sync_with_stdio(false);

    ConexionBD cn = ConexionBD();
    cn.abrir_conexion();

    if (cn.getConector()) {
        cout << "✓ Conexión exitosa con la base de datos\n";
    }
    else {
        cout << "✗ Error en la conexión con la base de datos\n";
        // En un caso real, podrías querer terminar el programa aquí
    }

    do {
        cout << "\n════════ Menú Principal ════════\n";
        cout << "1. Gestión de Base de Datos\n";
        cout << "2. Cálculos de Salud del Usuario\n";
        cout << "3. Salir del programa\n";
        cout << "Seleccione una opción (1-3): ";

        while (!(cin >> opcion) || opcion < 1 || opcion > 3) {
            cout << "Error: Ingrese un número entre 1 y 3: ";
            limpiarBuffer();
        }
        limpiarBuffer();

        switch (opcion) {
        case 1:
            manejarBaseDatos(cn);
            break;
        case 2:
            menuCalculosSalud(usuario);
            break;
        case 3:
            cout << "Saliendo del programa...\n";
            break;
        }
    } while (opcion != 3);

    cn.cerrar_conexion();
    return 0;
}
//--------------------------------------------FIN MAIN PROYECTO SALUD--------------------------------------------//

