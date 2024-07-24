#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include <iomanip>
#include <bitset>

class TruthTableLUTSynthesizer {
public:
    TruthTableLUTSynthesizer(int n_inputs, int n_outputs, const std::vector<std::pair<std::string, std::string>>& truth_table, int rarity)
        : n_inputs(n_inputs), n_outputs(n_outputs), truth_table(truth_table), rarity(rarity) {}

    void process_dont_cares(const std::vector<std::string>& inputs) {
        for (const auto& pattern : inputs) {
            input_counts[pattern]++;
        }

        for (const auto& entry : truth_table) {
            if (input_counts[entry.first] >= rarity) {
                filtered_truth_table.push_back(entry);
            }
        }
    }

    void generate_verilog(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }

        std::string module_name = filename.substr(0, filename.find_last_of('.'));
        file << "module " << module_name << " ( ";
        file << "input [" << (n_inputs - 1) << ":0] M0, ";
        file << "output [" << (n_outputs - 1) << ":0] M1 ";
        file << ");\n\n";

        file << "    (*rom_style = \"distributed\" *) reg [" << (n_outputs - 1) << ":0] M1r;\n";
        file << "    assign M1 = M1r;\n";
        file << "    always @ (M0) begin\n";
        file << "        case (M0)\n";

        for (const auto& entry : filtered_truth_table) {
            file << "            " << n_inputs << "'b" << entry.first << ": M1r = " << n_outputs << "'b" << entry.second << ";\n";
        }

        file << "        endcase\n";
        file << "    end\n";
        file << "endmodule\n";
        file.close();
    }

private:
    int n_inputs;
    int n_outputs;
    int rarity;
    std::vector<std::pair<std::string, std::string>> truth_table;
    std::vector<std::pair<std::string, std::string>> filtered_truth_table;
    std::unordered_map<std::string, int> input_counts;
};

std::vector<std::pair<std::string, std::string>> read_truth_table(const std::string& filename, int n_inputs, int n_outputs) {
    std::ifstream file(filename);
    std::vector<std::pair<std::string, std::string>> truth_table;
    std::string line;
    int i = 0;

    while (std::getline(file, line)) {
        std::stringstream ss;
        ss << std::hex << line;
        unsigned int value;
        ss >> value;

        std::string input_pattern = std::bitset<32>(i).to_string().substr(32 - n_inputs); // Use 32 to accommodate larger input widths
        std::string output_pattern = std::bitset<32>(value).to_string().substr(32 - n_outputs); // Use 32 to accommodate larger output widths

        truth_table.emplace_back(input_pattern, output_pattern);
        i++;
    }

    return truth_table;
}

std::vector<std::string> read_neuron_inputs(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<std::string> inputs;
    std::string line;

    while (std::getline(file, line)) {
        inputs.push_back(line);
    }

    return inputs;
}

int main(int argc, char* argv[]) {
    if (argc != 7) {
        std::cerr << "Usage: " << argv[0] << " <truth_table_file> <inputs_file> <rarity_threshold> <verilog_filename> <n_inputs> <n_outputs>\n";
        return 1;
    }

    std::string truth_table_file = argv[1];
    std::string inputs_file = argv[2];
    int rarity = std::stoi(argv[3]);
    std::string verilog_filename = argv[4];
    verilog_filename += ".v";
    int n_inputs = std::stoi(argv[5]);
    int n_outputs = std::stoi(argv[6]);

    auto truth_table = read_truth_table(truth_table_file, n_inputs, n_outputs);
    auto inputs = read_neuron_inputs(inputs_file);

    TruthTableLUTSynthesizer synthesizer(n_inputs, n_outputs, truth_table, rarity);
    synthesizer.process_dont_cares(inputs);
    synthesizer.generate_verilog(verilog_filename);

    std::cout << "Verilog file generated: " << verilog_filename << std::endl;
    return 0;
}
