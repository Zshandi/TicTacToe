#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include <SDL.h>
#include <string>
#include <cstdio>
using namespace std;

class Texture{
    SDL_Texture* texture;
    SDL_Renderer* renderer;
    int width;
    int height;
public:
    //Do not load any texture
    Texture(){
        texture = nullptr;
        renderer = nullptr;
    }
    //Load blank texture
    Texture(SDL_Renderer* ren, int w, int h, SDL_TextureAccess access = SDL_TEXTUREACCESS_TARGET){

        renderer = ren;
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, access, w, h);
        if(texture == nullptr){
            return;
        }
        width = w;
        height = h;
    }
    //load from surface
    Texture(SDL_Renderer* ren, SDL_Surface* surface){

        texture = nullptr;
        if(surface == nullptr) return;

        renderer = ren;
        texture = SDL_CreateTextureFromSurface( renderer, surface );
        if(texture == nullptr) return;

        width = surface->w;
        height = surface->h;
    }
    //Load from path
    Texture(SDL_Renderer* ren, std::string path, bool trans=false, Uint8 r=0, Uint8 g=0, Uint8 b=0){

        SDL_Surface* surface = nullptr;
        texture = nullptr;
        renderer = nullptr;
        #ifdef _SDL_IMAGE_H
        surface = IMG_Load(path.c_str());
        #endif
        #ifndef _SDL_IMAGE_H
        surface = SDL_LoadBMP(path.c_str());
        #endif
        if(surface == nullptr){
            return;
        }
        if(trans){
            SDL_SetColorKey( surface, SDL_TRUE, SDL_MapRGB( surface->format, r, g, b ) );
        }
        renderer = ren;

        texture = SDL_CreateTextureFromSurface( renderer, surface );
        if(texture == nullptr) return;

        width = surface->w;
        height = surface->h;

        SDL_FreeSurface(surface);
    }
    //Load from text, if TTF is available
    #ifdef _SDL_TTF_H
    Texture(SDL_Renderer* ren, std::string text, SDL_Color color, TTF_Font* font){

        SDL_Surface* surface = nullptr;

        surface = TTF_RenderText_Solid(font, text.c_str(), color);

        if(surface == nullptr) return;
        renderer = ren;

        texture = SDL_CreateTextureFromSurface( renderer, surface );
        if(texture == nullptr) return;

        width = surface->w;
        height = surface->h;

        SDL_FreeSurface(surface);
    }
    #endif // _SDL_TTF_H

    bool loaded(){
        return texture != nullptr;
    }
    bool render(int x = 0, int y = 0, SDL_Rect* clip = nullptr, double angle = 0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE){
        if(!loaded()) return false;
        bool success;
        SDL_Rect dest = {x, y, width, height};
        if(clip != nullptr){
            dest.w = clip->w;
            dest.h = clip->h;
        }
        success = SDL_RenderCopyEx(renderer, texture, clip, &dest, angle, center, flip) == 0;
        return success;
    }
    void unload(){
        if(loaded()){
            SDL_DestroyTexture(texture);
            texture = nullptr;
            renderer = nullptr;
            width = 0;
            height = 0;
        }
    }
    void setBlend(SDL_BlendMode mode = SDL_BLENDMODE_BLEND){
        SDL_SetTextureBlendMode(texture, mode);
    }
    void setAlpha(Uint8 alpha){
        SDL_SetTextureAlphaMod(texture, alpha);
    }
    void setColorMod(Uint8 r, Uint8 g, Uint8 b){
        SDL_SetTextureColorMod(texture, r, g, b);
    }
    void setAsTarget(bool target = true){
        SDL_SetRenderTarget(renderer, target?texture:nullptr);
    }

    int getHeight(){
        return height;
    }
    int getWidth(){
        return width;
    }

    Texture(const Texture& other){
        bool success = false;
        renderer = other.renderer;
        width = other.width;
        height = other.height;
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
        if(texture != nullptr){
            if(SDL_SetRenderTarget(renderer, texture) != -1){
                SDL_RenderClear(renderer);
                if(SDL_RenderCopy(renderer, other.texture, nullptr, nullptr) != -1){
                    SDL_RenderPresent(renderer);
                    SDL_SetRenderTarget(renderer, nullptr);
                    success = true;
                }
            }
            if(!success){
                SDL_DestroyTexture(texture);
                texture = nullptr;
            }
        }
    }
    Texture& operator= (const Texture& other){
        unload();
        bool success = false;
        renderer = other.renderer;
        width = other.width;
        height = other.height;
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
        if(texture != nullptr){
            if(SDL_SetRenderTarget(renderer, texture) != -1){
                SDL_RenderClear(renderer);
                if(SDL_RenderCopy(renderer, other.texture, nullptr, nullptr) != -1){
                    SDL_RenderPresent(renderer);
                    SDL_SetRenderTarget(renderer, nullptr);
                    success = true;
                }
            }
            if(!success){
                SDL_DestroyTexture(texture);
                texture = nullptr;
            }
        }
        return *this;
    }
    Texture(Texture&& other){
        renderer = other.renderer;
        width = other.width;
        height = other.height;
        texture = other.texture;
        //Remove ownership from other
        other.texture = nullptr;
        other.renderer = nullptr;
        other.width = 0;
        other.height = 0;
    }
    Texture& operator= (Texture&& other){
        unload();
        renderer = other.renderer;
        width = other.width;
        height = other.height;
        texture = other.texture;
        //Remove ownership from other
        other.texture = nullptr;
        other.renderer = nullptr;
        other.width = 0;
        other.height = 0;
        return *this;
    }

    ~Texture(){
        unload();
    }
};

#endif // TEXTURE_H_INCLUDED
