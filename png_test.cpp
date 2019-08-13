#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <fstream>

using namespace std;

unsigned char   png_sig[] = { (unsigned char)0x89, 0x50, 0X4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
unsigned char*  ihdr;
unsigned char*  iend;
unsigned char*  idat;

unsigned int *crc_table;

FILE *out_file;

void find_crc(unsigned char *in_data, int length, unsigned int *crc) {
	*crc = 0;
	*crc = ~(*crc);
    unsigned int rev_polynom = 0xEDB88320;

	while (length--) {
		*crc ^= *in_data++;
		for (int i = 0; i < 8; i++)
			if ((*crc & 0x01))
				*crc = (*crc >> 1) ^ rev_polynom;
			else
				*crc = *crc >> 1;
		cout << *crc << hex << endl;
	}
	*crc = ~(*crc);
}

void write_crc2chunk(unsigned char *out_chunk, int length, unsigned int crc) {
	out_chunk += length - 4;
	*out_chunk++ = (unsigned char)(crc / 0x1000000);
	*out_chunk++ = (unsigned char)(crc / 0x10000 % 0x100);
	*out_chunk++ = (unsigned char)(crc / 0x100 % 0x100);
	*out_chunk = (unsigned char)(crc % 0x100);
	out_chunk -= length;
}

void print_hex_chunk(unsigned char *out_chunk, int length) {
	cout << setfill('0');
	for (int i = 0; i < length; i++) {
		cout << setw(2) << hex << (int)out_chunk[i] << " ";
	}
	cout << endl;
	for (int i = 0; i < length; i++) {
		printf("%.2x ", out_chunk[i]);
	}
	printf("\n");
}

unsigned int * form_crc_table(unsigned int *table) {
	table = (unsigned int *)malloc(sizeof(unsigned int) * 256);
	unsigned int crc_v;
	unsigned int rev_polynom = 0xEDB88320;
	for (int i = 0; i < 256; i++) {
		crc_v = i;
		for (int j = 0; j < 8; j++)
			crc_v = crc_v & 0x01 ? crc_v = (crc_v >> 1) ^ rev_polynom : crc_v = crc_v >> 1;
		table[i] = crc_v;
	}
	return table;
}

void find_crc_with_table(unsigned char *in_data, int length, unsigned int *table, unsigned int *crc) {
	*crc = 0;
	*crc = ~(*crc);
	for( int i = 0 ; i < length ; ++i ) {
		*crc = (*crc >> 8) ^ table[in_data[i] ^ (*crc & 0xff)];
	}
	*crc = ~(*crc);
}

void create_ihdr(int width, int height, unsigned char *out_chunk) {

	unsigned int crc;

	out_chunk[0] = 0x00;
	out_chunk[1] = 0x00;
	out_chunk[2] = 0x00;
	out_chunk[3] = 0x0D;
	// IHDR
	out_chunk[4] = 0x49;
	out_chunk[5] = 0x48;
	out_chunk[6] = 0x44;
	out_chunk[7] = 0x52;
	// Width
	out_chunk[8] = (unsigned char)(width / 0x1000000);
	out_chunk[9] = (unsigned char)(width / 0x10000 % 0x100);
	out_chunk[10] = (unsigned char)(width / 0x100 % 0x100);
	out_chunk[11] = (unsigned char)(width % 0x100);
	// Height
	out_chunk[12] = (unsigned char)(height / 0x1000000);
	out_chunk[13] = (unsigned char)(height / 0x10000 % 0x100);
	out_chunk[14] = (unsigned char)(height / 0x100 % 0x100);
	out_chunk[15] = (unsigned char)(height % 0x100);
	// bit depth
	out_chunk[16] = 0x08;
	// type color
	out_chunk[17] = 0x02;

	out_chunk[18] = 0x00;
	out_chunk[19] = 0x00;
	out_chunk[20] = 0x00;

	find_crc(out_chunk + 4, 17, &crc);
	//find_crc_with_table(out_chunk + 4, 4, crc_table, &crc);

	write_crc2chunk(out_chunk, 25, crc);
	print_hex_chunk(out_chunk, 25);
}

void create_iend(unsigned char *out_chunk) {

	unsigned int crc;

	out_chunk[0] = 0x00;
	out_chunk[1] = 0x00;
	out_chunk[2] = 0x00;
	out_chunk[3] = 0x00;

	out_chunk[4] = 0x49;
	out_chunk[5] = 0x45;
	out_chunk[6] = 0x4E;
	out_chunk[7] = 0x44;
	out_chunk[8] = 0x00;
	out_chunk[9] = 0x00;
	out_chunk[10] = 0x00;
	out_chunk[11] = 0x00;

	find_crc(out_chunk + 4, 4, &crc);
	//find_crc_with_table(out_chunk + 4, 4, crc_table, &crc);

	write_crc2chunk(out_chunk, 12, crc);
	print_hex_chunk(out_chunk, 12);

}

void create_idat(unsigned char *out_chunk, int lenght) {
	unsigned int crc;

	out_chunk[0] = (unsigned char)((lenght - 12) / 0x1000000);
	out_chunk[1] = (unsigned char)((lenght - 12) / 0x10000 % 0x100);
	out_chunk[2] = (unsigned char)((lenght - 12) / 0x100 % 0x100);
	out_chunk[3] = (unsigned char)((lenght - 12) % 0x100);

    out_chunk[4] = 'I';
    out_chunk[5] = 'D';
    out_chunk[6] = 'A';
    out_chunk[7] = 'T';

	for( int i = 8 ; i < lenght - 4 ; i++ ) {
        out_chunk[i] = rand()/255;
    }

	find_crc(out_chunk + 4, lenght - 8, &crc);
	//find_crc_with_table(out_chunk + 4, 4, crc_table, &crc);

	write_crc2chunk(out_chunk, lenght, crc);
	print_hex_chunk(out_chunk, lenght);
}

void write_ca(FILE *f_p, unsigned char *write_c, int length) {
	for (int i = 0; i < length; i++) {
		fputc(write_c[i], f_p);
	}
}

int main() {
	ihdr = (unsigned char*)malloc(sizeof(unsigned char) * 25);
	iend = (unsigned char*)malloc(sizeof(unsigned char) * 12);
	idat = (unsigned char*)malloc(sizeof(unsigned char) * (100+4+4+4) );

	crc_table = form_crc_table(crc_table);

	out_file = fopen("test.png", "wb");

	write_ca(out_file, png_sig, 8);
	create_ihdr(10, 10, ihdr);
	create_iend(iend);
    create_idat(idat,100+4+4+4);
	write_ca(out_file, ihdr, 25);
	write_ca(out_file, idat, 100+4+4+4);
	write_ca(out_file, iend, 12);

	fclose(out_file);

	return 0;

}
