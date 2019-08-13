#include <iostream>

using namespace std;

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
#define     RV_FIND_B_IMM(instr)    ( ( ( instr & 0x80000000 ) >> 19 ) | ( ( instr & 0x00000080 ) << 4 ) | ( ( instr & 0x7E000000 ) >> 20 ) | ( ( instr & 0x00000F00 ) >> 7 ) )

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

        void load_instr(unsigned int instr_in) {
            instr = instr_in;
        }

        void pars_instr() {
            switch( RV_FIND_ITYPE(instr) ) {
                case    RVI:
                    switch( RV_FIND_OPCODE(instr) ) {
                        case    R_OP_0:
                            cout << "R_OP" << " ";
                            cout << "rd  = " << reg_list[RV_FIND_RD(instr)] << " ";
                            cout << "rs1 = " << reg_list[RV_FIND_RS1(instr)] << " ";
                            cout << "rs2 = " << reg_list[RV_FIND_RS2(instr)] << endl;
                        break;
                        case    U_OP_0:
                        case    U_OP_1:
                            cout << "U_OP" << " ";
                            cout << "rd  = " << reg_list[RV_FIND_RD(instr)] << " ";
                            cout << "0x" << hex << RV_FIND_U_IMM(instr) << endl;
                        break;
                        case    J_OP_0:
                            cout << "J_OP" << endl;
                        break;
                        case    S_OP_0:
                            cout << "S_OP" << " ";
                            cout << "rs1 = " << reg_list[RV_FIND_RS1(instr)] << " ";
                            cout << "rs2 = " << reg_list[RV_FIND_RS2(instr)] << " ";
                            cout << "0x" << hex << RV_FIND_S_IMM(instr) << endl;
                        break;
                        case    B_OP_0:
                            if( RV_FIND_F3(instr) == BEQ_F3 ) 
                                cout << "BEQ  ";
                            else if( RV_FIND_F3(instr) == BNE_F3 )
                                cout << "BNE  ";
                            else if( RV_FIND_F3(instr) == BLT_F3 )
                                cout << "BLT  ";
                            else if( RV_FIND_F3(instr) == BGE_F3 )
                                cout << "BGE  ";
                            else if( RV_FIND_F3(instr) == BLTU_F3 )
                                cout << "BLTU ";
                            else if( RV_FIND_F3(instr) == BGEU_F3 )
                                cout << "BGEU ";
                            cout << reg_list[RV_FIND_RS1(instr)] << " ";
                            cout << reg_list[RV_FIND_RS2(instr)] << " ";
                            cout << "0x" << hex << RV_FIND_B_IMM( instr ) << endl;
                        break;
                        case    I_OP_0:
                        case    I_OP_1:
                        case    I_OP_2:
                            cout << "I_OP" << " ";
                            cout << "rd  = " << reg_list[RV_FIND_RD(instr)] << " ";
                            cout << "rs1 = " << reg_list[RV_FIND_RS1(instr)] << " ";
                            cout << "imm = " << hex << RV_FIND_I_IMM(instr) << endl;
                        break;
                        case    CSR_OP_0:
                            cout << "CSR_OP" << endl;
                        break;
                    }
                break;
                default:
                    cout << "Non RVI insctruction" << endl;
                break;
            }
        }
};
