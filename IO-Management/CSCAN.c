#include <stdio.h>
#include <stdlib.h>

// Structure to represent the Disk Arm

typedef struct node{
	int request;
	struct node *link;
}*ReqNode;

typedef struct {
    int initialPosition; // Initial position of the disk arm
    ReqNode requests;       // Array of track requests
    int totalHeadMovement; // Total head movement
    int trackSize;        // Total number of tracks on the disk
} DiskCSCAN;

// Function prototypes
void initializeDisk(DiskCSCAN *A, int initialPosition, int diskSize);
void processRequestsCSCAN(DiskCSCAN *A, int *requests, int numRequests);
void displayResults(DiskCSCAN A);

int main() {
    DiskCSCAN D;
    int initialPosition, numRequests, diskSize, x;

    // Input: Disk size
    printf("Enter the track size on the disk: ");
    scanf("%d", &diskSize);

    // Input: Initial position of the disk arm
    printf("Enter the initial position of the disk arm: ");
    scanf("%d", &initialPosition);
	
	// Initialize the disk
    initializeDisk(&D, initialPosition, diskSize);
    
    // Input: Number of track requests
    printf("Enter the number of track requests: ");
    scanf("%d", &numRequests);
	int requests[numRequests];
    

    // Input: Sequence of track requests
    printf("Enter the track requests:\n");
    for (x = 0; x < numRequests; x++) {
        scanf("%d", &requests[x]);
    }

    // Process the requests using C-SCAN
    processRequestsCSCAN(&D, requests, numRequests);

    // Display the results
    displayResults(D);

    return 0;
}

// Function to initialize the disk
void initializeDisk(DiskCSCAN *A, int initialPosition, int diskSize) {
    A->initialPosition = initialPosition;
    A->trackSize = diskSize;
    A->totalHeadMovement = 0;
    A->requests = NULL;
}

// Function to process the requests using C-SCAN
void processRequestsCSCAN(DiskCSCAN *A, int *requests, int numRequests) {
    ReqNode low = NULL, high = NULL;
    ReqNode *trav, temp, node;
    int x, curPos, highDiff, lowDiff, alpha;

    // Sort requests into low and high lists
    for (x = 0; x < numRequests; x++) {
        node = (ReqNode)malloc(sizeof(struct node));
        if (node != NULL) {
            if (A->initialPosition > requests[x]) {
                for (trav = &low; *trav != NULL && (*trav)->request < requests[x]; trav = &(*trav)->link) {}
            } else {
                for (trav = &high; *trav != NULL && (*trav)->request < requests[x]; trav = &(*trav)->link) {}
            }
            node->request = requests[x];
            node->link = *trav;
            *trav = node;
        }
    }

    // Add the end track (trackSize - 1) to high
    if (low != NULL) {
        node = (ReqNode)malloc(sizeof(struct node));
        if (node != NULL) {
            for (trav = &high; *trav != NULL; trav = &(*trav)->link) {}
            node->request = A->trackSize - 1;
            node->link = NULL;
            *trav = node;
        }
        
        node = (ReqNode)malloc(sizeof(struct node));
        if (node != NULL) {
            node->request = 0;
            node->link = low;
            low = node;
        }
    }
	
	// Calculate THM
    curPos = A->initialPosition;
	for (temp = high; temp != NULL; temp = temp->link) {
		A->totalHeadMovement += abs(temp->request - curPos);
		curPos = temp->request;
	}
	
	if(low != NULL){
		curPos = 0;
		for (temp = low; temp != NULL; temp = temp->link) {
			A->totalHeadMovement += abs(temp->request - curPos);
			curPos = temp->request;
		}
		
	}
	
    // Concatenate high and low
    A->requests = high;
    for (trav = &high; *trav != NULL; trav = &(*trav)->link) {}
    *trav = low;

	for (temp = A->requests; temp->link != NULL; temp = temp->link) {}
	alpha = abs(A->initialPosition - temp->request);
	A->totalHeadMovement += alpha;
}

// Function to display the results
void displayResults(DiskCSCAN A) {
    printf("\nOrder of requests served (C-SCAN):\n");
    ReqNode temp;
    for(temp = A.requests; temp != NULL; temp = temp->link){
    	printf("%d ",temp->request);
	}

    printf("\n\nTotal head movement: %d\n", A.totalHeadMovement);
}
