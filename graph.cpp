#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <unordered_map>
using namespace std;

// Hàm xác định độ ưu tiên của toán tử
int precedence(char operator_) {
    if (operator_ == '+') return 1;
    else if (operator_ == '*') return 2;
    else return 0;
}

// Hàm thực hiện phép toán
string apply_operator(string operand1, string operand2, char operator_) {
    return operand1 + operator_ + operand2;
}

string apply_operator_inv(string operand1, string operand2, char operator_) {
    if (operator_ == '+') return operand1 + "*" + operand2;
    else return operand1 + "+" + operand2;
}

// Định nghĩa cấu trúc đồ thị
class Graph {
public:
    map<string, vector<string>> adj_list;

    void add_node(string node) {
        if (adj_list.find(node) == adj_list.end()) {
            adj_list[node] = {};
        }
    }

    void add_edge(string u, string v) {
        // Kiểm tra xem cạnh đã tồn tại hay chưa
        if (find(adj_list[u].begin(), adj_list[u].end(), v) == adj_list[u].end()) {
            adj_list[u].push_back(v);
            adj_list[v].push_back(u); // Đồ thị vô hướng
        }
    }

    void remove_edge(string u, string v) {
        adj_list[u].erase(remove(adj_list[u].begin(), adj_list[u].end(), v), adj_list[u].end());
        adj_list[v].erase(remove(adj_list[v].begin(), adj_list[v].end(), u), adj_list[v].end());
    }

    vector<string> neighbors(string node) {
        return adj_list[node];
    }

    bool has_edge(string u, string v) {
        auto it = find(adj_list[u].begin(), adj_list[u].end(), v);
        return it != adj_list[u].end();
    }
};

// Hàm xử lý biểu thức Boolean (đánh giá biểu thức)
string evaluate_expression(string expression, vector<string>& q) {
    stack<string> operand_stack;
    stack<char> operator_stack;
    int index = 0;

    while (index < expression.size()) {
        char token = expression[index];
        if (isalpha(token)) {
            operand_stack.push(string(1, token));
            index++;
        } else if (token == '+' || token == '*') {
            while (!operator_stack.empty() && precedence(operator_stack.top()) >= precedence(token)) {
                char operator_ = operator_stack.top();
                operator_stack.pop();
                string operand2 = operand_stack.top(); operand_stack.pop();
                string operand1 = operand_stack.top(); operand_stack.pop();
                string result = apply_operator(operand1, operand2, operator_);
                q.push_back(result);
                operand_stack.push(result);
            }
            operator_stack.push(token);
            index++;
        } else if (token == '(') {
            operator_stack.push(token);
            index++;
        } else if (token == ')') {
            while (operator_stack.top() != '(') {
                char operator_ = operator_stack.top();
                operator_stack.pop();
                string operand2 = operand_stack.top(); operand_stack.pop();
                string operand1 = operand_stack.top(); operand_stack.pop();
                string result = apply_operator(operand1, operand2, operator_);
                q.push_back(result);
                operand_stack.push(result);
            }
            operator_stack.pop();
            index++;
        } else {
            index++;
        }
    }

    while (!operator_stack.empty()) {
        char operator_ = operator_stack.top();
        operator_stack.pop();
        string operand2 = operand_stack.top(); operand_stack.pop();
        string operand1 = operand_stack.top(); operand_stack.pop();
        string result = apply_operator(operand1, operand2, operator_);
        q.push_back(result);
        operand_stack.push(result);
    }

    return operand_stack.top();
}

string evaluate_expression_inv(string expression, vector<string>& q) {
    stack<string> operand_stack;
    stack<char> operator_stack;
    int index = 0;

    while (index < expression.size()) {
        char token = expression[index];
        if (isalpha(token)) {
            operand_stack.push(string(1, token));
            index++;
        } else if (token == '+' || token == '*') {
            while (!operator_stack.empty() && precedence(operator_stack.top()) >= precedence(token)) {
                char operator_ = operator_stack.top();
                operator_stack.pop();
                string operand2 = operand_stack.top(); operand_stack.pop();
                string operand1 = operand_stack.top(); operand_stack.pop();
                string result = apply_operator_inv(operand1, operand2, operator_);
                q.push_back(result);
                operand_stack.push(result);
            }
            operator_stack.push(token);
            index++;
        } else if (token == '(') {
            operator_stack.push(token);
            index++;
        } else if (token == ')') {
            while (operator_stack.top() != '(') {
                char operator_ = operator_stack.top();
                operator_stack.pop();
                string operand2 = operand_stack.top(); operand_stack.pop();
                string operand1 = operand_stack.top(); operand_stack.pop();
                string result = apply_operator_inv(operand1, operand2, operator_);
                q.push_back(result);
                operand_stack.push(result);
            }
            operator_stack.pop();
            index++;
        } else {
            index++;
        }
    }

    while (!operator_stack.empty()) {
        char operator_ = operator_stack.top();
        operator_stack.pop();
        string operand2 = operand_stack.top(); operand_stack.pop();
        string operand1 = operand_stack.top(); operand_stack.pop();
        string result = apply_operator_inv(operand1, operand2, operator_);
        q.push_back(result);
        operand_stack.push(result);
    }

    return operand_stack.top();
}

// Hàm tạo đồ thị NMOS
void create_nmos(Graph& g_nmos, string expression) {
    vector<string> q;
    string end_node = evaluate_expression(expression, q);

    // Duyệt qua từng biểu thức con
    for (int i = 0; i < q.size(); i++) {
        string sub_expression = q[i];
        vector<string> current_nodes;

        // Thêm các node từ biểu thức con
        for (char token : sub_expression) {
            if (isalpha(token)) {
                string current_node(1, token);
                g_nmos.add_node(current_node);
                current_nodes.push_back(current_node);
            }
        }

        // Thêm các cạnh giữa các node trong biểu thức con
        if (sub_expression.find('+') != string::npos) {
            // Nếu biểu thức chứa '+', các node được nối song song (không liên tiếp)
            for (size_t j = 0; j < current_nodes.size(); j++) {
                g_nmos.add_edge(current_nodes[0], current_nodes[j]); // Liên kết với node đầu tiên
            }
        } else {
            // Nếu biểu thức chứa '*', các node được nối nối tiếp
            for (size_t j = 1; j < current_nodes.size(); j++) {
                g_nmos.add_edge(current_nodes[j - 1], current_nodes[j]);
            }
        }
    }
}
void print_graph(Graph& g) {
    for (auto& pair : g.adj_list) {
        cout << pair.first << " -> ";
        for (string neighbor : pair.second) {
            cout << neighbor << " ";
        }
        cout << endl;
    }
}

// Hàm chính
int main() {
    string expression = "A*(B+C)+D*E";
    Graph g_nmos;

    // Tạo đồ thị NMOS
    create_nmos(g_nmos, expression);

    // In đồ thị NMOS
    cout << "NMOS Graph:" << endl;
    print_graph(g_nmos);

    return 0;
}