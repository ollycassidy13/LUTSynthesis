# Truth Table LUT Synthesizer

This program synthesizes a truth table into a Verilog file based on a specified rarity threshold and specified inputs.

## Description

The TruthTableLUTSynthesizer takes a truth table and truth table inputs, filters the truth table based on a rarity threshold, and generates a corresponding Verilog file. This can be useful for reducing LUT utilisation.

## Usage

To compile the program, use a C++ compiler like g++:
```
g++ -o TruthTableLUTSynthesiser.exe TruthTableLUTSynthesiser.cpp
```


To run the program, use the following command:
```
./TruthTableLUTSynthesiser.exe <truth_table_file> <inputs_file> <rarity_threshold> <verilog_filename> <n_inputs> <n_outputs>
```


### Arguments

- `<truth_table_file>`: The file containing the truth table data.
- `<inputs_file>`: The file containing the specified inputs.
- `<rarity_threshold>`: The rarity threshold for filtering inputs. This should be an integer.
- `<verilog_filename>`: The name of the output Verilog file (without the .v extension).
- `<n_inputs>`: The input bitwidth. This should be an integer.
- `<n_outputs>`: The output bitwidth. This should be an integer.

### Example
```
./TruthTableLUTSynthesiser.exe truth_table.txt inputs.txt 5 output_file 12 4
```

This command will read the truth table from `truth_table.txt`, the specified inputs from `inputs.txt`, use a rarity threshold of 5, generate a Verilog file named `output_file.v`, use an input bitwidth of 12 and an output bitwidth of 4.

### Directory Structure

.<br>
├── TruthTableLUTSynthesiser.cpp<br>
├── TruthTableLUTSynthesiser.exe<br>
├── truth_table.txt<br>
├── inputs.txt<br>
└── output_file.v<br>

## Input Files

### Truth Table File

The truth table file should contain one hexadecimal value per line. Each line corresponds to an output for a given input pattern. The input patterns are assumed to be in increasing binary order.

Example:<br>
A<br>
B<br>
C<br>
D<br>
...

### Inputs File

The inputs file should contain one binary pattern per line. These patterns indicate which input combinations are passed to the model during the training process and are used for the purpose of filtering the truth table.

Example:<br>
000000000000<br>
000000000001<br>
000000000010<br>
...

## Output

The program generates a Verilog file based on the filtered truth table. The Verilog file will have a module named <verilog_filename> with the specified input and output widths.

Example Verilog output:

```
module layer0_N0 ( input [11:0] M0, output [3:0] M1 );

    (*rom_style = "distributed" *) reg [3:0] M1r;
    assign M1 = M1r;
    always @ (M0) begin
        case (M0)
            12'b000000000000: M1r = 4'b0000;
            12'b000000000001: M1r = 4'b0001;
            // more cases...
        endcase
    end
endmodule
```
## References

This work is based on [Synthesis of LUT Networks for Random-Looking Dense Functions with
Don’t Cares — Towards Efficient FPGA Implementation of DNN](https://people.eecs.berkeley.edu/~alanmi/publications/2024/fccm24_lut.pdf) by Yukio Miyasaka, Alan Mishchenko, John Wawrzynek, Nicholas J. Frase.
