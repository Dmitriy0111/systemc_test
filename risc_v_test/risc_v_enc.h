#include <iostream>
#include <iomanip>
#include <stdio.h>

using namespace std;

#define     RV_FORM_INSTR(name,i_type,comp,f7,f3,opcode,type)    { name, i_type, comp, f7, f3, opcode, type }
#define     RV_FIND_OPCODE(instr)   ( ( instr >> 2 ) & 0b11111 )
#define     RV_FIND_ITYPE(instr)    ( instr & 0b11 )
#define     RV_FIND_F3(instr)       ( ( instr >> 12 ) & 0b111 )
#define     RV_FIND_F7(instr)       ( ( instr >> 25 ) & 0b1111111 )
#define     RV_FIND_RS1(instr)      ( ( instr >> 15 ) & 0b11111 )
#define     RV_FIND_RS2(instr)      ( ( instr >> 20 ) & 0b11111 )
#define     RV_FIND_RD(instr)       ( ( instr >> 7  ) & 0b11111 )
#define     RV_FIND_I_IMM(instr)    ( instr >> 20 )
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

struct instr_s {
    string          instr_name;
    char            instr_type;
    unsigned int    comp;
    unsigned int    f7     : 7;
    unsigned int    f3     : 5;
    unsigned int    opcode : 5;
    unsigned int    type   : 2;
};

instr_s i_list[] = {
    // R type
    RV_FORM_INSTR( "ADD  " , 'R' , 7 , Z_F7    , ADD_F3   , R_OP_0 , RVI ),
    RV_FORM_INSTR( "SUB  " , 'R' , 7 , SUB_F7  , SUB_F3   , R_OP_0 , RVI ),
    RV_FORM_INSTR( "SLL  " , 'R' , 3 , Z_F7    , SLL_F3   , R_OP_0 , RVI ),
    RV_FORM_INSTR( "SLT  " , 'R' , 3 , Z_F7    , SLT_F3   , R_OP_0 , RVI ),
    RV_FORM_INSTR( "SLTU " , 'R' , 3 , Z_F7    , SLTU_F3  , R_OP_0 , RVI ),
    RV_FORM_INSTR( "XOR  " , 'R' , 3 , Z_F7    , XOR_F3   , R_OP_0 , RVI ),
    RV_FORM_INSTR( "SRL  " , 'R' , 7 , Z_F7    , SRL_F3   , R_OP_0 , RVI ),
    RV_FORM_INSTR( "SRA  " , 'R' , 7 , SRA_F7  , SRA_F3   , R_OP_0 , RVI ),
    RV_FORM_INSTR( "OR   " , 'R' , 3 , Z_F7    , OR_F3    , R_OP_0 , RVI ),
    RV_FORM_INSTR( "AND  " , 'R' , 3 , Z_F7    , AND_F3   , R_OP_0 , RVI ),
    // I type
    // load instr
    RV_FORM_INSTR( "LB   " , 'I' , 3 , Z_F7    , LB_F3    , I_OP_1 , RVI ),
    RV_FORM_INSTR( "LH   " , 'I' , 3 , Z_F7    , LH_F3    , I_OP_1 , RVI ),
    RV_FORM_INSTR( "LW   " , 'I' , 3 , Z_F7    , LW_F3    , I_OP_1 , RVI ),
    RV_FORM_INSTR( "LBU  " , 'I' , 3 , Z_F7    , LBU_F3   , I_OP_1 , RVI ),
    RV_FORM_INSTR( "LHU  " , 'I' , 3 , Z_F7    , LHU_F3   , I_OP_1 , RVI ),
    // logic and ariphmetic instr
    RV_FORM_INSTR( "ADDI " , 'I' , 3 , Z_F7    , ADDI_F3  , I_OP_0 , RVI ),
    RV_FORM_INSTR( "SLLI " , 'I' , 3 , Z_F7    , SLLI_F3  , I_OP_0 , RVI ),
    RV_FORM_INSTR( "SLTI " , 'I' , 3 , Z_F7    , SLTI_F3  , I_OP_0 , RVI ),
    RV_FORM_INSTR( "SLTUI" , 'I' , 3 , Z_F7    , SLTUI_F3 , I_OP_0 , RVI ),
    RV_FORM_INSTR( "XORI " , 'I' , 3 , Z_F7    , XORI_F3  , I_OP_0 , RVI ),
    RV_FORM_INSTR( "SRLI " , 'I' , 7 , Z_F7    , SRLI_F3  , I_OP_0 , RVI ),
    RV_FORM_INSTR( "SRAI " , 'I' , 7 , SRAI_F7 , SRAI_F3  , I_OP_0 , RVI ),
    RV_FORM_INSTR( "ORI  " , 'I' , 3 , Z_F7    , ORI_F3   , I_OP_0 , RVI ),
    RV_FORM_INSTR( "ANDI " , 'I' , 3 , Z_F7    , ANDI_F3  , I_OP_0 , RVI ),
    // jalr instr
    RV_FORM_INSTR( "JALR " , 'I' , 3 , Z_F7    , JALR_F3  , I_OP_2 , RVI ),
    // S type
    // store instr
    RV_FORM_INSTR( "SB   " , 'S' , 3 , Z_F7    , SB_F3    , S_OP_0 , RVI ),
    RV_FORM_INSTR( "SH   " , 'S' , 3 , Z_F7    , SH_F3    , S_OP_0 , RVI ),
    RV_FORM_INSTR( "SW   " , 'S' , 3 , Z_F7    , SW_F3    , S_OP_0 , RVI ),
    // B type
    // branch instr
    RV_FORM_INSTR( "BEQ  " , 'B' , 3 , Z_F7    , BEQ_F3   , B_OP_0 , RVI ),
    RV_FORM_INSTR( "BNE  " , 'B' , 3 , Z_F7    , BNE_F3   , B_OP_0 , RVI ),
    RV_FORM_INSTR( "BLT  " , 'B' , 3 , Z_F7    , BLT_F3   , B_OP_0 , RVI ),
    RV_FORM_INSTR( "BGE  " , 'B' , 3 , Z_F7    , BGE_F3   , B_OP_0 , RVI ),
    RV_FORM_INSTR( "BLTU " , 'B' , 3 , Z_F7    , BLTU_F3  , B_OP_0 , RVI ),
    RV_FORM_INSTR( "BGEU " , 'B' , 3 , Z_F7    , BGEU_F3  , B_OP_0 , RVI ),
    // U type
    // LUI
    RV_FORM_INSTR( "LUI  " , 'U' , 1 , Z_F7    , Z_F3     , U_OP_0 , RVI ),
    // AUIPC
    RV_FORM_INSTR( "AUIPC" , 'U' , 1 , Z_F7    , Z_F3     , U_OP_1 , RVI ),
    // J type
    // jal instr
    RV_FORM_INSTR( "JAL  " , 'J' , 1 , Z_F7    , Z_F3     , J_OP_0 , RVI )
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

    unsigned int    instr;

    void pars_type_instr(char type) {
        switch( type ) {
            case 'R':   pars_r_type_instr(); break;
            case 'I':   pars_i_type_instr(); break;
            case 'U':   pars_u_type_instr(); break;
            case 'S':   pars_s_type_instr(); break;
            case 'J':   pars_j_type_instr(); break;
            case 'B':   pars_b_type_instr(); break;
        }
    }

    void pars_i_type_instr() {
        cout << reg_list[RV_FIND_RD(instr)] << " ";
        cout << reg_list[RV_FIND_RS1(instr)] << " ";
        cout << "0x" << hex << RV_FIND_I_IMM(instr) << endl;
    }

    void pars_r_type_instr() {
        cout << reg_list[RV_FIND_RD(instr)] << " ";
        cout << reg_list[RV_FIND_RS1(instr)] << " ";
        cout << reg_list[RV_FIND_RS2(instr)] << endl;
    }

    void pars_u_type_instr() {
        cout << reg_list[RV_FIND_RD(instr)] << " ";
        cout << "0x" << hex << RV_FIND_U_IMM(instr) << endl;
    }

    void pars_j_type_instr() {
        cout << reg_list[RV_FIND_RD(instr)] << " ";
        cout << "0x" << hex << RV_FIND_J_IMM(instr) << endl;
    }

    void pars_s_type_instr() {
        cout << reg_list[RV_FIND_RS1(instr)] << " ";
        cout << reg_list[RV_FIND_RS2(instr)] << " ";
        cout << "0x" << hex << RV_FIND_S_IMM(instr) << endl;
    }

    void pars_b_type_instr() {
        cout << reg_list[RV_FIND_RS1(instr)] << " ";
        cout << reg_list[RV_FIND_RS2(instr)] << " ";
        cout << "0x" << hex << RV_FIND_B_IMM(instr) << endl;
    }

    void load_instr(unsigned int instr_in) {
        instr = instr_in;
    }

    void pars_instr() {
        switch( RV_FIND_ITYPE(instr) ) {
        case    RVI:
                for( int i = 0 ; i < 37 ; i++ ) {
                    if( 
                        ( 
                            ( ( ( RV_FIND_OPCODE(instr) == i_list[i].opcode ) ? 1 : 0 ) << 0 ) |
                            ( ( ( RV_FIND_F3(instr)     == i_list[i].f3     ) ? 1 : 0 ) << 1 ) | 
                            ( ( ( RV_FIND_F7(instr)     == i_list[i].f7     ) ? 1 : 0 ) << 2 )
                        ) == i_list[i].comp
                    ) {
                        cout << i_list[i].instr_name << " ";
                        pars_type_instr(i_list[i].instr_type);
                    }
                }
            break;
        default:
            cout << "Not RVI instruction" << endl;
            break;
        }
    }
};
