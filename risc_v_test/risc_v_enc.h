#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <fstream>

using namespace std;

#define     RV_FORM_INSTR(name,i_type,f7,f3,opcode,type,mask,rd,rs1,rs2)        { name, i_type, ( ( f7 << 25 ) | ( f3 << 12 ) | ( opcode <<  2 ) | ( type << 0 ) ),mask, rd, rs1, rs2 }
#define     RV_FORM_CSR_INSTR(name,i_type,f12,f3,opcode,type,mask,rd,rs1,rs2)    { name, i_type, ( ( f12 << 20 ) | ( f3 << 12 ) | ( opcode <<  2 ) | ( type << 0 ) ),mask, rd, rs1, rs2 }
#define     RV_FORM_MASK(f7,f3,opcode,type) (int)( ( type == 1 ? 0b11 : 0b00 ) | ( opcode == 1 ? 0x7C : 0x00 ) | ( f3 == 1 ? 0x7000 : 0x0000 ) | ( f7 == 1 ? 0xFE000000 : 0x00000000 ) )
#define     RV_FORM_CSR_MASK(f12,f3,opcode,type) (int)( ( type == 1 ? 0b11 : 0b00 ) | ( opcode == 1 ? 0x7C : 0x00 ) | ( f3 == 1 ? 0x7000 : 0x0000 ) | ( f12 == 1 ? 0xFFF00000 : 0x00000000 ) )
#define     RV_FIND_OPCODE(instr)   ( ( instr >> 2 ) & 0b11111 )
#define     RV_FIND_ITYPE(instr)    ( instr & 0b11 )
#define     RV_FIND_F3(instr)       ( ( instr >> 12 ) & 0b111 )
#define     RV_FIND_F7(instr)       ( ( instr >> 25 ) & 0b1111111 )
#define     RV_FIND_RS1(instr)      ( ( instr >> 15 ) & 0b11111 )
#define     RV_FIND_RS2(instr)      ( ( instr >> 20 ) & 0b11111 )
#define     RV_FIND_RD(instr)       ( ( instr >> 7  ) & 0b11111 )
#define     RV_FIND_CSR_ADDR(instr) ( instr >> 20 )
#define     RV_FIND_CSR_ZIMM(instr) ( ( instr >> 20 ) & 0b11111 )
#define     RV_FIND_I_IMM(instr)    ( instr >> 20 )
#define     RV_FIND_IS_IMM(instr)   ( ( instr & 0x01F00000 ) >> 20 )
#define     RV_FIND_U_IMM(instr)    ( instr >> 12 )
#define     RV_FIND_S_IMM(instr)    ( ( ( instr & 0xFE000000 ) >> 20 ) | ( ( instr & 0xFE000F80 ) >> 7 ) )
#define     RV_FIND_J_IMM(instr)    ( ( ( instr & 0x80000000 ) >> 11 ) | ( ( instr & 0x000FF000 ) >> 0 ) | ( ( instr & 0x00100000 ) >> 9 )  | ( ( instr & 0x7FE00000 ) >> 20 ) )
#define     RV_FIND_B_IMM(instr)    ( ( ( instr & 0x80000000 ) >> 19 ) | ( ( instr & 0x00000080 ) << 4 ) | ( ( instr & 0x7E000000 ) >> 20 ) | ( ( instr & 0x00000F00 ) >> 7  ) )

#define     RVI         0b11

#define     R_OP_0      0b01100 
#define     U_OP_0      0b01101 
#define     U_OP_1      0b00101 
#define     J_OP_0      0b11011 
#define     S_OP_0      0b01000 
#define     B_OP_0      0b11000 
#define     I_OP_0      0b00100 
#define     I_OP_1      0b00000 
#define     I_OP_2      0b11001 
#define     CSR_OP_0    0b11100

#define     BEQ_F3      0b000
#define     BNE_F3      0b001
#define     BLT_F3      0b100
#define     BGE_F3      0b101
#define     BLTU_F3     0b110
#define     BGEU_F3     0b111

#define     LB_F3       0b000
#define     LH_F3       0b001
#define     LW_F3       0b010
#define     LBU_F3      0b100
#define     LHU_F3      0b101

#define     SB_F3       0b000
#define     SH_F3       0b001
#define     SW_F3       0b010

#define     ADD_F3      0b000
#define     SUB_F3      0b000
#define     SLL_F3      0b001
#define     SLT_F3      0b010
#define     SLTU_F3     0b011
#define     XOR_F3      0b100
#define     SRL_F3      0b101
#define     SRA_F3      0b101
#define     OR_F3       0b110
#define     AND_F3      0b111

#define     ADDI_F3     ADD_F3
#define     SLLI_F3     SLL_F3
#define     SLTI_F3     SLT_F3
#define     SLTUI_F3    SLTU_F3
#define     XORI_F3     XOR_F3
#define     SRLI_F3     SRL_F3
#define     SRAI_F3     SRA_F3
#define     ORI_F3      OR_F3
#define     ANDI_F3     AND_F3

#define     JALR_F3     0b000

#define     Z_F3        0b000

#define     SRA_F7      0b0100000
#define     SRAI_F7     SRA_F7
#define     SUB_F7      0b0000000
#define     Z_F7        0b0000000

#define     WFI_F3      0b000
#define     MRET_F3     0b000
#define     ECALL_F3    0b000
#define     EBREAK_F3   0b000
#define     CSRRW_F3    0b001
#define     CSRRS_F3    0b010
#define     CSRRC_F3    0b011
#define     CSRRWI_F3   0b101
#define     CSRRSI_F3   0b110
#define     CSRRCI_F3   0b111


#define     WFI_F12     0b000100000101
#define     MRET_F12    0b001100000010
#define     ECALL_F12   0b000000000000
#define     EBREAK_F12  0b000000000001
#define     Z_F12       0b000000000000

struct instr_s {
    string          instr_name;
    char            instr_type;
    int             instr_v;
    int             instr_m;
    char            rd_pos;
    char            rs1_pos;
    char            rs2_pos;
    void print_instr() {
        cout << instr_name << " " << instr_type << " " << instr_v << " " << rd_pos << " " << rs1_pos << " " << rs2_pos << endl;
    }
};

struct csr {
    string      csr_name;
    int         csr_addr;
};

instr_s i_list[] = {
    // R type
    RV_FORM_INSTR( "ADD   " , 'R' , Z_F7    , ADD_F3   , R_OP_0 , RVI , RV_FORM_MASK(1,1,1,1) ,  7 , 15 , 20 ),
    RV_FORM_INSTR( "SUB   " , 'R' , SUB_F7  , SUB_F3   , R_OP_0 , RVI , RV_FORM_MASK(1,1,1,1) ,  7 , 15 , 20 ),
    RV_FORM_INSTR( "SLL   " , 'R' , Z_F7    , SLL_F3   , R_OP_0 , RVI , RV_FORM_MASK(1,1,1,1) ,  7 , 15 , 20 ),
    RV_FORM_INSTR( "SLT   " , 'R' , Z_F7    , SLT_F3   , R_OP_0 , RVI , RV_FORM_MASK(1,1,1,1) ,  7 , 15 , 20 ),
    RV_FORM_INSTR( "SLTU  " , 'R' , Z_F7    , SLTU_F3  , R_OP_0 , RVI , RV_FORM_MASK(1,1,1,1) ,  7 , 15 , 20 ),
    RV_FORM_INSTR( "XOR   " , 'R' , Z_F7    , XOR_F3   , R_OP_0 , RVI , RV_FORM_MASK(1,1,1,1) ,  7 , 15 , 20 ),
    RV_FORM_INSTR( "SRL   " , 'R' , Z_F7    , SRL_F3   , R_OP_0 , RVI , RV_FORM_MASK(1,1,1,1) ,  7 , 15 , 20 ),
    RV_FORM_INSTR( "SRA   " , 'R' , SRA_F7  , SRA_F3   , R_OP_0 , RVI , RV_FORM_MASK(1,1,1,1) ,  7 , 15 , 20 ),
    RV_FORM_INSTR( "OR    " , 'R' , Z_F7    , OR_F3    , R_OP_0 , RVI , RV_FORM_MASK(1,1,1,1) ,  7 , 15 , 20 ),
    RV_FORM_INSTR( "AND   " , 'R' , Z_F7    , AND_F3   , R_OP_0 , RVI , RV_FORM_MASK(1,1,1,1) ,  7 , 15 , 20 ),
    // I type
    // load instr
    RV_FORM_INSTR( "LB    " , 'I' , Z_F7    , LB_F3    , I_OP_1 , RVI , RV_FORM_MASK(0,1,1,1) ,  7 , 15 , 20 ),
    RV_FORM_INSTR( "LH    " , 'I' , Z_F7    , LH_F3    , I_OP_1 , RVI , RV_FORM_MASK(0,1,1,1) ,  7 , 15 , 20 ),
    RV_FORM_INSTR( "LW    " , 'I' , Z_F7    , LW_F3    , I_OP_1 , RVI , RV_FORM_MASK(0,1,1,1) ,  7 , 15 , 20 ),
    RV_FORM_INSTR( "LBU   " , 'I' , Z_F7    , LBU_F3   , I_OP_1 , RVI , RV_FORM_MASK(0,1,1,1) ,  7 , 15 , 20 ),
    RV_FORM_INSTR( "LHU   " , 'I' , Z_F7    , LHU_F3   , I_OP_1 , RVI , RV_FORM_MASK(0,1,1,1) ,  7 , 15 , 20 ),
    // logic and ariphmetic instr
    RV_FORM_INSTR( "ADDI  " , 'I' , Z_F7    , ADDI_F3  , I_OP_0 , RVI , RV_FORM_MASK(0,1,1,1) ,  7 , 15 , 20 ),
    RV_FORM_INSTR( "SLLI  " , 'I' , Z_F7    , SLLI_F3  , I_OP_0 , RVI , RV_FORM_MASK(1,1,1,1) ,  7 , 15 , 20 ),
    RV_FORM_INSTR( "SLTI  " , 'I' , Z_F7    , SLTI_F3  , I_OP_0 , RVI , RV_FORM_MASK(0,1,1,1) ,  7 , 15 , 20 ),
    RV_FORM_INSTR( "SLTUI " , 'I' , Z_F7    , SLTUI_F3 , I_OP_0 , RVI , RV_FORM_MASK(0,1,1,1) ,  7 , 15 , 20 ),
    RV_FORM_INSTR( "XORI  " , 'I' , Z_F7    , XORI_F3  , I_OP_0 , RVI , RV_FORM_MASK(0,1,1,1) ,  7 , 15 , 20 ),
    RV_FORM_INSTR( "SRLI  " , 'I' , Z_F7    , SRLI_F3  , I_OP_0 , RVI , RV_FORM_MASK(1,1,1,1) ,  7 , 15 , 20 ),
    RV_FORM_INSTR( "SRAI  " , 'I' , SRAI_F7 , SRAI_F3  , I_OP_0 , RVI , RV_FORM_MASK(1,1,1,1) ,  7 , 15 , 20 ),
    RV_FORM_INSTR( "ORI   " , 'I' , Z_F7    , ORI_F3   , I_OP_0 , RVI , RV_FORM_MASK(0,1,1,1) ,  7 , 15 , 20 ),
    RV_FORM_INSTR( "ANDI  " , 'I' , Z_F7    , ANDI_F3  , I_OP_0 , RVI , RV_FORM_MASK(0,1,1,1) ,  7 , 15 , 20 ),
    // jalr instr
    RV_FORM_INSTR( "JALR  " , 'I' , Z_F7    , JALR_F3  , I_OP_2 , RVI , RV_FORM_MASK(0,1,1,1) ,  7 , 15 , 20 ),
    // S type
    // store instr
    RV_FORM_INSTR( "SB    " , 'S' , Z_F7    , SB_F3    , S_OP_0 , RVI , RV_FORM_MASK(0,1,1,1) , -1 , 15 , 20 ),
    RV_FORM_INSTR( "SH    " , 'S' , Z_F7    , SH_F3    , S_OP_0 , RVI , RV_FORM_MASK(0,1,1,1) , -1 , 15 , 20 ),
    RV_FORM_INSTR( "SW    " , 'S' , Z_F7    , SW_F3    , S_OP_0 , RVI , RV_FORM_MASK(0,1,1,1) , -1 , 15 , 20 ),
    // B type
    // branch instr
    RV_FORM_INSTR( "BEQ   " , 'B' , Z_F7    , BEQ_F3   , B_OP_0 , RVI , RV_FORM_MASK(0,1,1,1) , -1 , 15 , 20 ),
    RV_FORM_INSTR( "BNE   " , 'B' , Z_F7    , BNE_F3   , B_OP_0 , RVI , RV_FORM_MASK(0,1,1,1) , -1 , 15 , 20 ),
    RV_FORM_INSTR( "BLT   " , 'B' , Z_F7    , BLT_F3   , B_OP_0 , RVI , RV_FORM_MASK(0,1,1,1) , -1 , 15 , 20 ),
    RV_FORM_INSTR( "BGE   " , 'B' , Z_F7    , BGE_F3   , B_OP_0 , RVI , RV_FORM_MASK(0,1,1,1) , -1 , 15 , 20 ),
    RV_FORM_INSTR( "BLTU  " , 'B' , Z_F7    , BLTU_F3  , B_OP_0 , RVI , RV_FORM_MASK(0,1,1,1) , -1 , 15 , 20 ),
    RV_FORM_INSTR( "BGEU  " , 'B' , Z_F7    , BGEU_F3  , B_OP_0 , RVI , RV_FORM_MASK(0,1,1,1) , -1 , 15 , 20 ),
    // U type
    // LUI
    RV_FORM_INSTR( "LUI   " , 'U' , Z_F7    , Z_F3     , U_OP_0 , RVI , RV_FORM_MASK(0,0,1,1) ,  7 , -1 , -1 ),
    // AUIPC
    RV_FORM_INSTR( "AUIPC " , 'U' , Z_F7    , Z_F3     , U_OP_1 , RVI , RV_FORM_MASK(0,0,1,1) ,  7 , -1 , -1 ),
    // J type
    // jal instr
    RV_FORM_INSTR( "JAL   " , 'J' , Z_F7    , Z_F3     , J_OP_0 , RVI , RV_FORM_MASK(0,0,1,1) ,  7 , -1 , -1 ),
    // CSR
    RV_FORM_CSR_INSTR( "WFI   " , 'D' , WFI_F12    , WFI_F3    , CSR_OP_0 , RVI , RV_FORM_CSR_MASK(1,1,1,1) , -1 , -1 , -1 ),
    RV_FORM_CSR_INSTR( "MRET  " , 'D' , MRET_F12   , MRET_F3   , CSR_OP_0 , RVI , RV_FORM_CSR_MASK(1,1,1,1) , -1 , -1 , -1 ),
    RV_FORM_CSR_INSTR( "ECALL " , 'D' , ECALL_F12  , ECALL_F3  , CSR_OP_0 , RVI , RV_FORM_CSR_MASK(1,1,1,1) , -1 , -1 , -1 ),
    RV_FORM_CSR_INSTR( "EBREAK" , 'D' , EBREAK_F12 , EBREAK_F3 , CSR_OP_0 , RVI , RV_FORM_CSR_MASK(1,1,1,1) , -1 , -1 , -1 ),
    RV_FORM_CSR_INSTR( "CSRRW " , 'D' , Z_F12      , CSRRW_F3  , CSR_OP_0 , RVI , RV_FORM_CSR_MASK(0,1,1,1) ,  7 , 15 , -1 ),
    RV_FORM_CSR_INSTR( "CSRRS " , 'D' , Z_F12      , CSRRS_F3  , CSR_OP_0 , RVI , RV_FORM_CSR_MASK(0,1,1,1) ,  7 , 15 , -1 ),
    RV_FORM_CSR_INSTR( "CSRRC " , 'D' , Z_F12      , CSRRC_F3  , CSR_OP_0 , RVI , RV_FORM_CSR_MASK(0,1,1,1) ,  7 , 15 , -1 ),
    RV_FORM_CSR_INSTR( "CSRRWI" , 'D' , Z_F12      , CSRRWI_F3 , CSR_OP_0 , RVI , RV_FORM_CSR_MASK(0,1,1,1) ,  7 , -1 , -1 ),
    RV_FORM_CSR_INSTR( "CSRRSI" , 'D' , Z_F12      , CSRRSI_F3 , CSR_OP_0 , RVI , RV_FORM_CSR_MASK(0,1,1,1) ,  7 , -1 , -1 ),
    RV_FORM_CSR_INSTR( "CSRRCI" , 'D' , Z_F12      , CSRRCI_F3 , CSR_OP_0 , RVI , RV_FORM_CSR_MASK(0,1,1,1) ,  7 , -1 , -1 )
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

    csr csr_list_[221];

    rv_instr() {
        string var;
        ifstream csr_file("csr_file_list.txt", fstream::in);
        for(int i = 0 ; i < 221 ; i ++) {
            csr_file >> csr_list_[i].csr_name;
            csr_file >> hex >> csr_list_[i].csr_addr;
        }

        for(int i = 0 ; i < 221 ; i ++) {
            cout << csr_list_[i].csr_name << " ";
            cout << hex << csr_list_[i].csr_addr;
            cout << endl;
        }
    }

    unsigned int    instr;

    void pars_instr(instr_s instr_s_p) {
        cout << instr_s_p.instr_name << " ";
        if( instr_s_p.rd_pos != -1 ) {
            cout << reg_list[RV_FIND_RD(instr)] << " ";
        }
        if( instr_s_p.rs1_pos != -1 ) {
            cout << reg_list[RV_FIND_RS1(instr)] << " ";
        }
        if( instr_s_p.rs2_pos != -1 ) {
            cout << reg_list[RV_FIND_RS2(instr)] << " ";
        }
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
