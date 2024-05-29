//컴퓨터공학과 20192816 김도엽
// 부산헹 (3)
// 3 - 1 전부
// 3 - 2 빌런/ 빌런 추가 및 <이동> <행동> 조건 / 전부 구현
// 3 - 3 시민들/ 시민 위치 랜덤, 인원 표시, 좀비 물릴경우 삭제, 탈출 및 성공 조건   /구현X *시민 수가 랜덤이 아닌 고정되어있음
// 3 - 4 변이/ 강화좀비로 변경, 1턴마다 움직임  / 전부 구현




#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#define LEN_MIN 15 
#define LEN_MAX 50
#define STM_MIN 0 
#define STM_MAX 5
#define PROB_MIN 10 
#define PROB_MAX 90
#define AGGRO_MIN 1 
#define AGGRO_MAX 5 
#define MOVE_LEFT 1 
#define MOVE_STAY 0 
#define ACTION_REST 0 
#define ACTION_PROVOKE 1
#define ACTION_PULL 2

int LEN, PROB, STM;
int cPosition, vPosition, zPosition, mPosition, kPosition, zzPosition;
int cAggro = 1,vAggro = 1, mAggro = 1;
int zTurnCount = 0;
int zStunned = 0;
int mAction; 
int zAction;
int prevCPosition;
int prevVPosition;
int prevZPosition;
int change;
int stage = 1;
int changec = 1;
int bite = 0;
int people = 4;
int success = 4;
int kPosition1, kPosition2;
int prevKPosition, prevKPosition1, prevKPosition2;

void printPattern(int length) {
    for (int i = 0; i < length; i++) {
        printf("#");
    }
    printf("\n");
}

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

void clearInputBuffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

void MAXMIN() {
    if (STM > STM_MAX) STM = STM_MAX;
    if (STM < STM_MIN) STM = STM_MIN;
    if (mAggro < AGGRO_MIN) mAggro = AGGRO_MIN;
    if (mAggro > AGGRO_MAX) mAggro = AGGRO_MAX;
    if (cAggro < AGGRO_MIN) cAggro = AGGRO_MIN;
    if (cAggro > AGGRO_MAX) cAggro = AGGRO_MAX;
    if (vAggro < AGGRO_MIN) vAggro = AGGRO_MIN;
    if (vAggro > AGGRO_MAX) vAggro = AGGRO_MAX;
}

void initializeGame() {
    MAXMIN();


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

    while (1) {
        printf("madongseok stamina(%d~%d)>> ", STM_MIN, STM_MAX);
        if (scanf_s("%d", &STM) != 1 || STM < STM_MIN || STM > STM_MAX) {
            int RESETSTM;

            RESETSTM = STM;

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

    cAggro = 1;
    mAggro = 1; 

    printf("\n\n");
    printPatternWithCharacters(LEN, cPosition, zPosition, mPosition);
    printf("\n");


    prevCPosition = cPosition;
    prevVPosition = vPosition;
    prevZPosition = zPosition;
    prevKPosition = kPosition;
    prevKPosition1 = kPosition1;
    prevKPosition2 = kPosition2;

}

void moveCitizen() {


    if (stage == 1) {
        int moveProb = rand() % 100;
        if (moveProb < (100 - PROB)) {
            if (cPosition > 0) {
                cPosition--; 

            }
        }
    }
    else if (stage == 2) {
        if (vPosition - cPosition >= 0) {
            int moveProb = rand() % 100;
            if (moveProb < (100 - PROB)) {
                if (cPosition > 0) {
                    cPosition--; 
                    if (stage == 2) {
                        vPosition--;
                    }
                }
            }
        }
    }
    else if (stage == 3) {
        int moveProb = rand() % 100;
        if (moveProb < (100 - PROB)) {

            cPosition--;  
            kPosition--;
            kPosition1--;
            kPosition2--;


        }


    }
    else if (stage == 4) { //4스테이지
        int moveProb = rand() % 100;
        if (bite == 0) {
            if (moveProb < (100 - PROB)) {
                cPosition--;  
                kPosition--;
                kPosition1--;
                kPosition2--;
            }

        }
            else if (bite == 1) {
                if (moveProb < (100 - PROB)) {
                    kPosition--;
                    kPosition1--;
                    kPosition2--;
                }

                if (moveProb < (PROB)) {
                    cPosition--;
                }
            }


            else if (bite == 2) {
                if (moveProb < (100 - PROB)) {

                    kPosition1--;
                    kPosition2--;

                }

                if (moveProb < (PROB)) {
                    cPosition--;
                    kPosition--;
                }
            }


            else if (bite == 3) {
                if (moveProb < (100 - PROB)) {


                    kPosition2--;

                }

                if (moveProb < (PROB)) {
                    cPosition--;
                    kPosition--;
                    kPosition1--;
                }
            }


            else {
                if (moveProb < (100 - PROB)) {

                    kPosition2--;

                }

                if (moveProb < (PROB)) {
                    cPosition--;
                    kPosition--;
                    kPosition1--;
                }
            }
 
    }
    else  { //4스테이지
        int moveProb = rand() % 100;
        if (bite == 0) {
            if (moveProb < (100 - PROB)) {
                cPosition--;
                kPosition--;
                kPosition1--;
                kPosition2--;
            }

        }
        else if (bite == 1) {
            if (moveProb < (100 - PROB)) {
                kPosition--;
                kPosition1--;
                kPosition2--;
            }

            if (moveProb < (PROB)) {
                cPosition--;
            }
        }


        else if (bite == 2) {
            if (moveProb < (100 - PROB)) {

                kPosition1--;
                kPosition2--;

            }

            if (moveProb < (PROB)) {
                cPosition--;
                kPosition--;
            }
        }


        else if (bite == 3) {
            if (moveProb < (100 - PROB)) {


                kPosition2--;

            }

            if (moveProb < (PROB)) {
                cPosition--;
                kPosition--;
                kPosition1--;
            }
        }


        else {
            if (moveProb < (100 - PROB)) {

                kPosition2--;

            }

            if (moveProb < (PROB)) {
                cPosition--;
                kPosition--;
                kPosition1--;
            }
        }

        }
}

void moveZombie() {
    if (zStunned == 1) {
        return;  
    }

    if (zTurnCount % 2 != 0) {
        return;  
    }

    int moveProb = rand() % 100;
    if (moveProb >= PROB) {
        return;  
    }

    if (cAggro >= mAggro) {
        if (zPosition > 0 && zPosition - 1 != cPosition && zPosition - 1 != mPosition) {
            zPosition--;  
        }
    }
    else {
        if (zPosition < LEN - 1 && zPosition + 1 != cPosition && zPosition + 1 != mPosition) {
            zPosition++;  
        }
    }
}

void moveM() {

    if (abs(mPosition - zPosition) == 1) {
        printf("madongseok move (0: stay)>> ");
        while (1) {
            if (scanf_s("%d", &mAction) != 1 || mAction != MOVE_STAY) {
                printf("madongseok move (0: stay)>> ");
                clearInputBuffer();
            }
            else {

                clearInputBuffer();
                break;
            }
        }
    }
    else {
        printf("madongseok move (0: stay, 1: move left)>> ");
        while (1) {
            if (scanf_s("%d", &mAction) != 1 || (mAction != MOVE_STAY && mAction != MOVE_LEFT)) {
                printf("madongseok move (0: stay)>> ");
                clearInputBuffer();
            }
            else {

                clearInputBuffer();
                break;
            }
        }

        if (mAction == MOVE_LEFT && mPosition > 0) {
            mPosition--;
        }

    }



}

void performCAction() {
    //not thing
}

void performZAction() {
    if (abs(zPosition - cPosition) == 1 && abs(zPosition - mPosition) == 1) {
        if (cAggro >= mAggro) {
            zAction = 1; 
        }
        else {
            zAction = 2;  
        }
    }
    else if (abs(zPosition - cPosition) == 1) {
        zAction = 1;  

    }
    else if (abs(zPosition - mPosition) == 1) {
        
        zAction = 2;  
    }
    else if (abs(zPosition - vPosition) == 1) {
        zAction = 3;  
    }
    else {
        zAction = 0;  
    }
}

void performMAction() {
    int STMa = STM;
    int mAggrom = mAggro;

    if (abs(mPosition - zPosition) == 1) {
        printf("madongseok action(0: rest, 1: provoke, 2: pull)>> ");
        while (1) {
            int choice;
            if (scanf_s("%d", &choice) != 1 || (choice < 0 || choice > 2)) {
                printf("madongseok action(0: rest, 1: provoke, 2: pull)>> ");
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
       
        printf("madongseok action(0: rest, 1: provoke)>> ");
        while (1) {
            int choice;
            if (scanf_s("%d", &choice) != 1 || (choice != ACTION_REST && choice != ACTION_PROVOKE)) {
                printf("madongseok action(0: rest, 1: provoke, 2: pull)>> ");
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
 
    if (mAction == ACTION_REST) {
        printf("\nmadongseok rests...\n");
        STM++; 
        mAggro--; 
        MAXMIN();
        printf("madongseok : %d (aggro: %d -> %d, stamina: %d -> %d)\n", mPosition, mAggrom, mAggro, STMa, STM);

    }
    else if (mAction == ACTION_PROVOKE) {
        printf("\nmadongseok provoked zombie...\n");
        MAXMIN();
        int mAggrom = mAggro;
        printf("madongseok : %d (aggro: %d -> %d, stamina: %d)\n", mPosition, mAggrom, mAggro = AGGRO_MAX, STM);
        mAggro = AGGRO_MAX;
    }
    else if (mAction == ACTION_PULL) {
        int mAggrom = mAggro;

        mAggro += 2; 
        STM--; 
        MAXMIN();
        int pullSuccess = rand() % (100 - PROB); 
        if (pullSuccess) {
            zStunned = 1; 
            printf("madongseok pulled zombie... Next turn, it can't move\n");
            printf("madongseok : %d (aggro: %d -> %d, stamina: %d -> %d)\n", mPosition, mAggrom, mAggro, STMa, STM);
        }
        else {
            zStunned = 0;
            printf("madongseok failed to pull zombie\n");
            printf("madongseok : %d (aggro: %d -> %d, stamina: %d -> %d)\n", mPosition, mAggrom, mAggro, STMa, STM);
        }

    }
    printf("\n");
}

void printZActionResult() {

    int STMa = STM;

    if (abs(zPosition - cPosition) == 1 && abs(zPosition - mPosition) == 1) {
        if (cAggro >= mAggro) {
            if (stage == 1) {
                printf("Zombie attacks citizen!\nGAME OVER!\n");
                exit(0); 
            }else if (stage == 2) {
                printf("Zombie attacks citizen!\nGAME OVER!\n");
                exit(0);
            }else if (stage == 3) {
          
                if (bite == 0) {

                    printf("\ncitizen has been attacked by zombie.\n");
                    if (bite == 0) {
                        bite = 1;
                    }
                    if (success == 4) {
                        success--;
                    }
                    if (people == 4) {
                        people --;
                    }
                }
             
            }else if (stage == 4){
      
                if (bite == 0) {
                    printf("\ncitizen has been attacked by zombie.\n");
                    if (bite == 0) {
                        bite = 1;
                    }
                    if (success == 4) {
                        success--;
                    }
                    if (people == 4) {
                        people --;
                    }
                }
            }
            else {
        
                if (bite == 0) {
                    printf("\ncitizen has been attacked by zombie.\n");
                    if (bite == 0) {
                        bite = 1;
                    }
                    if (success == 4) {
                        success--;
                    }
                    if (people == 4) {
                        people--;
                    }
                }
            }
        }

        else {
            STM--;
            MAXMIN();
            printf("Zombie attacks madongseok! (stamina: %d -> %d)\n", STMa, STM);
            if (STM == 0) {
                printf("GAME OEVER! madongseok dead...");
                exit(0);
            }
        }
    }
    else if (abs(zPosition - cPosition) == 1) {
        if (stage == 1) {
            printf("Zombie attacks citizen!\nGAME OVER!\n");
            exit(0); 
        }
        else if (stage == 2) {
            printf("Zombie attacks citizen!\nGAME OVER!\n");
            exit(0); 
        }
        else if (stage == 3) {
            
            if (bite == 0) {
                printf("\ncitizen has been attacked by zombie.\n");
       
                if (bite == 0) {
                    bite = 1;
                }
             
                    success--;
                
                if (people == 4) {
                    people--;
                }
            }

        }
        else if (stage == 4) {

            if (bite == 0) {
                printf("\ncitizen has been attacked by zombie.\n");

                if (bite == 0) {
                    bite = 1;
                }

                success--;

                if (people == 4) {
                    people--;
                }
            }

        }
        else{

            if (bite == 0) {
                printf("\ncitizen has been attacked by zombie.\n");
                printf("ciziten turned into a zombie!\n");
                if (bite == 0) {
                    bite = 1;
                }
                if (success == 4) {
                    success--;
                }
                if (people == 4) {
                    people --;
                }
            }
        }
    }
    else if (abs(zPosition - mPosition) == 1) {
        STM--;
        MAXMIN();
        printf("Zombie attacks madongseok! (stamina: %d -> %d)\n", STMa, STM);
        if (STM == 0) {
            printf("GAME OEVER! madongseok dead...");
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

int hasMoved(int prevPosition, int currentPosition) {
    return prevPosition != currentPosition;
}

void printCitizenMoveResult(int prevPosition) {
    int cAggroc = cAggro;
    int vAggroc = vAggro;
    if (prevPosition != cPosition) {

        cAggro++;
        vAggro++;
        MAXMIN();
        if (stage == 3) {
            printf("Citizens: MOVE (aggro: %d -> %d) (in STAGE 3)\n",  cAggroc, cAggro);
            if (bite <= 0) {
                printf("Citizen: %d -> %d (aggro: %d -> %d)\n", prevPosition, cPosition, cAggroc, cAggro);
            }
        }
        else if (stage == 4) {
            printf("Citizens: MOVE (aggro: %d -> %d) (in STAGE 4)\n", cAggroc, cAggro);
            if (bite <= 0) {
                printf("Citizen: %d -> %d (aggro: %d -> %d)\n", prevPosition, cPosition, cAggroc, cAggro);
            }
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

            printf("Citizens: stay (in STAGE 3)\n");

        }
        else if (stage == 4) {

            printf("Citizens: stay (in STAGE 4)\n");

        }
        else {

            printf("Citizen: stay %d (aggro: %d -> %d)\n", cPosition, cAggroc, cAggro);
            if (stage == 2) {
                printf("villain: stay %d (aggro: %d -> %d)\n", vPosition, vAggroc, vAggro);
            }
        }
        
    }

}

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

            if (stage == 3) {
                printf("NomalZombie: %d -> %d (in STAGE 3)\n", prevPosition, zPosition);
            }
            else if (stage == 4) {
                printf("NomalZombie: %d -> %d (in STAGE 4)\n", prevPosition, zPosition);
            }
            else {

                printf("Zombie: %d -> %d\n", prevPosition, zPosition);
            }

        }
        else {
           

            if (stage == 3) {
                printf("NomalZombie: stay %d (in STAGE 3)\n", zPosition);
            }
            else if (stage == 4) {
                printf("NomalZombie: stay %d (in STAGE 4)\n", zPosition);
            }
            else {

                printf("Zombie: stay %d\n", zPosition);
            }
        }
    }

    prevCPosition = cPosition;
    prevVPosition = vPosition;
    prevZPosition = zPosition;
    prevKPosition = kPosition;
    prevKPosition1 = kPosition1;
    prevKPosition2 = kPosition2;
    zStunned = 0;
}

void printMMoveResult() {

    int mAggrom = mAggro;

    if (mAction == MOVE_LEFT && mPosition > 0) {
        mAggro++;
        MAXMIN();
        printf("madongseok: %d -> %d (aggro: %d -> %d, stamina: %d)\n", mPosition + 1, mPosition, mAggrom, mAggro, STM);

    }
    else {
        mAggro--;
        MAXMIN();
        printf("madongseok: stay %d (aggro: %d -> %d, stamina: %d)\n", mPosition, mAggrom, mAggro, STM);

    }
}

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

void initializeGameSECOND() {
    MAXMIN();
  
    cPosition = LEN - 7;
    vPosition = LEN - 6;
    zPosition = LEN - 3;
    mPosition = LEN - 2;

    cAggro = 1; 
    vAggro = 1;
    mAggro = 1; 

    printf("\n\n");
    if (changec == 1) {
        printPatternWithCharactersSECOND(LEN, cPosition, vPosition, zPosition, mPosition);
    }else if (changec == 2) {
        printPatternWithCharactersSECONDCHANGE(LEN, vPosition, cPosition, zPosition, mPosition);
    }
    printf("\n");


    prevCPosition = cPosition;
    prevVPosition = vPosition;
    prevZPosition = zPosition;
    prevKPosition = kPosition;
    prevKPosition1 = kPosition1;
    prevKPosition2 = kPosition2;

}

void printPatternWithCharactersTHIRD(int length, int cPos, int stage, int bite, int zPos, int mPos, int kPos, int kPos1, int kPos2) {
    printPattern(length);
    for (int i = 0; i < length; i++) {
        if (i == 0 || i == length - 1) {
            printf("#");
        }
        else if (i == kPos2 || i == kPos1 || i == kPos) {
            if (stage == 3) {
                if (bite == 0) printf("C");
                else if (bite == 1 && i != cPos) printf("C");
                else if (bite == 2 && (i != cPos && i != kPos)) printf("C");
                else if (bite == 3 && (i != cPos && i != kPos && i != kPos1)) printf("C");
                else printf("");
            }
            else if (stage == 4) {
                if (bite == 0) printf("C");
                else if (bite == 1 && i != cPos) printf("C");
                else if (bite == 2 && (i != cPos && i != kPos)) printf("C");
                else if (bite == 3 && (i != cPos && i != kPos && i != kPos1)) printf("C");
                else printf("Z");
            }
        }
        else if (i == cPos) {
            if (stage == 3) {
                if (bite == 0) printf("C");
                else printf(" ");
            }
            else if (stage == 4) {
                if (bite == 0) printf("C");
                else printf("Z");
            }
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

void initializeGameTHIRD() {
    MAXMIN();
    cAggro = 1; 
    vAggro = 1;
    mAggro = 1; 

    int kPosProb = rand() % 100;
    int kPosProb1 = rand() % 100;
    int kPosProb2 = rand() % 100;

    cPosition = LEN - 6;
    zPosition = LEN - 3;
    mPosition = LEN - 2;

    if (kPosProb < (50)) {
        kPosition = LEN - 8;
    }
    else {
        kPosition = LEN / 2;
    }

    if (kPosProb1 < (50)) {
        kPosition1 = LEN - 11;
    }
    else {
        kPosition1 = LEN / 3;
    }

    if (kPosProb2 < (50)) {
        kPosition2 = LEN - 12;
    }
    else {
        kPosition2 = LEN / 5;
    }
    
 


    if (stage == 3) {
        printPatternWithCharactersTHIRD(LEN, cPosition, 3, bite, zPosition, mPosition, kPosition, kPosition1, kPosition2);
    }

    if (stage == 4) {
        printPatternWithCharactersTHIRD(LEN, cPosition, 4, bite, zPosition, mPosition, kPosition, kPosition1, kPosition2);
    }
 
    printf("\n");
    prevCPosition = cPosition;
    prevVPosition = vPosition;
    prevZPosition = zPosition; 
    prevKPosition = kPosition;
    prevKPosition1 = kPosition1;
    prevKPosition2 = kPosition2;


}

int cle, cle1 ,cle2, cle3, cle4;
void cleclear(){ 
    int cle = 0, cle1 = 0, cle2 = 0, cle3 = 0, cle4 = 0;

}

void CitizenBite() {
    
    if (cPosition == 1) {
       
        if (cle3 == 0) {
          
            success = 0;

        }
        cle3++;
    }
    if (kPosition == 1) {
        
        if (cle2 == 0) {
       
            success--;

        }
        cle2++;
    }
    if (kPosition1 == 1) {
       
        if (cle1 == 0) {
          
            success--;

        }
        cle1++;
        
    }
    if (kPosition2 == 1) {
      
        if (cle == 0) {
          
            success--;

        }
        cle++;

    }



    if (bite == 1) {
        if (abs(cPosition - kPosition) == 1) {
            if (stage == 3) {
               
                if (bite == 1) {
                    printf("citizen2 has been attacked by zombie.\n");

                    bite = 2;
                    people--;
                    success--;
               
                }
            }
            else if (stage == 4) {

                if (bite == 1) {
                    printf("citizen2 has been attacked by zombie.\n");
                    printf("ciziten2 turned into a zombie!\n");

                    bite = 2;
                    people--;
                    success--;
                }
            }
        }
   }

   
    if (bite == 2) {
        if (abs(kPosition - kPosition1) == 1) {
            if (stage == 3) {
                if (bite == 2) {
                    printf("citizen1 has been attacked by zombie.\n");
                    if (bite == 2) {
                        bite = 3;
                    }
                    if (people == 2) {
                        people --;
                    }
                    if (success == 2) {
                        success--;
                    }
                }
            }
            else if (stage == 4) {
                if (bite == 2) {
                    printf("citizen1 has been attacked by zombie.\n");
                    printf("ciziten1 turned into a zombie!\n");
                    if (bite == 2) {
                        bite = 3;
                    }
                    if (people == 2) {
                        people--;
                    }
                    if (success == 2) {
                        success--;
                    }
                }
            }
        }
    }

    if (bite == 3) {
        if (abs(kPosition1 - kPosition2) == 1) {
            if (stage == 3) {
                if (bite == 3) {
                    printf("citizen0 has been attacked by zombie.\n");

                   
                        bite = 4;
                    
                
                        people = 0;
                   
                   
                        success = 0;
                    
                }
            }
            else if (stage == 4) {
                if (bite == 3) {
                    printf("citizen0 has been attacked by zombie.\n");
                    printf("ciziten0 turned into a zombie!\n");
                   
                        bite = 4;
                   
                  
                        people = 0;
                  
                  
                        success = 0;
                  
                }
            }
        }
    }

    
}

void printCActionResult() {
    if (cPosition == 1) {
        if (stage == 1) {
            printPatternWithCharacters(LEN, cPosition, zPosition, mPosition);
            printf("SUCCESS!\ncitizen(s) escaped to the next train\n");

        }
        else if (stage == 2) {
            if (changec == 1) {
                printPatternWithCharactersSECOND(LEN, cPosition, vPosition, zPosition, mPosition);
                printf("SUCCESS!\ncitizen(s) escaped to the next train\n");

            }
            else if (changec == 2) {
                printPatternWithCharactersSECONDCHANGE(LEN, vPosition, cPosition, zPosition, mPosition);
                printf("SUCCESS!\ncitizen(s) escaped to the next train\n");
            }
        }
        else if (stage == 3) {
            printPatternWithCharactersTHIRD(LEN, cPosition, 3, bite, zPosition, mPosition, kPosition, kPosition1, kPosition2);

            if (bite =!1) {
                printf("SUCCESS!\ncitizen(s) escaped to the next train\n");
            }
            success = 0;
            stage++;
        }
        else {
            printPatternWithCharactersTHIRD(LEN, cPosition, 4, bite, zPosition, mPosition, kPosition, kPosition1, kPosition2);

            if (bite = !1) {
                printf("SUCCESS!\ncitizen(s) escaped to the train\n");
            }
      
            success = 0;
        }
    }
    else {
        if (stage == 1) {
            printf("Citizen does nothing.\n");

        }
        else if (stage == 2) {

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

        }
        else {

            printf("Citizens does nothing.\n");

        }
    }
}


void ending() {
    printf("\n\n\n");
    printf(" ~:-     ::.   .~:::~.    ,::     -:-       ~::    .::-    ~:. -::   ::~     ~:,         ~:,    \n");
    printf(" $@#,   ~@@   -$@@@@@$,   :@@     =@*       ;@@    ~@@=    @@: *@@   @@@.    #@!         #@;    \n");
    printf("  @@~   @@:  -@@@=*#@@@   :@@     =@*        @@.   ;@@$.  :@#- *@@   @@@$    #@!         #@;    \n");
    printf("  *@@  ;@#. .#@$.   -@@=  :@@     =@*        #@=   #@@@~  *@=  *@@   @@@@~   #@!         #@;    \n");
    printf("  ,@@; $@!  :@@~     ;@@. :@@     =@*        ;@@   @$@@~  *@=  *@@   @@;@#.  #@!         #@;    \n");
    printf("   !@#;@#.  ;@#      :@@: :@@     =@*        ~@@  ;@:*@! .$@:  *@@   @@.#@*  #@!         #@;    \n");
    printf("   -#@$@!   !@#      ~@@! :@@     =@*        -@@: @@~.@@ -@@   *@@   @@.,#@~ #@!         #@;    \n");
    printf("    $@@@    *@#      ~@@; :@@     =@*         =@* @#, @@ -@@   *@@   @@. !@$.#@!         #@;    \n");
    printf("    .@@~    ;@#      :@@~ ~@@     =@*         *@*;@*  *@:;@:   *@@   @@.  $@;$@!         #@:    \n");
    printf("     @@,    :@@:     *@@  .@@    ,#@!         :@$$@*  ~@##@    *@@   @@.  -@@#@!         ~:,    \n");
    printf("     @@,     $@@,   ;@@:   @@$   :@@,          @@@@-  -@@@@    *@@   @@.   ~@@@!                \n");
    printf("     @@,     ~#@@$=@@@=    !@@#=#@@=           @@@@   .$@@!    *@@   @@.   .$@@!        .#@;    \n");
    printf("     @@,      -*@@@@@:.    .!@@@@@;            ;@@$    *@@~    *@@   @@.    -@@!        .#@;    \n");
    printf("\n\n\n");

}
void stage1() {
    initializeGame(); 

    while (1) {
        moveCitizen();
        moveZombie();
        printPatternWithCharacters(LEN, cPosition, zPosition, mPosition);
        printCitizenMoveResult(prevCPosition);
        printZombieMoveResult(prevZPosition);

        printf("\n");

        moveM();
        printPatternWithCharacters(LEN, cPosition, zPosition, mPosition);
        printMMoveResult();
        printf("\n");


        performCAction();
        performZAction();
        printCActionResult();
        printZActionResult();

        if (cPosition == 1) {
            printf("Stage 1 Clear!\n\n");
            stage++;
            zTurnCount = 0;
            break; 
        }


        performMAction();

        printf("\n");


        zTurnCount++;
    }
}

void stage2() {
    initializeGameSECOND(); 

    while (1) {
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
        moveM();
        if (changec == 1) {
            printPatternWithCharactersSECOND(LEN, cPosition, vPosition, zPosition, mPosition);
        }
        else if (changec == 2) {
            printPatternWithCharactersSECONDCHANGE(LEN, vPosition, cPosition, zPosition, mPosition);
        }
        printMMoveResult();
        printf("\n");
        performCAction();
        performZAction();
        printCActionResult();
        printZActionResult();
        if (cPosition == 1) {
            stage++;
            zTurnCount = 0;
            printf("Stage 2 Clear!\n\n");
            break; 
        }
        performMAction();

        printf("\n");

        zTurnCount++;
    }
}
void stage3() {
    cleclear();
    CitizenBite();
    changec = 1;
    people = 4;
    bite = 0;
    success = 4;
    changec = 1;
    stage = 3;
    initializeGameTHIRD(LEN, cPosition, 3, bite, zPosition, mPosition, kPosition, kPosition1, kPosition2);
    while (1) {
        moveCitizen(bite);
        moveZombie(bite);
        printPatternWithCharactersTHIRD(LEN, cPosition, 3, bite, zPosition, mPosition, kPosition, kPosition1, kPosition2);
        printCitizenMoveResult(prevCPosition);
        printZombieMoveResult(prevZPosition);
        CitizenBite(bite);
        printf("\n");
        moveM();
        printPatternWithCharactersTHIRD(LEN, cPosition, 3, bite, zPosition, mPosition, kPosition, kPosition1, kPosition2);
        printMMoveResult(bite);
        printf("\n");
        performCAction(bite);
        performZAction(bite);
        printCActionResult(bite);
        printZActionResult(bite);
        CitizenBite();
        printf("\n%d citizen(s) alive(s)\n\n", people);
        if (bite == 4)
        {
            printf("\nall citizens(s) dead...!\n\n");
            exit(0);
        }
        if (success == 0) {
            printf("Stage 3 Clear!\n\n");
        
            zTurnCount = 0;
            break; 
        }
        performMAction();
        printf("\n");
        zTurnCount++;
    }
}
void stage4() {
    cleclear();
    CitizenBite();
    changec = 1;
    people = 4;
    bite = 0;
    success = 4;
    changec = 1;
    stage = 4;
    initializeGameTHIRD(LEN, cPosition, 4, bite, zPosition, mPosition, kPosition, kPosition1, kPosition2);
    while (1) {
        moveCitizen(bite);
        moveZombie(bite);
        printPatternWithCharactersTHIRD(LEN, cPosition, 4, bite, zPosition, mPosition, kPosition, kPosition1, kPosition2);
        printCitizenMoveResult(prevCPosition);
        printZombieMoveResult(prevZPosition);
        CitizenBite(bite);
        printf("\n");
        moveM();
        printPatternWithCharactersTHIRD(LEN, cPosition, 4, bite, zPosition, mPosition, kPosition, kPosition1, kPosition2);
        printMMoveResult(bite);
        printf("\n");
        performCAction(bite);
        performZAction(bite);
        printCActionResult(bite);
        printZActionResult(bite);
        CitizenBite();
        printf("\n%d citizen(s) alive(s)\n\n", people);
        if (bite == 4)
        {
            printf("\nall citizens(s) dead...!\n\n");
            exit(0);
        }
        if (success == 0) {
            printf("\n\nStage 4 Clear!\nYOU WIN !.... wait 1sec..\n");
            Sleep(1000);
            system("cls");
            ending();
            exit(0);
        }
        performMAction();
        printf("\n");
        zTurnCount++;
    }
}

int main(void) {
    srand((unsigned int)time(NULL));

    printf("잠시후 기차가 출발합니다...\n\n");
    printf("                                             --         .--         --,\n");
    printf("       @@=       *@@.          ..      -@@,        ~@@.    @@@ @@#\n");
    printf("       @@$       *@@.         :@@-     -@@,        ~@@.    @@@ @@#\n");
    printf("       @@#,,,,,,,=@@.         :@@-     -@@,    @@@@@@@@@@@ @@@ @@#\n");
    printf("       @@@@@@@@@@@@@.         :@@-     -@@,    @@@@@@@@@@@ @@@ @@#\n");
    printf("       @@@@@@@@@@@@@.         *@@!     -@@,        ~::,    @@@ @@#\n");
    printf("       @@$       *@@.         $@@$     -@@-      ;@@@@@#.  @@@ @@#\n");
    printf("       @@$       *@@.         @@@@,    -@@@@@=  *@@@##@@@..@@@ @@#\n");
    printf("       @@@@@@@@@@@@@.        @@@@@@    -@@@@@=  #@#   ,@@@@@@@ @@#\n");
    printf("       @@@@@@@@@@@@@.       $@@:.@@@:  -@@:--,  @@;    @@@@@@@ @@#\n");
    printf("       -------------      ,#@@!  -@@@#.-@@,     $@@; ,$@@- @@@ @@#\n");
    printf("                         -@@@*    .#@* -@@,     -@@@@@@@$  @@@ @@#\n");
    printf("    @@@@@@@@@@@@@@@@@@@.  -@*       !  -@@,      .=#@@$;   === @@#\n");
    printf("    @@@@@@@@@@@@@@@@@@@.      ..       -@@,            .~=$$;. \n");
    printf("    ........=@#........      !@@-      -@@,          !@@@@@@@@@$\n");
    printf("            *@#              !@@-      ,$$,         $@@@#*:;$@@@@\n");
    printf("            *@#              !@@-                  ,@@@.      !@@!\n");
    printf("            *@#              !@@-                  ~@@,        @@@\n");
    printf("            *@#              !@@;~~~~~~~~~~        .@@@-     ,*@@:\n");
    printf("            *@#              !@@@@@@@@@@@@@         -@@@@@@@@@@@$\n");
    printf("            *@#              !@@@@@@@@@@@@@          .@@@@@@@@@;\n");
    printf("            ,-,                                        .-----,\n");
    printf("\n\n\n");
    printf("                                                               START...  \n\n");

    Sleep(2000);
    system("cls");
    stage1();
    stage2();
    stage3(); 
    stage4();
    return 0;

}
