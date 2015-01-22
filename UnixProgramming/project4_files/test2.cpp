#include <stdio.h>
#include <string.h>

int main(){
	char good[] = "Go Gophers";
	for (int i = 0; i < strlen(good); ++i)
	{
		good[i] = good[i]+6;
	}
	printf("%s\n", good);
	return 0;
	
}