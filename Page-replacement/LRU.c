#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SENTINEL -65536
#define INF 65536

typedef struct {
    int data; // Page number
    int key;  // Key to track time (for LRU logic)
} LRUData;

typedef struct {
    LRUData **data; // Memory states for each frame
    char *status;   // 'H' for hit, 'M' for miss
    int pageHits;   // Total page hits
    int pageFaults; // Total page faults
} PageLRU;

void initPageLRU(PageLRU *A, int frames, int pageNum);
void PageReplacementLRU(PageLRU *A, int pageNum, int frames, int *pageRef);
void displayPageLRU(PageLRU A, int pageNum, int frames);

int main() {
    PageLRU P;
    int pageNum, frames, x;
    
    // Get user input for number of frames and pages
    printf("Please enter the number of frames: ");
    scanf("%d", &frames);
    
    printf("Please enter the number of pages: ");
    scanf("%d", &pageNum);
    
    initPageLRU(&P, frames, pageNum);
    
    int pageRef[pageNum];
    printf("Please enter %d page reference(s):\n", pageNum);
    for(x = 0; x < pageNum; x++) {
        scanf("%d", &pageRef[x]);
    }
    
    // Perform LRU page replacement
    PageReplacementLRU(&P, pageNum, frames, pageRef);
    
    // Display the simulation results
    displayPageLRU(P, pageNum, frames);
    
    // Free allocated memory
    for(x = 0; x < frames; x++) {
        free(P.data[x]);
    }
    free(P.data);
    free(P.status);
    
    return 0;
}

void initPageLRU(PageLRU *A, int frames, int pageNum) {
    int x, y;
    A->data = (LRUData **)malloc(frames * sizeof(LRUData *));
    A->status = (char *)malloc(pageNum * sizeof(char));
    A->pageHits = 0;
    A->pageFaults = 0;
    
    if(A->data != NULL && A->status != NULL) {
        for(x = 0; x < frames; x++) {
            A->data[x] = (LRUData *)malloc(pageNum * sizeof(LRUData));
            if(A->data[x] != NULL) {
                for(y = 0; y < pageNum; y++) {
                    A->data[x][y].data = SENTINEL; // Initialize with sentinel
                    A->data[x][y].key = INF;        // Key is initially invalid
                }
            }
        }
    }
}

void PageReplacementLRU(PageLRU *A, int pageNum, int frames, int *pageRef) {
    int time = 0; // Tracks the global time for the LRU logic
    int x, y, lruIndex;

    for (x = 0; x < pageNum; x++) {
        int found = 0; // To track if the page is found in memory

        // Copy the previous state for this step
        for (y = 0; y < frames; y++) {
            A->data[y][x] = (x == 0) ? (LRUData){SENTINEL, -1} : A->data[y][x - 1];
        }

        // Check if the page is already in memory
        for (y = 0; y < frames && !found; y++) {
            if (A->data[y][x].data == pageRef[x]) {
                found = 1;
                A->data[y][x].key = time++; // Update key (reset its last-used time)
            }
        }

        if (found) {
            A->status[x] = 'H'; // Page hit
            A->pageHits++;
        } else {
            A->status[x] = 'F'; // Page fault
            A->pageFaults++;

            // Find the least recently used page to replace
            int minKey = INF;
            lruIndex = -1;
            for (y = 0; y < frames; y++) {
                if (A->data[y][x].data == SENTINEL) {
                    // If there is an empty frame, use it immediately
                    lruIndex = y;
                    break;
                }
                if (A->data[y][x].key < minKey) {
                    minKey = A->data[y][x].key;
                    lruIndex = y;
                }
            }

            // Replace the LRU page or assign to an empty frame
            if (lruIndex != -1) {
                A->data[lruIndex][x].data = pageRef[x];
                A->data[lruIndex][x].key = time++; // Set its last-used time
            }
        }
    }
}

void displayPageLRU(PageLRU A, int pageNum, int frames) {
    int x, y;
    
    printf("\nSimulation Results:\n");
    printf("Page Reference | Memory State | Status\n");
    printf("----------------------------------------\n");
    
    for(x = 0; x < pageNum; x++) {
        printf("        %3d     |", x + 1);
        
        for(y = 0; y < frames; y++) {
            if(A.data[y][x].data == SENTINEL) {
                printf("  - ");
            } else {
                printf(" %3d", A.data[y][x].data);
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

