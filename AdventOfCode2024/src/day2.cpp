#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <vector>

std::vector<std::vector<int>> readInput(const std::string &filePath) {
  std::vector<std::vector<int>> report;

  if (!std::filesystem::exists(filePath)) {
    std::cerr << "File does not exist: " << filePath << std::endl;
    return report;
  }

  std::ifstream inputFile(filePath);
  if (!inputFile) {
    std::cerr << "Error opening file: " << filePath << std::endl;
    return report;
  }

  std::string line;
  int num1, num2;

  while (std::getline(inputFile, line)) {
    std::vector<int> row;

    std::istringstream lineStream(line);
    int num;

    // Extract integers from the line
    while (lineStream >> num) {
      row.push_back(num);
    }

    report.push_back(row); // Add the row to the result
  }

  inputFile.close();

  return report;
}

bool isSafe(const std::vector<int> &reportRow) {
  size_t sz = reportRow.size();

  if (sz < 2) {
    return false;
  }

  if (reportRow[1] == reportRow[0]) {
    return false;
  }

  bool isIncreasing = reportRow[1] - reportRow[0] > 0 ? true : false;

  for (auto i = 1; i < sz; i++) {
    int diff = reportRow[i] - reportRow[i - 1];

    if (diff == 0) {
      return false;
    }

    int absDiff = std::abs(diff);
    if (absDiff < 1 || absDiff > 3) {
      return false;
    }

    bool isIncreasing_ = diff > 0 ? true : false;

    bool isSameOrder = !(isIncreasing ^ isIncreasing_); // xnor
    if (!isSameOrder) {
      return false;
    }
  }

  return true;
}

int solverP1(const std::vector<std::vector<int>> &report) {
  int cnt = 0;
  // int rowNum = 0;
  for (const auto row : report) {
    if (isSafe(row)) {
      // std::cout << "row " << rowNum << " safe" << std::endl;
      cnt++;
    }
    // rowNum++;
  }

  return cnt;
}

bool isTolerableSafe(const std::vector<int> &reportRow) {
  if (isSafe(reportRow)) {
    return true;
  }

  size_t sz = reportRow.size();
  for (size_t i = 0; i < sz; i++) {
    std::vector<int> reportRow_(reportRow.begin(), reportRow.end());
    reportRow_.erase(reportRow_.begin() + i);
    if (isSafe(reportRow_)) {
      return true;
    }
  }
  return false;
}

int solverP2(const std::vector<std::vector<int>> &report) {
  int cnt = 0;
  //   int rowNum = 0;
  for (const auto row : report) {
    if (isTolerableSafe(row)) {
      //   std::cout << "row " << rowNum << " safe" << std::endl;
      cnt++;
    }
    // rowNum++;
  }

  return cnt;
}

int main(int argc, char *argv[]) {
  std::string filePath = argv[1];

  auto input = readInput(filePath);

  int resultP1 = solverP1(input);
  std::cout << "P1 result: " << resultP1 << std::endl;

  int resultP2 = solverP2(input);
  std::cout << "P2 result: " << resultP2 << std::endl;

  return 0;
}