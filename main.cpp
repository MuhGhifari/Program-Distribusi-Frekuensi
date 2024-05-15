#include <fstream>
#include <vector>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <string>
#include <algorithm>

using namespace std;

void printTabel(const string judulTabel, const vector<vector<string>>& tabel, ofstream& output);

int main() {
  string str;
  vector<double> input;
  vector<vector<string>> tabelDeskriptif, tabelDistribusi;
  ifstream fileInput("input.txt");

  if (!fileInput) {
    cerr << "[\x1b[31mERROR\x1b[0m] : file 'input.csv' tidak terdeteksi!" << endl;
		exit(1);
  }

  while (getline(fileInput, str)) {
    double nilaiInput;
    try {
      nilaiInput = stod(str);
    }
    catch (const invalid_argument& e) {
      cerr << "Error: " << e.what() << endl;
      continue;
    }

    input.push_back(nilaiInput);
  }
  fileInput.close();

  double rata2, varians, standarDeviasi, standarError;
  double n = input.size();

  for (size_t i = 0; i < n; i++) {
    rata2 += input[i];
  }
  rata2 = rata2 / n;

  for (size_t i = 0; i < n; i++) {
    varians += pow((input[i] - rata2), 2);
  }

  if (n <= 30) {
    varians = varians / (n - 1);
  } else {
    varians = varians / n;
  }

  standarDeviasi = sqrt(varians);
  standarError = standarDeviasi / sqrt(n);

  tabelDeskriptif = {
    {"Rata-Rata", to_string(rata2)},
    {"Varians", to_string(varians)},
    {"Simpangan Baku", to_string(standarDeviasi)},
    {"Standar Error", to_string(standarError)}
  };

  sort(input.begin(), input.end());
  auto max = max_element(input.begin(), input.end());
  auto min = min_element(input.begin(), input.end());
  double jangkauan = *max - *min;
  double kelas = ceil(1 + (3.3 * log10(n)));
  int interval = ceil(jangkauan / kelas);
  
  cout << "n\t\t: " << n << endl;
  cout << "max\t\t: " << *max << endl;
  cout << "min\t\t: " << *min << endl;
  cout << "jangkauan\t: " << jangkauan << endl;
  cout << "kelas\t\t: " << kelas << endl;
  cout << "interval\t: " << interval << endl;

  vector<int> frekuensi(kelas, 0);
  vector<string> ketFrekuensi;

  int awalan = *min;
  int indeksInput = 0;
  for (size_t i = 0; i < kelas; i++) {
    int akhiran = awalan + interval;
    for (size_t j = indeksInput; j < input.size(); j++) {
      if (input[j] < akhiran) {
        frekuensi[i]++;
        indeksInput++;
      }
    }
    str  = to_string(awalan) + "-" + to_string(akhiran - 1);
    ketFrekuensi.push_back(str);
    awalan = akhiran;
  }
  tabelDistribusi.push_back({"Jangkauan", "Frekuensi"});
  for (size_t i = 0; i < kelas; i++) {
    vector<string> baris;
    baris.push_back(ketFrekuensi[i]);
    baris.push_back(to_string(frekuensi[i]));
    tabelDistribusi.push_back(baris);
  }

  ofstream outputDeskriptif, outputDistribusi;
  outputDeskriptif.open("Tabel Deskriptif.txt");
  outputDistribusi.open("Tabel Distribusi Frekuensi.txt");

  printTabel("Tabel Deskriptif", tabelDeskriptif, outputDeskriptif);
  printTabel("Tabel Distribusi Frekuensi", tabelDistribusi, outputDistribusi);
  
  return 0;
}

void printTabel(const string judulTabel, const vector<vector<string>>& tabel, ofstream& output) {
  vector<size_t> lebarKolom(tabel[1].size(), 0);

  for (const auto& baris : tabel) {
    for (size_t i = 0; i < baris.size(); ++i) {
      lebarKolom[i] = max(lebarKolom[i], baris[i].size());
    }
  }

  int lebarHeader = 0;
  for (size_t i = 0; i < lebarKolom.size(); i++) {
    lebarHeader += lebarKolom[i] + 2;
  }

  if (judulTabel.size() > lebarHeader) {
    int lebarTambahan = judulTabel.size() - lebarHeader;
    lebarKolom[0] += lebarTambahan;
  }

  for (size_t i = 0; i < lebarKolom.size(); ++i) {
    output << "-" << string(lebarKolom[i] + 2, '-');
  }
  output << "-" << endl;

  output << "| " << setw(lebarHeader) << left << judulTabel << "|" << endl;
  
  for (size_t i = 0; i < lebarKolom.size(); ++i) {
    output << "+" << string(lebarKolom[i] + 2, '-');
  }
  output << "+" << endl;

  for (const auto& baris : tabel) {
    for (size_t i = 0; i < baris.size(); ++i) {
      output << "| " << setw(lebarKolom[i] + 1) << left << baris[i];
    }
    output << "|" << endl;
  }

  for (size_t i = 0; i < lebarKolom.size(); ++i) {
    output << "+"<< string(lebarKolom[i] + 2, '-');
  }
  output << "+";
  output.close();
}