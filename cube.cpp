#include<iostream>
#include<vector>
#include<unordered_map>
#include<unordered_set>
#include<algorithm>
#include <sstream>
#include <chrono>
#include <thread>
#include <random>


#include"settings.cpp"
using namespace std;

class Cube;class Edge;class Layer;

class Edge{
    //棱块类
    public:
        unordered_map<Layer*,char>color;
};
class Corner{
    //角块类
    public:
        unordered_map<Layer*,char>color;
};
class Layer{
    //单层类
    public:
        char center_color;//中心块颜色
        vector<Edge*>e;//棱块
        vector<Corner*>c;//角块
        vector<Layer*>l;//相邻的层
        int get_ei(Edge* e){
            for(int i=0;i<4;i++){
                if(this->e[i]==e) return i;
            }
            cout<<"error from Layer::get_ei(Edge* e)"<<endl;
            return -1;
        }
        int get_ci(Corner* c){
            for(int i=0;i<4;i++){
                if(this->c[i]==c) return i;
            }
            cout<<"error from Layer::get_ci(Corner* c)"<<endl;
            return -1;
        }
};
class Cube{
    private:
        vector<Layer*> layers;
        vector<Edge*> edges;
        vector<Corner*> corners;

        vector<char> colors;
        std::unordered_map<char, std::string> color_map;
        
        //初始化
        void init() {
            // 创建所有的棱块和角块
            edges.resize(12);
            for(auto &edge : edges) {
                edge = new Edge();
            }
            corners.resize(8);
            for(auto &corner : corners) {
                corner = new Corner();
            }

            // 预定义棱块和角块的顺序
            vector<vector<int>> edge_order   = {{0, 1, 2, 3},{11, 7, 3, 6}, {8, 4, 0, 7}, {9, 5, 1, 4}, {10, 6, 2, 5},  { 10, 9, 8,11}};
            vector<vector<int>> corner_order = {{0, 1, 2, 3},{7, 3, 2, 6}, {4, 0, 3, 7}, {5, 1, 0, 4}, {6, 2, 1, 5},  {5, 4, 7, 6}};

            layers.resize(6);
            for(int i = 0; i < 6; ++i) {
                layers[i] = new Layer();
                layers[i]->center_color = colors[i];
                // 根据预定义的顺序选择正确的棱块和角块
                for(int j = 0; j < 4; ++j) {
                    layers[i]->e.push_back(edges[edge_order[i][j]]);
                    layers[i]->c.push_back(corners[corner_order[i][j]]);
                    layers[i]->e.back()->color[layers[i]] = colors[i];
                    layers[i]->c.back()->color[layers[i]] = colors[i];
                }
            }
            // 设置每个层的相邻层
            layers[0]->l = {layers[2], layers[3], layers[4], layers[1] };
            layers[1]->l = {layers[5], layers[2], layers[0], layers[4] };
            layers[2]->l = {layers[5], layers[3], layers[0], layers[1] };
            layers[3]->l = {layers[5], layers[4], layers[0], layers[2] };
            layers[4]->l = {layers[5], layers[1], layers[0], layers[3] };
            layers[5]->l = {layers[4], layers[3], layers[2], layers[1] };

            display();
        }
        
        //相应的字符显示相应颜色
        void displayColor(char color){
            cout<<color_map[color]<<color<<"\033[0m";
        }

        //旋转指定层，direction为true时逆时针，否则顺时针
        void rotate(int i, bool direction) {
            // 获取要旋转的层
            Layer* layer = layers[i];

            // 创建临时数组来存储棱块和角块的颜色
            vector<char> corner_colors_u(4),edge_colors(4), corner_colors_l(4),corner_colors_r(4),edge_colors_u(4);
            for(int j = 0; j < 4; ++j) {
                edge_colors_u[j] = layer->e[j]->color[layer];
                edge_colors[j] = layer->e[j]->color[layer->l[j]];
                corner_colors_u[j] = layer->c[j]->color[layer];
                corner_colors_l[j] = layer->c[j]->color[layer->l[j]];
                corner_colors_r[j] = layer->c[j]->color[layer->l[(j+1)%4]];
            }

            //创建临时数组存储棱块，角块在邻层的索引
            vector<int>ei(4),ci_l(4),ci_r(4);
            for(int j=0;j<4;j++){
                ei[j]  =layer->l[j]->get_ei(layer->e[j]);
                ci_l[j]=layer->l[j]->get_ci(layer->c[(j+3)%4]);
                ci_r[j]=layer->l[j]->get_ci(layer->c[j]);
            }

            // 旋转棱块和角块
            if(direction) { 
                Edge* temp_edge = layer->e[0];
                Corner* temp_corner = layer->c[0];
                for(int j = 0; j < 3; ++j) {
                    layer->e[j] = layer->e[j + 1];
                    layer->c[j] = layer->c[j + 1];
                }
                layer->e[3] = temp_edge;
                layer->c[3] = temp_corner;

            } else { 
                Edge* temp_edge = layer->e[3];
                Corner* temp_corner = layer->c[3];
                for(int j = 3; j > 0; --j) {
                    layer->e[j] = layer->e[j - 1];
                    layer->c[j] = layer->c[j - 1];
                }
                layer->e[0] = temp_edge;
                layer->c[0] = temp_corner;
            }

            // 更新相邻层中的棱块和角块
            for(int j = 0; j < 4; ++j) {
                layer->l[j]->e[ei[j]] = layer->e[j];
                layer->l[j]->c[ci_l[j]]=layer->c[(j+3)%4];
                layer->l[j]->c[ci_r[j]]=layer->c[j]; 
            }

            //重新绑定棱块和角块的颜色
            for(int j = 0; j < 4; ++j) {
              
                layer->e[j]->color.clear();
                layer->c[j]->color.clear();
                
                layer->e[j]->color[layer->l[j]]       = edge_colors    [(j + (direction ? 3 : 1)) % 4];
                layer->e[j]->color[layer]             = edge_colors_u  [(j + (direction ? 3 : 1)) % 4];
                layer->c[j]->color[layer]             = corner_colors_u[(j + (direction ? 3 : 1)) % 4];
                layer->c[j]->color[layer->l[j]]       = corner_colors_l[(j + (direction ? 3 : 1)) % 4];
                layer->c[j]->color[layer->l[(j+1)%4]] = corner_colors_r[(j + (direction ? 3 : 1)) % 4];
            }

            display();
        }

        //按WCA标准旋转指定层，direction为true时逆时针，否则顺时针
        void rotateWCA(char layer, bool direction) {
            
            if (settings.layerMap.count(layer) > 0) {
                rotate(settings.layerMap[layer], direction);
                cout<<"move:"<<layer;
                if(direction) cout<<"\'";
                cout<<endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(settings.move_delay));
            } else {
                // 错误处理：无效的层
            }
        }

        
        
        //判断魔方是否还原
        bool isRestored(){
            for(auto layer:layers){
                for(auto edge:layer->e)
                    if(edge->color[layer]!=layer->center_color)
                        return false;
                for(auto corner:layer->c)
                    if(corner->color[layer]!=layer->center_color)
                        return false;
            } 
            return true;
        }

    public:
        
        virtual ~Cube(){
            for(auto layer:layers){
                delete layer;
            }
            for(auto edge:edges){
                delete edge;
            }
            for(auto corner:corners){
                delete corner;
            }
        }

        Cube(){
            //设置颜色
            settings.setColors(colors,color_map);
            //初始化
            init();
        }

        //判断公式是否合法
        bool isFormulaValid(const std::string& formula) {
            std::unordered_set<std::string> validMoves = {"U", "U'", "U2", "D", "D'", "D2", "R", "R'", "R2", "L", "L'", "L2", "F", "F'", "F2", "B", "B'", "B2"};

            size_t pos = 0;
            while(pos < formula.size()) {
                // 找到下一个空格的位置
                size_t nextPos = formula.find(' ', pos);
                if(nextPos == std::string::npos) {
                    nextPos = formula.size();
                }

                // 提取一个动作
                std::string move = formula.substr(pos, nextPos - pos);

                // 检查这个动作是否合法
                if(validMoves.find(move) == validMoves.end()) {
                    return false;
                }

                // 移动到下一个动作
                pos = nextPos + 1;
            }

            return true;
        }
        

        //随机生成打乱公式
        string generateScramble() {
            std::string moves[12] = {"U", "U'", "U2", "D", "D'", "D2", "R", "R'", "R2", "L", "L'", "L2"};
            std::string scramble = "";
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, 11);

            int lastMove = -1;
            for(int i = 0; i < 20; ++i) {
                int nextMove;
                do {
                    nextMove = dis(gen);
                } while(nextMove / 3 == lastMove / 3); // 确保不会连续出现相同的字母

                scramble += moves[nextMove];
                if(i != 19) {
                    scramble += " ";
                }

                lastMove = nextMove;
            }

            return scramble;
        }


        //随机打乱魔方
        void scrambleCube() {
            std::string scramble = generateScramble();
            move(scramble);
            cout<<"scramble:  "<<scramble<<endl;
        }

        //重置还原魔方
        void restoreCube(){
            for(auto layer:layers){
                for(auto edge:layer->e){
                    edge->color[layer]=layer->center_color;
                }
                for(auto corner:layer->c){
                    corner->color[layer]=layer->center_color;
                }
            }
            display();
        }

        
        //根据公式转动魔方
        void move(std::string formula) {
            std::istringstream iss(formula);
            std::string move;
            while (iss >> move) {
                bool direction = false;  // 默认为顺时针
                char layer = move[0];
                if (move.size() > 1) {
                    if (move[1] == '\'') {
                        direction = true;  // 逆时针
                    } else if (move[1] == '2') {
                        // 执行两次顺时针旋转
                        rotateWCA(layer, direction);
                        rotateWCA(layer, direction);
                        continue;
                    }
                }
                rotateWCA(layer, direction);
            }

            display();
        }

        //删除多余的空格
        std::string removeExtraSpaces(const std::string& input) {
            std::string output;
            bool lastCharWasSpace = false;

            for (char c : input) {
                if (c != ' ' || !lastCharWasSpace) {
                    output += c;
                }
                lastCharWasSpace = (c == ' ');
            }

            return output;
        }

        //给出公式的逆向
        std::string generateReverseFormula(std::string scramble) {
            std::istringstream iss(scramble);
            std::string move;
            std::vector<std::string> moves;
            while (iss >> move) {
                if (move.size() > 1) {
                    if (move[1] == '\'') {
                        move[1] = ' ';  // 逆时针旋转的逆操作是顺时针旋转
                    } else if (move[1] == '2') {
                        // 180度旋转的逆操作还是180度旋转
                    }
                } else {
                    move += '\'';  // 顺时针旋转的逆操作是逆时针旋转
                }
                moves.push_back(move);
            }
            std::string solution;
            for (auto it = moves.rbegin(); it != moves.rend(); ++it) {
                if (!solution.empty()) {
                    solution += ' ';
                }
                solution += *it;
            }
            return removeExtraSpaces(solution);
        }


        /*显示当前魔方的状态
               U         
             L F R B
               D         
             Restored/Not Restored
        */
        void display() {
            //清空
            cout << "\033c";

            cout<<"\n[*]v-cube  --made by "<<"\033[34m"<<"xz"<<"\033[0m\n"<<"(github name:"<<"\033[34m"<<"666xz666"<<"\033[0m"<<")\n"<<endl;
            int j=0;
            cout <<"      "; displayColor(layers[j]->c[2]->color[layers[j]]); cout << " "; displayColor(layers[j]->e[2]->color[layers[j]]); cout << " "; displayColor(layers[j]->c[1]->color[layers[j]]); cout << "\n";
            cout <<"      "; displayColor(layers[j]->e[3]->color[layers[j]]); cout << " "; displayColor(layers[j]->center_color);           cout << " "; displayColor(layers[j]->e[1]->color[layers[j]]); cout << "\n";
            cout <<"      "; displayColor(layers[j]->c[3]->color[layers[j]]); cout << " "; displayColor(layers[j]->e[0]->color[layers[j]]); cout << " "; displayColor(layers[j]->c[0]->color[layers[j]]); cout << "\n";
            for(j=1;j<5;j++) {
                displayColor(layers[j]->c[2]->color[layers[j]]); cout << " "; displayColor(layers[j]->e[2]->color[layers[j]]); cout << " "; displayColor(layers[j]->c[1]->color[layers[j]]); cout << " ";   
            }
            cout << "\n";
            for(j=1;j<5;j++) {
                displayColor(layers[j]->e[3]->color[layers[j]]); cout << " "; displayColor(layers[j]->center_color); cout << " "; displayColor(layers[j]->e[1]->color[layers[j]]); cout << " ";
            }
            cout << "\n";
            for(j=1;j<5;j++) {
                displayColor(layers[j]->c[3]->color[layers[j]]); cout << " "; displayColor(layers[j]->e[0]->color[layers[j]]); cout << " "; displayColor(layers[j]->c[0]->color[layers[j]]); cout << " ";
            }
            cout << "\n";
            cout <<"      "; displayColor(layers[j]->c[2]->color[layers[j]]); cout << " "; displayColor(layers[j]->e[2]->color[layers[j]]); cout << " "; displayColor(layers[j]->c[1]->color[layers[j]]); cout << "\n";
            cout <<"      "; displayColor(layers[j]->e[3]->color[layers[j]]); cout << " "; displayColor(layers[j]->center_color); cout << " "; displayColor(layers[j]->e[1]->color[layers[j]]); cout << "\n";
            cout <<"      "; displayColor(layers[j]->c[3]->color[layers[j]]); cout << " "; displayColor(layers[j]->e[0]->color[layers[j]]); cout << " "; displayColor(layers[j]->c[0]->color[layers[j]]); cout << "\n";
            cout << "\n";

            if(isRestored()) cout<<"^v^ Restored\n"<<endl;
            else             cout<<"-_- Not Restored\n"<<endl;

            // std::this_thread::sleep_for(std::chrono::milliseconds(settings.move_delay));
        }
};


// int main() {
//     //test

//     return 0;
// }