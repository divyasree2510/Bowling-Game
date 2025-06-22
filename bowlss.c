#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_PLAYERS 4
#define MAX_PINS 10
#define MAX_FRAMES 10
#define StartingPinCount 10

typedef struct {
    int RollResults[2];
} Frame;

typedef struct {
    char name[50];
    int score;
    Frame frames[MAX_FRAMES];
} Player;

void initializeGame(Player players[MAX_PLAYERS]);
void drawPins(int pinsLeft);
void playGame(Player players[MAX_PLAYERS], int numPlayers);
void displayScoreTable(Player players[MAX_PLAYERS]);
int CalculateBonusScore(Frame frames[], int frameIndex, int rollIndex, int rollsToConsider);

int Score(Frame frames[]) {
    int score = 0,frameIndex,rollIndex;

    for (frameIndex = 0; frameIndex < MAX_FRAMES; frameIndex++) {
        Frame frame = frames[frameIndex];
        int frameScore = 0;
        int bonusScore = 0;
        int isStrike = 0;

        // cap the roll index to 2 to avoid over-counting points if the last frame has bonus rolls
        int maxRollIndex = frame.RollResults[1] == 0 ? 1 : 2;

        for (rollIndex = 0; rollIndex < maxRollIndex; rollIndex++) {
            int result = frame.RollResults[rollIndex];
            frameScore += result;

            // calculate bonus score for a strike
            if (result == StartingPinCount) {
                isStrike = 1;

                // look 2 rolls ahead
                bonusScore += CalculateBonusScore(frames, frameIndex, rollIndex, 2);
                break;
            }
        }

        // calculate bonus score for a spare
        if (!isStrike && frameScore == StartingPinCount) {
            // look 1 roll ahead
            bonusScore += CalculateBonusScore(frames, frameIndex, maxRollIndex - 1, 1);
        }

        score += frameScore + bonusScore;
    }

    return score;
}

int CalculateBonusScore(Frame frames[], int frameIndex, int rollIndex, int rollsToConsider) {
    int bonusScore = 0,i;
    for (i = 0; i < rollsToConsider; i++) {
        int nextFrameIndex = frameIndex + 1;
        int nextRollIndex = 0;

        // Handle edge case for the last frame
        if (nextFrameIndex < MAX_FRAMES) {
            nextRollIndex = rollIndex + i;

            // If the next frame is a strike, move to the frame after that
            if (frames[nextFrameIndex].RollResults[nextRollIndex] == StartingPinCount) {
                nextFrameIndex++;
                nextRollIndex = 0;

                // If the next frame after the strike is also a strike, move to the frame after that
                if (nextFrameIndex < MAX_FRAMES && frames[nextFrameIndex].RollResults[nextRollIndex] == StartingPinCount) {
                    nextFrameIndex++;
                    nextRollIndex = 0;
                }
            }
        }

        bonusScore += frames[nextFrameIndex].RollResults[nextRollIndex];
    }

    return bonusScore;
}

void drawPins(int pinsLeft) {
	int i;
    printf("Pins:\n");

    // Display ASCII art representation of the bowling pins
    for (i = 0; i < pinsLeft; i++) {
        printf("   I   \n");
        printf("  III  \n");
        printf("   I   \n");
        printf("\n");
    }

    printf("\n");  // Add a newline after printing the pins

    // Display ASCII art representation of the bowling lane
    printf("  _______\n");
    printf(" |/  |  \\\n");
    printf(" |   |   |\n");
    printf(" |___|___|\n");
    printf("   /   \\\n");
    printf("  /     \\\n");
    printf(" /_______\\\n");
    printf("\n");
}

void playGame(Player players[MAX_PLAYERS], int numPlayers) {
    int remainingPins = MAX_PINS,frame,i,ball;

    for (frame = 1; frame <= MAX_FRAMES; frame++) {
        printf("\nFrame %d\n", frame);

        for (i = 0; i < numPlayers; i++) {
            printf("%s's turn:\n", players[i].name);

            for (ball = 1; ball <= 2; ball++) {
                int angle;
                do {
                    drawPins(remainingPins);
                    printf("Enter the angle (0-90 degrees) for ball %d: ", ball);
                    scanf("%d", &angle);

                    if (angle < 0 || angle > 90) {
                        printf("Invalid angle. Please enter a value between 0 and 90.\n");
                        while (getchar() != '\n'); // Clear the input buffer
                    }
                } while (angle < 0 || angle > 90);

                int pinsKnocked = remainingPins * sin(angle * M_PI / 180.0);
                remainingPins -= pinsKnocked;

                // Display the updated pins after each ball
                drawPins(remainingPins);

                printf("%s knocked down %d pins\n", players[i].name, pinsKnocked);

                // Record roll results
                players[i].frames[frame - 1].RollResults[ball - 1] = pinsKnocked;

                // Check for strike or spare
                if (remainingPins == 0) {
                    if (ball == 1) {
                        printf("STRIKE!\n");
                        remainingPins = MAX_PINS;  // reset pins for the next frame
                        break;  // no need for the second ball in this frame
                    } else {
                        printf("SPARE!\n");
                        remainingPins = MAX_PINS;  // reset pins for the next frame
                    }
                }
            }

            // Reset the remaining pins for the next player
            remainingPins = MAX_PINS;
        }

        if (frame < MAX_FRAMES) {
            printf("\nPress Enter to start the next frame...\n");
            while (getchar() != '\n'); // Clear the input buffer
        }
    }
}

void displayScoreTable(Player players[MAX_PLAYERS]) {
	int frame,i;
    printf("\nScore Table:\n");
    printf("| Player |");

    // Display frame numbers
    for (frame = 1; frame <= MAX_FRAMES; frame++) {
        printf("  Frame %2d  |", frame);
    }

    printf(" Total |\n");

    printf("|--------|");

    // Display separators for each frame
    for (frame = 1; frame <= MAX_FRAMES; frame++) {
        printf("------------|");
    }

    printf("---------|\n");

    for (i = 0; i < MAX_PLAYERS; i++) {
        if (players[i].name[0] != '\0') {
            printf("| %-6s |", players[i].name);

            // Display roll results for each frame
            for (frame = 0; frame < MAX_FRAMES; frame++) {
                printf("  %-2d %-2d    |", players[i].frames[frame].RollResults[0], players[i].frames[frame].RollResults[1]);
            }

            // Display total score
            printf(" %-5d |\n", Score(players[i].frames));
        } else {
            printf("\n end game....");
        }
    }
  }
