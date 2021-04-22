#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//this structure holds the data associated with a movie
struct movie {
	char* title;
	char* year;
	char* language;
	char* rating;
	struct movie* next;
};

struct movie* create_movie() {
	struct movie* m_ptr = malloc(sizeof(struct movie));

	// For use with strtok_r
	char* saveptr;

	//first token is Title
	//char* token = strtok_r(currLine, ",", &saveptr); // takes CSV and breaks first token off of current line
	m_ptr->title = NULL;

	//second token is Year
	m_ptr->year = NULL;

	//third token is Language
	m_ptr->language = NULL;

	//fourth token is Rating
	m_ptr->rating = NULL;

	return m_ptr;
}

int main() {
	
	create_movie();

	
	return 0;
}