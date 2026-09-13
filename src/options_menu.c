#include "ff.h"
GDB_CALL uint32 FUN_80069554(void) {
 FF_FUNCTION_MARKER(0x80069554u,"GAME.EXE");
 uint32 saved[65],buttons,text,result;int i;
 for(i=0;i<65;++i)saved[i]=ff_u32(0x801fd300+4*i);
 DrawSync(0);FUN_80056CCC(197);FUN_80061448(3,0x116000);FUN_80060394(0);FUN_800645E8();FUN_80011CCC(-256,-256,-256);FUN_800687E8(ff_u32(0x80092474));
 for(;;){
  FUN_80067E20();buttons=FUN_80068114(0);if(ff_u32(0x800940b0)==2)buttons|=FUN_80068114(1);
  FUN_8006BB44();FUN_8006B998();
  if(!FUN_8006BD9C(0x80093c00)){text=ff_u32(0x800924c4);goto prompt;}
  if((ff_u32(0x80093c00)&3)==2){
   FUN_80061800(ff_u32(0x800924bc),0x801fd300);FUN_800619E8(-1);FUN_80061ADC(ff_u32(0x8009467c),214);if(buttons&0x4000)break;
  }
  if((ff_u32(0x80093c00)&3)!=1)goto present;
  if(FUN_8006B77C(0,0x80093c04,0x000e2d40)==1){FUN_80067BAC();break;}
  if((ff_u32(0x80093c00)&3)!=1||FUN_8006B1C8(0,0x80093c04)==1)goto present;
  FUN_8006BFFC(0,0x116000,0x126000);FUN_8006BB44();FUN_8006B998();
  if(!FUN_8006BD9C(0x80093c00))ff_w32(0x80093ca0,0);
  if(ff_u32(0x80093ca0))break;text=ff_u32(0x800924c0);
prompt:
  FUN_80061800(text,0x801fd300);FUN_800619E8(-1);FUN_80061ADC(ff_u32(0x8009467c),214);if(buttons&0x4000)break;
present:
  FUN_80067358();
 }
 result=FUN_8005ED78();for(i=0;i<65;++i)ff_w32(0x801fd300+4*i,saved[i]);return result;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
GDB_CALL uint32 FUN_8006ADF8(uint32 player,uint32 score,uint32 name) {
 FF_FUNCTION_MARKER(0x8006adf8u,"GAME.EXE");
 sint32 position=0,character=2,fade;uint32 buttons,tick,result;
 FUN_80011CCC(-256,-256,-256);ff_w32(0x80093dd0,0);
 for(;;){
  tick=ff_u32(0x80093dd0);if((sint32)tick<33)FUN_8005EE58((sint32)(tick<<3));
  ff_w32(0x80093dd0,ff_u32(0x80093dd0)+1);
  ff_w8(name+(uint32)position,*(uint8*)ff_ptr(0x80093c70+(uint32)character,1));ff_w8(name+(uint32)position+1,0);
  FUN_8006A5B8();FUN_8006A870(player,score,name);buttons=FUN_80068114(player==2);
  if(buttons&0x80){--character;FUN_8004F468();}if(buttons&0x20){++character;FUN_8004F468();}
  if(position<3){if(character<0)character+=40;else if(character>=40)character-=40;}
  else {if(character<0)character+=2;else if(character>=2)character-=2;}
  if(buttons&0x4000){
   if(!character){if(position>0){--position;FUN_8004F490();}else FUN_80056670(5,223,7936);}
   else if(character==1)break;
   else {++position;if(position==3)character=1;FUN_8004F490();}
  }
  FUN_80067358();
 }
 FUN_8004F490();FUN_80067358();
 for(fade=256;fade>0;fade-=8){ff_w32(0x80093dd0,ff_u32(0x80093dd0)+1);FUN_8005EE58(fade);FUN_8006A5B8();FUN_8006A870(player,score,name);FUN_80067358();}
 result=name+(uint32)position;if(position)ff_w8(result,0);else {result=ff_u32(0x80093c6c);ff_w32(name,result);}return result;
}
GDB_CALL uint32 FUN_8006AB80(uint32 first,uint32 second) {
 FF_FUNCTION_MARKER(0x8006ab80u,"GAME.EXE");
 uint8 saved[8];uint32 player=0,buttons,tick,result,temp;sint32 index,fade;
 memcpy(saved,ff_ptr(0x801fce00,8),8);DrawSync(0);ff_w32(0x80094844,ff_u32(0x80093cfc));
 FUN_80056CCC(197);FUN_80061448(3,0x116000);FUN_80063870(24);FUN_80060394(0);FUN_800645E8();
 if(ff_u32(0x80093d5c)){
  if((sint32)second<(sint32)first)player=1;else {temp=first;first=second;second=temp;player=2;}
  index=FUN_8006AA70(first);if(index!=-1){FUN_8006ADF8(player,first,0x801fce00);ff_w32(0x80093e2c+12*(uint32)index,ff_u32(0x801fce00));}
  index=FUN_8006AA70(second);if(index!=-1){FUN_8006ADF8(3-player,second,0x801fce00);ff_w32(0x80093e2c+12*(uint32)index,ff_u32(0x801fce00));}
 }else {
  index=FUN_8006AA70(first);if(index!=-1){FUN_8006ADF8(0,first,0x801fce00);ff_w32(0x80093e2c+12*(uint32)index,ff_u32(0x801fce00));}
 }
 FUN_80011CCC(-256,-256,-256);ff_w32(0x80093dd0,0);
 for(;;){
  tick=ff_u32(0x80093dd0);if((sint32)tick<33)FUN_8005EE58((sint32)(tick<<3));ff_w32(0x80093dd0,ff_u32(0x80093dd0)+1);
  FUN_8006A5B8();FUN_8006A968();buttons=FUN_80068114(0);if((sint32)ff_u32(0x800940b0)>=2)buttons|=FUN_80068114(1);
  if(buttons&0x1000)break;FUN_80067358();
 }
 FUN_8004F490();FUN_80067358();
 for(fade=256;fade>0;fade-=8){ff_w32(0x80093dd0,ff_u32(0x80093dd0)+1);FUN_8005EE58(fade);FUN_8006A5B8();FUN_8006A968();result=FUN_80067358();}
 memcpy(ff_ptr(0x801fce00,8),saved,8);return result;
}
GDB_CALL sint32 FUN_8006A870(uint32 rank,uint32 score,uint32 name) {
 FF_FUNCTION_MARKER(0x8006a870u,"GAME.EXE");
 uint8 saved[129];sint32 width,result;uint32 text=ff_u32(rank==0?0x8009252c:(rank==1?0x80092530:0x80092534));
 memcpy(saved,ff_ptr(0x801fcd00,129),129);
 FUN_80061800(text,0x801fcd01);FUN_800619E8(32);FUN_80061800(ff_u32(0x80092538),0x801fcd01);FUN_800619E8(64);
 FUN_80061ADC(ff_u32(0x8009253c),112);FUN_80061ADC(ff_u32(0x80094694),214);
 sprintf(ff_ptr(0x801fcd01,128),ff_ptr(0x80093c64,1),score);FUN_80061ADC(0x801fcd01,88);
 width=FUN_8005EFB8(name);result=FUN_80062C0C(name,(sint32)(160u-(uint32)(width>>1)),144,0x80093be4);
 memcpy(ff_ptr(0x801fcd00,129),saved,129);return result;
}
GDB_CALL sint32 FUN_8006AA70(uint32 score) {
 FF_FUNCTION_MARKER(0x8006aa70u,"GAME.EXE");
 uint32 index=0,i,a,b,c,offset;
 while(index<10&&ff_u32(0x80093e28+12*index)>=score)++index;
 if(index==10)return -1;
 for(i=9;i>index;--i){offset=12*i;a=ff_u32(0x80093e1c+offset);b=ff_u32(0x80093e20+offset);c=ff_u32(0x80093e24+offset);
  ff_w32(0x80093e28+offset,a);ff_w32(0x80093e2c+offset,b);ff_w32(0x80093e30+offset,c);}
 offset=12*index;ff_w32(0x80093e28+offset,score);ff_w32(0x80093e30+offset,ff_u32(0x80093d68)!=0);
 /* Aligned SWL/SWR pair writes the same four-byte name, not two words. */
 ff_w32(0x80093e2c+offset,ff_u32(0x80093c6c));return (sint32)index;
}
static uint32 card_copy_scores(uint32 destination,uint32 source) {
 uint32 i,a,b,c,d;
 for(i=0;i<112;i+=16){a=ff_u32(source+i);b=ff_u32(source+i+4);c=ff_u32(source+i+8);d=ff_u32(source+i+12);
  ff_w32(destination+i,a);ff_w32(destination+i+4,b);ff_w32(destination+i+8,c);ff_w32(destination+i+12,d);}
 a=ff_u32(source+112);b=ff_u32(source+116);ff_w32(destination+112,a);ff_w32(destination+116,b);return a;
}
GDB_CALL uint32 FUN_80067BAC(void) {
 FF_FUNCTION_MARKER(0x80067bacu,"GAME.EXE");
 ff_w32(0x80093d6c,ff_u32(0xe2f60));ff_w8(0x80093d74,*(uint8*)ff_ptr(0xe2f65,1));
 ff_w8(0x80094480,*(uint8*)ff_ptr(0xe2f66,1));ff_w8(0x80094481,*(uint8*)ff_ptr(0xe2f67,1));
 ff_w16(0x80093d38,(uint16)(sint16)*(sint8*)ff_ptr(0xe2f68,1));ff_w16(0x80093d3a,(uint16)(sint16)*(sint8*)ff_ptr(0xe2f69,1));
 return card_copy_scores(0x80093e28,0xe2f6c);
}
GDB_CALL uint32 FUN_80067C84(void) {
 FF_FUNCTION_MARKER(0x80067c84u,"GAME.EXE");
 ff_w32(0xe2f60,ff_u32(0x80093d6c));ff_w8(0xe2f65,*(uint8*)ff_ptr(0x80093d74,1));
 ff_w8(0xe2f66,*(uint8*)ff_ptr(0x80094480,1));ff_w8(0xe2f67,*(uint8*)ff_ptr(0x80094481,1));
 ff_w8(0xe2f68,*(uint8*)ff_ptr(0x80093d38,1));ff_w8(0xe2f69,*(uint8*)ff_ptr(0x80093d3a,1));
 return card_copy_scores(0xe2f6c,0x80093e28);
}
static uint32 card_dialog_buttons(void) {
 uint32 buttons=FUN_80068114(0);if(ff_u32(0x800940b0)==2)buttons|=FUN_80068114(1);return buttons;
}
GDB_CALL sint32 FUN_80068180(uint32 retained_status_address) {
 FF_FUNCTION_MARKER(0x80068180u,"GAME.EXE");
 uint8 saved[257];uint32 buttons;
 /* Entry SP-118 is read even when6BD9C leaves it untouched. It is an explicit
  * input, never an uninitialized C local or an invented zero in this entry. */
 FUN_800687E8(ff_u32(0x80092474));FUN_8006BDBC(retained_status_address);
 if(ff_u32(retained_status_address)==ff_u32(0x80093c00))return 0;
 memcpy(saved,ff_ptr(0x801fd480,257),257);ff_w32(0x80093c14,1);
 do{FUN_80067E20();FUN_80061800(ff_u32(0x80092450),0x801fd480);FUN_800619E8(-1);FUN_80067358();buttons=card_dialog_buttons();}while(!(buttons&0xf000));
 FUN_8004F490();FUN_800680CC();memcpy(ff_ptr(0x801fd480,257),saved,257);return 1;
}
/* WIP native memory-card UI uses one retained stack cell, in emulated RAM
 * so checkpoints preserve it. Explicit68180 callers can supply original SP-118. */
static sint32 card_status_changed(void){return FUN_80068180(0x801fd440);}
GDB_CALL sint32 FUN_800690D8(void) {
 FF_FUNCTION_MARKER(0x800690d8u,"GAME.EXE");
 uint8 saved[513],ch;uint32 text,buttons,i=0,status; sint32 connected,result;
 memcpy(saved,ff_ptr(0x801fd800,513),513);text=ff_u32(ff_u32(0x80093d3c)?0x800924c8:0x8009245c);
 do{ch=*(uint8*)ff_ptr(text+i,1);ff_w8(0x80094628+i,ch);++i;}while(ch);
 ff_w32(0x80093ba4,1);DrawSync(0);FUN_800680CC();
 do{ff_pause_refresh_pad();buttons=card_dialog_buttons();}while(buttons);
 FUN_800687E8(ff_u32(0x80092474));ff_w8(0x80093cf0,255);FUN_8006BB44();FUN_8006B998();ff_w32(0x80093c14,0xffffffffu);
 for(;;){
reload_card:
  FUN_80067E20();FUN_8005EC90();FUN_8006BB44();status=FUN_8006BD9C(0x80093c00);buttons=card_dialog_buttons();
  if(!status||ff_u32(0x80093c00)==2){
   text=ff_u32(!status?0x800924a8:0x800924ac);FUN_80061800(text,0x801fd800);FUN_800619E8(-1);ff_w32(0x80093c14,3);
  }else{
   FUN_8006BA58(0x801fd900);FUN_80061800(0x801fd900,0x801fd800);FUN_800619E8(-1);
   connected=(sint32)ff_u32(0x8009400c);if(!connected)connected=1;
   if((ff_u32(0x80093c00)&3)==1&&*(sint8*)ff_ptr(0x80093cf0,1)>=0&&connected>=*(sint8*)ff_ptr(0x80093cf1,1))ff_w32(0x80093c14,4);else ff_w32(0x80093c14,3);
   if((buttons&0x4000)&&ff_u32(0x80093c14)==4){
    FUN_8006B998();if(FUN_8006B1C8(0,0x80093c04)){
     FUN_8004F490();FUN_800680CC();
     for(;;){
      if(card_status_changed())goto reload_card;
      FUN_800687E8(ff_u32(0x800924b0));if(FUN_8006B77C(0,0x80093c04,0xe2d40)==1)break;
      if(card_status_changed()||FUN_80068520(ff_u32(0x800924b4),0,1))goto reload_card;
     }
     FUN_80067BAC();if(!ff_u32(0x80093d3c)){
      ff_w8(0x800947d4,*(uint8*)ff_ptr(0xe2f40,1));ff_w8(0x80094816,*(uint8*)ff_ptr(0xe2f41,1));
      ff_w16(0x800b434c,(uint16)(sint16)*(sint8*)ff_ptr(0xe2f42,1));ff_w16(0x800b4440,(uint16)(sint16)*(sint8*)ff_ptr(0xe2f43,1));
      ff_w16(0x800b435a,(uint16)ff_s16(0xe2f44));ff_w16(0x800b444e,(uint16)ff_s16(0xe2f46));
      ff_w32(0x80094030,ff_u32(0xe2f48));ff_w32(0x80094034,ff_u32(0xe2f4c));ff_w32(0x800941b0,ff_u32(0xe2f50));ff_w32(0x800941b4,ff_u32(0xe2f54));
      ff_w32(0x80093d50,ff_u32(0xe2f58));ff_w8(0x80093de0,*(uint8*)ff_ptr(0xe2f5c,1));ff_w8(0x80094000,1);
     }
     ff_w32(0x80093c14,1);do{FUN_80067E20();FUN_80061ADC(ff_u32(0x800924b8),120);FUN_80067358();buttons=card_dialog_buttons();}while(!(buttons&0xf000));break;
    }
   }
  }
  if(buttons&0x1000)break;FUN_80067358();
 }
 FUN_8004F490();result=FUN_800680CC();memcpy(ff_ptr(0x801fd800,513),saved,513);return result;
}
GDB_CALL sint32 FUN_80068850(void) {
 FF_FUNCTION_MARKER(0x80068850u,"GAME.EXE");
 uint8 saved[257];uint32 buttons,palette,name,p; sint32 count,selected=0,scroll=0,i,y,last,result=2;
 if(card_status_changed())return 2;
 count=(sint32)FUN_8006BFFC(0,0x116000,0x126000);
 if(FUN_8006B1C8(0,0x80093c04))ff_w32(0x80093ca0,ff_u32(0x80093ca0)+1);
 if(ff_u32(0x80093ca0)){
  if(card_status_changed())return 2;
  if(FUN_8006B1C8(0,0x80093c04)){
   if(FUN_80068520(ff_u32(0x80092490),1,1))return 2;if(card_status_changed())return 2;
  }
  if(ff_u32(0x80093d3c)==1)FUN_80067C84();else{
   FUN_80067C84();ff_w8(0xe2f40,*(uint8*)ff_ptr(0x80093d58,1));ff_w8(0xe2f41,*(uint8*)ff_ptr(0x800940b0,1));
   ff_w8(0xe2f42,*(uint8*)ff_ptr(0x800b434c,1));ff_w8(0xe2f43,*(uint8*)ff_ptr(0x800b4440,1));
   ff_w16(0xe2f44,(uint16)ff_s16(0x800b435a));ff_w16(0xe2f46,(uint16)ff_s16(0x800b444e));
   ff_w32(0xe2f48,ff_u32(0x80094030));ff_w32(0xe2f4c,ff_u32(0x80094034));
   ff_w32(0xe2f50,ff_u32(0x800941b0));ff_w32(0xe2f54,ff_u32(0x800941b4));ff_w32(0xe2f58,ff_u32(0x80093d50));ff_w8(0xe2f5c,*(uint8*)ff_ptr(0x80093de0,1));
  }
  result=FUN_8006B22C(0,0x80093c04,0x80093c1c,0xe2d40);if(result!=1)return result;
  memset(ff_ptr(0xe0d40,8192),255,8192);result=FUN_8006B77C(0,0x80093c04,0xe0d40);if(result!=1)return result;
  return memcmp(ff_ptr(0xe2f40,164),ff_ptr(0xe0f40,164),164)==0;
 }
 if(FUN_80068520(ff_u32(0x80092488),1,1))return 2;if(card_status_changed())return 2;
 ff_w32(0x80093c14,14);last=(sint32)((uint32)count-1);memcpy(saved,ff_ptr(0x801fd600,257),257);
 for(;;){
  FUN_8006BB44();if(!FUN_8006BD9C(0x80093c00)){FUN_800680CC();break;}
  buttons=card_dialog_buttons();FUN_80067E20();FUN_80061800(ff_u32(0x8009248c),0x801fd600);FUN_800619E8(70);
  y=(sint32)(12*ff_u32(0x80094330)+80);name=0x116000+((uint32)scroll<<6);
  for(i=0;i<10&&(sint32)((uint32)i+(uint32)scroll)<count;++i,name+=64,y=(sint32)((uint32)y+10)){
   palette=selected==i?0x80093be4:0x80093bf0;ff_w8(name+23,0);
   snprintf((char*)ff_ptr(0x801fd600,257),257,"%d.",(sint32)((uint32)i+(uint32)scroll+1));
   FUN_80062C0C(0x801fd600,50-FUN_8005EFB8(0x801fd600),y,palette);FUN_80062C0C(name,40,y,palette);
  }
  FUN_80067358();
  if(buttons&0x40){if(selected<9&&scroll+selected<last){++selected;FUN_8004F468();}else if(selected==9&&scroll+9<last){++scroll;FUN_8004F468();}}
  if(buttons&0x10){if(selected>0){--selected;FUN_8004F468();}else if(!selected&&scroll>0){--scroll;FUN_8004F468();}}
  if(buttons&0x1000){FUN_8004F490();FUN_800680CC();break;}if(!(buttons&0x4000))continue;
  FUN_8004F490();if(FUN_80068520(ff_u32(0x80092460),1,1))break;
  FUN_800680CC();p=0x126000+40u*((uint32)scroll+(uint32)selected);if(!card_status_changed())FUN_8006B710(0,p);break;
 }
 memcpy(ff_ptr(0x801fd600,257),saved,257);return 2;
}
GDB_CALL sint32 FUN_80068D68(void) {
 FF_FUNCTION_MARKER(0x80068d68u,"GAME.EXE");
 uint8 saved[513];uint32 buttons; sint32 state,result;
 memcpy(saved,ff_ptr(0x801fca00,513),513);
 if(ff_u32(0x80093d3c)==1){strcpy(ff_ptr(0x80094628,1),ff_ptr(ff_u32(0x800924cc),1));ff_w32(0x80093ba4,1);}
 else {
  strcpy(ff_ptr(0x80094628,1),ff_ptr(ff_u32(0x80092458),1));ff_w32(0x80093ba4,0);DrawSync(0);
  ff_w32(0x80094844,ff_u32(0x80093cfc));FUN_80056CCC(197);FUN_80061448(3,0x116000);FUN_80060394(0);
  FUN_800645E8();FUN_80011CCC(-256,-256,-256);
  if(FUN_80068520(ff_u32(0x80092494),1,0)){result=-1;goto finished_save_dialog;}
 }
 ff_w8(0x80093cf0,255);
 if(ff_u32(0x80093d3c)==1){memset(ff_ptr(0xe2f40,164),0,164);ff_w8(0xe2f41,1);ff_w16(0xe2f44,380);}
 ff_w32(0x80093c14,0xffffffff);
 for(;;){
  buttons=card_dialog_buttons();FUN_8006BB44();state=(sint32)FUN_8006BD9C(0x80093c00);FUN_80067E20();
  if(!state){FUN_80061800(ff_u32(0x80092498),0x801fca01);FUN_800619E8(-1);ff_w32(0x80093c14,3);}
  else {
   if(ff_u32(0x80093c00)==2){FUN_80061800(ff_u32(0x8009249c),0x801fca01);FUN_800619E8(-1);ff_w32(0x80093c14,5);}
   else {ff_w32(0x80093c14,6);FUN_8006BA58(0x801fcb01);FUN_80061800(0x801fcb01,0x801fca01);FUN_800619E8(-1);}
   if(buttons&0x4000){
    FUN_8004F490();FUN_8004F490();FUN_800680CC();
    if((ff_u32(0x80093c00)&3)!=1){FUN_80068258();continue;}
    for(;;){state=FUN_80068850();if(state>0)break;if(FUN_80068520(ff_u32(0x800924a0),0,1)){state=2;break;}}
    if(state!=1)continue;
    ff_w32(0x80093c14,1);
    do{FUN_80067E20();FUN_80061ADC(ff_u32(0x800924a4),120);FUN_80067358();}while(!(card_dialog_buttons()&0xf000));
    FUN_8004F490();result=FUN_800680CC();break;
   }
  }
  FUN_80067358();
  if(buttons&0x1000){FUN_8004F490();if(ff_u32(0x80093d3c)==1){result=1;break;}
   result=FUN_80068520(ff_u32(0x80092494),1,0);if(result)break;}
 }
finished_save_dialog:
 memcpy(ff_ptr(0x801fca00,513),saved,513);return result;
}
static void card_dialog_choices(sint32 choice) {
 FUN_80062C0C(ff_u32(0x800924d8),110,170,choice?0x80093bf0:0x80093be4);
 FUN_80062C0C(ff_u32(0x800924dc),170,170,choice?0x80093be4:0x80093bf0);
}
GDB_CALL sint32 FUN_80068520(uint32 text,sint32 choice,sint32 monitor_card) {
 FF_FUNCTION_MARKER(0x80068520u,"GAME.EXE");
 uint8 saved[257];uint32 buttons; sint32 result;memcpy(saved,ff_ptr(0x801fc800,257),257);
 ff_w32(0x80093c14,0);
 for(;;){
  buttons=card_dialog_buttons();FUN_8006BB44();result=(sint32)FUN_8006BD9C(0x80093c00);
  if(monitor_card==1&&!result){FUN_800680CC();result=1;break;}
  FUN_80067E20();FUN_80061800(text,0x801fc801);FUN_800619E8(-1);card_dialog_choices(choice);FUN_80067358();
  if(buttons&0x80){choice=0;FUN_8004F468();}if(buttons&0x20){choice=1;FUN_8004F468();}
  if(buttons&0x4000){FUN_8004F490();FUN_800680CC();result=choice;break;}
  if(buttons&0x1000){FUN_8004F490();FUN_800680CC();result=1;break;}
 }
 memcpy(ff_ptr(0x801fc800,257),saved,257);return result;
}
GDB_CALL sint32 FUN_80068258(void) {
 FF_FUNCTION_MARKER(0x80068258u,"GAME.EXE");
 uint8 saved[257];uint32 buttons,prior,timer; sint32 choice=1,result=-1;
 memcpy(saved,ff_ptr(0x801fc800,257),257);
 for(;;){
  ff_w32(0x80093c14,0);buttons=card_dialog_buttons();FUN_80067E20();
  FUN_80061800(ff_u32(0x80092454),0x801fc801);FUN_800619E8(-1);card_dialog_choices(choice);FUN_80067358();
  if(buttons&0x80){choice=0;FUN_8004F468();}if(buttons&0x20){choice=1;FUN_8004F468();}
  if(buttons&0x4000)break;
  if(buttons&0x1000){FUN_8004F490();FUN_800680CC();goto finished;}
 }
 FUN_8004F490();FUN_800680CC();if(choice==1)goto finished;
 prior=ff_u32(0x80093c00);ff_w32(0x80093c14,0xffffffff);
 FUN_800687E8(ff_u32(0x80092474));FUN_8006BDBC(0x80093c00);
 if(prior!=ff_u32(0x80093c00))goto finished;
 FUN_800687E8(ff_u32(0x80092478));FUN_8006B6CC(0);FUN_8006BDBC(0x80093c00);
 ff_w32(0x80093c14,1);timer=0;
 if((ff_u32(0x80093c00)&3)!=1){
  do{
   FUN_80067E20();FUN_80061800(ff_u32(0x8009247c),0x801fc801);FUN_800619E8(-1);FUN_80067358();
   buttons=card_dialog_buttons();++timer;
  }while(!(buttons&0xf000)&&timer<150);
  FUN_800680CC();
 }else{
  do{
   FUN_80067E20();FUN_80061ADC(ff_u32(0x80092480),120);FUN_80067358();
   buttons=card_dialog_buttons();if(buttons&0xf000)break;++timer;
  }while(timer<150);
  FUN_800680CC();result=0;
 }
finished:
 memcpy(ff_ptr(0x801fc800,257),saved,257);return result;
}
GDB_CALL sint32 FUN_800686A8(uint32 text) {
 FF_FUNCTION_MARKER(0x800686a8u,"GAME.EXE");
 uint32 i,buttons;DrawSync(0);FUN_80061448(3,0x116000);FUN_80060394(0);FUN_800645E8();
 FUN_80011CCC(-256,-256,-256);ff_w32(0x80093c14,1);
 for(i=0;i<2;++i){FUN_80067E20();FUN_80061ADC(text,125);FUN_80067358();}
 do{ff_pause_refresh_pad();buttons=card_dialog_buttons();}while(!(buttons&0x5000));
 FUN_800680CC();ff_w32(0x80093c14,0xffffffff);return -1;
}
/* WIP native stack adapter for the 256-byte text workspace. The byte before
 * the buffer is addressable because 618DC can terminate an overlong first word. */
static sint32 draw_wrapped_dialog(uint32 text,int reload_text) {
 uint8 saved[257];uint32 i;uint8 *workspace=ff_ptr(0x801fc440,257);
 memcpy(saved,workspace,sizeof(saved));ff_w32(0x80093c14,0xffffffff);
 for(i=0;i<2;++i){
  FUN_80067E20();FUN_80061800(reload_text?ff_u32(0x80092484):text,0x801fc441);
  FUN_800619E8(-1);FUN_80067358();
 }
 memcpy(workspace,saved,sizeof(saved));return 0;
}
GDB_CALL sint32 FUN_80068788(void) {
 FF_FUNCTION_MARKER(0x80068788u,"GAME.EXE");
 return draw_wrapped_dialog(0,1);
}
GDB_CALL sint32 FUN_800687E8(uint32 text) {
 FF_FUNCTION_MARKER(0x800687e8u,"GAME.EXE");
 return draw_wrapped_dialog(text,0);
}
/* 8006A968: ten 12-byte score records. Width precedes optional '?' suffix. */
GDB_CALL sint32 FUN_8006A968(void) {
 uint8 saved[32],*buffer=ff_ptr(0x801fc100,32);int i,width;
 memcpy(saved,buffer,32);FUN_80061ADC(ff_u32(0x80092528),32);
 for(i=0;i<10;i++) {
  FUN_80061B7C(0x80093e2c+12u*i,70,56+14*i);
  snprintf((char*)buffer,32,(char*)ff_ptr(0x80093c64,1),ff_u32(0x80093e28+12u*i));width=FUN_8005EFB8(0x801fc100);
  if(ff_u32(0x80093e30+12u*i)&1)strcat((char*)buffer,(char*)ff_ptr(0x80093c68,1));
  FUN_80061B7C(0x801fc100,250-width,56+14*i);
 }
 memcpy(buffer,saved,32);return FUN_80061ADC(ff_u32(0x80094684),214);
}
/* 80052624: draw before input; use existing controller count without refresh. */
GDB_CALL sint32 FUN_80052624(void) {
 int i,result;FUN_8006A968();result=(sint32)ff_u32(0x800940b0);
 for(i=0;i<(sint32)ff_u32(0x800940b0);i++) {
  uint32 raw=(uint32)FUN_8005F5AC((sint32)ff_u32(0x80094288+4u*i)),edge=(raw^ff_u32(0x80092790+4u*i))&raw;
  ff_w32(0x80092798+4u*i,edge);ff_w32(0x80092790+4u*i,raw);ff_w32(0x800927a0,raw);
  if(edge&0x1000){ff_w32(0x800927ec,0x80050d98);return FUN_8004F490();}
  result=i+1<(sint32)ff_u32(0x800940b0);
 }
 return result;
}
/* 800519D4: display both player headings, then each connected player's scheme.
 * Preserve redundant decoder calls: they update disconnect/reset state. */
GDB_CALL sint32 FUN_800519D4(void) {
 uint8 saved[256],*buffer=ff_ptr(0x801fc100,256);int player,k,x,y;uint32 edge;
 FUN_8005EC90();ff_w32(0x800940b0,ff_u32(0x8009400c));
 if(!ff_u32(0x8009400c))return FUN_8004F4F8(98);
 memcpy(saved,buffer,256);FUN_80061ADC(ff_u32(0x80092500),20);
 for(player=0,x=32;player<2;player++,x+=150) {
  snprintf((char*)buffer,256,(char*)ff_ptr(0x800928dc,1),(char*)ff_ptr(ff_u32(0x80092504),1),player+1);
  FUN_80061B7C(0x801fc100,x,60);FUN_8005F5AC(player);
  if(*(sint8*)ff_ptr(0x80094114,1)!=-1) {
   snprintf((char*)buffer,256,(char*)ff_ptr(0x800928dc,1),(char*)ff_ptr(ff_u32(0x80092508),1),(sint32)*(sint8*)ff_ptr(0x80094480+player,1)+1);
   FUN_80061B7C(0x801fc100,x-10,70);
   for(k=0,y=90;k<6;k++,y+=16) {
    sint32 scheme=*(sint8*)ff_ptr(0x80094480+player,1),action=*(sint8*)ff_ptr(0x80093d78+6u*(uint32)scheme+k,1);
    snprintf((char*)buffer,256,(char*)ff_ptr(0x800928e4,1),(sint32)*(sint8*)ff_ptr(0x800928d4+k,1),(char*)ff_ptr(ff_u32(0x80094458+4u*(uint32)action),1));
    FUN_80061B7C(0x801fc100,x-16,y);
    if(action==3){if(ff_u32(0x80093d70)==2){y+=10;FUN_80061B7C(0x800928ec,x+65,y);}if(ff_u32(0x80093d70)==1){y+=10;FUN_80061B7C(0x800928f4,x+25,y);}}
   }
  }
 }
 memcpy(buffer,saved,256);
 for(player=0;player<2;player++) {
  FUN_8005F5AC(player);
  if(*(sint8*)ff_ptr(0x80094114,1)!=-1) {
   uint8 *scheme=ff_ptr(0x80094480+player,1);edge=FUN_80068114(player);
   if(edge&0x80){FUN_8004F490();*scheme=(uint8)(*scheme-1);}
   if(edge&0x20){FUN_8004F490();*scheme=(uint8)(*scheme+1);}
   *scheme&=7;
   if(edge&0x1000){FUN_8004F490();ff_w32(0x800927ec,0x80050d98);}
  }
 }
 return FUN_80061ADC(ff_u32(0x800946a8),214);
}
/* 800575F8: copy screen origin to both display environments. Return LHU Y. */
GDB_CALL uint32 FUN_800575F8(void) {
 uint16 x=(uint16)ff_s16(0x80093d38),y=(uint16)ff_s16(0x80093d3a);
 ff_w16(0x800947a8,x);ff_w16(0x800947bc,x);ff_w16(0x800947aa,y);ff_w16(0x800947be,y);return y;
}
/* 80051C98: adjustment edges from either pad, in left/right/up/down order.
 * The inclusive left guard permits -10 -> -11, as in actual MIPS. */
GDB_CALL sint32 FUN_80051C98(void) {
 uint32 edge;FUN_8005EC90();ff_w32(0x800940b0,ff_u32(0x8009400c));
 if(!ff_u32(0x8009400c))return FUN_8004F4F8(98);
 FUN_80061ADC(ff_u32(0x8009250c),20);edge=FUN_80068114(0);
 if(ff_u32(0x800940b0)==2)edge|=FUN_80068114(1);
 if((edge&0x80)&&ff_s16(0x80093d38)>=-10)ff_w16(0x80093d38,(uint16)(ff_s16(0x80093d38)-1));
 if((edge&0x20)&&ff_s16(0x80093d38)<16)ff_w16(0x80093d38,(uint16)(ff_s16(0x80093d38)+1));
 if((edge&0x10)&&ff_s16(0x80093d3a)>0)ff_w16(0x80093d3a,(uint16)(ff_s16(0x80093d3a)-1));
 if((edge&0x40)&&ff_s16(0x80093d3a)<16)ff_w16(0x80093d3a,(uint16)(ff_s16(0x80093d3a)+1));
 FUN_800575F8();
 if(edge&0x1000){FUN_8004F490();ff_w32(0x800927ec,0x80050d98);}
 return FUN_80061ADC(ff_u32(0x800946ac),214);
}
static sint32 aux(uint32 function,sint32 a,sint32 b){if(!ff_services.menu_aux)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);return ff_services.menu_aux(function,a,b);}
static void home(void){uint32 v[3];int i;ff_w32(0x800927ec,0x80050270);for(i=0;i<3;i++)v[i]=ff_u32(0x80092720+4u*i);FUN_8004F980(v,0,-12032);}
static int isrow(uint32 address){return ff_u32(0x80092850)==ff_u32(address);}
/* GAME:80050D98. Original stack text/colors are marshalled into a temporary RAM
 * region for the numeric-pointer text ABI, then restored before input handling. */
GDB_CALL sint32 FUN_80050D98(void) {
 uint8 saved[280],*scratch=ff_ptr(0x801fc100,280);uint32 phase=ff_u32(0x8009284c),n,edge,raw;int i,k,y,width=0,result;
 const uint32 phases[4]={0,512,1536,1024};
 memcpy(saved,scratch,280);memcpy(scratch+256,ff_ptr(0x8009288c,12),12);memcpy(scratch+268,ff_ptr(0x80092898,12),12);
 if(ff_u32(0x80093d68)&2){for(i=0;i<13;i++)ff_w32(0x80092854+4u*i,i);n=13;}
 else {ff_w32(0x80092854,999);ff_w32(0x80092858,999);for(i=2;i<13;i++)ff_w32(0x80092854+4u*i,i-2);n=11;}
 ff_w32(0x80092888,n);
 for(i=0;i<4;i++){uint32 sample=i?(phase+phases[i])&2047:phase;uint8 c=(uint8)(ff_s16(0x800802d0+2*sample)>>11);for(k=0;k<3;k++)scratch[256+3*i+k]=c;}
 ff_w32(0x8009284c,(phase+32)&2047);FUN_8005EC90();ff_w32(0x800940b0,ff_u32(0x8009400c));
 if(!ff_u32(0x8009400c)){memcpy(scratch,saved,280);return FUN_8004F4F8(98);}
 FUN_80061ADC(ff_u32(0x800924f8),20);
 FUN_80061ADC(ff_u32((isrow(0x8009286c)||isrow(0x80092870)||isrow(0x80092874)||isrow(0x80092878)||isrow(0x8009287c)||isrow(0x80092880)||isrow(0x80092884))?0x800946a0:0x800946a4),214);
 y=128-6*(int)n;if(ff_u32(0x80093d70)==2)y-=6;
 for(i=0;i<(sint32)ff_u32(0x80092888);i++) {
  uint32 text=0,row=(uint32)i;int slider=0;
  if(row==ff_u32(0x80092854))text=ff_u32(0x8007f79c+4*ff_u32(0x800927b4));
  else if(row==ff_u32(0x80092858))text=ff_u32(0x80093d68)&1?0x800928a4:0x800928b8;
  else if(row==ff_u32(0x8009285c)) {
   uint32 a=ff_u32(0x800924fc),b=ff_u32(0x800947e0+4u*(uint32)(sint32)*(sint8*)ff_ptr(0x80093d74,1));
   snprintf((char*)scratch,256,(char*)ff_ptr(0x800928cc,1),(char*)ff_ptr(a,1),(char*)ff_ptr(b,1));text=0x801fc100;
  }else if(row==ff_u32(0x80092864)) {
   text=ff_u32(0x8009251c);width=FUN_8005EFB8(text)+64;slider=1;
   if(ff_u32(0x80093d70)==2){FUN_80062AB4(240,y+12,*(uint8*)ff_ptr(0x80093d6e,1));width-=64;}
   else FUN_80062AB4((width>>1)+96,y+1,*(uint8*)ff_ptr(0x80093d6e,1));
  }else if(row==ff_u32(0x80092860)) {
   text=ff_u32(0x80092518);width=FUN_8005EFB8(text)+64;slider=1;FUN_80062AB4((width>>1)+96,y+2,*(uint8*)ff_ptr(0x80093d6f,1));
  }else if(row==ff_u32(0x80092868))text=ff_u32(0x80093d6c)&1?0x80092540:0x80092548;
  else {const uint32 labels[7]={0x80092500,0x80092520,0x800924c8,0x800924cc,0x8009250c,0x80092528,0x80092524};for(k=0;k<7;k++)if(row==ff_u32(0x8009286c+4u*k))text=ff_u32(labels[k]);}
  if(!slider&&text)width=FUN_8005EFB8(text);
  FUN_80062C0C(text,160-(width>>1),y,ff_u32(0x80092850)==row?0x801fc200:0x801fc20c);y+=12;
  if(row==ff_u32(0x80092864)&&ff_u32(0x80093d70)==2)y+=6;
 }
 memcpy(scratch,saved,280);result=(sint32)ff_u32(0x800940b0);
 for(i=0;i<(sint32)ff_u32(0x800940b0);i++) {
  raw=(uint32)FUN_8005F5AC((sint32)ff_u32(0x80094288+4u*i));edge=(raw^ff_u32(0x80092790+4u*i))&raw;
  ff_w32(0x80092798+4u*i,edge);ff_w32(0x80092790+4u*i,raw);ff_w32(0x800927a0,raw);
  if(edge&0x1000){home();return FUN_8004F490();}
  if(edge&0x10){FUN_8004F468();ff_w32(0x80092850,ff_u32(0x80092850)-1);if((sint32)ff_u32(0x80092850)<0)ff_w32(0x80092850,ff_u32(0x80092888)-1);}
  if(edge&0x40){FUN_8004F468();ff_w32(0x80092850,ff_u32(0x80092850)+1);if((sint32)(ff_u32(0x80092888)-1)<(sint32)ff_u32(0x80092850))ff_w32(0x80092850,0);}
  if(isrow(0x80092854)) {
   if((edge&0x80)&&(sint32)ff_u32(0x800927b4)>0)ff_w32(0x800927b4,ff_u32(0x800927b4)-1);
   if((edge&0x20)&&(sint32)ff_u32(0x800927b4)<25)ff_w32(0x800927b4,ff_u32(0x800927b4)+1);
  }else if(isrow(0x80092858)) {
   if(edge&0x80){ff_w32(0x80093d68,ff_u32(0x80093d68)&~1u);FUN_8004F490();}
   if(edge&0x20){ff_w32(0x80093d68,ff_u32(0x80093d68)|1u);FUN_8004F490();}
  }else if(isrow(0x8009285c)) {
   sint8 *v=ff_ptr(0x80093d74,1);if((edge&0x80)&&*v>0){--*v;FUN_8004F490();}if((edge&0x20)&&*v<2){++*v;FUN_8004F490();}
  }else if(isrow(0x80092860)||isrow(0x80092864)) {
   int music=isrow(0x80092860),v=*(uint8*)ff_ptr(music?0x80093d6f:0x80093d6e,1);
   if(raw&0x80)v-=2;if(raw&0x20)v+=2;if(v<0)v=0;if(v>255)v=255;
   if(music){aux(0x800569bc,v,v);ff_w32(0x80093d6c,(ff_u32(0x80093d6c)&0xffffff)|((uint32)v<<24));}
   else {ff_w32(0x80093d6c,(ff_u32(0x80093d6c)&0xff00ffff)|((uint32)v<<16));FUN_80056F08((sint32)ff_u32(0x80092750),128);}
  }else if(isrow(0x80092868)) {
   if((edge&0x80)&&(ff_u32(0x80093d6c)&1)){ff_w32(0x80093d6c,ff_u32(0x80093d6c)&~1u);FUN_8004F490();}
   if((edge&0x20)&&!(ff_u32(0x80093d6c)&1)){ff_w32(0x80093d6c,ff_u32(0x80093d6c)|1u);FUN_8004F490();}
  }else if(edge&0x4000) {
   if(isrow(0x8009286c)){FUN_8004F490();ff_w32(0x800927ec,0x800519d4);}
   else if(isrow(0x8009287c)){FUN_8004F490();ff_w32(0x800927ec,0x80051c98);}
   else if(isrow(0x80092870)||isrow(0x80092874)||isrow(0x80092878)){uint32 f=isrow(0x80092870)?0x80052428:isrow(0x80092874)?0x80052458:0x80052484;FUN_8004F490();aux(f,0,0);}
   else if(isrow(0x80092880)){ff_w32(0x800927ec,0x80052624);FUN_8004F490();}
   else if(isrow(0x80092884)){home();FUN_8004F490();}
  }
  result=i+1<(sint32)ff_u32(0x800940b0);
 }
 return result;
}
