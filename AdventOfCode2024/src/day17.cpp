#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

std::pair<std::vector<uint64_t>, std::vector<int>>
readInput(const std::string &filePath) {
    if (!std::filesystem::exists(filePath)) {
        std::cerr << "File does not exist: " << filePath << std::endl;
        return {};
    }

    std::ifstream inputFile(filePath);
    if (!inputFile) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return {};
    }

    std::regex register_regex("Register [ABC]: (\\d+)");
    std::regex program_regex("Program: ([\\d,]+)");
    std::regex number_regex("\\d+");

    std::vector<uint64_t> device;
    std::vector<int> program;

    std::string line;
    while (std::getline(inputFile, line)) {
        std::smatch match;

        if (std::regex_match(line, match, register_regex)) {
            device.push_back(std::stoll(match[1]));
        }

        if (std::regex_match(line, match, program_regex)) {
            std::string program_numbers = match[1];
            std::sregex_iterator it(program_numbers.begin(), program_numbers.end(),
                                    number_regex);
            std::sregex_iterator end;

            while (it != end) {
                program.push_back(std::stoi((*it).str()));
                ++it;
            }
        }
    }

    inputFile.close();

    return {device, program};
}

void dv(const uint64_t combo, uint64_t &ra, uint64_t &target) { target = (ra >> combo); }

void bxl(const uint64_t operand, uint64_t &rb) { rb ^= operand; }

void bst(const uint64_t combo, uint64_t &rb) { rb = combo % 8; }

void bxc(uint64_t &rb, const uint64_t &rc) { rb ^= rc; }

uint64_t out(const uint64_t combo) { return combo % 8; }

void doDebug(std::vector<uint64_t> &device, const int opcode, const int operand,
             std::vector<uint64_t> &output) {
    auto &ra = device[0];
    auto &rb = device[1];
    auto &rc = device[2];

    uint64_t combo;
    if (operand <= 3) {
        combo = operand;
    } else if (operand >= 4 && operand <= 6) {
        combo = device[operand - 4];
    } else {
        exit(-1);
    }

    switch (opcode) {
        case 0:
            dv(combo, ra, ra);
            break;
        case 1:
            bxl(operand, rb);
            break;
        case 2:
            bst(combo, rb);
            break;
        case 4:
            bxc(rb, rc);
            break;
        case 5:
            output.push_back(out(combo));
            break;
        case 6:
            dv(combo, ra, rb);
            break;
        case 7:
            dv(combo, ra, rc);
            break;
        default:
            break;
    }
}

std::vector<uint64_t> debugDevice(std::vector<uint64_t> &device,
                                  const std::vector<int> &program) {
    std::vector<uint64_t> output;
    int pIdx = 0;
    while (pIdx < program.size()) {
        const int opcode = program[pIdx];
        const int operand = program[pIdx + 1];

        if (opcode == 3) {
            if (device[0] != 0) {
                pIdx = operand;
                continue;
            }
        }
        doDebug(device, opcode, operand, output);
        pIdx += 2;
    }
    return output;
}

std::string
solverP1(std::pair<std::vector<uint64_t>, std::vector<int>> &inputData) {
    auto &[device, program] = inputData;
    auto output = debugDevice(device, program);
    std::string resultStr;

    // not to remove the comma!!!!!!!  (ﾒ ﾟ皿ﾟ)ﾒ
    for (auto it = output.begin(); it != output.end(); it++) {
        resultStr += std::to_string(*it);
        if (it != output.end() - 1) {
            resultStr += ",";
        }
    }

    return resultStr;
}


int main(int argc, char *argv[]) {
    const std::string &filePath = argv[1];
    auto input = readInput(filePath);

    const auto &[device, program] = input;

    auto resultP1 = solverP1(input);
    std::cout << "P1 result: " << resultP1 << std::endl;

    return 0;
}