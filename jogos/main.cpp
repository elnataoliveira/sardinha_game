#include <allegro.h>
#include<stdio.h>

volatile int exit_program;
void fecha_programa() { exit_program = TRUE; }
END_OF_FUNCTION(fecha_programa)

volatile int ticks;
void tick_counter() { ticks++; }
END_OF_FUNCTION(tick_counter)

volatile int milisegundos;
void msec_counter() { milisegundos++; }
END_OF_FUNCTION(msec_counter)

volatile int timer;
void incrementa_timer() { timer++; }
END_OF_FUNCTION(incrementa_timer)

volatile int tempo;
void decrementa_tempo() { tempo--; }
END_OF_FUNCTION(decrementa_tempo)

int resgate(float a, float b, float c, float d){

	if(a==b&&c==d){
                  
    return 0;
    }
	else
	return 1;
	
	printf("%d %d %d %d",a,b,c,d);
	system("pause");

}

float max(float a, float b)
{
  if(a >= b)
   return a;
  else
   return b;
}

float min(float a, float b)
{
  if(a <= b)
   return a;
  else
   return b;
}

///M?TODO PIXEL PERFECT COLIsÃO
int pixel_perfect_colision(float x1, float y1, BITMAP* obj1, float x2, float y2, BITMAP* obj2)
{
   float i, j;
   int colisao = FALSE;

   //Se os ret?ngulos estiverem se intersectando
   if(!( (x1 > x2 + obj2->w) || (y1 > y2 + obj2->h) || (x2 > x1 + obj1->w) || (y2 > y1 + obj1->h) ))
   {
     float cima = max(y1, y2);
	 float baixo = min(y1 + obj1->h, y2 + obj2->h);
	 float esquerda = max(x1, x2);
     float direita = min(x1 + obj1->w, x2 + obj2->w);

	 for(i = cima; i < baixo && !colisao; i++)
	 {
		for(j = esquerda; j < direita && !colisao; j++)
		{
		  if(getpixel(obj1, (int)(j-x1), (int)(i-y1)) != makecol(255,0,255) && getpixel(obj2, (int)(j-x2), (int)(i-y2)) != makecol(255,0,255))
           colisao = TRUE;
		}
	 }
   }
   return colisao;
}

int main()
{
  allegro_init();
  install_timer();
  install_keyboard();
  set_color_depth(32);
  set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0);
  install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,NULL);
  set_window_title("Sardinha Game");

  exit_program = FALSE;
  LOCK_FUNCTION(fecha_programa);
  LOCK_VARIABLE(exit_program);
  set_close_button_callback(fecha_programa);

  
  tempo = 150;
  LOCK_FUNCTION(decrementa_tempo);
  LOCK_VARIABLE(tempo);
  install_int_ex(decrementa_tempo, MSEC_TO_TIMER(1000));

  ///BITMAPS
  BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
  BITMAP* aquario = load_bitmap("aquario.bmp", NULL);
  BITMAP* diver = load_bitmap("diver.bmp", NULL);
  BITMAP* gold = load_bitmap("gold.bmp", NULL);
  BITMAP* shark = load_bitmap("shark.bmp", NULL);
  
  ///Variáveis
  int num_frames = 12;
  int frame_w = diver->w / num_frames;
  int frame_atual;
  int tempo_troca = 100;
  

  
  float x=-200,y=900,x1=-200,y1=900,x2=-200,y2=900,px=-40,py=-40;
  int dir=2,gI=1,gII=1,gIII=1,gIV=1,w=0;
  int topo=1,ponto=0,qntd=0;
  
  
  ///SONS
  MIDI* midi = load_midi("Jaws.mid");
  SAMPLE* sample = load_sample("efeito.wav");
  play_midi(midi,1);
  
  ///GAME LOOP
  
  while(!exit_program)
  {
     //INPUT
     if(key[KEY_ESC])
     fecha_programa();

     //UPDATE
     frame_atual = (milisegundos / tempo_troca) % num_frames;
     if(py==-39&&topo==0){
        topo=1;
        qntd++;
        ponto+=5000;
        }
        if(qntd==4){
        ponto+=(tempo*7);
        while(!key[KEY_ESC]){
        textprintf_centre_ex(screen, font, SCREEN_W / 2, 120,makecol(255, 0, 0), -1,"VOCE EH O MELHOR SEUS PONTOS %d!",ponto);}
        }
        if(tempo==-1)
        while(tempo<0&&!key[KEY_ESC]){
        textout_centre_ex(screen, font,"GAME OVER",SCREEN_W/2,SCREEN_H/2,makecol(255,0,0),makecol(0,0,0));
        }
     //DRAW
     draw_sprite(buffer, aquario, 0, 0);
     textprintf_ex(buffer, font, 350, 50, makecol(255,255,0), -1, "R$: %d    %d", ponto, tempo);
     
     
     if(gI){
     draw_sprite(buffer, gold, 400, 530);
            if(topo)
            topo=gI=resgate(px,400,py+50,530);
            }
     if(gII){
     draw_sprite(buffer, gold, 560, 530);
            if(topo)
            topo=gII=resgate(px,560,py+50,530);}
     if(gIII){
            draw_sprite(buffer, gold, 200, 530);  
            if(topo)
            topo=gIII=resgate(px,200,py+50,530);}
     if(gIV){
            draw_sprite(buffer, gold, 500, 530);
            if(topo)
            topo=gIV=resgate(px,500,py+50,530);}
     
              
     //MERGULHADOR
                           
     if(key[KEY_DOWN]&&(py<480)){
     draw_sprite(buffer, diver, (int)px, (int)py);
     py+=0.125;}
     else
         if(key[KEY_UP]&&(py>-40)){
         draw_sprite(buffer, diver, (int)px, (int)py);
         py-=0.125;}
         else
             if(key[KEY_RIGHT]&&(px<666)){
             draw_sprite(buffer, diver, (int)px, (int)py);
             px+=0.125;}
             else
                 if(key[KEY_LEFT]&&(px>-20)){
                 draw_sprite_h_flip(buffer, diver, (int)px,(int) py);
                 px-=0.125;}                           
                 else{
                 draw_sprite(buffer, diver, (int)px, (int)py);}
                               
     //SHARKS
     
      if(x<900){
      x+=0.25;
      x1+=0.25;
      y2-=0.25;
      
      if(x==900){
      dir++;
      y=900;
      y1=900;
      x2=-200;}
      }
      else
      if(y>-200){
      y-=0.25;
      y1-=0.25;
      x2+=0.25;
      if(y==-200){
      dir++;
      x=-200;
      x1=-200;
      y2=900;}
  }
  
     if(dir%2==0){
     draw_sprite(buffer, shark, (int)x, 50);             
     draw_sprite_h_flip(buffer, shark, (int)y2, 250);
     draw_sprite(buffer, shark, (int)x1, 450);
     }
     else{
     draw_sprite_h_flip(buffer, shark, (int)y, 30);
     draw_sprite(buffer, shark, (int)x2, 300);  
     draw_sprite_h_flip(buffer, shark, (int)y1, 530);
     }//END SHARKS
     
  
  if(pixel_perfect_colision(px, py, diver, x, 50, shark) == TRUE)
  while(!key[KEY_ESC]){
  textout_centre_ex(screen, font,"GAME OVER",SCREEN_W/2,SCREEN_H/2,makecol(255,0,0),makecol(0,0,0));
  }
  else
  if(pixel_perfect_colision(px, py, diver, y, 30, shark) == TRUE)
  while(!key[KEY_ESC]){
  textout_centre_ex(screen, font,"GAME OVER",SCREEN_W/2,SCREEN_H/2,makecol(255,0,0),makecol(0,0,0));
  }
  else
  if(pixel_perfect_colision(px, py, diver, y2, 250, shark) == TRUE)
  while(!key[KEY_ESC]){
  textout_centre_ex(screen, font,"GAME OVER",SCREEN_W/2,SCREEN_H/2,makecol(255,0,0),makecol(0,0,0));
  }
  else
  if(pixel_perfect_colision(px, py, diver, x2, 300, shark) == TRUE)
  while(!key[KEY_ESC]){
  textout_centre_ex(screen, font,"GAME OVER",SCREEN_W/2,SCREEN_H/2,makecol(255,0,0),makecol(0,0,0));
  }
  else
  if(pixel_perfect_colision(px, py, diver, x1, 450, shark) == TRUE)
  while(!key[KEY_ESC]){
  textout_centre_ex(screen, font,"GAME OVER",SCREEN_W/2,SCREEN_H/2,makecol(255,0,0),makecol(0,0,0));
  }
  else
  if(pixel_perfect_colision(px, py, diver, y1, 530, shark) == TRUE)
  while(!key[KEY_ESC]){
  textout_centre_ex(screen, font,"GAME OVER",SCREEN_W/2,SCREEN_H/2,makecol(255,0,0),makecol(0,0,0));
  }
  
  draw_sprite(screen, buffer, 0, 0);
  clear(buffer);
  }
    
  ///FINALIZAÇÃO
  destroy_bitmap(buffer);
  destroy_bitmap(gold);
  destroy_bitmap(aquario);
  destroy_bitmap(diver);
  destroy_bitmap(shark);
  destroy_midi(midi);
  destroy_sample(sample);

  return 0;
  
}
END_OF_MAIN();
