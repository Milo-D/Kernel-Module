/* LDX Proof of Concept */

/* Usage (1): ./ldxuser -w <string> */
/* Usage (2): ./ldxuser -r*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define DEVICE_FILE "/dev/ldx"
#define MAX_DATA_BUFFER 15

/* Prototypes (read and write char Device) */

ssize_t read_char_dev(void);
ssize_t write_char_dev(char*);

/* Prototypes (read and write char Device) */

int main(int argc, char** argv) {

	if(argc < 2) {

		printf("Missing arguments...\n");
		return EXIT_FAILURE;
	}

	if(argc < 3 && strcmp(argv[1], "-w") == 0) {

		printf("Please specify Data to write to Char Device...\n");
		return EXIT_FAILURE;
	}

	if(strcmp(argv[1], "-r") == 0) {

		printf("Total bytes read: %ld\n", read_char_dev());
		return EXIT_SUCCESS;
	}

	if(strcmp(argv[1], "-w") == 0) {
	
		printf("Total bytes written: %ld\n", write_char_dev( argv[2] ));
		return EXIT_SUCCESS;		
	}

	printf("Unknown Option: %s\n", argv[1]);
	
	return EXIT_FAILURE;
}

ssize_t read_char_dev(void) {

	int device_fd = open(DEVICE_FILE, O_RDONLY);

	if(device_fd < 0) {

		printf("An error occured: %s,\tLDX Device returned: %d\n", strerror(errno), device_fd);
		exit( EXIT_FAILURE );
	}

	char* dev_buffer = (char*) malloc(sizeof(char) * (MAX_DATA_BUFFER + 1));

	ssize_t total_bytes = read(device_fd, (void*) dev_buffer, MAX_DATA_BUFFER);
	dev_buffer[ total_bytes - 1] = '\0';

	printf("String read from LDX Device: %s\n", dev_buffer);

	free(dev_buffer);
	close(device_fd);

	return total_bytes;
}

ssize_t write_char_dev(char* data) {

	int device_fd = open(DEVICE_FILE, O_WRONLY);

	if(device_fd < 0) {

		printf("An error occured: %s,\tLDX Device returned: %d\n", strerror(errno), device_fd);
		exit( EXIT_FAILURE );
	}

	ssize_t total_bytes = write(device_fd, (const void*) data, strlen(data));

	if(total_bytes < 0) {

		printf("An error occured: Could not write to LDX,\tLDX Device returned: %ld\n", total_bytes);
		exit( EXIT_FAILURE );
	}

	close(device_fd);

	return total_bytes;
}



