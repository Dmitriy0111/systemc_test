
help:
	$(info make help           - show this message)
	$(info make clean          - delete synth and simulation folders)
	$(info make prog_comp_c    - compile C program and copy program.hex to program_file)
	$(info Open and read the Makefile for details)
	@true

PWD     := $(shell pwd)

clean: \
	prog_clean

########################################################
# compiling  - program

PROG_NAME ?= counter
CCF	= -march=rv32i -mabi=ilp32
LDF	= -b elf32-littleriscv
CPF = ihex -O ihex

prog_comp_c:
	mkdir -p program_file
	riscv-none-embed-as test_programs/boot.S -c -o program_file/boot.o $(CCF)
	riscv-none-embed-gcc -O1 test_programs/$(PROG_NAME).c -c -o program_file/main.o $(CCF)
	riscv-none-embed-gcc -O1 test_programs/vectors.c -c -o program_file/vectors.o $(CCF)
	riscv-none-embed-ld -o program_file/main.elf -Map program_file/main.map -T test_programs/program.ld program_file/boot.o program_file/main.o program_file/vectors.o $(LDF)
	riscv-none-embed-objdump -M no-aliases -S -w --disassemble-zeroes program_file/main.elf > program_file/main.lst
	riscv-none-embed-objcopy program_file/main.elf program_file/program.$(CPF)
	python test_programs/ihex2one_hex.py

prog_clean:
	rm -rfd $(PWD)/program_file
