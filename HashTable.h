#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class HashTable {
public:
    using Key   = string;
    using Value = string;

    //Constructor: capacidad fija (recomendado iniciar con un número mayor a 7)
    explicit HashTable(size_t capacity = 17);  // se usa explicit para evitar conversiones implícitas

    //INSERT
    //Inserta o actualiza una clave 
    void insert(const Key& key, const Value& value);

    //GET
    //Obtiene el valor asociado a la clave. Devuelve true si la encontró.
    bool get(const Key& key, Value& out_value) const;

    //REMOVE
    //Elimina una clave (marca como borrada). Devuelve true si la eliminó.
    bool remove(const Key& key);

    //DISPLAY
    //Muestra el contenido actual de la tabla.
    void display(ostream& os = cout) const;

    size_t size() const { return count_; }
    size_t capacity() const { return table_.size(); }

private:
    enum class SlotState : unsigned char { EMPTY, OCCUPIED, DELETED }; 
    //enum: define un tipo enumerado fuerte
    //SlotState: el estado de una casilla en open addressing
    //unsigned char: fija el tipo subyacente (1 byte) - Ahorra memoria

    //Valores
    //EMPTY:nunca se ha usado esa casilla (búsqueda se puede detener aquí)
    //OCCUPIED: hay un par (key, value) guardado ahí
    //DELETED: había un elemento, pero se eliminó (tombstone). Se mantiene para no romper el probing, pero permite reutilizarse al insertar.

    struct Entry {  //Entry: representa una casilla de la tabla
        Key key; //clave
        Value value;  //valor
        SlotState state = SlotState::EMPTY; //state: el estado de esa casilla, por defecto EMPTY
    };

    vector<Entry> table_; //arreglo principal de la hash table
    size_t count_ = 0; //conteo de elementos reales en la tabla (número de casillas OCCUPIED)

    //Hash sencillo
    size_t hashKey(const Key& key) const;

    // Probing cuadrático simple: idx(i) = (h + i*i) % m
    // Si encuentra la clave, devuelve true y deja 'index' donde está
    // Si no, devuelve false y deja 'index' en la mejor posición para insertar (EMPTY o primer DELETED)
    bool findIndex(const Key& key, size_t& index) const;
};

#endif // HASH_TABLE_H

//Explicación de mas o menos como funciona
//Insertar: Calcula índice h, si EMPTY ocupa; si OCCUPIED hace probing; si encuentra DELETED, lo guarda como mejor hueco para reutilizar
//Buscar: Probing hasta encontrar OCCUPIED con la misma key; si topa con EMPTY, para (no existe). DELETED no te detiene.
//Eliminar: Cambias state a DELETED (tombstone) y decrementas count_.
//SlotState define el ciclo de vida de cada slot, Entry es la unidad de almacenamiento, table_ es el contenedor, y count_ es el contador de elementos vigentes.