#include "ff.h"
#include <stdlib.h>
/* 61E6C early gates. Actual multiplayer continue remains explicit WIP. */
GDB_CALL uint32 ff_continue_hud_80061E6C(uint32 player) {
 uint32 actor=0x800b4318+244u*player;
 if(ff_u32(0x800940b0)==1||!*(sint8*)ff_ptr(actor+119,1)||ff_s16(0x800b435a+244u*(1u-player))<=0||(sint32)ff_u32(0x80093d50)<=0)return 0;
 ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);return 0;
}
static void hud_ammo(uint32 actor,uint32 player,uint32 slot) {
 sint32 index=*(sint8*)ff_ptr(actor+slot,1);uint32 item,type;int show;sint32 x;
 if(index==-1)return;
 item=0x8009a3c8+36u*(uint32)index;type=ff_u32(item);
 show=slot==125?(type==515||type==504):(type==515||type==506||(!player&&type==505)||type-499u<5u);
 if(!show)return;
 /* PSX-addressed temporary beyond game globals, equivalent to caller stack. */
 FUN_80061E0C(ff_s16(item+22),0x801fc100);
 x=FUN_80061B7C(0x80093a60,player?224:16,200);FUN_80061B7C(0x801fc100,x,200);
}
/* 620BC first-location HUD. Special stage choices and completion UI WIP. */
GDB_CALL uint32 ff_hud_800620BC_stage0(void) {
 uint32 i,actor,name,target; sint32 hp,shown,mode,x,index;
 if(ff_u32(0x80093d58)!=0||(sint32)ff_u32(0x800941b8)>=16)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
 for(i=0;i<8;i++) {
  actor=0x800b4318+244u*i;if(ff_s16(actor+52)==-1)continue;
  hp=ff_s16(actor+66);shown=ff_s16(actor+68);
  if(hp<shown){shown=(sint16)(shown-4);ff_w16(actor+68,(uint16)shown);if(hp>=shown){ff_w16(actor+68,(uint16)hp);ff_w16(actor+70,(uint16)hp);}}
  else if(shown<hp){shown=(sint16)(shown+4);ff_w16(actor+68,(uint16)shown);ff_w16(actor+70,(uint16)hp);if(shown>=hp)ff_w16(actor+68,(uint16)hp);}
 }
 for(i=0;i<2;i++) {
  actor=0x800b4318+244u*i;if(ff_s16(actor+52)==-1)continue;
  mode=(sint32)ff_u32(0x80093d5c);
  if((i&&ff_u32(0x800940b0)!=2)||!(mode==2||(mode<2&&ff_s16(actor+66)>0))){ff_continue_hud_80061E6C(i);continue;}
  name=ff_u32(actor+240);x=i?314-FUN_8005EFB8(name):16;
  x=FUN_80061B7C(name,x,16);
  if(mode<2){if(i)x=314-FUN_8005EFB8(name)-10-FUN_8005EFB8(0x80094740);FUN_80061D0C(0x80094738+8*i,x,16);}
  if(i)FUN_8006351C(0x001a0130,actor);else FUN_80063100(0x001a0010,actor);
  hud_ammo(actor,i,126);hud_ammo(actor,i,125);
  index=*(sint8*)ff_ptr(actor+116,1);
  if(*(sint8*)ff_ptr(actor+118,1)&&index!=-1) {
   target=0x800b4318+244u*(uint32)index;
   if(ff_s16(target+66)<=0||ff_s16(target+52)==-1)ff_w8(actor+118,0);
   else {name=ff_u32(target+240);FUN_80061B7C(name,i?314-FUN_8005EFB8(name):16,36);if(i)FUN_8006351C(0x002e0130,target);else FUN_80063100(0x002e0010,target);}
  }
 }
 return ff_u32(0x800941b8)&8;
}

/* 58634 game-side tail from587A8, after host display/previous-OT submit.
 * Stage0 only; game-over text is independent from the continue state machine. */
GDB_CALL uint32 ff_prepare_scene_800587A8_stage0(void) {
 sint32 fade;
 ff_hud_800620BC_stage0();
 if((sint32)ff_u32(0x80093dd4)>=192)FUN_80061ADC(0x80093570,110);
 if(ff_s16(0x8001000c)+ff_s16(0x8001000e)+ff_s16(0x80010010))FUN_8006471C();
 if(ff_u32(0x80094190))FUN_80064820();
 ff_menu_horizon_80058844();ff_draw_stage_background();
 if((sint32)ff_u32(0x80093dd0)>=33)return 0;
 fade=(sint32)((ff_u32(0x80093dd0)<<3)-256u);return FUN_80011CCC(fade,fade,fade);
}
