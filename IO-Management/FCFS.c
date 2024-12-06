#include <stdio.h>
#include <stdlib.h>

// Structure to represent the Disk Arm
typedef struct {
    int initialPosition;  // Initial position of the disk arm
    int *requests;        // Array of track requests
    int numRequests;      // Number of requests
    int totalHeadMovement; // Total head movement
} DiskFCFS;

// Function prototypes
void initializeDisk(DiskFCFS *disk, int initialPosition, int numRequests);
void processRequestsFCFS(DiskFCFS *disk);
void displayResults(DiskFCFS disk);

int main() {
    DiskFCFS disk;
    int initialPosition, numRequests, x;

    // Input: Initial position of the disk arm
    printf("Enter the initial position of the disk arm: ");
    scanf("%d", &initialPosition);

    // Input: Number of track requests
    printf("Enter the number of track requests: ");
    scanf("%d", &numRequests);

    // Initialize the disk
    initializeDisk(&disk, initialPosition, numRequests);

    // Input: Sequence of track requests
    printf("Enter the track requests:\n");
    for (x = 0; x < numRequests; x++) {
        scanf("%d", &disk.requests[x]);
    }

    // Process the requests using FCFS
    processRequestsFCFS(&disk);

    // Display the results
    displayResults(disk);

    return 0;
}

// Function to initialize the disk
void initializeDisk(DiskFCFS *disk, int initialPosition, int numRequests) {
    disk->initialPosition = initialPosition;
    disk->numRequests = numRequests;
    disk->totalHeadMovement = 0;
    disk->requests = (int *)malloc(numRequests * sizeof(int));
}

// Function to process the requests using FCFS
void processRequestsFCFS(DiskFCFS *disk) {
    int currentPosition = disk->initialPosition, x;
    for (x = 0; x < disk->numRequests; x++) {
        // Calculate the head movement
        disk->totalHeadMovement += abs(disk->requests[x] - currentPosition);

        // Update the current position of the disk arm
        currentPosition = disk->requests[x];
    }
}

// Function to display the results
void displayResults(DiskFCFS disk) {
	int x;
    printf("\nOrder of requests served:\n");
    for (x = 0; x < disk.numRequests; x++) {
        printf("%d ", disk.requests[x]);
    }

    printf("\n\nTotal head movement: %d\n", disk.totalHeadMovement);
}

