//g++ demo.cpp sources/icon.res -o demo.exe 修改exe图标

#include"cube.cpp"

#include<string>

using namespace std;

class Cubedemo{
    public:
        void printCommandList(){
            cout<<"\n[*]v-cube  --made by "<<"\033[34m"<<"xz"<<"\033[0m\n"<<"(github name:"<<"\033[34m"<<"666xz666"<<"\033[0m"<<")"<<endl;
            cout<<"https://github.com/666xz666/333cube/tree/v1.0.0 (Click to star)\n"<<endl;
            cout<<"\033[31m"<<"**commandlist**"<<"\033[0m\n"<<endl;
            cout<<"\"commandlist\""<<":"<<"Show commandlist\n"<<endl;
            cout<<"\"clear\""<<":"<<"Clear command line\n"<<endl;
            cout<<"\"move {formula}\""<<":"<<"Turn the cube according to the formula given"<<endl;
            cout<<" eg:\" move B2 U2 L2 D2 B' R2 B' U2 L2 F2 L2 R B' F D2 U' F' D L F2\"\n"<<endl;
            cout<<"\"scramble\""<<":"<<"Scramble the cube\n"<<endl;
            cout<<"\"restore\""<<":"<<"Restore the cube\n"<<endl;
            cout<<"\"generate scramble\""<<":"<<"Randomly generate scramble formula\n"<<endl;
            cout<<"\"generate reverseformula {formula}\""<<":"<<"Invert the formula given"<<endl;
            cout<<" eg:\"generate reverseformula B2 U2 L2 D2 B' R2 B' U2 L2 F2 L2 R B' F D2 U' F' D L F2\"\n"<<endl;
            cout<<"\"exit\""<<":"<<"Exit the program\n"<<endl;
        }
};

int main(){
    Cube cube;
    Cubedemo demo;
    string command;

    cout<<"Enter a command(Enter \"commandlist\" to see more)"<<endl;
    //程序主体
    while(true){
        cout<<"cube:>";
        cin>>command;

        if(command=="commandlist") 
            demo.printCommandList();
        else if(command=="exit"){
            cout<<"Program exit"<<endl;
            break;
        }
        else if(command=="clear"){
            cout<<"\033c";
            cube.display();
            cout<<"Enter a command(Enter \"commandlist\" to see more)"<<endl;
        }
        else if(command=="move"){
            string formula;
            std::cin >> std::ws;  // 吃掉一个空格
            getline(cin,formula);
            if(cube.isFormulaValid(formula)){
                cube.move(formula);
            }else{
                cout<<"\033[31m"<<"Invalid formula(Enter \"commandlist\" to see more)"<<"\033[0m\n"<<endl;
            }
        }
        else if(command=="scramble"){
            cube.scrambleCube();
        }
        else if(command=="restore"){
            cube.restoreCube();
        }
        else if(command=="generate"){
            string generate;
            std::cin >> std::ws;  // 吃掉一个空格
            cin>>generate;
            if(generate=="scramble"){
                cout<<cube.generateScramble()<<endl;
            }
            else if(generate=="reverseformula"){
                string formula;
                std::cin >> std::ws;  // 吃掉一个空格
                getline(cin,formula);
                if(cube.isFormulaValid(formula)){
                    cout<<cube.generateReverseFormula(formula)<<endl;
                }else{
                    cout<<"\033[31m"<<"Invalid formula(Enter \"commandlist\" to see more)"<<"\033[0m\n"<<endl;
                }
            }
            else cout<<"\033[31m"<<"invalid command(Enter \"commandlist\" to see more)"<<"\033[0m\n"<<endl;
        }
        else cout<<"\033[31m"<<"invalid command(Enter \"commandlist\" to see more)"<<"\033[0m\n"<<endl;
    }

    return 0;
}