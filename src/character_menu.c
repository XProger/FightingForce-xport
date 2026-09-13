#include "ff.h"
#include <stdio.h>
#include <string.h>
/* GAME.EXE:8004F4B8. Return one only if the candidate is absent. */
GDB_CALL sint32 FUN_8004F4B8(sint32 candidate,uint32 selected,sint32 count) {
 sint32 i;for(i=0;i<count;i++)if(ff_u32(selected+4u*i)==(uint32)candidate)return 0;return 1;
}
static uint32 actor_name(uint32 selection) {
 return ff_u32(0x800b6e90u+4u*(uint32)(sint32)ff_s16(0x800b434c+244u*selection));
}
/* Host ABI bridge for the original stack-local sprintf buffer. The scratch RAM
 * bytes are restored after packet emission; no pointer escapes into game state. */
static void draw_pair_names(uint32 format,uint32 first,uint32 second) {
 uint8 saved[72];char *buffer=(char*)ff_ptr(0x801fc100,72);
 memcpy(saved,buffer,72);
 snprintf(buffer,72,(const char*)ff_ptr(format,1),(const char*)ff_ptr(first,1),(const char*)ff_ptr(second,1));
 FUN_80061ADC(0x801fc100,50);memcpy(buffer,saved,72);
}
/* GAME.EXE:80050798. Character selection callback. Cancel resets ready bits but
 * intentionally does not return early: further same-frame input still executes. */
GDB_CALL sint32 FUN_80050798(void) {
 sint32 i;
 if(ff_u32(0x80092780)==ff_u32(0x8009277c))ff_w32(0x80092780,(ff_u32(0x80092780)+1)&3);
 FUN_80061ADC(ff_u32(0x8009469c),214);FUN_8005EC90();ff_w32(0x800940b0,ff_u32(0x8009400c));
 if((sint32)ff_u32(0x8009400c)<(sint32)ff_u32(0x80092784)) {
  ff_w32(0x800927ec,0x80050270);FUN_8004F980((uint32*)ff_ptr(0x80092720,12),0,-12032);return FUN_8004F490();
 }
 if(ff_u32(0x80092784)==1) {
  FUN_80061ADC(ff_u32(0x80092510),28);FUN_80061ADC(actor_name(ff_u32(0x8009277c)),50);
 }else {
  uint32 language=ff_u32(0x80093d70),format=language==1?0x80092828:language==2?0x80092834:0x80092840;
  FUN_80061ADC(ff_u32(0x80092514),28);draw_pair_names(format,actor_name(ff_u32(0x8009277c)),actor_name(ff_u32(0x80092780)));
 }
 for(i=0;i<(sint32)ff_u32(0x80092784);i++) {
  uint32 raw=(uint32)FUN_8005F5AC((sint32)ff_u32(0x80094288+4u*i)),edge=0x80092798+4u*i,selected=0x8009277c+4u*i;
  ff_w32(edge,(raw^ff_u32(0x80092790+4u*i))&raw);ff_w32(0x80092790+4u*i,raw);ff_w32(0x800927a0,raw);
  if(ff_u32(edge)&0x1000) {
   ff_w32(0x800927ec,0x80050270);ff_w32(0x8009272c,0);FUN_8004F980((uint32*)ff_ptr(0x80092720,12),0,-12032);FUN_8004F490();
  }
  if(!((ff_u32(0x8009272c)>>((uint32)i&31))&1)) {
   if((ff_u32(edge)&0x80)&&(sint32)ff_u32(selected)>0) {
    sint32 candidate=(sint32)(ff_u32(selected)-1);
    for(;candidate>=0;candidate--)if(FUN_8004F4B8(candidate,0x8009277c,(sint32)ff_u32(0x80092784))) {
     ff_w32(selected,(uint32)candidate);FUN_8004FA10();FUN_8004F468();break;
    }
    if(candidate==-1)ff_menu_effect(5,223);
   }
   if((ff_u32(edge)&0x20)&&(sint32)ff_u32(selected)<3) {
    sint32 candidate=(sint32)(ff_u32(selected)+1);
    for(;candidate<4;candidate++)if(FUN_8004F4B8(candidate,0x8009277c,(sint32)ff_u32(0x80092784))) {
     ff_w32(selected,(uint32)candidate);FUN_8004FA10();FUN_8004F468();break;
    }
    if(candidate==4)ff_menu_effect(5,223);
   }
   if(ff_u32(edge)&0x4000) {
    FUN_8004F490();ff_w32(0x800940b0,ff_u32(0x80092784));ff_w32(0x8009272c,ff_u32(0x8009272c)|(1u<<((uint32)i&31)));
   }
  }
 }
 if(ff_u32(0x80092784)==1&&ff_u32(0x8009272c)==1)ff_w32(0x80092788,1);
 if(ff_u32(0x80092784)==2){if(ff_u32(0x8009272c)==3)ff_w32(0x80092788,1);return 1;}
 return 3;
}
