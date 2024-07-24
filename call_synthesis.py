import subprocess
import os

def run_synthesiser(range_limit, layer_start_index, n_inputs, n_outputs):
    # Define the path to the directory with the hex LUT values
    results_dir = os.path.join(os.path.dirname(__file__), 'verilog')

    for x in range(range_limit):

        executable_base_path = './TruthTableLUTSynthesiser.exe'  
        truth_table_file = os.path.join(results_dir, f'layer{layer_start_index}_N{x}_hex.txt')
        neuron_input_file = os.path.join(results_dir, f'layer{layer_start_index}_N{x}_input.txt')
        rarity_threshold = '2'  # Set the desired rarity threshold
        verilog_filename = f'layer{layer_start_index}_N{x}_output'

        try:
            # Run the executable and capture the output
            result = subprocess.run(
                [
                    executable_base_path, 
                    truth_table_file, 
                    neuron_input_file, 
                    rarity_threshold, 
                    verilog_filename,
                    str(n_inputs), 
                    str(n_outputs)
                ],
                check=True,
                universal_newlines=True,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE
            )
            
            # Executable output
            print("Output from C++ executable:")
            print(result.stdout)

        except subprocess.CalledProcessError as e:
            print(f"Error running the executable: {e}")
            print("Standard output:")
            print(e.stdout)
            print("Standard error:")
            print(e.stderr)

        except FileNotFoundError:
            print("Executable not found. Please check the path.")

# Function calls
run_synthesiser(32, 0, 12, 4)  # Example: layer 0, 32 neurons, 12 input bits, 4 output bits
