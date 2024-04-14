#include <mysql.h>   // Libreria para la manipulacion de MySQL
#include <iostream>  // Libreria para la entrada y salida estandard

using namespace std;

// Declaramos la clase Conexionbasededatos
class Conexionbasededatos {
private: // Sección privada de la clase
	
    MYSQL* conexion; // Puntero que apunta a la estructura de tipo MYSQL para almacenar la conexión

public: // Sección pública de la clase

// Constructor de la clase. 
    Conexionbasededatos() {
        conexion = mysql_init(0); // Inicializa la conexión MySQL
        // Verifica si la inicialización fue exitosa
        if (conexion) {
        	 // Si la conexión se inicializa correctamente, imprime un mensaje de éxito
            cout << "La conexion se inicializo correctamente" << endl;
        } else {
        	 // Si la conexión no se inicializa correctamente, imprime un mensaje de error
            cerr << "Error al inicializar la conexion" << endl;
        }
    }
    
// Destructor de la clase. Libera la memoria ocupada por la conexión.
    ~Conexionbasededatos() {
        mysql_close(conexion); // Cerrar la conexión
        // Imprime un mensaje indicando que la conexión se cerro correctamente
        cout << "Conexion cerrada correctamente" << endl;
    }

// Función para conectar a la base de datos MySQL
// La función conectar devuelve un valor de tipo bool porque se utiliza para indicar si la conexión a la base de datos fue exitosa o no. 
bool conectar(string localhost, string usuario, string contrasena, string nombreBD, int puerto) { // Los parámetros de la funcion conectar se pasan como string porque así es como espera la función mysql_real_connect de la biblioteca MySQL.
    
	// Intenta establecer una conexión a la base de datos utilizando los parámetros proporcionados a la funcion conectar
	// La función c_str() se utiliza para convertir los objetos string en punteros de estilo C (arreglos de caracteres terminados en nulo) que son requeridos por la función mysql_real_connect.
    if (mysql_real_connect(conexion, localhost.c_str(), usuario.c_str(), contrasena.c_str(), nombreBD.c_str(), puerto, nullptr, 0)) { 
        // Si la conexión se establece correctamente, imprime un mensaje de éxito
        cout << "Conexion exitosa a la base de datos " << nombreBD << " en el puerto " << puerto << endl;
        // Devuelve true para indicar que la conexión fue exitosa
        return true;
    } else {
        // Si ocurre un error al conectar, imprime un mensaje de error junto con la descripción del error
        cerr << "Error al conectar a la base de datos: " << mysql_error(conexion) << endl;
        // Devuelve false para indicar que la conexión falló
        return false;
    }
}


    // Función para ejecutar una consulta SQL
    // La función ejecutarConsulta devuelve un valor de tipo bool porque se utiliza para indicar si la consulta a la base de datos fue exitosa o no.
    bool ejecutarConsulta(string consulta) {
        // Se intenta ejecutar la consulta utilizando la función mysql_query(),
        // convirtiendo la cadena de consulta de tipo string a una cadena de caracteres estilo C con c_str()
        if (mysql_query(conexion, consulta.c_str()) == 0) {
        	// Si la consulta se ejecuta con éxito (devuelve 0), imprime un mensaje indicando que se ejecutó correctamente
            cout << "Consulta ejecutada correctamente" << endl;
            // Devuelve true para indicar que la consulta se ejecutó con éxito
            return true;
        } else {
        	// Si la consulta falla, imprime un mensaje de error junto con la descripción del error utilizando mysql_error()
            cerr << "Error al ejecutar consulta: " << mysql_error(conexion) << endl;
            // Devuelve false para indicar que la consulta falló
            return false;
        }
    }

    // Función para obtener el resultado de una consulta SQL
    MYSQL_RES* obtenerResultado() {
        return mysql_store_result(conexion);
    }

    // Función para liberar la memoria ocupada por un resultado
    void liberarResultado(MYSQL_RES* resultado) {
        mysql_free_result(resultado);
    }
};




void mostrarAnimal(Conexionbasededatos &conexionbasededatos) {
    // Consulta SQL para mostrar animal y sus detalles
    string consulta = "SELECT a.ID, a.Nombre_comun, a.Estatura, a.Dieta, a.Descripcion, d.Edad, d.Sexo, d.Peso "
                      "FROM Animales a "
                      "JOIN Detalles d ON a.ID = d.Animal_id";

    // Ejecutar consulta
    if (conexionbasededatos.ejecutarConsulta(consulta)) {
        // Obtener resultado
        MYSQL_RES *resultado = conexionbasededatos.obtenerResultado();
        if (resultado) {
            // Mostrar resultados
            MYSQL_ROW fila;
            while ((fila = mysql_fetch_row(resultado))) {
                cout << "ID:           " << fila[0] << endl;
                cout << "Nombre comun: " << fila[1] << endl;
                cout << "Estatura:     " << fila[2] << endl;
                cout << "Dieta:        " << fila[3] << endl;
                cout << "Descripcion:  " << fila[4] << endl;
                cout << "Edad:         " << fila[5] << endl;
                cout << "Sexo:         " << fila[6] << endl;
                cout << "Peso:         " << fila[7] << endl;
                cout << "-------------------------" << endl;
            }

            // Liberar resultado
            conexionbasededatos.liberarResultado(resultado);
        } else {
            cerr << "Error al obtener resultado de la consulta" << endl;
        }
    } else {
        cerr << "Error al ejecutar consulta" << endl;
    }
}








int main() {
    // Crear un objeto de la clase Conexionbasededatos
    Conexionbasededatos pruebadeconexion;

    // Conectar a la base de datos
    if (!pruebadeconexion.conectar("localhost", "root", "admin", "admizx", 3306)) {
        cerr << "Error al conectar a la base de datos. Saliendo del programa." << endl;
        return 1;
    }

mostrarAnimal(pruebadeconexion);

    return 0;
}
