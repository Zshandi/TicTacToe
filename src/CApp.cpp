#include<windows.h>
#include "utility.h"
#include "CApp.h"
#undef main
using namespace std;

const int winCombos[8][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6}};

int CApp::OnExecute(){
    player = new HumanPlayer();

    if(OnInit() == false){
        return -1;
    }


    SDL_Event event;

    while(running){

        while(SDL_PollEvent(&event)){
            OnEvent(&event);
        }

        OnLoop();
        OnRender();

    }

    OnCleanup();

    return 0;
}

void CApp::newGame(){
    for(int i = 0; i < 9; i++){
        grid[i] = 0;
        history[i] = GRID_POS_NONE;
    }
    moves = 0;
    gameOver = false;

    turn = reversed+1;
    currentGridIndex = GRID_POS_NONE;
    previousGridIndex = GRID_POS_NONE;
    shouldWaitForEvent = false;

    render = true;
    //player->initGame(0,false);
    startNextTurn();
}

// Finishes up the current turn
void CApp::finishCurrentTurn(){
    grid[currentGridIndex] = turn;
    turn = switchTurn(turn);

    render = true;

    history[moves] = currentGridIndex;
    moves++;
    checkHistory();

    checkWin();
    if(!gameOver){
        startNextTurn();
    }
}

// Starts the next turn
void CApp::startNextTurn(){
    player->getNextMove( currentGridIndex, shouldWaitForEvent,
                        (moves==0 ? GRID_POS_NONE : history[moves-1]), utility::getGridIndexForMouse() );
    if( !shouldWaitForEvent ){
        if(grid[currentGridIndex] == 0){
            grid[currentGridIndex] = turn;
            turn = switchTurn(turn);

            render = true;

            history[moves] = currentGridIndex;
            moves++;
            checkHistory();
            if(currentGridIndex != previousGridIndex){
                render = true;
            }
        }else{
            shouldWaitForEvent = true;
        }
    }
}

void CApp::checkWin(){
    for(const int* combo : winCombos){
        if(grid[combo[0]] != 0 && grid[combo[0]] == grid[combo[1]] && grid[combo[1]] == grid[combo[2]]){
            int winner = grid[combo[0]];
            for(int i = 0; i < GRID_SIZE; i++){
                grid[i] = 0;
            }
            grid[combo[0]] = winner;
            grid[combo[1]] = winner;
            grid[combo[2]] = winner;
            gameOver = true;
        }
    }
    if(!gameOver){
        gameOver = true;
        for(int i = 0; i < 9; i++){
            if(grid[i] == 0){
                gameOver = false;
                break;
            }
        }
    }
}

int CApp::switchTurn(int turn){
    return !(turn-1)+1;
}

void CApp::undo(){
    if(moves == 0) return;
    moves--;
    if(history[moves] == -1) return;
    grid[history[moves]] = 0;
    turn = switchTurn(turn);

    render = true;
}

void CApp::redo(){
    if(history[moves] != -1){
        grid[history[moves]] = turn;
        moves++;
        turn = switchTurn(turn);

        render = true;
    }
}

void CApp::checkHistory(){
    for(int i = moves; i < 9; i++){
        history[i] = -1;
    }
}


bool CApp::OnInit(){
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        return false;
    }
    if((window = SDL_CreateWindow("Tic-Tac-Toe", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 600, SDL_WINDOW_SHOWN)) == nullptr){
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == nullptr){
        return false;
    }
    SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );

    SDL_Surface* temp = nullptr;
    if( (temp = SDL_LoadBMP("gfx/smiley.bmp")) ){
        SDL_SetWindowIcon(window, temp);
        SDL_FreeSurface(temp);
    }

    graphic_grid = Texture(renderer, "gfx/grid.bmp");
    graphic_x = Texture(renderer, "gfx/x.bmp", true, 0xFF, 0, 0xFF);
    graphic_o = Texture(renderer, "gfx/o.bmp", true, 0xFF, 0, 0xFF);

    if(!graphic_grid.loaded() || !graphic_x.loaded() || !graphic_o.loaded()){
        return false;
    }

    newGame();
    return true;
}

void CApp::OnEvent(SDL_Event* event){
    int i = 0;

    switch(event->type){
    case SDL_QUIT:
        running = false;
        break;
    case SDL_KEYDOWN:
        if(!gameOver){
            if(event->key.keysym.mod & KMOD_CTRL){
                //CTRL+Z and CTRL+Y undo and redo
                if(event->key.keysym.sym == SDLK_z){
                    if(moves == 1 && computer && reversed)
                        return;
                    undo();
                    if(computer)
                        undo();
                }else if(event->key.keysym.sym == SDLK_y){
                    redo();
                    if(computer)
                        redo();
                }
            }
        }
        //Turn computer player on/off
        if(event->key.keysym.sym == SDLK_F1){
            computer = !computer;
            newGame();
            return;
        }else if(event->key.keysym.sym == SDLK_F2){
            reversed = !reversed;
            newGame();
            return;
        }else if(event->key.keysym.sym == SDLK_F3){
            reversedGraphic = !reversedGraphic;
            render = true;
        }
        // Allow player to quit the game by closing the window
        else if(event->key.keysym.sym == SDLK_ESCAPE){
            newGame();
            return;
        }

    default:
        if( !shouldWaitForEvent ) break;
        player->getNextMoveOnEvent( currentGridIndex, shouldWaitForEvent,
                        (moves==0 ? GRID_POS_NONE : history[moves-1]), utility::getGridIndexForMouse(), event );
        if(currentGridIndex != previousGridIndex){
            render = true;
            previousGridIndex = currentGridIndex;
        }
        if( !shouldWaitForEvent ){
            if(grid[currentGridIndex] == 0){
                finishCurrentTurn();
            }else{
                shouldWaitForEvent = true;
            }
        }
    }
}

void CApp::OnLoop(){
    if(!gameOver){
        if(computer && turn == 2){
            int compMove = SimpleAI().getNextMove(grid);
            if(grid[compMove] == 0){
                currentGridIndex = compMove;
                finishCurrentTurn();
            }
        }
    }
}

void CApp::OnRender(){
    if(!render) return;

    SDL_RenderClear(renderer);

    graphic_grid.render();
    for(int i = 0; i < 9; i++){
        if(grid[i] == 0) continue;
        int x=0, y=0;
        utility::getScreenPosForGridIndex(i, x, y);
        if((grid[i] == 1) != reversedGraphic){
            graphic_x.render(x, y);
        }else{
            graphic_o.render(x, y);
        }
    }
    if(currentGridIndex != GRID_POS_NONE && grid[currentGridIndex] == 0 && !gameOver){
        int x=0, y=0;
        utility::getScreenPosForGridIndex(currentGridIndex, x, y);
        if((turn == 1) != reversedGraphic){
            graphic_x.setAlpha(255/2);
            graphic_x.render(x, y);
            graphic_x.setAlpha(255);
        }else{
            graphic_o.setAlpha(255/2);
            graphic_o.render(x, y);
            graphic_o.setAlpha(255);
        }
    }

    SDL_RenderPresent(renderer);

    render = false;
}

void CApp::OnCleanup(){
    graphic_grid.unload();
    graphic_x.unload();
    graphic_o.unload();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    renderer = nullptr;
    window = nullptr;

    SDL_Quit();
}

int main(){

    CApp theApp;
    return theApp.OnExecute();
}
