//
//  Disassembler.c
//  Disassembler
//
//  This program is capable of disassembling up to and including SIM 4 assembly.
//
//  Created by Connor Crawford on 11/23/15.
//  Copyright © 2015 Connor Crawford. All rights reserved.
//

#include <stdio.h>
#include <string.h>

int disassemble(FILE *in, FILE *out);

int main(int argc, const char * argv[]) {
    if (argc < 2) {
        fprintf(stderr, "%s: Expected argument.\n", argv[0]);
        return 1;
    } else if (argc == 2) {
        FILE *in, *out;
        if ((in = fopen(argv[1], "r")) == NULL) {
            fprintf(stderr, "Unable to open file %s", argv[1]);
            return 1;
        }
        if (!(out = fopen("out.s", "w"))) {
            fprintf(stderr, "Unable to write to file %s", "out.s");
            return 1;
        }
        disassemble(in, out);
    } else {
        fprintf(stderr, "%s: Too many arguments.\n", argv[0]);
        return 1;
    }
    return 0;
}

int disassemble(FILE *in, FILE *out) {
    if (!in || !out)
        return 0;
    char buffer[100], *memory, *instruction;
    int val = 0, inHalt = 0;
    while (!feof(in)) {
        fgets(buffer, 100, in);
        if (buffer[0] == '#') {
            // Safely ignore comment
            fprintf(out, "%s", buffer);
        } else {
            memory = strtok(buffer, " ");
            instruction = strtok(NULL, " ");
            if (memory && instruction) {
                fprintf(out, "%s %.4s ", memory, instruction);
                switch (instruction[0]) {
                    case '0':
                        if (inHalt)
                            fprintf(out, ".word");
                        else
                            fprintf(out, "HALT");
                        inHalt = 1;
                        break;
                    case '1':
                        sscanf((instruction + 1), "%d", &val);
                        fprintf(out, "LD %c", val);
                        break;
                    case '2':
                        sscanf((instruction + 1), "%d", &val);
                        fprintf(out, "ST %d", val);
                        break;
                    case '3':
                        sscanf((instruction + 1), "%d", &val);
                        fprintf(out, "ADD %d", val);
                        break;
                    case '4':
                        sscanf((instruction + 1), "%d", &val);
                        fprintf(out, "SUB %d", val);
                        break;
                    case '5':
                        sscanf((instruction + 1), "%d", &val);
                        fprintf(out, "LDA %d", val);
                        break;
                    case '6':
                        sscanf((instruction + 1), "%d", &val);
                        fprintf(out, "JMP %d", val);
                        break;
                    case '7':
                        switch (*(instruction + 1)) {
                            case '0':
                                fprintf(out, "SKIP");
                                break;
                            case '1':
                                fprintf(out, "SKEQ");
                                break;
                            case '2':
                                fprintf(out, "SKNE");
                                break;
                            case '3':
                                fprintf(out, "SKGT");
                                break;
                            case '4':
                                fprintf(out, "SKGE");
                                break;
                            case '5':
                                fprintf(out, "SKLT");
                                break;
                            case '6':
                                fprintf(out, "SKLE");
                                break;
                            default:
                                break;
                        }
                        if (*(instruction + 1) != '0')
                            fprintf(out, " %%r%c", *(instruction + 3));
                        break;
                    case '8':
                        switch (*(instruction + 1)) {
                            case '0':
                                fprintf(out, "IN");
                                break;
                            case '1':
                                fprintf(out, "OUT");
                                break;
                            case '2':
                                fprintf(out, "CLR");
                                break;
                            case '3':
                                fprintf(out, "INC");
                                break;
                            case '4':
                                fprintf(out, "DEC");
                                break;
                            case '5':
                                fprintf(out, "NEG");
                                break;
                            case '6':
                                fprintf(out, "SHL");
                                break;
                            case '7':
                                fprintf(out, "SHR");
                                break;
                            default:
                                break;
                        }
                        if (*(instruction + 1) != '0')
                            fprintf(out, " %%r%c", *(instruction + 3));
                        break;
                    case '9':
                        switch (*(instruction + 1)) {
                            case '0':
                                fprintf(out, "MVRR");
                                break;
                            case '1':
                                fprintf(out, "MVMR");
                                break;
                            case '2':
                                fprintf(out, "MVRM");
                                break;
                            case '3':
                                fprintf(out, "EXCH");
                                break;
                            case '4':
                                fprintf(out, "ADDR");
                                break;
                            case '5':
                                fprintf(out, "SUBR");
                                break;
                            default:
                                break;
                        }
                        fprintf(out, " %%r%c", *(instruction + 2));
                        fprintf(out, ",%%r%c", *(instruction + 3));
                        break;
                    default:
                        break;
                }
                fprintf(out, "\n");
            }
        }
    }
    return 1;
}