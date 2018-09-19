//this program to encode/decode string to base 64
/*
Reference :
-https://en.wikipedia.org/wiki/Base64
-https://www.base64encode.org/

encode/decode :
ayoub      ---(encode)--->    YXlvdWI=
YXlvdWI=   ---(decode)--->    ayoub


Example For : ayoub
+--------------------------------------------------------------------------------+
|  ASCII    -->              a         y        o        u        b              |
---------------------------------------------------------------------------------*
|  DEC      -->             97       121      111      117       98              |
---------------------------------------------------------------------------------*
|  BIN      -->       01100001  01111001 01101111 01110101 01100010              |
---------------------------------------------------------------------------------*
|  6-bits   -->    011000 010111 100101 101111 011101 010110 0010,00   000000    |
---------------------------------------------------------------------------------*
|  Index    -->        24     23     37     47     29     22     8	padding=2bit |
---------------------------------------------------------------------------------*
|  BASE64   -->         Y      X      l      v      d      W      I         =    |
+--------------------------------------------------------------------------------+

*/


#include <stdio.h>
#include <stdlib.h>

//all functions used
unsigned char* encode(unsigned const char*);
unsigned char* decode(unsigned const char*);

//character alphabet of base64
unsigned const char character_alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

//principal function
int main(){
	unsigned char string[] = "ayoub";

	printf("string : %s\n\n",string);

	printf("Encode : %s\n\n",encode(string));

	printf("decode : %s\n\n",decode(encode(string)));

	return 0;
}

//function to encode string to BASE64
unsigned char* encode(unsigned const char* string){
	//get length of string
	int length_of_string = 0;
	while(string[length_of_string] != '\0')	length_of_string++;

	int number_of_binary = length_of_string * 8; //multiplication by 8 so that each one character is represent on 8 bits

	//padding to complete the 6 bits when using base table 64
	int padding=0;

	while(1){
        if(number_of_binary % 6 == 0)	break;
        padding++;
        number_of_binary++;
    }

	unsigned char* encode_data = (unsigned char*)malloc(sizeof(unsigned char) * ((number_of_binary / 6) + (padding / 2))); //after the encoding each 6 bits will present a one character

	//pointer to store all bits and calloc just to initialize frame with 0
	int* store_result_binary = (int*)calloc(sizeof(int) , number_of_binary);

	//Jumping (8 bits) during when store bits in pointer
	int z_index = 8;

	for(int i=0;i<length_of_string;i++){
		int index = z_index;
		int quotien = string[i];

		while(1){
			if(quotien / 2){
				*(store_result_binary + (--index)) = quotien % 2;
				quotien /=  2;
			}else{
			*(store_result_binary + (--index)) = quotien % 2;
			break;
			}
		}

		index = z_index += 8;

	}
	//to see ASCII to binary + padding
	//for(int i=0;i<number_of_binary;i++)	printf("%d%s",*(store_result_binary + i),(i+1)%8 ? "" : " ");
	//printf("\n");

	//6-bits to decimal
	int index = number_of_binary / 6;
	for(int i=(number_of_binary -1); i>0 ; i -= 6){
		int j=0,power=0;
		double _6bit=0;
		while(j<6){
			if(*(store_result_binary + (i - j)) == 1 ){
				int powe=1;
				for(int k=0; k<power; k++){
					powe *= 2;
				}
				_6bit += powe;
			}
			power++;
			j++;
		}

		*(encode_data + (--index)) = *(character_alphabet + (int)_6bit);
	}

	//adding = (just for padding)
	if(padding == 2)
		*(encode_data + (number_of_binary / 6)) = '=';
	if(padding == 4){
		*(encode_data + (number_of_binary / 6)) = '=';
		*(encode_data + (number_of_binary / 6) + 1) = '=';
	}
	*(encode_data + ((number_of_binary / 6) + (padding / 2))) = '\0';
	//to see the encoding
	//for(int i=0; i<((number_of_binary / 6) + (padding / 2)) ;i++)	printf("-%d %02x\n",i,*(encode_data + i));

	//free pointers
	free(store_result_binary);
	store_result_binary=NULL;

	return encode_data;
}

//function to decode from BASE64 to string
unsigned char* decode(unsigned const char* string){
	//get length of string
	int length_of_string = 0;
	while(string[length_of_string] != '\0')	length_of_string++;

	//find the number of repetition of the letter '='
	int padding=0;
	int i=0;
	while(i < length_of_string){
		if(string[i] == '=')	++padding;
		i++;
	}

	int number_of_binary = ((length_of_string - padding) * 6) - (padding * 2);
	unsigned char* decode_data = (unsigned char*)malloc(sizeof(unsigned char) * (number_of_binary / 8) + 1);

	//pointer to store all bits and calloc just to initialize frame with 0
	int* store_result_binary = (int*)calloc(sizeof(int) , number_of_binary * 8);

	//Jumping (6 bits) during when store bits in pointer
	int z_index = 6;

	for(int i=0;i<(length_of_string - padding);i++){
		int index = z_index;
		int quotien = 0;
		int verify_base64 = -1;
		for(int j=0; j < 64; j++){
			if(string[i] == character_alphabet[j]){
				quotien = j;
				verify_base64 = 0;
				break;
			}
		}
		if(verify_base64){
			printf("No printable characters found");
			exit(0);
		}
		while(1){
			if(quotien / 2){
				*(store_result_binary + (--index)) = quotien % 2;
				quotien /=  2;
			}else{
			*(store_result_binary + (--index)) = quotien % 2;
			break;
			}
		}

		index = z_index += 6;

	}

	//to see base64 to binary without padding
	//for(int i=0;i<number_of_binary;i++)	printf("%d%s",*(store_result_binary + i),(i+1)%8 ? "" : " ");
	//printf("\n");

	//8-bits to decimal
	int index = 0;

	for(int i=0; i<number_of_binary ; i += 8){
		int j=7,power=0;
		double _8bit=0;
		while(j>=0){
			if(*(store_result_binary + (i + j)) == 1 ){
				int powe=1;
				for(int k=0; k<power; k++){
					powe *= 2;
				}
				_8bit += powe;
				powe=0;
			}
			power++;
			j--;
		}
		*(decode_data + (index++)) = (int)_8bit;
		j=7;power=0;_8bit=0;
	}
	*(decode_data + (number_of_binary / 8) + 1) = '\0';

	//to see the decoding
	//for(int i=0; i<(number_of_binary / 8) ;i++)	printf("-%d %02x\n",i,*(decode_data + i));

	//free pointers
	free(store_result_binary);
	store_result_binary=NULL;

	return decode_data;
}

