import sys
import textwrap
import argparse
import re

n = len(sys.argv)

parser = argparse.ArgumentParser(description="Assemble .asm for RISC V")
parser.add_argument("--input", "-i", type=str, required=True)
parser.add_argument("--out-vhdl", "-ovhd", type=str)
parser.add_argument("--out-byte", "-obyte", type=str)


args = parser.parse_args()

InstructionsTypes = {
    "add": ["r", 51, 0, 0],
    "sub": ["r", 51, 32, 0],
    "or": ["r", 51, 0,  6],
    "and": ["r", 51, 0, 7],
    "addi": ["i", 19, 0, 0],
    "andi": ["i", 19, 0, 7],
    "ori": ["i", 19, 9, 6],
    "ld": ["i", 3, 0, 6],
    "sd": ["s", 35, 0, 3],
    "beq": ["sb", 99, 0, 0],
    "bne": ["sb", 99, 0, 1],
}


def format_reg(reg_string):
    return "{0:05b}".format(int(reg_string.replace("x", "")))


def format_opcode(op_string):
    return "{0:07b}".format(op_string)


def format_func7(func7_string):
    return "{0:07b}".format(func7_string)


def format_func3(func3_string):
    return "{0:03b}".format(func3_string)


def format_address(address):
    return "{0:08b}".format(address)


def twosComplement(number, numBits):
    if number >= 0:
        return number
    else:
        return (1 << numBits) + number


def parseR(instruction_dict, match_obj, operation, instruction_address):
    op_code = format_opcode(instruction_dict[operation][1])
    rd = format_reg(match_obj.group(2))
    rs1 = format_reg(match_obj.group(3))
    rs2 = format_reg(match_obj.group(4))
    func7 = format_func7(instruction_dict[operation][2])
    func3 = format_func3(instruction_dict[operation][3])
    return (
        format_address(instruction_address)
        + ":"
        + func7
        + rs2
        + rs1
        + func3
        + rd
        + op_code
        + "\n"
    )


def parseI(instruction_dict, match_obj, operation, instruction_address):
    op_code = format_opcode(instruction_dict[operation][1])
    rd = format_reg(match_obj.group(2))
    rs1 = format_reg(match_obj.group(3))
    immediate = "{0:012b}".format(twosComplement(int(match_obj.group(4)), 12))
    func3 = format_func3(instruction_dict[operation][3])
    return (
        format_address(instruction_address)
        + ":"
        + immediate
        + rs1
        + func3
        + rd
        + op_code
        + "\n"
    )


def parseS(instruction_dict, match_obj, operation, instruction_address):
    op_code = format_opcode(instruction_dict[operation][1])
    immediate = "{0:012b}".format(twosComplement(int(match_obj.group(4)), 12))
    immediate_upper = immediate[0:7]
    immediate_lower = immediate[7:]
    rs1 = format_reg(match_obj.group(3))
    rs2 = format_reg(match_obj.group(2))
    func3 = format_func3(instruction_dict[operation][3])
    return (
        format_address(instruction_address)
        + ":"
        + immediate_upper
        + rs2
        + rs1
        + func3
        + immediate_lower
        + op_code
        + "\n"
    )


def parseSB(
    instruction_dict, match_obj, operation, instruction_address, label=False, offset=0
):
    op_code = format_opcode(instruction_dict[operation][1])
    if not label:
        immediate = "{0:013b}".format(twosComplement(int(match_obj.group(4)), 13))
    else:
        immediate = "{0:013b}".format(twosComplement(offset, 13))
        print(immediate)
    immediate_upper = immediate[0] + immediate[2:8]
    immediate_lower = immediate[8:12] + immediate[1]
    rs1 = format_reg(match_obj.group(2))
    rs2 = format_reg(match_obj.group(3))
    func3 = format_func3(instruction_dict[operation][3])
    return (
        format_address(instruction_address)
        + ":"
        + immediate_upper
        + rs2
        + rs1
        + func3
        + immediate_lower
        + op_code
        + "\n"
    )


def analyze_label(instructions_input):
    instruction_address = 0
    label_map = dict()
    while True:
        if space_match := re.match(r"\s+", instructions_input):
            instructions_input = instructions_input[space_match.end() :]
            continue
        elif single_comment_match := re.match(r"#.*\n?", instructions_input):
            instructions_input = instructions_input[single_comment_match.end() :]
            continue
        elif multi_comment_match := re.match(
            r"\/\*[\w\s\#\=\(\)\:\-\+\!]*\*\/", instructions_input
        ):
            instructions_input = instructions_input[multi_comment_match.end() :]
            continue
        elif label_match := re.match(r"(\w+):", instructions_input):
            label_map[label_match.group(1)] = instruction_address
            instructions_input = instructions_input[label_match.end() :]
            continue
        elif op_match := re.match(r"([a-zA-z]+)", instructions_input):
            op = op_match.group(1)
            if op in InstructionsTypes:
                match = re.match(
                    r"([a-zA-Z]+)\s+([xX]\d+)\s+([xX]\d+)\s+(((?:-|[xX])?\d+)|[a-zA-Z0-9]+)",
                    instructions_input,
                )
                if (
                    match
                    and InstructionsTypes[op][0] == "r"
                    and re.match(r"[xX]\d+", match.group(4))
                ):
                    instructions_input = instructions_input[match.end() :]

                elif (
                    match
                    and InstructionsTypes[op][0] == "i"
                    and re.match(r"-?\d+", match.group(4))
                ):
                    instructions_input = instructions_input[match.end() :]

                elif (
                    match
                    and InstructionsTypes[op][0] == "s"
                    and re.match(r"-?\d+", match.group(4))
                ):
                    instructions_input = instructions_input[match.end() :]

                elif (
                    match
                    and InstructionsTypes[op][0] == "sb"
                    and re.match(r"(-?\d+)|[a-zA-Z0-9]+", match.group(4))
                ):
                    if re.match(r"-?\d+", match.group(4)):
                        instructions_input = instructions_input[match.end() :]
                    elif re.match(r"[a-zA-Z0-9]+", match.group(4)):
                        instructions_input = instructions_input[match.end() :]

                else:
                    print("invalid Instruction while label decode")
                    break
                instruction_address += 4
                continue
            else:
                print("invalid Instruction while label decode")
                break
        print("Label Decode Successful....")
        break
    return label_map


def decode(label_map, instructions_input):
    instruction_address = 0
    instructions_translated = []
    while True:
        # print(instructions_input)
        if space_match := re.match(r"\s+", instructions_input):
            instructions_input = instructions_input[space_match.end() :]
            continue
        elif single_comment_match := re.match(r"#.*\n?", instructions_input):
            instructions_input = instructions_input[single_comment_match.end() :]
            continue
        elif multi_comment_match := re.match(
            r"\/\*[\w\s\#\=\(\)\:\-\+\!]*\*\/", instructions_input
        ):
            instructions_input = instructions_input[multi_comment_match.end() :]
            continue
        elif label_match := re.match(r"(\w+):", instructions_input):
            instructions_input = instructions_input[label_match.end() :]
            continue
        elif op_match := re.match(r"([a-zA-z]+)", instructions_input):
            op = op_match.group(1)
            if op in InstructionsTypes:
                match = re.match(
                    r"([a-zA-Z]+)\s+([xX]\d+)\s+([xX]\d+)\s+(((?:-|[xX])?\d+)|[a-zA-Z0-9]+)",
                    instructions_input,
                )
                if (
                    match
                    and InstructionsTypes[op][0] == "r"
                    and re.match(r"[xX]\d+", match.group(4))
                ):
                    instructions_translated.append(
                        parseR(InstructionsTypes, match, op, instruction_address)
                    )
                    instructions_input = instructions_input[match.end() :]

                elif (
                    match
                    and InstructionsTypes[op][0] == "i"
                    and re.match(r"-?\d+", match.group(4))
                ):
                    instructions_translated.append(
                        parseI(InstructionsTypes, match, op, instruction_address)
                    )
                    instructions_input = instructions_input[match.end() :]

                elif (
                    match
                    and InstructionsTypes[op][0] == "s"
                    and re.match(r"-?\d+", match.group(4))
                ):
                    instructions_translated.append(
                        parseS(InstructionsTypes, match, op, instruction_address)
                    )
                    instructions_input = instructions_input[match.end() :]

                elif (
                    match
                    and InstructionsTypes[op][0] == "sb"
                    and re.match(r"(-?\d+)|[a-zA-Z0-9]+", match.group(4))
                ):
                    if re.match(r"-?\d+", match.group(4)):
                        instructions_translated.append(
                            parseSB(InstructionsTypes, match, op, instruction_address)
                        )
                        instructions_input = instructions_input[match.end() :]
                    elif re.match(r"[a-zA-Z0-9]+", match.group(4)):
                        label = match.group(4)
                        try:
                            offset = label_map[label] - instruction_address
                            print('offset for', instruction_address , 'is', offset)
                            instructions_translated.append(
                                parseSB(
                                    InstructionsTypes,
                                    match,
                                    op,
                                    instruction_address,
                                    label=True,
                                    offset=offset,
                                )
                            )
                            instructions_input = instructions_input[match.end() :]
                        except:
                            break

                else:
                    break
                instruction_address += 4
                continue
            else:
                print("invalid Instruction in instruction decode")
                break
        print("Assembly Successful!.........")
        break
    return instructions_translated


instruction_address = 0
assem_file = open(args.input, "r")
instructions_input = assem_file.read()
assem_file.close()

label_map = analyze_label(instructions_input)
instructions_translated = decode(label_map, instructions_input)

instruction_vhdl = []
for i, instruction in enumerate(instructions_translated):
    instruction_blocks = list(
        reversed(
            textwrap.wrap(
                instruction[instruction.index(":") + 1 :].replace("\n", ""), 8
            )
        )
    )
    for j, block in enumerate(instruction_blocks):
        instruction_vhdl.append(
            'instruction_array({0})({1}) <= "{2}";\n'.format(i, j, block)
        )

# # print(instruction_vhdl)

if args.out_byte:
    machine_file = open(args.out_byte, "w")
    machine_file.writelines(instructions_translated)
    machine_file.close()

if args.out_vhdl:
    machine_vhdl_file = open(args.out_vhdl, "w")
    machine_vhdl_file.writelines(instruction_vhdl)
    machine_vhdl_file.close()
