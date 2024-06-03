#include <iostream>
#include "clui.h"
#include <chrono>
using namespace std;
using namespace chrono;

bool at_beginning_game, gameover, at_start, draw, timer, time_has_passed;
int menu, speed;
steady_clock::time_point start_time;
steady_clock::time_point end_time;

struct info{
    char name[50];
    int score;
}players[50], current_players[2], winner;
char input_name[50];

enum direciton{
    STOP, LEFT, RIGHT, UP, DOWN
}current_dir1, current_dir2;

const int width = 50;
const int height = 15;

int number_of_players;
int x1, y_1, x2, y2;
int savex1, savex2, savey1, savey2;
int fruit_x,fruit_y, bomb_x, bomb_y, profruit_x, profruit_y;
int tail_x1[600], tail_y1[600], tail_x2[600], tail_y2[600];
int tail_size1, tail_size2;

void swap(info& player1, info& player2){
    info temp = player1;
    player1 = player2;
    player2 = temp;
}

int number_length(int number){
    int result=0;
    if(number == 0)
    return 1;
    for(int i = 0 ; number > 0 ; i++){
        if(number > 0 && number < 10){
            result++;
            return result;
        }
        else{
            result++;
            number /= 10;
        }
    }
    return result;
}

void welcome(){
     int cols =  get_window_cols();
     int rows = get_window_rows();
     int middle = cols / 2;
    cursor_to_pos(0, 12);
    change_color_rgb(72, 58, 194);
    change_color_rgb(58, 40, 222);
    std::cout << "Hi!" << std:: endl;
    // cursor_to_pos(0, middle-10);
    // cursor_down(1);
    // save_cursor();
    change_color_rgb(72, 58, 194);
    change_color_rgb(58, 40, 222);
    std::cout << "Welcome to the ";
    change_color_rgb(46, 153, 135);
    //cursor_forward(16);
    //save_cursor();
    std:: cout << "SNAKE " ;
    change_color_rgb(72, 58, 194);
    change_color_rgb(58, 40, 222);
    //cursor_forward(6);
    std:: cout << "game!" << std:: endl << endl;
    reset_color();
}

void list(){
    change_color_rgb(35, 98, 186);
    change_color_rgb(51, 148, 204);
    std::cout << "Please choose one of the following options:" << endl;
    std::cout << "1)Start" << endl;
    std::cout << "2)Score's list" << endl;
    std::cout << "3)Enter username and change the player" << endl;
    std::cout << "4)Exit" << endl << endl;
    delay(1000);
    change_color_rgb(33, 169, 184);
    std::cout << "Enter the number of the chosen option: ";
    std::cin >> menu;

    if(menu == 4)
    quit();
}

void first_list_error(){
    //clear_screen();
    system("cls");
    if(at_beginning_game && menu==1){
            change_color_rgb(240, 26, 62);
            cout << "Sorry you haven't entered the players' names yet, first enter your name." << endl;
            list();
            if(menu == 3)
            at_beginning_game = false;
        }
        else if(at_beginning_game && menu==2){
            change_color_rgb(240, 26, 62);
            cout << "Sorry you haven't played yet! Please enter a valid option." << endl;
            list();
            if(menu == 3)
            at_beginning_game = false;
        }
        else if(menu<1 || menu > 4){
            change_color_rgb(240, 26, 62);
            cout << "Please enter a valid option!" << endl;
            list();
        }
}

bool check_names_eqaul(char* first, char* second){
    if(strlen(first) != strlen(second))
    return false;

    for(int i=0 ; i<strlen(first) ; i++){
        if(first[i] != second[i])
        return false;
    }
    
    return true;
}

int is_name_standard(char* str, int which_player){
    if(strlen(str) >= 31)
    return 2;
    
    if(which_player!=1){
        if(check_names_eqaul(str, current_players[0].name))
        return 3;
    }

    for(int i=0 ; i<number_of_players ; i++){
        if(check_names_eqaul(str, players[i].name))
        return 4;
    }

    return 1;
}

void get_information(){
    int check_repeat = -1;

    change_color_rgb(135, 28, 76);
    std:: cout << "I will ask you to choose which player do you want to be" << endl;
    std:: cout << "The ";
    change_color_rgb(79, 191, 227);
    std:: cout << "first player ";
    change_color_rgb(135, 28, 76);
    std:: cout << "will play with w, a, s and d keys." << endl;
    std:: cout << "The ";
    change_color_rgb(71, 71, 186);
    std:: cout << "second player ";
    change_color_rgb(135, 28, 76);
    std:: cout << "will play with directions keys on keyboard." << endl << endl;
    reset_color(); 
    delay(500);

    change_color_rgb(79, 191, 227);
    std::cout << "First player:" << endl;
    std::cout << "Please enter the name(with less than 30 characters): ";
    change_color_rgb(135, 28, 76);
    std::cin.ignore();
    std::cin.getline(input_name, 50);
    int check_standard_name = is_name_standard(input_name, 1);
    while(check_standard_name != 1){
        if(check_standard_name == 2){
            change_color_rgb(240, 26, 62);
            std::cout << "The name you chose is too long, please try a shorter one: ";
            change_color_rgb(135, 28, 76);
            std:: cin.getline(input_name, 50);
            check_standard_name = is_name_standard(input_name, 1);
            continue;
        }
        if(check_standard_name == 3){
            change_color_rgb(240, 26, 62);
            cout << "Sorry but you can't play against yourself! Please enter the name again" << endl;
            change_color_rgb(135, 28, 76);
            cin.getline(input_name, 50);
            check_standard_name = is_name_standard(input_name, 1);
            continue;
        }
        if(check_standard_name == 4){
            change_color_rgb(240, 26, 62);
            cout << "The name you chose, has been already chosen." << endl <<  "if you want to play with this name and use it's previous data enter y key;" << endl << "otherwise, enter any other keys to reenter the user name" << endl;
            change_color_rgb(135, 28, 76);
            check_repeat = getch();
            if(check_repeat == int('y') || check_repeat == int('Y')){
                break;
            }
            else{
                cin.getline(input_name, 50);
                check_standard_name = is_name_standard(input_name, 1);
            }
        }
    }
    
    if(at_beginning_game){
        strcpy(players[0].name, input_name);
        number_of_players++;
    }
    else if(check_repeat != 'y' && check_repeat != 'Y')
    strcpy(players[number_of_players++].name, input_name);

    strcpy(current_players[0].name, input_name);
    std::cout << endl;


    check_repeat = -1;
    change_color_rgb(71, 71, 186);
    std::cout << "Second player:" << endl;
    std::cout << "Please enter the name(with less than 30 characters): ";
    change_color_rgb(135, 28, 76);
    std::cin.getline(input_name, 32);
    
    check_standard_name = is_name_standard(input_name, 2);
        while(check_standard_name != 1){
            if(check_standard_name == 2){
                change_color_rgb(240, 26, 62);
                std::cout << "The name you chose is too long, please try a shorter one: ";
                change_color_rgb(135, 28, 76);
                std:: cin.getline(input_name, 32);
                check_standard_name = is_name_standard(input_name, 2);
                continue;
            }
            if(check_standard_name == 3){
                change_color_rgb(240, 26, 62);
                cout << "Sorry but you can't play against yourself! Please enter the name again" << endl;
                change_color_rgb(135, 28, 76);
                cin.getline(input_name, 32);
                check_standard_name = is_name_standard(input_name, 2);
                continue;
            }
            if(check_standard_name == 4){
                change_color_rgb(240, 26, 62);
                cout << "The name you chose, has been already chosen." << endl <<  "if you want to play with this name and use it's previous data enter y key;" << endl << "otherwise, enter any other keys to reenter the user name" << endl;
                change_color_rgb(135, 28, 76);
                check_repeat = getch();
                if(check_repeat == int('y') || check_repeat == int('Y')){
                    break;
                }
                else{
                    cin.getline(input_name, 32);
                    check_standard_name = is_name_standard(input_name, 2);
                }
            }
        }
    if(check_repeat != int('y') && check_repeat != int('Y'))
    strcpy(players[number_of_players++].name, input_name);

    strcpy(current_players[1].name, input_name);
    reset_color();
    //clear_screen();
    system("cls");
}

void set_very_first_data(){
    at_beginning_game = true;
    number_of_players = 0;
}

bool check_location(int fruit_or_bomb){

    if(fruit_or_bomb == 1){
        if((fruit_x == x1 && fruit_y == y_1) || (fruit_x == x2 && fruit_y == y2))
        return false;

        for(int i=0 ; i<tail_size1 ; i++){
            if(fruit_x == tail_x1[i] && fruit_y == tail_y1[i])
            return false;
        }
        for(int i=0 ; i<tail_size2 ; i++){
            if(fruit_x == tail_x2[i] && fruit_y == tail_y2[i])
            return false;
        }
    }
    else if(fruit_or_bomb == 2){
        if((bomb_x == x1 && bomb_y == y_1) || (bomb_x == x2 && bomb_y == y2))
        return false;

        for(int i=0 ; i<tail_size1 ; i++){
            if(bomb_x == tail_x1[i] && bomb_y == tail_y1[i])
            return false;
        }

        for(int i=0 ; i<tail_size2 ; i++){
            if(bomb_x == tail_x2[i] && bomb_y == tail_y2[i])
            return false;
        }

        if(bomb_x == fruit_x && bomb_y == fruit_y)
        return false;
    }
    else if(fruit_or_bomb == 3){
        if((fruit_x == x1 && fruit_y == y_1) || (fruit_x == x2 && fruit_y == y2))
        return false;

        for(int i=0 ; i<tail_size1 ; i++){
            if(fruit_x == tail_x1[i] && fruit_y == tail_y1[i])
            return false;
        }
        for(int i=0 ; i<tail_size2 ; i++){
            if(fruit_x == tail_x2[i] && fruit_y == tail_y2[i])
            return false;
        }

        if(bomb_x == fruit_x && bomb_y == fruit_y)
        return false;
        
    }
    else if(fruit_or_bomb == 4){
        if((profruit_x == x1 && profruit_y==y_1) || (profruit_x == x2) && (profruit_y == y2))
        return false;

        for(int i=0 ; i<tail_size1 ; i++){
            if(profruit_x == tail_x1[i] && profruit_y == tail_y1[i])
            return false;
        }
        for(int i=0 ; i<tail_size2 ; i++){
            if(profruit_x == tail_x2[i] && profruit_y == tail_y2[i])
            return false;
        }

        if(profruit_x == fruit_x && profruit_y == fruit_y)
        return false;

        if(bomb_x == profruit_x && bomb_y == profruit_y)
        return false;
    }
    return true;
}

void set_first_data(){
    gameover = false;
    at_start = true;
    timer = false;
    time_has_passed = false;
    draw = false;
    tail_size1 = 3;
    tail_size2 = 3;
    winner.score=0;
    current_dir1 = STOP;
    current_dir2 = STOP;
    current_players[0].score = 0;
    current_players[1].score = 0;
    x1 = (width/2)-3;
    y_1 = height/2;
    for(int i=0 ; i<3 ; i++){
        tail_x1[i] = x1;
        tail_y1[i] = y_1 + (i+1);

    }
    x2 = (width/2)+3;
    y2 = height/2;
    for(int i=0 ; i<3 ; i++){
        tail_x2[i] = x2;
        tail_y2[i] = y2 + (i+1);

    }
    srand(time(0));
    fruit_x = rand()%(width-2);
    fruit_y = rand()%(height-2);
    bool fruit_check = check_location(1);
    while(fruit_check == false){
        fruit_x = (rand()%(width-2))+1;
        fruit_y = rand()%(height-2)+1;
        fruit_check = check_location(1);
    }
    bomb_x = (rand()%(width-2))+1;
    bomb_y = (rand()%(height-2))+1;
    bool bomb_check = check_location(2);
    while(bomb_check == false){
        bomb_x = (rand()%(width-2))+1;
        bomb_y = (rand()%(height-2))+1;
        bomb_check = check_location(2);
    }
    profruit_x = (rand()%(width-2))+1;
    profruit_y = (rand()%(height-2))+1;
    fruit_check = check_location(4);
    while(fruit_check == false){
        profruit_x = (rand()%(width-2))+1;
        profruit_y = (rand()%(height-2))+1;
        fruit_check = check_location(4);
    }

}

void show(){

    reset_color();
    //clear_screen();
    system("cls");
    //change_color_rgb(135, 28, 76);
    for(int i=0 ; i<width+1 ; i++){
        cout << "#";
    }
    cout << endl;


    for(int i=0 ; i<height ; i++){
        for(int j=0; j<width ; j++){
            //reset_color();
            //change_color_rgb(135, 28, 76);
            if(j==0 || j==width-1){
                cout << "#";
            }

            if(i == y_1 && j == x1){
                //change_color_rgb(79, 191, 227);
                change_color_rgb(79, 191, 227);
                cout << "0";
                reset_color();
                continue;
            }
            else if(i == y2 && j==x2){
                //change_color_rgb(71, 71, 186);
                change_color_rgb(71, 71, 186);
                cout << "*";
                reset_color();
                continue;
            }
            else if (i == fruit_y && j == fruit_x){
                //change_color_rgb(24, 237, 91);
                cout << char(1);
                //reset_color();
                continue;
            }
            else if(i==profruit_y && j==profruit_x){
                //change_color_rgb(24, 237, 91);
                cout << char(2);
                //reset_color();
                continue;
            }
            else if(i == bomb_y && j == bomb_x){
                //change_color_rgb(240, 26, 62);
                cout << char(4);
               // reset_color();
                continue;
            }
            else
            {
                bool print1 = false, print2 = false;
                for (int k = 0; k<tail_size1 ; k++){
                    if (tail_x1[k] == j && tail_y1[k] == i){
                        //change_color_rgb(79, 191, 227);
                        cout << "0";
                        print1 = true;
                        //reset_color();
                    }
                }
                for(int k=0 ; k<tail_size2 ; k++){
                    if (tail_x2[k] == j && tail_y2[k] == i){
                       // change_color_rgb(71, 71, 186);
                        cout << "*";
                        print2 = true;
                        //reset_color();
                    }
                }

                if (!(print1 || print2))
                cout << ' ';
            }  
        }
        cout << endl;
    }
    reset_color();
    //change_color_rgb(135, 28, 76);
    for(int i=0 ; i<width+1 ; i++){
        cout << "#";
    }
    cout << endl;
    change_color_rgb(79, 191, 227);
    cout << current_players[0].name << "'s " << "score: " << current_players[0].score << endl;
    change_color_rgb(71, 71, 186);
    cout << current_players[1].name << "'s " << "score: " << current_players[1].score;
}

void input(){
    int key, check, prev_dir1, prev_dir2;
    if(is_keyboard_hit()){
        key = nb_getch();
        if(key=='w' || key=='W' || key=='a' || key=='A' || key=='s' || key=='S' || key=='d' || key=='D'){
            switch(key){
                case int('w'):
                case int('W'): if(current_dir1!=DOWN){current_dir1 = UP;} break;
                case int('s'):
                case int('S'): if(current_dir1!=UP && (current_dir1!=STOP)){current_dir1 = DOWN;} break;
                case int('a'):
                case int('A'): if(current_dir1!=RIGHT){current_dir1 = LEFT;} break;
                case int('d'):
                case int('D'): if(current_dir1!=LEFT){current_dir1 = RIGHT;} break;
                default: break;
            }
            check = 1;
        }
        else if(int(key)==224){
            key = _getch();
            switch(key){
                case 72: if(current_dir2 != DOWN) current_dir2=UP; break;
                case 80: if(current_dir2 != UP && current_dir2 != STOP) current_dir2=DOWN; break;
                case 75: if(current_dir2 != RIGHT) current_dir2=LEFT; break;
                case 77: if(current_dir2 != LEFT) current_dir2=RIGHT; break;
            }
            check = 2;
        }
    }

    if(is_keyboard_hit()){
        key = nb_getch();
        if(check!=1){
        if(key=='w' || key=='W' || key=='a' || key=='A' || key=='s' || key=='S' || key=='d' || key=='D'){
            switch(key){
                case int('w'):
                case int('W'): if(current_dir1!=DOWN){current_dir1 = UP;} break;
                case int('s'):
                case int('S'): if(current_dir1!=UP && (current_dir1!=STOP)){current_dir1 = DOWN;} break;
                case int('a'):
                case int('A'): if(current_dir1!=RIGHT){current_dir1 = LEFT;} break;
                case int('d'):
                case int('D'): if(current_dir1!=LEFT){current_dir1 = RIGHT;} break;
                default: break;
            }
        }}
        else if(check!=2){
            if (int(key)==224){
            key = _getch();
            switch(key){
                case 72: if(current_dir2 != DOWN) current_dir2=UP; break;
                case 80: if(current_dir2 != UP && current_dir2 != STOP) current_dir2=DOWN; break;
                case 75: if(current_dir2 != RIGHT) current_dir2=LEFT; break;
                case 77: if(current_dir2 != LEFT) current_dir2=RIGHT; break;
            }
        }}
    }
}

void draw_check(){

    bool snake1_hit_self=false, snake2_hit_self=false;
    bool snake1_hit_border = false, snake2_hit_border = false;
    bool snake1_hit_snake2 = false, snake2_hit_snake1 = false;
    bool snake1_hit_bomb = false, snake2_hit_bomb = false;
    
    if(x1 == bomb_x && y_1 == bomb_y)
    snake1_hit_bomb = true;
    if(x2 == bomb_x && y2 == bomb_y)
    snake2_hit_bomb = true;

    if(x1 == x2 && y_1 == y2){
        if(current_dir1 != STOP && current_dir2 != STOP)
        draw = true;
        gameover = true;
        return;
    }

    if(x1<0 || x1>=width-1 || y_1<0 || y_1>height-1)
    snake1_hit_border = true;
    if(x2<0 || x2>=width-1 || y2<0 || y2>height-1)
    snake2_hit_border = true;

    if(snake1_hit_border && snake2_hit_border){
        draw = true;
        gameover = true;
        return;
    }


    for(int i=0 ; i<tail_size1 ; i++){
        if(tail_x1[i] == x1 && tail_y1[i] == y_1){
            snake1_hit_self = true;
            break;
        }
    }
    for(int i=0 ; i<tail_size2 ; i++){
        if(tail_x2[i] == x2 && tail_y2[i] == y2){
            snake2_hit_self = true;
            break;
        }
    }
    if(snake1_hit_self && snake2_hit_self){
        draw = true;
        gameover = true;
        return;
    }

    for(int i=0 ; i<tail_size2 ; i++){
        if(tail_x2[i] == x1 && tail_y2[i] == y_1)
        snake1_hit_snake2 = true;
    }
    for(int i=0 ; i<tail_size1 ; i++){
        if(tail_x1[i] == x2 && tail_y1[i] == y2)
        snake2_hit_snake1 = true;
    }
    if((snake1_hit_border && snake2_hit_snake1) || (snake2_hit_border && snake1_hit_snake2)){
        draw = true;
        gameover = true;
        return;
    }


    if((snake1_hit_bomb && snake2_hit_border) || (snake2_hit_bomb && snake1_hit_border)){
        draw = true;
        gameover = true;
        return;
    }
    
    if((snake1_hit_self && snake2_hit_border) || (snake2_hit_self && snake1_hit_border)){
        draw = true;
        gameover = true;
        return;
    }

    if((snake1_hit_bomb && snake2_hit_self) || (snake2_hit_bomb && snake1_hit_self)){
        draw = true;
        gameover = true;
        return;
    }

    if((snake1_hit_bomb && snake2_hit_snake1) || (snake2_hit_bomb && snake1_hit_snake2)){
        draw = true;
        return;
    }

    draw = false;
}

void algorithm(){

    if(current_dir1 != STOP){
    for(int i=tail_size1-1 ; i>=0 ; i--){
        if(i==tail_size1-1){
            savex1=tail_x1[i];
            savey1=tail_y1[i];
        }
        if(i==0){
            tail_x1[i] = x1;
            tail_y1[i] = y_1;
        }
        else{
            tail_x1[i] = tail_x1[i-1];
            tail_y1[i] = tail_y1[i-1];
        }
    }}
    if(current_dir2 != STOP){
    for(int i=tail_size2-1 ; i>=0 ; i--){
        if(i==tail_size2-1){
            savex2=tail_x2[i];
            savey2=tail_y2[i];
        }
        if(i==0){
            tail_x2[i] = x2;
            tail_y2[i] = y2;
        }
        else{
            tail_x2[i] = tail_x2[i-1];
            tail_y2[i] = tail_y2[i-1];
        }
    }}

    switch(current_dir1){
        case LEFT: x1--; break;
        case RIGHT: x1++; break;
        case UP: y_1--; break;
        case DOWN: y_1++; break;
        default: break;
    }
    switch(current_dir2){
        case LEFT: x2--; break;
        case RIGHT: x2++; break;
        case UP: y2--; break;
        case DOWN: y2++; break;
        default: break;
    }

    draw_check();

    if(!draw){

        if(x1 < 0 || y_1 < 0 || x1 >= width-1 || y_1 > height-1){
            strcpy(winner.name, current_players[1].name);
            winner.score = current_players[1].score;
            current_players[0].score = 0;
            gameover = true;
        }
        else if(x2 < 0 || y2 < 0 || x2 >= width-1 || y2 > height-1){
            strcpy(winner.name, current_players[0].name);
            winner.score = current_players[0].score;
            current_players[1].score = 0;
            gameover = true;
        }
        
        for(int i=0 ; i<tail_size1 ; i++){
            if(tail_x1[i] == x1 && tail_y1[i] == y_1){
                strcpy(winner.name, current_players[1].name);
                winner.score = current_players[1].score;
                current_players[0].score = 0;
                gameover = true;
                break;
            }
        }
        for(int i=0 ; i<tail_size2 ; i++){
            if(tail_x2[i] == x2 && tail_y2[i] == y2){
                strcpy(winner.name, current_players[0].name);
                winner.score = current_players[0].score;
                current_players[1].score = 0;
                gameover = true;
                break;
            }
        }

        if(x1 == x2 && y_1 == y2){
            if(current_dir1 == STOP){
                strcpy(winner.name, current_players[0].name);
                winner.score = current_players[0].score;
                current_players[1].score = 0;
                gameover = true;
            }
            else{
                strcpy(winner.name, current_players[1].name);
                winner.score = current_players[1].score;
                current_players[0].score = 0;
                gameover = true;
            }
        }
        
        for(int i=0 ; i<tail_size2 ; i++){
            if(tail_x2[i] == x1 && tail_y2[i] == y_1){
                strcpy(winner.name, current_players[1].name);
                winner.score = current_players[1].score;
                current_players[0].score = 0;
                gameover = true;
                break;
            }
        }
        for(int i=0 ; i<tail_size1 ; i++){
            if(tail_x1[i] == x2 && tail_y1[i] == y2){
                strcpy(winner.name, current_players[0].name);
                winner.score = current_players[0].score;
                current_players[1].score = 0;
                gameover = true;
            }
        }


        if((x1 == bomb_x && y_1 == bomb_y)){
            strcpy(winner.name, current_players[1].name);
            winner.score = current_players[1].score;
            current_players[0].score = 0;
            gameover = true;
        }

        if((x2 == bomb_x && y2 == bomb_y)){
            strcpy(winner.name, current_players[0].name);
            winner.score = current_players[0].score;
            current_players[1].score = 0;
            gameover = true;
        }

        bool check_fruit;
        if(x1 == fruit_x && y_1 == fruit_y){
            ++tail_size1;
            tail_x1[tail_size1-1] = savex1;
            tail_y1[tail_size1-1] = savey1;
            current_players[0].score += 5;
            fruit_x = (rand()%(width-2))+1;
            fruit_y = (rand() % (height-2))+1;
            check_fruit = check_location(3);
            while(check_fruit == false){
                fruit_x = (rand()%(width-2))+1;
                fruit_y = (rand() % (height-2))+1;
                check_fruit = check_location(3);
            }
        }
        else if(x2 == fruit_x && y2 == fruit_y){
            ++tail_size2;
            tail_x2[tail_size2-1] = savex2;
            tail_y2[tail_size2-1] = savey2;
            current_players[1].score += 5;
            fruit_x = (rand()%(width-2))+1;
            fruit_y = (rand() % (height-2))+1;
            check_fruit = check_location(3);
            while(check_fruit == false){
                fruit_x = (rand()%(width-2))+1;
                fruit_y = (rand() % (height-2))+1;
                check_fruit = check_location(3);
            }
        }
        if(x1==profruit_x && y_1==profruit_y){
            ++tail_size1;
            tail_x1[tail_size1-1] = savex1;
            tail_y1[tail_size1-1] = savey1;
            current_players[0].score += 10;
            profruit_x = (rand()%(width-2))+1;
            profruit_y = (rand()%(height-1))+1;
            check_fruit = check_location(4);
            while(check_fruit == false){
                profruit_x = (rand()%(width-2))+1;
                profruit_y = (rand()%(height-1))+1;
                check_fruit = check_location(4);
            }
        }
        else if(x2==profruit_x && y2==profruit_y){
            ++tail_size2;
            tail_x2[tail_size2-1] = savex2;
            tail_y2[tail_size2-1] = savey2;
            current_players[1].score += 10;
            profruit_x = (rand()%(width-2))+1;
            profruit_y = (rand()%(height-1))+1;
            check_fruit = check_location(4);
            while(check_fruit == false){
                profruit_x = (rand()%(width-2))+1;
                profruit_y = (rand()%(height-1))+1;
                check_fruit = check_location(4);
            }
        }
    }
}

void finalize_data(){
    if(!draw){
        for(int i=0 ; i<number_of_players ; i++){
            if(check_names_eqaul(current_players[0].name, players[i].name)){
                players[i].score += current_players[0].score;
                continue;
            }
            if(check_names_eqaul(current_players[1].name, players[i].name)){
                players[i].score += current_players[1].score;
                continue;
            }
        }
    }
}

void sort(){
    for(int i=0 ; i<number_of_players-1 ; i++){
        for(int j=0 ; j<number_of_players-1-i ; j++){
            if(players[j].score < players[j+1].score){
                swap(players[j], players[j+1]);
            }
        }
    }
}

void show_league(){
    //clear_screen();
    system("cls");
    sort();
    for(int i=0 ; i<40 ; i++)
    cout << '_';
    cout << endl;
    for(int i=0 ; i<40 ; i++){
        if(i==0)
        cout << "|";
        else if(i==2){
            cout << "name";
            i+=(strlen("name")-1);
        }
        else if(i==25)
        cout << "|";
        else if(i==30){
            cout << "score";
            i+=(strlen("score")-1);
        }
        else if(i==39)
        cout << "|";
        else
        cout << " ";
    }
    cout << endl;
    for(int i=0 ; i<40 ; i++)
    cout << '_';
    cout << endl;
    for(int i=0, k=0 ; i<number_of_players ; i++){
        for(int j=0 ; j<40 ; j++){
            if(j==0 || j==24)
            cout << "|";
            if(j==2){
                cout << players[i].name;
                j+=(strlen(players[i].name)-1);
            }
            else if(j==27){
                cout << players[i].score;
                j+=(number_length(players[i].score)-1);
            }
            else if(j==37){
                cout << "|";
                cout << endl;
                for(int i=0 ; i<40 ; i++)
                cout << '_';
            }
            else
            cout << " ";
        }
        cout << endl;
    }

    cout << "enter any keys to comeback";
    int comeback = getch();
}

void ask_time(){

    //clear_screen();
    system("cls");

    change_color_rgb(240, 79, 67);
    cout << "1) Fast" << endl;
    cout << "2) Fast with 30 seconds timer" << endl;
    change_color_rgb(240, 168, 67);
    cout << "3) Normal" << endl;
    cout << "4) Normal with 30 seconds timer" << endl;
    change_color_rgb(107, 163, 16);
    cout << "5) Slow" << endl;
    cout << "6) Slow with 30 seconds timer" << endl;
    change_color_rgb(11, 76, 161);
    cin >> speed;
    while(speed != 1 && speed != 2 && speed != 3 && speed!=4 && speed!=5 && speed!=6){
        change_color_rgb(227, 34, 76);
        cout << "ERROR! the option is invalid" << endl;
        delay(3000);
        //clear_screen();
        system("cls");
        change_color_rgb(240, 79, 67);
        cout << "1) Fast" << endl;
        cout << "2) Fast with 30 seconds timer" << endl;
        change_color_rgb(240, 168, 67);
        cout << "3) Normal" << endl;
        cout << "4) Normal with 30 seconds timer" << endl;
        change_color_rgb(107, 163, 16);
        cout << "5) Slow" << endl;
        cout << "6) Slow with 30 seconds timer" << endl;
        change_color_rgb(11, 76, 161);
        cin >> speed;
    }
    reset_color();
}

bool who_is_winner(){

    if(current_players[0].score > current_players[1].score){
        strcpy(winner.name, current_players[0].name);
        winner.score = current_players[0].score;
        return true;
    }
    else if(current_players[1].score > current_players[0].score){
        strcpy(winner.name, current_players[1].name);
        winner.score = current_players[1].score;
        return true;
    }
    else
    return false;
}

int main(){

    init_clui();
    int restart;
    set_very_first_data();
    set_first_data();
    welcome();
    delay(1000);
    list();
    while((at_beginning_game && (menu==1 || menu==2)) || menu == 2 || menu < 1 || menu > 4){
        //clear_screen();
        system("cls");
        first_list_error();
    }
    while(menu != 1){
        if(menu == 3){
            //clear_screen();
            system("cls");
            get_information();
            list();
            continue;
        }
        if(menu == 2){
            //clear_screen();
            system("cls");
            change_color_rgb(240, 26, 62);
            cout << "Sorry you haven't played yet! Please enter a valid option." << endl;
            list();
        }
        else{
            //clear_screen();
            system("cls");
            change_color_rgb(240, 26, 62);
            cout << "Please enter a valid option!" << endl;
            list();
        }
    }
    at_beginning_game = false;

    while(!gameover){
        if(at_start){
            ask_time();
            if(speed == 2 || speed == 4 || speed == 6){
                start_time = steady_clock::now();
            }
            at_start = false;
        }
        show();
        input();
        algorithm();

        if(speed == 2 || speed == 4 || speed == 6){
            end_time = steady_clock::now();
            duration<double> elapsed = end_time - start_time;
            if (elapsed.count() > 30){
                gameover = true;
                time_has_passed = true;
            }
        }
        switch(speed){
            case 1:
            case 2: Sleep(10);break;
            case 3:
            case 4: Sleep(50); break;
            case 5:
            case 6: Sleep(100); break;
        }
        if(gameover){
            finalize_data();
            delay(3000);
            //clear_screen();
            system("cls");
            if((speed==1||speed==3||speed==5) && draw){
                change_color_rgb(63, 143, 119);
                cout << "The Game Ended In Draw, Both Players Earned Zero Score" << endl;
            }
            else if(speed == 1 || speed==3 || speed==5){
                change_color_rgb(224, 192, 117);
                cout << "The Winner Is: " << winner.name << "! And earned: " << winner.score << endl;
                change_color_rgb(79, 10, 87);
                cout << "And The Loser Earned Nothing!" << endl;
            }
            else if((speed == 2 || speed == 4 || speed == 6) && time_has_passed){
                bool have_winner = who_is_winner();
                if(!have_winner){
                    change_color_rgb(204, 118, 57);
                    cout << "The Game Ended In Draw, Both Players Earned: " << current_players[0].score << endl; 
                }
                else{
                    change_color_rgb(224, 192, 117);
                    cout << "The Winner Is: " << winner.name << "! And Earned: " << winner.score << endl;
                }
            }
            else if((speed == 2 || speed == 4 || speed == 6) && draw){
                change_color_rgb(63, 143, 119);
                cout << "The Game Ended In Draw, Both Players Earned Zero Score" << endl;
            }
            else{
                change_color_rgb(224, 192, 117);
                cout << "The Winner Is: " << winner.name << "! And earned: " << winner.score << endl;
                change_color_rgb(79, 10, 87);
                cout << "And Loser Earned Nothing!" << endl;
            }
            change_color_rgb(52, 152, 235);
            cout << "1) Show the main list" << endl;
            cout << "2) exit" << endl;
            cin >> restart;
            reset_color();
            if(restart == 1){
                //clear_screen();
                system("cls");
                list();
                while(menu != 1){
                    if(menu == 2){
                        show_league();
                        //clear_screen();
                        system("cls");
                        list();
                    }
                    else if(menu == 3){
                        //clear_screen();
                        system("cls");
                        get_information();
                        system("cls");
                        //clear_screen();
                        list();
                    }
                    else{
                        cout << "Please enter a valid option" << endl;
                        list();
                    }
                }
                set_first_data();
            }
        }
    }
}