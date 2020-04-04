#include<windows.h>
#include "utility.h"
#include "RandomComputerPlayer.h"
#include "CApp.h"
#undef main
using namespace std;

const int winCombos[8][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6}};

int CApp::OnExecute(){

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
        grid[i] = GRID_VAL_NONE;
    }
    history.clear();
    gameOver = false;

    currentPlayer = 0;

    currentGridIndex = GRID_POS_NONE;
    previousGridIndex = GRID_POS_NONE;
    shouldWaitForEvent = false;

    render = true;
    playerData[0]->player->initGame(playerData[1]->player,false);
    playerData[1]->player->initGame(playerData[0]->player,true);
    startNextTurn();
}

// Finishes up the current turn
void CApp::finishCurrentTurn(){
    grid[currentGridIndex] = currentPlayer;

    currentPlayer = !currentPlayer;

    render = true;

    history.push_back(currentGridIndex);

    checkWin();
    if(!gameOver){
        startNextTurn();
    }
}

// Starts the next turn
void CApp::startNextTurn(){
    currentGridIndex = GRID_POS_NONE;
    shouldWaitForEvent = false;

    playerData[currentPlayer]->player->getNextMove( currentGridIndex, shouldWaitForEvent,
                        history.empty() ? GRID_POS_NONE:history.back(), utility::getGridIndexForMouse() );

    if( !shouldWaitForEvent ){
        if(grid[currentGridIndex] == GRID_VAL_NONE){
            finishCurrentTurn();
        }else{
            shouldWaitForEvent = true;
        }
    }
}

void CApp::checkWin(){
    for(const int* combo : winCombos){
        if(grid[combo[0]] != GRID_VAL_NONE && grid[combo[0]] == grid[combo[1]] && grid[combo[1]] == grid[combo[2]]){
            int winner = grid[combo[0]];
            for(int i = 0; i < GRID_SIZE; i++){
                grid[i] = GRID_VAL_NONE;
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
            if(grid[i] == GRID_VAL_NONE){
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
    if(history.empty()) return;

    int lastMove = history.back();
    history.pop_back();

    grid[lastMove] = GRID_VAL_NONE;
    playerData[0]->player->undo(lastMove);
    playerData[1]->player->undo(lastMove);

    currentPlayer = !currentPlayer;

    render = true;

    if( playerData[currentPlayer]->player->isComputer() &&
        playerData[!currentPlayer]->player->isHuman() ){
        undo();
    }

    startNextTurn();
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

    playerData[0] = new PlayerData();
    playerData[1] = new PlayerData();

    playerData[0]->graphic = &graphic_x;
    playerData[1]->graphic = &graphic_o;

    playerData[0]->player = new HumanPlayer();
    playerData[1]->player = new HumanPlayer();

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
                //CTRL+Z undo
                if(event->key.keysym.sym == SDLK_z){
                    undo();
                }
            }
        }
        //Turn computer player on/off
        if(event->key.keysym.sym == SDLK_F1){
            if( playerData[1]->player->isComputer() ){
                delete playerData[1]->player;
                playerData[1]->player = new HumanPlayer();
            }else{
                delete playerData[1]->player;
                playerData[1]->player = new RandomComputerPlayer();
            }
            newGame();
            return;
        }else if(event->key.keysym.sym == SDLK_F2){
            PlayerData* temp = playerData[0];
            playerData[0] = playerData[1];
            playerData[1] = temp;
            newGame();
            return;
        }else if(event->key.keysym.sym == SDLK_F3){
            Texture* temp = playerData[0]->graphic;
            playerData[0]->graphic = playerData[1]->graphic;
            playerData[1]->graphic = temp;
            render = true;
        }
        // Allow player to quit the game by closing the window
        else if(event->key.keysym.sym == SDLK_ESCAPE){
            newGame();
            return;
        }

    default:
        if( !shouldWaitForEvent ) break;
        playerData[currentPlayer]->player->getNextMoveOnEvent( currentGridIndex, shouldWaitForEvent,
                        history.empty() ? GRID_POS_NONE:history.back(), utility::getGridIndexForMouse(), event );
        if(currentGridIndex != previousGridIndex){
            render = true;
            previousGridIndex = currentGridIndex;
        }
        if( !shouldWaitForEvent ){
            if(grid[currentGridIndex] == GRID_VAL_NONE){
                finishCurrentTurn();
            }else{
                shouldWaitForEvent = true;
            }
        }
    }
}

void CApp::OnLoop(){

}

void CApp::OnRender(){
    if(!render) return;

    SDL_RenderClear(renderer);

    graphic_grid.render();
    for(int i = 0; i < 9; i++){
        if(grid[i] == GRID_VAL_NONE) continue;
        int x=0, y=0;
        utility::getScreenPosForGridIndex(i, x, y);

        playerData[ grid[i] ]->graphic->render(x, y);
    }
    if(currentGridIndex != GRID_POS_NONE && grid[currentGridIndex] == GRID_VAL_NONE && !gameOver){
        int x=0, y=0;
        utility::getScreenPosForGridIndex(currentGridIndex, x, y);

        playerData[currentPlayer]->graphic->setAlpha(255/2);
        playerData[currentPlayer]->graphic->render(x, y);
        playerData[currentPlayer]->graphic->setAlpha(255);
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
