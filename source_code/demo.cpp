#include"cube.cpp"

#include<string>
#include<fstream>

using namespace std;

class Cubedemo{
    public:
        
        //在桌面生成readme.md,内容为commandlist
        void writeCommandListToFile() {
            std::string commandList = "\n# v-cube\n"
                                    "made by xz\n"
                                    "(github name: 666xz666)\n\n"
                                    "## Command List\n\n"
                                    "- `commandlist`: Show command list\n"
                                    "- `clear`: Clear command line\n"
                                    "- `move {formula}`: Turn the cube according to the formula given\n"
                                    "  - eg: `move B2 U2 L2 D2 B' R2 B' U2 L2 F2 L2 R B' F D2 U' F' D L F2`\n"
                                    "- `scramble`: Scramble the cube\n"
                                    "- `reset`: Reset the cube\n"
                                    "- `generate {function}`: Generate a formula on request\n"
                                    "  - `generate scramble`: Randomly generate scramble formula\n"
                                    "  - `generate reverseformula {formula}`: Invert the formula given\n"
                                    "    - eg: `generate reverseformula B2 U2 L2 D2 B' R2 B' U2 L2 F2 L2 R B' F D2 U' F' D L F2`\n"
                                    "- `restoreby {function}`: Restore by a certain function\n"
                                    "  - `restoreby reverse`: Restore in reverse order\n"
                                    "- `step back`: Revoke last step\n"
                                    "- `step forward`: Restore the recently revoked step\n"
                                    "- `exit`: Exit the program\n\n"
                                    "Click to star\n\n";

            std::ofstream file;
            file.open("D:/Desktop/readme.md", std::ios::out); // Open the file in write mode. It will create the file if it does not exist.
            if (file.is_open()) {
                file << commandList; // Write the command list to the file.
                file.close(); // Close the file after writing.
            } else {
                std::cout << "Unable to open file";
            }
        }

        void printCommandList() {
            std::cout << "\n[*]v-cube  --made by \033[34m xz \033[0m\n"
                    << "(github name: \033[34m 666xz666 \033[0m)\n\n";

            std::cout << "\033[31m **commandlist** \033[0m\n\n";

            std::cout << "\"commandlist\": Show commandlist\n\n"
                    << "\"clear\": Clear command line\n\n"
                    << "\"move {formula}\": Turn the cube according to the formula given\n"
                    << " eg: \"move B2 U2 L2 D2 B' R2 B' U2 L2 F2 L2 R B' F D2 U' F' D L F2\"\n\n"
                    << "\"scramble\": Scramble the cube\n\n"
                    << "\"reset\": Reset the cube\n\n"
                    << "\"generate {function}\": Generate a formula on request\n"
                    << "\t\"generate scramble\": Randomly generate scramble formula\n"
                    << "\t\"generate reverseformula {formula}\": Invert the formula given\n"
                    << "\t eg: \"generate reverseformula B2 U2 L2 D2 B' R2 B' U2 L2 F2 L2 R B' F D2 U' F' D L F2\"\n\n"
                    << "\"restoreby {function}\": Restore by a certain function\n"
                    << "\t\"restoreby reverse\": Restore in reverse order\n\n"
                    << "\"step back\": Revoke last step\n\n"
                    << "\"step forward\": Restore the recently revoked step\n\n"
                    << "\"exit\": Exit the program\n\n"
                    << "https://github.com/666xz666/333cube\n (Click to star)\n\n";
        }

        //程序主体
        void mainProgram(){
            Cube cube;
            string command;
            //writeCommandListToFile();

            //test code
            //cout<<string(__FILE__);
            //end test

            cout<<"Enter a command(Enter \"commandlist\" to see more)"<<endl;
            while(true){
                

                //test code
                
                //end test

                cout<<"cube:>";
                cin>>command;

                if(command=="commandlist") 
                    printCommandList();
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
                else if(command=="reset"){
                    cube.resetCube();
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
                else if(command=="step"){
                    string direction;
                    std::cin >> std::ws;  // 吃掉一个空格
                    getline(cin,direction);
                    if(direction=="back"){
                        cube.stepBack();
                    }
                    else if(direction=="forward"){
                        cube.stepForward();
                    }
                    else{
                        cout<<"\033[31m"<<"invalid command(Enter \"commandlist\" to see more)"<<"\033[0m\n"<<endl;
                    }
                }
                else if(command=="restoreby"){
                    string function;
                    std::cin >> std::ws;  // 吃掉一个空格
                    getline(cin,function);
                    if(function=="reverse"){
                        cube.restoreByReverse();
                    }
                    else{
                        cout<<"\033[31m"<<"invalid command(Enter \"commandlist\" to see more)"<<"\033[0m\n"<<endl;
                    }
                }
                else cout<<"\033[31m"<<"invalid command(Enter \"commandlist\" to see more)"<<"\033[0m\n"<<endl;
            }
        }
};

int main(){
    Cubedemo demo;
    demo.mainProgram();
    return 0;
}