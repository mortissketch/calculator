#include "calculator.h"

/*
* Этот калькулятор роботает с целым выражением в формате строки
* На ввод могут приниматься операции и функции: +, -, /, *, ^, mod,
* sqrt, sin, cos, tan, asin, acos, atan, ln, log -- а также скобки.
* Принцип работы основан на обратной польской натации
* (RPN - reverse polish notation)
* Осталась только обработка исключений и проверка на корректность
* вводимых данных
*/

int main()
{
    // Приветствие
    greeting();

    int flag = 1;
    while (flag) {

        string expression;
        double result;
        vector<lexeme> lex;
        error_list error = OK;

        // Ввод выражения
        input_expression(&expression);

        // Проверка выражения на корректность ввода
        main_validate(expression, &error);

        if (expression == "stop") flag = 0;
        else if (error != OK) {
            cout << "error type: " << error << endl;
        }
        else 
        {

            // работа с выражением, подсчет
            change_templates(&expression);
            create_polish_notation(expression, &lex);
            calculating(lex, &result);

            // Вывод ответа
            output_result(result);
        }

    }
    return 0;
}

/// <summary>
/// Функция выводит на экран сообщение с приветствием и
/// просьбой ввести выражение
/// </summary>
void greeting()
{
    cout << "\t\tYou are welcome! This is a calculator" << endl;
    cout << "You can use: +, -, *, /, ^, sqrt, log, ln, mod,";
    cout << " sin, cos, tan, asin, acos, atan" << endl;
    cout << "If you want to stop write \"stop\"" << endl;
}

/// <summary>
/// Функция принимает на ввод выражение
/// </summary>
/// <param name="expression"></param>
void input_expression(string* expression)
{
    cout << "\nPlease enter your expression : ";
    cin >> *expression;
}

/// <summary>
/// Выводит результат подсчета
/// </summary>
/// <param name="result"></param>
void output_result(double result)
{
    cout << "Your answer is: " << setprecision(15) <<result << endl;
}

/// <summary>
/// Заменяет все функции, которые состоят больее чем из 1 символа
/// К примеру: tan -> t, sin -> s, mod -> % и тд
/// </summary>
/// <param name="expression"></param>
void change_templates(string* expression)
{
    // Шаблоны, которые будут искаться в выражении и затем заменяться
    string samples[] = { "asin", "acos", "atan", "sqrt", "mod",
                         "log", "ln", "sin", "cos", "tan" };

    // То, на что будут заменены найденные шаблоны
    string changed[] = { "S", "C", "T", "q", "%",
                         "l", "L", "s", "c", "t" };

    for (size_t i = 0; i < 10; i++)
        *expression = change_one(*expression, samples[i], changed[i]);
}

/// <summary>
/// Заменяет указанный шаблон "sample" на заданную строку 
/// "changed", если такой шаблон найден
/// </summary>
/// <param name="expression"></param>
/// <param name="sample"></param>
/// <param name="chenged"></param>
string change_one(string expression, string sample, string changed)
{
    string result;
    size_t index = expression.find(sample);

    if (index != string::npos) {
        for (size_t i = 0; i < index; i++) result += expression[i];
        result += changed;
        for (size_t i = index + sample.length(); i < expression.length(); i++)
            result += expression[i];
    }
    else result = expression;

    return result;
}

/// <summary>
/// выделяет по индексу первое число
/// </summary>
/// <param name="expression"></param>
/// <param name="number"></param>
double get_number(string expression, size_t* index)
{
    double result;
    string sample_numbers = "0123456789.", tmp;
    while (sample_numbers.find(expression[*index]) != string::npos) {
        tmp += expression[*index];
        *index += 1;
    }
    *index -= 1;
    result = atof(tmp.c_str());
    return result;
}

/// <summary>
/// создает вектор, элементы которого являются элементами
/// исходного выражения, но стоящие в порядке
/// обратной польской нотации
/// </summary>
/// <param name="expression"></param>
/// <param name="result"></param> 
void create_polish_notation(string expression, vector<lexeme>* result)
{
    stack <lexeme> operations;
    string sample_numbers = "0123456789.", priority[] = { "+-", "*/%", "^","sctSCTqlL" };
    for (size_t i = 0; i < expression.length(); i++) {
        lexeme tmp;
        if (sample_numbers.find(expression[i]) != string::npos) {
            tmp.number = get_number(expression, &i);
            tmp.operation = 'Z';
            result->push_back(tmp);
        }
        else if (expression[i] == '(') {
            tmp.operation = '(';
            operations.push(tmp);
        }
        else if (expression[i] == ')') {
            while (operations.top().operation != '(') {
                result->push_back(operations.top());
                operations.pop();
            }
            operations.pop();
        }
        else if (operations.size() == 0 || operations.top().operation == '(') {
            tmp.operation = expression[i];
            operations.push(tmp);
        }
        else if (priority[3].find(expression[i]) != string::npos) {
            if (priority[3].find(operations.top().operation) != string::npos) {
                result->push_back(operations.top());
                operations.pop();
            }
            tmp.operation = expression[i];
            operations.push(tmp);
        }
        else if (priority[2].find(expression[i]) != string::npos) {
            if (priority[3].find(operations.top().operation) != string::npos ||
                priority[2].find(operations.top().operation) != string::npos) {
                result->push_back(operations.top());
                operations.pop();
            }
            tmp.operation = expression[i];
            operations.push(tmp);
        }
        else if (priority[1].find(expression[i]) != string::npos) {
            if (priority[0].find(operations.top().operation) == string::npos) {
                result->push_back(operations.top());
                operations.pop();
            }
            tmp.operation = expression[i];
            operations.push(tmp);
        }
        else if (priority[0].find(expression[i]) != string::npos) {
            result->push_back(operations.top());
            operations.pop();
            tmp.operation = expression[i];
            operations.push(tmp);
        }
    }
    while (operations.size() != 0) {
        result->push_back(operations.top());
        operations.pop();
    }
}

/// <summary>
/// производит подсчет заданного выражения
/// </summary>
/// <param name="expression"></param>
/// <param name="result"></param>
void calculating(vector<lexeme> expression, double* result)
{
    size_t index = 0;
    string binary_operations = "+-*/^%";
    while (!(expression.size() == 1 && expression[0].operation == 'Z')) {
        if (expression[index].operation != 'Z') {
            if (binary_operations.find(expression[index].operation) != string::npos)
                binary_calc(&expression, &index);
            else
                unary_calc(&expression, &index);
        }
        index++;
    }
    *result = expression[0].number;
}

/// <summary>
/// вычисляет бинарные операции: +,-,/,*,^,% и изменяет вектор
/// </summary>
/// <param name="expression"></param>
/// <param name="index"></param>
void binary_calc(vector<lexeme>* expression, size_t* index)
{
    lexeme result;
    result.operation = 'Z';
    result.number = binary_num_num((*expression)[*index - 2].number, (*expression)[*index - 1].number, (*expression)[*index].operation);
    (*expression)[*index - 2] = result;
    expression->erase(expression->begin() + (*index - 1), expression->begin() + (*index + 1));
    *index = 0;
}

/// <summary>
/// производит непосредственно заданную операцию
/// </summary>
/// <param name="number1"></param>
/// <param name="number2"></param>
/// <param name="operation"></param>
/// <returns></returns>
double binary_num_num(double number1, double number2, char operation)
{
    double result=0;
    switch (operation)
    {
    case '+': result = number1 + number2;
        break;
    case '-': result = number1 - number2;
        break;
    case '*': result = number1 * number2;
        break;
    case '/': result = number1 / number2;
        break;
    case '^': result = pow(number1, number2);
        break;
    case '%': result = modf(number1, &number2);
        break;
    default:
        break;
    }
    return result;
}

/// <summary>
/// работает с унарными операциями (триганометрическими, а так же
/// с квадратным корнем и логорифмами) + изменяет вектор
/// </summary>
/// <param name="number"></param>
/// <param name="operation"></param>
/// <returns></returns>
double unary_num(double number, char operation)
{
    double result=0;
    switch (operation)
    {
    case 's': result = sin(number * M_PI / 180);
        break;
    case 'c': result = cos(number * M_PI / 180);
        break;
    case 't': result = tan(number * M_PI / 180);
        break;
    case 'S': result = asin(number);
        break;
    case 'C': result = acos(number);
        break;
    case 'T': result = atan(number);
        break;
    case 'q': result = sqrt(number);
        break;
    case 'l': result = log10(number);
        break;
    case 'L': result = log(number);
        break;
    default:
        break;
    }
    return result;
}

/// <summary>
/// работает непосредственно с заданной операцией и числом
/// </summary>
/// <param name="expression"></param>
/// <param name="index"></param>
void unary_calc(vector<lexeme>* expression, size_t* index)
{
    lexeme result;
    result.operation = 'Z';
    result.number = unary_num((*expression)[*index - 1].number, (*expression)[*index].operation);
    (*expression)[*index - 1] = result;
    expression->erase(expression->begin() + (*index), expression->begin() + (*index+1));
    *index = 0;
}
