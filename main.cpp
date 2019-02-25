#include <iostream>
#include "stack.hpp"
#include <string>

using namespace std;

template<typename T>
bool test(stack<T> & st, T* test_cont, int size) {
    bool ans = true;
    for (int i = 0; i < size; ++i) {
        st.push(test_cont[i]);
    }
    for (int i = size; i > 0; --i) {
        T extr = st.pop();
        if (extr != test_cont[i - 1]) {
            cout << "Unexpected poped value. Expected " << test_cont[i - 1] << " vs " << extr << endl;
            ans = false;
        }
    }
    return ans;
}

int main()
{
    {
        stack<int> stack_int;
        int test_content[] = {42, 32, 43, 343, 43251, -1233, 0, -431};
        test(stack_int, test_content, sizeof(test_content) / sizeof (int));
        cout << "stack size: " << stack_int.size() << endl;
    }
    {
        stack<std::string> stack_str;
        std::string test_content[] = {"42", "32", "43", "343", "43251", "-1233", "0", "-431"};
        test(stack_str, test_content, sizeof(test_content) / sizeof (std::string));
        cout << "stack size: " << stack_str.size() << endl;
    }

    return 0;
}
