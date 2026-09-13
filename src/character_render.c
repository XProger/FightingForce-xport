#include "ff.h"
#include <stdlib.h>
#include <stdio.h>
/* 5C740: object-local transform, with wrapped 32-bit camera subtraction. */
GDB_CALL void FUN_8005C740(uint32 object) {
 uint32 type=ff_u32(object);sint32 angle;
 FUN_800100C4((sint32)(ff_u32(object+4)-ff_u32(0x8008d4a4)),
  (sint32)(ff_u32(object+8)+ff_u32(0x800990d8+4u*type)-ff_u32(0x8008d4a8)),
  (sint32)(ff_u32(object+12)-ff_u32(0x8008d4ac)));
 FUN_800101CC(32768);
 angle=ff_s16(object+16);if(angle)FUN_8001033C(angle);
 angle=ff_s16(object+18);if(angle)FUN_800101CC(angle);
 angle=ff_s16(object+20);if(angle)FUN_800104A0(angle);
}
/* 5C808: complete type dispatch; unknown kinds are original no-draw cases. */
GDB_CALL uint32 FUN_8005C808(uint32 object) {
 uint32 type=ff_u32(object),temp=0x801fc0b0,shade,table=0; sint32 position[4],age,sample=0;int i;
 FUN_80010040();FUN_8005C740(object);FUN_800120BC(0,0,0,temp);
 if(ff_u32(temp+12))return FUN_80010028();
 for(i=0;i<4;i++)position[i]=(sint32)ff_u32(temp+4u*i);
 if(type-862u<3) {
  position[0]=(sint32)((uint32)position[0]-12u);position[1]=(sint32)((uint32)position[1]-8u);
  FUN_80012390(position,ff_s16(0x8009a628+2u*type));return FUN_80010028();
 }
 if(type==869)return FUN_80010028(); /* Original stores size but never draws. */
 if(type==881) {
  ff_w16(object+22,(uint16)((uint16)ff_s16(object+22)+1u));
  if(ff_s16(object+22)==7)ff_w32(object,0xffffffffu);return FUN_80010028();
 }
 age=ff_s16(object+22);
 switch(type) {
 case 870:
  shade=32u-((uint32)age<<1);position[3]=384;
  ff_w32(0x8008d4bc,0x2c000000u+(shade<<16)+(shade<<8)+shade);table=0x8009ad0a;sample=age;break;
 case 871:case 872:
  shade=32u-(uint32)age;position[3]=512;
  ff_w32(0x8008d4bc,type==871?0x2c000000u+(shade<<16)+(shade<<8)+shade:0x2c000000u+(shade<<8));
  table=0x8009ad0a;sample=age>>1;break;
 case 879:position[3]=512;table=0x8009ad94;sample=age;break;
 case 873:case 874:case 880:
  position[3]=512;ff_w32(0x8008d4bc,0x2c303030);
  table=type==880?0x8009ad9c:0x8009ad78;sample=age>>(type==874?2:1);break;
 case 877:
  position[2]=(sint32)((uint32)position[2]-16u);position[3]=(sint32)ff_u32(0x8008010c+4u*(uint32)age);
  ff_w32(0x8008d4bc,0x2c608080);table=0x8009adce;sample=age;break;
 case 865:case 866:position[3]=256;table=type==865?0x8009acd6:0x8009ad4c;sample=age;break;
 case 575:
  shade=64u-((uint32)age<<2);ff_w32(0x8008d4bc,0x2c000000u+(shade<<14)+(shade<<7)+shade);
  position[3]=(sint32)(((uint32)age<<5)+256u);table=0x8009ad5a;break;
 case 576:
  shade=32u-(uint32)(age>>1);ff_w32(0x8008d4bc,0x2c000000u+(shade<<16)+(shade<<8)+shade);
  position[3]=(sint32)(((uint32)age<<5)+256u);table=0x8009ad76;break;
 default:return FUN_80010028();
 }
 /* 5CC94 repeats type879, unreachable after its earlier5C9D8 match. */
 FUN_80012594(position,ff_s16(table+2u*(uint32)sample));return FUN_80010028();
}
GDB_CALL uint32 FUN_8005D820(uint32 object) {
 uint32 type=ff_u32(object),a,b;sint32 flags;
 FUN_80010040();FUN_8005C740(object);
 flags=ff_s16(object+22);
 if(flags<0&&(flags&2)){return FUN_80010028();}
 if(type-496u<2u&&flags>0)FUN_8002BDD0(object);
 if(!ff_u32(object+8)&&!ff_s16(object+24)) {
  if(type==477)FUN_800100C4(0,0,-3670016);
  if(type==490||type==683)FUN_800100C4(0,0,-2621440);
  if(type==508)FUN_800100C4(0,2621440,0);
  if(type==515)FUN_800100C4(0,2621440,-2097152);
  a=ff_u32(0x80094550);b=ff_u32(0x80094554);
  if(((a!=0xffffffffu&&0x8009a3c8+36u*a==object)||(b!=0xffffffffu&&0x8009a3c8+36u*b==object))
   &&!*(uint8*)ff_ptr(0x80093dd8,1)&&!(ff_u32(0x8008d4d4)&31))ff_w32(0x8008d4bc,0x2cffffff);
  ff_static_model_80010B18(ff_u32(0x80081720+4u*type),0,0);
 } else {
  ff_static_model_80010B18(ff_u32(0x80081720+4u*type),0,0);
  if(type==683&&ff_s16(object+28)>0) {
   ff_w16(object+28,(uint16)(ff_s16(object+28)-1));FUN_800104A0(8192);FUN_800100C4(0,0,9437184);
   ff_static_model_80010B18(ff_u32(0x800821d4+4u*(uint32)(sint32)ff_s16(object+28)),0,0);
  }
 }
 return FUN_80010028();
}
/* Exact twins 5DA8C/5DAAC share their sole callee, not separate translations. */
GDB_CALL uint32 FUN_8005DA8C(uint32 object){return FUN_8005D820(object);}
GDB_CALL uint32 FUN_8005DAAC(uint32 object){return FUN_8005D820(object);}
/* Actual 10F94 uses the low32 product before SRA14. */
GDB_CALL void FUN_80010F94(sint32 x,sint32 y,sint32 z) {
 uint32 matrix=ff_u32(0x8008d494),i;uint32 scale[3]={(uint32)x,(uint32)y,(uint32)z};
 for(i=0;i<3;i++,matrix+=6) {
  sint16 a=ff_s16(matrix),b=ff_s16(matrix+2),c=ff_s16(matrix+4);
  ff_w16(matrix,(uint16)((sint32)(scale[0]*(uint32)(sint32)a)>>14));
  ff_w16(matrix+2,(uint16)((sint32)(scale[1]*(uint32)(sint32)b)>>14));
  ff_w16(matrix+4,(uint16)((sint32)(scale[2]*(uint32)(sint32)c)>>14));
 }
}
/* 5FB44: continue prompt, fade, countdown and one/two-controller selection. */
GDB_CALL sint32 FUN_8005FB44(void) {
 uint32 value=ff_u32(0x80093dd4),step,text=0x801fc200,buttons,palette;
 sint32 red=0,green,timer,width,x;
 if((sint32)value<=0)return (sint32)value;
 step=value-128u;if((sint32)step<0)step=0;
 green=(sint32)(0u-(step<<1));if(green<-256)green=-256;
 if((sint32)step>=65){red=(sint32)(0u-((step-64u)<<1));if(red<-256)red=-256;}
 FUN_80011CCC(red,green,green);FUN_80011FEC();
 if((sint32)ff_u32(0x80093d50)<(sint32)ff_u32(0x800940b0)){ff_w32(0x80093d4c,329);goto advance;}
 if(ff_u32(0x80093dd4)!=191)goto advance;
 FUN_80067D3C();
 sprintf((char*)ff_ptr(text,128),"%s ?",(const char*)ff_ptr(ff_u32(0x800924ec),1));FUN_80061ADC(text,100);
 timer=(sint32)ff_u32(0x80093d4c);sprintf((char*)ff_ptr(text,128),"%d",(sint32)(10u-(uint32)(timer/30)));
 timer=(sint32)ff_u32(0x80093d4c);if(timer%30<4)FUN_80061B28(text,130);else FUN_80061ADC(text,130);
 if(ff_u32(0x80093d70)==2)sprintf((char*)ff_ptr(text,128),"ANZAHL DER CONTINUES = %d",(sint32)ff_u32(0x80093d50));
 else sprintf((char*)ff_ptr(text,128),"%sS = %d",(const char*)ff_ptr(ff_u32(0x800924ec),1),(sint32)ff_u32(0x80093d50));
 FUN_80061ADC(text,190);
 width=FUN_8005EFB8(ff_u32(0x800924dc));x=FUN_8005EFB8(ff_u32(0x800924d8));x=(sint32)(320u-(uint32)width-(uint32)x)>>1;
 palette=*(sint8*)ff_ptr(0x80093d48,1)?0x80093bf0:0x80093be4;FUN_80062C0C(ff_u32(0x800924d8),x,160,palette);
 palette=*(sint8*)ff_ptr(0x80093d48,1)==1?0x80093be4:0x80093bf0;width=FUN_8005EFB8(ff_u32(0x800924d8));
 FUN_80062C0C(ff_u32(0x800924dc),(sint32)((uint32)x+(uint32)width+10u),160,palette);
 buttons=(uint32)FUN_8005F5AC((sint32)ff_u32(0x80094288));
 if(ff_u32(0x800940b0)==2)buttons|=(uint32)FUN_8005F5AC((sint32)ff_u32(0x8009428c));
 if(buttons&128)ff_w8(0x80093d48,0);if(buttons&32)ff_w8(0x80093d48,1);
 value=ff_u32(0x80093d4c)+1u;ff_w32(0x80093d4c,value);
 if(value==329)ff_w32(0x80093dd4,ff_u32(0x80093dd4)+2u);
 if(!(buttons&16384))return 1;
 if(*(sint8*)ff_ptr(0x80093d48,1)!=1){ff_w32(0x80093dd4,384);return 384;}
 ff_w32(0x80093d4c,329);ff_w32(0x80093d50,0);
advance:
 value=ff_u32(0x80093dd4)+2u;ff_w32(0x80093dd4,value);return (sint32)value;
}
/* Retain the existing platform/debug symbol while routing the complete body. */
GDB_CALL sint32 ff_game_over_8005FB44_live(void) {return FUN_8005FB44();}
static void actor_color(uint32 actor,int player) {
 sint32 r=128+*(sint8*)ff_ptr(actor+157,1),g=128+*(sint8*)ff_ptr(actor+158,1),b=128+*(sint8*)ff_ptr(actor+159,1);
 uint8 flags=*(uint8*)ff_ptr(actor+156,1);uint32 color=0x2c000000;
 if(flags&2){color=0x2e000000;ff_w32(0x8008d4c0,flags>>4);}
 if(flags&4) {
  r+=player?96:128;g+=player?96:128;b+=player?96:128;
  if(r>=256)r=255;if(g>=256)g=255;if(b>=256)b=255;ff_w8(actor+156,flags&251);
 }
 ff_w32(0x8008d4bc,color|(uint32)r|((uint32)g<<8)|((uint32)b<<16));
}
/* 58D9C: item correction rotations in Z/Y/X order, then signed fixed translation. */
GDB_CALL uint32 FUN_80058D9C(uint32 correction) {
 sint32 angle=ff_s16(correction+2);if(angle)FUN_800104A0(angle);
 angle=ff_s16(correction+4);if(angle)FUN_8001033C(angle);
 angle=ff_s16(correction);if(angle)FUN_800101CC(angle);
 return FUN_800100C4((sint32)((uint32)(sint32)ff_s16(correction+6)<<16),
  (sint32)((uint32)(sint32)ff_s16(correction+8)<<16),(sint32)((uint32)(sint32)ff_s16(correction+10)<<16));
}
/* Player right-hand path59E78..5A05C. NPC/left-hand paths remain explicit WIP. */
static void player_right_item(uint32 actor,uint32 index,uint32 color) {
 sint32 slot=*(sint8*)ff_ptr(actor+126,1),type;uint32 item,kind;
 if(slot==-1)return;
 item=0x8009a3c8+36u*(uint32)slot;kind=ff_u32(item);
 if(kind==0xffffffffu){ff_w8(actor+126,255);return;}
 FUN_80011F74(0x800ba234+504u*index);FUN_800100C4(0,-786432,0);FUN_8001033C(6144);
 type=ff_s16(actor+52);
 if(type==1){
  if(kind==506){FUN_800100C4(0,0,-786432);FUN_800101CC(-1536);FUN_8001033C(-2048);}
  else {FUN_8001033C(61440);FUN_800104A0(4096);}
 }
 type=ff_s16(actor+52);
 if(type==2){
  if(kind==506)FUN_800101CC(-2560);
  else {if(kind==515){FUN_800100C4(0,0,131072);FUN_800101CC(64512);FUN_8001033C(10240);}else FUN_8001033C(61440);FUN_800104A0(64512);}
 }
 if(ff_s16(actor+52)==3){
  if(kind==506){FUN_800100C4(0,0,-786432);FUN_8001033C(-3072);FUN_800101CC(-1024);}
  else {FUN_8001033C(61440);FUN_800104A0(4096);}
 }
 ff_w32(item+4,ff_u32(actor+40));ff_w32(item+12,ff_u32(actor+48));
 FUN_80058D9C(0x8009d208+12u*kind);ff_w32(0x8008d4bc,color);
 ff_static_model_80010B18(ff_u32(0x80081720+4u*kind),0,0);
 if(kind-496u<2u&&ff_s16(item+22)>0)FUN_8002BDD0(item);
}
/* NPC right hand and both left hands share the unadjusted bone transform. */
static void actor_other_item(uint32 actor,uint32 index,uint32 color,int left,int player) {
 uint32 field=left?125:126,item,kind; sint32 slot=*(sint8*)ff_ptr(actor+field,1),type;
 if(slot==-1)return;item=0x8009a3c8+36u*(uint32)slot;kind=ff_u32(item);
 if(kind==0xffffffffu){ff_w8(actor+field,255);return;}
 FUN_80011F74((left?0x800ba214u:0x800ba234u)+504u*index);FUN_800100C4(0,-786432,0);FUN_8001033C(6144);
 if(player&&left) {
  type=ff_s16(actor+52);
  if(type==1&&kind==515){FUN_800100C4(-524288,-524288,-1310720);FUN_800101CC(59392);FUN_8001033C(5632);FUN_800104A0(7936);type=ff_s16(actor+52);}
  if(type==3&&kind==515){FUN_800100C4(-524288,-524288,-1310720);FUN_800101CC(59392);FUN_8001033C(5632);FUN_800104A0(7936);}
 }
 ff_w32(item+4,ff_u32(actor+40));ff_w32(item+12,ff_u32(actor+48));FUN_80058D9C(0x8009d208+12u*kind);
 ff_w32(0x8008d4bc,color);ff_static_model_80010B18(ff_u32(0x80081720+4u*kind),0,0);
 if(!left&&kind-496u<2&&ff_s16(item+22)>0)FUN_8002BDD0(item);
}
/* Preserve original four-load/four-store ordering, including possible overlap. */
static void actor_copy_pose(uint32 to,uint32 from) {
 uint32 i,a,b,c,d;
 for(i=0;i<496;i+=16){a=ff_u32(from+i);b=ff_u32(from+i+4);c=ff_u32(from+i+8);d=ff_u32(from+i+12);ff_w32(to+i,a);ff_w32(to+i+4,b);ff_w32(to+i+8,c);ff_w32(to+i+12,d);}
 a=ff_u32(from+496);b=ff_u32(from+500);ff_w32(to+496,a);ff_w32(to+500,b);
}
static void actor_trail(uint32 actor,uint32 index) {
 uint32 color=ff_u32(0x8008d4bc);
 ff_w32(0x8008d4bc,color|0x02000000);ff_w32(0x8008d4c0,4);
 FUN_8001060C(ff_u32(0x80081720+4u*(uint32)(sint32)ff_s16(actor+52)),0x800bb3c8);
 if(!*(sint8*)ff_ptr(0x80093dd8,1)&&!(ff_u32(0x80093dd0)&1))actor_copy_pose(0x800bb3c8,0x800bb1d0);
 ff_w32(0x8008d4bc,color|0x02000000);ff_w32(0x8008d4c0,1);
 FUN_8001060C(ff_u32(0x80081720+4u*(uint32)(sint32)ff_s16(actor+52)),0x800bb1d0);
 if(!*(sint8*)ff_ptr(0x80093dd8,1)) {
  if(!(ff_u32(0x80093dd0)&1))actor_copy_pose(0x800bb1d0,0x800ba210+504u*index);
  if(!*(sint8*)ff_ptr(0x80093dd8,1))ff_w8(actor+156,*(uint8*)ff_ptr(actor+156,1)&247);
 }
 ff_w32(0x8008d4bc,color);
}
static void actor_van(uint32 actor) {
 FUN_80010040();
 FUN_800100C4((sint32)(ff_u32(actor+40)-ff_u32(0x8008d4a4)),(sint32)(ff_u32(actor+44)+ff_u32(0x8009911c)-ff_u32(0x8008d4a8)),(sint32)(ff_u32(actor+48)-ff_u32(0x8008d4ac)));
 FUN_800101CC(32768);FUN_8001033C((sint16)((uint32)(sint32)ff_s16(actor+56)<<5));
 ff_static_model_80010B18(ff_u32(0x80081824),0,0);
 FUN_80010040();FUN_800104A0(ff_s16(actor+60));FUN_800100C4(0,4194304,0);
 ff_static_model_80010B18(ff_u32(0x80081764),0,0);FUN_80010028();
 ff_static_model_80010B18(ff_u32(0x80081830),0,4096);FUN_800100C4(0,0,30408704);FUN_8001033C(16384);FUN_80059394();FUN_80010028();
}
static void actor_body(uint32 actor,uint32 index,sint32 type,int player) {
 uint32 color;
 sint32 yaw,scale;
 FUN_80010040();
 FUN_800100C4((sint32)(ff_u32(actor+40)-ff_u32(0x8008d4a4)),(sint32)(ff_u32(actor+44)+ff_u32(0x800990d8+4u*(uint32)type)-ff_u32(0x8008d4a8)),(sint32)(ff_u32(actor+48)-ff_u32(0x8008d4ac)));
 if((!player&&type==51)||FUN_80011E5C()) {
  ff_w32(actor+232,ff_u32(actor+232)&~2u);actor_color(actor,player);color=ff_u32(0x8008d4bc);
  if(!player&&type==51)FUN_8001033C(32768);
  yaw=(sint16)(0u-((uint32)(sint32)ff_s16(actor+56)<<5))+32768;FUN_8001033C(yaw);FUN_800100C4(0,(sint32)ff_u32(actor+16),(sint32)ff_u32(actor+32));
  if(!player&&type==14&&(*(uint8*)ff_ptr(actor+156,1)&8))actor_trail(actor,index);
  FUN_8001060C(ff_u32(0x80081720+4u*(uint32)ff_s16(actor+52)),0x800ba210+504u*index);
  if(player)player_right_item(actor,index,color);
  else actor_other_item(actor,index,color,0,0);
  actor_other_item(actor,index,color,1,player);
  FUN_80010028();FUN_80010040();
  FUN_800100C4((sint32)(ff_u32(actor+40)-ff_u32(0x8008d4a4)),(sint32)(0u-ff_u32(0x8008d4a8)),(sint32)(ff_u32(actor+48)-ff_u32(0x8008d4ac)));yaw=(sint16)(0u-((uint32)(sint32)ff_s16(actor+56)<<5))+32768;FUN_8001033C(yaw);
  if(player)ff_static_model_80010B18(ff_u32(0x80082490),0,0);
  else {
   if(type==28||type==51)FUN_8001033C(16384);
   scale=((sint32)(ff_u32(actor+44)+ff_u32(actor+16))>>12)+(type==50?24576:16384);
   if(scale>0){FUN_80010F94(scale,scale,scale);ff_static_model_80010B18(ff_u32(0x80082490),0,0);}
  }
 } else ff_w32(actor+232,player?(ff_u32(actor+232)|2u):2u);
 FUN_80010028();
}
/* 593FC: character, held-item, trail, van and shadow rendering. */
GDB_CALL sint32 ff_characters_800593FC(void) {
 sint32 i,type,count;uint32 actor;
 FUN_80011C60();ff_game_over_8005FB44_live();
 for(i=2;i<8;i++) {
  actor=0x800b4318+244u*(uint32)i;type=ff_s16(actor+52);
  if(type==-1||(*(uint8*)ff_ptr(actor+119,1)&2))continue;
  if(type==17){actor_van(actor);continue;}
  actor_body(actor,(uint32)i,type,0);
 }
 count=(sint32)ff_u32(0x800940b0);if(count<=0)return count;
 for(i=0;i<(sint32)ff_u32(0x800940b0);i++) {
  if(ff_u32(0x80093e04)==2)return 2;
  actor=0x800b4318+244u*(uint32)i;type=ff_s16(actor+52);
  if(type==-1||(*(uint8*)ff_ptr(actor+119,1)&2))continue;
  if(ff_s16(actor+98)) {
   FUN_80010040();FUN_800100C4((sint32)(ff_u32(actor+40)-ff_u32(0x8008d4a4)),(sint32)(ff_u32(actor+44)-ff_u32(0x8008d4a8)),(sint32)(ff_u32(actor+48)-ff_u32(0x8008d4ac)));
   ff_static_model_80010B18(ff_u32(0x800824ac+4u*((uint32)i&1)),ff_u32(actor+8),0);FUN_80010028();
  }
  actor_body(actor,(uint32)i,type,1);
 }
 return 0;
}

/* 2BDD0: weapon effect intensity groups, projected depth bias and sprite. */
GDB_CALL uint32 FUN_8002BDD0(uint32 object) {
 FF_FUNCTION_MARKER(0x8002bdd0u,"GAME.EXE");
 sint32 group=(ff_s16(object+22)+29)/30,position[4];
 uint32 temp=0x801fc0e0;int i;
 /* Group2/above takes the unconditional path at2BE0C. */
 if(group<2 && (group!=1 || !(ff_u32(0x80093dd0)&2)))return 2;
 FUN_800120BC(0,0,0,temp);
 if((sint32)ff_u32(temp+8)>=65)ff_w32(temp+8,ff_u32(temp+8)-64u);
 if(ff_u32(temp+12))return (uint32)group<<1;
 for(i=0;i<4;i++)position[i]=(sint32)ff_u32(temp+4u*i);
 return FUN_80012390(position,ff_s16(0x8009adba+2u*(uint32)group));
}

/* 5D67C: settled debris local transform and kind-dependent origin bias. */
GDB_CALL uint32 FUN_8005D67C(uint32 object) {
 FF_FUNCTION_MARKER(0x8005d67cu,"GAME.EXE");
 uint32 kind;
 FUN_80010040();FUN_8005C740(object);
 kind=ff_u32(object);FUN_800100C4(0,0,kind==529?-3932160:kind==530?-3670016:-3145728);
 ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,0);
 return FUN_80010028();
}
/* 5B178: base and two hinged panels, caller owns the outer transform. */
GDB_CALL uint32 FUN_8005B178(uint32 object) {
 FF_FUNCTION_MARKER(0x8005b178u,"GAME.EXE");
 sint32 phase=ff_s16(object+28),height,angle;
 uint32 offset=(uint32)(phase<3?phase:2)<<9;
 height=(ff_s16(0x800802d0+offset)*5)>>12;
 ff_w16(object+26,0);ff_w16(object+22,0);
 angle=ff_s16(object+16);if(angle)FUN_8001033C(angle);
 ff_static_model_80010B18(ff_u32(0x800818c8),0,8192);
 FUN_800100C4(0,(sint32)((40u-(uint32)height)<<16),0);
 angle=ff_s16(object+20);if(angle)FUN_800104A0(angle);
 angle=ff_s16(object+18);if(angle)FUN_800101CC(angle);
 FUN_80010040();ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,0);FUN_80010028();
 FUN_80010040();FUN_800100C4(0,5963776,-9437184);
 FUN_800101CC(ff_s16(0x800802d0+((uint32)(sint32)ff_s16(object+28)<<8))>>2);
 ff_static_model_80010B18(ff_u32(0x80081724+4u*ff_u32(object)),0,0);FUN_80010028();
 FUN_800100C4(0,5767168,14286848);
 FUN_800101CC((-ff_s16(0x800802d0+((uint32)(sint32)ff_s16(object+28)<<8)))>>2);
 return ff_static_model_80010B18(ff_u32(0x80081728+4u*ff_u32(object)),0,0);
}
