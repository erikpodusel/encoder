#include<stdio.h>
#include<string.h>
#include<stdlib.h>

long decimalToBinary(int); // Funkcia na prevod z decimálnej sústavy na binárnu

int main() {
    char *string = "podus"; // Pre zmenu písmen vpíšte iné znaky do " " ale program funguje len pre 5 znakov
    char binaryToModulated[41];
    int asciiFromString[5];
    long binaryFromAscii[5];
    FILE *asciiFile = fopen("converted.txt", "w+");

    if(asciiFile == NULL) {
        fprintf(stderr, "File couldn't be created/opened, now terminating...\n");
        return 0;
    }

    fprintf(asciiFile, "%s\n", string);
    for(int i = 0; i < 5; i++) {                // Cyklus pre prevod stringu na ASCII
        asciiFromString[i] = string[i];         //  a následne na binárny kód
        binaryFromAscii[i] = decimalToBinary(asciiFromString[i]);
        fprintf(asciiFile, "0%ld", binaryFromAscii[i]);
    }

    fprintf(asciiFile, "\nFM-mofulace:\n");
    fseek(asciiFile, 6, SEEK_SET);
    fgets(binaryToModulated, 41, asciiFile);
    fseek(asciiFile, 0, SEEK_END);
    for(int i = 0; i < strlen(binaryToModulated); i++) {    // Cyklus pre FM moduláciu
        if(binaryToModulated[i] == '0') {
            fprintf(asciiFile, "PN");
        }
        if(binaryToModulated[i] == '1') {
            fprintf(asciiFile, "PP");
        }
    }

    fprintf(asciiFile, "\nMFM-modulace:\n");        // Cyklus pre MFM moduláciu
    for(int i = 0; i < strlen(binaryToModulated); i++) {
        if(i == 0) {
            if(binaryToModulated[0] == '1') {
                fprintf(asciiFile, "NP");
            }
            if(binaryToModulated[0] == '0') {
                fprintf(asciiFile, "PN");
            }
        }
        if(i > 0) {
            if(binaryToModulated[i] == '0' && binaryToModulated[i - 1] == '0') {
                fprintf(asciiFile, "PN");
            }
            else if(binaryToModulated[i] == '0' && binaryToModulated[i - 1] == '1') {
                fprintf(asciiFile, "NN");
            }
            else if(binaryToModulated[i] == '1') {
                fprintf(asciiFile, "NP");
            }
        }
    }
    
    fprintf(asciiFile, "\nRLL-modulace:\n");
    for(int i = 0; i < strlen(binaryToModulated);) {    // Cyklus pre RLL moduláciu
        if(binaryToModulated[i] == '0') {
            if(binaryToModulated[i + 1] == '1') {
                if(binaryToModulated[i + 2] == '1') {
                    fprintf(asciiFile, "NNPNNN");
                    i += 3;
                } else {
                    fprintf(asciiFile, "PNNPNN");
                    i += 3;
                }
            }
            else if(binaryToModulated[i + 1] == '0') {
                if(binaryToModulated[i + 2] == '0') {
                    fprintf(asciiFile, "NNNPNN");
                    i += 3;
                }
                else if(binaryToModulated[i + 2] == '1') {
                    if(binaryToModulated[i + 3] == '0') {
                        fprintf(asciiFile, "NNPNNPNN");
                        i += 4;
                    }
                    else {
                        fprintf(asciiFile, "NNNNPNNN");
                        i += 4;
                    }
                }
            }
        }
        else if(binaryToModulated[i] == '1') {
            if(binaryToModulated[i + 1] == '1') {
                fprintf(asciiFile, "PNNN");
                i += 2;
            }
            else {
                fprintf(asciiFile, "NPNN");
                i += 2;
            }
        }
    }
    fprintf(asciiFile, "\n");
    fclose(asciiFile);
    return 0;
}

long decimalToBinary(int numberToConvert) {
    long convertedNumber = 0;
    long bitAddingNumber = 10000000;
    int bitNumber = 128;
    int tempNumber = numberToConvert;
    
    while(tempNumber > 0) {
        if(tempNumber >= bitNumber) {
            tempNumber -= bitNumber;
            convertedNumber += bitAddingNumber;
        }
        bitAddingNumber /= 10;
        bitNumber /= 2;
    }
    return convertedNumber;
}