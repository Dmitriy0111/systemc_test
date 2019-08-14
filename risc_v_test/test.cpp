#include "risc_v_enc.h"

rv_instr    *rv_instr_p;

unsigned int prog[] =   {
                            0x0400006f,     //   0: 0400006f    jal     zero,40 
                            0x0dc0006f,     //   4: 0dc0006f    jal     zero,e0 
                            0x1700006f,     //   8: 1700006f    jal     zero,178
                            0x1700006f,     //   c: 1700006f    jal     zero,17c
                            0x1700006f,     //  10: 1700006f    jal     zero,180
                            0x1700006f,     //  14: 1700006f    jal     zero,184
                            0x1700006f,     //  18: 1700006f    jal     zero,188
                            0x1700006f,     //  1c: 1700006f    jal     zero,18c
                            0x1700006f,     //  20: 1700006f    jal     zero,190
                            0x1700006f,     //  24: 1700006f    jal     zero,194
                            0x1700006f,     //  28: 1700006f    jal     zero,198
                            0x1700006f,     //  2c: 1700006f    jal     zero,19c
                            0x1700006f,     //  30: 1700006f    jal     zero,1a0
                            0x1700006f,     //  34: 1700006f    jal     zero,1a4
                            0x1700006f,     //  38: 1700006f    jal     zero,1a8
                            0x1700006f,     //  3c: 1700006f    jal     zero,1ac
                            0x000000b7,     //  40: 000000b7    lui     ra,0x0
                            0x00000137,     //  44: 00000137    lui     sp,0x0
                            0x000001b7,     //  48: 000001b7    lui     gp,0x0
                            0x00000237,     //  4c: 00000237    lui     tp,0x0
                            0x000002b7,     //  50: 000002b7    lui     t0,0x0
                            0x00000337,     //  54: 00000337    lui     t1,0x0
                            0x000003b7,     //  58: 000003b7    lui     t2,0x0
                            0x00000437,     //  5c: 00000437    lui     s0,0x0
                            0x000004b7,     //  60: 000004b7    lui     s1,0x0
                            0x00000537,     //  64: 00000537    lui     a0,0x0
                            0x000005b7,     //  68: 000005b7    lui     a1,0x0
                            0x00000637,     //  6c: 00000637    lui     a2,0x0
                            0x000006b7,     //  70: 000006b7    lui     a3,0x0
                            0x00000737,     //  74: 00000737    lui     a4,0x0
                            0x000007b7,     //  78: 000007b7    lui     a5,0x0
                            0x00000837,     //  7c: 00000837    lui     a6,0x0
                            0x000008b7,     //  80: 000008b7    lui     a7,0x0
                            0x00000937,     //  84: 00000937    lui     s2,0x0
                            0x000009b7,     //  88: 000009b7    lui     s3,0x0
                            0x00000a37,     //  8c: 00000a37    lui     s4,0x0
                            0x00000ab7,     //  90: 00000ab7    lui     s5,0x0
                            0x00000b37,     //  94: 00000b37    lui     s6,0x0
                            0x00000bb7,     //  98: 00000bb7    lui     s7,0x0
                            0x00000c37,     //  9c: 00000c37    lui     s8,0x0
                            0x00000cb7,     //  a0: 00000cb7    lui     s9,0x0
                            0x00000d37,     //  a4: 00000d37    lui     s10,0x0
                            0x00000db7,     //  a8: 00000db7    lui     s11,0x0
                            0x00000e37,     //  ac: 00000e37    lui     t3,0x0
                            0x00000eb7,     //  b0: 00000eb7    lui     t4,0x0
                            0x00000f37,     //  b4: 00000f37    lui     t5,0x0
                            0x00000fb7,     //  b8: 00000fb7    lui     t6,0x0
                            0x00001137,     //  bc: 00001137    lui     sp,0x1
                            0xf0010113,     //  c0: f0010113    addi    sp,sp,-256 # f00
                            0x0040006f,     //  c4: 0040006f    jal     zero,c8
                            0xff010113,     //  c8: ff010113    addi    sp,sp,-16
                            0x00012623,     //  cc: 00012623    sw      zero,12(sp)
                            0x00c12783,     //  d0: 00c12783    lw      a5,12(sp)
                            0x00178793,     //  d4: 00178793    addi    a5,a5,1 # 1
                            0x00f12623,     //  d8: 00f12623    sw      a5,12(sp)
                            0xff5ff06f,     //  dc: ff5ff06f    jal     zero,d0
                            0x14202773,     //  e0: 14202773    csrrs   a4,scause,zeros
                            0x00500793,     //  e4: 00500793    addi    a5,zero,5
                            0x00f70e63,     //  e8: 00f70e63    beq     a4,a5,104
                            0x000017b7,     //  ec: 000017b7    lui     a5,0x1
                            0xf0078613,     //  f0: f0078613    addi    a2,a5,-256 # f00
                            0xff860813,     //  f4: ff860813    addi    a6,a2,-8 # fffffff8
                            0xf0078793,     //  f8: f0078793    addi    a5,a5,-256
                            0x00900513,     //  fc: 00900513    addi    a0,zero,9
                            0x0200006f,     // 100: 0200006f    jal     zero,120
                            0x00001137,     // 104: 00001137    lui     sp,0x1
                            0xf0010113,     // 108: f0010113    addi    sp,sp,-256 # f00
                            0xfe1ff06f,     // 10c: fe1ff06f    jal     zero,ec
                            0x00b78d23,     // 110: 00b78d23    sb      a1,26(a5)
                            0x40475713,     // 114: 40475713    srai    a4,a4,0x4
                            0xfff78793,     // 118: fff78793    addi    a5,a5,-1
                            0x01078c63,     // 11c: 01078c63    beq     a5,a6,134
                            0x00f77693,     // 120: 00f77693    andi    a3,a4,15
                            0x03768593,     // 124: 03768593    addi    a1,a3,55 # 37
                            0xfed564e3,     // 128: fed564e3    bltu    a0,a3,110
                            0x03068593,     // 12c: 03068593    addi    a1,a3,48
                            0xfe1ff06f,     // 130: fe1ff06f    jal     zero,110
                            0x000307b7,     // 134: 000307b7    lui     a5,0x30
                            0x1b200713,     // 138: 1b200713    addi    a4,zero,434
                            0x00e7a623,     // 13c: 00e7a623    sw      a4,12(a5) # 3000c
                            0x00400713,     // 140: 00400713    addi    a4,zero,4
                            0x00e7a023,     // 144: 00e7a023    sw      a4,0(a5)
                            0x01d60513,     // 148: 01d60513    addi    a0,a2,29
                            0x00030737,     // 14c: 00030737    lui     a4,0x30
                            0x00500693,     // 150: 00500693    addi    a3,zero,5
                            0x00068593,     // 154: 00068593    addi    a1,a3,0
                            0x00064783,     // 158: 00064783    lbu     a5,0(a2)
                            0x00f72223,     // 15c: 00f72223    sw      a5,4(a4) # 30004
                            0x00b72023,     // 160: 00b72023    sw      a1,0(a4)
                            0x00072783,     // 164: 00072783    lw      a5,0(a4)
                            0xfed78ee3,     // 168: fed78ee3    beq     a5,a3,164
                            0x00160613,     // 16c: 00160613    addi    a2,a2,1
                            0xfea614e3,     // 170: fea614e3    bne     a2,a0,158
                            0x0000006f,     // 174: 0000006f    jal     zero,174
                            0x0000006f,     // 178: 0000006f    jal     zero,178
                            0x0000006f,     // 17c: 0000006f    jal     zero,17c
                            0x0000006f,     // 180: 0000006f    jal     zero,180
                            0x0000006f,     // 184: 0000006f    jal     zero,184
                            0x0000006f,     // 188: 0000006f    jal     zero,188
                            0x0000006f,     // 18c: 0000006f    jal     zero,18c
                            0x0000006f,     // 190: 0000006f    jal     zero,190
                            0x0000006f,     // 194: 0000006f    jal     zero,194
                            0x0000006f,     // 198: 0000006f    jal     zero,198
                            0x0000006f,     // 19c: 0000006f    jal     zero,19c
                            0x0000006f,     // 1a0: 0000006f    jal     zero,1a0
                            0x0000006f,     // 1a4: 0000006f    jal     zero,1a4
                            0x0000006f,     // 1a8: 0000006f    jal     zero,1a8
                            0x0000006f      // 1ac: 0000006f    jal     zero,1ac
                        };

int main() {
    rv_instr_p = new rv_instr();
    cout << "Test program" << endl;
    cout << "Program size = " << sizeof(prog) / 4 << endl;
    for( int i = 0 ; i < sizeof(prog) / 4 ; i++ ) {
        rv_instr_p->load_instr(prog[i]);
        rv_instr_p->pars_instr();
    }
    return 0;
}
