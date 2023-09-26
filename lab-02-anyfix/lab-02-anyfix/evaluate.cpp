// Project identifier: 1CAEF3A0FEDD0DEC26BA9808C69D4D22A9962768

#include "evaluate.hpp"

#include <cstdint>

bool is_op(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/');
}

int64_t eval(char ch, int64_t a, int64_t b) {
    if (ch == '+')
        return a + b;
    if (ch == '-')
        return a - b;
    if (ch == '*')
        return a * b;
    else
        return a / b;
}



std::int64_t evaluate(std::string const& expression) {
    // Your implementation goes here
    vector<int64_t> operands;
    vector<char> ops;
    for (char ch : expression) {
        if (!is_op(ch)) {
            operands.push_back(static_cast<int>(ch - '0'));
        }
        else {
            ops.push_back(ch);
        }
        if (ops.size() > 0 && operands.size() > 1) {
            int64_t b = operands.back();
            operands.pop_back();
            int64_t a = operands.back();
            operands.pop_back();
            char ch = ops.back();
            ops.pop_back();
            operands.push_back(eval(ch, a, b));
        }
    }
    return operands.back();
}
