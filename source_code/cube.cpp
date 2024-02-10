#include<iostream>
#include<vector>
#include<unordered_map>
#include<unordered_set>
#include<algorithm>
#include <sstream>
#include <chrono>
#include <thread>
#include <random>
#include<stack>
#include <cctype>
#include <array>
#include <memory>
#include <filesystem>
#include "include/json.hpp"


#include"settings.cpp"
using namespace std;

class Cube;class Edge;class Layer;

//棱块类
class Edge{
    
    public:
        unordered_map<Layer*,char>color;
};

//角块类
class Corner{
    
    public:
        unordered_map<Layer*,char>color;
};

//单层类
class Layer{
    
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


//魔方类
class Cube{
    private:
        //主体
        vector<Layer*> layers;
        vector<Edge*> edges;
        vector<Corner*> corners;

        //操作栈
        stack<string>st_back;
        stack<string>st_forward;
        stack<string>st;

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
            vector<vector<int>> edge_order   = {{0, 1, 2, 3},{11, 7, 3, 6},
             {8, 4, 0, 7}, {9, 5, 1, 4},{10, 6, 2, 5},{ 10, 9, 8,11}};
            vector<vector<int>> corner_order = {{0, 1, 2, 3},{7, 3, 2, 6},
             {4, 0, 3, 7},{5, 1, 0, 4},{6, 2, 1, 5},{5, 4, 7, 6}};

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
            if(i>=0&&i<6){//单层
            // 获取要旋转的层
            Layer* layer = layers[i];

            // 创建临时数组来存储棱块和角块的颜色
            vector<char> corner_colors_u(4),edge_colors(4),
             corner_colors_l(4),corner_colors_r(4),edge_colors_u(4);
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
                
                layer->e[j]->color[layer->l[j]]       =
                 edge_colors    [(j + (direction ? 3 : 1)) % 4];
                layer->e[j]->color[layer]             =
                 edge_colors_u  [(j + (direction ? 3 : 1)) % 4];
                layer->c[j]->color[layer]             =
                 corner_colors_u[(j + (direction ? 3 : 1)) % 4];
                layer->c[j]->color[layer->l[j]]       =
                 corner_colors_l[(j + (direction ? 3 : 1)) % 4];
                layer->c[j]->color[layer->l[(j+1)%4]] =
                 corner_colors_r[(j + (direction ? 3 : 1)) % 4];
            }
            }else if(i>=6&&i<9){ //中间层
                //单层反向
                rotate(settings.middleLayerMap[i],!direction);
                //双层同向
                rotate(settings.middleLayerMap[i]+9,direction);
            }else{               //双层
                //对层同方向旋转
                rotate(settings.faceToMap[i-9],direction);
        
                //整体旋转
                if(i==9||i==12||i==11){
                    rotateWholeCube(settings.axisMap[i-9],direction);
                
                }else{
                    rotateWholeCube(settings.axisMap[i-9],!direction);
                
                }
            }
            //display();交给rotateWCA函数显示

        }

        // 旋转整个魔方
        void rotateWholeCube(char axis, bool direction) {
         
            vector<int> l_index(4),e1_index(4),e2_index(4);
            vector<char> cen_color(4),e1_color(4),e2_color(4);
            switch(axis) {
                case 'x': 
                    rotate(1, !direction);
                    rotate(3, direction);
                    l_index = {5,2,0,4}; 
                    e1_index={0,0,0,2};
                    e2_index={2,2,2,0};
                    break;
                case 'y': 
                    rotate(0, direction);
                    rotate(5, !direction);
                    l_index = {4,3,2,1}; 
                    e1_index={1,1,1,1};
                    e2_index={3,3,3,3};
                    break;
                case 'z': 
                    rotate(2, direction);
                    rotate(4, !direction);
                    display();
                    l_index = {0,3,5,1}; 
                    e1_index={3,2,1,0};
                    e2_index={1,0,3,2};
                    break;
            }

            //保存颜色
            for(int i=0;i<4;i++){
                cen_color[i]=layers[l_index[i]]->center_color;
                e1_color[i]=
                layers[l_index[i]]->e[e1_index[i]]->color[layers[l_index[i]]];
                e2_color[i]=
                layers[l_index[i]]->e[e2_index[i]]->color[layers[l_index[i]]];
            }

            
            if(!direction) {
                // 顺时针旋转
                std::rotate(l_index.begin(), l_index.begin() + 1, l_index.end());
                std::rotate(e1_index.begin(), e1_index.begin() + 1, e1_index.end());
                std::rotate(e2_index.begin(), e2_index.begin() + 1, e2_index.end());
            } else {
                // 逆时针旋转
                std::rotate(l_index.rbegin(), l_index.rbegin() + 1, l_index.rend());
                std::rotate(e1_index.rbegin(), e1_index.rbegin() + 1, e1_index.rend());
                std::rotate(e2_index.rbegin(), e2_index.rbegin() + 1, e2_index.rend());
            } 

            //更新颜色
            for(int i=0;i<4;i++){
                layers[l_index[i]]->center_color=cen_color[i];
                layers[l_index[i]]->e[e1_index[i]]->color[layers[l_index[i]]]=e1_color[i];
                layers[l_index[i]]->e[e2_index[i]]->color[layers[l_index[i]]]=e2_color[i];
            }
           
        }

        //按WCA标准旋转指定层，direction为true时逆时针，否则顺时针，isForward为前进标记
        void rotateWCA(char layer, bool direction, bool isForward) {
            if (layer == 'x' || layer == 'y' || layer == 'z') { // 如果是整体旋转
                rotateWholeCube(layer, direction);
                display();
                cout << "move:" << layer;
                if (direction) cout << "\'";
                cout << endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(settings.move_delay));
            } else if (settings.layerMap.count(layer) > 0) { // 如果是单层旋转
                rotate(settings.layerMap[layer], direction);
                display();
                cout << "move:" << layer;
                if (direction) cout << "\'";
                cout << endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(settings.move_delay));
            } else {
                // 错误处理：无效的层
            }

            //前进操作
            if (isForward) {
                //将操作存入操作栈
                string step;
                step += layer;
                if (direction)
                    step += '\'';

                st_back.push(step);
                st.push(step);

                while (!st_forward.empty()) st_forward.pop();
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

        //删除所有空格
        void removeAllSpaces(string&str){
            str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
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

      




    public:

          //利用python脚本从Algdb获取公式
        vector<string> generateAlgdbFormulas(string& type,string& caseName){
           
            std::filesystem::path currentFilePath(__FILE__);
            std::filesystem::path currentDir = currentFilePath.parent_path();
            std::filesystem::path pythonFilePath = currentDir / "algdb.py";

            std::string command = "python ";
            command += pythonFilePath.string();
            command+= " get_algs ";
            command+= type;

            // 创建一个管道并运行命令
            std::array<char, 128> buffer;
            std::string result;
            std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
            if (!pipe) {
                throw std::runtime_error("popen() failed!");
            }
            while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
                result += buffer.data();
            }

            std::string jsonString = result;

            nlohmann::json jsonArray = nlohmann::json::parse(jsonString);

            // 现在 jsonArray 是一个包含多个 JSON 对象的数组
            for (nlohmann::json& j : jsonArray) {
                // 你可以像操作普通 C++ 对象一样操作每个 JSON 对象
                
                if(j["name"]==caseName){
                    // 你可以像操作普通 C++ 对象一样操作每个 JSON 对象
                    auto jsonAlgArray = j["caseAlgs"];
                    vector<string>algList;
                    for (nlohmann::json& alg : jsonAlgArray) {
                        std::string algValue = alg["moves"];

                        algList.push_back(algValue);
                    }
                    return algList;
                }
            }

            return {};
        }
        
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
            std::unordered_set<std::string> validMoves = { 
            "U", "U'", "U2","D", "D'","D2", 
            "R", "R'", "R2", "L", "L'","L2",
            "F", "F'", "F2", "B", "B'","B2",
            "x", "x'", "x2", "y", "y'", "y2", 
            "z", "z'", "z2","M", "M'", "M2",
            "E", "E'", "E2","S", "S'", "S2",
            "u", "u'", "u2","d", "d'","d2",
            "r", "r'", "r2", "l", "l'","l2",
            "f", "f'", "f2", "b", "b'","b2"
            };

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
            std::string moves[12] = {"U", "U'", "U2", "D", "D'", "D2",
                                     "R", "R'", "R2", "L", "L'", "L2"};
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

        //重置魔方
        void resetCube(){
            for(int i=0;i<6;i++){
                layers[i]->center_color=colors[i];
                for(auto edge:layers[i]->e){
                    edge->color[layers[i]]=colors[i];
                }
                for(auto corner:layers[i]->c){
                    corner->color[layers[i]]=colors[i];
                }
            }
            
            while(!st_back.empty())st_back.pop();
            while(!st_forward.empty())st_forward.pop();
            while(!st.empty())st.pop();
            display();
        }

        //逆序还原
        void restoreByReverse(){
            while(!st.empty()){
                string step=st.top();
                st.pop();
                string step_reverse=generateReverseFormula(step);
                removeAllSpaces(step_reverse);
                if(step_reverse.size()==2){
                    //逆时针
                    rotateWCA(step_reverse[0],true,false);
                }else{
                    //顺时针
                    rotateWCA(step_reverse[0],false,false);
                }
            }
            //清空所有操作栈
            while(!st_forward.empty())st_forward.pop();
            while(!st_back.empty())st_back.pop();

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
                        rotateWCA(layer, direction,true);
                        rotateWCA(layer, direction,true);
                        continue;
                    }
                }
                rotateWCA(layer, direction,true);
            }

            display();
        }

        //撤销操作
        void stepBack(){
            if(!st_back.empty()){
                string step=st_back.top();
                st_back.pop();

                string step_back=generateReverseFormula(step);
                removeAllSpaces(step_back);

                if(step_back.size()==2)
                    //逆时针
                    rotateWCA(step_back[0],true,false);
                else{
                    //顺时针
                    rotateWCA(step_back[0],false,false);
                }

                st_forward.push(step);
                st.pop();
                display();
            }else{
                cout<<"\033[31m"<<"No step back"<<"\033[0m\n"<<endl;
            }
        }

        //前进操作
        void stepForward(){
            if(!st_forward.empty()){
                string step=st_forward.top();
                st_forward.pop();

                if(step.size()==2)
                    //逆时针
                    rotateWCA(step[0],true,true);
                else{
                    //顺时针
                    rotateWCA(step[0],false,true);
                }

                st_back.push(step);
                //st.push(step);
                display();
            }else{
                cout<<"\033[31m"<<"No step forward"<<"\033[0m\n"<<endl;
            }
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

            cout<<"\n[*]v-cube  --made by "<<"\033[34m"<<"xz"<<"\033[0m]\n"<<"(github name:"<<"\033[34m"<<"666xz666"<<"\033[0m"<<")\n"<<endl;

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

            if(isRestored()) cout<<"\033[32m"<<"^v^ Restored\n"<<"\033[0m"<<endl;
            else             cout<<"\033[31m"<<"-_- Not Restored\n"<<"\033[0m"<<endl;

            cout<<"https://github.com/666xz666/333cube\n (Click to star)\n"<<endl;
            // std::this_thread::sleep_for(std::chrono::milliseconds(settings.move_delay));
        }

    


        //test code
        // stack<string> getSt(){
        //     return st;
        // }

        //end test
};


// int main() {
//     //test
//     Cube cube;

//     string type = "pll";
//     string caseName = "Aa";
//     vector<string>algList=cube.generateAlgdbFormulas(type,caseName);
//     for(auto alg:algList){
//         cout<<alg<<endl;
//     }
    
//     cube.move(algList[0]);

//     return 0;
//     //end test
// }