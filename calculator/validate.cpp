#include "validate.h"

void main_validate(string expression, error_list* error)
{
	if (expression.length() > 255) *error = LENGTH;
	if (*error == OK) other_symbols(expression, error);
	if (*error == OK) count_brackets(expression, error);
}

void other_symbols(string expression, error_list *error)
{
	string sample = "1234567890.-+*/^sincotaqrmdlg()";
	for (size_t i = 0; i < expression.length() && *error == OK; i++) {
		if (sample.find(expression[i]) == string::npos)
			*error = UNKNOWN_SYMBOLS;
	}
}

void count_brackets(string expression, error_list* error)
{
	int bracket_flag = 0;
	for (size_t i = 0; i < expression.length() && *error == OK; i++) {
		if (expression[i] == '(') bracket_flag++;
		if (expression[i] == ')') bracket_flag--;
		if (bracket_flag < 0) *error = BRACKETS;
	}
	if (*error == OK && bracket_flag != 0) *error = BRACKETS;
}

//void correct_operation(string expression, error_list* error)
//{
//	string num_brack_sample = "01234567890.()", binary_op_sample = "+-/*^";
//	for (size_t i = 0; i < expression.length(); i++) {
//		if (binary_op_sample.find(expression[i]) != string::npos) {
//			if (numb_brack_sample.find(expression[i - 1]) != string::npos &&
//				numb_brack_sample.find(expression[i + 1]) != string::npos) {
//
//			}
//		}
//	}
//}
