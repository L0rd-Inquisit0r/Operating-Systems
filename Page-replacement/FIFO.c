#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SENTINEL -65536

typedef struct {
    int **data;  // Memory states for each time step
    char *status; // 'H' for hit, 'M' for miss
    int pageHits; // Total page hits
    int pageFaults; // Total page faults
} PageFIFO;

void initPageFIFO(PageFIFO *A, int frames, int pageNum);
void PageReplacementFIFO(PageFIFO *A, int pageNum, int frames, int *pageRef);
void displayPageFIFO(PageFIFO A, int pageNum, int frames);
int main(){
	PageFIFO P;
	int pageNum, frames, x;
	
	printf("Please enter the number of frames: ");
	scanf("%d", &frames);
	
	printf("Please enter the number of pages: ");
	scanf("%d", &pageNum);
	
	initPageFIFO(&P, frames, pageNum);
	
	int pageRef[pageNum];
	
	printf("Please enter %d page reference/s: ");
	for(x = 0; x < pageNum; x++){
		scanf("%d", &pageRef[x]);
	}
	
	PageReplacementFIFO(&P, pageNum, frames, pageRef);
	displayPageFIFO(P, pageNum, frames);
	return 0;
}

void initPageFIFO(PageFIFO *A, int frames, int pageNum) {
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

void PageReplacementFIFO(PageFIFO *A, int pageNum, int frames, int *pageRef) {
    int ptr = 0, x, y;
    
    for(x = 0; x < pageNum; x++) {
        int found = 0;
        
        // Check if the page is already in memory
        for(y = 0; y < frames && !found; y++) {
            if(A->data[y][x == 0 ? 0 : x - 1] == pageRef[x]) {
                found = 1;
            }
        }
        
        // Copy the previous state
        for(y = 0; y < frames; y++) {
            A->data[y][x] = (x == 0) ? SENTINEL : A->data[y][x - 1];
        }
        
        if(found) {
            A->status[x] = 'H'; // Page hit
            A->pageHits++;
        } else {
            A->status[x] = 'F'; // Page fault
            A->pageFaults++;
            
            // Replace the page in the current frame
            A->data[ptr][x] = pageRef[x];
            ptr = (ptr + 1) % frames; // Move to the next frame
        }
    }
}

void displayPageFIFO(PageFIFO A, int pageNum, int frames) {
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




