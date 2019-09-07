#include <iostream>
#include <iomanip>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <algorithm>


using namespace std;

const int N = 4000;


struct Record {
    char fio[30];
    unsigned short int sum;
    char date[10];
    char fio_adv[22];
};

struct Node {
    Record record;
    Node *next;
};

string prompt(const string &str) {
    cout << str;
    cout << "\n> ";
    string ans;
    cin >> ans;
    return ans;
}

int strcomp(const string &str1, const string &str2, int len = -1) {
    if (len == -1) {
        len = (int) str1.length();
    }
    for (int i = 0; i < len; ++i) {
        if (str1[i] == '\0' and str2[i] == '\0') {
            return 0;
        } else if (str1[i] == ' ' and str2[i] != ' ') {
            return -1;
        } else if (str1[i] != ' ' and str2[i] == ' ') {
            return 1;
        } else if (str1[i] < str2[i]) {
            return -1;
        } else if (str1[i] > str2[i]) {
            return 1;
        }
    }
    return 0;
}

int compare(const Record &record1, const Record &record2) {
    for (int i = 0; i < 22; ++i) {
        if (record1.fio_adv[i] > record2.fio_adv[i]) {
            return -1;
        } else if (record1.fio_adv[i] < record2.fio_adv[i]) {
            return 1;
        }
    }
    if (record1.sum > record2.sum) {
        return -1;
    } else if (record1.sum < record2.sum) {
        return 1;
    }
    return 0;
}

struct body {
    Node *head;
    Node *tail;
};

void MergeSort(Node *&S, int n) {
    int t, q, r, i, m;
    Node *a, *b, *k, *p;
    body c[2];
    a = S;
    b = S->next;
    k = a;
    p = b;
    int x = 0;
    while (p != NULL) {
        k->next = p->next;
        k = p;
        p = p->next;
    }
    t = 1;
    Node *temp1 = a, *temp2 = b;
    while (temp1 != NULL) {
        temp1 = temp1->next;
    }
    while (temp2 != NULL) {
        temp2 = temp2->next;
    }

    while (t < n) {
        c[0].tail = c[0].head = NULL;
        c[1].tail = c[1].head = NULL;
        i = 0;
        m = n;
        while (m > 0) {
            if (m >= t) q = t;
            else q = m;
            m = m - q;
            if (m >= t) r = t;
            else r = m;
            m = m - r;
            while (q != 0 && r != 0) {
                if (compare(a->record, b->record) > 0) {
                    if (c[i].tail == NULL) {
                        c[i].tail = c[i].head = a;
                    } else {
                        c[i].tail->next = a;
                        c[i].tail = a;
                    }
                    a = a->next;
                    q--;

                } else {

                    if (c[i].tail == NULL) {
                        c[i].tail = c[i].head = b;
                    } else {
                        c[i].tail->next = b;
                        c[i].tail = b;
                    }
                    b = b->next;
                    r--;

                }
            }
            while (q > 0) {
                if (c[i].tail == NULL) {
                    c[i].tail = c[i].head = a;
                } else {
                    c[i].tail->next = a;
                    c[i].tail = a;
                }
                a = a->next;
                q--;
            }
            while (r > 0) {
                if (c[i].tail == NULL) {
                    c[i].tail = c[i].head = b;
                } else {
                    c[i].tail->next = b;
                    c[i].tail = b;
                }
                b = b->next;
                r--;
            }
            i = 1 - i;
            x++;
        }
        a = c[0].head;
        b = c[1].head;
        t = 2 * t;
    }
    c[0].tail->next = NULL;
    S = c[0].head;
}

Node *load_to_memory() {
    Node *root = NULL;
    ifstream file("testBase3.dat", ios::binary);
    if (not file.is_open()) {
        return NULL;
    }

    for (int i = 0; i < N; ++i) {
        Record record;
        file.read((char *) &record, sizeof(Record));
        root = new Node{record, root};
    }
    file.close();
    return root;
}

void print_head() {
    std::cout << "Record  Fio                            Sum    Date       Fio adv\n";
}

void print_record(Record *record) {
    std::cout << "  " << record->fio
              << "  " << setw(5) << record->sum
              << "  " << record->date
              << "  " << record->fio_adv << "\n";
}

void show_list(Record *records[], int n = N) {
    int ind = 0;
    while (true) {
        print_head();
        for (int i = 0; i < 20; i++) {
            Record *record = records[ind + i];
            cout << "[" << setw(4) << ind + i + 1 << "]";
            print_record(record);

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

int quick_search(Record *arr[], const std::string &key) {
    int l = 0;
    int r = N - 1;
    while (l < r) {
        int m = (l + r) / 2;
        if (strcomp(arr[m]->fio_adv, key, 3) < 0) {
            l = m + 1;
        } else {
            r = m;
        }
    }
    if (strcomp(arr[r]->fio_adv, key, 3) == 0) {
        return r;
    }
    return -1;
}

void make_index_array(Record *arr[], Node *root, int n = N) {
    Node *p = root;
    for (int i = 0; i < n; i++) {
        arr[i] = &(p->record);
        p = p->next;
    }
}

struct Vertex {
    Record *data;
    Vertex *left;
    Vertex *right;
    int balance;
};


void dbd_add(Record *data, Vertex *&p) {
    static int vr = 1;
    static int hr = 1;
    if (!p) {
        p = new Vertex{data, NULL, NULL, 0};
        vr = 1;
    } else if (strcomp(data->date, p->data->date) < 0) {
        dbd_add(data, p->left);
        if (vr == 1) {
            if (p->balance == 0) {
                Vertex *q = p->left;
                p->left = q->right;
                q->right = p;
                p = q;
                q->balance = 1;
                vr = 0;
                hr = 1;
            } else {
                p->balance = 0;
                vr = 1;
                hr = 0;
            }
        } else {
            hr = 0;
        }
    } else if (strcomp(data->date, p->data->date) >= 0) {
        dbd_add(data, p->right);
        if (vr == 1) {
            p->balance = 1;
            hr = 1;
            vr = 0;
        } else if (hr == 1) {
            if (p->balance == 1) {
                Vertex *q = p->right;
                p->balance = 0;
                q->balance = 0;
                p->right = q->left;
                q->left = p;
                p = q;
                vr = 1;
                hr = 0;
            } else {
                hr = 0;
            }
        }
    }
}

void Print_tree(Vertex *p, int &i) {
    if (p) {
        Print_tree(p->left, i);
        std::cout << "[" << std::setw(4) << i++ << "]";
        print_record(p->data);
        Print_tree(p->right, i);
    }
}

void search_in_tree(Vertex *root, const string &key, int &i) {
    if (root) {
        if (strcomp(key, root->data->date) == -1) {
            search_in_tree(root->left, key, i);
        } else if (strcomp(key, root->data->date) == 1) {
            search_in_tree(root->right, key, i);
        } else if (strcomp(key, root->data->date) == 0) {
            search_in_tree(root->left, key, i);
            std::cout << "[" << std::setw(4) << i++ << "]";
            print_record(root->data);
            search_in_tree(root->right, key, i);
        }
    }
}

void rmtree(Vertex *root) {
    if (root) {
        rmtree(root->right);
        rmtree(root->left);
        delete root;
    }
}

void tree(Record *arr[], int n) {
    Vertex *root = NULL;
    for (int i = 0; i < n; ++i) {
        dbd_add(arr[i], root);
    }
    print_head();
    int i = 1;
    Print_tree(root, i);
    string key;
    do {
        key = prompt("Input search key (date), q - exit");
        print_head();
        i = 1;
        search_in_tree(root, key, i);
    } while (key[0] != 'q');
    rmtree(root);
}

void search(Record *arr[], int &ind, int &n) {
    string key;
    do {
        key = prompt("Input search key (3 characters of fio adv)");
    } while (key.length() != 3);
    ind = quick_search(arr, key);
    if (ind == -1) {
        cout << "Not found\n";
    } else {
        int i;
        for (i = ind + 1; strcomp(arr[i]->fio_adv, key, 3) == 0; ++i) {}
        n = i - ind;
        show_list(&arr[ind], n);
    }
}

int Med(int L, int R, double *p) {
    double SL = 0;
    for (int i = L; i <= R; i++) {
        SL = SL + p[i];
    }
    double SR = p[R];
    int m = R;
    while (SL >= SR) {
        m--;
        SL = SL - p[m];
        SR = SR + p[m];
    }
    return m;
}

void Fano(int L, int R, int k, double *p, int Length[], char c[][20]) {
    int m;
    if (L < R) {
        k++;
        m = Med(L, R, p);
        for (int i = L; i <= R; i++) {
            if (i <= m) {
                c[i][k] = '0';
                Length[i] = Length[i] + 1;
            } else {
                c[i][k] = '1';
                Length[i] = Length[i] + 1;
            }
        }
        Fano(L, m, k, p, Length, c);
        Fano(m + 1, R, k, p, Length, c);
    }
}

unordered_map<char, int> get_char_counts_from_file(const string &file_name, int &file_size, int n = N) {
    ifstream file(file_name, ios::binary);
    if (!file.is_open()) {
        throw runtime_error("Could not open file");
    }
    char ch_arr[sizeof(Record) * n];
    file.read((char *) ch_arr, sizeof(Record) * n);
    file.close();

    unordered_map<char, int> counter_map;
    file_size = 0;
    for (auto ch : ch_arr) {
        counter_map[ch]++;
        file_size++;
    }
    return counter_map;
}

vector<pair<double, char>> calc_probabilities(const unordered_map<char, int> &counter_map, int count) {
    vector<pair<double, char>> probabilities;
    probabilities.reserve(counter_map.size());
    for (auto i : counter_map) {
        probabilities.emplace_back(make_pair((double) i.second / count, i.first));
    }
    return probabilities;
}

void coding() {
    int file_size;
    unordered_map<char, int> counter_map;
    counter_map = get_char_counts_from_file("testBase3.dat", file_size);

    auto probabilities = calc_probabilities(counter_map, file_size);
    counter_map.clear();

    sort(probabilities.begin(), probabilities.end(), greater<pair<double, char>>());
    cout << "Probabil.  char\n";
    for (auto i : probabilities) {
        cout << fixed << i.first << " | " << i.second << '\n';
    }

    const int n = (int) probabilities.size();

    char c[n][20];
    int Length[n];
    for (auto &i : Length) {
        i = 0;
    }

    auto p = new double[n];
    for (int i = 0; i < n; ++i) {
        p[i] = probabilities[i].first;
    }

    double shen = 0;
    Fano(0, n - 1, -1, p, Length, c);
    cout << "\nShannon Code:\n";
    cout << "\nCh  Prob      Code\n";
    double avg_len = 0;
    double entropy = 0;
    for (int i = 0; i < n; i++) {
        avg_len += Length[i] * p[i];
        entropy -= p[i] * log2(p[i]);
        printf("%c | %.5lf | ", probabilities[i].second, p[i]);
        for (int j = 0; j < Length[i]; ++j) {
            printf("%c", c[i][j]);
        }
        cout << '\n';
        shen += p[i];
    }
    cout << "Average length = " << avg_len << '\n'
         << "Entropy = " << entropy << '\n'
         << "Average length < entropy + 1\n"
         << "N = " << n << "\n\n";

}


void mainloop(Record *unsorted_ind_array[], Record *sorted_ind_array[]) {
    int search_ind, search_n = -1;
    while (true) {
        std::string chose = prompt("1: Show unsorted list\n"
                                   "2: Show sorted list\n"
                                   "3: Search\n"
                                   "4: Tree\n"
                                   "5: Coding\n"
                                   "Any key: Exit");
        switch (chose[0]) {
            case '1':
                show_list(unsorted_ind_array);
                break;
            case '2':
                show_list(sorted_ind_array);
                break;
            case '3':
                search(sorted_ind_array, search_ind, search_n);
                break;
            case '4':
                if (search_n == -1) {
                    std::cout << "Please search first\n";
                } else {
                    tree(&sorted_ind_array[search_ind], search_n);
                }
                break;
            case '5':
                coding();
                break;
            default:
                return;
        }
    }
}


int main() {
    Node *root = load_to_memory();
    if (!root) {
        cout << "File not found" << endl;
        return 1;
    }
    Record *unsorted_ind_arr[N];
    Record *sorted_ind_arr[N];
    make_index_array(unsorted_ind_arr, root);
    MergeSort(root, N);
    make_index_array(sorted_ind_arr, root);
    mainloop(unsorted_ind_arr, sorted_ind_arr);
}
