#ifndef IMAGEUTILITY_H_INCLUDED
#define IMAGEUTILITY_H_INCLUDED
#include <SDL.h>
#include <string>


bool loadImage(std::string path, SDL_Surface** image, SDL_PixelFormat* format = nullptr){
    if(*image != nullptr){
        //if the image pointer contains data, unload it
        SDL_FreeSurface(*image);
    }
    //load the raw bitmap image before optimizing
    SDL_Surface* unformatted = nullptr;
    unformatted = SDL_LoadBMP(path.c_str());
    if(unformatted != nullptr){
        if(format == nullptr){
            *image = unformatted;
            return true;
        }
        *image = SDL_ConvertSurface(unformatted, format, 0);
        if(*image == nullptr){
            *image = unformatted;
        }
        return true;
    }
    return false;
}

bool drawImage(SDL_Surface* image, SDL_Surface* canvas, int x, int y){
    if(image == nullptr || canvas == nullptr)
        return false;
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    SDL_BlitSurface(image, nullptr, canvas, &dest);
    return true;
}
bool drawTile(SDL_Surface* image, SDL_Surface* canvas, int dest_x, int dest_y, int src_x, int src_y, int src_w, int src_h){
    if(image == nullptr || canvas == nullptr)
        return false;

    SDL_Rect src;
    SDL_Rect dest;

    dest.x = dest_x;
    dest.y = dest_y;

    src.x = src_x;
    src.y = src_y;
    src.w = src_w;
    src.h = src_h;

    SDL_BlitSurface(image, &src, canvas, &dest);
    return true;
}

void makeTransparent(SDL_Surface* image, Uint8 r, Uint8 g, Uint8 b){
    SDL_SetColorKey( image, SDL_TRUE, SDL_MapRGB( image->format, r, g, b ) );
}

#endif // IMAGEUTILITY_H_INCLUDED
