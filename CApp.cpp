#include<windows.h>
#include "CApp.h"
#undef main
using namespace std;

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

int main(){
    CApp theApp;
    return theApp.OnExecute();
}
