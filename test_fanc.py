import os
import subprocess

import pytest


def get_test_cases(input_dir, output_dir, program_dir, expected_dir):
    _test_cases = []
    input_files = os.listdir(input_dir)

    for input_file in input_files:
        if input_file.endswith('.in'):
            test_name = os.path.splitext(input_file)[0]
            program_file = os.path.join(program_dir, test_name + '.ll')
            output_file = os.path.join(output_dir, test_name + '.res')
            expected_file = os.path.join(expected_dir, test_name + '.out')
            _test_cases.append((program_file, output_file, expected_file, os.path.join(input_dir, input_file)))

    return _test_cases


compiler_path = 'cmake-build-debug/fanc'
test_cases = get_test_cases('test/compiler/input', 'test/compiler/output',
                            'test/compiler/programs', 'test/compiler/expected')


@pytest.mark.parametrize("program_file, output_file, expected_file, input_file", test_cases, ids=[input_file.split("/")[-1] for _, _, _, input_file in test_cases])
def test_compiler(program_file, output_file, expected_file, input_file):
    # Compile the program using the compiler_path
    compile_command = f'{compiler_path} < {input_file} > {program_file}'
    subprocess.run(compile_command, shell=True, check=True)

    # Execute the compiled program using lli
    execute_command = f'lli {program_file} > {output_file}'
    subprocess.run(execute_command, shell=True, check=True)

    # Read the expected output from expected_file

    # Example code to compare the output with the expected output
    with open(output_file, 'r') as output:
        output_data = output.read()

    with open(expected_file, 'r') as expected:
        expected_data = expected.read()

    assert output_data == expected_data
