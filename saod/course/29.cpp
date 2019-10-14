#include <iostream>  // ����-����� �� �������
#include <iomanip>  // �������������� ������ �� �������
#include <fstream>  // ��� ������ � �������


using namespace std;

const int N = 4000;  // ������ ���� ������

// ��������� ��
struct Record {
    char fio[32];
    char street[18];
    short int home;
    short int appartament;
    char date[10];
};

// ��������� ��� ����, ����� ����������� ��������� ������� ������
struct Node {
    Record record;
    Node *next;
};

// ������� ��� ��������� ������ ����������� (�� �����������)
// ����� �������� ������ ����������� � > �� ���-������ ������
string prompt(const string &str) {
    cout << str;
    cout << "\n> ";
    string ans;
    cin >> ans;
    return ans;
}

// �������� �� � ����� � ������������ ������ (� ������� ������)
Node *load_to_memory() {
    ifstream file("testBase4.dat", ios::binary);
    if (not file.is_open()) {  // ���� �� ������� ������� ��, ���������� 0
        return NULL;
    }

    Node *root = NULL;
    for (int i = 0; i < N; ++i) {
        Record record;
        file.read((char *) &record, sizeof(Record));
        root = new Node{record, root};
    }
    file.close();
    return root;
}

// �������� ���������� �������
void make_index_array(Record *arr[], Node *root, int n = N) {
    Node *p = root;
    for (int i = 0; i < n; i++) {
        arr[i] = &(p->record);
        p = p->next;
    }
}

// ��������� ���� "�������" (��������) ��� ����������
// ���������� -1, ���� record1 > record2, 1, ���� ������
int compare(const Record &record1, const Record &record2) {
    for (int i = 0; i < 18; ++i) {  // � ����� ������������ ����� �����������
        if (record1.street[i] > record2.street[i]) {
            return -1;
        } else if (record1.street[i] < record2.street[i]) {
            return 1;
        }
    }
    if (record1.home > record2.home) {  // ������������ ������ �����
        return -1;
    } else if (record1.home < record2.home) {
        return 1;
    }
    return 0;
}

void MakeHeap(Record *array[], int l, int r) {
    Record *x = array[l];
    int i = l;
    while (true) {
        int j = i * 2;
        if (j >= r) {
            break;
        }
        if (j < r) {
            if (compare(*array[j + 1], *array[j]) != 1) {
                ++j;
            }
        }
        if (compare(*x, *array[j]) != 1) {
            break;
        }
        array[i] = array[j];
        i = j;
    }
    array[i] = x;
}

void HeapSort(Record *array[], int n) {
    int l = (n - 1) / 2;
    while (l >= 0) {
        MakeHeap(array, l, n);
        --l;
    }
    int r = n - 1;
    while (r > 0) {
        Record *temp = array[0];
        array[0] = array[r];
        array[r] = temp;
        r--;
        MakeHeap(array, 0, r);
    }
}

// ���������� ���������� �������
void print_head() {
    cout << "Record Full Name                        Street          Home  Apt  Date\n";
}

// ����� ����� "������"
void print_record(Record *record, int i) {
    cout << "[" << setw(4) << i << "] "
         << record->fio
         << "  " << record->street
         << "  " << record->home
         << "  " << setw(3) << record->appartament
         << "  " << record->date << "\n";
}

// ����� 20 "�������" � ��������� ������ ������������
void show_list(Record *ind_arr[], int n = N) {
    int ind = 0;
    while (true) {
        system("CLS");  // ������ ������
        print_head();
        for (int i = 0; i < 20; i++) {  // ����� 20 �������
            Record *record = ind_arr[ind + i];
            print_record(record, ind + i + 1);

        }
        string chose = prompt("w: Next page\t"
                              "q: Last page\t"
                              "e: Skip 10 next pages\n"
                              "s: Prev page\t"
                              "a: First page\t"
                              "d: Skip 10 prev pages\n"
                              "Any key: Exit");
        switch (chose[0]) {
            case 'w':
                ind += 20;
                break;
            case 's':
                ind -= 20;
                break;
            case 'a':
                ind = 0;
                break;
            case 'q':
                ind = n - 20;
                break;
            case 'd':
                ind -= 200;
                break;
            case 'e':
                ind += 200;
                break;
            default:
                return;
        }
        if (ind < 0) {
            ind = 0;
        } else if (ind > n - 20) {
            ind = n - 20;
        }
    }
}

// �������� ���� (������� ���� ���������)
void mainloop(Record *unsorted_ind_array[], Record *sorted_ind_array[]) {
    while (true) {
        string chose = prompt("1: Show unsorted list\n"
                              "2: Show sorted list\n"
                              "Any key: Exit");
        switch (chose[0]) {
            case '1':
                show_list(unsorted_ind_array);
                break;
            case '2':
                show_list(sorted_ind_array);
                break;
            default:
                return;
        }
    }
}

int main() {
    Node *root = load_to_memory();  // �������� �� � ������� ������
    if (!root) {  // ���� �� ������� ���������, ������� ������ � ��������� ���������
        cout << "File not found" << endl;
        return 1;
    }
    Record *unsorted_ind_arr[N];
    Record *sorted_ind_arr[N];
    make_index_array(unsorted_ind_arr, root);  // ������ 2 ��������� ������� (���� ���
    make_index_array(sorted_ind_arr, root);    // �����������������, ������ ��� ����������) ��� �������
                                               // ��������� �� ��������
    HeapSort(sorted_ind_arr, N);  // ���������� ���������� �������
    mainloop(unsorted_ind_arr, sorted_ind_arr);  // ����� �������� ����
}
