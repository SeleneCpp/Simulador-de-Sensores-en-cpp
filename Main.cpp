//============================================================================
// Name        : SensoresMenu.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Sensor.h"


using namespace std;
void mostrarMenu() {
    cout << "\n======= MENU =======\n";
    cout << "1. Cargar sensor\n";
    cout << "2. Cargar medicion\n";
    cout << "3. Mostrar Todo\n";
    cout << "4. Calcular estadisticas por tipo de sensor\n";
    cout << "5. Guardar reporte en archivo .txt\n";
    cout << "6. Mostrar mediciones fuera de rango\n";
    cout << "7. Consultar sensores por zona\n";
// NUEVO
    cout << "0. Salir\n";
    cout << "Seleccione una opcion: ";}

int main() {

	 auto start = std::chrono::high_resolution_clock::now();

	 SensorManager manager;
	    int opcion;
	    bool salir = false;

	    do {
	        mostrarMenu();
	        cin >> opcion;

	        switch (opcion) {
	            case 1:
	                manager.cargarSensor();
	                break;
	            case 2:
	             manager.cargarMedicion();
	                break;
            case 3:
            	manager.mostrarTodo();

            break;
            case 4: {
                char tipo;
                cout << "Ingrese el tipo de sensor (por ejemplo, T para temperatura): ";
                cin >> tipo;
                manager.estadisticasPorTipo(tipo);
                break;
            }
            case 5: {
                ofstream archivo("reporte_sensores.txt");
                if (archivo.is_open()) {
                    manager.mostrarTodo(archivo);
                    archivo.close();
                    cout << "Se guardo el informe en reporte_sensores.txt\n";
                } else {
                    cout << "No se pudo abrir el archivo para escritura.\n";
                }
                break;
            }
            case 6:  // NUEVO
                       manager.mostrarErrores();
                       break;

            case 7: {
                char zona;
                cout << "Ingrese la zona a consultar (A, B, C...): ";
                cin >> zona;
                manager.consultarPorZona(zona);
                break;
            }


	            case 0:{
	                auto end = std::chrono::high_resolution_clock::now();
	                auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
	               cout << "Tiempo transcurrido: " << duration.count() << " segundos" <<endl;
	                salir = true;
	                break;}
	            default:
	                cout << "Opcion invalida.\n";
	        }
	    } while (!salir);

	    return 0;
}
