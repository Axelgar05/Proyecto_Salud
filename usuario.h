#pragma once
#include <iostream>
using namespace std;
class usuario{
	//atributos
protected: string nombrePaciente, apellidoPaciente, generoPaciente, departamentoPaciente, municipioPaciente, trabajoPaciente;
		 float alturaPaciente, pesoPaciente;
		 int edadPaciente = 0;
	//constructor
	usuario(){}
	usuario(string nom, string ape, int edad, string gen, string depto, string mun, string trab,float alt,float peso) {
		alturaPaciente = alt;
		pesoPaciente = peso;
		nombrePaciente = nom;
		apellidoPaciente = ape;
		edadPaciente = edad;
		generoPaciente = gen;
		departamentoPaciente = depto;
		municipioPaciente = mun;
		trabajoPaciente = trab;
		
	}
};

