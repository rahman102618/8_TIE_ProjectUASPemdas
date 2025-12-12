#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// ===================== STRUKTUR DATA & GLOBAL =====================
struct Prioritas {
    static const int Rendah = 1;
    static const int Sedang = 2;
    static const int Tinggi = 3;
};

struct Status {
    static const int Tunda = 1;
    static const int DalamProses = 2;
    static const int Selesai = 3;
};

struct Task {
    string nama;
    string deadline;
    int prioritas;
    int status;
};

Task listTugas[50];
int jumlah = 0;

// ===================== HELPER =====================
string teksPrioritas(int p) {
    if (p == Prioritas::Rendah) return "Rendah";
    if (p == Prioritas::Sedang) return "Sedang";
    return "Tinggi";
}

string teksStatus(int s) {
    if (s == Status::Tunda) return "Ditunda";
    if (s == Status::DalamProses) return "Dalam Proses";
    return "Selesai";
}

int parsePrioritas(string p) {
    if (p == "Tinggi") return Prioritas::Tinggi;
    if (p == "Sedang") return Prioritas::Sedang;
    return Prioritas::Rendah;
}

int parseStatus(string s) {
    if (s == "Selesai") return Status::Selesai;
    if (s == "Dalam Proses") return Status::DalamProses;
    return Status::Tunda;
}

string stringToRendah(string str) {
    string temp = str;
    for (int i = 0; i < temp.length(); i++) {
        temp[i] = tolower(temp[i]); // merubah huruf besar ke kecil
    }
    return temp;
}

// ===================== FUNGSI REKURSIF =====================
int hitungSelesai(int i) {
    if (i == jumlah) return 0;
    return (listTugas[i].status == Status::Selesai ? 1 : 0) + hitungSelesai(i + 1); // ? = if  &   : = else
}

// ===================== FUNGSI POINTER =====================
void tandaiSelesai(Task* t) {
    t->status = Status::Selesai;
}

// ===================== MANAJEMEN FILE =====================
void simpanFile() {
    ofstream file("To Do List.txt");
    file << "No.|Tugas|Deadline|Prioritas|Status" << endl;
    for (int i = 0; i < jumlah; i++) {
        file << (i + 1) << "|"
             << listTugas[i].nama << "|"
             << listTugas[i].deadline << "|"
             << teksPrioritas(listTugas[i].prioritas) << "|"
             << teksStatus(listTugas[i].status) << endl;
    }
    file.close();
    cout << "Data disimpan.\n";
}

void muatFile() {
    ifstream file("To Do List.txt");
    if (!file.is_open()) return;

    jumlah = 0;
    string line;

    getline(file, line); //melewati header

    while (getline(file, line)) {
        int p1 = line.find('|');
        int p2 = line.find('|', p1 + 1);
        int p3 = line.find('|', p2 + 1);
        int p4 = line.find('|', p3 + 1);

        if (p1 == -1) continue;

        listTugas[jumlah].nama = line.substr(p1 + 1, p2 - (p1 + 1));// ambil string nama
        listTugas[jumlah].deadline = line.substr(p2 + 1, p3 - (p2 + 1));// ambil string deadline
        
        string strPrio = line.substr(p3 + 1, p4 - (p3 + 1)); // ambil string prioritas
        listTugas[jumlah].prioritas = parsePrioritas(strPrio); // rubah string ke int

        string strStatus = line.substr(p4 + 1); // ambil string status
        listTugas[jumlah].status = parseStatus(strStatus); // rubah string ke int

        jumlah++;
    }
    file.close();
}

// ===================== FITUR TAMBAH =====================
void tambahTugas() {
    if (jumlah >= 50) {
        cout << "List penuh!\n";
        return;
    }

    cout << "Nama tugas: ";
    cin.ignore();
    getline(cin, listTugas[jumlah].nama);

    cout << "Deadline: ";
    getline(cin, listTugas[jumlah].deadline);

    cout << "Prioritas (1=Rendah, 2=Sedang, 3=Tinggi): ";
    int p;
    cin >> p;
    listTugas[jumlah].prioritas = p;

    listTugas[jumlah].status = Status::Tunda;

    jumlah++;
    simpanFile();
    cout << "Tugas ditambahkan!\n";
}

// ===================== FITUR TAMPIL =====================
void tampilTugas() {
    if (jumlah == 0) {
        cout << "Belum ada tugas.\n";
        return;
    }

    cout << "\n=== DAFTAR TUGAS ===\n";
    cout << "No.|Tugas|Deadline|Prioritas|Status" << endl;
    for (int i = 0; i < jumlah; i++) {
        cout << i + 1 << ". " << listTugas[i].nama
             << " [Deadline: " << listTugas[i].deadline << "] "
             << " [Prioritas: " << teksPrioritas(listTugas[i].prioritas) << "] "
             << " -> [" << teksStatus(listTugas[i].status) << "]\n";
    }
}

// ===================== FITUR HAPUS =====================
void hapusTugas(int index) {
    for (int i = index; i < jumlah - 1; i++) {
        listTugas[i] = listTugas[i + 1];
    }
    jumlah--;
    simpanFile();
    cout << "Tugas berhasil dihapus.\n";
}

void menuHapus() {
    tampilTugas();
    if (jumlah == 0) return;

    int pilih;
    cout << "Nomor tugas yang DIHAPUS: ";
    cin >> pilih;

    if (pilih < 1 || pilih > jumlah) {
        cout << "Nomor tidak valid!\n";
        return;
    }

    hapusTugas(pilih - 1);
}

// ===================== FITUR EDIT (SWITCH CASE) =====================
void editTugas() {
    tampilTugas();
    if (jumlah == 0) return;

    int pilih;
    cout << "\nNomor tugas yang ingin diedit: ";
    cin >> pilih;

    if (pilih < 1 || pilih > jumlah) {
        cout << "Nomor tidak valid!\n";
        return;
    }

    Task &t = listTugas[pilih - 1]; 
    int opsi;

    do {
        cout << "\n-----------------------------------\n";
        cout << " SEDANG MENGEDIT: " << t.nama << "\n";
        cout << "-----------------------------------\n";
        cout << "1. Ubah Nama\n";
        cout << "2. Ubah Deadline\n";
        cout << "3. Ubah Prioritas (" << teksPrioritas(t.prioritas) << ")\n";
        cout << "4. Ubah Status (" << teksStatus(t.status) << ")\n";
        cout << "0. Selesai & Simpan\n";
        cout << "Pilihan: ";
        cin >> opsi;

        switch (opsi) {
            case 1:
                cout << "Nama baru: ";
                cin.ignore(); 
                getline(cin, t.nama);
                break;
            case 2:
                cout << "Deadline baru: ";
                cin.ignore();
                getline(cin, t.deadline);
                break;
            case 3:
                cout << "Prioritas baru (1=Rendah, 2=Sedang, 3=Tinggi): ";
                cin >> t.prioritas;
                break;
            case 4:
                cout << "Status baru (1=Ditunda, 2=Dalam Proses, 3=Selesai): ";
                cin >> t.status;
                break;
            case 0:
                cout << "Keluar dari menu edit...\n";
                break;
            default:
                cout << "Pilihan tidak valid!\n";
        }

        if (opsi != 0) cout << ">> Data berhasil diperbarui.\n";

    } while (opsi != 0);

    simpanFile();
    cout << "Semua perubahan tersimpan ke file.\n";
}

// ===================== FITUR PENCARIAN (CASE INSENSITIVE) =====================
void cariTugas() {
    if (jumlah == 0) {
        cout << "Belum ada tugas.\n";
        return;
    }

    cin.ignore();
    string key;
    cout << "Cari tugas (nama): ";
    getline(cin, key);

    string keyRendaher = stringToRendah(key);

    cout << "\nHasil pencarian '" << key << "':\n";
    bool ketemu = false;

    for (int i = 0; i < jumlah; i++) {
        string namaRendaher = stringToRendah(listTugas[i].nama);

        if (namaRendaher.find(keyRendaher) != string::npos) {
            cout << "- " << listTugas[i].nama 
                 << " [" << teksStatus(listTugas[i].status) << "]\n";
            ketemu = true;
        }
    }

    if (!ketemu) cout << "Tidak ada tugas yang cocok.\n";
}

// ===================== FITUR ARSIP =====================
void arsipkan() {
    ofstream arsip("arsip.txt", ios::app);
    Task baru[50];
    int jBaru = 0;

    for (int i = 0; i < jumlah; i++) {
        if (listTugas[i].status == Status::Selesai) {
            arsip << listTugas[i].nama << "|"
                  << listTugas[i].deadline << "|"
                  << teksPrioritas(listTugas[i].prioritas) << endl;
        } else {
            baru[jBaru++] = listTugas[i];
        }
    }

    arsip.close();

    jumlah = jBaru;
    for (int i = 0; i < jumlah; i++) listTugas[i] = baru[i];

    simpanFile();
    cout << "Semua tugas selesai telah DIARSIPKAN.\n";
}

// ===================== MENU UTAMA =====================
void menuSelesai();

void menu() {
    int pilih;
    do {
        cout << "\n=== TO-DO LIST ===\n";
        cout << "1. Tambah Tugas\n";
        cout << "2. Tampilkan Tugas\n";
        cout << "3. Tandai Selesai (Pointer)\n";
        cout << "4. Hitung Tugas Selesai (Rekursif)\n";
        cout << "5. Hapus Tugas\n";
        cout << "6. Edit Tugas\n";
        cout << "7. Cari Tugas\n";
        cout << "8. Arsipkan Tugas Selesai\n";
        cout << "9. Keluar\n";
        cout << "Pilihan: ";
        cin >> pilih;

        switch (pilih) {
            case 1: tambahTugas(); break;
            case 2: tampilTugas(); break;
            case 3: menuSelesai(); break;
            case 4: cout << "Tugas selesai: " << hitungSelesai(0) << endl; break;
            case 5: menuHapus(); break;
            case 6: editTugas(); break;
            case 7: cariTugas(); break;
            case 8: arsipkan(); break;
            case 9: cout << "Keluar...\n"; break;
            default: cout << "Pilihan tidak valid!\n";
        }
    } while (pilih != 9);
}

void menuSelesai() {
    tampilTugas();
    int pilih;
    cout << "Pilih nomor tugas: ";
    cin >> pilih;

    if (pilih < 1 || pilih > jumlah) {
        cout << "Nomor salah!\n";
        return;
    }

    tandaiSelesai(&listTugas[pilih - 1]);
    simpanFile();
    cout << "Tugas selesai.\n";
}

int main() {
    muatFile();
    menu();
    return 0;

}
