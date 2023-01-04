#include <iostream>
#include <SDL2/SDL.h>
#include <stdio.h>

const int WIDTH = 800, HEIGHT = 600;

int main(int argc, char* argv[]){


SDL_Init(SDL_INIT_EVERYTHING);

SDL_Window *window = SDL_CreateWindow("TitelForWindow", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);

if ( NULL == window ){    std::cout  << "Could not create window" << SDL_GetError( ) <<std::endl;   return 1; }


SDL_Renderer* ren = SDL_CreateRenderer(window, -1, 0);



//Ladda in bakgrund
SDL_Surface* bgSurface = SDL_LoadBMP("./images/bgg.bmp"); // Ladda in en bild
    // Användes när bakgrunden var svart
        //if(bgSurface == NULL){std::cout<<"DEN ÄR NULL! | " << SDL_GetError()<<std::endl;}
SDL_Texture* bgTexture = SDL_CreateTextureFromSurface(ren, bgSurface); // Omvandla till en texture
SDL_FreeSurface(bgSurface); // Eftersom jag skapat texturen av bgSurface borde den tas bort såhär


//Ladda in gubbe
SDL_Surface* gubbSurface = SDL_LoadBMP("./images/gubbeng.bmp");
if(gubbSurface == NULL){std::cout<<"DEN ÄR NULL! | " << SDL_GetError()<<std::endl;}
SDL_Rect gubbRect = {300,0, gubbSurface->w, gubbSurface->h};  //Parametrar: (XKoordinat, YKoordinat,bredd, höjd) Görs innan surface är clearat
Uint32 green = SDL_MapRGB(gubbSurface->format, 0, 255, 0); // Dessa 2 rader gör att alla pixlar som har GRÖN färg görs genomskinliga
SDL_SetColorKey(gubbSurface, true, green); // Rad 2 för att göra ogenomskinlig
SDL_Texture* gubbTexture = SDL_CreateTextureFromSurface(ren, gubbSurface); //Framställer texture från Surface. Görs sist efter all manipulering.
SDL_FreeSurface(gubbSurface); // Eftersom jag skapat texturen av gubbSurface borde den tas bort såhär

// Loop som kollar efter events för programmet
bool goOn = true;
bool drag = false;
while (goOn){
     SDL_Event eve;

    //Denna inre loop tar fram nästa händelse i händelsekön -> returnar 0 om ingen händelse finns, 1 om det finns
    while(SDL_PollEvent(&eve)){
        switch(eve.type){
            case SDL_QUIT: goOn = false; break;
            case SDL_KEYDOWN:
                switch(eve.key.keysym.sym){
                    case SDLK_RIGHT: gubbRect.x+=3; break;
                    case SDLK_LEFT: gubbRect.x-=3; break;
                    case SDLK_UP: gubbRect.y-=3; break;
                    case SDLK_DOWN: gubbRect.y+=3; break;
                }
            case SDL_MOUSEBUTTONDOWN: {
                SDL_Point p = {eve.button.x, eve.button.y };
                if(SDL_PointInRect(&p, &gubbRect)) { drag = true; }
            }
                break;
            case SDL_MOUSEBUTTONUP: drag = false; break;
            case SDL_MOUSEMOTION:
                if(drag){
                    gubbRect.x += eve.motion.xrel;
                    gubbRect.y += eve.motion.yrel;
                }
                break;
    }
    }

// Skriv ut Texture på skärmen -> bilder/images: 1. Sudda bakgrund 2.Skriv ut rendercopy i rendereraren.
SDL_RenderCopy(ren,bgTexture, NULL, NULL); // Man har null så sträcks bilden ut och sätts över hela skärmen / Har man flera sprites(Spritesheet kan man ändra här)
SDL_RenderCopy(ren,gubbTexture, NULL, &gubbRect);  // Sista parametern tar en pekare, så skriver adressen till gubbRect
SDL_RenderPresent(ren); // Skriver ut från bufferten som renderaren håller, ut på skärmen
SDL_RenderClear(ren); // Suddar, cleanar fönstret
}


// Rensa program i slutet
SDL_DestroyRenderer(ren);
SDL_DestroyWindow(window);
SDL_Quit( );

return EXIT_SUCCESS;


}