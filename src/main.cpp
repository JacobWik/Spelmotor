#include <iostream>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL2/SDL_ttf.h>

#define FPS 30

const int WIDTH = 800, HEIGHT = 600;


bool check_collision( SDL_Rect A, SDL_Rect B )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = A.x;
    rightA = A.x + A.w;
    topA = A.y;
    bottomA = A.y + A.h;

    //Calculate the sides of rect B
    leftB = B.x;
    rightB = B.x + B.w;
    topB = B.y;
    bottomB = B.y + B.h;

     //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}



int main(int argc, char* argv[]){





SDL_Init(SDL_INIT_EVERYTHING);

SDL_Window *window = SDL_CreateWindow("TitelForWindow", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);

if ( NULL == window ){    std::cout  << "Could not create window" << SDL_GetError( ) <<std::endl;   return 1; }

if(TTF_Init() < 0) {
    std::cout << "Couldn't initialize TTF lib: " << TTF_GetError() << std::endl;
    return 1;
}


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
//SDL_Rect gubbRect = {300,0, gubbSurface->w, gubbSurface->h};  //Parametrar: (XKoordinat, YKoordinat,bredd, höjd) Görs innan surface är clearat || Var f�r stor
SDL_Rect gubbRect = {500,80, 300,300};
Uint32 green = SDL_MapRGB(gubbSurface->format, 0, 255, 0); // Dessa 2 rader gör att alla pixlar som har GRÖN färg görs genomskinliga
SDL_SetColorKey(gubbSurface, true, green); // Rad 2 för att göra ogenomskinlig
SDL_Texture* gubbTexture = SDL_CreateTextureFromSurface(ren, gubbSurface); //Framställer texture från Surface. Görs sist efter all manipulering.


// Test med en till gubbe
//SDL_Rect gubbRect2 = {100,50, gubbSurface->w, gubbSurface->h};
SDL_Rect gubbRect2 = {100,300, 100, 100}; //Parametrar: (XKoordinat, YKoordinat,bredd, höjd) Görs innan surface är clearat
SDL_Texture* gubbTexture2 = SDL_CreateTextureFromSurface(ren, gubbSurface); //Framställer texture från Surface. Görs sist efter all manipulering.
//En till gubbe
SDL_Rect gubbRect3 = {100,400, 100, 100}; //Parametrar: (XKoordinat, YKoordinat,bredd, höjd) Görs innan surface är clearat
SDL_Texture* gubbTexture3 = SDL_CreateTextureFromSurface(ren, gubbSurface); //Framställer texture från Surface. Görs sist efter all manipulering.


//SDL_FreeSurface(gubbSurface); // Eftersom jag skapat texturen av gubbSurface borde den tas bort såhär


bool goOn = true;
bool drag = false;

// Bool f�r att hantera b�da gubbarna
bool g1 = false;
bool g2 = false;
bool g3 = false;
int counter = 0;



//testar fixa tick
const int tickInterval = 1000 / FPS; // I fps
Uint32 nextTick;
int delay;

//F�rs�k till att skapa collision
bool collision = false;
bool collision2 = false;

// Testa rita ut text l�gger en siffra �ver varje karakt�r
TTF_Font* font = TTF_OpenFont("c:\\Windows\\Fonts\\arial.ttf",36);
if(font == NULL){std::cout<<"DEN ÄR NULL! | " << SDL_GetError()<<std::endl;}
SDL_Color color = {255,0,0};
SDL_Surface* textSurface = TTF_RenderText_Solid(font, "3", color);
SDL_Texture* textTexture = SDL_CreateTextureFromSurface(ren, textSurface);
SDL_Rect txtRect = {90,80,textSurface->w,textSurface->h};
//gubbe2
SDL_Surface* textSurface2 = TTF_RenderText_Solid(font, "2", color);
SDL_Texture* textTexture2 = SDL_CreateTextureFromSurface(ren, textSurface2);
SDL_Rect txtRect2 = {90,80,textSurface2->w,textSurface2->h};
//Huvudgubbe
SDL_Surface* textSurface3 = TTF_RenderText_Solid(font, "1", color);
SDL_Texture* textTexture3 = SDL_CreateTextureFromSurface(ren, textSurface3);
SDL_Rect txtRect3 = {90,80,textSurface3->w,textSurface3->h};

// Loop som kollar efter events för programmet
while (goOn){

    //Tick f�r "Servern"
    nextTick = SDL_GetTicks() + tickInterval;

    //varje tick g� lilla  gubben lite h�ger
    gubbRect2.x+=1;
    gubbRect3.x+=1;
    if(gubbRect2.x >=780)
        gubbRect2.x = 5;
    if(gubbRect3.x >=780)
        gubbRect3.x = 5;

    //G�r s� textSiffran f�ljer gubben
    txtRect.x = (gubbRect3.x + 30);
    txtRect.y = (gubbRect3.y - 14);
    txtRect2.x = (gubbRect2.x + 30);
    txtRect2.y = (gubbRect2.y - 14);
    // txtRect3.x = (gubbRect.x + 85);
    // txtRect3.y = (gubbRect.y - 14);
    txtRect3.x = (gubbRect.x + 100 );
    txtRect3.y = (gubbRect.h);

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
                if(SDL_PointInRect(&p, &gubbRect)) {
                     drag = true;
                     g1 = true;
                }
                if(SDL_PointInRect(&p, &gubbRect2)) {
                     drag = true;
                     g2 = true;
                }
                if(SDL_PointInRect(&p, &gubbRect3)) {
                     drag = true;
                     g3 = true;
                }
            }
                break;
            case SDL_MOUSEBUTTONUP:
                if(drag && g1){
                    drag = false;
                    g1 = false;
                }else if(drag && g2){
                    drag = false;
                    g2 = false;
                }else if(drag && g3){
                    drag = false;
                    g3 = false;
                }
                break;
            case SDL_MOUSEMOTION:
                if(drag && g1){
                    gubbRect.x += eve.motion.xrel;
                    gubbRect.y += eve.motion.yrel;
                }
                if(drag && g2){
                    gubbRect2.x += eve.motion.xrel;
                    gubbRect2.y += eve.motion.yrel;
                }
                if(drag && g3){
                    gubbRect3.x += eve.motion.xrel;
                    gubbRect3.y += eve.motion.yrel;
                }
                break;
        }
    }

    //Tick f�r "Servern"
    delay = nextTick - SDL_GetTicks();
    if(delay > 0)
        SDL_Delay(delay);

    //Testar med collision
    collision = check_collision(gubbRect, gubbRect2);
    collision2 = check_collision(gubbRect, gubbRect3);

    if(collision){
        counter++;
        std::cout << counter << std::endl;
       // green = SDL_MapRGB(gubbSurface->format, 255, 255, 255); // Dessa 2 rader gör att alla pixlar som har GRÖN färg görs genomskinliga
        SDL_SetColorKey(gubbSurface, false, green); // Rad 2 för att göra ogenomskinlig
        gubbTexture = SDL_CreateTextureFromSurface(ren, gubbSurface);
        gubbTexture2 = SDL_CreateTextureFromSurface(ren, gubbSurface);
    }else if(collision2){
        counter++;
        std::cout << counter << std::endl;
       // green = SDL_MapRGB(gubbSurface->format, 255, 255, 255); // Dessa 2 rader gör att alla pixlar som har GRÖN färg görs genomskinliga
        SDL_SetColorKey(gubbSurface, false, green); // Rad 2 för att göra ogenomskinlig
        gubbTexture = SDL_CreateTextureFromSurface(ren, gubbSurface);
        gubbTexture3 = SDL_CreateTextureFromSurface(ren, gubbSurface);
    }
    else{
       // green = SDL_MapRGB(gubbSurface->format, 0, 255, 0); // Dessa 2 rader gör att alla pixlar som har GRÖN färg görs genomskinliga
        SDL_SetColorKey(gubbSurface, true, green); // Rad 2 för att göra ogenomskinlig
        gubbTexture = SDL_CreateTextureFromSurface(ren, gubbSurface); //Framställer texture från Surface. Görs sist efter all manipulering.
        gubbTexture2 = SDL_CreateTextureFromSurface(ren, gubbSurface);
        gubbTexture3 = SDL_CreateTextureFromSurface(ren, gubbSurface);
    }


// Skriv ut Texture på skärmen -> bilder/images: 1. Sudda bakgrund 2.Skriv ut rendercopy i rendereraren.
SDL_RenderCopy(ren,bgTexture, NULL, NULL); // Man har null så sträcks bilden ut och sätts över hela skärmen / Har man flera sprites(Spritesheet kan man ändra här)
SDL_RenderCopy(ren,gubbTexture, NULL, &gubbRect);  // Sista parametern tar en pekare, så skriver adressen till gubbRect

//Testar l�gga till ny gubbe
SDL_RenderCopy(ren,gubbTexture2, NULL, &gubbRect2);
SDL_RenderCopy(ren,gubbTexture3, NULL, &gubbRect3);

//Testar l�gga in text
SDL_RenderCopy(ren, textTexture,NULL, &txtRect);
SDL_RenderCopy(ren, textTexture2,NULL, &txtRect2);
SDL_RenderCopy(ren, textTexture3,NULL, &txtRect3);



SDL_RenderPresent(ren); // Skriver ut från bufferten som renderaren håller, ut på skärmen
SDL_RenderClear(ren); // Suddar, cleanar fönstret



}

SDL_FreeSurface(textSurface);
SDL_FreeSurface(textSurface2);
SDL_FreeSurface(textSurface3);

SDL_FreeSurface(gubbSurface); // Eftersom jag skapat texturen av gubbSurface borde den tas bort såhär

SDL_DestroyTexture(textTexture);
SDL_DestroyTexture(textTexture2);
SDL_DestroyTexture(textTexture2);
TTF_CloseFont(font);
// Rensa program i slutet
SDL_DestroyRenderer(ren);
SDL_DestroyWindow(window);
SDL_Quit( );

return EXIT_SUCCESS;



}

