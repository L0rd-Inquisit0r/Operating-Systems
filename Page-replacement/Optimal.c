#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SENTINEL -65536
#define INF 65536

typedef struct {
    int **data;  // Memory states for each time step
    char *status; // 'H' for hit, 'M' for miss
    int pageHits; // Total page hits
    int pageFaults; // Total page faults
} PageOptimal;

void initPageOptimal(PageOptimal *A, int frames, int pageNum);
void PageReplacementOptimal(PageOptimal *A, int pageNum, int frames, int *pageRef);
void displayPageOptimal(PageOptimal A, int pageNum, int frames);

int main() {
    PageOptimal P;
    int pageNum, frames, x;
    
    printf("Please enter the number of frames: ");
    scanf("%d", &frames);
    
    printf("Please enter the number of pages: ");
    scanf("%d", &pageNum);
    
    initPageOptimal(&P, frames, pageNum);
    
    int pageRef[pageNum];
    printf("Please enter %d page reference/s: ", pageNum);
    for (x = 0; x < pageNum; x++) {
        scanf("%d", &pageRef[x]);
    }
    
    PageReplacementOptimal(&P, pageNum, frames, pageRef);
    displayPageOptimal(P, pageNum, frames);
    
    return 0;
}

void initPageOptimal(PageOptimal *A, int frames, int pageNum) {
    int x, y;
    A->data = (int **)malloc(frames * sizeof(int *));
    A->status = (char *)malloc(pageNum * sizeof(char));
    A->pageHits = 0;
    A->pageFaults = 0;
    
    if(A->data != NULL && A->status != NULL) {
        for(x = 0; x < frames; x++) {
            A->data[x] = (int *)malloc(pageNum * sizeof(int));
            if(A->data[x] != NULL) {
                for(y = 0; y < pageNum; y++) {
                    A->data[x][y] = SENTINEL; // Initialize with sentinel
                }
            }
        }
    }
}

void PageReplacementOptimal(PageOptimal *A, int pageNum, int frames, int *pageRef) {
    int x, y, z, frameIndex;

    for (x = 0; x < pageNum; x++) {
        int found = 0; // To track if the page is already in memory
        
        // Copy the previous state
        for (y = 0; y < frames; y++) {
            A->data[y][x] = (x == 0) ? SENTINEL : A->data[y][x - 1];
        }

        // Check if the page is already in memory
        for (y = 0; y < frames && !found; y++) {
            if (A->data[y][x] == pageRef[x]) {
                found = 1;
            }
        }

        if (found) {
            A->status[x] = 'H'; // Page hit
            A->pageHits++;
        } else {
            A->status[x] = 'F'; // Page fault
            A->pageFaults++;
            
            // Find an empty frame or the optimal page to replace
            frameIndex = -1;
            for (y = 0; y < frames; y++) {
                if (A->data[y][x] == SENTINEL) {
                    frameIndex = y; // Found an empty frame
                    break;
                }
            }

            if (frameIndex == -1) {
                // No empty frame, apply optimal replacement
                int farthest = -1, replaceIndex = -1;

                for (y = 0; y < frames; y++) {
                    int nextUse = INF; // Assume page will not be used in the future
                    for (z = x + 1; z < pageNum && nextUse == INF; z++) {
                        if (A->data[y][x] == pageRef[z]) {
                            nextUse = z; // Found next usage of the page
                        }
                    }

                    // Find the page with the farthest next use or not used at all
                    if (nextUse > farthest) {
                        farthest = nextUse;
                        replaceIndex = y;
                    }
                }

                frameIndex = replaceIndex; // Replace this frame
            }

            // Place the new page in the selected frame
            A->data[frameIndex][x] = pageRef[x];
        }
    }
}

void displayPageOptimal(PageOptimal A, int pageNum, int frames) {
    int x, y;
    
    printf("\nSimulation Results:\n");
    printf("Page Reference | Memory State | Status\n");
    printf("----------------------------------------\n");
    
    for(x = 0; x < pageNum; x++) {
        printf("        %3d     |", x + 1);
        
        for(y = 0; y < frames; y++) {
            if(A.data[y][x] == SENTINEL) {
                printf("  - ");
            } else {
                printf(" %3d", A.data[y][x]);
            }
        }
        
        printf(" |    %c\n", A.status[x]);
    }
    
    // Display performance metrics
    printf("\nPerformance Metrics:\n");
    printf("Page Hits: %d\n", A.pageHits);
    printf("Page Faults: %d\n", A.pageFaults);
    printf("Hit Percentage: %.2f%%\n", (float)A.pageHits / pageNum * 100);
    printf("Fault Percentage: %.2f%%\n", (float)A.pageFaults / pageNum * 100);
}
