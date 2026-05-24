#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

// STRUCT TRANSACTION
struct Transaction {
    int id;
    char type[20];
    char category[30];
    int amount;
    char description[100];
};

// LINKED LIST
struct Node {
    Transaction data;
    Node* next;
};

Node* head = NULL;

// STACK (UNDO TRANSAKSI)
struct StackNode {
    Transaction data;
    StackNode* next;
};

StackNode* topStack = NULL;

// PUSH STACK
void pushStack(Transaction t) {

    StackNode* newNode = new StackNode;

    newNode->data = t;
    newNode->next = topStack;

    topStack = newNode;
}

// POP STACK
Transaction popStack() {

    Transaction empty;
    empty.id = -1;

    if(topStack == NULL) {
        return empty;
    }

    StackNode* temp = topStack;

    Transaction data = temp->data;

    topStack = topStack->next;

    delete temp;

    return data;
}

// QUEUE (ANTRIAN APPROVAL)
struct QueueNode {
    Transaction data;
    QueueNode* next;
};

QueueNode* frontQueue = NULL;
QueueNode* rearQueue = NULL;

// ENQUEUE
void enqueue(Transaction t) {

    QueueNode* newNode = new QueueNode;

    newNode->data = t;
    newNode->next = NULL;

    if(rearQueue == NULL) {
        frontQueue = rearQueue = newNode;
    }
    else {
        rearQueue->next = newNode;
        rearQueue = newNode;
    }
}

// LIHAT QUEUE
void TampilQueue() {

    QueueNode* current = frontQueue;

    cout << "\n=== ANTRIAN APPROVAL ===\n";

    while(current != NULL) {

        cout << "ID : "
             << current->data.id
             << " | "
             << current->data.description
             << endl;

        current = current->next;
    }
}

// HASH TABLE
const int TABLE_SIZE = 26;

struct HashNode {
    char category[30];
    HashNode* next;
};

HashNode* hashTable[TABLE_SIZE];

// HASH FUNCTION
int hashFunction(char category[]) {

    int sum = 0;

    for(int i = 0; category[i] != '\0'; i++) {
        sum += category[i];
    }

    return sum % TABLE_SIZE;
}

// INSERT HASH
void insertCategory(char category[]) {

    int index = hashFunction(category);

    HashNode* newNode = new HashNode;

    strcpy(newNode->category, category);

    newNode->next = hashTable[index];

    hashTable[index] = newNode;
}

// LIHAT HASH TABLE
void TampilHashTable() {

    cout << "\n=== HASH TABLE ===\n";

    for(int i = 0; i < TABLE_SIZE; i++) {

        cout << i << "  : ";

        HashNode* current = hashTable[i];

        while(current != NULL) {

            cout << current->category << " -> ";

            current = current->next;
        }

        cout << "NULL\n";
    }
}

// BINARY SEARCH TREE
struct TreeNode {
    Transaction data;
    TreeNode* left;
    TreeNode* right;
};

TreeNode* root = NULL;

// INSERT BST
TreeNode* insertBST(TreeNode* root, Transaction t) {

    if(root == NULL) {

        TreeNode* newNode = new TreeNode;

        newNode->data = t;
        newNode->left = NULL;
        newNode->right = NULL;

        return newNode;
    }

    if(t.id < root->data.id) {
        root->left = insertBST(root->left, t);
    }
    else {
        root->right = insertBST(root->right, t);
    }

    return root;
}

// INORDER BST
void inorder(TreeNode* root) {

    if(root != NULL) {

        inorder(root->left);

        cout << "ID : "
             << root->data.id
             << " | "
             << root->data.description
             << endl;

        inorder(root->right);
    }
}

void preorder(TreeNode* root) {

    if(root != NULL) {

        cout << "ID : "
             << root->data.id
             << " | "
             << root->data.description
             << endl;

        preorder(root->left);

        preorder(root->right);
    }
}

void postorder(TreeNode* root) {

    if(root != NULL) {

        postorder(root->left);

        postorder(root->right);

        cout << "ID : "
             << root->data.id
             << " | "
             << root->data.description
             << endl;
    }
}


// GRAPH
const int MAX_MEMBER = 5;

char members[MAX_MEMBER][30] = {
    "Ketua",
    "Bendahara",
    "Sekretaris",
    "Acara",
    "Konsumsi"
};

int graph[MAX_MEMBER][MAX_MEMBER] = {

    {0,1,1,0,0},
    {1,0,1,1,1},
    {1,1,0,0,0},
    {0,1,0,0,1},
    {0,1,0,1,0}
};

// Tampilkan GRAPH
void TampilGraph() {

    cout << "\n=== GRAPH ANGGOTA ===\n";

    for(int i = 0; i < MAX_MEMBER; i++) {

        cout << members[i] << " --> ";

        bool first = true;

        for(int j = 0; j < MAX_MEMBER; j++) {

            if(graph[i][j] == 1) {

                if(!first) {
                    cout << " + ";
                }

                cout << members[j];

                first = false;
            }
        }

        cout << endl;
    }
}

// SAVE CSV
void SimpanKeCSV(Transaction t) {

    ofstream file("cashflow.csv", ios::app);

    file << t.id << ","
         << t.type << ","
         << t.category << ","
         << t.amount << ","
         << t.description << endl;

    file.close();
}

// TAMBAH TRANSAKSI
void TambahTransaksi() {

    Transaction t;

    cout << "\nID : ";
    cin >> t.id;

    cout << "Jenis Transaksi (Pemasukan/Pengeluaran) : ";
    cin >> t.type;

    cout << "Kategori Uang : ";
    cin >> t.category;

    cout << "Jumlah : ";
    cin >> t.amount;

    cin.ignore();

    cout << "Deskripsi : ";
    cin.getline(t.description, 100);

    // LINKED LIST
    Node* newNode = new Node;

    newNode->data = t;
    newNode->next = NULL;

    if(head == NULL) {
        head = newNode;
    }
    else {

        Node* current = head;

        while(current->next != NULL) {
            current = current->next;
        }

        current->next = newNode;
    }

    // STACK
    pushStack(t);

    // QUEUE
    enqueue(t);

    // HASH TABLE
    insertCategory(t.category);

    // BST
    root = insertBST(root, t);

    // CSV
    SimpanKeCSV(t);

    cout << "\nTransaksi berhasil ditambahkan!!!\n";
}

// LIHAT TRANSACTION
void TampilTransaksi() {

    Node* current = head;

    cout << "\n=== DATA TRANSAKSI ===\n";

    while(current != NULL) {

        cout << "ID         : "
             << current->data.id << endl;

        cout << "Jenis      : "
             << current->data.type << endl;

        cout << "Kategori   : "
             << current->data.category << endl;

        cout << "Jumlah     : "
             << current->data.amount << endl;

        cout << "Deskripsi  : "
             << current->data.description << endl;

        cout << "-------------------------\n";

        current = current->next;
    }
}

// SEARCHING
void SearchTransaksi() {

    int target;

    bool ditemukan = false;

    cout << "\nMasukkan ID yang dicari : ";
    cin >> target;

    Node* current = head;

    while(current != NULL) {

        if(current->data.id == target) {

            ditemukan = true;

            cout << "\n=== DATA DITEMUKAN ===\n";

            cout << "ID         : "
                 << current->data.id
                 << endl;

            cout << "Jenis      : "
                 << current->data.type
                 << endl;

            cout << "Kategori   : "
                 << current->data.category
                 << endl;

            cout << "Jumlah     : "
                 << current->data.amount
                 << endl;

            cout << "Deskripsi  : "
                 << current->data.description
                 << endl;

            cout << "----------------------\n";
        }

        current = current->next;
    }

    if(!ditemukan) {

        cout << "\nData tidak ditemukan!!!\n";
    }
}

// SORTING (BUBBLE SORT)
void PengurutanTransaksi() {

    if(head == NULL) {
        return;
    }

    bool swapped;

    do {

        swapped = false;

        Node* current = head;

        while(current->next != NULL) {

            if(current->data.amount >
               current->next->data.amount) {

                Transaction temp = current->data;

                current->data = current->next->data;

                current->next->data = temp;

                swapped = true;
            }

            current = current->next;
        }

    } while(swapped);

    cout << "\nData berhasil diurutkan!!!\n";
}

// REKURSIF
int recursiveTotal(Node* current) {

    if(current == NULL) {
        return 0;
    }

    return current->data.amount
           + recursiveTotal(current->next);
}

// REWRITE CSV
void rewriteCSV() {

    ofstream file("cashflow.csv");

    Node* current = head;

    while(current != NULL) {

        file << current->data.id << ","
             << current->data.type << ","
             << current->data.category << ","
             << current->data.amount << ","
             << current->data.description
             << endl;

        current = current->next;
    }

    file.close();
}

// REBUILD QUEUE   
void rebuildQueue() {

    // RESET QUEUE
    frontQueue = NULL;
    rearQueue = NULL;

    Node* current = head;

    // ISI ULANG DARI LINKED LIST
    while(current != NULL) {

        enqueue(current->data);

        current = current->next;
    }
}

// REBUILD HASH TABLE ()
void rebuildHashTable() {

    // RESET HASH TABLE
    for(int i = 0; i < TABLE_SIZE; i++) {

        hashTable[i] = NULL;
    }

    Node* current = head;

    // ISI ULANG DARI LINKED LIST
    while(current != NULL) {

        insertCategory(current->data.category);

        current = current->next;
    }
}

//REBUILD BST
void rebuildBST() {

    // RESET ROOT
    root = NULL;

    Node* current = head;

    // BUILD ULANG DARI LINKED LIST
    while(current != NULL) {

        root = insertBST(root, current->data);

        current = current->next;
    }
}

// UNDO TRANSAKSI
void UndoTransaksi() {

    Transaction t = popStack();

    if(t.id == -1) {

        cout << "\nTidak ada transaksi!!!\n";
        return;
    }

    if(head == NULL) {
        return;
    }

    if(head->next == NULL) {

        delete head;
        head = NULL;
        rewriteCSV();
        rebuildQueue();
        rebuildHashTable();
    }

    else {

        Node* current = head;

        while(current->next->next != NULL) {
            current = current->next;
        }

        delete current->next;

        current->next = NULL;

        rewriteCSV();

        rebuildQueue();
    }

    cout << "\nUndo berhasil!!!\n";
}


//LIHAT TOTAL CASH FLOW
void LihatTotalCashflow() {

    int pemasukan = 0;
    int pengeluaran = 0;

    Node* current = head;

    while(current != NULL) {

        // CEK JENIS TRANSAKSI
        if(strcmp(current->data.type, "pemasukan") == 0) {

            pemasukan += current->data.amount;
        }

        else if(strcmp(current->data.type, "pengeluaran") == 0) {

            pengeluaran += current->data.amount;
        }

        current = current->next;
    }

    int totalAkhir = pemasukan - pengeluaran;

    cout << "\n=== TOTAL KEUANGAN ===\n";

    cout << "Pemasukan            : "
         << pemasukan
         << endl;

    cout << "Pengeluaran          : "
         << pengeluaran
         << endl;

    cout << "Total Akhir Keuangan : "
         << totalAkhir
         << endl;
}

// MENU
void menu() {

    cout << "\n============================\n";
    cout << "     CASHFLOW ORGANIZER\n";
    cout << "============================\n";

    cout << "1. Tambah Transaksi\n";
    cout << "2. Lihat Transaksi\n";
    cout << "3. Cari Transaksi\n";
    cout << "4. Sorting Jumlah\n";
    cout << "5. Undo Transaksi\n";
    cout << "6. Lihat Queue\n";
    cout << "7. Lihat Hash Table\n";
    cout << "8. Lihat BST\n";
    cout << "9. Lihat Graph\n";
    cout << "10. Total Keuangan/Cashflow organisation\n";
    cout << "0. Exit\n";

    cout << "Pilih : ";
}


void SimpanDariCSV() {

    ifstream file("cashflow.csv");

    if(!file.is_open()) {
        return;
    }

    Transaction t;

    char line[300];

    while(file.getline(line, 300)) {

        char* token;

        // ID
        token = strtok(line, ",");
        t.id = atoi(token);

        // TYPE
        token = strtok(NULL, ",");
        strcpy(t.type, token);

        // CATEGORY
        token = strtok(NULL, ",");
        strcpy(t.category, token);

        // AMOUNT
        token = strtok(NULL, ",");
        t.amount = atoi(token);

        // DESCRIPTION
        token = strtok(NULL, ",");
        strcpy(t.description, token);

        // =========================
        // MASUKKAN KE LINKED LIST
        // =========================

        Node* newNode = new Node;

        newNode->data = t;
        newNode->next = NULL;

        if(head == NULL) {
            head = newNode;
        }
        else {

            Node* current = head;

            while(current->next != NULL) {
                current = current->next;
            }

            current->next = newNode;
        }

        // HASH TABLE
        insertCategory(t.category);

        // BST
        root = insertBST(root, t);

        // QUEUE
        enqueue(t);

        //STACK
        pushStack(t);
    }

    file.close();
}

// MAIN
int main() {

    SimpanDariCSV();

    int choice;
    char input[20];
    do {

        menu();

        cin >> input;

        choice = atoi(input);

        switch(choice) {

            case 1:
                TambahTransaksi();
                break;

            case 2:
                TampilTransaksi();
                break;

            case 3:
                SearchTransaksi();
                break;

            case 4:
                PengurutanTransaksi();
                break;

            case 5:
                UndoTransaksi();
                break;

            case 6:
                TampilQueue();
                break;

            case 7:
                TampilHashTable();
                break;

            case 8:

                cout << "\n=== INORDER ===\n";
                inorder(root);

                cout << "\n=== PREORDER ===\n";
                preorder(root);

                cout << "\n=== POSTORDER ===\n";
                postorder(root);

                break;

            case 9:
                TampilGraph();
                break;

            case 10:

            LihatTotalCashflow();

            break;

            case 0:

                cout << "\nProgram selesai...\n";

                break;

            default:

                cout << "\nMenu tidak tersedia!!!\n";
        }

    } while(choice != 0);

    return 0;
}