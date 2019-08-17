#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <fstream>
#include "risc_v_constants.h"
#include "risc_v_macros.h"

using namespace std;

struct instr_s {
    string          instr_name;
    char            instr_type;
    int             instr_v;
    int             instr_m;
    char            op;
    void print_instr() {
        cout << instr_name << " " << instr_type << " " << instr_v << " " << op << endl;
    }
};

struct csr {
    string      csr_name;
    int         csr_addr;
};

instr_s i_list[] = {
    // R type
    RV_FORM_INSTR( "ADD   " , 'R' , Z_F7    , ADD_F3   , R_OP_0 , RVI , RV_FORM_MASK(1,1,1,1) , 0xE ),
    RV_FORM_INSTR( "SUB   " , 'R' , SUB_F7  , SUB_F3   , R_OP_0 , RVI , RV_FORM_MASK(1,1,1,1) , 0xE ),
    RV_FORM_INSTR( "SLL   " , 'R' , Z_F7    , SLL_F3   , R_OP_0 , RVI , RV_FORM_MASK(1,1,1,1) , 0xE ),
    RV_FORM_INSTR( "SLT   " , 'R' , Z_F7    , SLT_F3   , R_OP_0 , RVI , RV_FORM_MASK(1,1,1,1) , 0xE ),
    RV_FORM_INSTR( "SLTU  " , 'R' , Z_F7    , SLTU_F3  , R_OP_0 , RVI , RV_FORM_MASK(1,1,1,1) , 0xE ),
    RV_FORM_INSTR( "XOR   " , 'R' , Z_F7    , XOR_F3   , R_OP_0 , RVI , RV_FORM_MASK(1,1,1,1) , 0xE ),
    RV_FORM_INSTR( "SRL   " , 'R' , Z_F7    , SRL_F3   , R_OP_0 , RVI , RV_FORM_MASK(1,1,1,1) , 0xE ),
    RV_FORM_INSTR( "SRA   " , 'R' , SRA_F7  , SRA_F3   , R_OP_0 , RVI , RV_FORM_MASK(1,1,1,1) , 0xE ),
    RV_FORM_INSTR( "OR    " , 'R' , Z_F7    , OR_F3    , R_OP_0 , RVI , RV_FORM_MASK(1,1,1,1) , 0xE ),
    RV_FORM_INSTR( "AND   " , 'R' , Z_F7    , AND_F3   , R_OP_0 , RVI , RV_FORM_MASK(1,1,1,1) , 0xE ),
    // I type
    // load instr
    RV_FORM_INSTR( "LB    " , 'I' , Z_F7    , LB_F3    , I_OP_1 , RVI , RV_FORM_MASK(0,1,1,1) , 0xD ),
    RV_FORM_INSTR( "LH    " , 'I' , Z_F7    , LH_F3    , I_OP_1 , RVI , RV_FORM_MASK(0,1,1,1) , 0xD ),
    RV_FORM_INSTR( "LW    " , 'I' , Z_F7    , LW_F3    , I_OP_1 , RVI , RV_FORM_MASK(0,1,1,1) , 0xD ),
    RV_FORM_INSTR( "LBU   " , 'I' , Z_F7    , LBU_F3   , I_OP_1 , RVI , RV_FORM_MASK(0,1,1,1) , 0xD ),
    RV_FORM_INSTR( "LHU   " , 'I' , Z_F7    , LHU_F3   , I_OP_1 , RVI , RV_FORM_MASK(0,1,1,1) , 0xD ),
    // logic and ariphmetic instr
    RV_FORM_INSTR( "ADDI  " , 'I' , Z_F7    , ADDI_F3  , I_OP_0 , RVI , RV_FORM_MASK(0,1,1,1) , 0xD ),
    RV_FORM_INSTR( "SLLI  " , 'I' , Z_F7    , SLLI_F3  , I_OP_0 , RVI , RV_FORM_MASK(1,1,1,1) , 0xD ),
    RV_FORM_INSTR( "SLTI  " , 'I' , Z_F7    , SLTI_F3  , I_OP_0 , RVI , RV_FORM_MASK(0,1,1,1) , 0xD ),
    RV_FORM_INSTR( "SLTUI " , 'I' , Z_F7    , SLTUI_F3 , I_OP_0 , RVI , RV_FORM_MASK(0,1,1,1) , 0xD ),
    RV_FORM_INSTR( "XORI  " , 'I' , Z_F7    , XORI_F3  , I_OP_0 , RVI , RV_FORM_MASK(0,1,1,1) , 0xD ),
    RV_FORM_INSTR( "SRLI  " , 'I' , Z_F7    , SRLI_F3  , I_OP_0 , RVI , RV_FORM_MASK(1,1,1,1) , 0xD ),
    RV_FORM_INSTR( "SRAI  " , 'I' , SRAI_F7 , SRAI_F3  , I_OP_0 , RVI , RV_FORM_MASK(1,1,1,1) , 0xD ),
    RV_FORM_INSTR( "ORI   " , 'I' , Z_F7    , ORI_F3   , I_OP_0 , RVI , RV_FORM_MASK(0,1,1,1) , 0xD ),
    RV_FORM_INSTR( "ANDI  " , 'I' , Z_F7    , ANDI_F3  , I_OP_0 , RVI , RV_FORM_MASK(0,1,1,1) , 0xD ),
    // jalr instr
    RV_FORM_INSTR( "JALR  " , 'I' , Z_F7    , JALR_F3  , I_OP_2 , RVI , RV_FORM_MASK(0,1,1,1) , 0xD ),
    // S type
    // store instr
    RV_FORM_INSTR( "SB    " , 'S' , Z_F7    , SB_F3    , S_OP_0 , RVI , RV_FORM_MASK(0,1,1,1) , 0x7 ),
    RV_FORM_INSTR( "SH    " , 'S' , Z_F7    , SH_F3    , S_OP_0 , RVI , RV_FORM_MASK(0,1,1,1) , 0x7 ),
    RV_FORM_INSTR( "SW    " , 'S' , Z_F7    , SW_F3    , S_OP_0 , RVI , RV_FORM_MASK(0,1,1,1) , 0x7 ),
    // B type
    // branch instr
    RV_FORM_INSTR( "BEQ   " , 'B' , Z_F7    , BEQ_F3   , B_OP_0 , RVI , RV_FORM_MASK(0,1,1,1) , 0x7 ),
    RV_FORM_INSTR( "BNE   " , 'B' , Z_F7    , BNE_F3   , B_OP_0 , RVI , RV_FORM_MASK(0,1,1,1) , 0x7 ),
    RV_FORM_INSTR( "BLT   " , 'B' , Z_F7    , BLT_F3   , B_OP_0 , RVI , RV_FORM_MASK(0,1,1,1) , 0x7 ),
    RV_FORM_INSTR( "BGE   " , 'B' , Z_F7    , BGE_F3   , B_OP_0 , RVI , RV_FORM_MASK(0,1,1,1) , 0x7 ),
    RV_FORM_INSTR( "BLTU  " , 'B' , Z_F7    , BLTU_F3  , B_OP_0 , RVI , RV_FORM_MASK(0,1,1,1) , 0x7 ),
    RV_FORM_INSTR( "BGEU  " , 'B' , Z_F7    , BGEU_F3  , B_OP_0 , RVI , RV_FORM_MASK(0,1,1,1) , 0x7 ),
    // U type
    // LUI
    RV_FORM_INSTR( "LUI   " , 'U' , Z_F7    , Z_F3     , U_OP_0 , RVI , RV_FORM_MASK(0,0,1,1) , 0x9 ),
    // AUIPC
    RV_FORM_INSTR( "AUIPC " , 'U' , Z_F7    , Z_F3     , U_OP_1 , RVI , RV_FORM_MASK(0,0,1,1) , 0x9 ),
    // J type
    // jal instr
    RV_FORM_INSTR( "JAL   " , 'J' , Z_F7    , Z_F3     , J_OP_0 , RVI , RV_FORM_MASK(0,0,1,1) , 0x9 ),
    // CSR
    RV_FORM_CSR_INSTR( "WFI   " , 'D' , WFI_F12    , WFI_F3    , CSR_OP_0 , RVI , RV_FORM_CSR_MASK(1,1,1,1) , 0x0 ),
    RV_FORM_CSR_INSTR( "MRET  " , 'D' , MRET_F12   , MRET_F3   , CSR_OP_0 , RVI , RV_FORM_CSR_MASK(1,1,1,1) , 0x0 ),
    RV_FORM_CSR_INSTR( "ECALL " , 'D' , ECALL_F12  , ECALL_F3  , CSR_OP_0 , RVI , RV_FORM_CSR_MASK(1,1,1,1) , 0x0 ),
    RV_FORM_CSR_INSTR( "EBREAK" , 'D' , EBREAK_F12 , EBREAK_F3 , CSR_OP_0 , RVI , RV_FORM_CSR_MASK(1,1,1,1) , 0x0 ),
    RV_FORM_CSR_INSTR( "CSRRW " , 'D' , Z_F12      , CSRRW_F3  , CSR_OP_0 , RVI , RV_FORM_CSR_MASK(0,1,1,1) , 0xC ),
    RV_FORM_CSR_INSTR( "CSRRS " , 'D' , Z_F12      , CSRRS_F3  , CSR_OP_0 , RVI , RV_FORM_CSR_MASK(0,1,1,1) , 0xC ),
    RV_FORM_CSR_INSTR( "CSRRC " , 'D' , Z_F12      , CSRRC_F3  , CSR_OP_0 , RVI , RV_FORM_CSR_MASK(0,1,1,1) , 0xC ),
    RV_FORM_CSR_INSTR( "CSRRWI" , 'D' , Z_F12      , CSRRWI_F3 , CSR_OP_0 , RVI , RV_FORM_CSR_MASK(0,1,1,1) , 0x9 ),
    RV_FORM_CSR_INSTR( "CSRRSI" , 'D' , Z_F12      , CSRRSI_F3 , CSR_OP_0 , RVI , RV_FORM_CSR_MASK(0,1,1,1) , 0x9 ),
    RV_FORM_CSR_INSTR( "CSRRCI" , 'D' , Z_F12      , CSRRCI_F3 , CSR_OP_0 , RVI , RV_FORM_CSR_MASK(0,1,1,1) , 0x9 )
};

string reg_list[] = {
                        "zero ",
                        "ra   ",
                        "sp   ",
                        "gp   ",
                        "tp   ",
                        "t0   ",
                        "t1   ",
                        "t2   ",
                        "s0/fp",
                        "s1   ",
                        "a0   ",
                        "a1   ",
                        "a2   ",
                        "a3   ",
                        "a4   ",
                        "a5   ",
                        "a6   ",
                        "a7   ",
                        "s2   ",
                        "s3   ",
                        "s4   ",
                        "s5   ",
                        "s6   ",
                        "s7   ",
                        "s8   ",
                        "s9   ",
                        "s10  ",
                        "s11  ",
                        "t3   ",
                        "t4   ",
                        "t5   ",
                        "t6   "
                    };

class rv_instr {
public:

    csr *csr_list_;

    rv_instr() {
        csr_list_init();
    }

    void csr_list_init() {
        ifstream csr_file("csr_file_list.txt", fstream::in);
        int csr_list_size = 0;
        csr_file >> dec >> csr_list_size;
        csr_list_ = new csr[csr_list_size];
        for(int i = 0 ; i < csr_list_size-1 ; i ++) {
            csr_file >> csr_list_[i].csr_name;
            csr_file >> hex >> csr_list_[i].csr_addr;
        }
    }

    unsigned int    instr;

    void pars_instr(instr_s instr_s_p) {
        cout << instr_s_p.instr_name << " ";
        if( RV_FIND_RD_MASK( instr_s_p.op ) ) {
            cout << reg_list[RV_FIND_RD(instr)] << " ";
        }
        if( ( RV_FIND_OPCODE(instr) == CSR_OP_0 ) && ( RV_FIND_F3(instr) != Z_F3 ) )  {
            for( int i = 0 ; i < 221 ; i++ ) {
                if( RV_FIND_CSR_ADDR(instr) == csr_list_[i].csr_addr )
                    cout << csr_list_[i].csr_name << " ";
            }
        }
        if( RV_FIND_RS1_MASK( instr_s_p.op ) ) {
            cout << reg_list[RV_FIND_RS1(instr)] << " ";
        }
        if( RV_FIND_RS2_MASK( instr_s_p.op ) ) {
            cout << reg_list[RV_FIND_RS2(instr)] << " ";
        }
        if( RV_FIND_IMM_MASK( instr_s_p.op ) ) {
            switch( instr_s_p.instr_type ) {
                case 'R':   break;
                case 'I': 
                            cout << "0x" << hex << ( ( RV_FIND_F3(instr) != SRA_F3 ) || ( RV_FIND_F3(instr) != SRL_F3 ) || ( RV_FIND_F3(instr) != SLL_F3 ) ? RV_FIND_I_IMM(instr) : RV_FIND_IS_IMM(instr) ); 
                            break;
                case 'U': 
                            cout << "0x" << hex << RV_FIND_U_IMM(instr); 
                            break;
                case 'S':   
                            cout << "0x" << hex << RV_FIND_S_IMM(instr);
                            break;
                case 'J':   
                            cout << "0x" << hex << RV_FIND_J_IMM(instr);
                            break;
                case 'B': 
                            cout << "0x" << hex << RV_FIND_B_IMM(instr);
                            break;
                case 'D':   
                            cout << "0x" << hex << RV_FIND_CSR_ZIMM(instr);
                            break;
            }
        }
        cout << endl;
    }

    void load_instr(unsigned int instr_in) {
        instr = instr_in;
    }

    void pars_instr() {
        switch( RV_FIND_ITYPE(instr) ) {
        case    RVI:
                for( int i = 0 ; i < 47 ; i++ ) {
                    if( ( instr & i_list[i].instr_m ) == i_list[i].instr_v ) { 
                        pars_instr(i_list[i]);
                    }
                }
            break;
        default:
            cout << "Not RVI instruction" << endl;
            break;
        }
    }
};
