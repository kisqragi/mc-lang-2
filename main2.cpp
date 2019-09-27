#include <iostream>
#include <string>

extern "C" {
    int add(int, int);
    int sub(int, int);
    int mul(int, int);
    int udiv(int, int);
    int test();
}

int main() {
    int x, y, result;
    char op;
    std::string op_str;

    std::cout << "電卓プログラム" << std::endl;

    while (1) {

        std::cout << "----------------------" << std::endl << std::endl;
        std::cout << "計算式 : x (op) y" << std::endl;
        std::cout << "----------------------" << std::endl << std::endl;

        std::cout << "x  : ";
        std::cin >> x;

        std::cout << "op : ";
        std::cin >> op;

        std::cout << "y  : ";
        std::cin >> y;


        switch (op) {
            case '+':
                result = add(x, y);
                op_str = "add";
                break;
            case '-':
                result = sub(x, y);
                op_str = "sub";
                break;
            case '*':
                result = mul(x, y);
                op_str = "mul";
                break;
            case '/':
                result = udiv(x, y);
                op_str = "udiv";
                break;
            default:
                std::cout << "Unknown operator" << std::endl;
                return -1;
        }

        printf("%s(%d %c %d) = %d\n", op_str.c_str(), x, op, y, result);
    }

    return 0;
}
