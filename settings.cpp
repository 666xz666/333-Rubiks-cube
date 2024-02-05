#include<unordered_map>
#include<vector>
#include<string>

using namespace std;

//魔方相关设置
class Settings{
    private:
        
    public:
        vector<char> colors;
        std::unordered_map<char, std::string> color_map;
        int move_delay;
        std::unordered_map<char, int> layerMap;
        Settings(){
                move_delay=300;
                layerMap = {{'U', 0}, {'L', 1}, {'F', 2}, {'R', 3}, {'B', 4}, {'D', 5}};
                //颜色转义字符
                color_map['W'] = "\033[37m";  // 白色
                color_map['O'] = "\033[33m";  // 橙色（黄色的ANSI转义码可能看起来像橙色）
                color_map['G'] = "\033[32m";  // 绿色
                color_map['R'] = "\033[31m";  // 红色
                color_map['B'] = "\033[34m";  // 蓝色
                color_map['Y'] = "\033[93m";  // 黄色
                //指定颜色
                colors.resize(6);
                //ULFRBD
                colors[0]='W',colors[1]='O',colors[2]='G',
                colors[3]='R',colors[4]='B',colors[5]='Y';
        }
        void setColors(vector<char>& _colors,std::unordered_map<char, std::string>& _color_map){
            _colors=colors;
            _color_map=color_map;
        }
};

//实例化魔方设置
Settings settings;