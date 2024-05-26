#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#define LEN_MIN 15 // ���� ����
#define LEN_MAX 50
#define STM_MIN 0 // M ü��
#define STM_MAX 5
#define PROB_MIN 10 // Ȯ��
#define PROB_MAX 90
#define AGGRO_MIN 1 // ��׷� ����
#define AGGRO_MAX 5 
#define MOVE_LEFT 1 // �̵� ����
#define MOVE_STAY 0 
#define ACTION_REST 0  // �ൿ
#define ACTION_PROVOKE 1
#define ACTION_PULL 2

int LEN, PROB, STM;
int cPosition, vPosition, zPosition, mPosition;
int cAggro = 1,vAggro = 1, mAggro = 1;
int zTurnCount = 0;
int zStunned = 0;
int mAction;  // M�� �ൿ�� ������ ����
int zAction;
int prevCPosition;
int prevVPosition;
int prevZPosition; // ���� ������ ��ġ�� �����ϴ� ����
int change;
int stage = 1;
int changec = 1;
int kPos[50];


// ���� ���� ��� �Լ�
void printPattern(int length) {
    for (int i = 0; i < length; i++) {
        printf("#");
    }
    printf("\n");
}

// ������ ĳ���� ��� �Լ�
void printPatternWithCharacters(int length, int cPos, int zPos, int mPos) {
    printPattern(length);
    for (int i = 0; i < length; i++) {
        if (i == 0 || i == length - 1) {
            printf("#");
        }
        else if (i == cPos) {
            printf("C");
        }
        else if (i == zPos) {
            printf("Z");
        }
        else if (i == mPos) {
            printf("M");
        }
        else {
            printf(" ");
        }
    }
    printf("\n");
    printPattern(length);
    printf("\n");
}

// �Է� ���۸� ���� �Լ�
void clearInputBuffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

void MAXMIN() {
    // M�� ���׹̳��� STM_MAX�� �ʰ����� �ʵ��� ����
    if (STM > STM_MAX) STM = STM_MAX;
    if (STM < STM_MIN) STM = STM_MIN;
    // ��׷� ��ġ�� MIN�� MAX�� ����� �ʵ��� ����
    if (mAggro < AGGRO_MIN) mAggro = AGGRO_MIN;
    if (mAggro > AGGRO_MAX) mAggro = AGGRO_MAX;
    if (cAggro < AGGRO_MIN) cAggro = AGGRO_MIN;
    if (cAggro > AGGRO_MAX) cAggro = AGGRO_MAX;
    if (vAggro < AGGRO_MIN) vAggro = AGGRO_MIN;
    if (vAggro > AGGRO_MAX) vAggro = AGGRO_MAX;
}


// ���� �ʱ�ȭ �Լ�
void initializeGame() {
    MAXMIN();


    // �Է°��� ������ ��� ������ �ݺ��Ͽ� �Է� ���� 
    while (1) {
        printf("train length(%d~%d)>> ", LEN_MIN, LEN_MAX);
        if (scanf_s("%d", &LEN) != 1 || LEN < LEN_MIN || LEN > LEN_MAX) {

         

            clearInputBuffer();
        }
        else {
            clearInputBuffer();
            break;
        }
    }

    // ������ �Է°��� �����ϰ� ó��
    while (1) {
        printf("M stamina(%d~%d)>> ", STM_MIN, STM_MAX);
        if (scanf_s("%d", &STM) != 1 || STM < STM_MIN || STM > STM_MAX) {

            clearInputBuffer();
        }
        else {
            clearInputBuffer();
            break;
        }
    }

    while (1) {
        printf("percentile probability 'p'(%d~%d)>> ", PROB_MIN, PROB_MAX);
        if (scanf_s("%d", &PROB) != 1 || PROB < PROB_MIN || PROB > PROB_MAX) {

            clearInputBuffer();
        }
        else {
            clearInputBuffer();
            break;
        }
    }

    cPosition = LEN - 6;
    zPosition = LEN - 3;
    mPosition = LEN - 2;

    cAggro = 1; // �ù��� aggro �⺻��
    mAggro = 1; // M�� aggro �⺻��

    printf("\n\n");
    printPatternWithCharacters(LEN, cPosition, zPosition, mPosition);
    printf("\n");


    //���� �ֵ�
    prevCPosition = cPosition;
    prevZPosition = zPosition; // �ʱ�ȭ �� ���� ���� ��ġ ����

}



// �ù� �̵� �Լ� + 2�������� ���� + 3�������� �ù�
void moveCitizen() {
    
    if (stage == 1) {
        int moveProb = rand() % 100;
        if (moveProb < (100 - PROB)) {
            if (cPosition > 0) {
                cPosition--; // �������� 1ĭ �̵�  
   
            }
        }


    }
    else if (stage == 2) {
        if (vPosition - cPosition >= 0) {
            int moveProb = rand() % 100;
            if (moveProb < (100 - PROB)) {
                if (cPosition > 0) {
                    cPosition--; // �������� 1ĭ �̵�  
                    if (stage == 2) {
                        vPosition--;
                    }
                }
            }
        }
        else{
            // ����
        }
    }
    else {
        int moveProb = rand() % 100;
        if (moveProb < (100 - PROB)) {
            if (cPosition > 0) {
                cPosition--; // �������� 1ĭ �̵�  
                for (int i = 0; i < (LEN / 2); i++) {
                    if (kPos[i] > 0) {
                        kPos[i]--;
                    }
                }
            }
        }
    }
}

// ���� �̵� �Լ�
void moveZombie() {
    if (zStunned == 1) {
        return;  // M�� pull�� �������� �� ���� �����̹Ƿ� �������� ����
    }

    if (zTurnCount % 2 != 0) {
        return;  // ¦�� ���� �ƴϸ� �������� ����
    }

    // ���� ���� �߰�
    int moveProb = rand() % 100;
    if (moveProb >= PROB) {
        return;  // Ȯ���� ���� �������� ����
    }

    // �̵��� �� �ִ� ���, ������ �̵� ���� �߰�
    if (cAggro >= mAggro) {
        if (zPosition > 0 && zPosition - 1 != cPosition && zPosition - 1 != mPosition) {
            zPosition--;  // �ù��� �� ���� aggro�� ���� �� �������� �̵�
        }
    }
    else {
        if (zPosition < LEN - 1 && zPosition + 1 != cPosition && zPosition + 1 != mPosition) {
            zPosition++;  // M�� �� ���� aggro�� ������ ���������� �̵�
        }
    }
}

// M �̵� �Լ�
void moveM() {

    // ����� �پ����� ���
    if (abs(mPosition - zPosition) == 1) {
        // �̵����� �ʵ��� ������ 0���� ����
        printf("M's move (0: stay)>> ");
        while (1) {
            if (scanf_s("%d", &mAction) != 1 || mAction != MOVE_STAY) {

                clearInputBuffer();
            }
            else {

                clearInputBuffer();
                break;
            }
        }
    }
    else {
        // ����� �پ����� ���� ��쿡�� �̵��� ������ �����ϵ��� ��û
        printf("M's move (0: stay, 1: move left)>> ");
        while (1) {
            if (scanf_s("%d", &mAction) != 1 || (mAction != MOVE_STAY && mAction != MOVE_LEFT)) {

                clearInputBuffer();
            }
            else {

                clearInputBuffer();
                break;
            }
        }

        // ���ÿ� ���� ��ġ ����
        if (mAction == MOVE_LEFT && mPosition > 0) {
            mPosition--;
        }

    }



}

// �ù� �ൿ ó�� �Լ�
void performCAction() {
    //not thing
}

// ���� �ൿ ó�� �Լ�
void performZAction() {
    if (abs(zPosition - cPosition) == 1 && abs(zPosition - mPosition) == 1) {
        if (cAggro >= mAggro) {
            zAction = 1;  // ���� �ù��� ����
        }
        else {

            zAction = 2;  // ���� M�� ����
        }
    }
    else if (abs(zPosition - cPosition) == 1) {
        zAction = 1;  // ���� �ù��� ����
    }
    else if (abs(zPosition - mPosition) == 1) {
        ;
        zAction = 2;  // ���� M�� ����
    }
    else if (abs(zPosition - vPosition) == 1) {
        zAction = 3;  // ���� ������ ����
    }
    else {
        zAction = 0;  // ���� �ƹ��͵� ���� ����
    }
}


// M �ൿ ó�� �Լ�
void performMAction() {
    int STMa = STM;
    int mAggrom = mAggro;

    // �پ��ִ� ���
    if (abs(mPosition - zPosition) == 1) {
        printf("M's action(0: rest, 1: provoke, 2: pull)>> ");
        while (1) {
            int choice;
            if (scanf_s("%d", &choice) != 1 || (choice < 0 || choice > 2)) {

                clearInputBuffer();
            }
            else {
                mAction = choice;
                clearInputBuffer();
                break;
            }
        }
    }
    else {
        // �پ����� ���� ��쿡�� �̵��̳� provoke ������ �� �ֵ���
        printf("M's action(0: rest, 1: provoke)>> ");
        while (1) {
            int choice;
            if (scanf_s("%d", &choice) != 1 || (choice != ACTION_REST && choice != ACTION_PROVOKE)) {

                clearInputBuffer();
            }
            else {
                mAction = choice;
                clearInputBuffer();
                break;
            }
        }
    }
    printf("\n");
    // ���ÿ� ���� aggro ��ġ ����
    if (mAction == ACTION_REST) {
        printf("\nM's rests...\n");
        STM++; // �޽� �� stamina ����
        mAggro--; // �޽� �� aggro ��ġ ����
        MAXMIN();
        printf("M's : %d (aggro: %d -> %d, stamina: %d -> %d)\n", mPosition, mAggrom, mAggro, STMa, STM);

    }
    else if (mAction == ACTION_PROVOKE) {
        printf("\nM's provoked zombie...\n");
        MAXMIN();
        int mAggrom = mAggro;
        printf("M's : %d (aggro: %d -> %d, stamina: %d)\n", mPosition, mAggrom, mAggro = AGGRO_MAX, STM);
        mAggro = AGGRO_MAX;
    }
    else if (mAction == ACTION_PULL) {
        int mAggrom = mAggro;

        mAggro += 2; // PULL �� ��׷� 2 ����
        STM--; // PULL �� ���¹̳� 1 ����
        MAXMIN();
        int pullSuccess = rand() % (100 - PROB); // PULL ���� Ȯ�� ���
        if (pullSuccess) {
            zStunned = 1; // ���� �������� ǥ��
            printf("M's pulled zombie... Next turn, it can't move\n");
            printf("M's : %d (aggro: %d -> %d, stamina: %d -> %d)\n", mPosition, mAggrom, mAggro, STMa, STM);
        }
        else {
            zStunned = 0;
            printf("M's failed to pull zombie\n");
            printf("M's : %d (aggro: %d -> %d, stamina: %d -> %d)\n", mPosition, mAggrom, mAggro, STMa, STM);
        }

    }
    printf("\n");
}


// ���� �ൿ ��� ��� �Լ�
void printZActionResult() {

    int STMa = STM;

    if (abs(zPosition - cPosition) == 1 && abs(zPosition - mPosition) == 1) {
        if (cAggro >= mAggro) {
            if (stage == 1) {
                printf("Zombie attacks citizen!\nGAME OVER!\n");
                exit(0); // ���� ����
            }else if (stage == 2) {
                printf("Zombie attacks citizen!\nGAME OVER!\n");
                exit(0); // ���� ����
            }else if (stage == 3) {

                printf("citizen has been attacked by zombie.\n");
                kPos[cPosition] = ' ';
             
            }
        }

        else {
            STM--;
            MAXMIN();
            printf("Zombie attacks M! (stamina: %d -> %d)\n", STMa, STM);
            if (STM == 1) {
                printf("GAME OEVER! m dead...");
                exit(0);
            }
        }
    }
    else if (abs(zPosition - cPosition) == 1) {
        if (stage == 1) {
            printf("Zombie attacks citizen!\nGAME OVER!\n");
            exit(0); // ���� ����
        }
        else if (stage == 2) {
            printf("Zombie attacks citizen!\nGAME OVER!\n");
            exit(0); // ���� ����
        }
        else if (stage == 3) {
            printf("citizen has been attacked by zombie.\n");
            kPos[cPosition] = ' ';

        }
    }
    else if (abs(zPosition - mPosition) == 1) {
        STM--;
        MAXMIN();
        printf("Zombie attacks M! (stamina: %d -> %d)\n", STMa, STM);
        if (STM == 1) {
            printf("GAME OEVER! m dead...");
            exit(0);
        }


    }

    if ((zAction) == 0) {
        printf("zombie attcked nobody.\n");
    }
    if ((zAction) == 3) {
        printf("zombie attcked villain.\n");
    }


}



// ���� ��ġ�� ���� ��ġ�� ���Ͽ� �޶��������� Ȯ���ϴ� �Լ�
int hasMoved(int prevPosition, int currentPosition) {
    return prevPosition != currentPosition;
}



// �ù� �̵� ��� ��� �Լ�
void printCitizenMoveResult(int prevPosition) {

    int cAggroc = cAggro;
    int vAggroc = vAggro;

    if (prevPosition != cPosition) {

        cAggro++;
        vAggro++;
        MAXMIN();
        if (stage == 3) {
            printf("Citizens MOVE (aggro: %d -> %d)\n",  cAggroc, cAggro);
        }
        else if (stage == 4) {
            printf("Citizens MOVE (aggro: %d -> %d)\n", cAggroc, cAggro);
        }
        else {
            printf("Citizen: %d -> %d (aggro: %d -> %d)\n", prevPosition, cPosition, cAggroc, cAggro);

            if (stage == 2) {
                printf("villain: %d -> %d (aggro: %d -> %d)\n", vPosition + 1, vPosition, vAggroc, vAggro);
            }
        }
    }
    else {

        cAggro--;
        vAggro--;
        MAXMIN();

        if (stage == 3) {

            printf("Citizens: stay\n");

        }
        else if (stage == 4) {

            printf("Citizens: stay\n");

        }
        else {

            printf("Citizen: stay %d (aggro: %d -> %d)\n", cPosition, cAggroc, cAggro);
            if (stage == 2) {
                printf("villain: stay %d (aggro: %d -> %d)\n", vPosition, vAggroc, vAggro);
            }
        }
        
    }

}



// ���� �̵� ��� ��� �Լ�
void printZombieMoveResult(int prevPosition) {
    MAXMIN();
    if (zStunned == 1 && mAction == ACTION_PULL) {
        printf("Zombie: stay %d (cannot move PULL)\n", zPosition);
    }
    else if (zTurnCount % 2 != 0) {
        printf("Zombie: stay %d (cannot move)\n", zPosition);
    }
    else {
        if (prevPosition != zPosition) {
            printf("Zombie: %d -> %d\n", prevPosition, zPosition);
        }
        else {
            printf("Zombie: stay %d\n", zPosition);
        }
    }

    //���� �ֵ�
    prevCPosition = cPosition;
    prevZPosition = zPosition; // �ʱ�ȭ �� ���� ���� ��ġ ����

    zStunned = 0;
}



// M �̵� ��� ��� �Լ�
void printMMoveResult() {

    int mAggrom = mAggro;

    if (mAction == MOVE_LEFT && mPosition > 0) {
        mAggro++;
        MAXMIN();
        printf("M's: %d -> %d (aggro: %d -> %d, stamina: %d)\n", mPosition + 1, mPosition, mAggrom, mAggro, STM);

    }
    else {
        mAggro--;
        MAXMIN();
        printf("M's: stay %d (aggro: %d -> %d, stamina: %d)\n", mPosition, mAggrom, mAggro, STM);

    }
}



// ������ ĳ���� ��� �Լ� 2�������� 
void printPatternWithCharactersSECOND(int length, int cPos, int vPos, int zPos, int mPos) {
    printPattern(length);
    for (int i = 0; i < length; i++) {
        if (i == 0 || i == length - 1) {
            printf("#");
        }
        else if (i == cPos) {
            printf("C");
        }
        else if (i == vPos) {
            printf("V");
        }
        else if (i == zPos) {
            printf("Z");
        }
        else if (i == mPos) {
            printf("M");
        }
        else {
            printf(" ");
        }
    }
    printf("\n");
    printPattern(length);
    printf("\n");
}

// ������ ĳ���� ��� �Լ� 2�������� ü����
void printPatternWithCharactersSECONDCHANGE(int length, int vPos, int cPos, int zPos, int mPos) {
    printPattern(length);
    for (int i = 0; i < length; i++) {
        if (i == 0 || i == length - 1) {
            printf("#");
        }
        else if (i == vPos) {
            printf("V");
        }
        else if (i == cPos) {
            printf("C");
        }
        else if (i == zPos) {
            printf("Z");
        }
        else if (i == mPos) {
            printf("M");
        }
        else {
            printf(" ");
        }
    }
    printf("\n");
    printPattern(length);
    printf("\n");
}

// ���� �ʱ�ȭ �Լ� 2��������
void initializeGameSECOND() {
    MAXMIN();
  
    cPosition = LEN - 7;
    vPosition = LEN - 6;
    zPosition = LEN - 3;
    mPosition = LEN - 2;

    cAggro = 1; // �ù��� aggro �⺻��
    vAggro = 1;
    mAggro = 1; // M�� aggro �⺻��

    printf("\n\n");
    if (changec == 1) {
        printPatternWithCharactersSECOND(LEN, cPosition, vPosition, zPosition, mPosition);
    }else if (changec == 2) {
        printPatternWithCharactersSECONDCHANGE(LEN, vPosition, cPosition, zPosition, mPosition);
    }
    printf("\n");


    //���� �ֵ�
    prevCPosition = cPosition;
    prevVPosition = vPosition;
    prevZPosition = zPosition; // �ʱ�ȭ �� ���� ���� ��ġ ����

}



// �ù� �ൿ ��� ��� �Լ�
void printCActionResult() {
    if (cPosition == 1) {
        if (stage == 1) {
            printPatternWithCharacters(LEN, cPosition, zPosition, mPosition);
            printf("YOU WIN! ������\n");

        } 
        else if (stage == 2) {
            if (changec == 1) {
                printPatternWithCharactersSECOND(LEN, cPosition, vPosition, zPosition, mPosition);
                printf("YOU WIN! ������\n");

            }
            else if (changec == 2) {
                printPatternWithCharactersSECONDCHANGE(LEN, vPosition, cPosition, zPosition, mPosition);
                printf("YOU WIN! ������\n");
            }
        }
        else if (stage == 3){
            printPatternWithCharacters(LEN, cPosition, zPosition, mPosition); //���� �ؾ��Ҽ���
             printf("YOU WIN! ������\n");
        }

    }else {
        if (stage == 1) {
            printf("Citizen does nothing.\n");

        }else if (stage == 2) {

            if (changec == 1) {
                
                    int changeProb = rand() % 100;
                    if (changeProb < (30)) {
                        changec = 2;
                        change = cPosition;
                        cPosition = vPosition;
                        vPosition = change;
                        printPatternWithCharactersSECONDCHANGE(LEN, vPosition, cPosition, zPosition, mPosition);
                        printf("Citizen does nothing.\n");
                        printf("Villain changed place a citizen!\n");
                       
                    }
                    else {
                        printPatternWithCharactersSECOND(LEN, cPosition, vPosition, zPosition, mPosition);
                        printf("Citizen does nothing.\n");
                        printf("villain does nothing.\n");
                    }
            }
            else if (changec == 2) {
                if (stage == 2) {
                   
                    int changeProb = rand() % 100;
                    if (changeProb < (30)) {
                        changec = 1;
                        change = vPosition;
                        vPosition = cPosition;
                        cPosition = change;
                        printPatternWithCharactersSECOND(LEN, cPosition, vPosition, zPosition, mPosition);
                        printf("Citizen does nothing.\n");
                        printf("Villain changed place a citizen!\n");
                       
                    }
                    else {
                        printPatternWithCharactersSECONDCHANGE(LEN, vPosition, cPosition, zPosition, mPosition);
                        printf("Citizen does nothing.\n");
                        printf("villain does nothing.\n");
                    }

                }
            }

        } else {

            printf("Citizens does nothing.\n");

        }
    }
}






// 1. initializeGameTHIRD ���� �ʱ�ȭ 3��������
void initializeGameTHIRD(int LEN, int* kPos) {
    // ���� ���̿� ���� K�� ���� ����
    int kCount = (LEN / 4 < LEN / 2) ? LEN / 4 : LEN / 2;

    // ������ ĳ���� �ʱ� ��ġ ����
    cPosition = LEN - 6;
    zPosition = LEN - 3;
    mPosition = LEN - 2;
    
    
    // K�� ��ġ ���� ����
    for (int i = 0; i < kCount; i++) {
        int kPosCandidate;
        do {
            kPosCandidate = rand() % LEN;
        } while (kPosCandidate == cPosition || kPosCandidate == zPosition || kPosCandidate == mPosition);
        kPos[i] = kPosCandidate;
    }
}


// 2. printPatternWithCharactersTHIRD �Լ� ����
void printPatternWithCharactersTHIRD(int length, int* kPos) {
    printPattern(length);

    for (int i = 0; i < length; i++) {
        if (i == 0 || i == length - 1) {
            printf("#");
        }
        else if (i == cPosition) {
            printf("C");
        }
        else if (i == zPosition) {
            printf("Z");
        }
        else if (i == mPosition) {
            printf("M");
        }
        else {
            int isK = 0;
            for (int j = 0; j < LEN / 4; j++) {
                if (i == kPos[j]) {
                    printf("K");
                    isK = 1;
                    break;
                }
            }
            if (!isK) {
                printf(" ");
            }
        }
    }
    printf("\n");
    printPattern(length);
    printf("\n");
}



// 3. citizens �Լ� ����
void citizens() {

    
}

// stage1: ù ��° ���������� ó���ϴ� �Լ�
void stage1() {
    initializeGame(); // ���� �ʱ�ȭ

    while (1) {
        //�ʱ� ȭ�� <�̵�>������ 
        moveCitizen();
        moveZombie();
        printPatternWithCharacters(LEN, cPosition, zPosition, mPosition);
        printCitizenMoveResult(prevCPosition);
        printZombieMoveResult(prevZPosition);

        printf("\n");

        //<�̵�>������ ���
        moveM();
        printPatternWithCharacters(LEN, cPosition, zPosition, mPosition);
        printMMoveResult();
        printf("\n");

        //<�ൿ>������ 

        performCAction();
        performZAction();
        //<�ൿ>������ ���
        printCActionResult();
        printZActionResult();

        // ���� ���� ���� Ȯ��
        if (cPosition == 1) {
            printf("Stage 1 Clear!\n\n");
            stage++;
            zTurnCount = 0;
            break; // ���� ���������� �̵�
        }


        performMAction();

        printf("\n");


        // zTurnCount ����
        zTurnCount++;
    }
}

// stage2: �� ��° ���������� ó���ϴ� �Լ�
void stage2() {
    initializeGameSECOND(); // ���� �ʱ�ȭ

    while (1) {
        //�ʱ� ȭ�� <�̵�>������ 
        moveCitizen();
        moveZombie();
        if (changec == 1) {
            printPatternWithCharactersSECOND(LEN, cPosition, vPosition, zPosition, mPosition);
        }
        else if (changec == 2) {
            printPatternWithCharactersSECONDCHANGE(LEN, vPosition, cPosition, zPosition, mPosition);
        }
        printCitizenMoveResult(prevCPosition);
        printZombieMoveResult(prevZPosition);

        printf("\n");

        //<�̵�>������ ���
        moveM();
        if (changec == 1) {
            printPatternWithCharactersSECOND(LEN, cPosition, vPosition, zPosition, mPosition);
        }
        else if (changec == 2) {
            printPatternWithCharactersSECONDCHANGE(LEN, vPosition, cPosition, zPosition, mPosition);
        }
        printMMoveResult();
        printf("\n");

        //<�ൿ>������ 

        performCAction();
        performZAction();
        //<�ൿ>������ ���
        printCActionResult();
        printZActionResult();

        // ���� ���� ���� Ȯ��
        if (cPosition == 1) {
            stage++;
            zTurnCount = 0;
            printf("Stage 2 Clear!\n\n");
            break; // ���� ���������� �̵�
        }
        performMAction();

        printf("\n");


        // zTurnCount ����
        zTurnCount++;
    }
}






// stage3: �� ��° ���������� ó���ϴ� �Լ�
void stage3() {
    
    // �� ��° ���������� ó���� ���� �ڵ� �ۼ�
    initializeGameTHIRD(LEN, kPos); // ���� �ʱ�ȭ



    while (1) {
       
        // printPatternWithCharactersTHIRD �Լ��� ������ ĳ���� ���
        printPatternWithCharactersTHIRD(LEN, kPos);
        
        moveCitizen();
        moveZombie();
        printPatternWithCharactersTHIRD(LEN, kPos);
        printCitizenMoveResult(prevCPosition);
        printZombieMoveResult(prevZPosition);

        printf("\n");

        //<�̵�>������ ���
        moveM();
        printPatternWithCharactersTHIRD(LEN, kPos);
        printMMoveResult();
        printf("\n");

        //<�ൿ>������ 

        performCAction();
        performZAction();
        //<�ൿ>������ ���
        printCActionResult();
        printZActionResult();

        // ���� ���� ���� Ȯ��
        if (cPosition == 1) {
            printf("Stage 3 Clear!\n\n");
            stage++;
            zTurnCount = 0;
            break; // ���� ���������� �̵�
        }


        performMAction();

        printf("\n");


        // zTurnCount ����
        zTurnCount++;
    }
}

void stage4() {

}


// ���� �Լ�
int main(void) {
    srand((unsigned int)time(NULL));

    stage1(); // ù ��° �������� ����
    stage2(); // �� ��° �������� ����
    stage3(); // �� ��° �������� ����
    //changec = 1;
    // stage4(); // �� ��° �������� ����


    return 0;
}