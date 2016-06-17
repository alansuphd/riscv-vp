#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	char input;
	printf(
	"#########################################\n"
	"#                                       #\n"
	"#      	  This is our I/O test!         #\n"
	"#        Key in @ to exit I/O test      #\n"
	"#                                       #\n"
	"#########################################\n"
	);

	while(1){
		scanf("%c", &input);
		printf("output: %c \n", input);
		if(input == '@')
			break;
	}
	printf(
"#########################################\n"
"#                                       #\n"
"#      	     Enter Bash!!               #\n"
"#                                       #\n"
"#########################################\n"
);

	execl("/bin/bash", "/bin/bash", NULL);

   return 0;
}
