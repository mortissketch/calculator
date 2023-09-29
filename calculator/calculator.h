// Created by mortissketch 28.09.2023

#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <corecrt_math_defines.h>

using namespace std;

struct lexeme {
	char operation;
	double number;
};

void greeting();
void input_expression(string* expression);
void output_result(double result);
void change_templates(string* expression);
string change_one(string expression, string sample, string changed);
double get_number(string expression, size_t* index);
void create_polish_notation(string expression, vector <lexeme>* result);
void calculating(vector<lexeme> expression, double* result);
void binary_calc(vector<lexeme>* expression, size_t* index);
double binary_num_num(double number1, double number2, char operation);
double unary_num(double number, char operation);
void unary_calc(vector<lexeme>* expression, size_t* index);
