#include <iostream>
using namespace std;

struct Produk
{
    int id;
    string nama;
    int stok;
    string kategori;
    int harga;
    Produk *next;
    Produk *left; 
    Produk *right;
};

struct Pelanggan
{
    int id;
    string nama;
    string alamat;
    string no_telepon;
    Pelanggan *next;
};

struct DetailPesanan
{
    int idProduk;
    string namaProduk;
    int kuantitas;
    int harga;
    DetailPesanan *next;
};

struct Pesanan
{
    int idPesanan;
    int idPelanggan;
    int idProduk;
    string status;
    DetailPesanan *detailPesanan;
    Pesanan *next;
};

struct Stack
{
    Pesanan *data;
    Stack *next;
};

struct Queue
{
    Pesanan *data;
    Queue *next;
};

struct GraphNode
{
    int idProduk;
    int jumlahDibeli;
    GraphNode *next;
};

struct Hash
{
    Pelanggan *data;
    Hash *next;
};

struct TreeNode
{
    Produk *produk;
    TreeNode *left;
    TreeNode *right;
};

TreeNode *root = nullptr;

const int TABLE_SIZE = 100;
Hash *hashTable[TABLE_SIZE] = {nullptr};

GraphNode *graphProduk[100];
Produk *kepalaProduk = nullptr;
Pesanan *kepalaPesanan = nullptr;

Pelanggan *kepalaPelanggan = nullptr;
Pelanggan *akhirPelanggan = nullptr;

Queue *front = nullptr;
Queue *rear = nullptr;
Stack *top = nullptr;
Stack *riwayatPesananTop = nullptr;

void menu();
void dashboardProduk();
void dashboardPelanggan();
void dashboardTransaksi();
void insertPelangganToHashTable(Pelanggan *pelanggan);
void tampilkanPelangganSorted();
void pushRiwayatPesanan(Pesanan *pesanan);

static int produkId = 1;
static int pelangganId = 1;
static int pesananId = 1;

// Fungsi generateId untuk Produk - rekursif - O(n)
int generateProdukId()
{
    static int id = 1;
    if (id == produkId)
    {
        produkId++;
        return id;
    }
    else
    {
        id++;
        return generateProdukId();
    }
}

// Fungsi generateId untuk Pelanggan - rekursif - O(n)
int generatePelangganId()
{
    static int id = 1;
    if (id == pelangganId)
    {
        pelangganId++;
        return id;
    }
    else
    {
        id++;
        return generatePelangganId();
    }
}

// Fungsi generateId untuk Pesanan - rekursif - O(n)
int generatePesananId()
{
    static int id = 1;
    if (id == pesananId)
    {
        pesananId++;
        return id;
    }
    else
    {
        id++;
        return generatePesananId();
    }
}

// Fungsi untuk memasukkan pesanan dalam antrian - queue - O(1)
void enqueue(Pesanan *pesanan)
{
    Queue *newNode = new Queue();
    newNode->data = pesanan;
    newNode->next = nullptr;

    if (front == nullptr && rear == nullptr)
    {
        front = newNode;
        rear = newNode;
    }
    else
    {
        rear->next = newNode;
        rear = newNode;
    }
}

// fungsi untuk mengambil pesanan dalam antrian - queue -  O(1)
Pesanan *dequeue()
{
    if (front == nullptr)
    {
        return nullptr;
    }

    Queue *temp = front;
    Pesanan *pesanan = temp->data;
    front = front->next;

    if (front == nullptr)
    {
        rear = nullptr;
    }

    delete temp;
    return pesanan;
}

// fungsi menambah node (vertex) baru pada graph - graph - O(n)
void tambahNodeGraph(int idProduk)
{
    GraphNode *newNode = new GraphNode();
    newNode->idProduk = idProduk;
    newNode->jumlahDibeli = 1;
    newNode->next = nullptr;

    GraphNode *temp = graphProduk[idProduk];
    if (temp == nullptr)
    {
        graphProduk[idProduk] = newNode;
    }
    else
    {
        while (temp->next != nullptr)
        {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

// fungsi mengupdate data di node (vertex) pada graph - graph - O(n)
void updateNodeGraph(int idProduk)
{
    GraphNode *temp = graphProduk[idProduk];
    if (temp != nullptr)
    {
        temp->jumlahDibeli++;
    }
    else
    {
        tambahNodeGraph(idProduk);
    }
}

// fungsi hashing menggunakan ASCII dengan menjumlahkannya sesuai nama pelanggan - hash table - O(n)
int hashFunction(string key)
{
    int sum = 0;
    for (char c : key)
    {
        sum += c;
    }
    return sum % TABLE_SIZE;
}

// fungsi untuk membuat node(vertex) pada tree - tree - O(1)
TreeNode *tambahTreeNode(Produk *produk)
{
    TreeNode *newNode = new TreeNode();
    newNode->produk = produk;
    newNode->left = nullptr;
    newNode->right = nullptr;
    return newNode;
}

// fungsi menambahkan data produk kedalam tree - tree - O(n)
TreeNode *insertTree(TreeNode *root, Produk *produk)
{
    if (root == nullptr)
    {
        return tambahTreeNode(produk);
    }
    if (produk->kategori < root->produk->kategori)
    {
        root->left = insertTree(root->left, produk);
    }
    else if (produk->kategori > root->produk->kategori)
    {
        root->right = insertTree(root->right, produk);
    }
    else
    {
        // Jika kategorinya sama, masukkan berdasarkan nama produk
        if (produk->nama < root->produk->nama)
        {
            root->left = insertTree(root->left, produk);
        }
        else
        {
            root->right = insertTree(root->right, produk);
        }
    }
    return root;
}

// fungsi untuk membantu menemukan data produk di dalam tree - tree - O(n)
TreeNode *minValueNode(TreeNode *node)
{
    TreeNode *current = node;
    while (current && current->left != nullptr)
        current = current->left;
    return current;
}

// fungsi untuk menghapus data produk dari tree - tree - O(n)
TreeNode *hapusTreeNode(TreeNode *root, Produk *produk)
{
    if (root == nullptr)
        return root;

    if (produk->kategori < root->produk->kategori)
        root->left = hapusTreeNode(root->left, produk);
    else if (produk->kategori > root->produk->kategori)
        root->right = hapusTreeNode(root->right, produk);
    else
    {
        if (produk->nama < root->produk->nama)
            root->left = hapusTreeNode(root->left, produk);
        else if (produk->nama > root->produk->nama)
            root->right = hapusTreeNode(root->right, produk);
        else
        {
            if (root->left == nullptr)
            {
                TreeNode *temp = root->right;
                delete root;
                return temp;
            }
            else if (root->right == nullptr)
            {
                TreeNode *temp = root->left;
                delete root;
                return temp;
            }

            TreeNode *temp = minValueNode(root->right);
            root->produk = temp->produk;
            root->right = hapusTreeNode(root->right, temp->produk);
        }
    }
    return root;
}

// fungsi untuk mencari data produk sub tree di dalam tree - dengan motede binary seacrh tree - O(n)
void inorderTree(TreeNode *root)
{
    if (root != nullptr)
    {
        inorderTree(root->left);
        cout << "ID: " << root->produk->id << ", Nama: " << root->produk->nama << ", Stok: " << root->produk->stok << ", Kategori: " << root->produk->kategori << ", Harga: " << root->produk->harga << endl;
        inorderTree(root->right);
    }
}

// fungsi untuk mengatur space string dalam tampilan - O(n)
string padString(const string &str, int width)
{
    if (str.length() < width)
    {
        return str + string(width - str.length(), ' ');
    }
    return str.substr(0, width);
}

// fungsi untuk mengatur space int dalam tampilan - O(n)
string padNumber(int num, int width)
{
    string str = to_string(num);
    if (str.length() < width)
    {
        return str + string(width - str.length(), ' ');
    }
    return str.substr(0, width);
}

// fungsi untuk menampilkan kategori produk - tree - O(n)
void displayCategories(TreeNode *root, const string &category)
{
    if (root != nullptr)
    {
        displayCategories(root->left, category);
        if (root->produk->kategori == category)
        {
            cout << "| "
                 << padString(root->produk->kategori, 15) << " | "
                 << padNumber(root->produk->id, 8) << " | "
                 << padString(root->produk->nama, 20) << " | "
                 << padNumber(root->produk->stok, 10) << " | "
                 << padNumber(root->produk->harga, 10) << " |"
                 << endl;
        }
        displayCategories(root->right, category);
    }
}

// fungsi menambahkan produk baru - linkedlist dan tree - O(n)
void tambahProduk()
{
    system("CLS");
    Produk *baru = new Produk();
    baru->id = generateProdukId();
    cout << "Masukkan nama produk: ";
    cin.ignore();
    getline(cin, baru->nama);
    cout << "Masukkan stok produk: ";
    cin >> baru->stok;
    cout << "Masukkan kategori produk: ";
    cin.ignore();
    getline(cin, baru->kategori);
    cout << "Masukkan harga produk: ";
    cin >> baru->harga;
    baru->next = nullptr;
    baru->left = nullptr;
    baru->right = nullptr;

    if (kepalaProduk == nullptr)
    {
        kepalaProduk = baru;
    }
    else
    {
        Produk *temp = kepalaProduk;
        while (temp->next != nullptr)
        {
            temp = temp->next;
        }
        temp->next = baru;
    }

    root = insertTree(root, baru); // tambah ke tree

    cout << "Produk berhasil ditambahkan!" << endl;
    cout << "\nIngin menambahkan produk lagi? (Y/N) : ";
    string tmbhproduk;
    cin >> tmbhproduk;
    if (tmbhproduk == "y" || tmbhproduk == "Y")
    {
        tambahProduk();
    }
    else
    {
        cout << "\nTekan [ENTER] untuk kembali ke menu" << endl;
        cin.ignore();
        cin.get();
    }
}

// fungsi mengupdate data produk - linkedlist - O(n)
void updateProduk()
{
    system("CLS");
    int id;
    cout << "Masukkan ID produk yang ingin diupdate: ";
    cin >> id;
    Produk *temp = kepalaProduk;
    while (temp != nullptr)
    {
        if (temp->id == id)
        {
            cout << "Masukkan nama produk baru: ";
            cin.ignore();
            getline(cin, temp->nama);
            cout << "Masukkan stok produk baru: ";
            cin >> temp->stok;
            cout << "Masukkan kategori produk baru: ";
            cin.ignore();
            getline(cin, temp->kategori);
            cout << "Produk berhasil diupdate!" << endl;
            return;
        }
        temp = temp->next;
    }
    cout << "Produk dengan ID " << id << " tidak ditemukan." << endl;
    cout << "\nIngin mengupade produk lagi? (Y/N) : ";
    string updtproduk;
    cin >> updtproduk;
    if (updtproduk == "y" || updtproduk == "Y")
    {
        updateProduk();
    }
    else
    {
        cout << "\nTekan [ENTER] untuk kembali ke menu" << endl;
        cin.ignore();
        cin.get();
    }
}

// fungsi menghapus produk - linkedlist dan tree - O(n)
void hapusProduk()
{
    system("CLS");
    int id;
    cout << "Masukkan ID produk yang ingin dihapus: ";
    cin >> id;
    Produk *temp = kepalaProduk;
    Produk *prev = nullptr;
    while (temp != nullptr)
    {
        if (temp->id == id)
        {
            cout << "Apakah anda yakin ingin menghapus produk ini? (y/n): ";
            char konfirmasi;
            cin >> konfirmasi;
            if (konfirmasi == 'y' || konfirmasi == 'Y')
            {
                if (prev == nullptr)
                {
                    kepalaProduk = temp->next;
                }
                else
                {
                    prev->next = temp->next;
                }

                root = hapusTreeNode(root, temp); // hapus dari tree

                delete temp;
                cout << "Produk berhasil dihapus!" << endl;
            }
            else
            {
                cout << "Hapus dibatalkan." << endl;
            }
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    cout << "Produk dengan ID " << id << " tidak ditemukan." << endl;
    cout << "\nIngin menghapus produk lagi? (Y/N) : ";
    string hpsproduk;
    cin >> hpsproduk;
    if (hpsproduk == "ya" || hpsproduk == "Ya")
    {
        hapusProduk();
    }
    else
    {
        cout << "\nTekan [ENTER] untuk kembali ke menu" << endl;
        cin.ignore();
        cin.get();
    }
}

// fungsi mencari produk - algoritma searching (sequintial search) - O(n)
void cariProduk()
{
    system("CLS");
    char pilihan;
    cout << "Cari produk berdasarkan:" << endl;
    cout << "1. Nama Produk" << endl;
    cout << "2. ID Produk" << endl;
    cout << "Pilihan: ";
    cin >> pilihan;
    cin.ignore();

    switch (pilihan)
    {
    case '1':
    {
        string namaProduk;
        cout << "Masukkan nama produk: ";
        getline(cin, namaProduk);
        Produk *temp = kepalaProduk;
        bool ditemukan = false;
        while (temp != nullptr)
        {
            if (temp->nama == namaProduk)
            {
                cout << "ID: " << temp->id << ", Nama: " << temp->nama << ", Stok: " << temp->stok << ", Kategori: " << temp->kategori << endl;
                ditemukan = true;
                break;
            }
            temp = temp->next;
        }
        if (!ditemukan)
        {
            cout << "Produk dengan nama '" << namaProduk << "' tidak ditemukan." << endl;
        }
        break;
    }
    case '2':
    {
        int idProduk;
        cout << "Masukkan ID produk: ";
        cin >> idProduk;
        Produk *temp = kepalaProduk;
        bool ditemukan = false;
        while (temp != nullptr)
        {
            if (temp->id == idProduk)
            {
                cout << "ID: " << temp->id << ", Nama: " << temp->nama << ", Stok: " << temp->stok << ", Kategori: " << temp->kategori << endl;
                ditemukan = true;
                break;
            }
            temp = temp->next;
        }
        if (!ditemukan)
        {
            cout << "Produk dengan ID " << idProduk << " tidak ditemukan." << endl;
        }
        break;
    }
    default:
        cout << "Pilihan tidak valid." << endl;
        break;
    }

    cout << "\nTekan [ENTER] untuk kembali ke menu" << endl;
    cin.ignore();
    cin.get();
}

// fungsi menampilkan produk berdasarkan kategori produk - tree - O(n)
void kategoriProduk()
{
    system("CLS");
    string kategori;
    cout << "Masukkan kategori produk: ";
    cin.ignore();
    getline(cin, kategori);
    cout << "Produk dalam kategori " << kategori << ":" << endl;
    cout << "+-----------------+----------+----------------------+------------+------------+" << endl;
    cout << "| Kategori        | ID Produk| Nama Produk          | Stok Produk| Harga Produk|" << endl;
    cout << "+-----------------+----------+----------------------+------------+------------+" << endl;
    displayCategories(root, kategori);
    cout << "+-----------------+----------+----------------------+------------+------------+" << endl;
    cout << "\nTekan [ENTER] untuk kembali ke menu" << endl;
    cin.ignore();
    cin.get();
}

// fungsi menampilkan produk- linked list - O(n)
void tampilkanProduk()
{
    system("CLS");
    cout << "+----+------------------+-------+------------------+----------+" << endl;
    cout << "| ID |       Nama       | Stok  |    Kategori      |  Harga   |" << endl;
    cout << "+----+------------------+-------+------------------+----------+" << endl;

    Produk *temp = kepalaProduk;
    while (temp != nullptr)
    {
        cout << "| ";
        if (temp->id < 10)
            cout << " ";
        cout << temp->id << " ";

        cout << "| " << temp->nama;
        int namaPadding = 17 - temp->nama.length();
        for (int i = 0; i < namaPadding; i++)
        {
            cout << " ";
        }
        cout << " ";

        cout << "| ";
        if (temp->stok < 10)
            cout << " ";
        if (temp->stok < 100)
            cout << " ";
        if (temp->stok < 1000)
            cout << " ";
        cout << temp->stok << " ";

        cout << "| " << temp->kategori;
        int kategoriPadding = 17 - temp->kategori.length();
        for (int i = 0; i < kategoriPadding; i++)
        {
            cout << " ";
        }
        cout << " ";

        cout << "| ";
        if (temp->harga < 10000)
            cout << " ";
        if (temp->harga < 1000)
            cout << " ";
        if (temp->harga < 100)
            cout << " ";
        if (temp->harga < 10)
            cout << " ";
        cout << temp->harga << " |" << endl;

        temp = temp->next;
    }

    cout << "+----+------------------+-------+------------------+----------+" << endl;
    cout << "\nTekan [ENTER] untuk kembali ke menu" << endl;
    cin.ignore();
    cin.get();
}

// fungsi untuk mensorting produk berdasarkan nama secara ascending (Aa-Zz) - algoritma sorting (bubble sort) - O(n)
void sortingProduk()
{
    system("CLS");
    Produk *temp = kepalaProduk;
    int size = 0;
    while (temp != nullptr)
    {
        size++;
        temp = temp->next;
    }

    for (int i = 0; i < size - 1; i++)
    {
        temp = kepalaProduk;
        for (int j = 0; j < size - i - 1; j++)
        {
            Produk *next = temp->next;
            if (temp->nama > next->nama)
            {
                swap(temp->id, next->id);
                swap(temp->nama, next->nama);
                swap(temp->stok, next->stok);
                swap(temp->kategori, next->kategori);
                swap(temp->harga, next->harga);
            }
            temp = next;
        }
    }

    cout << "Daftar Produk setelah Sorting berdasarkan Nama (Ascending):" << endl;
    tampilkanProduk();
}

// fungsi menambahkan pelanggan - hash table dan collision handling metode chaining (linkedlist dalam array) - O(1) atau O(n)
void tambahPelanggan()
{
    system("CLS");
    Pelanggan *baru = new Pelanggan();
    baru->id = generatePelangganId();
    cout << "Masukkan nama pelanggan: ";
    cin.ignore();
    getline(cin, baru->nama);
    cout << "Masukkan alamat pelanggan: ";
    getline(cin, baru->alamat);
    cout << "Masukkan nomor telepon pelanggan: ";
    getline(cin, baru->no_telepon);
    baru->next = nullptr;

    if (kepalaPelanggan == nullptr)
    {
        kepalaPelanggan = baru;
        akhirPelanggan = baru;
    }
    else
    {
        akhirPelanggan->next = baru;
        akhirPelanggan = baru;
    }
    cout << "Pelanggan berhasil ditambahkan!" << endl;

    insertPelangganToHashTable(baru);
    cout << "\nIngin menambah pelanggan lagi? (Y/N) : ";
    string tmbhpelanggan;
    cin >> tmbhpelanggan;
    if (tmbhpelanggan == "ya" || tmbhpelanggan == "Ya")
    {
        tambahPelanggan();
    }
    else
    {
        cout << "\nTekan [ENTER] untuk kembali ke menu" << endl;
        cin.ignore();
        cin.get();
    }
}

// fungsi update data pelanggan - hash table dan collision handling metode chaining (linkedlist dalam array) - O(n)
void updatePelanggan()
{
    system("CLS");
    int id;
    cout << "Masukkan ID pelanggan yang ingin diupdate: ";
    cin >> id;
    Pelanggan *temp = kepalaPelanggan;
    while (temp != nullptr)
    {
        if (temp->id == id)
        {
            cout << "Masukkan nama pelanggan baru: ";
            cin.ignore();
            getline(cin, temp->nama);
            cout << "Masukkan alamat pelanggan baru: ";
            getline(cin, temp->alamat);
            cout << "Masukkan nomor telepon pelanggan baru: ";
            getline(cin, temp->no_telepon);
            cout << "Pelanggan berhasil diupdate!" << endl;
            return;
        }
        temp = temp->next;
    }
    cout << "Pelanggan dengan ID " << id << " tidak ditemukan." << endl;
    cout << "\nIngin mengupdate pelanggan lagi? (Y/N) : ";
    string updtpelanggan;
    cin >> updtpelanggan;
    if (updtpelanggan == "ya" || updtpelanggan == "Ya")
    {
        updatePelanggan();
    }
    else
    {
        cout << "\nTekan [ENTER] untuk kembali ke menu" << endl;
        cin.ignore();
        cin.get();
    }
}

// fungsi menghapus pelanggan - hash table - O(n)
void hapusPelanggan()
{
    system("CLS");
    int id;
    cout << "Masukkan ID pelanggan yang ingin dihapus: ";
    cin >> id;
    Pelanggan *temp = kepalaPelanggan;
    Pelanggan *prev = nullptr;
    while (temp != nullptr)
    {
        if (temp->id == id)
        {
            cout << "Apakah anda yakin ingin menghapus pelanggan ini? (y/n): ";
            char konfirmasi;
            cin >> konfirmasi;
            if (konfirmasi == 'y' || konfirmasi == 'Y')
            {
                if (prev == nullptr)
                {
                    kepalaPelanggan = temp->next;
                }
                else
                {
                    prev->next = temp->next;
                }
                if (temp == akhirPelanggan)
                {
                    akhirPelanggan = prev;
                }
                delete temp;
                cout << "Pelanggan berhasil dihapus!" << endl;
            }
            else
            {
                cout << "Hapus dibatalkan." << endl;
            }
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    cout << "Pelanggan dengan ID " << id << " tidak ditemukan." << endl;
    cout << "\nIngin menghapus produk lagi? (Y/N) : ";
    string hpspelanggan;
    cin >> hpspelanggan;
    if (hpspelanggan == "ya" || hpspelanggan == "Ya")
    {
        hapusPelanggan();
    }
    else
    {
        cout << "\nTekan [ENTER] untuk kembali ke menu" << endl;
        cin.ignore();
        cin.get();
    }
}

// fungsi mencari data pelanggan - algoritma searching (sequential search) - O(n)
void cariPelanggan()
{
    system("CLS");
    char pilihan;
    cout << "Cari pelanggan berdasarkan:" << endl;
    cout << "1. Nama Pelanggan" << endl;
    cout << "2. ID Pelanggan" << endl;
    cout << "Pilihan: ";
    cin >> pilihan;
    cin.ignore();

    switch (pilihan)
    {
    case '1':
    {
        string namaPelanggan;
        cout << "Masukkan nama pelanggan: ";
        getline(cin, namaPelanggan);
        int index = hashFunction(namaPelanggan);
        Hash *temp = hashTable[index];
        bool ditemukan = false;
        while (temp != nullptr)
        {
            if (temp->data->nama == namaPelanggan)
            {
                cout << "ID: " << temp->data->id << ", Nama: " << temp->data->nama << ", Alamat: " << temp->data->alamat << ", No. Telepon: " << temp->data->no_telepon << endl;
                ditemukan = true;
                break;
            }
            temp = temp->next;
        }
        if (!ditemukan)
        {
            cout << "Pelanggan dengan nama '" << namaPelanggan << "' tidak ditemukan." << endl;
        }
        break;
    }
    case '2':
    {
        int idPelanggan;
        cout << "Masukkan ID pelanggan: ";
        cin >> idPelanggan;
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            Hash *temp = hashTable[i];
            while (temp != nullptr)
            {
                if (temp->data->id == idPelanggan)
                {
                    cout << "ID: " << temp->data->id << ", Nama: " << temp->data->nama << ", Alamat: " << temp->data->alamat << ", No. Telepon: " << temp->data->no_telepon << endl;
                    return;
                }
                temp = temp->next;
            }
        }
        cout << "Pelanggan dengan ID " << idPelanggan << " tidak ditemukan." << endl;
        break;
    }
    default:
        cout << "Pilihan tidak valid." << endl;
        break;
    }

    cout << "\nTekan [ENTER] untuk kembali ke menu" << endl;
    cin.ignore();
    cin.get();
}

// fungsi menampilkan data pelanggan - O(n)
void tampilkanPelanggan()
{
    Pelanggan *temp = kepalaPelanggan;
    while (temp != nullptr)
    {
        cout << "ID: " << temp->id << ", Nama: " << temp->nama << ", Alamat: " << temp->alamat << ", No. Telepon: " << temp->no_telepon << endl;
        temp = temp->next;
    }
}

// fungsi untuk membantu memasukkan data pelanggan ke hash table - hash table - O(1)
void insertPelangganToHashTable(Pelanggan *pelanggan)
{
    int index = hashFunction(pelanggan->nama);
    Hash *newNode = new Hash();
    newNode->data = pelanggan;
    newNode->next = nullptr;

    if (hashTable[index] == nullptr)
    {
        hashTable[index] = newNode;
    }
    else
    {
        Hash *temp = hashTable[index];
        while (temp->next != nullptr)
        {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

// fungsi untuk mensorting pelanggan berdasarkan nama secara ascending (Aa-Zz) - algoritma sorting (bubble sort) - O(n^2)
void sortingPelanggan()
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        Hash *temp = hashTable[i];
        int size = 0;
        while (temp != nullptr)
        {
            size++;
            temp = temp->next;
        }

        temp = hashTable[i];
        for (int j = 0; j < size - 1; j++)
        {
            for (int k = 0; k < size - j - 1; k++)
            {
                Hash *next = temp->next;
                if (temp->data->nama > next->data->nama)
                {
                    swap(temp->data->id, next->data->id);
                    swap(temp->data->nama, next->data->nama);
                    swap(temp->data->alamat, next->data->alamat);
                    swap(temp->data->no_telepon, next->data->no_telepon);
                }
                temp = next;
            }
            temp = hashTable[i];
        }
    }

    cout << "Daftar Pelanggan setelah Sorting berdasarkan Nama (Ascending):" << endl;
    tampilkanPelangganSorted();
}

// fungsi untuk menampilkan data pelanggan yang disorting - O(n)
void tampilkanPelangganSorted()
{
    cout << "+----+------------------+------------------+---------------+" << endl;
    cout << "| ID |       Nama       |      Alamat      | No. Telepon   |" << endl;
    cout << "+----+------------------+------------------+---------------+" << endl;

    for (int i = 0; i < TABLE_SIZE; i++)
    {
        Hash *temp = hashTable[i];
        while (temp != nullptr)
        {
            cout << "| ";
            if (temp->data->id < 10)
                cout << " ";
            cout << temp->data->id << " ";

            cout << "| " << temp->data->nama;
            int namaPadding = 17 - temp->data->nama.length();
            for (int j = 0; j < namaPadding; j++)
            {
                cout << " ";
            }
            cout << " ";

            cout << "| " << temp->data->alamat;
            int alamatPadding = 17 - temp->data->alamat.length();
            for (int j = 0; j < alamatPadding; j++)
            {
                cout << " ";
            }
            cout << " ";

            cout << "| " << temp->data->no_telepon << " |" << endl;

            temp = temp->next;
        }
    }

    cout << "+----+------------------+------------------+---------------+" << endl;
    cout << "\nTekan [ENTER] untuk kembali ke menu" << endl;
    cin.ignore();
    cin.get();
}

// fungsi transaksi pembelian produk oleh pelanggan - queue - O(n+m)
void pembelianProduk()
{
    system("CLS");
    cout << "Pelanggan:" << endl;
    cout << "+----+------------------+------------------+---------------+" << endl;
    cout << "| ID |       Nama       |      Alamat      | No. Telepon   |" << endl;
    cout << "+----+------------------+------------------+---------------+" << endl;

    for (int i = 0; i < TABLE_SIZE; i++)
    {
        Hash *temp = hashTable[i];
        while (temp != nullptr)
        {
            cout << "| ";
            if (temp->data->id < 10)
                cout << " ";
            cout << temp->data->id << " ";

            cout << "| " << temp->data->nama;
            int namaPadding = 17 - temp->data->nama.length();
            for (int j = 0; j < namaPadding; j++)
            {
                cout << " ";
            }
            cout << " ";

            cout << "| " << temp->data->alamat;
            int alamatPadding = 17 - temp->data->alamat.length();
            for (int j = 0; j < alamatPadding; j++)
            {
                cout << " ";
            }
            cout << " ";

            cout << "| " << temp->data->no_telepon << " |" << endl;

            temp = temp->next;
        }
    }

    cout << "+----+------------------+------------------+---------------+" << endl;
    cout << endl;

    int idPelanggan;
    cout << "Masukkan ID Pelanggan: ";
    cin >> idPelanggan;

    Pelanggan *pelanggan = kepalaPelanggan;
    bool pelangganDitemukan = false;

    while (pelanggan != nullptr)
    {
        if (pelanggan->id == idPelanggan)
        {
            pelangganDitemukan = true;
            break;
        }
        pelanggan = pelanggan->next;
    }

    if (!pelangganDitemukan)
    {
        cout << "Pelanggan tidak ditemukan." << endl;
        return;
    }

    Pesanan *pesanan = new Pesanan();
    pesanan->idPesanan = generatePesananId();
    pesanan->idPelanggan = idPelanggan;
    pesanan->status = "Verifikasi";
    pesanan->detailPesanan = nullptr;
    pesanan->next = nullptr;

    DetailPesanan *kepalaPesananProduk = nullptr;
    DetailPesanan *akhirPesananProduk = nullptr;
    pesanan->detailPesanan = kepalaPesananProduk;

    cout << "Produk:" << endl;
    cout << "+----+------------------+-------+------------------+----------+" << endl;
    cout << "| ID |       Nama       | Stok  |    Kategori      |  Harga   |" << endl;
    cout << "+----+------------------+-------+------------------+----------+" << endl;

    Produk *temp = kepalaProduk;
    while (temp != nullptr)
    {
        cout << "| ";
        if (temp->id < 10)
            cout << " ";
        cout << temp->id << " ";

        cout << "| " << temp->nama;
        int namaPadding = 17 - temp->nama.length();
        for (int i = 0; i < namaPadding; i++)
        {
            cout << " ";
        }
        cout << " ";

        cout << "| ";
        if (temp->stok < 10)
            cout << " ";
        if (temp->stok < 100)
            cout << " ";
        if (temp->stok < 1000)
            cout << " ";
        cout << temp->stok << " ";

        cout << "| " << temp->kategori;
        int kategoriPadding = 17 - temp->kategori.length();
        for (int i = 0; i < kategoriPadding; i++)
        {
            cout << " ";
        }
        cout << " ";

        cout << "| ";
        if (temp->harga < 10000)
            cout << " ";
        if (temp->harga < 1000)
            cout << " ";
        if (temp->harga < 100)
            cout << " ";
        if (temp->harga < 10)
            cout << " ";
        cout << temp->harga << " |" << endl;

        temp = temp->next;
    }
    cout << "+----+------------------+-------+------------------+----------+" << endl;
    cout << endl;

    char lanjut;
    do
    {
        int idProduk;
        cout << "Masukkan ID produk: ";
        cin >> idProduk;
        Produk *produk = kepalaProduk;
        while (produk != nullptr)
        {
            if (produk->id == idProduk)
            {
                if (produk->stok > 0)
                {
                    int kuantitas;
                    cout << "Masukkan kuantitas: ";
                    cin >> kuantitas;
                    if (kuantitas <= produk->stok)
                    {
                        DetailPesanan *detailPesanan = new DetailPesanan();
                        detailPesanan->idProduk = idProduk;
                        detailPesanan->namaProduk = produk->nama;
                        detailPesanan->kuantitas = kuantitas;
                        detailPesanan->harga = produk->harga;
                        detailPesanan->next = nullptr;

                        if (kepalaPesananProduk == nullptr)
                        {
                            kepalaPesananProduk = detailPesanan;
                            akhirPesananProduk = detailPesanan;
                        }
                        else
                        {
                            akhirPesananProduk->next = detailPesanan;
                            akhirPesananProduk = detailPesanan;
                        }

                        produk->stok -= kuantitas;
                        updateNodeGraph(idProduk); // Panggil fungsi updateNodeGraph di sini
                        cout << "Produk berhasil ditambahkan ke pesanan." << endl;
                    }
                    else
                    {
                        cout << "Kuantitas melebihi stok produk." << endl;
                    }
                }
                else
                {
                    cout << "Stok produk habis." << endl;
                }
                break;
            }
            produk = produk->next;
        }
        if (produk == nullptr)
        {
            cout << "Produk tidak ditemukan." << endl;
        }

        cout << "Tambah produk lagi? (y/n): ";
        cin >> lanjut;
    } while (lanjut == 'y' || lanjut == 'Y');

    pesanan->detailPesanan = kepalaPesananProduk;
    enqueue(pesanan);
}

// fungsi verifikai pesanan dari transaksi pembelian produk - queue - O(n+m+k)
void verifikasiPesanan()
{
    system("CLS");
    Pesanan *pesanan = dequeue();
    if (pesanan == nullptr)
    {
        cout << "Tidak ada pesanan dalam antrian." << endl;
        return;
    }

    int idPelanggan = pesanan->idPelanggan;
    Pelanggan *pelanggan = kepalaPelanggan;
    while (pelanggan != nullptr)
    {
        if (pelanggan->id == idPelanggan)
        {
            break;
        }
        pelanggan = pelanggan->next;
    }

    cout << "+-------------+---------------+---------------+----------+" << endl;
    cout << "| ID Pesanan  | ID Pelanggan  |    Produk     |  Status  |" << endl;
    cout << "+-------------+---------------+---------------+----------+" << endl;
    cout << "| ";
    if (pesanan->idPesanan < 10)
        cout << " ";
    cout << pesanan->idPesanan << "          | ";
    if (pesanan->idPelanggan < 10)
        cout << " ";
    cout << pesanan->idPelanggan << "             | ";

    DetailPesanan *detailPesanan = pesanan->detailPesanan;
    string produkDipesan = "";
    while (detailPesanan != nullptr)
    {
        produkDipesan += detailPesanan->namaProduk + " x" + to_string(detailPesanan->kuantitas) + ", ";
        detailPesanan = detailPesanan->next;
    }
    cout << produkDipesan.substr(0, produkDipesan.length() - 2);

    cout << "| " << pesanan->status << " |" << endl;
    cout << "+-------------+---------------+---------------+----------+" << endl;

    cout << "\nPelanggan: " << pelanggan->nama << endl;

    int totalHarga = 0;
    cout << "Daftar Produk:" << endl;
    cout << "+----+------------------+----------+----------+" << endl;
    cout << "| ID |       Nama       | Kuantitas |  Harga   |" << endl;
    cout << "+----+------------------+----------+----------+" << endl;

    detailPesanan = pesanan->detailPesanan;
    while (detailPesanan != nullptr)
    {
        cout << "| ";
        if (detailPesanan->idProduk < 10)
            cout << " ";
        cout << detailPesanan->idProduk << " ";

        cout << "| " << detailPesanan->namaProduk;
        int namaPadding = 17 - detailPesanan->namaProduk.length();
        for (int i = 0; i < namaPadding; i++)
        {
            cout << " ";
        }
        cout << " ";

        cout << "| ";
        if (detailPesanan->kuantitas < 10)
            cout << " ";
        cout << detailPesanan->kuantitas << "        ";

        cout << "| ";
        if (detailPesanan->harga < 10000)
            cout << " ";
        if (detailPesanan->harga < 1000)
            cout << " ";
        if (detailPesanan->harga < 100)
            cout << " ";
        if (detailPesanan->harga < 10)
            cout << " ";
        cout << detailPesanan->harga * detailPesanan->kuantitas << " |" << endl;

        totalHarga += detailPesanan->harga * detailPesanan->kuantitas;
        detailPesanan = detailPesanan->next;
    }

    cout << "+----+------------------+----------+----------+" << endl;
    cout << "Total: " << totalHarga << endl;

    cout << "Pilih bank pembayaran:" << endl;
    cout << "1. Bank BCA" << endl;
    cout << "2. Bank Mandiri" << endl;
    cout << "3. Bank BRI" << endl;
    cout << "Pilihan: ";
    int pilihan;
    cin >> pilihan;

    string noRekening;
    cout << "Masukkan nomor rekening: ";
    cin >> noRekening;

    int hargaBayar;
    cout << "Masukkan harga yang dibayar: ";
    cin >> hargaBayar;

    int kembalian = 0;
    if (hargaBayar == totalHarga)
    {
        pesanan->status = "Selesai";
        pushRiwayatPesanan(pesanan);
        cout << "Pesanan telah diverifikasi dan berhasil dibayar." << endl;
    }
    else if (hargaBayar > totalHarga)
    {
        kembalian = hargaBayar - totalHarga;
        pesanan->status = "Selesai";
        pushRiwayatPesanan(pesanan);
        cout << "Pesanan telah diverifikasi dan berhasil dibayar." << endl;
        cout << "Kembalian: " << kembalian << endl;
    }
    else
    {
        cout << "Harga pembayaran tidak sesuai dengan total harga." << endl;
    }
}

// fungsi untuk push laporan atau riwayat pesanan - stack - O(!)
void pushRiwayatPesanan(Pesanan *pesanan)
{
    Stack *newNode = new Stack();
    newNode->data = pesanan;
    newNode->next = top;
    top = newNode;
}

// fungsi menampilkan produk yang sering dibeli - graph - O(n * m)
void produkSeringDibeli()
{
    system("CLS");
    cout << "+----+------------------+---------------+" << endl;
    cout << "| ID |       Nama       | Dibeli (Kali) |" << endl;
    cout << "+----+------------------+---------------+" << endl;

    for (int i = 0; i < 100; i++)
    {
        GraphNode *temp = graphProduk[i];
        while (temp != nullptr)
        {
            Produk *produk = kepalaProduk;
            while (produk != nullptr)
            {
                if (produk->id == temp->idProduk)
                {
                    cout << "| ";
                    if (produk->id < 10)
                        cout << " ";
                    cout << produk->id << " ";

                    cout << "| " << produk->nama;
                    int namaPadding = 17 - produk->nama.length();
                    for (int j = 0; j < namaPadding; j++)
                    {
                        cout << " ";
                    }
                    cout << " ";

                    cout << "| ";
                    if (temp->jumlahDibeli < 10)
                        cout << " ";
                    if (temp->jumlahDibeli < 100)
                        cout << " ";
                    if (temp->jumlahDibeli < 1000)
                        cout << " ";
                    cout << temp->jumlahDibeli << " |" << endl;
                    break;
                }
                produk = produk->next;
            }
            temp = temp->next;
        }
    }

    cout << "+----+------------------+---------------+" << endl;
    cout << "\nTekan [ENTER] untuk kembali ke menu" << endl;
    cin.ignore();
    cin.get();
}

// fungsi untuk pop hasil pesanan dari queue ke riwayat pesanan - stack - O(1)
Pesanan *popRiwayatPesanan()
{
    if (top == nullptr)
    {
        return nullptr;
    }
    Stack *temp = top;
    Pesanan *pesanan = temp->data;
    top = top->next;
    delete temp;
    return pesanan;
}

// fungsi menampilkan riwayat transaksi pesanan - stack - O(n)
void riwayatPesananPelanggan()
{
    system("CLS");
    cout << "+-------------+---------------+---------------+----------+" << endl;
    cout << "| ID Pesanan  | ID Pelanggan  |    Produk     |  Status  |" << endl;
    cout << "+-------------+---------------+---------------+----------+" << endl;

    Stack *temp = top;
    while (temp != nullptr)
    {
        Pesanan *pesanan = temp->data;
        cout << "| ";
        if (pesanan->idPesanan < 10)
            cout << " ";
        cout << pesanan->idPesanan << "          | ";
        if (pesanan->idPelanggan < 10)
            cout << " ";
        cout << pesanan->idPelanggan << "             | ";

        DetailPesanan *detailPesanan = pesanan->detailPesanan;
        string produkDipesan = "";
        while (detailPesanan != nullptr)
        {
            produkDipesan += detailPesanan->namaProduk + " x" + to_string(detailPesanan->kuantitas) + ", ";
            detailPesanan = detailPesanan->next;
        }
        cout << produkDipesan.substr(0, produkDipesan.length() - 2);

        cout << "| " << pesanan->status << " |" << endl;
        temp = temp->next;
    }

    cout << "+-------------+---------------+---------------+----------+" << endl;
    cout << "\nTekan [ENTER] untuk kembali ke menu" << endl;
    cin.ignore();
    cin.get();
}

// fungsi menampilkan menu - O(1)
void menu() 
{
    int pilihan;
    do
    {
        cout << "=== Menu Manajemen Toko Online ===" << endl;
        cout << "1. Dashboard Produk" << endl;
        cout << "2. Dashboard Pelanggan" << endl;
        cout << "3. Dashboard Transaksi" << endl;
        cout << "4. Keluar" << endl;
        cout << "Pilih menu: ";
        cin >> pilihan;

        switch (pilihan)
        {
        case 1:
            dashboardProduk();
            break;
        case 2:
            dashboardPelanggan();
            break;
        case 3:
            dashboardTransaksi();
            break;
        case 4:
            cout << "Terima kasih telah menggunakan aplikasi ini!" << endl;
            break;
        default:
            cout << "Pilihan tidak valid." << endl;
        }
    } while (pilihan != 4);
}

// fungsi menampilkan dashboard produk - O(1)
void dashboardProduk()
{
    system("CLS");
    int pilihan;
    do
    {
        cout << "=== Dashboard Produk ===" << endl;
        cout << "1. Tambah Produk" << endl;
        cout << "2. Update Produk" << endl;
        cout << "3. Hapus Produk" << endl;
        cout << "4. Cari Produk" << endl;
        cout << "5. Kategori Produk" << endl;
        cout << "6. Tampilkan dan Sorting Semua Produk" << endl;
        cout << "7. Kembali ke Menu Utama" << endl;
        cout << "Pilih menu: ";
        cin >> pilihan;

        switch (pilihan)
        {
        case 1:
            tambahProduk();
            break;
        case 2:
            updateProduk();
            break;
        case 3:
            hapusProduk();
            break;
        case 4:
            cariProduk();
            break;
        case 5:
            kategoriProduk();
            break;
        case 6:
            sortingProduk();
            break;
        case 7:
            return;
        default:
            cout << "Pilihan tidak valid." << endl;
        }
    } while (pilihan != 7);
}

// fungsi menampilkan dashboard pelanggan - O(1)
void dashboardPelanggan()
{
    int pilihan;
    do
    {
        cout << "=== Dashboard Pelanggan ===" << endl;
        cout << "1. Tambah Pelanggan" << endl;
        cout << "2. Update Pelanggan" << endl;
        cout << "3. Hapus Pelanggan" << endl;
        cout << "4. Cari Pelanggan" << endl;
        cout << "5. Tampilkan dan Sorting Pelanggan" << endl;
        cout << "6. Kembali ke Menu Utama" << endl;
        cout << "Pilih menu: ";
        cin >> pilihan;

        switch (pilihan)
        {
        case 1:
            tambahPelanggan();
            break;
        case 2:
            updatePelanggan();
            break;
        case 3:
            hapusPelanggan();
            break;
        case 4:
            cariPelanggan();
            break;
        case 5:
            sortingPelanggan();
            break;
        case 6:
            return;
        default:
            cout << "Pilihan tidak valid." << endl;
        }
    } while (pilihan != 7);
}

// fungsi menampilkan dashboard transaksi - O(1)
void dashboardTransaksi()
{
    int pilihan;
    do
    {
        cout << "=== Dashboard Transaksi ===" << endl;
        cout << "1. Pembelian Produk oleh Pelanggan" << endl;
        cout << "2. Verifikasi Pesanan Pelanggan" << endl;
        cout << "3. Riwayat Pesanan Pelanggan" << endl;
        cout << "4. Produk yang Sering Dibeli oleh Pelanggan" << endl;
        cout << "5. Kembali ke Menu Utama" << endl;
        cout << "Pilih menu: ";
        cin >> pilihan;

        switch (pilihan)
        {
        case 1:
            pembelianProduk();
            break;
        case 2:
            verifikasiPesanan();
            break;
        case 3:
            riwayatPesananPelanggan();
            break;
        case 4:
            produkSeringDibeli();
            break;
        case 5:
            return;
        default:
            cout << "Pilihan tidak valid." << endl;
        }
    } while (pilihan != 5);
}

int main()
{
    menu();
    return 0;
}