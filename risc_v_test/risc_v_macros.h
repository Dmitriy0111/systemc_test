#define     RV_FORM_INSTR(name,i_type,f7,f3,opcode,type,mask,op)        \
                                        { name, i_type, ( ( f7 << 25 ) | ( f3 << 12 ) | ( opcode <<  2 ) | ( type << 0 ) ),mask, op }
#define     RV_FORM_CSR_INSTR(name,i_type,f12,f3,opcode,type,mask,op)   \
                                        { name, i_type, ( ( f12 << 20 ) | ( f3 << 12 ) | ( opcode <<  2 ) | ( type << 0 ) ),mask, op }
#define     RV_FORM_MASK(f7,f3,opcode,type)                             \
                                        (int)( ( type == 1 ? 0b11 : 0b00 ) | ( opcode == 1 ? 0x7C : 0x00 ) | ( f3 == 1 ? 0x7000 : 0x0000 ) | ( f7 == 1 ? 0xFE000000 : 0x00000000 ) )
#define     RV_FORM_CSR_MASK(f12,f3,opcode,type)                        \
                                        (int)( ( type == 1 ? 0b11 : 0b00 ) | ( opcode == 1 ? 0x7C : 0x00 ) | ( f3 == 1 ? 0x7000 : 0x0000 ) | ( f12 == 1 ? 0xFFF00000 : 0x00000000 ) )
#define     RV_FIND_OPCODE(instr)       ( ( instr >> 2 ) & 0b11111 )
#define     RV_FIND_ITYPE(instr)        ( instr & 0b11 )
#define     RV_FIND_F3(instr)           ( ( instr >> 12 ) & 0b111 )
#define     RV_FIND_F7(instr)           ( ( instr >> 25 ) & 0b1111111 )
#define     RV_FIND_RS1(instr)          ( ( instr >> 15 ) & 0b11111 )
#define     RV_FIND_RS2(instr)          ( ( instr >> 20 ) & 0b11111 )
#define     RV_FIND_RD(instr)           ( ( instr >> 7  ) & 0b11111 )
#define     RV_FIND_CSR_ADDR(instr)     ( instr >> 20 )
#define     RV_FIND_CSR_ZIMM(instr)     ( ( instr >> 15 ) & 0b11111 )
#define     RV_FIND_I_IMM(instr)        ( instr >> 20 )
#define     RV_FIND_IS_IMM(instr)       ( ( instr & 0x01F00000 ) >> 20 )
#define     RV_FIND_U_IMM(instr)        ( instr >> 12 )
#define     RV_FIND_S_IMM(instr)        ( ( ( instr & 0xFE000000 ) >> 20 ) | ( ( instr & 0xFE000F80 ) >> 7 ) )
#define     RV_FIND_J_IMM(instr)        ( ( ( instr & 0x80000000 ) >> 11 ) | ( ( instr & 0x000FF000 ) >> 0 ) | ( ( instr & 0x00100000 ) >> 9 )  | ( ( instr & 0x7FE00000 ) >> 20 ) )
#define     RV_FIND_B_IMM(instr)        ( ( ( instr & 0x80000000 ) >> 19 ) | ( ( instr & 0x00000080 ) << 4 ) | ( ( instr & 0x7E000000 ) >> 20 ) | ( ( instr & 0x00000F00 ) >> 7  ) )
#define     RV_FIND_RD_MASK(mask)       ( ( mask & 0b1000 ) >> 3 )
#define     RV_FIND_RS1_MASK(mask)      ( ( mask & 0b0100 ) >> 2 )
#define     RV_FIND_RS2_MASK(mask)      ( ( mask & 0b0010 ) >> 1 )
#define     RV_FIND_IMM_MASK(mask)      ( ( mask & 0b0001 ) >> 0 )