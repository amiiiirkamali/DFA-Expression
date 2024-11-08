#include <iostream>
#include <stack>
#include <sstream>

using namespace std;

// DFA States
enum State {
    Q1,
    Q2,
    Q3,
    Q4,
    Q5,
    TRAP
};

// Function to check if a character is a digit
bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

// Function to check if a character is an operator
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

// DFA transition function
State transition(State currentState, char input) {
    switch (currentState) {
        case Q1:
            if (isDigit(input)) return Q2;
            else if (input == '(') return Q4;
            else return TRAP;

        case Q2:
            if (isDigit(input)) return Q2;
            else if (input == '+' || input == '-' || input == '*' || input == '/') return Q3;
            else if (input == ')') return Q5;
            else return TRAP;

        case Q3:
            if (isDigit(input)) return Q2;
            else if (input == '(') return Q4;
            else return TRAP;

        case Q4:
            if (isDigit(input)) return Q2;
            else if (input == '(') return Q4;
            else return TRAP;

        case Q5:
            if (input == '+' || input == '-' || input == '*' || input == '/') return Q3;
            else if (input == ')') return Q5;
            else return TRAP;

        case TRAP:
            return TRAP;
    }

    return TRAP; // Unreachable
}

// Function to validate the expression using DFA
bool validateExpression(const string& expression) {
    State currentState = Q1;

    for (char input : expression) {
        currentState = transition(currentState, input);

        if (currentState == TRAP) {
            cout << "Invalid expression." << endl;
            return false;
        }
    }

    // Check the final state
    if (currentState != Q2 && currentState != Q5) {
        cout << "Invalid expression." << endl;
        return false;
    }

    return true;
}

// Function to apply binary operators
int applyOperator(int a, int b, char op) {
    switch (op) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            return a / b;
        default:
            return 0; // Invalid operator
    }
}

// Function to evaluate the expression with parentheses
int evaluateExpression(const string& expression) {
    stack<int> values;
    stack<char> operators;

    stringstream ss(expression);
    char token;

    while (ss >> token) {
        if (isdigit(token)) {
            ss.putback(token);
            int value;
            ss >> value;
            values.push(value);
        } else if (token == '(') {
            operators.push(token);
        } else if (token == ')') {
            while (!operators.empty() && operators.top() != '(') {
                char op = operators.top();
                operators.pop();

                int b = values.top();
                values.pop();
                int a = values.top();
                values.pop();

                values.push(applyOperator(a, b, op));
            }

            operators.pop();
        } else if (isOperator(token)) {
            while (!operators.empty() && isOperator(operators.top())) {
                char op = operators.top();
                operators.pop();

                int b = values.top();
                values.pop();
                int a = values.top();
                values.pop();

                values.push(applyOperator(a, b, op));
            }

            operators.push(token);
        }
    }

    while (!operators.empty()) {
        char op = operators.top();
        operators.pop();

        int b = values.top();
        values.pop();
        int a = values.top();
        values.pop();

        values.push(applyOperator(a, b, op));
    }

    return values.top();
}

int main() {
    cout << "Enter an expression:";
    string expression;
    getline(cin, expression);

    if (validateExpression(expression)) {
        int result = evaluateExpression(expression);
        cout << "Result: " << result << endl;
    }

    return 0;
}