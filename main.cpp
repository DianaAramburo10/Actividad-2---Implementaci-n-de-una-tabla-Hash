#include <iostream>
#include "HashTable.h"
using namespace std;

int main() {
    HashTable ht(11); // capacidad fija y pequeña para ver colisiones

    cout << "== Insertando ==\n";
    ht.insert("Alicia", "555-1234");
    ht.insert("Bob",   "555-5678");
    ht.insert("Carol", "555-9012");
    ht.insert("Viviana",  "555-0001");
    ht.insert("Marlene",  "555-0002");

    ht.display();
    cout << "\n";

    cout << "== Búsqueda ==\n";
    string out;
    if (ht.get("Alicia", out)) cout << "Alicia: " << out << "\n";
    if (!ht.get("Zoe", out))  cout << "Zoe no encontrada\n";
    cout << "\n";

    cout << "== Actualización ==\n";
    ht.insert("Alicia", "555-9999");
    ht.get("Alicia", out);
    cout << "Alicia (nuevo) : " << out << "\n\n";

    cout << "== Eliminar ==\n";
    ht.remove("Bob");
    ht.display();
    cout << "\n";

    cout << "== Insertar Nuevo Elemento ==\n";
    ht.insert("Zoe", "555-3333");  // debería caer en un slot liberado
    ht.display();
    cout << "\n";

    return 0;
}
