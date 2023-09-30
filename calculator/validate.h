#include <string>
using namespace std;


enum error_list {
	OK,
	LENGTH,
	UNKNOWN_SYMBOLS,
	BRACKETS,
	INCORRECT_INPUT
};

void main_validate(string expression, error_list* error);
void other_symbols(string expression, error_list *error);
void count_brackets(string expression, error_list* error);
//void correct_operation(string expression, error_list* error);

