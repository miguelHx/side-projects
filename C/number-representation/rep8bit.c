/*
============================================================================
 Name        : rep8bit.c
 Author      : Miguel Hernandez
 Version     : 1.0 created March 27, 2017
 Copyright   : Open source
 Description : Takes a maximum of an 8-bit binary number, and shows 5 different number representations, which
 	 	 	   are (1) unsigned, (2) signed, (3) 1's complement, (4) 2's complement, (5) biased.
 	 	 	   The executable will take in no more, no less than an 8-bit binary number.
 	 	 	   A table will be displayed, 1 row with 6 columns.  The columns are as follows:
 	 	 	   col1: the user input binary number, col2: unsigned, col3: signed, col4: 1's comp, col5: 2's comp, col6: biased
 	 	 	   I made this program to check my answers when doing my homework for CSE 31.
 	 	 	   It took a few hours to complete.
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>

int verifyBinary(char[], int); //declaring a method meant to check whether the user input an 8-bit binary number or not.
int unsignedRep(char[], int);
int signedRep(char[], int);
int onesComp(char[], int);
int twosComp(char[], int);
int biasedRep(char[], int);
int checkLeftMostBit(char);
void printCeiling();
void addOne(char[], char[], int);
void flipBits(char[], int);

int main() {
	char binaryNum[32]; //string that holds binary number with 32 slots (0-31). We intend to use only 8 slots. Extra memory to avoid error
	int unsignedVal, signedVal, onesCompVal, twosCompVal, biasedVal;
	int length = 8;

	/*Just checking what happens when we add two characters together.  Result is an integer holding the sum of ascii values of the char.
	char one = '1', zero = '0';
	printf("%c + %c = %d + %d = %d\n\n\n", one, zero, (int)one, (int)zero, (one+zero));
	printf("%c + %c = %d + %d = %d\n\n\n", zero, one, (int)zero, (int)one, (zero+one));
	printf("%c + %c = %d + %d = %d\n\n\n", zero, zero, (int)zero, (int)zero, (zero+zero));
	printf("%c + %c = %d + %d = %d\n\n\n", one, one, (int)one, (int)one, (one+one));
	printf("%c + %c + %c = %d + %d + %d = %d\n\n\n", one, one, one, (int)one, (int)one, (int)one, (one+one+one));
	*/

	printf("Enter binary number (8-bits): ");
	scanf("%s", binaryNum); //putting input into binaryNum
	int check = verifyBinary(binaryNum, length); //returns 0 if yes binary, 1 if not binary or not 8-bit

	if (check) {
		printf("Invalid binary number %s.\nMust be only 1s and 0s and 8bits\nExiting program...\n", binaryNum);
	    exit(0);
	 }
	 //if binary is valid, then we continue with the program

	 printf("Binary number %s is in correct format\n", binaryNum);

	 printCeiling();
	 //Now the ceiling of the little table is setup.

	 printf("%s\t    ", binaryNum);

	 //calculate the integer value of binary based on corresponding number representation
	 //start with unsigned, then signed, 1's comp, 2's comp, and biased
	 unsignedVal = unsignedRep(binaryNum, length);
	 signedVal = signedRep(binaryNum, length);
	 biasedVal = biasedRep(binaryNum, length);
	 onesCompVal = onesComp(binaryNum, length); //calling these two last because they modify the actual binaryNum char array
	 twosCompVal = twosComp(binaryNum, length);

	 //format and print
	 printf("%d\t\t   %d\t\t       %d\t\t      %d\t\t      %d\n", unsignedVal, signedVal, onesCompVal, twosCompVal, biasedVal);
	 return 0;
}

int verifyBinary(char str[], int length) {
	//first check if 8 bits
	if (length != 8) {
		return 1;
	}

    int i;
    for (i = 0; i < length; i++) {
        if ((str[i] - '0' != 1 ) && (str[i] - '0' != 0)) { //if char not a 1 or 0
        	//not a binary number
            return 1;
        }
    }

    return 0;
}

void printCeiling() {
	printf("========================================================================================================\n");
	printf("Binary\t\t[Unsigned]\t[Signed]\t[1's Complement]\t[2's Complement]\t[Biased]\n");
}

int unsignedRep(char binStr[], int length) {
	//for unsigned, we convert to decimal right away.
	int sum = 0;
	int j = 0; //for treating the end of the string as the beginning so that we can properly add powers of two
	for (int i = length-1; i >= 0; i--) {
		//sum up powers of two
		if (binStr[i] == '1') {
			sum += (1 << j); //1 << p will give p to the power of 2.  (ex: 1 << 0 will give 1, 1 << 1 will give 2, 1 << 2 will give 4)
		}
		j++;
	}
	return sum;
}

int signedRep(char binStr[], int length) {
	//for signed, we evaluate all the bits except the leftmost.
	//The leftmost bit will give us our sign.
	int multNeg = checkLeftMostBit(binStr[0]);

	int sum = 0;
	int j = 0; //for treating the end of the string as the beginning so that we can properly add powers of two

	for (int i = length-1; i > 0; i--) { //don't want to include leftmost bit in this case, hence why I used i > 0 rather than i >= 0
		//sum up powers of two
		if (binStr[i] == '1') {
			sum += (1 << j); //1 << p will give p to the power of 2.  (ex: 1 << 0 will give 1, 1 << 1 will give 2, 1 << 2 will give 4)
		}
		j++;
	}

	return sum * multNeg;
}

int biasedRep(char binStr[], int length) {
	//no need to worry about flipping bits back.
	//in biased, we want 0000 0000 to represent the smallest value
	//8 bits, so bias will be 2^(8-1) = 2^7 = 128
	//since 0000 0000 is the smallest value, it is negative and all values with leading 0 are negative
	signed int output;
	//all we gotta do is get unsigned value and subtract bias from it.
	output = unsignedRep(binStr, length) - 128;
	return output;
}

int onesComp(char binStr[], int length) {
	//for 1's complement representation, we simply flip all the bits and then evaluate.  If the leftmost bit is 1, then we have a negative.
	int multNeg = checkLeftMostBit(binStr[0]);

	//first, flip the bits
	flipBits(binStr, length);

	//then convert the flipped version to decimal
	int sum = 0;
	int j = 0; //for treating the end of the string as the beginning so that we can properly add powers of two
	for (int i = length-1; i >= 0; i--) {
		//sum up powers of two
		if (binStr[i] == '1') {
			sum += (1 << j); //1 << p will give p to the power of 2.  (ex: 1 << 0 will give 1, 1 << 1 will give 2, 1 << 2 will give 4)
		}
		j++;
	}

	return sum * multNeg;
}

int twosComp(char binStr[], int length) {
	//for 2's complement, if the leftmost bit is 0, then we treat it as an unsigned value
	//if the leftmost bit is 1, however, we must take the 2's complement of the number and then add 1 while
	//slapping a negative sign onto it to get the 2's complement value

	//first, flip the bits back to normal (undoing the bitflip from the onesComp method)
	flipBits(binStr, length);

	char leftMostBit = binStr[0];
	int multNeg = checkLeftMostBit(leftMostBit);

	if (leftMostBit == '0') {
		//we can sum up as an unsigned value
		return unsignedRep(binStr, length);
	}
	//otherwise, handle using 2's complement method.
	//We know that since leftmostbit is NOT 0, then it must be 1.  Therefore our return value will be negative.

	//flip the bits from normal input
	flipBits(binStr, length);

	int resultLength = 9;
	char result[resultLength];
	//now, add one to the flipped result
	addOne(binStr, result, length);

	//now that we added one, we can sum it up as an unsigned integer
	return (unsignedRep(result, resultLength) * multNeg);
}

int checkLeftMostBit(char lmb) {
	if (lmb == '1') {
		//then it is negative
		return -1;
	} else {
		return 1; //otherwise, multiplying by 1 won't change it
	}
}

void addOne(char binStr[], char result[], int length) {
	int carry = 0;
	char binOne[] = "00000001";

	for (int i = length-1; i >= 0; i--) {
		int sum = (binStr[i] + binOne[i]) + carry;//char + char = int.  The resulting int is the sum of the ascii values of 1 or 0. (1 = 49, 0 = 48)
		if (sum == 97) { // (char) 0 + (char) 1 = (char) 1 + (char) 0 = (int) 48 + (int) 49 = 97
			result[i+1] = '1';
			carry = 0;
		} else if (sum == 96) { // (char) 0 + (char) 0 = 48 + 48 = 96
			result[i+1] = '0';
			carry = 0;
		} else if (sum == 98) { // 1 + 1 = 49 + 49 = 98
			result[i+1] = '0';
			carry = 1;
		} else if (sum == 147) { // 1 + 1 + 1 = 49 + 49 + 49 = 147
			result[i+1] = '1';
			carry = 1;
		}
	}
	//once we exit, there is still a leftmost slot unused in result because we declared it's size to be 9 in 2's comp method
	if (carry) {
		result[0] = '1';
	} else {
		result[0] = '0';
	}
	return;
}

void flipBits(char str[], int length) {
	//flipping bits of the parameter str[]
	for (int k = 0; k < length; k++) {
		if (str[k] == '1') {
			str[k] = '0';
		} else {
			str[k] = '1';
		}
	}
}
