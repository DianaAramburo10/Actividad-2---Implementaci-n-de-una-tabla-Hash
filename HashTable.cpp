#include "HashTable.h". // Incluye la declaración de la clase, struct Entry y enums
#include <iostream> 
#include <string> 
using namespace std;

// Constructor
HashTable::HashTable(size_t capacity) {
    if (capacity < 3) capacity = 3; // mínimo razonable
    table_.assign(capacity, Entry{}); // Cada casilla se inicializa con Entry{} ( key="", value="" ) y state=EMPTY
}

// hashKey
// Aplica std::hash<Key> a la clave y reduce el valor al rango [0, m-1],
// donde m = table_.size(). Esta es la posición base para empezar a probar.
size_t HashTable::hashKey(const Key& key) const {
    return std::hash<Key>{}(key) % table_.size();
}

// findIndex
// Busca el índice de una clave usando probing cuadrático
bool HashTable::findIndex(const Key& key, size_t& index) const {
    size_t m = table_.size();      // número de casillas
    size_t h = hashKey(key);       // índice base (hash reducido)

    size_t first_deleted = m;      //guarda el primer índice DELETED visto (m = "no hay")
    for (size_t i = 0; i < m; ++i) {
        size_t idx = (h + i * i) % m;  // probing cuadrático
        const Entry& e = table_[idx];  // referencia a la casilla actual

        if (e.state == SlotState::EMPTY) {
            // EMPTY significa que nunca hubo nada aquí: si llegamos hasta aquí,la clave no existe. Para insertar, preferimos un DELETED previo
            //si no, este EMPTY.
            index = (first_deleted != m) ? first_deleted : idx;
            return false; // clave NO encontrada
        }
        if (e.state == SlotState::DELETED) {
            // DELETED: guardamos el primero que veamos para insertar ahí si al final confirmamos que la clave no existe.
            if (first_deleted == m) first_deleted = idx;
            continue; // seguimos probando
        }
        // Si llegamos aquí, la casilla está OCCUPIED.
        if (e.key == key) {
            // Encontramos la clave: devolvemos su índice.
            index = idx;
            return true; // clave encontrada
        }
        // Si está OCCUPIED pero con otra clave, seguimos probando con i+1
    }

    // Si recorremos toda la tabla:
    // -Si había un DELETED, sugerimos insertarlo ahí.
    // -Si no, devolvemos m como "sin hueco" (caso patológico si estaba llena).
    index = (first_deleted != m) ? first_deleted : m;
    return false; // no se encontró la clave
}

//insert
//Inserta o actualiza un (key, value)
void HashTable::insert(const Key& key, const Value& value) {
    // Si no hay espacios (ni EMPTY ni DELETED), no podemos insertar.
    if (count_ == table_.size()) {
        cout << "[WARN] La tabla está llena. No se puede insertar \"" << key << "\".\n";
        return;
    }

    size_t idx = 0;
    bool found = findIndex(key, idx);  //busca posición o hueco ideal

    if (found) {
        // La clave ya existe: actualizamos su valor
        table_[idx].value = value;                  // actualiza
    } else {
        //La clave no existe: debemos insertar en 'idx'.
        if (idx == table_.size()) {     // no encontró hueco (muy raro si no está llena)
            cout << "[WARN] No hay hueco disponible para \"" << key << "\".\n";
            return;
        }
        //inserta
        table_[idx].key   = key;    // guardamos la clave
        table_[idx].value = value;  // guarda el valor
        table_[idx].state = SlotState::OCCUPIED;  // marcamos la casilla como ocupada
        ++count_;   // aumenta el número de elementos reales
    }
}

// get
// Busca una clave y, si existe, devuelve su valor en 'out_value' y true.
bool HashTable::get(const Key& key, Value& out_value) const {
    size_t idx = 0;
    bool found = findIndex(key, idx); // localiza la posición de la clave
    if (!found) return false;  // no existe
    out_value = table_[idx].value;  // devuelve el valor por referencia
    return true;
}

// remove
// Elimina una clave si existe. En open addressing se marca la casilla como DELETED (tombstone)
bool HashTable::remove(const Key& key) {
    size_t idx = 0;
    bool found = findIndex(key, idx);  // localiza la posición de la clave
    if (!found) return false; // no existe, nada que borrar

    table_[idx].state = SlotState::DELETED; // tombstone
    table_[idx].key.clear();  //limpia contendido 
    table_[idx].value.clear();  //limpia contendido 
    --count_;    // decrementa número de elementos reales
    return true;
}

// display
// Imprime la tabla:
void HashTable::display(ostream& os) const {
    os << "HashTable { size=" << count_ << ", capacity=" << table_.size() << " }\n";
    os << "Index | State     | Key / Value\n";
    os << "---------------------------------\n";
    for (size_t i = 0; i < table_.size(); ++i) {
        const Entry& e = table_[i];  // casilla actual
        // Convertimos el enum a texto para mostrarlo
        string st = (e.state == SlotState::EMPTY)   ? "EMPTY" :
                    (e.state == SlotState::OCCUPIED)? "OCCUPIED" : "DELETED";
        os << i << " | " << st << " | ";
        if (e.state == SlotState::OCCUPIED) os << e.key << " : " << e.value;
        os << "\n";
    }
}
