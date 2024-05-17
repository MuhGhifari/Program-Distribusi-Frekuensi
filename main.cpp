#include <fstream>
#include <vector>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <string>
#include <algorithm>
#include <sstream>

using namespace std;

string formatDesimal(const float &angka, const int &digit);
void printTabel(const string judulTabel, const vector<vector<string>>& tabel, ofstream& output);

int main() {
  string str;
  vector<double> input;
  vector<vector<string>> tabelDeskriptif, tabelDistribusi;
  ifstream fileInput("input.txt");

  if (!fileInput) {
    cerr << "[\x1b[31mERROR\x1b[0m] : file 'input.txt' tidak terdeteksi!" << endl;
		exit(1);
  }

  int noBaris = 1;
  while (getline(fileInput, str)) {
    double nilaiInput;
    try {
      nilaiInput = stod(str);
    }
    catch (const invalid_argument& e) {
      cerr << "[\x1b[31mERROR\x1b[0m] : nilai '" << str << "' pada baris ke-" << noBaris << " tidak valid!"  << endl;
      cerr << "[\x1b[33mWARNING\x1b[0m] : baris ke-" << noBaris << " dilewati..." << endl;
      noBaris++;
      continue;
    }

    input.push_back(nilaiInput);
    noBaris++;
  }
  fileInput.close();

  int n = input.size();
  double sum = 0, rata2, varians, standarDeviasi, standarError;

  for (size_t i = 0; i < n; i++) {
    sum += input[i];
  }
  rata2 = sum / n;

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

  sort(input.begin(), input.end());
  int max = *max_element(input.begin(), input.end());
  int min = *min_element(input.begin(), input.end());
  int jangkauan = max - min;
  int kelas = ceil(1 + (3.3 * log10(n)));
  int interval = ceil(jangkauan / kelas);

  tabelDeskriptif = {
    {"Jumlah Data", to_string(n)},
    {"Nilai Maks", to_string(max)},
    {"Nilai Min", to_string(min)},
    {"Rata-Rata", formatDesimal(rata2, 1)},
    {"Jangkauan", to_string(jangkauan)},
    {"Kelas", to_string(kelas)},
    {"Interval", to_string(interval)},
    {"Varians", formatDesimal(varians, 3)},
    {"Standar Deviasi", formatDesimal(standarDeviasi, 3)},
    {"Standar Error", formatDesimal(standarError, 3)}
  };

  vector<int> frekuensi(kelas, 0);
  vector<string> ketFrekuensi;

  int awalan = min, akhiran = awalan;
  int indeksInput = 0;
  for (size_t i = 0; i < kelas; i++) {
    akhiran += interval;
    for (size_t j = indeksInput; j < input.size(); j++) {
      if (i == kelas - 1 && input[j] == akhiran) {akhiran++;};
      if (input[j] < akhiran) {
        frekuensi[i]++;
        indeksInput++;
      } else {
        break;
      }
    }
    str  = to_string(awalan) + " - " + to_string(akhiran - 1);
    ketFrekuensi.push_back(str);
    awalan = akhiran;
  }

  tabelDistribusi.push_back({"Interval", "Frekuensi"});
  for (size_t i = 0; i < kelas; i++) {
    vector<string> baris;
    baris.push_back(ketFrekuensi[i]);
    baris.push_back(to_string(frekuensi[i]));
    tabelDistribusi.push_back(baris);
  }

  ofstream fileOutput;
  fileOutput.open("output.txt");

  printTabel("Tabel Distribusi Frekuensi", tabelDistribusi, fileOutput);
  fileOutput << endl;
  printTabel("Keterangan", tabelDeskriptif, fileOutput);
  fileOutput.close();
  
  return 0;
}

string formatDesimal(const float &angka, const int &digit) {
  stringstream ss;
  ss << fixed << setprecision(digit) << angka;
  return ss.str();
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
}