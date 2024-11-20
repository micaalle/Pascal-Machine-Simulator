#include <stdio.h>

#define ARRAY_SIZE 500
#define MAX_CODE_LENGTH 1000

typedef struct Instruction Instruction;

struct Instruction
{
    int op, l, m;
};

// Initialize pas
int pas[ARRAY_SIZE] = {0};
int activationBars[20] = {0};
int activationRecords = 0;

int base(int BP, int l){
    int arb = BP;
    while (l > 0){
        arb = pas[arb];
        l--;
    }
    return arb;
}

int main(int argc, char **argv){

    // Registers
    int SP = 500;
    int BP = 499;
    int PC = 0;
    Instruction *IR;

    IR->l = 0;
    IR->m = 0;
    IR->op = 0;

    // Read input from file
    FILE * file = fopen(argv[1], "r");

    // Read file input into pas[]
    int currentIndex = 0;
    while (fscanf(file, "%d", (pas + currentIndex)) == 1){
        currentIndex++;
    }

    fclose(file);

    // Initial Output
    printf("\t\tPC\tBP\tSP\tStack\n");
    printf("\nInitial values:\t%d\t%d\t%d\n\n", PC, BP, SP);

    // Initiate halt condition
    int halt = 1;
    while (halt != 0){
        // Read in instructions
        IR->op = pas[PC];
        IR->l = pas[PC + 1];
        IR->m = pas[PC + 2];
        PC += 3;

        switch (IR->op){
            // Then, the op, l, and m components of the instruction are used to execute

        case 1: // LIT
            SP--;
            pas[SP] = IR->m;
            printf("LIT");
            break;
        case 2: // OPR
            switch (IR->m)
            {
            case 0: // RTN

                SP = BP + 1;
                PC =pas[SP - 3];
                BP = pas[SP - 2];
                activationRecords--;
                
                printf("RTN");
                break;

            case 1: // ADD
                SP++;
                pas[SP] = pas[SP] + pas[SP - 1];
                printf("ADD");
                break;
            case 2: // SUB
                SP++;
                pas[SP] = pas[SP] - pas[SP - 1];
                printf("SUB");
                break;
            case 3: // MUL
                SP++;
                pas[SP] = pas[SP] * pas[SP - 1];
                printf("MUL");
                break;
            case 4: // DIV
                SP++;
                pas[SP] = pas[SP] / pas[SP - 1];
                printf("DIV");
                break;
            case 5: // EQL
                SP++;
                pas[SP] = ((pas[SP] == pas[SP - 1]) ? 1 : 0);
                printf("EQL");
                break;
            case 6: // NEQ
                SP++;
                pas[SP] = ((pas[SP] != pas[SP - 1]) ? 1 : 0);

                break;
            case 7: // LSS
                SP++;
                pas[SP] = ((pas[SP] < pas[SP - 1]) ? 1 : 0);
                break;
            case 8: // LEQ
                SP++;
                pas[SP] = ((pas[SP] <= pas[SP - 1]) ? 1 : 0);
                break;
            case 9: // GTR
                SP++;
                pas[SP] = ((pas[SP] > pas[SP - 1]) ? 1 : 0);
                printf("GTR");
                break;
            case 10: // GEQ
                SP++;
                pas[SP] = ((pas[SP] >= pas[SP - 1]) ? 1 : 0);
                break;
            case 11: // ODD
                SP++;
                pas[SP] = (pas[SP] % 2);
                break;
            }
            break;
        case 3: // LOD
            SP--;
            pas[SP] = pas[base(BP, IR->l) - IR->m];

            printf("LOD");
            break;
        case 4: // STO

            pas[base(BP, IR->l) - IR->m] = pas[SP];
            SP++;

            printf("STO");
            break;
        case 5: // CAL 

            activationBars[activationRecords] = SP;

            pas[SP - 1] = base(BP, IR->l);
            pas[SP - 2] = BP;
            pas[SP - 3] = PC;
            BP = SP - 1;
            PC = IR->m;

            activationRecords++;

            printf("CAL");
            break;
        case 6: // INC
            SP = SP - IR->m;
            printf("INC");
            break;
        case 7: // JMP
            PC = IR->m;
            printf("JMP");
            break;

        case 8: // JPC
            if (pas[SP] == 0){
                PC = IR->m;
            }
            SP++;

            printf("JPC");
            break;
        case 9: // SYS
            switch(IR->m){
                case 1: // OUTPUT
                    printf("Output result is: %d\n", pas[SP]);
                    SP++;
                    printf("SOU");
                    break;
                
                case 2: // INPUT
                    SP--;
                    printf("Please Enter an Integer: ");
                    scanf("%d", &pas[SP]);
                    printf("\nSIN");
                    break;
                case 3: // HALT
                    halt = 0;
                    printf("EOP");
                    break;
            }
            break;        
        }
        

        // Fixes output for m values > 9
        if (IR->m > 9)
            printf(" %d %d\t%d\t%d\t%d", IR->l, IR->m, PC, BP, SP);
        else
            printf(" %d %d\t\t%d\t%d\t%d", IR->l, IR->m, PC, BP, SP);
        
        // Prints stack
        for (int j = 499; j >= SP; j--){
            printf(" %d ", pas[j]);

            // Prints activation records
            for (int i = 0; i < activationRecords; i++){
                if (j == activationBars[i])printf(" | ");
            }
            
        }
        
        printf("\n");
    }
}