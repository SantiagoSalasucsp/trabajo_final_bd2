#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <list>
//#include <bits/stdc++.h>
using namespace std;



static int id_sec = 1;
static int id_pis = 1;
static int id_sup = 1;
static int id_dis = 1;

struct discodu;
struct disco;
struct superficie;
struct pista;
struct sector;
struct tipo;


static vector<sector*> todos_sectores;
static vector<pista*> todas_pistas;
static vector<superficie*> todas_superficies;
static vector<disco*> todos_discos;


struct fff{
    string name;
    tipo* j;
    
};


struct tipo {
    int tam;
    int objeto_id;
    string name;
    tipo(int t, int id, string n){
        tam = t;
        objeto_id = id;
        name = n;
    }
    virtual void getvalor() = 0;
};

struct INTEGER: public tipo {
    int val;
    //vector<fff*> g;
    INTEGER(int val, int ID, int t, string n, int index=0): tipo(t, ID, n) {
        this->val = val;
    }
    void getvalor() override {
        cout << "el valor es:" << val << endl;
    }
};



struct DECIMAL: public tipo {
    double val;
    int precision;
    int escala;
    
    DECIMAL(double val, int ID, int precision, int escala, string n): tipo(8, ID, n) {
        this->val = val;
        this->precision = precision;
        this->escala = escala;
    }
    
    void getvalor() override {
        cout << "el valor es:" << val << endl;
    }
};


struct objeto {
    vector<tipo*> tipos;
    vector<tipo*> reserva;
    int tam;
    int objeto_id;
    
    
    objeto(vector<tipo*>& tipos_entrada) {
        
        if (!tipos_entrada.empty()) {
            objeto_id = tipos_entrada[0]->objeto_id;
            tam = 0;
            for (auto* tipo : tipos_entrada) {
                tam += tipo->tam;
            }
            tipos = tipos_entrada;
        }
    }
};



class Dato {
public:
    virtual ~Dato() {}
};

class DatoInt : public Dato {
public:
    int valor;
    DatoInt(int v) : valor(v) {}
};

class DatoString : public Dato {
public:
    std::string valor;
    DatoString(const std::string& v) : valor(v) {}
};

class DatoDouble : public Dato {
public:
    double valor;
    DatoDouble(double v) : valor(v) {}
};






struct Enlace {
    sector* ptr;

    Enlace(sector* s) : ptr(s) {}
    virtual ~Enlace() = default;
    virtual void mostrar() const = 0;
};


struct EnlaceInteger : public Enlace {
    int valor;

    EnlaceInteger(int val, sector* s)
        : Enlace(s), valor(val) {}
    
    void mostrar() const override {
        cout << "Entero: " << valor << endl;
    }
};


struct EnlaceDecimal : public Enlace {
    double valor;

    EnlaceDecimal(double val, sector* s)
        : Enlace(s), valor(val) {}
    
    void mostrar() const override {
        cout << "Decimal: " << valor << endl;
    }
};


struct EnlaceVarchar : public Enlace {
    string valor;

    EnlaceVarchar(string val, sector* s)
        : Enlace(s), valor(val) {}
    
    void mostrar() const override {
        cout << "String: " << valor << endl;
    }
};


template<class T>
struct nodo {
    T val;
    nodo<T>* next;

    nodo(T v, nodo<T>* n = nullptr) : val(v), next(n) {}
};


template<class T>
struct List {
    nodo<T>* head = nullptr;

    void add(T v) {
        head = new nodo<T>(v, head);
    }
};


class Tabla {
public:
    string nombre;

    Tabla(string n) : nombre(n) {}
    virtual int hash_func(int valor) { return 0; }
    virtual int hash_func(double valor) { return 0; }
    virtual int hash_func(const string& valor) { return 0; }
    virtual void insertar(Enlace* enlace) = 0;
    virtual Enlace* buscar(Dato* dato) = 0;
    virtual vector<vector<int>> buscar2(Dato* dato) = 0;
    virtual ~Tabla() = default;
};


struct TablaHashInteger : public Tabla {

    vector<List<Enlace*>> tabla;
    size_t tam;
    
public:
    TablaHashInteger(string n, size_t t) : Tabla(n), tam(t) {
        tabla.resize(tam);
    }
    
    int hash_func(int valor) override {
        return valor % tam;
    }
    
    void insertar(Enlace* enlace) override {
        int index = hash_func(static_cast<EnlaceInteger*>(enlace)->valor);
        tabla[index].add(enlace);
    }
    Enlace* buscar(Dato* dato) override;
    vector<vector<int>> buscar2(Dato* dato) override;
    /*Enlace* buscar(Dato* dato) override {
        DatoInt* datoInt = dynamic_cast<DatoInt*>(dato);
        if (!datoInt) return nullptr;
        
        int index = hash_func(datoInt->valor);
        
        for (nodo<Enlace*>* current = tabla[index].head; current != nullptr; current = current->next) {
            EnlaceInteger* enlace = dynamic_cast<EnlaceInteger*>(current->val);
            if (enlace && enlace->valor == datoInt->valor) {
                sector* sector_actual = enlace->ptr;
                
                cout << "Ubicación del dato:" << endl;
                cout << "Sector ID: " << sector_actual->ID_sector << endl;
                cout << "Pista ID: " << sector_actual->pista_padre->ID_pista << endl;
                cout << "Superficie ID: " << sector_actual->pista_padre->superficie_padre->ID_superficie << endl;
                cout << "Disco ID: " << sector_actual->pista_padre->superficie_padre->disco_padre->ID_disco << endl;
                
                return enlace;
            }
        }
        
        return nullptr;
    }*/
};


struct TablaHashDecimal : public Tabla {

    vector<List<Enlace*>> tabla;
    size_t tam;

public:
    TablaHashDecimal(string n, size_t t) : Tabla(n), tam(t) {
        tabla.resize(tam);
    }

    int hash_func(double valor) override {
        return static_cast<int>(valor) % tam;
    }

    void insertar(Enlace* enlace) override {
        int index = hash_func(static_cast<EnlaceDecimal*>(enlace)->valor);
        tabla[index].add(enlace);
    }
    Enlace* buscar(Dato* dato) override;
    vector<vector<int>> buscar2(Dato* dato) override ;
    /*Enlace* buscar(Dato* dato) override {
        DatoDouble* datoDouble = dynamic_cast<DatoDouble*>(dato);
        if (!datoDouble) return nullptr;
        
        int index = hash_func(datoDouble->valor);
        
        for (nodo<Enlace*>* current = tabla[index].head; current != nullptr; current = current->next) {
            EnlaceDecimal* enlace = dynamic_cast<EnlaceDecimal*>(current->val);
            if (enlace && enlace->valor == datoDouble->valor) {
                sector* sector_actual = enlace->ptr;
                
                cout << "Ubicación del dato:" << endl;
                cout << "Sector ID: " << sector_actual->ID_sector << endl;
                cout << "Pista ID: " << sector_actual->pista_padre->ID_pista << endl;
                cout << "Superficie ID: " << sector_actual->pista_padre->superficie_padre->ID_superficie << endl;
                cout << "Disco ID: " << sector_actual->pista_padre->superficie_padre->disco_padre->ID_disco << endl;
                
                return enlace;
            }
        }
        
        return nullptr;
    }*/
};


struct TablaHashVarchar : public Tabla {

    vector<List<Enlace*>> tabla;
    size_t tam;


    TablaHashVarchar(string n, size_t t) : Tabla(n), tam(t) {
        tabla.resize(tam);
    }

    int hash_func(const string& valor) override {
        size_t hash = 0;
        for (char c : valor) {
            hash = hash * 31 + c;
        }
        return hash % tam;
    }

    void insertar(Enlace* enlace) override {
        int index = hash_func(static_cast<EnlaceVarchar*>(enlace)->valor);
        tabla[index].add(enlace);
    }
    vector<vector<int>> buscar2(TablaHashVarchar& tabla, Dato* dato);
    Enlace* buscar(Dato* dato) override;
    vector<vector<int>> buscar2(Dato* dato) override;
    /*Enlace* buscar(Dato* dato) override {
        DatoString* datoString = dynamic_cast<DatoString*>(dato);
        if (!datoString) return nullptr;
        
        int index = hash_func(datoString->valor);
        
        for (nodo<Enlace*>* current = tabla[index].head; current != nullptr; current = current->next) {
            EnlaceVarchar* enlace = dynamic_cast<EnlaceVarchar*>(current->val);
            if (enlace && enlace->valor == datoString->valor) {
                sector* sector_actual = enlace->ptr;
                
                cout << "Ubicación del dato:" << endl;
                cout << "Sector ID: " << sector_actual->ID_sector << endl;
                cout << "Pista ID: " << sector_actual->pista_padre->ID_pista << endl;
                cout << "Superficie ID: " << sector_actual->pista_padre->superficie_padre->ID_superficie << endl;
                cout << "Disco ID: " << sector_actual->pista_padre->superficie_padre->disco_padre->ID_disco << endl;
                
                return enlace;
            }
        }
        
        return nullptr;
    }*/
};











struct sector {
    int tamano;
    sector* next_sector;
    pista* pista_padre;
    vector<int> objeto_ids;
    vector<tipo*> almacenados;
    int ID_sector;

    sector(int tam, pista* padre) {
        tamano = tam;
        next_sector = nullptr;
        pista_padre = padre;
        ID_sector = id_sec++;
        todos_sectores.push_back(this);
    }
};

struct pista {
    vector<sector*> sectores;
    pista* next_pista;
    superficie* superficie_padre;
    vector<int> objeto_ids;
    int ID_pista;

    pista(int numSectores, int tamSector, superficie* padre) {
        next_pista = nullptr;
        superficie_padre = padre;
        ID_pista = id_pis++;
        todas_pistas.push_back(this);

        for (int i = 0; i < numSectores; i++) {
            sector* nuevo_sector = new sector(tamSector, this);
            sectores.push_back(nuevo_sector);
        }
    }
};

struct superficie {
    vector<pista*> pistas;
    superficie* next_superficie;
    disco* disco_padre;
    vector<int> objeto_ids;
    int ID_superficie;

    superficie(int numPistas, int numSectores, int tamSector, disco* padre) {
        next_superficie = nullptr;
        disco_padre = padre;
        ID_superficie = id_sup++;
        todas_superficies.push_back(this);

        for (int i = 0; i < numPistas; i++) {
            pista* nueva_pista = new pista(numSectores, tamSector, this);
            pistas.push_back(nueva_pista);
        }
    }
};

struct disco {
    vector<superficie*> superficies;
    disco* next_disco;
    discodu* dicodu_padre;
    vector<int> objeto_ids;
    int ID_disco;

    disco(int numPistas, int numSectores, int tamSector, discodu* padre) {
        next_disco = nullptr;
        dicodu_padre = padre;
        ID_disco = id_dis++;
        todos_discos.push_back(this);

        for (int i = 0; i < 2; i++) {
            superficie* nueva_superficie = new superficie(numPistas, numSectores, tamSector, this);
            superficies.push_back(nueva_superficie);
        }
    }
};

struct partir {
    int tam1 = 0;
    int tam2 = 0;
    sector* sec1 = nullptr;
    sector* sec2 = nullptr;
};

struct VARCHAR: public tipo {
    string val;
    int longitud_maxima;
    partir particion;
    
    VARCHAR(string val, int ID, int max, string n): tipo(val.length(), ID, n) {
        this->longitud_maxima = max;
        if(val.length() <= max) {
            this->val = val;
        } else {
            cout << "no se pudo ingresar" << endl;
        }
    }
    
    void partir_tipo(sector* sector_actual) {
        
        int espacio_disponible = sector_actual->tamano;
        
        
        particion.sec1 = sector_actual;
        particion.tam1 = espacio_disponible;
        
        
        if (sector_actual->next_sector != nullptr) {
            particion.sec2 = sector_actual->next_sector;
            particion.tam2 = val.length() - espacio_disponible;
        }
    }
    
    void getvalor() override {
        cout << "el valor es:" << val << endl;
    }
};

struct discodu {
    vector<disco*> discos;
    vector<int> objeto_ids;
    int numDiscos;
    int numSuperficies;
    int numPistas;
    int numSectores;
    int tamSector;
    int sectoresporbloque;

    sector* ultimo_sector_usado;
    pista* ultima_pista_usada;
    superficie* ultima_superficie_usada;
    disco* ultimo_disco_usado;
    
    sector* main1;
    
    vector<Tabla*> tablas;

    void enlazar_elementos() {
        if (!todos_sectores.empty()) ultimo_sector_usado = todos_sectores[0];
        if (!todas_pistas.empty()) ultima_pista_usada = todas_pistas[0];
        if (!todas_superficies.empty()) ultima_superficie_usada = todas_superficies[0];
        if (!todos_discos.empty()) ultimo_disco_usado = todos_discos[0];
        main1 = todos_sectores[0];
        for(size_t i = 0; i < todos_sectores.size() - 1; i++) {
            todos_sectores[i]->next_sector = todos_sectores[i + 1];
        }
        if (!todos_sectores.empty()) {
            todos_sectores.back()->next_sector = nullptr;
        }

        for(size_t i = 0; i < todas_pistas.size() - 1; i++) {
            todas_pistas[i]->next_pista = todas_pistas[i + 1];
        }
        if (!todas_pistas.empty()) {
            todas_pistas.back()->next_pista = nullptr;
        }

        for(size_t i = 0; i < todas_superficies.size() - 1; i++) {
            todas_superficies[i]->next_superficie = todas_superficies[i + 1];
        }
        if (!todas_superficies.empty()) {
            todas_superficies.back()->next_superficie = nullptr;
        }

        for(size_t i = 0; i < todos_discos.size() - 1; i++) {
            todos_discos[i]->next_disco = todos_discos[i + 1];
        }
        if (!todos_discos.empty()) {
            todos_discos.back()->next_disco = nullptr;
        }
    }

    discodu(int nDiscos, int nPistas, int nSectores, int tSector,int secporblo, vector<Tabla*> tablas_iniciales) {
        todos_sectores.clear();
        todas_pistas.clear();
        todas_superficies.clear();
        todos_discos.clear();

        numDiscos = nDiscos;
        numSuperficies = 2;
        numPistas = nPistas;
        numSectores = nSectores;
        tamSector = tSector;
        sectoresporbloque=secporblo;
        tablas=tablas_iniciales;
        cout << "Creando dicodu con " << numDiscos << " discos" << endl;
        
        for (int i = 0; i < numDiscos; i++) {
            disco* nuevo_disco = new disco(numPistas, numSectores, tamSector, this);
            discos.push_back(nuevo_disco);
        }

        enlazar_elementos();

        todos_sectores.clear();
        todas_pistas.clear();
        todas_superficies.clear();
        todos_discos.clear();
    }
    
    

    ~discodu() {
        for (auto disco : discos) {
            for (auto superficie : disco->superficies) {
                for (auto pista : superficie->pistas) {
                    for (auto sector : pista->sectores) {
                        delete sector;
                    }
                    delete pista;
                }
                delete superficie;
            }
            delete disco;
        }
    }
    
    
    void actualizar_ids_sin_redundancia(vector<int>& vector_ids, int nuevo_id) {
        bool encontrado = false;
        for (int i = 0; i < vector_ids.size(); i++) {
            if (vector_ids[i] == nuevo_id) {
                encontrado = true;
                break;
            }
        }
        if (!encontrado) {
            vector_ids.push_back(nuevo_id);
        }
    }

    
    
    void insertar(objeto* obj) {
        if (main1 == nullptr) {
            cout << "Error: No hay sectores disponibles" << endl;
            return;
        }

        sector*& sector_actual = main1;
        bool ver = true;

        while (!obj->tipos.empty()) {
            tipo* tipo_actual = obj->tipos[0];

            if (sector_actual->tamano >= tipo_actual->tam) {
                sector_actual->almacenados.push_back(tipo_actual);

                // Crear enlace según el tipo
                Enlace* enlace = nullptr;

                // Identificar tipo y crear enlace correspondiente
                INTEGER* integer_actual = dynamic_cast<INTEGER*>(tipo_actual);
                VARCHAR* varchar_actual = dynamic_cast<VARCHAR*>(tipo_actual);
                DECIMAL* decimal_actual = dynamic_cast<DECIMAL*>(tipo_actual);

                if (integer_actual) {
                    enlace = new EnlaceInteger(integer_actual->val, sector_actual);
                }
                else if (varchar_actual) {
                    // Insertar el VARCHAR completo (sin particionar)
                    enlace = new EnlaceVarchar(varchar_actual->val, sector_actual);
                }
                else if (decimal_actual) {
                    enlace = new EnlaceDecimal(decimal_actual->val, sector_actual);
                }

                // Buscar tabla correspondiente e insertar usando el nombre del tipo
                for (int i = 0; i < tablas.size(); i++) {
                    if (tablas[i]->nombre == tipo_actual->name) {
                        tablas[i]->insertar(enlace);
                        break;
                    }
                }

                if (ver) {
                    actualizar_ids_sin_redundancia(sector_actual->objeto_ids, obj->objeto_id);

                    pista* pista_actual = sector_actual->pista_padre;
                    actualizar_ids_sin_redundancia(pista_actual->objeto_ids, obj->objeto_id);

                    superficie* superficie_actual = pista_actual->superficie_padre;
                    actualizar_ids_sin_redundancia(superficie_actual->objeto_ids, obj->objeto_id);

                    disco* disco_actual = superficie_actual->disco_padre;
                    actualizar_ids_sin_redundancia(disco_actual->objeto_ids, obj->objeto_id);

                    actualizar_ids_sin_redundancia(objeto_ids, obj->objeto_id);

                    ver = false;
                }

                sector_actual->tamano -= tipo_actual->tam;
                obj->tipos.erase(obj->tipos.begin());
            }
            else {
                // Manejo de espacios insuficientes para VARCHAR
                VARCHAR* varchar_actual = dynamic_cast<VARCHAR*>(tipo_actual);
                if (varchar_actual) {
                    // Configurar partición del VARCHAR en el sector actual
                    varchar_actual->particion.tam1 = sector_actual->tamano;
                    varchar_actual->particion.sec1 = sector_actual;

                    int tam_restante = varchar_actual->tam - sector_actual->tamano;
                    varchar_actual->tam = tam_restante;

                    if (sector_actual->next_sector != nullptr) {
                        varchar_actual->particion.sec2 = sector_actual->next_sector;
                        varchar_actual->particion.tam2 = varchar_actual->tam;
                    }

                    // Crear el enlace para el VARCHAR completo en el sector actual
                    Enlace* enlace = new EnlaceVarchar(varchar_actual->val, sector_actual);

                    // Buscar tabla correspondiente e insertar
                    for (int i = 0; i < tablas.size(); i++) {
                        if (tablas[i]->nombre == varchar_actual->name) {
                            tablas[i]->insertar(enlace);
                            break;
                        }
                    }

                    // Mover el resto de los tipos a la reserva
                    while (!obj->tipos.empty()) {
                        obj->reserva.push_back(obj->tipos[0]);
                        obj->tipos.erase(obj->tipos.begin());
                    }

                    // Actualizar el tamaño del sector y avanzar al siguiente
                    sector_actual->tamano = 0;
                    sector_actual = sector_actual->next_sector;
                    break;
                }
                else {
                    // Para otros tipos, simplemente mover a reserva
                    obj->reserva.push_back(tipo_actual);
                    obj->tipos.erase(obj->tipos.begin());
                }
            }


            if (obj->tipos.empty()) {
                sector_actual = sector_actual->next_sector;
                break;
            }
        }

        // Recursivamente insertar objetos en reserva
        if (!obj->reserva.empty()) {
            objeto reserva_objeto(obj->reserva);
            obj->reserva.clear();
            insertar(&reserva_objeto);
        }

        cout << "Objeto insertado" << endl;
    }

    
    
    //////////////////////////////////////////////////////
    
    /*
    void actualizar_ids_sin_redundancia(vector<int>& vector_ids, int nuevo_id) {
        bool encontrado = 0;
        for (int i = 0; i < vector_ids.size(); i++) {
            if (vector_ids[i] == nuevo_id) {
                encontrado = 1;
                break;
            }
        }
        if (!encontrado) {
            vector_ids.push_back(nuevo_id);
        }
    }
    
    
    void insertar(objeto* obj) {
        bool ver=1;
        int x=0;
        if (main1 == nullptr) {
            cout << "Error: No hay sectores disponibles" << endl;
            return;
        }

        
        sector* sector_actual = main1;
        tipo* tipo_actual = obj->tipos[0];
        
        while (!obj->tipos.empty()) {
            

            //tipo* tipo_actual = obj->tipos[x];
            
            if (sector_actual->tamano >= tipo_actual->tam) {
                
                sector_actual->almacenados.push_back(tipo_actual);
                
                
                if(ver){
                    //sector_actual->objeto_ids.push_back(obj->objeto_id);
                    actualizar_ids_sin_redundancia(sector_actual->objeto_ids, obj->objeto_id);
                    
                    pista* pista_actual = sector_actual->pista_padre;
                    actualizar_ids_sin_redundancia(pista_actual->objeto_ids, obj->objeto_id);
                    
                
                    superficie* superficie_actual = pista_actual->superficie_padre;
                    actualizar_ids_sin_redundancia(superficie_actual->objeto_ids, obj->objeto_id);
                    
                    
                    disco* disco_actual = superficie_actual->disco_padre;
                    actualizar_ids_sin_redundancia(disco_actual->objeto_ids, obj->objeto_id);
                    
                    actualizar_ids_sin_redundancia(objeto_ids, obj->objeto_id);

         


                    
                    ver=0;
                }
                sector_actual->tamano-=tipo_actual->tam;
                //sector_actual->objeto_ids.push_back(tipo_actual->objeto_id);
                
                //x++;
                obj->tipos.erase(obj->tipos.begin());
                
                
            }
            else {
                obj->reserva.push_back(tipo_actual);
                obj->tipos.erase(obj->tipos.begin());
                //x++;
            }
            
        }
        objeto xd(obj->reserva);
        main1++;
        insertar(&xd);

        cout << "se ingreso" << endl;
    }
    
    
    */
    
    
    
    void buscar(string nombre, int ID_objeto) {
        vector<int> camino;
        
     
        disco* disco_actual = this->discos[0];
        bool encontrado_en_disco = false;
        
        while (disco_actual != nullptr && !encontrado_en_disco) {
            for (int i = 0; i < disco_actual->objeto_ids.size(); i++) {
                if (disco_actual->objeto_ids[i] == ID_objeto) {
                    encontrado_en_disco = true;
                    break;
                }
            }
            if(!encontrado_en_disco){
                disco_actual = disco_actual->next_disco;
            }
        }
        
        if (encontrado_en_disco)
            camino.push_back(disco_actual->ID_disco);
        
   
        superficie* superficie_actual = disco_actual->superficies[0];
        bool encontrado_en_superficie = false;
        
        while (superficie_actual != nullptr && !encontrado_en_superficie) {
            for (int i = 0; i < superficie_actual->objeto_ids.size(); i++) {
                if (superficie_actual->objeto_ids[i] == ID_objeto) {
                    encontrado_en_superficie = true;
                    break;
                }
            }
            if(!encontrado_en_superficie){
                superficie_actual = superficie_actual->next_superficie;
            }
        }
        
        if(encontrado_en_superficie)
            camino.push_back(superficie_actual->ID_superficie);
        
      
        pista* pista_actual = superficie_actual->pistas[0];
        bool encontrado_en_pista = false;
        
        while (pista_actual != nullptr && !encontrado_en_pista) {
            for (int i = 0; i < pista_actual->objeto_ids.size(); i++) {
                if (pista_actual->objeto_ids[i] == ID_objeto) {
                    encontrado_en_pista = true;
                    break;
                }
            }
            if(!encontrado_en_pista){
                pista_actual = pista_actual->next_pista;
            }
        }
        
        if(encontrado_en_pista)
            camino.push_back(pista_actual->ID_pista);
        
       
        sector* sector_actual = pista_actual->sectores[0];
        bool encontrado = false;
        
       
        while (sector_actual != nullptr && !encontrado) {
          
            bool encontrado_en_sector = false;
            for (int i = 0; i < sector_actual->objeto_ids.size(); i++) {
                if (sector_actual->objeto_ids[i] == ID_objeto) {
                    encontrado_en_sector = true;
                    break;
                }
            }
            
          
            if (encontrado_en_sector) {
                for (int i = 0; i < sector_actual->almacenados.size(); i++) {
                    tipo* tipo_actual = sector_actual->almacenados[i];
                    if (tipo_actual->name == nombre && tipo_actual->objeto_id == ID_objeto) {
                        camino.push_back(sector_actual->ID_sector);
                        
                        
                        cout << "Objeto encontrado:" << endl;
                        cout << "  Disco: " << camino[0] << endl;
                        cout << "  Superficie: " << camino[1] << endl;
                        cout << "  Pista: " << camino[2] << endl;
                        cout << "  Sector: " << camino[3] << endl;
                        
                        tipo_actual->getvalor();
                        encontrado = true;
                        break;
                    }
                }
            }
            
           
            if (!encontrado) {
                sector_actual = sector_actual->next_sector;
            }
        }
        
    
        if (!encontrado) {
            cout << "Objeto no encontrado" << endl;
        }
    }
    
    
    Enlace* buscar1(Dato* dato, const std::string& nombreTabla) {
        
        for (Tabla* tabla : tablas) {
            if (tabla->nombre == nombreTabla) {
                return tabla->buscar(dato);
            }
        }
        
        
        cout << "Tabla con el nombre '" << nombreTabla << "' no encontrada." << endl;
        return nullptr;
    }


     

            


    
    
    
};





// TablaHashInteger
Enlace* TablaHashInteger::buscar(Dato* dato)  {
    DatoInt* datoInt = dynamic_cast<DatoInt*>(dato);
    if (!datoInt) return nullptr;
    
    int index = hash_func(datoInt->valor);
    
    for (nodo<Enlace*>* current = tabla[index].head; current != nullptr; current = current->next) {
        EnlaceInteger* enlace = dynamic_cast<EnlaceInteger*>(current->val);
        if (enlace && enlace->valor == datoInt->valor) {
            sector* sector_actual = enlace->ptr;
            
            cout << "Ubicación del dato:" << endl;
            cout << "Sector ID: " << sector_actual->ID_sector << endl;
            cout << "Pista ID: " << sector_actual->pista_padre->ID_pista << endl;
            cout << "Superficie ID: " << sector_actual->pista_padre->superficie_padre->ID_superficie << endl;
            cout << "Disco ID: " << sector_actual->pista_padre->superficie_padre->disco_padre->ID_disco << endl;
            
            return enlace;
        }
    }
    
    return nullptr;
}

vector<vector<int>> TablaHashInteger::buscar2(Dato* dato) {
    vector<vector<int>> resultados;
    DatoInt* datoInt = dynamic_cast<DatoInt*>(dato);
    if (!datoInt) return resultados;
    
    int index = hash_func(datoInt->valor);
    
    for (nodo<Enlace*>* current = tabla[index].head; current != nullptr; current = current->next) {
        EnlaceInteger* enlace = dynamic_cast<EnlaceInteger*>(current->val);
        if (enlace && enlace->valor == datoInt->valor) {
            vector<int> ubicacion;
            sector* sector_actual = enlace->ptr;
            
            // Buscar el ID del tipo en los sectores
            int tipo_id = -1;
            for (auto tipo : sector_actual->almacenados) {
                INTEGER* intTipo = dynamic_cast<INTEGER*>(tipo);
                if (intTipo && intTipo->val == datoInt->valor) {
                    tipo_id = intTipo->objeto_id;
                    break;
                }
            }
            
            // Agregar el objeto_id encontrado
            ubicacion.push_back(tipo_id);
            
            // Luego agregar los IDs en el orden especificado
            ubicacion.push_back(sector_actual->ID_sector);
            ubicacion.push_back(sector_actual->pista_padre->ID_pista);
            ubicacion.push_back(sector_actual->pista_padre->superficie_padre->ID_superficie);
            ubicacion.push_back(sector_actual->pista_padre->superficie_padre->disco_padre->ID_disco);
            
            resultados.push_back(ubicacion);
        }
    }
    
    return resultados;
}




// TablaHashDecimal
Enlace* TablaHashDecimal::buscar(Dato* dato)  {
    DatoDouble* datoDouble = dynamic_cast<DatoDouble*>(dato);
    if (!datoDouble) return nullptr;
    
    int index = hash_func(datoDouble->valor);
    
    for (nodo<Enlace*>* current = tabla[index].head; current != nullptr; current = current->next) {
        EnlaceDecimal* enlace = dynamic_cast<EnlaceDecimal*>(current->val);
        if (enlace && enlace->valor == datoDouble->valor) {
            sector* sector_actual = enlace->ptr;
            
            cout << "Ubicación del dato:" << endl;
            cout << "Sector ID: " << sector_actual->ID_sector << endl;
            cout << "Pista ID: " << sector_actual->pista_padre->ID_pista << endl;
            cout << "Superficie ID: " << sector_actual->pista_padre->superficie_padre->ID_superficie << endl;
            cout << "Disco ID: " << sector_actual->pista_padre->superficie_padre->disco_padre->ID_disco << endl;
            
            return enlace;
        }
    }
    
    return nullptr;
}

vector<vector<int>> TablaHashDecimal::buscar2(Dato* dato){
        vector<vector<int>> resultados;
        DatoDouble* datoDouble = dynamic_cast<DatoDouble*>(dato);
        if (!datoDouble) return resultados;
        
        int index = hash_func(datoDouble->valor);
        
        for (nodo<Enlace*>* current = tabla[index].head; current != nullptr; current = current->next) {
            EnlaceDecimal* enlace = dynamic_cast<EnlaceDecimal*>(current->val);
            if (enlace && enlace->valor == datoDouble->valor) {
                vector<int> ubicacion;
                sector* sector_actual = enlace->ptr;
                
                // Buscar el ID del tipo en los sectores
                int tipo_id = -1;
                for (auto tipo : sector_actual->almacenados) {
                    DECIMAL* decimalTipo = dynamic_cast<DECIMAL*>(tipo);
                    if (decimalTipo && decimalTipo->val == datoDouble->valor) {
                        tipo_id = decimalTipo->objeto_id;
                        break;
                    }
                }
                
                // Agregar el objeto_id encontrado
                ubicacion.push_back(tipo_id);
                
                // Luego agregar los IDs en el orden especificado
                ubicacion.push_back(sector_actual->ID_sector);
                ubicacion.push_back(sector_actual->pista_padre->ID_pista);
                ubicacion.push_back(sector_actual->pista_padre->superficie_padre->ID_superficie);
                ubicacion.push_back(sector_actual->pista_padre->superficie_padre->disco_padre->ID_disco);
                
                resultados.push_back(ubicacion);
            }
        }
        
        return resultados;
    }




// TablaHashVarchar
Enlace* TablaHashVarchar::buscar(Dato* dato)  {
    DatoString* datoString = dynamic_cast<DatoString*>(dato);
    if (!datoString) return nullptr;
    
    int index = hash_func(datoString->valor);
    
    for (nodo<Enlace*>* current = tabla[index].head; current != nullptr; current = current->next) {
        EnlaceVarchar* enlace = dynamic_cast<EnlaceVarchar*>(current->val);
        if (enlace && enlace->valor == datoString->valor) {
            sector* sector_actual = enlace->ptr;
            
            cout << "Ubicación del dato:" << endl;
            cout << "Sector ID: " << sector_actual->ID_sector << endl;
            cout << "Pista ID: " << sector_actual->pista_padre->ID_pista << endl;
            cout << "Superficie ID: " << sector_actual->pista_padre->superficie_padre->ID_superficie << endl;
            cout << "Disco ID: " << sector_actual->pista_padre->superficie_padre->disco_padre->ID_disco << endl;
            
            return enlace;
        }
    }
    
    return nullptr;
}

vector<vector<int>> TablaHashVarchar::buscar2(Dato* dato)  {
        vector<vector<int>> resultados;
        DatoString* datoString = dynamic_cast<DatoString*>(dato);
        if (!datoString) return resultados;
        
        int index = hash_func(datoString->valor);
        
        for (nodo<Enlace*>* current = tabla[index].head; current != nullptr; current = current->next) {
            EnlaceVarchar* enlace = dynamic_cast<EnlaceVarchar*>(current->val);
            if (enlace && enlace->valor == datoString->valor) {
                vector<int> ubicacion;
                sector* sector_actual = enlace->ptr;
                
                // Buscar el ID del tipo en los sectores
                int tipo_id = -1;
                for (auto tipo : sector_actual->almacenados) {
                    if (tipo->name == datoString->valor) {
                        tipo_id = tipo->objeto_id;
                        break;
                    }
                }
                
                // Agregar el objeto_id encontrado
                ubicacion.push_back(tipo_id);
                
                // Luego agregar los IDs en el orden especificado
                ubicacion.push_back(sector_actual->ID_sector);
                ubicacion.push_back(sector_actual->pista_padre->ID_pista);
                ubicacion.push_back(sector_actual->pista_padre->superficie_padre->ID_superficie);
                ubicacion.push_back(sector_actual->pista_padre->superficie_padre->disco_padre->ID_disco);
                
                resultados.push_back(ubicacion);
            }
        }
        
        return resultados;
    }




class tabla {
private:
    vector<string> tokens;

public:
    tabla() {
        ifstream archivo("estructura.txt");
        if (!archivo.is_open()) {
            cerr << "Error: No se pudo abrir el archivo estructura.txt" << endl;
            return;
        }

        string linea;
        getline(archivo, linea);

        while (getline(archivo, linea)) {
            if (linea == ");") {
                break;
            }

            if (!linea.empty() && linea != ")") {
                if (linea[linea.length() - 1] == ',') {
                    linea = linea.substr(0, linea.length() - 1);
                }

                stringstream ss(linea);
                string palabra;

                while (ss >> palabra) {
                    if (palabra == "PRIMARY" || palabra == "KEY" || palabra == "NOT" || palabra == "NULL") {
                        continue;
                    }

                    if (palabra == ");") {
                        break;
                    }

                    size_t pos = palabra.find("(");
                    if (pos != string::npos) {
                        string tipo = palabra.substr(0, pos);
                        string numero = palabra.substr(pos + 1);

                        if (!numero.empty() && (numero[numero.length() - 1] == ')' || numero[numero.length() - 1] == ',')) {
                            numero = numero.substr(0, numero.length() - 1);
                        }

                        if (!tipo.empty()) tokens.push_back(tipo);
                        if (!numero.empty()) tokens.push_back(numero);
                        continue;
                    }

                    if (palabra[palabra.length() - 1] == ')' || palabra[palabra.length() - 1] == ',') {
                        palabra = palabra.substr(0, palabra.length() - 1);
                    }

                    if (!palabra.empty()) {
                        tokens.push_back(palabra);
                    }
                }
            }
        }
        archivo.close();

        cout << "Tokens encontrados:" << endl;
        for (const auto& token : tokens) {
            cout << token << endl;
        }

        cout << "\nTokens en formato array:" << endl;
        cout << "{";
        for (size_t i = 0; i < tokens.size(); i++) {
            cout << "\"" << tokens[i] << "\"";
            if (i < tokens.size() - 1) cout << ", ";
        }
        cout << "}" << endl;
    }

    vector<string> getTokens() {
        return tokens;
    }
};





int main() {
    vector<Tabla*> mis_tablas = {
            //new TablaHashInteger("", 101),
            new TablaHashDecimal("Cost", 10),
            new TablaHashDecimal("tax", 10),
            new TablaHashDecimal("total", 10),
            new TablaHashVarchar("Item", 10)
        };
    discodu d(3, 3, 5,1000,4,mis_tablas);
    
    
    vector<tipo*> tipos;
    cout << "IDs asignados:\n";
    cout << "Último sector: " << id_sec - 1 << endl;
    cout << "Última pista: " << id_pis - 1 << endl;
    cout << "Última superficie: " << id_sup - 1 << endl;
    cout << "Último disco: " << id_dis - 1 << endl;
    //tabla xd;
    
    
    //fila 2
        INTEGER* index = new INTEGER(1, 1, 4, "Index");
        DECIMAL* cost = new DECIMAL(7.97, 1, 2, 2, "Cost");
        DECIMAL* tax = new DECIMAL(0.6, 1, 2, 2, "Tax");
        VARCHAR* item = new VARCHAR("Fruit of the Loom Gir", 1, 40, "Item");
        tipos.push_back(index);
        tipos.push_back(cost);
        tipos.push_back(tax);
        tipos.push_back(item);
        objeto* obj1 = new objeto(tipos);
        d.insertar(obj1);
        tipos.clear();

        // Fila 3
        index = new INTEGER(2, 2, 4, "Index");
        cost = new DECIMAL(2.97, 2, 2, 2, "Cost");
        tax = new DECIMAL(0.22, 2, 2, 2, "Tax");
        item = new VARCHAR("Rawlings Little Leag", 2, 40, "Item");
        tipos.push_back(index);
        tipos.push_back(cost);
        tipos.push_back(tax);
        tipos.push_back(item);
        objeto* obj2 = new objeto(tipos);
        d.insertar(obj2);
        tipos.clear();

        // Fila 4
        index = new INTEGER(3, 3, 4, "Index");
        cost = new DECIMAL(1.29, 3, 2, 2, "Cost");
        tax = new DECIMAL(0.1, 3, 2, 2, "Tax");
        item = new VARCHAR("Secret Antiperspirant", 3, 40, "Item");
        tipos.push_back(index);
        tipos.push_back(cost);
        tipos.push_back(tax);
        tipos.push_back(item);
        objeto* obj3 = new objeto(tipos);
        d.insertar(obj3);
        tipos.clear();

        // Fila 5
        index = new INTEGER(4, 4, 4, "Index");
        cost = new DECIMAL(14.96, 4, 2, 2, "Cost");
        tax = new DECIMAL(1.12, 4, 2, 2, "Tax");
        item = new VARCHAR("Deadpool DVD", 4, 40, "Item");
        tipos.push_back(index);
        tipos.push_back(cost);
        tipos.push_back(tax);
        tipos.push_back(item);
        objeto* obj4 = new objeto(tipos);
        d.insertar(obj4);
        tipos.clear();

        // Fila 6
        index = new INTEGER(5, 5, 4, "Index");
        cost = new DECIMAL(7.28, 5, 2, 2, "Cost");
        tax = new DECIMAL(0.55, 5, 2, 2, "Tax");
        item = new VARCHAR("Maxwell House Coffe", 5, 40, "Item");
        tipos.push_back(index);
        tipos.push_back(cost);
        tipos.push_back(tax);
        tipos.push_back(item);
        objeto* obj5 = new objeto(tipos);
        d.insertar(obj5);
        tipos.clear();

        // Fila 7
        index = new INTEGER(6, 6, 4, "Index");
        cost = new DECIMAL(6.68, 6, 2, 2, "Cost");
        tax = new DECIMAL(0.5, 6, 2, 2, "Tax");
        item = new VARCHAR("Banana Boat Sunscr", 6, 40, "Item");
        tipos.push_back(index);
        tipos.push_back(cost);
        tipos.push_back(tax);
        tipos.push_back(item);
        objeto* obj6 = new objeto(tipos);
        d.insertar(obj6);
        tipos.clear();

        // Fila 8
        index = new INTEGER(7, 7, 4, "Index");
        cost = new DECIMAL(10.0, 7, 2, 2, "Cost");
        tax = new DECIMAL(0.75, 7, 2, 2, "Tax");
        item = new VARCHAR("Wrench Set, 18 piec", 7, 40, "Item");
        tipos.push_back(index);
        tipos.push_back(cost);
        tipos.push_back(tax);
        tipos.push_back(item);
        objeto* obj7 = new objeto(tipos);
        d.insertar(obj7);
        tipos.clear();

        // Fila 9
        index = new INTEGER(8, 8, 4, "Index");
        cost = new DECIMAL(8.98, 8, 2, 2, "Cost");
        tax = new DECIMAL(0.67, 8, 2, 2, "Tax");
        item = new VARCHAR("M and M, 42 oz", 8, 40, "Item");
        tipos.push_back(index);
        tipos.push_back(cost);
        tipos.push_back(tax);
        tipos.push_back(item);
        objeto* obj8 = new objeto(tipos);
        d.insertar(obj8);
        tipos.clear();

        // Fila 10
        index = new INTEGER(9, 9, 4, "Index");
        cost = new DECIMAL(2.12, 9, 2, 2, "Cost");
        tax = new DECIMAL(0.16, 9, 2, 2, "Tax");
        item = new VARCHAR("Bertoli Alfredo Sauc", 9, 40, "Item");
        tipos.push_back(index);
        tipos.push_back(cost);
        tipos.push_back(tax);
        tipos.push_back(item);
        objeto* obj9 = new objeto(tipos);
        d.insertar(obj9);
        tipos.clear();

        // Fila 11
        index = new INTEGER(10, 10, 4, "Index");
        cost = new DECIMAL(6.19, 10, 2, 2, "Cost");
        tax = new DECIMAL(0.46, 10, 2, 2, "Tax");
        item = new VARCHAR("Large Paperclips, 10", 10, 40, "Item");
        tipos.push_back(index);
        tipos.push_back(cost);
        tipos.push_back(tax);
        tipos.push_back(item);
        objeto* obj10 = new objeto(tipos);
        d.insertar(obj10);
        tipos.clear();
    
    
    d.buscar("Cost", 7);
    cout<<endl<<endl<<endl;
    DatoDouble xd(10);
    d.buscar1(&xd, "Cost");
    
    

    return 0;
}

