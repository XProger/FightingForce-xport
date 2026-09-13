#include "ff.h"
#include <string.h>
#include "ff_gpu.h"
GDB_CALL uint32 FUN_80067940(void) {
 FF_FUNCTION_MARKER(0x80067940u,"GAME.EXE");
 uint32 handle=(uint32)ff_pc_create(0x80093bb8),block,row,offset,p,source,destination,value;
 uint8 group[16];
 if(!handle)return 0; /* Original tests zero, so the -1 error also enters. */
 FUN_80079FF4(handle,0x80093bc8,18);
 for(block=0;block<256;block+=16){
  DrawSync(0);ff_w16(0x80093bde,(uint16)(240-block));ff_gpu_store_image(0x80093bdc,ff_u32(0x800947f0));DrawSync(0);
  p=ff_u32(0x800947f0);for(offset=0;offset<5120;++offset,p+=2){value=(uint16)ff_s16(p);ff_w16(p,(uint16)(((value&31)<<10)+(value&0x3e0)+((value>>10)&31)));}
  for(row=0;row<16;++row){destination=ff_u32(0x80094844)+640*row;source=ff_u32(0x800947f0)+640*(15-row);
   for(offset=0;offset<640;offset+=16){memcpy(group,ff_ptr(source+offset,16),16);memcpy(ff_ptr(destination+offset,16),group,16);}}
  FUN_80079FF4(handle,ff_u32(0x80094844),10240);
 }
 FUN_80079FC4(handle);value=*(uint8*)ff_ptr(0x80093bbf,1)+1;ff_w8(0x80093bbf,(uint8)value);
 if((sint8)value<58)return 1;ff_w8(0x80093bbf,48);
 value=*(uint8*)ff_ptr(0x80093bbe,1)+1;ff_w8(0x80093bbe,(uint8)value);
 if((sint8)value<58)return 1;ff_w8(0x80093bbe,48);
 value=*(uint8*)ff_ptr(0x80093bbd,1)+1;ff_w8(0x80093bbd,(uint8)value);return value;
}
GDB_CALL uint32 FUN_800697A4(void) {
 FF_FUNCTION_MARKER(0x800697a4u,"GAME.EXE");
 uint32 descriptor,p,palette;int i;sint32 width;
 FUN_80067D3C();FUN_80067284();if(ff_s16(0x8001000c)+ff_s16(0x8001000e)+ff_s16(0x80010010))FUN_8006471C();
 descriptor=0x800a8768+12u*(uint32)(sint32)ff_s16(0x8009acc8+2u*(uint32)(sint32)ff_s16(0x800af4aa));
 for(i=0;i<3;++i,descriptor+=12){
  p=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,p+20);ff_w8(p+3,4);ff_w8(p+7,0x64);
  ff_w8(p+12,*(uint8*)ff_ptr(descriptor,1));ff_w8(p+13,*(uint8*)ff_ptr(descriptor+1,1));
  ff_w16(p+16,i==2?64:128);ff_w16(p+18,255);ff_w16(p+8,(uint16)(128*i));ff_w16(p+10,65529);
  ff_w8(p+4,128);ff_w8(p+5,128);ff_w8(p+6,128);FUN_80011E40(ff_u32(0x8008d4b4)+4,p);FUN_80066A44((uint32)(sint32)ff_s16(descriptor+6),1);
 }
 p=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,p+24);ff_w8(p+3,5);ff_w8(p+7,0x28);
 ff_w8(p+4,0);ff_w8(p+5,0);ff_w8(p+6,0);ff_w16(p+8,0);ff_w16(p+10,0);ff_w16(p+12,320);ff_w16(p+14,0);ff_w16(p+16,0);ff_w16(p+18,256);ff_w16(p+20,320);ff_w16(p+22,256);FUN_80011E40(ff_u32(0x8008d4b4),p);
 palette=ff_u32(0x80093d70)==0?0x80093be4:0x80093bf0;FUN_80062C0C(0x80093c2c,24,175,palette);
 palette=ff_u32(0x80093d70)==1?0x80093be4:0x80093bf0;width=FUN_8005EFB8(0x80093c34);FUN_80062C0C(0x80093c34,160-(width>>1),175,palette);
 palette=ff_u32(0x80093d70)==2?0x80093be4:0x80093bf0;FUN_80062C0C(0x80093c40,220,175,palette);return FUN_80061ADC(0x80093c48,30);
}
GDB_CALL uint32 FUN_800673BC(void) {
 FF_FUNCTION_MARKER(0x800673bcu,"GAME.EXE");
 uint32 p,model,page;int i;
 FUN_80067284();
 for(i=0;i<2;++i){
  p=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,p+20);ff_w8(p+3,4);ff_w8(p+7,0x65);
  ff_w16(p+14,32704);ff_w8(p+12,0);ff_w8(p+13,0);ff_w16(p+16,i?64:256);ff_w16(p+18,i?256:255);ff_w16(p+8,i?256:0);ff_w16(p+10,65528);
  FUN_80011E40(ff_u32(0x8008d4b4)+4*i,p);
  p=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,p+12);ff_w8(p+3,2);model=*(uint8*)ff_ptr(0x800872ec,1);page=i?140:138;
  ff_w32(p+4,0xe1000000u|page|((model==1||model==2)?0x1000:0x400));ff_w32(p+8,0);
  FUN_80011E40(ff_u32(0x8008d4b4)+4*i,p);
 }
 return FUN_80067358();
}
GDB_CALL uint32 FUN_80069B34(void) {
 FF_FUNCTION_MARKER(0x80069b34u,"GAME.EXE");
 uint32 buttons,tick;sint32 fade;
 DrawSync(0);FUN_80061448(2,0x116000);FUN_80060394(0);FUN_800645E8();FUN_80011CCC(-256,-256,-256);FUN_80063870(24);ff_w32(0x80093dd0,0);
 for(;;){
  tick=ff_u32(0x80093dd0);if((sint32)tick<33)FUN_8005EE58((sint32)(tick<<3));
  FUN_800697A4();FUN_80061ADC(0x80093c58,214);FUN_80067358();buttons=FUN_80068114(0);if(ff_u32(0x800940b0)==2)buttons|=FUN_80068114(1);
  if((buttons&128)&&(sint32)ff_u32(0x80093d70)>0)ff_w32(0x80093d70,ff_u32(0x80093d70)-1);
  if((buttons&32)&&(sint32)ff_u32(0x80093d70)<2)ff_w32(0x80093d70,ff_u32(0x80093d70)+1);
  if(buttons&0x4000)break;ff_w32(0x80093dd0,ff_u32(0x80093dd0)+1);
 }
 for(fade=256;fade>0;fade-=8){ff_w32(0x80093dd0,ff_u32(0x80093dd0)+1);FUN_8005EE58(fade);FUN_800697A4();FUN_80067358();}
 return FUN_8005ED78();
}
GDB_CALL uint32 FUN_8005A280(void) {
 FF_FUNCTION_MARKER(0x8005a280u,"GAME.EXE");
 uint32 scroll=0,frame=0,text=FUN_800611F8(0x80093584),p,source,destination,shade,result,saved[32];
 sint32 fade,slide,sprite,next,y,position[4];int i;uint8 c;
 for(i=0;i<32;++i)saved[i]=ff_u32(0x801fd100+4*i);
 FUN_80063870(12);
 for(;;++frame){
  fade=(sint32)(frame&255)-192;if(fade<0)fade=0;fade*=2;slide=((sint32)frame>>8)%5;
  if((sint32)frame<17)FUN_8005EE58((sint32)(frame<<4));FUN_80067D3C();FUN_80067284();
  if(ff_s16(0x8001000c)+ff_s16(0x8001000e)+ff_s16(0x80010010))FUN_8006471C();
  p=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,p+24);ff_w8(p+3,5);ff_w8(p+7,0x28); /* SKIP6E798/6E6F8(0). */
  ff_w8(p+4,0);ff_w8(p+5,0);ff_w8(p+6,0);ff_w16(p+8,0);ff_w16(p+10,0);ff_w16(p+12,320);ff_w16(p+14,0);ff_w16(p+16,0);ff_w16(p+18,256);ff_w16(p+20,320);ff_w16(p+22,256);
  FUN_80011E40(ff_u32(0x8008d4b4),p);
  sprite=ff_s16(0x8009acc8+2u*(uint32)(sint32)ff_s16(0x800af41a+2u*(uint32)slide));
  position[0]=96;position[1]=128;position[2]=256;position[3]=128;
  shade=128u-(uint32)fade;shade=(shade<<16)+(shade<<8)+0x2c000000u+shade;
  ff_w32(0x8008d4bc,shade);FUN_80012594(position,sprite);position[0]+=127;ff_w32(0x8008d4bc,shade);FUN_80012594(position,sprite+1);position[0]-=127;
  next=slide==4?ff_s16(0x8009acc8+2u*(uint32)(sint32)ff_s16(0x800af41a)):sprite+2;
  shade=((uint32)fade<<16)+((uint32)fade<<8)+0x2c000000u+(uint32)fade;
  ff_w32(0x8008d4bc,shade);FUN_80012594(position,next);position[0]+=127;ff_w32(0x8008d4bc,shade);FUN_80012594(position,next+1);
  if((sint32)scroll<2062)++scroll;if(scroll==2062)FUN_80061ADC(ff_u32(0x8009467c),214);
  source=text;y=256-((sint32)scroll>>1);
  for(;;){
   c=*(uint8*)ff_ptr(source,1);if(c=='<')break;destination=0x801fd100;
   do{ff_w8(destination++,c);c=*(uint8*)ff_ptr(++source,1);}while(c!=10&&c!='<');
   ++source;if(c=='<')break;ff_w8(destination-1,0); /* Strip CR before LF, as5A5B4. */
   if((uint32)y+9u<265u){if(*(uint8*)ff_ptr(0x801fd100,1)=='@')FUN_80061B28(0x801fd101,y);else FUN_80061ADC(0x801fd100,y);}
   y=(sint32)((uint32)y+10);
  }
  FUN_80067358();
  if(scroll==2062)for(i=0;i<(sint32)ff_u32(0x800940b0);++i){
   result=(uint32)FUN_8005F5AC((sint32)ff_u32(0x80094288+4u*i))&0x4008;
   if(result){for(i=0;i<32;++i)ff_w32(0x801fd100+4*i,saved[i]);return result;}
  }
 }
}
/* 58634: audited background dispatch 5887C..58BC8. The caller performs
 * horizon projection and the final fade, so this helper emits no entry marker. */
void ff_draw_stage_background(void) {
 uint32 stage=ff_u32(0x80093d58);
 switch(stage){
 case 1:break;
 case 2:FUN_800660DC();return;
 case 3:FUN_80064C1C();break;
 case 4:FUN_80064F38();break;
 case 7:FUN_800655C0();break;
 case 10:FUN_80064F38();FUN_80064380();return;
 case 8:case 9:case 11:FUN_80065274();FUN_80064380();return;
 case 12:case 13:FUN_80065918();FUN_80064380();return;
 case 14:FUN_800659A8();FUN_80064380();return;
 case 17:FUN_80065B3C();FUN_80064380();return;
 case 18:{
  uint32 x=ff_u32(0x8008d4a4),y=ff_u32(0x8008d4a8),z=ff_u32(0x8008d4ac),tick,px,py,pz;
  FUN_8006494C();tick=ff_u32(0x80093dd0);pz=tick<<23;ff_w32(0x8008d4ac,pz);
  px=(uint32)(sint32)ff_s16(0x800802d0+((tick<<5)&0xfe0))<<4;ff_w32(0x8008d4a4,px);
  py=ff_u32(0x8008d4a8)+0xfcec0000u;ff_w32(0x8008d4a8,py);
  ff_w32(0x8008d4a4,(uint32)((sint32)px>>2));ff_w32(0x8008d4a8,(uint32)((sint32)py>>2));ff_w32(0x8008d4ac,(uint32)((sint32)pz>>2));
  FUN_80063EEC();ff_w32(0x8008d4a4,x);ff_w32(0x8008d4a8,y);ff_w32(0x8008d4ac,z);return;
 }
 case 19:case 24:FUN_80064C1C();FUN_80064380();return;
 case 22:FUN_800660DC();FUN_80064380();return;
 case 23:FUN_80066160();return;
 case 26:FUN_8006686C();break;
 case 31:case 32:case 33:FUN_800664F0();break;
 case 34:if((sint32)ff_u32(0x80093dd0)>=301){FUN_800667B8();return;}FUN_800660DC();break;
 default:FUN_8006494C();break;
 }
 FUN_80063EEC();
}
static uint32 menu_black_quad(uint16 top,uint16 bottom,int horizon);
static void menu_sky_gradient(uint32 top,uint32 bottom,uint32 height_bias,int reload_horizon) {
 uint8 red=(uint8)(*(uint8*)ff_ptr(0x8008d4c8,1)<<3);uint32 color=ff_u32(0x8008d4c8);
 uint8 green=(uint8)(((sint32)color>>2)&~7),blue=(uint8)(((sint32)color>>7)&~7);
 uint32 p=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,p+36);ff_w8(p+3,8);ff_w8(p+7,0x38);
 ff_w8(p+4,0);ff_w8(p+5,0);ff_w8(p+6,0);ff_w8(p+12,0);ff_w8(p+13,0);ff_w8(p+14,0);
 ff_w8(p+20,red);ff_w8(p+21,green);ff_w8(p+22,blue);ff_w8(p+28,red);ff_w8(p+29,green);ff_w8(p+30,blue);
 ff_w16(p+8,0);if(reload_horizon){uint32 h=(uint16)ff_s16(0x8008d4d0);top=h-256;bottom=h-height_bias+(reload_horizon==1?1:0);}
 ff_w16(p+10,(uint16)top);ff_w16(p+16,320);ff_w16(p+18,(uint16)top);ff_w16(p+24,0);
 ff_w16(p+26,(uint16)bottom);ff_w16(p+32,320);ff_w16(p+34,(uint16)bottom);FUN_80011E40(ff_u32(0x8008d4b4),p);
}
static void menu_sky_sprite(uint32 x,uint32 y,uint32 v,uint32 height,int reload_horizon) {
 uint32 p=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,p+20);ff_w8(p+3,4);ff_w8(p+7,0x65);ff_w16(p+14,0x7c00);
 ff_w16(p+8,(uint16)x);if(reload_horizon)y=(uint16)ff_s16(0x8008d4d0)-y;
 ff_w16(p+10,(uint16)y);ff_w8(p+12,0);ff_w8(p+13,(uint8)v);ff_w16(p+16,256);ff_w16(p+18,(uint16)height);
 FUN_80011E40(ff_u32(0x8008d4b4),p);
}
GDB_CALL uint32 FUN_80064F38(void) {
 FF_FUNCTION_MARKER(0x80064f38u,"GAME.EXE");
 uint32 bias=160,scroll,i;
 if(ff_u32(0x80093d58)-8<2){FUN_80064D34(0xf0000000u,0);FUN_80064D34(0,0x10000000);}
 if(ff_u32(0x80093d58)==10)FUN_80064D34(0xfb000000u,0x05000000);
 if(ff_u32(0x80093d58)==4){bias=64;ff_w32(0x8008d4c8,4198);}
 if((sint32)(ff_u32(0x8008d4d0)-bias)>0)menu_sky_gradient(0,0,bias,1);
 scroll=((uint16)ff_s16(0x8008d490)>>5)&255;
 for(i=0;i<3;++i)menu_sky_sprite(scroll+256*i-256,bias,0,256,1);
 return FUN_80066A44(142,0);
}
GDB_CALL uint32 FUN_800655C0(void) {
 FF_FUNCTION_MARKER(0x800655c0u,"GAME.EXE");
 uint32 scroll=((uint32)(ff_s16(0x8008d490)>>5)+160)&255,i;
 for(i=0;i<3;++i)menu_sky_sprite(scroll+256*i-256,128,128,128,1);
 if((sint32)(ff_u32(0x8008d4d0)-128)>0)
  for(i=0;i<3;++i)menu_sky_sprite(scroll+256*i-256,256,0,128,1);
 return FUN_80066A44(142,0);
}
GDB_CALL uint32 FUN_800664F0(void) {
 FF_FUNCTION_MARKER(0x800664f0u,"GAME.EXE");
 uint32 scroll,i;if((sint32)(ff_u32(0x8008d4d0)-128)>0)menu_sky_gradient(0,0,128,2);
 scroll=((uint16)ff_s16(0x8008d490)>>5)&255;
 for(i=0;i<3;++i)menu_sky_sprite(scroll+256*i-256,128,0,256,1);
 return FUN_80066A44(142,0);
}
GDB_CALL uint32 FUN_80065274(void) {
 FF_FUNCTION_MARKER(0x80065274u,"GAME.EXE");
 uint32 horizon=ff_u32(0x8008d4d0),top,middle,angle,scroll,v,i;
 if(ff_u32(0x80093d58)-8<2)horizon+=24;
 top=horizon-256;middle=horizon-128;if((sint32)top>0)top=0;
 if((sint32)middle>0)menu_sky_gradient(top,middle,0,0);
 menu_black_quad((uint16)horizon,256,0);
 angle=(uint32)(ff_s16(0x8008d490)>>5)+160;scroll=angle&255;v=(angle&256)>>1;
 for(i=0;i<3;++i)menu_sky_sprite(scroll+256*i-256,middle,i==1?128-v:v,128,0);
 return FUN_80066A44(142,0);
}
GDB_CALL uint32 FUN_80065B3C(void) {
 FF_FUNCTION_MARKER(0x80065b3cu,"GAME.EXE");
 uint32 angle,horizon,p,color,scroll,v,i;uint8 red,green,blue;
 if((sint32)ff_u32(0x8008d4ac)<(sint32)0xf4000000u)ff_sky_occluder_80065B3C();
 angle=(uint16)ff_s16(0x8008d490)>>5;
 ff_w32(0x80093b98,ff_u32(0x80093b98)-((uint32)(sint32)ff_s16(0x800802d0+2*angle)<<3));
 horizon=ff_u32(0x8008d4d0);if((sint32)(horizon-84)>0)menu_sky_gradient(horizon-212,horizon-84,0,0);
 red=(uint8)(*(uint8*)ff_ptr(0x8008d4cc,1)<<3);color=ff_u32(0x8008d4cc);
 green=(uint8)(((sint32)color>>2)&~7);blue=(uint8)(((sint32)color>>7)&~7);
 p=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,p+24);ff_w8(p+3,5);ff_w8(p+7,0x28);
 ff_w8(p+4,red);ff_w8(p+5,green);ff_w8(p+6,blue);
 ff_w16(p+8,0);ff_w16(p+10,(uint16)(horizon+44));ff_w16(p+12,320);ff_w16(p+14,(uint16)(horizon+44));
 ff_w16(p+16,0);ff_w16(p+18,255);ff_w16(p+20,320);ff_w16(p+22,255);FUN_80011E40(ff_u32(0x8008d4b4),p);
 angle=(uint32)(ff_s16(0x8008d490)>>5)+(uint32)(sint32)ff_s16(0x80093b9a);scroll=angle&255;v=(angle&256)>>1;
 for(i=0;i<3;++i)menu_sky_sprite(scroll+256*i-256,horizon-84,i==1?128-v:v,128,0);
 return FUN_80066A44(142,0);
}
GDB_CALL sint32 FUN_800659A8(void) {
 FF_FUNCTION_MARKER(0x800659a8u,"GAME.EXE");
 uint32 p=ff_u32(0x8008d4b0);sint32 phase,volume=0;
 ff_w32(0x8008d4b0,p+24);ff_w8(p+3,5);ff_w32(p+4,0x28000000);
 ff_w32(p+8,0);ff_w32(p+12,320);ff_w32(p+16,160u<<16);ff_w32(p+20,(160u<<16)|320);
 FUN_80011E40(ff_u32(0x8008d4b4),p);
 phase=(sint32)(ff_u32(0x80093dd0)&511)-472;
 if(phase>=0 && *(sint8*)ff_ptr(0x800801a4+(uint32)phase,1)) {
  p=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,p+24);ff_w8(p+3,5);ff_w32(p+4,0x2a808080);
  ff_w32(p+8,0);ff_w32(p+12,320);ff_w32(p+16,256u<<16);ff_w32(p+20,(256u<<16)|320);
  FUN_80011E40(ff_u32(0x8008d4b4)+8188,p);FUN_80066A44(206,2047);volume=255;
 }
 return FUN_80056F08((sint32)ff_u32(0x800944bc),volume);
}
GDB_CALL sint32 FUN_80052428(void) {
 FF_FUNCTION_MARKER(0x80052428u,"GAME.EXE");
 ff_w32(0x80093d3c,0);FUN_800690D8();ff_w32(0x80092734,1);return 1;
}
GDB_CALL sint32 FUN_80052458(void) {
 FF_FUNCTION_MARKER(0x80052458u,"GAME.EXE");
 ff_w32(0x80093d3c,1);return FUN_800690D8();
}
GDB_CALL sint32 FUN_80052484(void) {
 FF_FUNCTION_MARKER(0x80052484u,"GAME.EXE");
 ff_w32(0x80093d3c,1);return FUN_80068D68();
}
GDB_CALL uint32 FUN_80064380(void) {
 FF_FUNCTION_MARKER(0x80064380u,"GAME.EXE");
 uint32 x=ff_u32(0x8008d4a4),z=ff_u32(0x8008d4ac),rx=x&0xffffff,rz=z&0xffffff;
 uint32 column=(uint32)((sint32)~x>>24),row=(uint32)((sint32)~z>>24),heading=(uint32)((ff_s16(0x8008d490)+2048)>>12)&15;
 uint32 i,p;uint16 horizon;
 if(ff_s16(0x8008d492)<8192){
  uint32 dx=ff_u32(0x80093b10+4*heading),dz=ff_u32(0x80093b50+4*heading);
  rx+=dx<<23;rz+=dz<<23;column+=(uint32)((sint32)dx>>1);row+=(uint32)((sint32)dz>>1);
 }
 FUN_80011C60();
 for(i=0;i<4;++i){
  FUN_80010040();FUN_800100C4((sint32)(((i&1)?0xf8000000u:0x08000000u)-rx),
   (sint32)(0u-ff_u32(0x8008d4a8)),(sint32)(((i&2)?0xf8000000u:0x08000000u)-rz));
  FUN_800112A4(0,column+((i&1)?16:0),row+((i&2)?16:0));FUN_80010028();
 }
 p=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,p+24);ff_w8(p+3,5);ff_w32(p+4,0x28000000);
 ff_w16(p+8,0);horizon=(uint16)ff_s16(0x8008d4d0);ff_w16(p+10,horizon);ff_w16(p+12,320);ff_w16(p+14,horizon);
 ff_w16(p+16,0);horizon+=26;ff_w16(p+18,horizon);ff_w16(p+20,320);ff_w16(p+22,horizon);
 return FUN_80011E40(ff_u32(0x8008d4b4),p);
}
GDB_CALL uint32 FUN_80066160(void) {
 FF_FUNCTION_MARKER(0x80066160u,"GAME.EXE");
 uint32 height=(uint32)((sint32)ff_u32(0x80093db8)>>9)+64u+ff_u32(0x8008d4d0);
 sint32 top=(sint32)(height-256),middle=(sint32)(height-128);
 uint32 color,p,i,angle,scroll,v;uint8 red,green,blue;if(top>0)top=0;
 if(middle>0){
  red=(uint8)(*(uint8*)ff_ptr(0x8008d4c8,1)<<3);color=ff_u32(0x8008d4c8);
  green=(uint8)(((sint32)color>>2)&~7);blue=(uint8)(((sint32)color>>7)&~7);
  p=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,p+36);ff_w8(p+3,8);ff_w8(p+7,0x38);
  ff_w8(p+4,0);ff_w8(p+5,0);ff_w8(p+6,0);ff_w8(p+12,0);ff_w8(p+13,0);ff_w8(p+14,0);
  ff_w8(p+20,red);ff_w8(p+21,green);ff_w8(p+22,blue);ff_w8(p+28,red);ff_w8(p+29,green);ff_w8(p+30,blue);
  ff_w16(p+8,0);ff_w16(p+10,(uint16)top);ff_w16(p+16,320);ff_w16(p+18,(uint16)top);
  ff_w16(p+24,0);ff_w16(p+26,(uint16)middle);ff_w16(p+32,320);ff_w16(p+34,(uint16)middle);
  FUN_80011E40(ff_u32(0x8008d4b4),p);
 }
 angle=(uint32)(ff_s16(0x8008d490)>>5)+256;scroll=angle&255;v=(angle&256)>>1;
 for(i=0;i<3;++i){
  p=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,p+20);ff_w8(p+3,4);ff_w8(p+7,0x65);ff_w16(p+14,0x7c00);
  ff_w16(p+8,(uint16)(scroll+256*i-256));ff_w16(p+10,(uint16)(height-128));
  ff_w8(p+12,0);ff_w8(p+13,(uint8)(i==1?128-v:v));ff_w16(p+16,256);ff_w16(p+18,128);
  FUN_80011E40(ff_u32(0x8008d4b4),p);
 }
 FUN_80066A44(0x8e,0);
 red=(uint8)(*(uint8*)ff_ptr(0x8008d4cc,1)<<3);color=ff_u32(0x8008d4cc);
 green=(uint8)(((sint32)color>>2)&~7);blue=(uint8)(((sint32)color>>7)&~7);
 p=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,p+24);ff_w8(p+3,5);ff_w8(p+7,0x28);
 ff_w8(p+4,red);ff_w8(p+5,green);ff_w8(p+6,blue);
 ff_w16(p+8,0);ff_w16(p+10,256);ff_w16(p+12,320);ff_w16(p+14,256);
 ff_w16(p+16,0);ff_w16(p+18,(uint16)(height-256));ff_w16(p+20,320);ff_w16(p+22,(uint16)(height-256));
 return FUN_80011E40(ff_u32(0x8008d4b4),p);
}
static uint32 menu_black_quad(uint16 top,uint16 bottom,int horizon) {
 uint32 p=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,p+24);
 ff_w8(p+3,5);ff_w32(p+4,0x28000000);ff_w16(p+8,0);ff_w16(p+10,top);
 ff_w16(p+12,320);ff_w16(p+14,top);ff_w16(p+16,0);
 if(horizon)bottom=(uint16)((uint16)ff_s16(0x8008d4d0)+20);
 ff_w16(p+18,bottom);ff_w16(p+20,320);ff_w16(p+22,bottom);
 return FUN_80011E40(ff_u32(0x8008d4b4),p);
}
GDB_CALL uint32 FUN_80064C1C(void) {
 FF_FUNCTION_MARKER(0x80064c1cu,"GAME.EXE");
 return menu_black_quad(0,0,1);
}
GDB_CALL uint32 FUN_80065918(void) {
 FF_FUNCTION_MARKER(0x80065918u,"GAME.EXE");
 return menu_black_quad(0,0,1);
}
GDB_CALL uint32 FUN_800660DC(void) {
 FF_FUNCTION_MARKER(0x800660dcu,"GAME.EXE");
 return menu_black_quad(0,256,0);
}
GDB_CALL uint32 FUN_80064CAC(void) {
 FF_FUNCTION_MARKER(0x80064cacu,"GAME.EXE");
 return menu_black_quad(160,255,0);
}
GDB_CALL uint32 FUN_800667B8(void) {
 FF_FUNCTION_MARKER(0x800667b8u,"GAME.EXE");
 uint32 p=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,p+36);
 ff_w8(p+3,8);ff_w8(p+7,0x38);
 ff_w8(p+4,0);ff_w8(p+5,0);ff_w8(p+6,32);
 ff_w8(p+12,0);ff_w8(p+13,0);ff_w8(p+14,32);
 ff_w8(p+20,64);ff_w8(p+21,32);ff_w8(p+22,16);
 ff_w8(p+28,64);ff_w8(p+29,32);ff_w8(p+30,16);
 ff_w16(p+8,0);ff_w16(p+10,0);ff_w16(p+16,320);ff_w16(p+18,0);
 ff_w16(p+24,0);ff_w16(p+26,128);ff_w16(p+32,320);ff_w16(p+34,128);
 return FUN_80011E40(ff_u32(0x8008d4b4),p);
}
GDB_CALL sint32 FUN_800524B0(void) {
 FF_FUNCTION_MARKER(0x800524b0u,"GAME.EXE");
 uint32 i,object,target;FUN_8004F520();
 ff_w16(0x80093d34,(uint16)ff_s16(0x8008d490));ff_w16(0x80093d36,(uint16)ff_s16(0x8008d492));
 for(i=0;i<3;++i)ff_w32(0x80093d28+4*i,ff_u32(0x8008d4a4+4*i));
 ff_w32(0x80093dd0,ff_u32(0x80093dd0)+1);
 ff_menu_prepare_80058634_stage26();ff_menu_stage_8001E168();FUN_80050CC8();
 /* A0 is the address of the selected-player array, not the IDA scalar. */
 FUN_80058E24(0x8009277c,(sint32)ff_u32(0x80092784));
 for(i=0;(sint32)i<(sint32)ff_u32(0x8009403c);++i){
  object=0x800b89f0+36*i;target=ff_u32(0x800bbf78+4*ff_u32(object));
  if(target)ff_object_call(target,object);
 }
 FUN_8005C17C();FUN_8001EE44();FUN_80011D20();
 FUN_800101CC(ff_s16(0x8008d492));FUN_8001033C(ff_s16(0x8008d490));
 FUN_8001EF68();FUN_8001EDB8();return FUN_8001ED1C();
}
static uint32 menu_dim_backdrop(void) {
 uint32 desc,p,i;desc=0x800a8768+12u*(uint32)(sint32)ff_s16(0x8009acc8+2u*(uint32)(sint32)ff_s16(0x800af4ae));
  for(i=0;i<3;++i,desc+=12){
   p=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,p+20);
   ff_w8(p+3,4);ff_w8(p+7,0x64);
   ff_w8(p+12,*(uint8*)ff_ptr(desc,1));ff_w8(p+13,*(uint8*)ff_ptr(desc+1,1));
   ff_w16(p+16,i==2?64:128);ff_w16(p+18,255);ff_w16(p+8,(uint16)(i*128));ff_w16(p+10,(uint16)-7);
   ff_w8(p+4,32);ff_w8(p+5,32);ff_w8(p+6,32);
   FUN_80011E40(ff_u32(0x8008d4b4)+4,p);FUN_80066A44((uint32)(sint32)ff_s16(desc+6),1);
  }
  p=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,p+24);
  ff_w8(p+3,5);ff_w32(p+4,0x28000000);
  ff_w16(p+8,0);ff_w16(p+10,0);ff_w16(p+12,320);ff_w16(p+14,0);
  ff_w16(p+16,0);ff_w16(p+18,256);ff_w16(p+20,320);ff_w16(p+22,256);
  return FUN_80011E40(ff_u32(0x8008d4b4),p);
}
GDB_CALL uint32 FUN_8006A5B8(void) {
 FF_FUNCTION_MARKER(0x8006a5b8u,"GAME.EXE");
 FUN_80067D3C();FUN_80067284();
 if(ff_s16(0x8001000c)+ff_s16(0x8001000e)+ff_s16(0x80010010))FUN_8006471C();
 return menu_dim_backdrop();
}
GDB_CALL sint32 FUN_80067E20(void) {
 FF_FUNCTION_MARKER(0x80067e20u,"GAME.EXE");
 FUN_80067D3C();if(ff_u32(0x80093ba4))FUN_800524B0();else {FUN_80067284();menu_dim_backdrop();}
 return FUN_80061ADC(0x80094628,30);
}
GDB_CALL sint32 FUN_800680CC(void) {
 FF_FUNCTION_MARKER(0x800680ccu,"GAME.EXE");
 uint32 i;ff_w32(0x80093c14,0xffffffff);
 for(i=0;i<2;++i){FUN_80067E20();FUN_80067358();}return 0;
}
GDB_CALL sint32 FUN_800520AC(void) {
 FF_FUNCTION_MARKER(0x800520acu,"GAME.EXE");
 static const uint32 color_offsets[4]={0,3,9,6};
 uint32 phase=ff_u32(0x80092908),offset,color,raw,edge,selection; sint32 width,count;int i,j;
 for(i=0;i<4;i++){
  offset=i?(phase+512u*i)&2047u:phase;color=(uint8)(ff_s16(0x800802d0+offset*2u)>>11);
  if(i==3)ff_w32(0x80092908,(phase+32u)&2047u);
  for(j=2;j>=0;j--)ff_w8(0x8007f834+color_offsets[i]+j,(uint8)color);
 }
 width=FUN_8005EFB8(0x80094848);FUN_80061B7C(0x80094848,160-(width>>1),78);
 width=FUN_8005EFB8(ff_u32(0x800924dc));selection=*(uint8*)ff_ptr(0x800927fc,1);
 FUN_80062C0C(ff_u32(0x800924dc),120-(width>>1),108,selection?0x8009290c:0x8007f834);
 width=FUN_8005EFB8(ff_u32(0x800924d8));selection=*(uint8*)ff_ptr(0x800927fc,1);
 FUN_80062C0C(ff_u32(0x800924d8),200-(width>>1),108,selection==1?0x8007f834:0x8009290c);
 count=(sint32)ff_u32(0x800940b0);if(count<=0)return count;
 for(i=0;i<(sint32)ff_u32(0x800940b0);i++){
  raw=FUN_8005F5AC(ff_u32(0x80094288+4u*i));edge=(raw^ff_u32(0x80092790+4u*i))&raw;ff_w32(0x80092798+4u*i,edge);ff_w32(0x80092790+4u*i,raw);
  edge=ff_u32(0x80092798+4u*i);ff_w32(0x800927a0,raw);
  if(edge&0x4000u){FUN_8004F490();selection=*(uint8*)ff_ptr(0x800927fc,1);ff_w32(0x800927ec,ff_u32(selection?0x800927f4:0x800927f8));}
  if((ff_u32(0x80092798+4u*i)&0x80u)&&*(uint8*)ff_ptr(0x800927fc,1))ff_w8(0x800927fc,0);
  if((ff_u32(0x80092798+4u*i)&0x20u)&&*(uint8*)ff_ptr(0x800927fc,1)!=1)ff_w8(0x800927fc,1);
 }return 0;
}
GDB_CALL sint32 FUN_8005A690(void) {
 FF_FUNCTION_MARKER(0x8005a690u,"GAME.EXE");
 uint32 phase=456960u,packet,pad;int frame,i;
 FUN_800638B8(24);
 for(frame=0;frame<210;frame++){
  if(frame<120)phase-=(uint32)(119-frame)<<6;
  if(frame<17)FUN_8005EE58(frame*16);if(209-frame<17)FUN_8005EE58((209-frame)*16);
  FUN_80067284();if(ff_s16(0x8001000c)+ff_s16(0x8001000e)+ff_s16(0x80010010))FUN_8006471C();
  packet=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,packet+24u);ff_w8(packet+3,5);ff_w32(packet+4,0x28000000);
  ff_w16(packet+8,0);ff_w16(packet+10,0);ff_w16(packet+12,320);ff_w16(packet+14,0);ff_w16(packet+16,0);ff_w16(packet+18,256);ff_w16(packet+20,320);ff_w16(packet+22,256);
  FUN_80011E40(ff_u32(0x8008d4b4),packet);FUN_80011D20();FUN_800100C4(0,0,((uint32)((sint32)phase>>9)+136u)<<16);FUN_800101CC(32768);FUN_800104A0(phase);
  ff_static_model_80010B18(ff_u32(0x80081e28),0,0);FUN_80067358();
  for(i=0;i<(sint32)ff_u32(0x800940b0);i++){
   pad=FUN_8005F5AC(ff_u32(0x80094288+4u*i));if(pad&8u){FUN_8005ED78();FUN_80067724();FUN_8005EE58(0);return 1;}
   if(pad&0x4000u){FUN_80067724();return 0;}
  }
 }FUN_80067724();return 0;
}
/* This entry consumes live MIPS v1. There are no resolved callers; expose it
 * explicitly instead of inventing the missing input from a RAM global. */
GDB_CALL sint32 FUN_8004F6D0(uint32 entry_v1) {
 FF_FUNCTION_MARKER(0x8004f6d0u,"GAME.EXE");
 uint32 speed,target,p,values[5],result;int i;sint32 wanted,current;
 speed=entry_v1+0x10000u;ff_w32(0x800927c4,speed);
 if((sint32)speed<-2097152)ff_w32(0x800927c4,0xffe00000);else if((sint32)speed>2097152)ff_w32(0x800927c4,0x200000);
 for(i=0;i<2;i++){
  p=4u*i;target=(uint32)((sint32)(ff_u32(0x800927bc+p)-ff_u32(0x8008d4a8+p))>>3);speed=ff_u32(0x800927c8+p);
  if((sint32)speed<(sint32)(target-0x10000u))speed+=0x10000u;
  else if((sint32)(target+0x10000u)<(sint32)speed)speed-=0x10000u;else speed=target;
  ff_w32(0x800927c8+p,speed);if((sint32)speed<-2097152)ff_w32(0x800927c8+p,0xffe00000);else if((sint32)speed>2097152)ff_w32(0x800927c8+p,0x200000);
 }
 for(i=0;i<2;i++){
  p=2u*i;wanted=(ff_s16(0x800927dc+p)-ff_s16(0x8008d490+p))>>4;current=ff_s16(0x800927e0+p);
  if(current<wanted-128)ff_w16(0x800927e0+p,(uint16)(current+128));
  current=ff_s16(0x800927e0+p);ff_w16(0x800927e0+p,(uint16)(wanted+128<current?current-128:wanted));
 }
 result=(uint32)(uint16)ff_s16(0x8008d490)+(uint16)ff_s16(0x800927e0);values[0]=result;
 values[1]=(uint32)(uint16)ff_s16(0x8008d492)+(uint16)ff_s16(0x800927e2);
 for(i=0;i<3;i++)values[2+i]=ff_u32(0x8008d4a4+4u*i)+ff_u32(0x800927c4+4u*i);
 ff_w16(0x8008d490,(uint16)values[0]);ff_w16(0x8008d492,(uint16)values[1]);for(i=0;i<3;i++)ff_w32(0x8008d4a4+4u*i,values[2+i]);return (sint32)result;
}
GDB_CALL sint32 FUN_80051E50(void) {
 FF_FUNCTION_MARKER(0x80051e50u,"GAME.EXE");
 uint32 phase=ff_u32(0x80092900),count,i,raw,edge;uint8 color;sint32 width;
 color=(uint8)(ff_s16(0x800802d0+2u*phase)>>11);ff_w8(0x8007f82a,color);ff_w8(0x8007f829,color);ff_w8(0x8007f828,color);
 color=(uint8)(ff_s16(0x800802d0+2u*((phase+512u)&2047u))>>11);ff_w8(0x8007f82d,color);ff_w8(0x8007f82c,color);ff_w8(0x8007f82b,color);
 color=(uint8)(ff_s16(0x800802d0+2u*((phase+1024u)&2047u))>>11);ff_w8(0x8007f833,color);ff_w8(0x8007f832,color);ff_w8(0x8007f831,color);
 color=(uint8)(ff_s16(0x800802d0+2u*((phase+1536u)&2047u))>>11);ff_w32(0x80092900,(phase+32u)&2047u);ff_w8(0x8007f830,color);ff_w8(0x8007f82f,color);ff_w8(0x8007f82e,color);
 width=FUN_8005EFB8(0x80094848);FUN_80061B7C(0x80094848,(sint32)(160u-(uint32)(width>>1)),78);
 width=FUN_8005EFB8(0x80092904);FUN_80062C0C(0x80092904,(sint32)(160u-(uint32)(width>>1)),108,0x8007f828);
 count=ff_u32(0x800940b0);if((sint32)count<=0)return (sint32)count;i=0;
 do{
  raw=(uint32)FUN_8005F5AC(ff_u32(0x80094288+4u*i));edge=0x80092798+4u*i;ff_w32(edge,(raw^ff_u32(0x80092790+4u*i))&raw);ff_w32(0x80092790+4u*i,raw);
  raw=ff_u32(edge);ff_w32(0x800927a0,ff_u32(0x80092790+4u*i));
  if(raw&0x4000u){FUN_8004F490();ff_w32(0x800927ec,ff_u32(0x800927f0));}
  count=ff_u32(0x800940b0);i++;
 }while((sint32)i<(sint32)count);return 0;
}
#include "ff_gpu.h"
#include "platform_dummy.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/* 5717C. IRQ/card BIOS lifecycle calls do not execute on Windows; card events
 * are WIP. Host polling replaces InitPAD/StartPAD and ChangeClearPAD(0).
 * _bu_init and InitCARD/StartCARD have no storage behind them. */
GDB_CALL sint32 FUN_8005717C(void) {
 static const uint32 spec[4]={4,0x8000,0x100,0x2000};uint32 i;sint32 result=0;
 ResetCallback(); /* 75C14: native scheduling belongs to the window loop. */
 ++ff_dummy_calls[1]; /* WIP729DC CdInit: host assets need no drive reset. */
 ff_w32(0x8008b6d4,0); /* 72BC0(0) */
 ff_dummy_card_events_reset();
 for(i=0;i<8;i++)ff_w32(0x80093cb0+4*i,ff_dummy_card_open_event(i<4?0xf4000001:0xf0000011,spec[i&3],0x2000,0));
 FUN_8006DE48(0);ff_host_pad_init(0x80094748,34,0x80094770,34);FUN_8006DE9C();
 ++ff_dummy_calls[0]; /* WIP _bu_init. */
 ff_host_pad_state[4]=1; /* SKIP6D178 StartPAD wrapper. */
 if(ResetGraph(0)<0)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);ff_gpu_reset_graph_state();
 SetGraphDebug(0);ff_w8(0x800872ee,0);
 for(i=0;i<8;i++)result=ff_dummy_card_enable_event(ff_u32(0x80093cb0+4*i));
 return result;
}
/* 574B4..575E0, display part of 5748C. Platform/pad/event init5717C,
 * VBlank registration and CD TOC675FC precede this region and remain WIP.
 * Hardware fill/display/VSync/GTE calls use the AgentArmstrong host layer. */
GDB_CALL void ff_display_setup_800574B4(void) {
 uint16 offset;
 ff_gpu_set_def_draw_env(0x800b8938,0,0,320,256);
 ff_gpu_set_def_draw_env(0x800b8994,320,0,320,256);
 ff_gpu_set_def_disp_env(0x800947a0,320,0,320,256);
 ff_gpu_set_def_disp_env(0x800947b4,0,0,320,256);
 offset=(uint16)ff_s16(0x80093d38);ff_w16(0x800947a8,offset);ff_w16(0x800947bc,offset);
 offset=(uint16)ff_s16(0x80093d3a);ff_w16(0x800947aa,offset);ff_w16(0x800947be,offset);
 SetDispMask(1);ff_gpu_clear_menu_surfaces();
 FUN_80011D50(0xe7140);FUN_80011D50(0xe5140);
 VSync(0);ff_w32(0x8008d4c4,0);InitGeom();SetGeomScreen(256);SetGeomOffset(160,120);
 FUN_80011C60();
}
/* 80011F50: count >> 1 WORDS, not bytes. Forward load/store order is deliberate. */
GDB_CALL uint32 FUN_80011F50(uint32 destination,uint32 source,uint32 count) {
 uint32 result;count>>=1;
 do {result=ff_u32(source);source+=4;ff_w32(destination,result);--count;destination+=4;}while(count);
 return result;
}
/* 80058C14: rotate three five-halfword blocks, copy 60 bytes, draw status text. */
GDB_CALL uint32 FUN_80058C14(void) {
 int i,k;uint32 p=0x800947f8,flags,result;
 ff_w32(0x8008d4d4,ff_u32(0x8008d4d4)+1);
 for(i=0;i<3;i++) {
  uint16 first=(uint16)ff_s16(p);
  for(k=0;k<4;k++,p+=2)ff_w16(p,(uint16)ff_s16(p+2));
  ff_w16(p,first);p+=2;
 }
 FUN_80011F50(0x800b50d8,0x800947f8,30);flags=*(uint8*)ff_ptr(0x80093dd8,1);
 if(flags&15) {
  if(ff_u32(0x800940b0)==1){if(flags&3)FUN_80061ADC(ff_u32(0x800924e0),115);}
  else {if(flags&1)FUN_80061ADC(ff_u32(0x800924e4),115);if(*(uint8*)ff_ptr(0x80093dd8,1)&2)FUN_80061ADC(ff_u32(0x800924e8),115);}
 }
 if(*(uint8*)ff_ptr(0x80093dd8,1)&4)FUN_80061ADC(ff_u32(0x800924d0),130);
 if(*(uint8*)ff_ptr(0x80093dd8,1)&8)FUN_80061ADC(ff_u32(0x800924d4),130);
 result=ff_u32(0x8008d4c4)+1;ff_w32(0x8008d4c4,result);return result;
}
/* 80011CCC: NEG/ADDI are trapping instructions; retain that domain explicitly. */
GDB_CALL uint32 FUN_80011CCC(sint32 r,sint32 g,sint32 b) {
 sint32 values[3]={r,g,b};int i;
 for(i=0;i<3;i++) {
  __int64 n=-(__int64)values[i];
  if(n>2147483647 || n-255<-2147483647-1)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
  values[i]=(sint32)((n>=255?255:(uint32)n)&255);
 }
 for(i=0;i<3;i++)ff_w16(0x8001000c+2u*i,(uint16)values[i]);
 return 0x8001000c;
}
GDB_CALL uint32 FUN_8005EE58(sint32 level) {
 sint32 color[3];uint32 delta=(uint32)level-256u;int i;
 for(i=0;i<3;i++){color[i]=(sint32)((uint32)(sint32)ff_s16(0x80093e10+2u*i)+delta);if(color[i]<-256)color[i]=-256;}
 return FUN_80011CCC(color[0],color[1],color[2]);
}
/* Region 80050090..80050238 of 8004FBCC, with the scene-skip branch at
 * 8004FF90. Return 1 at cleanup entry, 0 at the next frame boundary.
 * Initialization and scene drawing remain separate WIP regions. */
/* 5F214: clear the three pad/demo override words in original store order. */
GDB_CALL void FUN_8005F214(void) {
 ff_w32(0x80093d08,0);ff_w32(0x80093d14,0);ff_w32(0x80093d10,0);
}
GDB_CALL sint32 ff_menu_tail_80050090(sint32 skip_scene) {
 int i;uint32 count;
 if(skip_scene)ff_w32(0x80092734,0);
 else {
  for(i=0;i<2;i++) {
   uint32 raw=(uint32)FUN_8005F5AC((sint32)ff_u32(0x80094288+4u*i));
   if(raw&65535)ff_w32(0x8009276c+4u*i,ff_u32(0x80093dd0));
   if((raw&0x8680)!=0x8680)ff_w32(0x80092774+4u*i,ff_u32(0x80093dd0));
  }
  if(*(sint8*)ff_ptr(0x80094000,1)) {
   ff_w32(0x80093d1c,0xffffffff);ff_w32(0x80093d58,(uint32)(sint32)*(sint8*)ff_ptr(0x800947d4,1));
   ff_w32(0x800940b0,(uint32)(sint32)*(sint8*)ff_ptr(0x80094816,1));return 1;
  }
 }
 if(!ff_u32(0x80092788))return 0;
 count=ff_u32(0x8009278c);ff_w32(0x8009278c,count+1);
 {
  sint32 phase=(sint32)(count-15u),level;if(phase<0)phase=0;
  level=(sint32)(256u-((uint32)phase<<3));if(level<0)level=0;FUN_8005EE58(level);
 }
 if(ff_u32(0x8009278c)!=48)return 0;
 for(i=0;i<(sint32)ff_u32(0x80092784);i++)ff_w16(0x800b434c+244u*i,(uint16)ff_u32(0x800927a4+4*ff_u32(0x8009277c+4u*i)));
 ff_w32(0x80093d58,(ff_u32(0x80093d68)&2)?ff_u32(0x800927b4):0);
 ff_w32(0x80093d1c,0xffffffff);ff_w32(0x800940b0,ff_u32(0x80092784));return 1;
}
/* GAME.EXE:8004F520. Signed halfword phase, wrapped fixed-point products,
 * then 1/16 smoothing even after the sine transition has finished. */
GDB_CALL sint32 FUN_8004F520(void) {
 sint32 phase=ff_s16(0x800927e8),i,result=0;
 if(phase<512) {
  sint32 sine=ff_s16(0x800802d0+2*((uint32)phase&2047)),coefficient=sine>>3;
  ff_w16(0x800927e8,(uint16)(phase+32));
  for(i=0;i<3;i++)ff_w32(0x800927d0+4u*i,ff_u32(0x800927b8+4u*i)+(uint32)coefficient*(uint32)((sint32)ff_u32(0x800927c4+4u*i)>>12));
  for(i=0;i<2;i++) {
   sint32 product=(sint32)((uint32)sine*(uint32)(sint32)ff_s16(0x800927e0+2u*i));
   ff_w16(0x800927e4+2u*i,(uint16)((uint16)ff_s16(0x800927dc+2u*i)+(product>>15)));
  }
 }
 for(i=0;i<2;i++) {
  sint32 current=ff_s16(0x8008d490+2u*i);
  ff_w16(0x8008d490+2u*i,(uint16)(current+((ff_s16(0x800927e4+2u*i)-current)>>4)));
 }
 for(i=0;i<3;i++) {
  uint32 current=ff_u32(0x8008d4a4+4u*i);
  result=(sint32)(ff_u32(0x800927d0+4u*i)-current)>>4;
  ff_w32(0x8008d4a4+4u*i,current+(uint32)result);
 }
 return result;
}
static sint32 div_mips(sint32 a,sint32 b) {
 if(!b || (b==-1&&(uint32)a==0x80000000u))ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
 return a/b;
}
/* GAME.EXE:8001614C. Piecewise angle approximation, including distance output.
 * Preserve 32-bit shifts, signed division and final signed 16-bit angle. */
GDB_CALL sint32 FUN_8001614C(sint32 x0,sint32 y0,sint32 x1,sint32 y1) {
 sint32 dy=(sint32)((uint32)y1-(uint32)y0),dx=(sint32)((uint32)x1-(uint32)x0);uint32 q=0,value;
 if(dy<0){dy=(sint32)(0u-(uint32)dy);q=1;}
 if(dx<0){dx=(sint32)(0u-(uint32)dx);q+=2;}
 if(dy<dx) {
  ff_w32(0x800941a0,(uint32)(dy>>1)+(uint32)dx);
  value=(uint32)div_mips((sint32)((uint32)dy<<13),dx)^0x3fff;
 }else {
  ff_w32(0x800941a0,(uint32)(dx>>1)+(uint32)dy);if(!dy)dy=1;
  value=(uint32)div_mips((sint32)((uint32)dx<<13),dy);
 }
 return (sint16)((value+ff_u32(0x8007aebc+4*q))^ff_u32(0x8007aeac+4*q));
}
/* GAME.EXE:8004F980. Capture camera state and compute fixed-point deltas. */
GDB_CALL sint32 FUN_8004F980(const uint32 *position,sint32 yaw,sint32 pitch) {
 uint32 a=ff_u32(0x8008d4a4),b=ff_u32(0x8008d4a8),c=ff_u32(0x8008d4ac);
 uint16 oldyaw=(uint16)ff_s16(0x8008d490),oldpitch=(uint16)ff_s16(0x8008d492);
 ff_w16(0x800927e8,128);ff_w16(0x800927dc,oldyaw);ff_w16(0x800927de,oldpitch);
 ff_w32(0x800927b8,a);ff_w32(0x800927bc,b);ff_w32(0x800927c0,c);
 ff_w16(0x800927e0,(uint16)((uint32)yaw-oldyaw));ff_w16(0x800927e2,(uint16)((uint32)pitch-oldpitch));
 ff_w32(0x800927c4,position[0]-a);ff_w32(0x800927c8,position[1]-(b-6291456u));
 ff_w32(0x800927cc,position[2]-c);return (sint32)(position[2]-c);
}
/* GAME.EXE:8004FA10. Center camera between selected actors then orbit via sine. */
GDB_CALL sint32 FUN_8004FA10(void) {
 sint32 i,n=(sint32)ff_u32(0x80092784);uint32 position[3]={0,0,0},angle;
 uint32 centerx=ff_u32(0x80092714),centerz=ff_u32(0x8009271c),index;
 for(i=0;i<n;i++) {
  uint32 actor=ff_u32(0x8009277c+4u*i),offset=244u*actor;
  position[0]+=ff_u32(0x800b4340+offset);position[2]+=ff_u32(0x800b4348+offset);
 }
 position[0]=(uint32)div_mips((sint32)position[0],n);position[2]=(uint32)div_mips((sint32)position[2],n);
 angle=((uint32)FUN_8001614C((sint32)position[0]>>16,(sint32)position[2]>>16,(sint32)centerx>>16,(sint32)centerz>>16)-0x8000u)<<16;
 index=(uint32)((sint32)angle>>21);
 position[0]=centerx+((uint32)(sint32)ff_s16(0x800802d0+2*(index&2047))<<11);
 position[1]=0xfef00000u;
 position[2]=centerz-((uint32)(sint32)ff_s16(0x800802d0+2*((index+512)&2047))<<11);
 return FUN_8004F980(position,(sint32)angle>>16,0);
}
/* Normalized twin pair 8004F468/8004F490: same effect dispatch, adapted ID/volume.
 * The underlying GAME:80056670 sound allocator is a required WIP service. */
sint32 ff_menu_effect(sint32 id,sint32 volume) {
 if(!ff_services.play_effect)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);return ff_services.play_effect(id,volume,0x1f00);
}
GDB_CALL sint32 FUN_8004F468(void){return ff_menu_effect(3,255);}
GDB_CALL sint32 FUN_8004F490(void){return ff_menu_effect(4,127);}
GDB_CALL sint32 FUN_8004F4F8(sint32 y){return FUN_80061ADC(ff_u32(0x800924d0),y);}

/* GAME.EXE:80050270. Main menu callback. Drawing precedes edge-triggered input.
 * Callback values remain original image addresses; native dispatcher maps them. */
GDB_CALL sint32 FUN_80050270(void) {
 uint32 phase=ff_u32(0x80092800),counts,mode,frame;int i,k; sint32 result;
 const uint32 phases[4]={0,512,1536,1024};
 for(i=0;i<4;i++) {
  uint32 sample=i?((phase+phases[i])&2047):phase;
  uint8 color=(uint8)(ff_s16(0x800802d0+sample*2)>>11);
  for(k=0;k<3;k++)*(uint8*)ff_ptr(0x8007f81c+3u*i+k,1)=color;
 }
 ff_w32(0x80092784,0);ff_w32(0x80092800,(phase+32)&2047);
 FUN_8005EC90();ff_w32(0x800940b0,ff_u32(0x8009400c));
 if(!ff_u32(0x8009400c))return FUN_8004F4F8(98);
 FUN_80061C68(96,16);FUN_80061ADC(ff_u32(0x80094698),214);
 mode=ff_u32(0x800940b0)!=1;counts=0x8007f804+4*mode;
 ff_w32(0x8007f804,ff_u32(0x8007f80c));ff_w32(0x8007f808,ff_u32(0x8007f810));
 {
  uint32 y=138u-10u*ff_u32(counts);
  for(i=0;i<(sint32)ff_u32(counts);i++) {
   uint32 text=ff_u32(0x80094600+16*mode+4u*i);
   sint32 x=(sint32)(160u-(uint32)(FUN_8005EFB8(text)>>1));
   FUN_80062C0C(text,x,(sint32)y,ff_u32(0x80092730)==(uint32)i?0x8007f81c:0x80092804);y+=20;
  }
 }
 frame=ff_u32(0x80093dd0);
 if((sint32)(ff_u32(0x80092774)+60u)<(sint32)frame || (sint32)(ff_u32(0x80092778)+60u)<(sint32)frame) {
  ff_w32(0x80092778,frame);ff_w32(0x80092774,frame);ff_w32(0x80093d68,ff_u32(0x80093d68)^2);
 }
 if(ff_u32(0x80093d68)&2)FUN_80061B7C(0x80092810,(sint32)(160u-(uint32)(FUN_8005EFB8(0x80092810)>>1)),200);
 result=(sint32)ff_u32(0x800940b0);
 for(i=0;i<(sint32)ff_u32(0x800940b0);i++) {
  uint32 raw=(uint32)FUN_8005F5AC((sint32)ff_u32(0x80094288+4u*i)),edge=0x80092798+4u*i;
  ff_w32(edge,(raw^ff_u32(0x80092790+4u*i))&raw);ff_w32(0x80092790+4u*i,raw);ff_w32(0x800927a0,raw);
  if(ff_u32(edge)&0x10) {
   FUN_8004F468();ff_w32(0x80092730,ff_u32(0x80092730)-1);
   if((sint32)ff_u32(0x80092730)<0)ff_w32(0x80092730,ff_u32(counts)-1);
  }
  if(ff_u32(edge)&0x40) {
   FUN_8004F468();ff_w32(0x80092730,ff_u32(0x80092730)+1);
   if((sint32)(ff_u32(counts)-1)<(sint32)ff_u32(0x80092730))ff_w32(0x80092730,0);
  }
  if(ff_u32(edge)&0x1000) {
   if(!ff_services.game_longjmp)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);ff_services.game_longjmp(0x80094298,0);ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
  }
  if(ff_u32(edge)&0x4000) {
   uint32 selection=ff_u32(0x80092730);
   if((!selection&&!i)||(mode&&selection==1)) {
    FUN_8004F490();ff_w32(0x80092784,(!selection&&!i)?1:2);ff_w32(0x80093d5c,(!selection&&!i)?0:1);
    ff_w32(0x800927ec,0x80050798);ff_w32(0x8009272c,0);FUN_8004FA10();
   }else if((!mode&&selection==1)||(mode&&selection==2)) {
    uint32 position[3];for(k=0;k<3;k++)position[k]=ff_u32(0x8009281c+4u*k);
    FUN_8004F490();ff_w32(0x800927ec,0x80050d98);FUN_8004F980(position,0,0x4000);
   }
  }
  result=i+1<(sint32)ff_u32(0x800940b0);
 }
 return result;
}

/* 8001E168 stage26 path. Other stage scripts are unported gameplay WIP. */
GDB_CALL sint32 ff_menu_stage_8001E168(void) {
 sint32 timer;if(ff_u32(0x80093d58)!=26)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
 timer=(sint32)ff_u32(0x80093dfc);if(timer>0)ff_w32(0x80093dfc,(uint32)timer-1);
 return 20;
}

/* 800586D0..80058768: selected display screen offset and shake term. */
GDB_CALL void ff_display_offsets_800586D0(void) {
 uint32 shake=ff_u32(0x800941a8),offset=20*(ff_u32(0x8008d4c4)&1);
 sint32 denominator=(sint32)(18u-shake),delta;
 if(!denominator)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__); /* original BREAK7 */
 delta=(sint32)((shake&1)<<5)/denominator;
 ff_w16(0x800947a8+offset,(uint16)((uint32)delta+(uint16)ff_s16(0x80093d38)));
 ff_w16(0x800947aa+offset,(uint16)((uint32)delta+(uint16)ff_s16(0x80093d3a)));
}

/* 8006471C/80064820 share the same audited full-screen packet tail.
 * SDK shape setters reuse PsyQ; draw mode follows the guest GPU model. */
static uint32 menu_overlay(uint8 r,uint8 g,uint8 b,uint16 page) {
 uint32 p=ff_u32(0x8008d4b0);uint8 *q;
 ff_w32(0x8008d4b0,p+24);q=(uint8*)ff_ptr(p,24);
 setPolyF4(q);setSemiTrans(q,1);q[4]=r;q[5]=g;q[6]=b;
 ff_w32(p+8,0);ff_w32(p+12,320);ff_w32(p+16,0x01000000);ff_w32(p+20,0x01000140);
 FUN_80011E40(ff_u32(0x8008d4b4)+8188,p);
 return FUN_80066A44(page,2047);
}
uint32 FUN_8006471C(void) {
 return menu_overlay(*(uint8*)ff_ptr(0x8001000c,1),*(uint8*)ff_ptr(0x8001000e,1),*(uint8*)ff_ptr(0x80010010,1),206);
}
uint32 FUN_80064820(void) {
 uint32 phase=ff_u32(0x80094190)-6u;
 uint8 color=(uint8)((sint32)phase>0?255u-85u*phase:255u+42u*phase);
 return menu_overlay(color,color,color,174);
}

/* Menu startup dependency: 80014498, eight 244-byte actor records.
 * Only type -1 is inactive; other negative signed types still initialize. */
uint32 FUN_80014498(void) {
 uint32 i,p=0x800b43b5;
 ff_w32(0x80094034,0);ff_w32(0x80094030,0);
 ff_w32(0x800941b4,50000);ff_w32(0x800941b0,50000);ff_w32(0x80093d64,0);
 for(i=0;i<8;i++,p+=244)if(ff_s16(p-105)!=-1) {
  uint8 *v;
  ff_w16(p-89,380);ff_w16(p-87,380);ff_w16(p-91,380);ff_w16(p-85,384);
  v=(uint8*)ff_ptr(p,3);v[2]=0;v[1]=0;v[0]=0;
 }
 ff_w32(0x80093e1c,4);ff_w32(0x80093f38,0);ff_w32(0x80093f3c,0);return 4;
}

uint32 FUN_80011F20(uint32 value) {
 return (value>>24)|(value<<24)|((value>>8)&0xff00u)|((value&0xff00u)<<8);
}
/* 80061384..80061424: resident archive lookup, without loading-screen prefix.
 * Missing names originally enter fatal 61120 forever; host stops explicitly.
 * Original strcpy assumes short resource names. Reject overflow on the host. */
static uint32 resource_entry(uint32 name) {
 uint8 key[16]={0};uint32 n=0,entry=ff_u32(0x80094038),i;
 sint32 count=(sint32)FUN_80011F20(ff_u32(entry));entry+=4;
 for(;;) {
  uint8 c=*(uint8*)ff_ptr(name+n,1);
  if(n>=sizeof(key)){fprintf(stderr,"WIP: oversized archive name\n");ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);}
  key[n++]=c;if(!c)break;
 }
 for(i=0;(sint32)i<count;i++,entry+=20)if(!memcmp(key,ff_ptr(entry,12),12))return entry;
 return 0;
}
uint32 ff_resource_find_80061384(uint32 name) {
 uint32 entry=resource_entry(name);
 if(!entry){fprintf(stderr,"WIP: archive resource not found\n");ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);}
 ff_w32(0x80094570,FUN_80011F20(ff_u32(entry+12)));
 return FUN_80011F20(ff_u32(entry+16))+ff_u32(0x80094818);
}
/* 61140 deliberately swaps AFTER adding raw archive offset and base.
 * Caller60394 tests nonzero; this is not the611F8 data pointer calculation. */
uint32 FUN_80061140(uint32 name) {
 uint32 entry=resource_entry(name);
 return entry?FUN_80011F20(ff_u32(0x80094818)+ff_u32(entry+16)):0;
}

uint32 FUN_800611F8(uint32 name) {
 sint32 total=ff_s16(0x80093566);
 ff_w32(0x80093e0c,ff_u32(0x80093e0c)+1);
 if(total>0) {
  POLY_G4 p;uint32 i,parity; sint32 count=(sint32)ff_u32(0x80093e0c),width;
  if(count>total)count=total;
  /* Preserve MIPS wrapping shifts/adds BEFORE signed division. */
  width=(sint32)((uint32)count*170u)/total;
  memset(&p,0,sizeof(p));setPolyG4(&p);
  p.r0=p.r1=255;p.r2=p.r3=216;p.b2=p.b3=178;
  p.x0=p.x1=77;p.x2=p.x3=(sint16)((uint32)width+77);
  p.y0=p.y2=203;p.y1=p.y3=214;
  for(i=0;i<2;i++) {
   parity=ff_u32(0x8008d4c4)&1;ff_gpu_put_draw_env(0x800b8938+92*parity,320*parity,0);
   ff_gpu_packet(&p); /* DrawSync is synchronous on the host. */
   ff_w32(0x8008d4c4,ff_u32(0x8008d4c4)+1);
  }
  ff_gpu_present();
 }
 return ff_resource_find_80061384(name);
}
uint32 FUN_80067108(uint32 texture_name,uint32 palette_name) {
 uint32 texture=FUN_800611F8(texture_name),palette,i,result=0;
 uint8 first=*(uint8*)ff_ptr(texture+32,1),last=*(uint8*)ff_ptr(texture+32799,1);
 ff_w16(0x80093b9e,0);
 if(!ff_gpu_load_image(0x80093b9c,texture+32))ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
 /* DrawSync has no work to wait for in the synchronous host upload. */
 ff_w16(0x80093b9e,128);
 if(!ff_gpu_load_image(0x80093b9c,texture+32+32800))ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
 palette=FUN_800611F8(palette_name);
 ff_w32(0x8008d4c8,(uint16)ff_s16(palette+2u*first)&0x7fff);
 ff_w32(0x8008d4cc,(uint16)ff_s16(palette+2u*last)&0x7fff);
 for(i=0;i<512;i+=16) {
  uint32 words[4];memcpy(words,ff_ptr(palette+i,16),16);
  memcpy(ff_ptr(0x800b4ab8+i,16),words,16);result=words[0];
 }
 return result;
}

/* 80063900..80063AD0, ending before map lookup/object initialization.
 * Return carries original s1 into the forthcoming map phase. */
uint32 ff_terrain_textures_80063900(uint32 name) {
 uint32 pbl=FUN_800611F8(name),source=pbl+512,dest,i,x,y;
 for(i=0;i<512;i+=16) {
  uint32 words[4];memcpy(words,ff_ptr(pbl+i,16),16);memcpy(ff_ptr(0x800b4cb8+i,16),words,16);
 }
 ff_w16(0x800b4cb8,0);
 if(!ff_gpu_load_image(0x80093a78,source))ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
 dest=ff_u32(0x800947f0)+20u*FUN_80011F20(ff_u32(ff_u32(0x80094038)))+4;
 ff_w32(0x800947f0,dest);
 for(y=0;y<128;y++)for(x=0;x<128;x++)
  *(uint8*)ff_ptr(dest+y*128+x,1)=*(uint8*)ff_ptr(source+y*512+x*2,1);
 if(!ff_gpu_load_image(0x80093a80,dest))ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
 for(y=0;y<64;y++)for(x=0;x<64;x++)
  *(uint8*)ff_ptr(dest+y*64+x,1)=*(uint8*)ff_ptr(source+y*1024+x*4,1);
 if(!ff_gpu_load_image(0x80093a88,dest))ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
 return source;
}

/* 80063AE8..80063C38, after object initialization has returned. */
uint32 ff_terrain_map_80063AE8(uint32 map,uint32 pixels) {
 uint32 i,x,y;
 for(i=0;i<8192;i+=16) {
  uint32 words[4];memcpy(words,ff_ptr(map+i,16),16);memcpy(ff_ptr(0x8009f870+i,16),words,16);
 }
 for(y=0;y<64;y++)for(x=0;x<64;x++) {
  sint32 tile=ff_s16(0x8009f870+2*(y*64+x))>>2;
  uint32 dest=0x800a3290+y*256+x*2;
  if(tile<0) {
   *(uint8*)ff_ptr(dest+129,1)=252;*(uint8*)ff_ptr(dest+128,1)=252;
   *(uint8*)ff_ptr(dest+1,1)=252;*(uint8*)ff_ptr(dest,1)=252;
  } else {
   uint32 src=pixels+32u*((uint32)tile&7)+8192u*((uint32)tile>>3);
   *(uint8*)ff_ptr(dest,1)=*(uint8*)ff_ptr(src+2570,1);
   *(uint8*)ff_ptr(dest+1,1)=*(uint8*)ff_ptr(src+2580,1);
   *(uint8*)ff_ptr(dest+128,1)=*(uint8*)ff_ptr(src+5130,1);
   *(uint8*)ff_ptr(dest+129,1)=*(uint8*)ff_ptr(src+5140,1);
  }
 }
 return 0;
}

/* 8006109C..800610E8: convert MPD entries to texture descriptor offsets.
 * Invoke once after descriptor setup, never every frame. */
uint32 ff_terrain_relocate_8006109C(void) {
 uint32 i;
 for(i=0;i<4096;i++) {
  uint32 address=0x8009f870+2*i,value=(uint16)ff_s16(address);
  if(value!=65532u)value=3u*(value+(uint32)(sint32)ff_s16(0x800af4b2));
  ff_w16(address,(uint16)value);
 }
 return 0;
}

/* 8003EECC: next is the record after the object just inserted by173B0. */
uint32 FUN_8003EECC(uint32 next,uint32 type,uint32 x,uint32 z) {
 uint32 p=next-36,i;
 if(type==268) {
  for(i=0;i<3;i++,p+=36) {
   ff_w32(p+4,i==2?0xf3800000u:x);ff_w32(p+12,z);
   ff_w16(p+30,(uint16)i);ff_w32(p,268);
  }
  return 2;
 }
 if(type==359){ff_w32(p+4,0x1f400000);ff_w32(p+12,0x1c200000);return 0;}
 if(type==781){ff_w16(p+16,0xc000);ff_w32(0x800940ac,p);}
 return 0;
}

uint32 FUN_80034244(uint32 next,uint32 type,uint32 x,uint32 z) {
 uint32 p=next,i,newtype=0,angle=0;
 if(type==920) {
  static const uint32 zs[3]={0xe9a00000,0xe7200000,0xe4900000};p-=36;
  for(i=0;i<3;i++,p+=36){ff_w16(p+16,0xc000);ff_w32(p+4,0x50000000);ff_w32(p+12,zs[i]);ff_w32(p,79);}
  return 2;
 }
 if(type-94u<2){newtype=93;angle=0x4900;}
 else if(type-108u<2){newtype=93;angle=0xa000;}
 else if(type==720){newtype=723;angle=0xe000;}
 else if(type==62){newtype=65;angle=0xbc00;}
 if(newtype){ff_w32(p+4,x);ff_w32(p+12,z);ff_w32(p,newtype);ff_w16(p+16,(uint16)angle);ff_w16(p-20,(uint16)angle);return 1;}
 if(type==133) {
  ff_w32(0x80094620,ff_u32(0x8009403c)-1);
  for(i=0;i<7;i++,p+=36){ff_w32(p+4,x);ff_w32(p+12,z);ff_w32(p,i<3?133:134);}return 7;
 }
 if(type==663){ff_w32(p+4,0u-x);ff_w32(p+12,z);ff_w16(p+16,0x8000);ff_w32(p,663);return 1;}
 if(type==211)newtype=212;
 if(type==776){p-=36;ff_w16(p+16,0xc000);}
 if(type==784)newtype=783;
 if(type-911u<4){p-=36;ff_w32(p,813);ff_w16(p+16,(uint16)ff_s16(0x8007acbe+2*type));}
 if(type==398)newtype=399;
 if(newtype){ff_w32(p+4,x);ff_w32(p+12,z);ff_w32(p,newtype);return 1;}
 if(type==429||type==431){ff_w32(p+4,x);ff_w32(p+12,z);ff_w32(p,433);ff_w16(p+26,0);ff_w32(0x80094338,624);return 1;}
 if(type==137)ff_w32(0x800940d0,p-36);
 if(type==138)ff_w32(0x800940d4,p-36);
 if(type==341)ff_w32(0x800947c8,p-36);
 return 0;
}

uint32 FUN_800406C4(uint32 next,uint32 type) {
 uint32 p=next-36,index;
 switch(type) {
 case 832:ff_w32(p+4,0xf0000000);ff_w32(p+8,0xdc000000);ff_w32(p+12,0x10000000);ff_w16(p+16,0x3000);ff_w16(p+18,0);ff_w16(p+20,0);break;
 case 833:ff_w32(p+4,0x01000000);ff_w32(p+8,0xef000000);ff_w32(p+12,0);ff_w16(p+16,0);ff_w16(p+18,0);ff_w16(p+20,0);break;
 case 835:ff_w16(p+16,0x2000);break;
 case 836:ff_w16(p+16,0xd000);break;
 case 739:
  ff_w16(p+16,0xed00);
  ff_w16(p+22,(uint16)((sint32)(ff_u32(p+4)-((uint32)(sint32)ff_s16(0x800805a0)<<14))>>16));
  ff_w16(p+24,(uint16)((sint32)(ff_u32(p+12)-((uint32)(sint32)ff_s16(0x800802d0+2*(0xed00>>5))<<14))>>16));break;
 case 405:case 754:ff_w16(p+24,0);ff_w16(p+26,0);break;
 case 168:ff_w16(p+26,0xffff);ff_w16(p+30,0xffff);break;
 case 166:ff_w16(p+16,0x7c00);ff_w16(p+26,0xffff);break;
 case 175:ff_w16(p+16,0xc000);break;
 case 160:ff_w16(p+16,0xc000);ff_w16(p+26,0xffff);break;
 case 687:ff_w32(p+12,0x40000000);ff_w16(p+28,0);break;
 case 199:ff_w16(p+16,0xf000);ff_w16(p+24,20);break;
 case 202:ff_w16(p+16,0xf000);break;
 case 205:ff_w16(p+16,0x1000);break;
 case 689:case 690:ff_w16(p+16,0x1800);break;
 case 516:case 517:case 518:ff_w16(p+26,(uint16)(ff_rand_8006D348()&31));break;
 case 696:
  p=next;index=ff_rand_8006D348()&2047;
  ff_w32(p+4,((uint32)(sint32)ff_s16(0x800802d0+2*index)<<6)+ff_u32(p-32));
  ff_w32(p+12,((uint32)(sint32)ff_s16(0x800806d0+2*index)<<6)+ff_u32(p-24));
  ff_w32(p,473);ff_w32(p+8,0xff700000);
  ff_w16(p+16,(uint16)ff_s16(p-20));ff_w16(p+18,(uint16)ff_s16(p-18));ff_w16(p+20,(uint16)ff_s16(p-16));ff_w16(p+26,0);
  /* MIPS shift/add sequence is multiplication modulo2^32, then signed>>2. */
  ff_w16(p-6,(uint16)((sint32)(954437177u*(p-0x800b89f0))>>2));return 1;
 case 702:ff_w32(p,701);ff_w16(p+16,0x8000);break;
 }
 return 0;
}

static sint32 mpd_s16(uint32 p) {
 const uint8 *b=(const uint8*)ff_ptr(p,2);return (sint16)((uint16)b[0]*256u+b[1]);
}
/* Scene26 and sequence27 paths of173B0. Dynamic spawn classes and other
 * named gameplay regions remain WIP. Sequence27 uses kinds 0,7,8,14. */
uint32 ff_menu_objects_800173B0(uint32 map) {
 uint32 cursor=map+8192,object=0x800b89f0,region,i;
 if(ff_u32(0x80093d58)!=0&&ff_u32(0x80093d58)!=26&&ff_u32(0x80093d58)!=27){fprintf(stderr,"WIP: unsupported-stage MPD initialization\n");ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);}
 ff_w32(0x8009403c,0);ff_w32(0x800942dc,0);memset(ff_ptr(object,4608),0,4608);
 while(*(uint8*)ff_ptr(cursor,1)!=255) {
  sint32 code=*(sint8*)ff_ptr(cursor++,1);
  uint32 type=(uint32)(sint32)ff_s16(ff_u32(0x800946bc)+(uint32)(code*2)-2),x,z,n;
  x=0u-((uint32)mpd_s16(cursor)<<19);z=0u-((uint32)mpd_s16(cursor+2)<<19);cursor+=4;
  if(type-477u<86) {
   sint32 slot=FUN_800167C4((sint32)type,x,0,z,0);
   ff_w16(0x8009a3d8+36u*(uint32)slot,(uint16)(2u*ff_rand_8006D348()));continue;
  }
  if(type-587u<10){fprintf(stderr,"WIP: MPD pickup spawn16654\n");ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);}
  if(object+36*8>0x800b9bf0)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
  ff_w32(object+4,x);ff_w32(object+12,z);ff_w32(object,type);ff_w32(0x8009403c,ff_u32(0x8009403c)+1);
  if(type-464u<8){n=ff_u32(0x800942dc);ff_w32(0x800942dc,n+1);ff_w32(0x800940b8+4*n,object);}
  if(type==466){ff_w32(0x800942dc,ff_u32(0x800942dc)-1);ff_w32(0x800940d4,object);}
  object+=36;
  n=FUN_80034244(object,type,x,z);object+=36*n;ff_w32(0x8009403c,ff_u32(0x8009403c)+n);
  n=FUN_8003EECC(object,type,x,z);object+=36*n;ff_w32(0x8009403c,ff_u32(0x8009403c)+n);
  n=FUN_800406C4(object,type);object+=36*n;ff_w32(0x8009403c,ff_u32(0x8009403c)+n);
 }
 for(i=0;i<ff_u32(0x8009403c);i++) {
  uint32 p=0x800b89f0+36*i,type=ff_u32(p);
  if(type-473u>=2&&type!=475)ff_w32(p+8,(uint32)(sint32)ff_s16(0x800a7290+2*type)<<16);
 }
 cursor++;region=0x8009ec70;memset(ff_ptr(region,3072),0,3072);
 ff_w32(0x800941cc,0);ff_w32(0x800941c8,0);ff_w32(0x8009479c,0);
 while(*(uint8*)ff_ptr(cursor,1)!=255) {
  uint32 kind=*(uint8*)ff_ptr(cursor++,1);sint32 mode,a,b,c,d;
  if(ff_u32(0x80093d58)==27||ff_u32(0x80093d58)==0) {
   if(kind==14){ff_w32(ff_u32(0x800941c8)?0x800941cc:0x800941c8,region);kind=0;}
   else if(kind!=0&&kind!=7&&kind!=8&&!(ff_u32(0x80093d58)==0&&kind==2)){fprintf(stderr,"WIP: unsupported stage MPD region\n");ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);}
  } else if(kind!=2&&kind!=3){fprintf(stderr,"WIP: non-menu MPD region\n");ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);}
  if(region>=0x8009f870)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
  mode=*(sint8*)ff_ptr(0x80094488+kind,1);
  ff_w16(region+8,(uint16)kind);ff_w16(region+10,(uint16)mode);ff_w16(region+12,*(uint8*)ff_ptr(0x800946d8+kind,1));
  a=-8*mpd_s16(cursor);b=-8*mpd_s16(cursor+2);c=-8*mpd_s16(cursor+4);d=-8*mpd_s16(cursor+6);cursor+=8;
  if(mode==2){sint32 cx=(c+a)>>1,cz=(d+b)>>1,dx=a-cx,dz=b-cz;c=dx<dz?dz:dx;a=cx;b=cz;}
  else for(i=0;i<ff_u32(0x8009403c);i++) {
   uint32 p=0x800b89f0+36*i;sint32 x=ff_s16(p+6),z=ff_s16(p+14);
   if(x>=c&&z>=d&&a>=x&&b>=z){ff_w32(region+20,p);break;}
  }
  ff_w16(region,(uint16)c);ff_w16(region+2,(uint16)d);ff_w16(region+4,(uint16)a);ff_w16(region+6,(uint16)b);
  ff_w32(0x8009479c,ff_u32(0x8009479c)+1);region+=24;
 }
 return 0xffffffffu;
}

/* Complete63900 sequencing for scene26 with progress-disabled lookup. */
uint32 FUN_80063900(uint32 texture_name,uint32 map_name) {
 uint32 pixels=ff_terrain_textures_80063900(texture_name);
 uint32 map=FUN_800611F8(map_name);
 ff_menu_objects_800173B0(map);
 return ff_terrain_map_80063AE8(map,pixels);
}

static void terrain_descriptor(uint32 address,uint8 left,uint8 top,uint8 right,uint8 bottom,uint16 page) {
 uint8 *p=(uint8*)ff_ptr(address,12);
 ff_w16(address+2,31808);ff_w16(address+6,page);
 p[8]=left;p[0]=left;p[5]=top;p[1]=top;p[10]=right;p[4]=right;p[11]=bottom;p[9]=bottom;
}
/* 80060DAC..80061098:64 tiles, four orientations, two background records.
 * The count advances256, excluding the final two records as in MIPS. */
void ff_terrain_descriptors_80060DAC(void) {
 uint32 base=ff_u32(0x80094838),p=0x800a8768+12*base,i;
 ff_w16(0x800af4b2,(uint16)base);
 for(i=0;i<64;i++) {
  uint8 u=(uint8)(32*(i&7)),v=(uint8)(32*(i>>3));uint16 page=(uint16)getTPage(1,0,768,0);
  ff_w16(0x800947cc,u);ff_w16(0x800947ce,v);
  terrain_descriptor(p,u,(uint8)(v+31),(uint8)(u+31),v,page);p+=12;
  terrain_descriptor(p,(uint8)(u+31),(uint8)(v+31),u,v,page);p+=12;
  terrain_descriptor(p,u,v,(uint8)(u+31),(uint8)(v+31),page);p+=12;
  terrain_descriptor(p,(uint8)(u+31),v,u,(uint8)(v+31),page);p+=12;
  ff_w32(0x80094838,ff_u32(0x80094838)+4);
 }
 terrain_descriptor(p,0,224,255,239,(uint16)getTPage(1,0,0,256));
 terrain_descriptor(p+12,0,224,63,239,(uint16)getTPage(1,0,128,256));
}

/* 80066EF4: reset the 64x256 texture allocation map and row occupancy. */
uint32 FUN_80066EF4(void) {
 uint32 i;
 memset(ff_ptr(0x000e1140,0x4000),0,0x4000);
 memset(ff_ptr(0x80094208,128),0,128);
 for(i=0;i<32;i++) {
  memset(ff_ptr(0x000e1140+256*i,160),1,160);
  memset(ff_ptr(0x000e1220+256*i,32),1,32);
  memset(ff_ptr(0x000e1200+256*i,32),1,32);
  ff_w16(0x80094208+2*i,224);
 }
 for(i=0;i<16;i++) {
  uint32 n=i<8?24:16;
  memset(ff_ptr(0x000e11e0+256*i,n),1,n);
  ff_w16(0x80094208+2*i,(uint16)ff_s16(0x80094208+2*i)+n);
 }
 for(i=60;i<62;i++) {
  memset(ff_ptr(0x000e1140+256*i,40),1,40);
  ff_w16(0x80094208+2*i,(uint16)ff_s16(0x80094208+2*i)+40);
 }
 for(i=62;i<64;i++) {
  memset(ff_ptr(0x000e1140+256*i,64),1,64);
  ff_w16(0x80094208+2*i,64);
 }
 return 0;
}

uint32 FUN_80066E4C(sint32 x,sint32 y,sint32 width,sint32 height) {
 sint32 row,col,yend=(sint32)((uint32)y+(uint32)height),xend=(sint32)((uint32)x+(uint32)width);
 for(row=y;row<yend;row++) {
  if(ff_s16(0x80094208+2*(uint32)row)==256)return 0;
  for(col=x;col<xend;col++)if(*(uint8*)ff_ptr(0xe1140+256*(uint32)row+(uint32)col,1))return 0;
 }
 return 1;
}
uint32 FUN_80066D14(void) {
 uint32 y,x;
 for(y=32;y<64;y++) {
  if(ff_s16(0x80094208+2*y)==256)continue;
  for(x=0;x<256;x+=4) {
   uint8 *p=(uint8*)ff_ptr(0xe1140+256*y+x,4);
   if(p[0]||p[1]||p[2]||p[3])continue;
   memset(p,1,4);ff_w16(0x80094208+2*y,(uint16)ff_s16(0x80094208+2*y)+4);
   ff_w16(0x800947cc,4*x);ff_w16(0x800947ce,8*y);return 8*y;
  }
 }
 fprintf(stderr,"WIP: texture palette allocation exhausted (original fatal loop)\n");ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);return 0;
}
uint32 FUN_80066ACC(sint32 width,sint32 height,sint32 mode,uint32 rect) {
 sint32 w=width,h,y,band,page,x,row,step;
 /* Resource loader supplies positive dimensions and modes 0..2. */
 if(width<=0||height<=0||width>257||height>257||mode<0||mode>2) {
  fprintf(stderr,"WIP: unsupported texture allocation input\n");ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
 }
 step=16<<mode;if(mode==0)w>>=1;if(mode==2)w*=2;
 w=(w+7)>>3;h=(height+7)>>3;if(w>32)w=32;if(h>32)h=32;
 for(band=0;band<64;band+=32)for(y=band;y<band+33-h;y++) {
  if(ff_s16(0x80094208+2*y)==256)continue;
  for(page=0;page<256;page+=step)for(x=page;x<page+step-(w-1);x++) {
   if(!FUN_80066E4C(x,y,w,h))continue;
   for(row=y;row<y+h;row++) {
    memset(ff_ptr(0xe1140+256*row+x,w),1,w);
    ff_w16(0x80094208+2*row,(uint16)ff_s16(0x80094208+2*row)+w);
   }
   ff_w16(0x800947cc,8*x);ff_w16(0x800947ce,8*y);
   ff_w16(rect,4*x);ff_w16(rect+2,8*y);ff_w16(rect+4,4*w);ff_w16(rect+6,8*h);
   return 8*h;
  }
 }
 fprintf(stderr,"WIP: texture allocation exhausted (original fatal loop)\n");ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);return 0;
}

/* 8006055C..80060710: effect palette tables for resource index zero. */
void ff_effect_palette_8006055C(uint32 palette) {
 uint32 level,i;
 for(level=0;level<9;level++)for(i=0;i<16;i++) {
  uint32 c=(uint16)ff_s16(palette+160+2*i);
  sint32 r=(c&31)-4*level,g=((c>>5)&31)-4*level,b=((c>>10)&31)-4*level;
  if(r<0)r=0;if(g<0)g=0;if(b<0)b=0;
  ff_w16(0x800a1870+32*level+2*i,0x8000+r+32*g+1024*b);
 }
 for(i=0;i<15;i++)ff_w16(0x800947f8+2*i,(uint16)ff_s16(palette+32+2*i));
 if(ff_u32(0x80093d58)==0)for(i=0;i<8;i++) {
  uint32 c=(uint16)ff_s16(palette+96+2*i)|0x8000;
  uint32 r=c&31,g=(c>>5)&31,b=(c>>10)&31;
  ff_w16(palette+96+2*i,c);ff_w16(0x80094418+2*i,c);
  if(r)r--;if(g)g--;if(b)b--;
  ff_w16(0x80094438+2*i,0x8000+r+32*g+1024*b);
 }
 /* Stages1,3,4 run a memory-free delay loop in the original. */
}

/* 800609B8..80060B8C: common texture descriptor, before resource-specific UV fixes. */
uint32 ff_texture_descriptor_800609B8(uint32 width,uint32 height,uint32 mode,uint32 flags,
 uint32 allocated,uint32 palette_slot,sint32 palette_x,sint32 palette_y_next) {
 uint32 count=ff_u32(0x80094838),p=0x800a8768+12*(count+1-allocated);
 uint32 x=(uint16)ff_s16(0x800947cc),y=(uint16)ff_s16(0x800947ce),page;
 uint8 *d=(uint8*)ff_ptr(p,12);
 ff_w16(0x8009acc8+2*ff_u32(0x8009440c),count);ff_w32(0x80094838,count+allocated);
 ff_w16(p+2,(palette_slot+496)<<6);
 if(mode==0)ff_w16(p+2,(uint32)(palette_x>>4)+(((uint32)palette_y_next-1)<<6));
 page=(uint16)getTPage(mode,ff_u32(0x8008012c+4*flags),((sint32)((x&0xff00)<<16))>>17,y&256);
 x&=255;y&=255;
 if(mode==0){x*=2;if(x>=256){page=(uint16)(page+1);x&=255;}}
 if(mode==2)x>>=1;
 ff_w16(p+6,page);ff_w16(0x800947cc,x);ff_w16(0x800947ce,y);
 d[0]=d[8]=(uint8)x;d[1]=d[5]=(uint8)y;
 d[4]=d[10]=(uint8)(x+width-1);d[9]=d[11]=(uint8)(y+height-1);
 if(flags)d[0]++;
 return p;
}

/* 80060B90..80060D60: resource-specific animated texture UV limits. */
void ff_texture_uv_fix_80060B90(uint32 resource,uint32 ordinal,uint32 descriptor) {
 uint32 target=0,stage=ff_u32(0x80093d58);uint8 y=*(uint8*)ff_ptr(0x800947ce,1);
 uint8 *d=(uint8*)ff_ptr(descriptor,12);
 if(resource==70) {
  if(stage==0 && ordinal-6<2)target=0x800945cc+2*ordinal;
  else if(stage-12<2 && ordinal-136<4)target=0x800944c8+2*ordinal;
 } else if(resource==8 && ordinal==62)target=0x800945d8;
 else if(resource==9 && ordinal-40<4)target=0x80094588+2*ordinal;
 else if(resource==20 && ordinal-144<6)target=0x800944b8+2*ordinal;
 else if(resource==23 && ordinal-92<2)target=0x80094520+2*ordinal;
 if(target) {
  d[1]=y;ff_w16(target,(sint8)y);d[9]=d[11]=(uint8)(y+31);
 }
}

/* 60394: archive palette/texture loading, descriptor tables and map relocation.
 * Optional611F8 progress display remains explicitly unsupported. */
uint32 FUN_80060394(uint32 all_characters) {
 uint32 i,slot=2,left=0; sint32 palx=0,paly=0;
 FUN_80066EF4();ff_w32(0x8009440c,0);ff_w32(0x80094838,0);ff_w32(0x80093a58,0);
 for(i=0;i<79;i++) {
  uint32 name,p,ordinal,end;
  if(!all_characters && i>=36 && i<=40 && ff_s16(0x800b434c)!=(sint32)(i-36) && ff_s16(0x800b4440)!=(sint32)(i-36))continue;
  name=ff_u32(0x800ba040+4*i);
  if(name && FUN_80061140(name)) {
   uint32 j,d; p=FUN_800611F8(name);
   if(i==0)ff_effect_palette_8006055C(p);
   d=0x800b4ab8+512*(++slot);
   for(j=0;j<512;j+=16){uint8 block[16];memcpy(block,ff_ptr(p+j,16),16);memcpy(ff_ptr(d+j,16),block,16);}
  }
  name=ff_u32(0x800bbe38+4*i);if(!name||!FUN_80061140(name))continue;
  p=FUN_800611F8(name);end=p+ff_u32(0x80094570);ordinal=0;
  ff_w16(0x800af418+2*i,ff_u32(0x8009440c));
  while(p<end) {
   sint32 w=ff_s16(p),h=ff_s16(p+2),mode=*(sint8*)ff_ptr(p+5,1);
   uint32 flags=*(uint8*)ff_ptr(p+4,1)&3,size,allocated=1,d;
   p+=32;
   if(w<=0||h<=0||w>257||h>257||mode<0||mode>2){fprintf(stderr,"WIP: malformed texture record\n");ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);}
   size=(((mode==0)?((w*h)>>1)+32:(mode==1?w*h:2*w*h))+31)&~31u;
   if(w==8&&h==8) {
    flags=0;mode=1;allocated=0;ff_w32(0x80093a58,ff_u32(0x80093a58)+1);
   } else {
    uint8 saved_rect[8];const uint32 rect=0x1f8003f0;
    if(mode==0) {
     if(!left){FUN_80066D14();palx=ff_s16(0x800947cc);paly=ff_s16(0x800947ce);left=8;}
     if(!ff_gpu_load_rect(p,(sint16)palx,(sint16)paly,16,1))ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
     paly++;left--;p+=32;size-=32;
    }
    /* Temporary host bridge for the original stack RECT, restored immediately. */
    memcpy(saved_rect,ff_ptr(rect,8),8);FUN_80066ACC(w,h,mode,rect);
    if(!ff_gpu_load_image(rect,p))ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);memcpy(ff_ptr(rect,8),saved_rect,8);
    p+=size;
   }
   d=ff_texture_descriptor_800609B8(w,h,mode,flags,allocated,slot,palx,paly);
   ff_texture_uv_fix_80060B90(i,ordinal,d);
   ff_w32(0x8009440c,ff_u32(0x8009440c)+1);ordinal++;
  }
 }
 ff_terrain_descriptors_80060DAC();return ff_terrain_relocate_8006109C();
}

uint32 FUN_8005E7C0(void) {
 FUN_80067108(0x80093908,0x80093914);
 return FUN_80063900(0x80093920,0x8009392c);
}

uint32 FUN_80013A30(void) {
 uint32 i;ff_w32(0x800947f0,0x116000);
 for(i=0;i<289;i++) {
  ff_w32(0x800ae580+4*i,0);ff_w32(0x800ae0f8+4*i,0);
  ff_w32(0x800aea08+4*i,0);ff_w32(0x800adc70+4*i,0);
 }
 return 0;
}
uint32 FUN_80013848(void) {
 uint32 i,j,first[5],second[5],result=2;
 for(i=0;i<5;i++){first[i]=ff_u32(0x8008d4e0+4*i);second[i]=ff_u32(0x8008d4f4+4*i);}
 for(i=0;i<8;i++) {
  uint32 p=0x800b4318+244*i;uint8 *a=(uint8*)ff_ptr(p,244);
  ff_w32(p+232,0);ff_w32(p,0);ff_w32(p+4,0xffffffff);ff_w32(p+8,0);ff_w32(p+12,69);
  ff_w16(p+74,0);a[118]=0;a[116]=a[120]=a[125]=a[126]=255;
  for(j=54;j<=60;j+=2)ff_w16(p+j,0);
  if(i>=2)ff_w16(p+52,65535);
  a[119]=ff_s16(p+66)<1?62:0;
 }
 memset(ff_ptr(0x800ba210,4032),0,4032);
 for(i=0;i<2;i++) {
  uint32 p=0x800b4318+244*i,meta=0x800a8588+40*i;sint32 type=ff_s16(p+52);
  if(i && ff_u32(0x800940b0)!=2)break;if(type==-1)continue;
  if(type<0||type>4){fprintf(stderr,"WIP: invalid actor type during reset\n");ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);}
  ff_w32(p+180,meta);ff_w32(p+184,0);
  *(uint8*)ff_ptr(meta+5,1)=(uint8)first[type];*(uint8*)ff_ptr(meta+7,1)=(uint8)second[type];
  ff_w32(p+240,ff_u32(0x800b6e90+4*type));if(i)result=ff_u32(p+240);
 }
 return result;
}

uint32 FUN_800526F0(void) {
 memset(ff_ptr(0x80094640,32),0,32);memset(ff_ptr(0x80094374,2),0,2);return 0x80094374;
}
uint32 FUN_80018318(void) {
 static const uint32 zero_words[]={0x80093dc4,0x80093dc0,0x80093dbc,0x800940a8,
  0x800944d8,0x80094410,0x80093dd4,0x80093dd0,0x80094198,0x80093e04,
  0x80093ddc,0x80093df0,0x80093df8,0x80093df4,0x80094674,0x800945d0};
 static const uint32 later_words[]={0x80093db8,0x80093da8,0x80093dac,0x80093db0,
  0x80093d60,0x800941b8,0x80093d24,0x80093d20};
 uint32 i,stage=ff_u32(0x80093d58),shade;
 ff_w32(0x80093d04,0);ff_w32(0x80093d40,0);*(uint8*)ff_ptr(0x80093d48,1)=0;
 for(i=0;i<128;i++)ff_w32(0x8009d1d0+36*i,0xffffffff);
 for(i=0;i<64;i++)ff_w32(0x8009a3c8+36*i,0xffffffff);
 for(i=0;i<5;i++)ff_w32(0x80094018+4*i,0);
 ff_w32(0x800946b8,ff_u32(0x800a7dc8+4*stage));ff_w32(0x80093e1c,1);
 for(i=0;i<sizeof(zero_words)/sizeof(zero_words[0]);i++)ff_w32(zero_words[i],0);
 ff_w16(0x8008d492,0);*(uint8*)ff_ptr(0x80093dda,1)=0;*(uint8*)ff_ptr(0x80093dd8,1)=0;*(uint8*)ff_ptr(0x80093e00,1)=0;
 if(stage==2)ff_w32(0x80094674,32);
 for(i=0;i<sizeof(later_words)/sizeof(later_words[0]);i++)ff_w32(later_words[i],0);
 ff_w32(0x80093db4,2);*(uint8*)ff_ptr(0x800941a4,1)=0;*(uint8*)ff_ptr(0x80093dd9,1)=0;
 ff_w32(0x80093e1c,0xffffffff);FUN_800526F0();
 ff_w32(0x80094554,0xffffffff);ff_w32(0x80094550,0xffffffff);
 *(uint8*)ff_ptr(0x80094670,1)=stage==2;if(stage==2)*(uint8*)ff_ptr(0x80093dd9,1)=1;
 shade=(stage==0||stage-8<2||stage-10<2||stage-12<2||stage==16||stage==19||stage-21<2||stage==24)?0xffffffd0:0;
 ff_w32(0x800941bc,shade);
 for(i=0;i<8;i++) {
  uint32 p=0x800b4318+244*i;uint8 *a=(uint8*)ff_ptr(p,244);
  a[159]=a[158]=a[157]=(uint8)shade;ff_w16(p+88,65535);ff_w16(p+138,0);
  if(ff_s16(p+66)<=0)a[119]=62;
 }
 ff_w32(0x800947c8,0);memset(ff_ptr(0x800944b8,32),255,32);return 0x800944b8;
}

uint32 FUN_8005FF2C(uint32 name,uint32 type,uint32 texture_group) {
 uint32 p=FUN_800611F8(name),vertices,faces,i; sint32 nv,nf;
 ff_w32(0x800990d8+4*type,ff_u32(p));nv=(sint32)ff_u32(p+8);nf=(sint32)ff_u32(p+16);
 vertices=p+20;faces=vertices+8*(uint32)nv;ff_w32(p+4,vertices);ff_w32(p+12,faces);
 for(i=0;(sint32)i<nv;i++) {
  uint32 a=vertices+8*i;ff_w16(a,ff_s16(a)>>1);ff_w16(a+2,ff_s16(a+2)>>1);ff_w16(a+4,ff_s16(a+4)>>1);
 }
 for(i=0;(sint32)i<nf;i++) {
  uint32 a=faces+12*i;
  if(ff_s16(a+4)!=-1||ff_s16(a+6)!=-1) {
   sint32 index=ff_s16(0x800af418+2*texture_group)+ff_s16(a+10)/12;
   ff_w16(a+10,12*ff_s16(0x8009acc8+2*(uint32)index));
  }
 }
 ff_w32(0x80081720+4*type,p+4);
 if(type-665<2||type==667||type==94){ff_w32(0x800990d8+4*type,0);return 4*type;}
 return 94;
}
uint32 FUN_800600F0(uint32 name,uint32 type,uint32 texture_group) {
 return FUN_8005FF2C(name,type,texture_group);
}

static uint32 resource_string_length(uint32 address) {
 uint32 n=0;if(!address)return 0;while(*(uint8*)ff_ptr(address+n,1))n++;return n;
}
uint32 FUN_8001721C(uint32 name,sint32 type) {
 uint32 table,p;
 if(type==-1)table=0x8008409c;else if(type==-2)table=0x80084520;else if(type==-3)table=0x800849a4;
 else table=ff_u32(0x800b3be8+4*(uint32)type);
 p=FUN_800611F8(name);
 while(*(uint8*)ff_ptr(p,1)) {
  uint32 length=resource_string_length(p),entry;if(length>8)length=8;
  for(entry=0x8007a12c;ff_u32(entry+8)!=0xffffffff;entry+=12) {
   uint32 text=ff_u32(entry),index=ff_u32(entry+8);
   if(resource_string_length(text)==length&&!memcmp(ff_ptr(p,length),ff_ptr(text,length),length)) {
    ff_w32(table+4*index,p+12);ff_w16(0x800b39a0+2*index,ff_s16(entry+4));
   }
  }
  p+=8+ff_u32(p+8);
 }
 return 0;
}
uint32 FUN_80011D74(uint32 address,uint32 base) {
 int64_t sum=(int64_t)(sint32)ff_u32(address)+(sint32)base;
 if(sum>2147483647LL||sum<(-2147483647LL-1)){fprintf(stderr,"MIPS add overflow in relocation\n");ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);}
 ff_w32(address,(uint32)sum);return (uint32)sum;
}
uint32 FUN_80060110(uint32 name,uint32 type,uint32 group) {
 uint32 p=FUN_800611F8(name),source=p+4,size=ff_u32(p+20),dest,i,faces,value=0xff50;
 sint32 count;
 ff_w32(0x800990d8+4*type,ff_u32(p));FUN_80011D74(p+20,source);dest=ff_u32(0x80094844);
 for(i=0;(sint32)i<(sint32)size;i++)*(uint8*)ff_ptr(dest+i,1)=*(uint8*)ff_ptr(source+i,1);
 ff_w32(0x80094844,dest+size);count=(sint32)ff_u32(dest+12);
 FUN_80011D74(dest,dest);FUN_80011D74(dest+8,dest);faces=ff_u32(dest+8);
 for(i=0;(sint32)i<count;i++) {
  uint32 a=faces+12*i;sint32 index=ff_s16(0x800af418+2*group)+ff_s16(a+10)/12;
  ff_w16(a+10,12*ff_s16(0x8009acc8+2*(uint32)index));
 }
 ff_w32(0x80081720+4*type,dest);
 switch(type) {
 case 0:case 5:case 6:case 7:case 8:case 9:case 10:value=0xff58;break;
 case 1:value=0xff68;break;case 2:case 14:case 31:case 49:value=0xff5c;break;
 case 3:value=0xff74;break;case 11:value=0xff54;break;
 case 12:case 13:case 29:case 38:value=0xff60;break;case 48:value=0xff6c;break;
 case 51:value=0xffc0;break;case 28:value=0xffb0;break;
 }
 value<<=16;ff_w32(0x800990d8+4*type,value);return value;
}

uint32 FUN_80061448(uint32 index,uint32 destination) {
 uint32 attempt,name=ff_u32(0x800b9bf0+4*index);
 ff_w32(0x80094818,destination);
 for(attempt=0;attempt<5;attempt++) {
  sint32 size;FUN_80067554(name,destination);size=(sint32)ff_u32(0x80094570);
  if(size>0){ff_w32(0x800947d0,size);ff_w32(0x80094038,destination);return size;}
 }
 fprintf(stderr,"WIP: archive load failed (original fatal loop)\n");ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);return 0;
}
GDB_CALL sint32 FUN_80067554(uint32 name,uint32 destination) {
 FF_FUNCTION_MARKER(0x80067554u,"GAME.EXE");
 uint32 offset=destination&0x1fffff; sint32 size,sectors;
 /* WIP CdSearchFile/Setloc/Read/ReadSync boundary: extracted files are copied
  * synchronously by the existing adapter. No physical CD or padded sectors. */
 size=ff_dummy_archive_read((const char*)ff_ptr(name,1),ff_ptr(destination,1),0x200000-offset);
 if(size<=0){
  ff_wip_visit(0x80067580,"cd",__FUNCTION__,__FILE__,__LINE__,1,"archive unavailable; return failure instead of original infinite error loop");
  ff_w32(0x80094570,0);return -1;
 }
 ff_w32(0x80094570,(uint32)size);sectors=(sint32)((uint32)size+2047)>>11;
 (void)sectors; /* Original read uses sectors and mode80; host completion is0. */
 return 0;
}
uint32 FUN_80040AA0(void) {
 static const uint32 models[][3]={
 {0x80091a48,887,0},{0x80091a54,888,0},{0x80091a60,860,0},
 {0x80091a6c,454,34},{0x80091a7c,455,34},{0x80091a8c,456,34},
 {0x80091a9c,457,34},{0x80091aa8,458,34},{0x80091ab4,459,34},
 {0x80091ac0,832,70},{0x80091ad0,833,70},{0x80091adc,564,70},
 {0x80091ae8,834,70},{0x80091af8,835,70},{0x80091b08,836,70},
 {0x80091b18,837,70},{0x80091b28,838,70},{0x80091b34,839,70},{0x80091b40,840,70}};
 uint32 i,result=0;
 FUN_80061448(66,ff_u32(0x80094844));ff_w32(0x80094844,ff_u32(0x80094844)+ff_u32(0x800947d0));
 for(i=0;i<19;i++) {
  if(i==12||i==15)FUN_800600F0(models[i][0],models[i][1],models[i][2]);
  else FUN_8005FF2C(models[i][0],models[i][1],models[i][2]);
 }
 for(i=0;i<4;i++)FUN_8001721C(0x80091b4c+12*i,i);
 for(i=0;i<4;i++)result=FUN_80060110(0x80091b7c+12*i,i,36+i);
 return result;
}

uint32 FUN_8005ED78(void) {
 ff_w16(0x80093564,640);ff_w16(0x80093566,65535);
 /* Host uploads are synchronous; DrawSync/VSync hardware waits are omitted. */
 ff_gpu_clear_menu_surfaces();ff_w32(0x80094190,0);ff_w32(0x800941a8,0);
 ff_w16(0x80093e14,0);ff_w16(0x80093e12,0);ff_w16(0x80093e10,0);
 FUN_80011CCC(-256,-256,-256);FUN_80011D50(ff_u32(0x8008d4b4));FUN_80064684();
 return FUN_80011D50(ff_u32(0x8008d4b4));
}
uint32 FUN_800645E8(void) {
 if(!ff_gpu_load_image(0x80093b90,0x800b4ab8))ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);return 0;
}

sint32 FUN_80067724(void) {
 ff_dummy_cd_pause();ff_w32(0x80093bac,0xffffffff);ff_w32(0x80093bb4,0);
 return -1;
}

uint32 FUN_80067284(void) {
 uint32 parity=ff_u32(0x8008d4c4)&1,disp=0x800947a0+20*parity;
 /* Synchronous host DrawSync/PutDrawEnv/PutDispEnv boundary. */
 ff_w16(disp+8,(uint16)ff_s16(0x80093d38));ff_w16(disp+10,(uint16)ff_s16(0x80093d3a));
 ff_gpu_begin();ff_gpu_draw_env(0x800b8938+92*parity,320*parity,0);
 FUN_80064684();return FUN_80011D50(ff_u32(0x8008d4b4));
}
uint32 FUN_80066A44(uint32 page,uint32 bucket) {
 uint32 p=ff_u32(0x8008d4b0),model=*(uint8*)ff_ptr(0x800872ec,1);
 ff_w32(0x8008d4b0,p+12);*(uint8*)ff_ptr(p+3,1)=2;
 /* 70048/70344: SetDrawMode(p,1,0,page,NULL), including GPU-model mask.
  * 70580(NULL) returns a NOP word, not a texture-window command. */
 ff_w32(p+4,0xe1000000u|((model==1||model==2)?((page&0x27ff)|0x1000):((page&0x09ff)|0x400)));
 ff_w32(p+8,0);return FUN_80011E40(ff_u32(0x8008d4b4)+4*bucket,p);
}
uint32 FUN_80067358(void) {
 uint32 index=ff_u32(0x80093c14),parity=ff_u32(0x8008d4c4)&1,result;
 if(index!=0xffffffff)FUN_80061ADC(ff_u32(0x80094678+4*index),216);
 if(ff_gpu_ot(ff_u32(0x8008d4b4))<0)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
 ff_gpu_display_offset(ff_s16(0x800947a8+20*parity),ff_s16(0x800947aa+20*parity));ff_gpu_present();
 result=ff_u32(0x8008d4c4)+1;ff_w32(0x8008d4c4,result);
 if(ff_services.frontend_frame)ff_services.frontend_frame();return result;
}
uint32 FUN_80063C54(void) {
 uint32 i,desc,p;FUN_80067284();
 desc=0x800a8768+12*(uint32)ff_s16(0x8009acc8+2*(uint32)ff_s16(0x800af4ae));
 for(i=0;i<3;i++,desc+=12) {
  p=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,p+20);
  /* SetSprt and SetShadeTex preserve RGB and CLUT bytes in the arena. */
  *(uint8*)ff_ptr(p+3,1)=4;*(uint8*)ff_ptr(p+7,1)=0x65;
  *(uint8*)ff_ptr(p+12,1)=*(uint8*)ff_ptr(desc,1);*(uint8*)ff_ptr(p+13,1)=*(uint8*)ff_ptr(desc+1,1);
  ff_w16(p+16,i==2?64:128);ff_w16(p+18,255);ff_w16(p+8,128*i);ff_w16(p+10,0);
  FUN_80011E40(ff_u32(0x8008d4b4)+4,p);FUN_80066A44((uint32)ff_s16(desc+6),1);
 }
 return FUN_80067358();
}
uint32 FUN_80063E50(void) {
 uint32 p;FUN_80067284();p=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,p+24);
 /* SetPolyF4, then SetSemiTrans(1): black 320x256 quad in OT bucket 0. */
 *(uint8*)ff_ptr(p+3,1)=5;ff_w32(p+4,0x2a000000);
 ff_w16(p+8,0);ff_w16(p+10,0);ff_w16(p+12,320);ff_w16(p+14,0);
 ff_w16(p+16,0);ff_w16(p+18,256);ff_w16(p+20,320);ff_w16(p+22,256);
 FUN_80011E40(ff_u32(0x8008d4b4),p);return FUN_80067358();
}
uint32 FUN_80069CC8(void) {
 uint32 i,desc,p;FUN_80067284();
 if(ff_s16(0x8001000c)+ff_s16(0x8001000e)+ff_s16(0x80010010))FUN_8006471C();
 desc=0x800a8768+12*(uint32)ff_s16(0x8009acc8+2*(uint32)ff_s16(0x800af4ac));
 for(i=0;i<3;i++,desc+=12) {
  p=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,p+20);
  *(uint8*)ff_ptr(p+3,1)=4;ff_w32(p+4,0x64808080);
  *(uint8*)ff_ptr(p+12,1)=*(uint8*)ff_ptr(desc,1);*(uint8*)ff_ptr(p+13,1)=*(uint8*)ff_ptr(desc+1,1);
  ff_w16(p+16,i==2?64:128);ff_w16(p+18,255);ff_w16(p+8,128*i);ff_w16(p+10,65529);
  FUN_80011E40(ff_u32(0x8008d4b4)+4,p);FUN_80066A44((uint32)ff_s16(desc+6),1);
 }
 /* Opaque background is linked last in bucket 0; submission is the caller's job. */
 p=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,p+24);
 *(uint8*)ff_ptr(p+3,1)=5;ff_w32(p+4,0x28000000);
 ff_w16(p+8,0);ff_w16(p+10,0);ff_w16(p+12,320);ff_w16(p+14,0);
 ff_w16(p+16,0);ff_w16(p+18,256);ff_w16(p+20,320);ff_w16(p+22,256);
 return FUN_80011E40(ff_u32(0x8008d4b4),p);
}
uint32 FUN_80057630(void) {
 uint32 i=0,count;FUN_800645E8();FUN_80063E50();FUN_80063E50();FUN_80063E50();
 count=ff_u32(0x800940b0);if((sint32)count<=0)return count;
 do {FUN_8005E9E8((sint32)i);i++;} while((sint32)i<(sint32)ff_u32(0x800940b0));
 return 0;
}
uint32 FUN_80069F78(void) {
 uint32 edge; sint32 level;
 ff_w32(0x80094190,0);ff_w32(0x800941a8,0);FUN_800575F8();
 /* DrawSync(0) is satisfied by the synchronous host GPU adapter. */
 FUN_80061448(170,0x116000);FUN_80060394(0);FUN_800645E8();
 FUN_80011CCC(-256,-256,-256);FUN_80063870(24);
 ff_w32(0x80092790,0xffffffff);ff_w32(0x80093dd0,0);
 for(;;) {
  if((sint32)ff_u32(0x80093dd0)<17)FUN_8005EE58((sint32)(ff_u32(0x80093dd0)<<4));
  FUN_80069CC8();
  if(ff_u32(0x80093dd0)&16)FUN_80061ADC(ff_u32(0x80092444),170);
  FUN_80067358();edge=FUN_80068114(0);
  if(ff_u32(0x800940b0)==2)edge|=FUN_80068114(1);
  if((edge&8)||ff_u32(0x80093dd0)==600)break;
  ff_w32(0x80093dd0,ff_u32(0x80093dd0)+1);
 }
 for(level=256;level>0;level-=16){FUN_8005EE58(level);FUN_80069CC8();FUN_80067358();}
 FUN_8005ED78();return ff_u32(0x80093dd0)==600;
}
/* 4FBCC..4FEF4: return 0 at the first frame prefix, 1 on title timeout. */
uint32 ff_menu_startup_8004FBCC(void) {
 uint32 actor,p,yaw,i; sint32 angle=-192;
 ff_w32(0x80093d68,0);memset(ff_ptr(0x800b4318,1952),0,1952);FUN_80014498();
 ff_w32(0x80093d50,0);FUN_8005EC90();if(FUN_80069F78())return 1;
 FUN_8005ECCC(64);FUN_80018318();FUN_80013A30();FUN_80013848();
 ff_w32(0x80093d58,26);FUN_80061448(31,0x116000);
 ff_w32(0x800946bc,ff_u32(0x800b3e68));
 if(ff_u32(0x800a86d8+4*26)!=0x8005e7c0)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);FUN_8005E7C0();
 FUN_80060394(1);FUN_80056CCC(197);
 ff_w16(0x800b434c,3);ff_w16(0x800b4534,1);ff_w16(0x800b4440,0);ff_w16(0x800b4628,2);
 FUN_80040AA0();ff_w32(0x80092784,ff_u32(0x800940b0));
 /* MIPS compares with the address B46E8, not a pointer loaded there. */
 for(actor=0x800b4318;actor<0x800b46e8;actor+=244,angle+=128) {
  p=actor+52;yaw=(uint32)angle&2047;ff_w16(p+4,(uint16)yaw);
  *(uint8*)ff_ptr(p+70,1)=255;ff_w16(p+6,0);ff_w16(p+8,0);ff_w16(p+2,0);ff_w16(p+10,(uint16)yaw);
  ff_w32(p-12,(uint32)(sint32)ff_s16(0x800802d0+2*yaw)<<10);
  ff_w32(p-4,0xf3000000u-((uint32)(sint32)ff_s16(0x800802d0+2*((yaw+512)&2047))<<10));
  ff_w32(actor,0);ff_w32(p-48,0xffffffff);ff_w32(p-44,0);ff_w32(p-40,69);
  FUN_80019F00(actor,ff_u32(0x800b3be8+4*(uint32)ff_s16(p)),69);
 }
 FUN_8005EC90();FUN_8005FB00(0);FUN_8005FB00(1);FUN_8006A50C();FUN_80057630();
 ff_w16(0x8008d492,0xd100);ff_w32(0x8008d4a8,0xfef00000);
 ff_w16(0x800941a6,0);ff_w16(0x8009419c,0);
 for(i=0;i<3;i++){ff_w32(0x800942c8+8*i,0);ff_w32(0x80094474+4*i,0);}
 ff_w16(0x8008d490,0);ff_w32(0x8008d4a4,0);ff_w32(0x8008d4ac,0xef800000);
 FUN_8004F980((const uint32*)ff_ptr(0x8008d4a4,12),0,-12032);FUN_80063870(24);
 ff_w32(0x80092788,0);ff_w32(0x8009278c,0);*(uint8*)ff_ptr(0x80093cf8,1)=0;
 ff_w32(0x80092738,(uint32)FUN_80056670(0,0,7936));ff_w32(0x80092744,(uint32)FUN_80056670(1,0,7936));
 ff_w32(0x80092794,0xffffffff);ff_w32(0x80092790,0xffffffff);ff_w32(0x800927ec,0x80050270);
 for(i=0;i<4;i++)ff_w32(0x8009276c+4*i,0);
 *(uint8*)ff_ptr(0x80094000,1)=0;ff_w32(0x80092730,0);ff_w32(0x8009272c,0);ff_w32(0x80092734,0);return 0;
}
uint32 FUN_8005ECCC(uint32 steps) {
 if(ff_services.audit_input_event)ff_services.audit_input_event(3);
 FUN_8005ED78();ff_w32(0x80094844,ff_u32(0x80093cfc));
 ff_w16(0x80093564,640);ff_w16(0x80093566,65535);FUN_80067724();
 FUN_80061448(0,0x116000);FUN_80060394(0);FUN_800645E8();
 FUN_80063C54();FUN_80063C54();FUN_80063C54();FUN_80011CCC(-256,-256,-256);
 ff_w16(0x80093564,77);ff_w16(0x80093566,(uint16)(steps-1));ff_w32(0x80093e0c,0);return 77;
}

uint32 FUN_80046854(void) {FUN_800456CC();FUN_80045BE4();return FUN_80046244();}
uint32 FUN_8003EE4C(void) {FUN_8003BB1C();FUN_8003D370();return FUN_8003ED10();}
/* BEGIN constant initializers: tools/translate_const_init.py */
static void ff_init_copy289(uint32 destination,uint32 source) {
 uint32 i,a,b,c,d;
 for(i=0;i<288;i+=4) {
  a=ff_u32(source+4*i);b=ff_u32(source+4*i+4);c=ff_u32(source+4*i+8);d=ff_u32(source+4*i+12);
  ff_w32(destination+4*i,a);ff_w32(destination+4*i+4,b);ff_w32(destination+4*i+8,c);ff_w32(destination+4*i+12,d);
 }
 ff_w32(destination+1152,ff_u32(source+1152));
}
uint32 FUN_800456CC(void) {
 ff_w32(0x800bbe70,0x80091bfc); /* 800456DC */
 ff_w32(0x800ba078,0x80091c08); /* 800456F0 */
 ff_w32(0x800b9c1c,0x80091c14); /* 80045704 */
 ff_w32(0x800b9cac,0x80091c2c); /* 80045710 */
 ff_w32(0x800b9eb4,0x80091c48); /* 8004571C */
 ff_w32(0x800bbec0,0x80091c60); /* 80045728 */
 ff_w32(0x800ba0c8,0x80091c6c); /* 80045734 */
 ff_w32(0x800b9c6c,0x80091c78); /* 80045740 */
 ff_w32(0x800b9cf8,0x80091c90); /* 8004574C */
 ff_w32(0x800b9f04,0x80091cac); /* 80045758 */
 ff_w32(0x800b9d44,0x80091cc4); /* 80045764 */
 ff_w32(0x800b9d48,0x80091cd8); /* 80045770 */
 ff_w32(0x800b9d4c,0x80091cec); /* 8004577C */
 ff_w32(0x800b9d50,0x80091d00); /* 80045788 */
 ff_w32(0x800b9d54,0x80091d14); /* 80045794 */
 ff_w32(0x800b9d58,0x80091d28); /* 800457A0 */
 ff_w32(0x800b9d5c,0x80091d3c); /* 800457AC */
 ff_w32(0x800b9d60,0x80091d50); /* 800457B8 */
 ff_w32(0x800b9d64,0x80091d64); /* 800457C4 */
 ff_w32(0x800b9d68,0x80091d7c); /* 800457D0 */
 ff_w32(0x800b9d6c,0x80091d94); /* 800457DC */
 ff_w32(0x800b9d70,0x80091da8); /* 800457E8 */
 ff_w32(0x800b9d74,0x80091dc0); /* 800457F4 */
 ff_w32(0x800b9d78,0x80091dd4); /* 80045800 */
 ff_w32(0x800b9d7c,0x80091de8); /* 8004580C */
 ff_w32(0x800b9d80,0x80091dfc); /* 80045818 */
 ff_w32(0x800b9d84,0x80091e10); /* 80045824 */
 ff_w32(0x800b9d88,0x80091e24); /* 80045830 */
 ff_w32(0x800b9d8c,0x80091e38); /* 8004583C */
 ff_w32(0x800b9d90,0x80091e4c); /* 80045848 */
 ff_w32(0x800b9d94,0x80091e60); /* 80045854 */
 ff_w32(0x800b9d98,0x80091e74); /* 80045860 */
 ff_w32(0x800b9d9c,0x80091e88); /* 8004586C */
 ff_w32(0x800b9da0,0x80091e9c); /* 80045878 */
 ff_w32(0x800b9da4,0x80091eb0); /* 80045884 */
 ff_w32(0x800b9d38,0x80091ec4); /* 80045890 */
 ff_w32(0x800b9d3c,0x80091ed8); /* 8004589C */
 ff_w32(0x800b9d40,0x80091ef0); /* 800458A8 */
 ff_w32(0x800b9da8,0x80091f08); /* 800458B4 */
 ff_w32(0x800b9dac,0x80091f1c); /* 800458C0 */
 ff_w32(0x800b9db0,0x80091f30); /* 800458CC */
 ff_w32(0x800b9db4,0x80091f44); /* 800458D8 */
 ff_w32(0x800b9db8,0x80091f58); /* 800458E4 */
 ff_w32(0x800b9dbc,0x80091f6c); /* 800458F0 */
 ff_w32(0x800b9dc0,0x80091f80); /* 800458FC */
 ff_w32(0x800b9dc4,0x80091f94); /* 80045908 */
 ff_w32(0x800b9dc8,0x80091fa8); /* 80045914 */
 ff_w32(0x800b9dcc,0x80091fbc); /* 80045920 */
 ff_w32(0x800b9dd0,0x80091fd0); /* 8004592C */
 ff_w32(0x800b9dd4,0x80091fe4); /* 80045938 */
 ff_w32(0x800b9dd8,0x80091ff8); /* 80045944 */
 ff_w32(0x800b9ddc,0x8009200c); /* 80045950 */
 ff_w32(0x800b9de0,0x80092020); /* 8004595C */
 ff_w32(0x800b9de4,0x80092034); /* 80045968 */
 ff_w32(0x800b9de8,0x80092048); /* 80045974 */
 ff_w32(0x800b9dec,0x8009205c); /* 80045980 */
 ff_w32(0x800b9df0,0x80092070); /* 8004598C */
 ff_w32(0x800b9df4,0x80092070); /* 80045990 */
 ff_w32(0x800b9df8,0x80092084); /* 8004599C */
 ff_w32(0x800b9dfc,0x80092098); /* 800459A8 */
 ff_w32(0x800b9e00,0x800920ac); /* 800459B4 */
 ff_w32(0x800b9e04,0x800920c0); /* 800459C0 */
 ff_w32(0x800b9e08,0x800920d4); /* 800459CC */
 ff_w32(0x800b9e0c,0x800920e8); /* 800459D8 */
 ff_w32(0x800b9e10,0x800920fc); /* 800459E4 */
 ff_w32(0x800b9e14,0x80092110); /* 800459F0 */
 ff_w32(0x800b9e18,0x80092124); /* 800459FC */
 ff_w32(0x800b9e1c,0x80092138); /* 80045A08 */
 ff_w32(0x800b9e20,0x8009214c); /* 80045A14 */
 ff_w32(0x800b9e24,0x8009214c); /* 80045A18 */
 ff_w32(0x800b9e28,0x80092160); /* 80045A24 */
 ff_w32(0x800b9e2c,0x80092174); /* 80045A30 */
 ff_w32(0x800b9e30,0x80092188); /* 80045A3C */
 ff_w32(0x800b9e34,0x8009219c); /* 80045A48 */
 ff_w32(0x800b9e38,0x800921b0); /* 80045A54 */
 ff_w32(0x800b9e3c,0x800921c4); /* 80045A60 */
 ff_w32(0x800b9e40,0x800921d8); /* 80045A6C */
 ff_w32(0x800b9e44,0x800921ec); /* 80045A78 */
 ff_w32(0x800b9e48,0x80092200); /* 80045A84 */
 ff_w32(0x800b9e4c,0x80092214); /* 80045A90 */
 ff_w32(0x800b9e50,0x80092228); /* 80045A9C */
 ff_w32(0x800b9e54,0x8009223c); /* 80045AA8 */
 ff_w32(0x800b9e58,0x8009223c); /* 80045AAC */
 ff_w32(0x800b9e5c,0x80092250); /* 80045AB8 */
 ff_w32(0x800b9e60,0x80092264); /* 80045AC4 */
 ff_w32(0x800b9e64,0x80092278); /* 80045AD0 */
 ff_w32(0x800b9e68,0x8009228c); /* 80045ADC */
 ff_w32(0x800b9e6c,0x800922a0); /* 80045AE8 */
 ff_w32(0x800b9e70,0x800922b4); /* 80045AF4 */
 ff_w32(0x800b9e74,0x800922c8); /* 80045B00 */
 ff_w32(0x800b9e78,0x800922dc); /* 80045B0C */
 ff_w32(0x800b9e7c,0x800922f0); /* 80045B18 */
 ff_w32(0x800b9e80,0x80092304); /* 80045B24 */
 ff_w32(0x800b9e84,0x80092318); /* 80045B30 */
 ff_w32(0x800b9e88,0x80092318); /* 80045B34 */
 ff_w32(0x800b9e8c,0x8009232c); /* 80045B40 */
 ff_w32(0x800b9e90,0x80092340); /* 80045B4C */
 ff_w32(0x800b9e94,0x80092354); /* 80045B58 */
 ff_w32(0x800bbf70,0x80092368); /* 80045B64 */
 ff_w32(0x800ba178,0x80092374); /* 80045B70 */
 ff_w16(0x800943ac,0x0); /* 80045B78 */
 ff_w16(0x800943f4,0x0); /* 80045B80 */
 ff_w16(0x8009431c,0x0); /* 80045B88 */
 ff_w32(0x800b3e68,0x8007b3e4); /* 80045B98 */
 ff_w32(0x800a7e30,0x8007efa8); /* 80045BA8 */
 ff_w32(0x800a8740,0x8005e7c0); /* 80045BB8 */
 ff_w32(0x8009cc70,0x80040aa0); /* 80045BC8 */
 ff_w32(0x800adc48,0x80040a48); /* 80045BD8 */
 return 0x80040a48;
}
uint32 FUN_8003EF78(void) {
 ff_w32(0x800bc430,0x8003fdcc); /* 8003EF84 */
 ff_w32(0x800bc598,0x8003f9c0); /* 8003EF94 */
 ff_w32(0x800bc514,0x8003f410); /* 8003EFA4 */
 ff_w32(0x800bcbac,0x8003f088); /* 8003EFB4 */
 ff_w32(0x800bcbb0,0x8003f388); /* 8003EFC4 */
 ff_w32(0x800979a4,0x8003ee7c); /* 8003EFD4 */
 ff_w16(0x800a755e,0xfb6b); /* 8003EFE0 */
 return 0xfffffb6b;
}
uint32 FUN_8003ED10(void) {
 ff_w32(0x800a8600,0x64); /* 8003ED1C */
 ff_w32(0x800a8604,0x64); /* 8003ED20 */
 ff_w32(0x800a8608,0x64); /* 8003ED24 */
 ff_w32(0x800a860c,0x64); /* 8003ED28 */
 ff_w32(0x800a8610,0x64); /* 8003ED2C */
 ff_w32(0x800a8614,0x64); /* 8003ED30 */
 ff_w32(0x800a8618,0xc8); /* 8003ED38 */
 ff_w32(0x800a861c,0x96); /* 8003ED40 */
 ff_w32(0x800a8620,0x96); /* 8003ED44 */
 ff_w32(0x800a8624,0xfa); /* 8003ED4C */
 ff_w32(0x800a8628,0xc8); /* 8003ED50 */
 ff_w32(0x800a862c,0x64); /* 8003ED54 */
 ff_w32(0x800a8630,0x2ee); /* 8003ED5C */
 ff_w32(0x800a8638,0x249f0); /* 8003ED68 */
 ff_w32(0x800a863c,0x64); /* 8003ED6C */
 ff_w32(0x800a8648,0x15e); /* 8003ED74 */
 ff_w32(0x800a864c,0x1c2); /* 8003ED7C */
 ff_w32(0x800a8650,0x226); /* 8003ED84 */
 ff_w32(0x800a8654,0x28a); /* 8003ED8C */
 ff_w32(0x800a8658,0x320); /* 8003ED94 */
 ff_w32(0x800a865c,0x1f4); /* 8003ED9C */
 ff_w32(0x800a8660,0x28a); /* 8003EDA0 */
 ff_w32(0x800a8664,0x2ee); /* 8003EDA4 */
 ff_w32(0x800a8668,0x4e2); /* 8003EDAC */
 ff_w32(0x800a866c,0x12c); /* 8003EDB4 */
 ff_w32(0x800a8670,0x64); /* 8003EDB8 */
 ff_w32(0x800a8674,0x3e8); /* 8003EDC0 */
 ff_w32(0x800a8678,0x320); /* 8003EDC4 */
 ff_w32(0x800a867c,0x1f4); /* 8003EDC8 */
 ff_w32(0x800a8680,0x2710); /* 8003EDD0 */
 ff_w32(0x800a8684,0x4e2); /* 8003EDD4 */
 ff_w32(0x800a8688,0x5dc); /* 8003EDDC */
 ff_w32(0x800a868c,0x258); /* 8003EDE4 */
 ff_w32(0x800a8690,0x320); /* 8003EDE8 */
 ff_w32(0x800a8694,0x3e8); /* 8003EDEC */
 ff_w32(0x800a8698,0x3e8); /* 8003EDF0 */
 ff_w32(0x800a869c,0x4b0); /* 8003EDF8 */
 ff_w32(0x800a86a0,0x3e8); /* 8003EDFC */
 ff_w32(0x800a86a4,0x4e2); /* 8003EE00 */
 ff_w32(0x800a86a8,0x5dc); /* 8003EE04 */
 ff_w32(0x800a86ac,0x6a4); /* 8003EE0C */
 ff_w32(0x800a86b0,0x1f4); /* 8003EE10 */
 ff_w32(0x800a86b4,0x258); /* 8003EE14 */
 ff_w32(0x800a86b8,0x2bc); /* 8003EE1C */
 ff_w32(0x800a86bc,0x384); /* 8003EE24 */
 ff_w32(0x800a86c0,0x124f8); /* 8003EE30 */
 ff_w32(0x800a86c4,0x61a8); /* 8003EE38 */
 ff_w32(0x800a86c8,0x124f8); /* 8003EE3C */
 ff_w32(0x800a86d0,0x1f4); /* 8003EE40 */
 ff_w32(0x800a86d4,0x320); /* 8003EE48 */
 return 0x800a8600;
}
uint32 FUN_80046244(void) {
 ff_w16(0x800a7910,0xce00); /* 80046250 */
 ff_w16(0x800a7912,0xfd00); /* 80046258 */
 ff_w16(0x800a7856,0xfb00); /* 80046260 */
 ff_w16(0x800a787a,0xfec0); /* 80046268 */
 ff_w16(0x800a7878,0xfec0); /* 8004626C */
 ff_w16(0x800a77ee,0xfb80); /* 80046274 */
 ff_w16(0x800a77fa,0xfdf0); /* 8004627C */
 ff_w16(0x8009eb1c,0x4800); /* 8004628C */
 ff_w16(0x8009eb1e,0x0); /* 80046290 */
 ff_w16(0x8009eb20,0x0); /* 80046294 */
 ff_w16(0x8009eb22,0x30); /* 8004629C */
 ff_w16(0x8009eb24,0xffc0); /* 800462A4 */
 ff_w16(0x8009eb26,0x10); /* 800462AC */
 ff_w16(0x8009eb28,0x0); /* 800462B0 */
 ff_w16(0x8009eb2a,0x500); /* 800462B8 */
 ff_w16(0x8009eb2c,0x0); /* 800462BC */
 ff_w16(0x8009eb2e,0x30); /* 800462C0 */
 ff_w16(0x8009eb30,0xffa0); /* 800462C8 */
 ff_w16(0x8009eb32,0x0); /* 800462CC */
 ff_w16(0x8009eb34,0xf800); /* 800462D4 */
 ff_w16(0x8009eb36,0xff00); /* 800462DC */
 ff_w16(0x8009eb38,0x0); /* 800462E0 */
 ff_w16(0x8009eb3a,0x28); /* 800462E8 */
 ff_w16(0x8009eb3c,0xffc0); /* 800462EC */
 ff_w16(0x8009eb3e,0x0); /* 800462F0 */
 ff_w16(0x8009eb64,0x0); /* 800462F4 */
 ff_w16(0x8009eb66,0x4000); /* 800462FC */
 ff_w16(0x8009eb68,0x2d00); /* 80046304 */
 ff_w16(0x8009eb6a,0xffea); /* 8004630C */
 ff_w16(0x8009eb6c,0xe); /* 80046314 */
 ff_w16(0x8009eb6e,0x9); /* 8004631C */
 ff_w16(0x8009eb70,0x0); /* 80046320 */
 ff_w16(0x8009eb72,0x4000); /* 80046324 */
 ff_w16(0x8009eb74,0x2d00); /* 80046328 */
 ff_w16(0x8009eb76,0xffea); /* 8004632C */
 ff_w16(0x8009eb78,0xe); /* 80046330 */
 ff_w16(0x8009eb7a,0x9); /* 80046334 */
 ff_w16(0x8009ec00,0x4300); /* 8004633C */
 ff_w16(0x8009ec02,0x0); /* 80046340 */
 ff_w16(0x8009ec04,0x1000); /* 80046348 */
 ff_w16(0x8009ec06,0x20); /* 80046350 */
 ff_w16(0x8009ec08,0xffb0); /* 80046358 */
 ff_w16(0x8009ec0a,0x28); /* 8004635C */
 ff_w16(0x8009ea38,0x0); /* 80046360 */
 ff_w16(0x8009ea3a,0x0); /* 80046364 */
 ff_w16(0x8009ea3c,0x0); /* 80046368 */
 ff_w16(0x8009ea3e,0x10); /* 8004636C */
 ff_w16(0x8009ea40,0x8); /* 80046374 */
 ff_w16(0x8009ea42,0x0); /* 80046378 */
 ff_w16(0x8009ea44,0x4800); /* 8004637C */
 ff_w16(0x8009ea46,0x0); /* 80046380 */
 ff_w16(0x8009ea48,0x0); /* 80046384 */
 ff_w16(0x8009ea4a,0x18); /* 8004638C */
 ff_w16(0x8009ea4c,0x0); /* 80046390 */
 ff_w16(0x8009ea4e,0xfff8); /* 80046398 */
 ff_w16(0x8009ea50,0x1000); /* 8004639C */
 ff_w16(0x8009ea52,0xf000); /* 800463A4 */
 ff_w16(0x8009ea54,0x0); /* 800463A8 */
 ff_w16(0x8009ea56,0x10); /* 800463AC */
 ff_w16(0x8009ea58,0x8); /* 800463B0 */
 ff_w16(0x8009ea5a,0x10); /* 800463B4 */
 ff_w32(0x800bcd78,0x80044d90); /* 800463C8 */
 ff_w32(0x800c0d00,0x800450f8); /* 800463D8 */
 ff_w32(0x800bcc78,0x8004348c); /* 800463E4 */
 ff_w32(0x800bcc7c,0x80043520); /* 800463F0 */
 ff_w32(0x800bc3a8,0x800403dc); /* 800463FC */
 ff_w32(0x800bcb04,0x8004322c); /* 80046408 */
 ff_w32(0x800bcb08,0x80042ba0); /* 80046414 */
 ff_w32(0x800bcb2c,0x80042f80); /* 80046420 */
 ff_w32(0x800bc7ec,0x8002ba68); /* 8004642C */
 ff_w32(0x800bc7f0,0x8002ba68); /* 80046430 */
 ff_w32(0x800bcb40,0x80043128); /* 8004643C */
 ff_w32(0x800bcb50,0x80043680); /* 80046448 */
 ff_w32(0x800bcb54,0x80043680); /* 8004644C */
 ff_w32(0x800bcb58,0x80043680); /* 80046450 */
 ff_w32(0x800bcb5c,0x80043680); /* 80046454 */
 ff_w32(0x800bcb60,0x80043680); /* 80046458 */
 ff_w32(0x800bcb64,0x80043680); /* 8004645C */
 ff_w32(0x800bcb68,0x80043680); /* 80046460 */
 ff_w32(0x800bcb6c,0x80043680); /* 80046464 */
 ff_w32(0x800bcb70,0x80043680); /* 80046468 */
 ff_w32(0x800bcb74,0x80043680); /* 8004646C */
 ff_w32(0x800bcb78,0x80043680); /* 80046470 */
 ff_w32(0x800bcb7c,0x80043680); /* 80046474 */
 ff_w32(0x800bc5cc,0x800429f0); /* 80046480 */
 ff_w32(0x800bc5c8,0x8004298c); /* 8004648C */
 ff_w32(0x800bc218,0x80043978); /* 80046498 */
 ff_w32(0x800bc210,0x80044334); /* 800464A4 */
 ff_w32(0x800bc1f8,0x80044844); /* 800464B0 */
 ff_w32(0x800bc234,0x8004471c); /* 800464BC */
 ff_w32(0x800bc230,0x80041d60); /* 800464C8 */
 ff_w32(0x800bca34,0x800420f8); /* 800464D4 */
 ff_w32(0x800bca4c,0x80041dd8); /* 800464E0 */
 ff_w32(0x800bc234,0x8004471c); /* 800464E4 */
 ff_w32(0x800bc238,0x8004471c); /* 800464E8 */
 ff_w32(0x800bc23c,0x8004471c); /* 800464EC */
 ff_w32(0x800bc240,0x8004471c); /* 800464F0 */
 ff_w32(0x800bc244,0x8004471c); /* 800464F4 */
 ff_w32(0x800bc248,0x8004471c); /* 800464F8 */
 ff_w32(0x800bc24c,0x8004471c); /* 800464FC */
 ff_w32(0x800bc250,0x8004471c); /* 80046500 */
 ff_w32(0x800bc2a8,0x80041e6c); /* 8004650C */
 ff_w32(0x800bc294,0x8004483c); /* 80046518 */
 ff_w32(0x800bcae4,0x800419d4); /* 80046524 */
 ff_w32(0x80097838,0x80044cd4); /* 80046538 */
 ff_w32(0x8009810c,0x800451a0); /* 80046544 */
 ff_w32(0x80098114,0x800452ac); /* 80046550 */
 ff_w32(0x80098118,0x800452ac); /* 80046554 */
 ff_w32(0x80098108,0x800455dc); /* 80046560 */
 ff_w32(0x80097f94,0x800454e0); /* 8004656C */
 ff_w32(0x80097fb8,0x8005c0e8); /* 80046578 */
 ff_w32(0x80097c7c,0x8005c0e8); /* 8004657C */
 ff_w32(0x80097c80,0x8005c0e8); /* 80046580 */
 ff_w32(0x80097f98,0x8005c0e8); /* 80046584 */
 ff_w32(0x80097cbc,0x8005c0e8); /* 80046588 */
 ff_w32(0x80097cc0,0x8005c0e8); /* 8004658C */
 ff_w32(0x80097cc4,0x8005c0e8); /* 80046590 */
 ff_w32(0x80097cc8,0x8005c0e8); /* 80046594 */
 ff_w32(0x80097f9c,0x8005c0e8); /* 80046598 */
 ff_w32(0x80097fa0,0x8005c0e8); /* 8004659C */
 ff_w32(0x80097fa4,0x8005c0e8); /* 800465A0 */
 ff_w32(0x80097fa8,0x8005c0e8); /* 800465A4 */
 ff_w32(0x80097fac,0x8005c0e8); /* 800465A8 */
 ff_w32(0x80097fb0,0x8005c0e8); /* 800465AC */
 ff_w32(0x80097fb4,0x8005c0e8); /* 800465B0 */
 ff_w32(0x80097fbc,0x8005c0e8); /* 800465B4 */
 ff_w32(0x80097fc0,0x8005c0e8); /* 800465B8 */
 ff_w32(0x80097fc4,0x8005c0e8); /* 800465BC */
 ff_w32(0x80097fc8,0x8005c0e8); /* 800465C0 */
 ff_w32(0x80097fcc,0x8005c0e8); /* 800465C4 */
 ff_w32(0x80097fd0,0x8005c0e8); /* 800465C8 */
 ff_w32(0x80097fd4,0x8005b71c); /* 800465D4 */
 ff_w32(0x80097fd8,0x8005c0e8); /* 800465D8 */
 ff_w32(0x80097fdc,0x8005c0e8); /* 800465DC */
 ff_w32(0x80097ec4,0x8004277c); /* 800465E8 */
 ff_w32(0x800bf388,0x800417ec); /* 800465FC */
 ff_w32(0x800bf38c,0x80041600); /* 80046608 */
 ff_w32(0x800bf390,0x8004160c); /* 80046614 */
 ff_w32(0x800bf394,0x800416e0); /* 80046620 */
 ff_w32(0x800bf3a0,0x800419a4); /* 8004662C */
 ff_w32(0x800bf398,0x800415f4); /* 80046638 */
 ff_w32(0x80099018,0x8005c808); /* 8004664C */
 ff_w32(0x8009901c,0x8005cdbc); /* 80046658 */
 ff_w32(0x80099020,0x8005cee4); /* 80046664 */
 ff_w32(0x80099024,0x8005c808); /* 80046668 */
 ff_w32(0x80099030,0x8005c808); /* 8004666C */
 ff_w32(0x80099028,0x8005d024); /* 80046678 */
 ff_w32(0x80094090,0x800226c4); /* 80046688 */
 ff_w16(0x800944e0,0xc0); /* 80046694 */
 ff_w16(0x800944e2,0xe0); /* 800466A0 */
 ff_w16(0x800944e4,0x120); /* 800466AC */
 ff_w16(0x800944e6,0x90); /* 800466B8 */
 ff_w16(0x800944e8,0xfe); /* 800466C4 */
 ff_w16(0x800944ea,0xe0); /* 800466CC */
 ff_w16(0x800944ec,0xe0); /* 800466D4 */
 ff_w16(0x800944ee,0xe0); /* 800466DC */
 ff_w16(0x800944f0,0xe0); /* 800466E4 */
 ff_w16(0x800944f2,0xe0); /* 800466EC */
 ff_w16(0x800944f4,0xe0); /* 800466F4 */
 ff_w16(0x8009453e,0xe0); /* 800466FC */
 ff_w16(0x800944f6,0xe0); /* 80046704 */
 ff_w16(0x800944f8,0xe0); /* 8004670C */
 ff_w16(0x800944fa,0xe0); /* 80046714 */
 ff_w16(0x800944fc,0x120); /* 8004671C */
 ff_w16(0x800944fe,0xe0); /* 80046724 */
 ff_w16(0x80094500,0xe0); /* 8004672C */
 ff_w16(0x80094504,0xe0); /* 80046734 */
 ff_w16(0x80094506,0xe0); /* 8004673C */
 ff_w16(0x80094508,0xe0); /* 80046744 */
 ff_w16(0x8009450a,0xe0); /* 8004674C */
 ff_w16(0x8009450c,0xe0); /* 80046754 */
 ff_w16(0x8009450e,0xe0); /* 8004675C */
 ff_w16(0x80094510,0xe0); /* 80046764 */
 ff_w16(0x80094512,0xe0); /* 8004676C */
 ff_w16(0x80094514,0xe0); /* 80046774 */
 ff_w16(0x80094516,0xe0); /* 8004677C */
 ff_w16(0x80094518,0xe0); /* 80046784 */
 ff_w16(0x8009451a,0xe0); /* 8004678C */
 ff_w16(0x8009451c,0xe0); /* 80046794 */
 ff_w16(0x8009451e,0x120); /* 8004679C */
 ff_w16(0x80094520,0x120); /* 800467A4 */
 ff_w16(0x80094522,0xe0); /* 800467AC */
 ff_w16(0x80094524,0xe0); /* 800467B4 */
 ff_w16(0x80094526,0xe0); /* 800467BC */
 ff_w16(0x80094528,0xe0); /* 800467C4 */
 ff_w16(0x8009452a,0xe0); /* 800467CC */
 ff_w16(0x8009452c,0xe0); /* 800467D4 */
 ff_w16(0x8009452e,0xe0); /* 800467DC */
 ff_w16(0x80094530,0xe0); /* 800467E4 */
 ff_w16(0x80094532,0xe0); /* 800467EC */
 ff_w16(0x80094534,0xe0); /* 800467F4 */
 ff_w16(0x80094536,0xe0); /* 800467FC */
 ff_w16(0x80094538,0xe0); /* 80046804 */
 ff_w16(0x8009453a,0xe0); /* 8004680C */
 ff_w16(0x8009453c,0xe0); /* 80046814 */
 ff_w16(0x8009453e,0xe0); /* 8004681C */
 ff_w16(0x80094540,0xe0); /* 80046824 */
 ff_w16(0x80094542,0x120); /* 8004682C */
 ff_w16(0x80094544,0x140); /* 80046838 */
 ff_w16(0x80094548,0xe0); /* 80046840 */
 ff_w16(0x8009454a,0xe0); /* 80046848 */
 return 0xe0;
}
uint32 FUN_8003BB1C(void) {
 uint32 i;
 ff_w32(0x800bbe38,0x800908a4); /* 8003BB2C */
 ff_w32(0x800bbf54,0x800908b4); /* 8003BB38 */
 ff_w32(0x800bbf50,0x800908c0); /* 8003BB44 */
 ff_w32(0x800bbf5c,0x800908cc); /* 8003BB50 */
 ff_w32(0x800bbec8,0x800908dc); /* 8003BB5C */
 ff_w32(0x800bbecc,0x800908e8); /* 8003BB68 */
 ff_w32(0x800bbed0,0x800908f4); /* 8003BB74 */
 ff_w32(0x800bbed4,0x80090900); /* 8003BB80 */
 ff_w32(0x800bbed8,0x8009090c); /* 8003BB8C */
 ff_w32(0x800bbedc,0x80090918); /* 8003BB98 */
 ff_w32(0x800bbee0,0x80090924); /* 8003BBA4 */
 ff_w32(0x800bbee4,0x80090930); /* 8003BBB0 */
 ff_w32(0x800bbee8,0x8009093c); /* 8003BBBC */
 ff_w32(0x800bbeec,0x80090948); /* 8003BBC8 */
 ff_w32(0x800bbef0,0x80090954); /* 8003BBD4 */
 ff_w32(0x800bbef4,0x80090960); /* 8003BBE0 */
 ff_w32(0x800bbef8,0x8009096c); /* 8003BBEC */
 ff_w32(0x800bbefc,0x80090978); /* 8003BBF8 */
 ff_w32(0x800bbf00,0x80090984); /* 8003BC04 */
 ff_w32(0x800bbf04,0x80090994); /* 8003BC10 */
 ff_w32(0x800bbf08,0x800909a0); /* 8003BC1C */
 ff_w32(0x800bbf0c,0x800909b0); /* 8003BC28 */
 ff_w32(0x800bbf10,0x800909bc); /* 8003BC34 */
 ff_w32(0x800bbf14,0x800909cc); /* 8003BC40 */
 ff_w32(0x800bbf18,0x800909d8); /* 8003BC4C */
 ff_w32(0x800bbf1c,0x800909e4); /* 8003BC58 */
 ff_w32(0x800bbf20,0x800909f4); /* 8003BC64 */
 ff_w32(0x800bbf24,0x80090a00); /* 8003BC70 */
 ff_w32(0x800bbf28,0x80090a10); /* 8003BC7C */
 ff_w32(0x800bbf2c,0x80090a1c); /* 8003BC88 */
 ff_w32(0x800bbf30,0x80090a28); /* 8003BC94 */
 ff_w32(0x800bbf34,0x80090a34); /* 8003BCA0 */
 ff_w32(0x800bbf38,0x80090a44); /* 8003BCAC */
 ff_w32(0x800bbf3c,0x80090a50); /* 8003BCB8 */
 ff_w32(0x800bbf40,0x80090a5c); /* 8003BCC4 */
 ff_w32(0x800bbf44,0x80090a68); /* 8003BCD0 */
 ff_w32(0x800bbf48,0x80090a74); /* 8003BCDC */
 ff_w32(0x800bbe54,0x80090a80); /* 8003BCE8 */
 ff_w32(0x800bbe58,0x80090a8c); /* 8003BCF4 */
 ff_w32(0x800bbe5c,0x80090a98); /* 8003BD00 */
 ff_w32(0x800bbe64,0x80090aa4); /* 8003BD0C */
 ff_w32(0x800bbe68,0x80090ab0); /* 8003BD18 */
 ff_w32(0x800bbe6c,0x80090abc); /* 8003BD24 */
 ff_w32(0x800bbe74,0x80090ac8); /* 8003BD30 */
 ff_w32(0x800bbe78,0x80090ad4); /* 8003BD3C */
 ff_w32(0x800bbe7c,0x80090ae0); /* 8003BD48 */
 ff_w32(0x800bbe80,0x80090aec); /* 8003BD54 */
 ff_w32(0x800bbe84,0x80090afc); /* 8003BD60 */
 ff_w32(0x800bbe88,0x80090b08); /* 8003BD6C */
 ff_w32(0x800bbe8c,0x80090b14); /* 8003BD78 */
 ff_w32(0x800bbe90,0x80090b20); /* 8003BD84 */
 ff_w32(0x800bbe94,0x80090b2c); /* 8003BD90 */
 ff_w32(0x800bbe98,0x80090b38); /* 8003BD9C */
 ff_w32(0x800bbea0,0x80090b44); /* 8003BDA8 */
 ff_w32(0x800bbea4,0x80090b50); /* 8003BDB4 */
 ff_w32(0x800bbea8,0x80090b5c); /* 8003BDC0 */
 ff_w32(0x800bbeac,0x80090b68); /* 8003BDCC */
 ff_w32(0x800bbeb0,0x80090b74); /* 8003BDD8 */
 ff_w32(0x800bbeb4,0x80090b80); /* 8003BDE4 */
 ff_w32(0x800bbebc,0x80090b8c); /* 8003BDF0 */
 ff_w32(0x800bbe60,0x80090b98); /* 8003BDFC */
 ff_w32(0x800bbf60,0x80090ba4); /* 8003BE08 */
 ff_w32(0x800bbf64,0x80090bb0); /* 8003BE14 */
 ff_w32(0x800bbf68,0x80090bbc); /* 8003BE20 */
 ff_w32(0x800bbf58,0x80090bc8); /* 8003BE2C */
 ff_w32(0x800bbe3c,0x80090bd8); /* 8003BE38 */
 ff_w32(0x800bbe40,0x80090be8); /* 8003BE44 */
 ff_w32(0x800bbe44,0x80090bf8); /* 8003BE50 */
 ff_w32(0x800bbe48,0x80090c08); /* 8003BE5C */
 ff_w32(0x800bbe4c,0x80090c18); /* 8003BE68 */
 ff_w32(0x800bbe50,0x80090c28); /* 8003BE74 */
 ff_w32(0x800ba040,0x80090c38); /* 8003BE88 */
 ff_w32(0x800ba15c,0x80090c48); /* 8003BE94 */
 ff_w32(0x800ba158,0x80090c54); /* 8003BEA0 */
 ff_w32(0x800ba164,0x80090c60); /* 8003BEAC */
 ff_w32(0x800ba0d0,0x80090c70); /* 8003BEB8 */
 ff_w32(0x800ba0d4,0x80090c7c); /* 8003BEC4 */
 ff_w32(0x800ba0d8,0x80090c88); /* 8003BED0 */
 ff_w32(0x800ba0dc,0x80090c94); /* 8003BEDC */
 ff_w32(0x800ba0e0,0x80090ca0); /* 8003BEE8 */
 ff_w32(0x800ba0e4,0x80090cac); /* 8003BEF4 */
 ff_w32(0x800ba0e8,0x80090cb8); /* 8003BF00 */
 ff_w32(0x800ba0ec,0x80090cc4); /* 8003BF0C */
 ff_w32(0x800ba0f0,0x80090cd0); /* 8003BF18 */
 ff_w32(0x800ba0f4,0x80090cdc); /* 8003BF24 */
 ff_w32(0x800ba0f8,0x80090ce8); /* 8003BF30 */
 ff_w32(0x800ba0fc,0x80090cf4); /* 8003BF3C */
 ff_w32(0x800ba100,0x80090d00); /* 8003BF48 */
 ff_w32(0x800ba104,0x80090d0c); /* 8003BF54 */
 ff_w32(0x800ba108,0x80090d18); /* 8003BF60 */
 ff_w32(0x800ba10c,0x80090d28); /* 8003BF6C */
 ff_w32(0x800ba110,0x80090d34); /* 8003BF78 */
 ff_w32(0x800ba114,0x80090d44); /* 8003BF84 */
 ff_w32(0x800ba118,0x80090d50); /* 8003BF90 */
 ff_w32(0x800ba11c,0x80090d60); /* 8003BF9C */
 ff_w32(0x800ba120,0x80090d6c); /* 8003BFA8 */
 ff_w32(0x800ba124,0x80090d78); /* 8003BFB4 */
 ff_w32(0x800ba128,0x80090d88); /* 8003BFC0 */
 ff_w32(0x800ba12c,0x80090d94); /* 8003BFCC */
 ff_w32(0x800ba130,0x80090da4); /* 8003BFD8 */
 ff_w32(0x800ba134,0x80090db0); /* 8003BFE4 */
 ff_w32(0x800ba138,0x80090dbc); /* 8003BFF0 */
 ff_w32(0x800ba13c,0x80090dc8); /* 8003BFFC */
 ff_w32(0x800ba140,0x80090dd8); /* 8003C008 */
 ff_w32(0x800ba148,0x80090de4); /* 8003C014 */
 ff_w32(0x800ba14c,0x80090df0); /* 8003C020 */
 ff_w32(0x800ba150,0x80090dfc); /* 8003C02C */
 ff_w32(0x800ba144,0x80090e08); /* 8003C038 */
 ff_w32(0x800ba05c,0x80090e14); /* 8003C044 */
 ff_w32(0x800ba060,0x80090e20); /* 8003C050 */
 ff_w32(0x800ba064,0x80090e2c); /* 8003C05C */
 ff_w32(0x800ba06c,0x80090e38); /* 8003C068 */
 ff_w32(0x800ba070,0x80090e44); /* 8003C074 */
 ff_w32(0x800ba074,0x80090e50); /* 8003C080 */
 ff_w32(0x800ba07c,0x80090e5c); /* 8003C08C */
 ff_w32(0x800ba080,0x80090e68); /* 8003C098 */
 ff_w32(0x800ba084,0x80090e74); /* 8003C0A4 */
 ff_w32(0x800ba088,0x80090e80); /* 8003C0B0 */
 ff_w32(0x800ba08c,0x80090e90); /* 8003C0BC */
 ff_w32(0x800ba090,0x80090e9c); /* 8003C0C8 */
 ff_w32(0x800ba094,0x80090ea8); /* 8003C0D4 */
 ff_w32(0x800ba098,0x80090eb4); /* 8003C0E0 */
 ff_w32(0x800ba09c,0x80090ec0); /* 8003C0EC */
 ff_w32(0x800ba0a0,0x80090ecc); /* 8003C0F8 */
 ff_w32(0x800ba0a8,0x80090ed8); /* 8003C104 */
 ff_w32(0x800ba0ac,0x80090ee4); /* 8003C110 */
 ff_w32(0x800ba0b0,0x80090ef0); /* 8003C11C */
 ff_w32(0x800ba0b4,0x80090efc); /* 8003C128 */
 ff_w32(0x800ba0b8,0x80090f08); /* 8003C134 */
 ff_w32(0x800ba0bc,0x80090f14); /* 8003C140 */
 ff_w32(0x800ba0c4,0x80090f20); /* 8003C14C */
 ff_w32(0x800ba068,0x80090f2c); /* 8003C158 */
 ff_w32(0x800ba168,0x80090f38); /* 8003C164 */
 ff_w32(0x800ba16c,0x80090f44); /* 8003C170 */
 ff_w32(0x800ba170,0x80090f50); /* 8003C17C */
 ff_w32(0x800ba160,0x80090f5c); /* 8003C188 */
 ff_w32(0x800ba044,0x80090f6c); /* 8003C194 */
 ff_w32(0x800ba048,0x80090f7c); /* 8003C1A0 */
 ff_w32(0x800ba04c,0x80090f8c); /* 8003C1AC */
 ff_w32(0x800ba050,0x80090f9c); /* 8003C1B8 */
 ff_w32(0x800ba054,0x80090fac); /* 8003C1C4 */
 ff_w32(0x800ba058,0x80090fbc); /* 8003C1D0 */
 ff_w32(0x800b9bf0,0x80090fcc); /* 8003C1E4 */
 ff_w32(0x800b9e98,0x80090fe4); /* 8003C1F0 */
 ff_w32(0x800b9bf4,0x80090ff8); /* 8003C1FC */
 ff_w32(0x800b9bf8,0x8009100c); /* 8003C208 */
 ff_w32(0x800b9bfc,0x80091024); /* 8003C214 */
 ff_w32(0x800b9c00,0x8009103c); /* 8003C220 */
 ff_w32(0x800b9f30,0x80091054); /* 8003C22C */
 ff_w32(0x800b9f34,0x8009106c); /* 8003C238 */
 ff_w32(0x800b9f38,0x80091080); /* 8003C244 */
 ff_w32(0x800b9c04,0x80091094); /* 8003C250 */
 ff_w32(0x800b9c08,0x800910ac); /* 8003C25C */
 ff_w32(0x800b9c0c,0x800910c4); /* 8003C268 */
 ff_w32(0x800b9c10,0x800910dc); /* 8003C274 */
 ff_w32(0x800b9c14,0x800910f4); /* 8003C280 */
 ff_w32(0x800b9c18,0x8009110c); /* 8003C28C */
 ff_w32(0x800b9c20,0x80091124); /* 8003C298 */
 ff_w32(0x800b9c24,0x8009113c); /* 8003C2A4 */
 ff_w32(0x800b9c28,0x80091158); /* 8003C2B0 */
 ff_w32(0x800b9c2c,0x80091174); /* 8003C2BC */
 ff_w32(0x800b9c30,0x8009118c); /* 8003C2C8 */
 ff_w32(0x800b9c34,0x800911a8); /* 8003C2D4 */
 ff_w32(0x800b9c38,0x800911c0); /* 8003C2E0 */
 ff_w32(0x800b9c3c,0x800911d8); /* 8003C2EC */
 ff_w32(0x800b9c40,0x800911f0); /* 8003C2F8 */
 ff_w32(0x800b9c44,0x80091208); /* 8003C304 */
 ff_w32(0x800b9c48,0x80091220); /* 8003C310 */
 ff_w32(0x800b9c4c,0x80091238); /* 8003C31C */
 ff_w32(0x800b9c50,0x80091250); /* 8003C328 */
 ff_w32(0x800b9c54,0x80091268); /* 8003C334 */
 ff_w32(0x800b9c58,0x80091280); /* 8003C340 */
 ff_w32(0x800b9c5c,0x80091298); /* 8003C34C */
 ff_w32(0x800b9c60,0x800912b0); /* 8003C358 */
 ff_w32(0x800b9c64,0x800912c8); /* 8003C364 */
 ff_w32(0x800b9c70,0x800912e0); /* 8003C370 */
 ff_w32(0x800b9c74,0x800912f8); /* 8003C37C */
 ff_w32(0x800b9c78,0x80091310); /* 8003C388 */
 ff_w32(0x800b9c7c,0x80091328); /* 8003C394 */
 ff_w32(0x800b9c80,0x80091340); /* 8003C3A0 */
 ff_w32(0x800b9c84,0x80091358); /* 8003C3AC */
 ff_w32(0x800b9c88,0x80091370); /* 8003C3B8 */
 ff_w32(0x800b9c8c,0x80091388); /* 8003C3C4 */
 ff_w32(0x800b9c90,0x800913a4); /* 8003C3D0 */
 ff_w32(0x800b9c94,0x800913c0); /* 8003C3DC */
 ff_w32(0x800b9c98,0x800913dc); /* 8003C3E8 */
 ff_w32(0x800b9c9c,0x800913f8); /* 8003C3F4 */
 ff_w32(0x800b9ca0,0x80091414); /* 8003C400 */
 ff_w32(0x800b9ca4,0x80091430); /* 8003C40C */
 ff_w32(0x800b9ca8,0x8009144c); /* 8003C418 */
 ff_w32(0x800b9cb0,0x80091468); /* 8003C424 */
 ff_w32(0x800b9cb4,0x80091484); /* 8003C430 */
 ff_w32(0x800b9cb8,0x800914a0); /* 8003C43C */
 ff_w32(0x800b9cbc,0x800914bc); /* 8003C448 */
 ff_w32(0x800b9cc0,0x800914d8); /* 8003C454 */
 ff_w32(0x800b9cc8,0x800914f4); /* 8003C460 */
 ff_w32(0x800b9ccc,0x80091510); /* 8003C46C */
 ff_w32(0x800b9cd0,0x8009152c); /* 8003C478 */
 ff_w32(0x800b9cd4,0x80091548); /* 8003C484 */
 ff_w32(0x800b9cd8,0x80091564); /* 8003C490 */
 ff_w32(0x800b9cdc,0x80091580); /* 8003C49C */
 ff_w32(0x800b9ce0,0x8009159c); /* 8003C4A8 */
 ff_w32(0x800b9ce4,0x800915b8); /* 8003C4B4 */
 ff_w32(0x800b9ce8,0x800915d4); /* 8003C4C0 */
 ff_w32(0x800b9cec,0x800915f0); /* 8003C4CC */
 ff_w32(0x800b9cf0,0x8009160c); /* 8003C4D8 */
 ff_w32(0x800b9cfc,0x80091628); /* 8003C4E4 */
 ff_w32(0x800b9d00,0x80091644); /* 8003C4F0 */
 ff_w32(0x800b9d04,0x80091660); /* 8003C4FC */
 ff_w32(0x800b9d08,0x8009167c); /* 8003C508 */
 ff_w32(0x800b9d0c,0x80091698); /* 8003C514 */
 ff_w32(0x800b9d10,0x800916b4); /* 8003C520 */
 ff_w32(0x800b9d14,0x800916d0); /* 8003C52C */
 ff_w32(0x800b9d18,0x800916ec); /* 8003C538 */
 ff_w32(0x800b9d1c,0x80091708); /* 8003C544 */
 ff_w32(0x800b9d20,0x80091724); /* 8003C550 */
 ff_w32(0x800b9d24,0x80091738); /* 8003C55C */
 ff_w32(0x800b9d28,0x8009174c); /* 8003C568 */
 ff_w32(0x800b9d2c,0x80091760); /* 8003C574 */
 ff_w32(0x800b9d30,0x80091774); /* 8003C580 */
 ff_w32(0x800b9d34,0x80091788); /* 8003C58C */
 ff_w32(0x800b9e9c,0x8009179c); /* 8003C598 */
 ff_w32(0x800b9ea0,0x800917b4); /* 8003C5A4 */
 ff_w32(0x800b9ea4,0x800917cc); /* 8003C5B0 */
 ff_w32(0x800b9ea8,0x800917e4); /* 8003C5BC */
 ff_w32(0x800b9eac,0x800917e4); /* 8003C5C0 */
 ff_w32(0x800b9eb0,0x800917fc); /* 8003C5CC */
 ff_w32(0x800b9eb8,0x80091814); /* 8003C5D8 */
 ff_w32(0x800b9ebc,0x8009182c); /* 8003C5E4 */
 ff_w32(0x800b9ec0,0x8009182c); /* 8003C5E8 */
 ff_w32(0x800b9ec4,0x80091844); /* 8003C5F4 */
 ff_w32(0x800b9ec8,0x8009185c); /* 8003C600 */
 ff_w32(0x800b9ecc,0x80091874); /* 8003C60C */
 ff_w32(0x800b9ed0,0x8009188c); /* 8003C618 */
 ff_w32(0x800b9ed4,0x800918a4); /* 8003C624 */
 ff_w32(0x800b9ed8,0x800918bc); /* 8003C630 */
 ff_w32(0x800b9edc,0x800918d4); /* 8003C63C */
 ff_w32(0x800b9ee0,0x800918ec); /* 8003C648 */
 ff_w32(0x800b9ee4,0x80091904); /* 8003C654 */
 ff_w32(0x800b9ee8,0x8009191c); /* 8003C660 */
 ff_w32(0x800b9eec,0x80091934); /* 8003C66C */
 ff_w32(0x800b9ef0,0x8009194c); /* 8003C678 */
 ff_w32(0x800b9ef4,0x80091964); /* 8003C684 */
 ff_w32(0x800b9ef8,0x8009197c); /* 8003C690 */
 ff_w32(0x800b9efc,0x80091994); /* 8003C69C */
 ff_w32(0x800b9f08,0x8009179c); /* 8003C6A0 */
 ff_w32(0x800b9f0c,0x8009179c); /* 8003C6A4 */
 ff_w32(0x800b9f10,0x8009179c); /* 8003C6A8 */
 ff_w32(0x800b9f14,0x8009179c); /* 8003C6AC */
 ff_w32(0x800b9f18,0x8009179c); /* 8003C6B0 */
 ff_w32(0x800b9f1c,0x8009179c); /* 8003C6B4 */
 ff_w32(0x800b9f20,0x8009179c); /* 8003C6B8 */
 ff_w32(0x800b9f24,0x8009179c); /* 8003C6BC */
 ff_w32(0x800b9f28,0x8009179c); /* 8003C6C0 */
 ff_w32(0x800b9f2c,0x800919ac); /* 8003C6CC */
 ff_w32(0x800b6e90,0x800919c4); /* 8003C6E0 */
 ff_w32(0x800b6e94,0x800919cc); /* 8003C6EC */
 ff_w32(0x800b6e98,0x800919d4); /* 8003C6F8 */
 ff_w32(0x800b6e9c,0x800919dc); /* 8003C704 */
 ff_w32(0x800b6ea0,0x800919e4); /* 8003C710 */
 ff_w32(0x800b6ea4,0x1); /* 8003C718 */
 ff_w32(0x800b6ea8,0x1); /* 8003C71C */
 ff_w32(0x800b6eac,0x1); /* 8003C720 */
 ff_w32(0x800b6eb0,0x1); /* 8003C724 */
 ff_w32(0x800b6eb4,0x1); /* 8003C728 */
 ff_w32(0x800b6eb8,0x1); /* 8003C72C */
 ff_w32(0x800b6ebc,0x1); /* 8003C730 */
 ff_w32(0x800b6ec0,0x2); /* 8003C738 */
 ff_w32(0x800b6ec4,0x1); /* 8003C73C */
 ff_w32(0x800b6ec8,0x800919ec); /* 8003C748 */
 ff_w32(0x800b6ecc,0x1); /* 8003C74C */
 ff_w32(0x800b6ed0,0x1); /* 8003C750 */
 ff_w32(0x800b6ed4,0x1); /* 8003C754 */
 ff_w32(0x800b6ed8,0x1); /* 8003C758 */
 ff_w32(0x800b6edc,0x1); /* 8003C75C */
 ff_w32(0x800b6ee0,0x1); /* 8003C760 */
 ff_w32(0x800b6ee4,0x1); /* 8003C764 */
 ff_w32(0x800b6ee8,0x1); /* 8003C768 */
 ff_w32(0x800b6eec,0x1); /* 8003C76C */
 ff_w32(0x800b6ef0,0x1); /* 8003C770 */
 ff_w32(0x800b6ef4,0x1); /* 8003C774 */
 ff_w32(0x800b6ef8,0x1); /* 8003C778 */
 ff_w32(0x800b6efc,0x1); /* 8003C77C */
 ff_w32(0x800b6f00,0x1); /* 8003C780 */
 ff_w32(0x800b6f04,0x2); /* 8003C784 */
 ff_w32(0x800b6f08,0x2); /* 8003C788 */
 ff_w32(0x800b6f0c,0x1); /* 8003C78C */
 ff_w32(0x800b6f10,0x800919f4); /* 8003C798 */
 ff_w32(0x800b6f14,0x1); /* 8003C79C */
 ff_w32(0x800b6f18,0x1); /* 8003C7A0 */
 ff_w32(0x800b6f1c,0x1); /* 8003C7A4 */
 ff_w32(0x800b6f20,0x1); /* 8003C7A8 */
 ff_w32(0x800b6f24,0x1); /* 8003C7AC */
 ff_w32(0x800b6f28,0x2); /* 8003C7B0 */
 ff_w32(0x800b6f2c,0x1); /* 8003C7B4 */
 ff_w32(0x800b6f30,0x1); /* 8003C7B8 */
 ff_w32(0x800b6f34,0x1); /* 8003C7BC */
 ff_w32(0x800b6f38,0x1); /* 8003C7C0 */
 ff_w32(0x800b6f3c,0x1); /* 8003C7C4 */
 ff_w32(0x800b6f40,0x1); /* 8003C7C8 */
 ff_w32(0x800b6f44,0x1); /* 8003C7CC */
 ff_w32(0x800b6f48,0x1); /* 8003C7D0 */
 ff_w32(0x800b6f4c,0x1); /* 8003C7D4 */
 ff_w32(0x800b6f50,0x800919fc); /* 8003C7E0 */
 ff_w32(0x800b6f54,0x80091a04); /* 8003C7EC */
 ff_w32(0x800b6f58,0x80091a08); /* 8003C7F8 */
 ff_w32(0x800b6f60,0x1); /* 8003C7FC */
 ff_w32(0x800b6f64,0x80091a10); /* 8003C808 */
 ff_w32(0x800b3be8,0x800ae580); /* 8003C81C */
 ff_w32(0x800b3bec,0x800ae0f8); /* 8003C828 */
 ff_w32(0x800b3bf0,0x800aea08); /* 8003C834 */
 ff_w32(0x800b3bf4,0x800adc70); /* 8003C840 */
 ff_w32(0x800b3bf8,0x800af518); /* 8003C84C */
 ff_w32(0x800b3bfc,0x8008409c); /* 8003C858 */
 ff_w32(0x800b3c00,0x8008409c); /* 8003C85C */
 ff_w32(0x800b3c04,0x8008409c); /* 8003C860 */
 ff_w32(0x800b3c08,0x8008409c); /* 8003C864 */
 ff_w32(0x800b3c0c,0x8008409c); /* 8003C868 */
 ff_w32(0x800b3c10,0x8008409c); /* 8003C86C */
 ff_w32(0x800b3c14,0x8008409c); /* 8003C870 */
 ff_w32(0x800b3c18,0x80084520); /* 8003C87C */
 ff_w32(0x800b3c1c,0x800849a4); /* 8003C888 */
 ff_w32(0x800b3c20,0x80082a08); /* 8003C894 */
 ff_w32(0x800b3c24,0x800849a4); /* 8003C898 */
 ff_w32(0x800b3c30,0x8008409c); /* 8003C89C */
 ff_w32(0x800b3c34,0x8008409c); /* 8003C8A0 */
 ff_w32(0x800b3c38,0x8008409c); /* 8003C8A4 */
 ff_w32(0x800b3c3c,0x8008409c); /* 8003C8A8 */
 ff_w32(0x800b3c40,0x8008409c); /* 8003C8AC */
 ff_w32(0x800b3c44,0x8008409c); /* 8003C8B0 */
 ff_w32(0x800b3c48,0x8008409c); /* 8003C8B4 */
 ff_w32(0x800b3c4c,0x8008409c); /* 8003C8B8 */
 ff_w32(0x800b3c50,0x8008409c); /* 8003C8BC */
 ff_w32(0x800b3c54,0x8008409c); /* 8003C8C0 */
 ff_w32(0x800b3c58,0x80082584); /* 8003C8CC */
 ff_w32(0x800b3c5c,0x80084520); /* 8003C8D0 */
 ff_w32(0x800b3c60,0x80084520); /* 8003C8D4 */
 ff_w32(0x800b3c64,0x800849a4); /* 8003C8D8 */
 ff_w32(0x800b3c68,0x80082e8c); /* 8003C8E4 */
 ff_w32(0x800b3c6c,0x8008409c); /* 8003C8E8 */
 ff_w32(0x800b3c70,0x8008409c); /* 8003C8EC */
 ff_w32(0x800b3c74,0x8008409c); /* 8003C8F0 */
 ff_w32(0x800b3c78,0x8008409c); /* 8003C8F4 */
 ff_w32(0x800b3c7c,0x8008409c); /* 8003C8F8 */
 ff_w32(0x800b3c80,0x80084520); /* 8003C8FC */
 ff_w32(0x800b3c84,0x8008409c); /* 8003C900 */
 ff_w32(0x800b3c88,0x8008409c); /* 8003C904 */
 ff_w32(0x800b3c8c,0x8008409c); /* 8003C908 */
 ff_w32(0x800b3c90,0x8008409c); /* 8003C90C */
 ff_w32(0x800b3c94,0x8008409c); /* 8003C910 */
 ff_w32(0x800b3c98,0x8008409c); /* 8003C914 */
 ff_w32(0x800b3c9c,0x8008409c); /* 8003C918 */
 ff_w32(0x800b3ca0,0x8008409c); /* 8003C91C */
 ff_w32(0x800b3ca4,0x8008409c); /* 8003C920 */
 ff_w32(0x800b3ca8,0x80083310); /* 8003C92C */
 ff_w32(0x800b3cac,0x80083794); /* 8003C938 */
 ff_w32(0x800b3cb0,0x80083c18); /* 8003C944 */
 ff_w32(0x800b3cb4,0x8008409c); /* 8003C948 */
 ff_w32(0x800b3cb8,0x8008409c); /* 8003C94C */
 ff_w32(0x800b3cbc,0x8008409c); /* 8003C950 */
 for(i=0;i<54;i++)ff_w16(0x80094120+2*i,0x78); /* 8003C96C */
 ff_w16(0x80094142,0x1a0); /* 8003C98C */
 ff_w16(0x80094140,0x1a0); /* 8003C990 */
 ff_w32(0x800b7074,0x80052944); /* 8003C9A4 */
 ff_w32(0x800b7080,0x80052944); /* 8003C9A8 */
 ff_w32(0x800b7078,0x80052944); /* 8003C9AC */
 ff_w32(0x800b7084,0x80052944); /* 8003C9B0 */
 ff_w32(0x800b707c,0x80052944); /* 8003C9B4 */
 ff_w32(0x800b7088,0x80052944); /* 8003C9B8 */
 ff_w32(0x800b7094,0x80052944); /* 8003C9BC */
 ff_w32(0x800b708c,0x80052944); /* 8003C9C0 */
 ff_w32(0x800b70a8,0x80052944); /* 8003C9C4 */
 ff_w32(0x800b70ac,0x80052944); /* 8003C9C8 */
 ff_w32(0x800b70b0,0x80052944); /* 8003C9CC */
 ff_w32(0x800b70b4,0x80052944); /* 8003C9D0 */
 ff_w32(0x800b70b8,0x80052944); /* 8003C9D4 */
 ff_w32(0x800b70bc,0x80052944); /* 8003C9D8 */
 ff_w32(0x800b70c0,0x80052944); /* 8003C9DC */
 ff_w32(0x800b70c4,0x80052944); /* 8003C9E0 */
 ff_w32(0x800b70c8,0x80052944); /* 8003C9E4 */
 ff_w32(0x800b70cc,0x80052944); /* 8003C9E8 */
 ff_w32(0x800b70d0,0x8004be84); /* 8003C9F4 */
 ff_w32(0x800b70d4,0x80052944); /* 8003C9F8 */
 ff_w32(0x800b70d8,0x80052944); /* 8003C9FC */
 ff_w32(0x800b70dc,0x80052944); /* 8003CA00 */
 ff_w32(0x800b70e0,0x80052944); /* 8003CA04 */
 ff_w32(0x800b70e4,0x80052944); /* 8003CA08 */
 ff_w32(0x800b70e8,0x80052944); /* 8003CA0C */
 ff_w32(0x800b70ec,0x80052944); /* 8003CA10 */
 ff_w32(0x800b70f0,0x80052944); /* 8003CA14 */
 ff_w32(0x800b70f4,0x80052944); /* 8003CA18 */
 ff_w32(0x800b70f8,0x80052944); /* 8003CA1C */
 ff_w32(0x800b70fc,0x80052944); /* 8003CA20 */
 ff_w32(0x800b7100,0x80052944); /* 8003CA24 */
 ff_w32(0x800b7104,0x80052944); /* 8003CA28 */
 ff_w32(0x800b7108,0x80052944); /* 8003CA2C */
 ff_w32(0x800b710c,0x80052944); /* 8003CA30 */
 ff_w32(0x800b711c,0x80052944); /* 8003CA34 */
 ff_w32(0x800b7110,0x80056530); /* 8003CA40 */
 ff_w32(0x800b7114,0x80056530); /* 8003CA44 */
 ff_w32(0x800b7118,0x80056530); /* 8003CA48 */
 ff_w32(0x800b7120,0x8004ddec); /* 8003CA54 */
 ff_w32(0x800b7124,0x80052944); /* 8003CA58 */
 ff_w32(0x800b7128,0x8004df2c); /* 8003CA64 */
 ff_w32(0x800b712c,0x80055c30); /* 8003CA70 */
 ff_w32(0x800b70a4,0x80049e6c); /* 8003CA7C */
 ff_w32(0x800b7098,0x8003b8e4); /* 8003CA88 */
 ff_w32(0x800b7090,0x8003b83c); /* 8003CA94 */
 ff_w32(0x800b7130,0x80052944); /* 8003CA98 */
 ff_w32(0x800b7134,0x80052944); /* 8003CA9C */
 for(i=0;i<921;i++)ff_w32(0x800bbf78+4*i,0x0); /* 8003CAB4 */
 ff_w32(0x800bccf0,0x8002ac8c); /* 8003CAD8 */
 ff_w32(0x800bccf4,0x8002ac8c); /* 8003CADC */
 ff_w32(0x800bccf8,0x8002ac8c); /* 8003CAE0 */
 ff_w32(0x800bc874,0x8002ace8); /* 8003CAEC */
 ff_w32(0x800bc878,0x8002ad7c); /* 8003CAF8 */
 ff_w32(0x800bcd0c,0x8002b1e4); /* 8003CB04 */
 ff_w32(0x800bcd3c,0x8002ac8c); /* 8003CB08 */
 ff_w32(0x800bcd40,0x8002ac8c); /* 8003CB0C */
 ff_w32(0x800bcd44,0x8002ac8c); /* 8003CB10 */
 ff_w32(0x800bcd48,0x8002ac8c); /* 8003CB14 */
 ff_w32(0x800bcd4c,0x8002ac8c); /* 8003CB18 */
 ff_w32(0x800bccfc,0x8002b28c); /* 8003CB24 */
 ff_w32(0x800bcd00,0x8002b28c); /* 8003CB28 */
 ff_w32(0x800bcd50,0x8002b2b4); /* 8003CB34 */
 ff_w32(0x800bc868,0x8002fa90); /* 8003CB40 */
 ff_w32(0x800bc86c,0x80041b54); /* 8003CB4C */
 ff_w32(0x800bc870,0x80041b54); /* 8003CB50 */
 ff_w32(0x800bc7b0,0x8002ba68); /* 8003CB5C */
 ff_w32(0x800bc7b4,0x8002ba68); /* 8003CB60 */
 ff_w32(0x800bc738,0x8002bc1c); /* 8003CB6C */
 ff_w32(0x800bc73c,0x8002bc1c); /* 8003CB70 */
 ff_w32(0x800bc778,0x8002e11c); /* 8003CB7C */
 ff_w32(0x800bc77c,0x8002e11c); /* 8003CB80 */
 ff_w32(0x800bc780,0x8002e11c); /* 8003CB84 */
 ff_w32(0x800bc8f8,0x8002d5d4); /* 8003CB90 */
 ff_w32(0x800bc8fc,0x8002d5d4); /* 8003CB94 */
 ff_w32(0x800bc900,0x8002d5d4); /* 8003CB98 */
 ff_w32(0x800bc904,0x8002d5d4); /* 8003CB9C */
 ff_w32(0x800bc908,0x8002d5d4); /* 8003CBA0 */
 ff_w32(0x800bc150,0x8002a974); /* 8003CBAC */
 ff_w32(0x800bc0b0,0x8002f59c); /* 8003CBB8 */
 ff_w32(0x800bc0b4,0x8002f59c); /* 8003CBBC */
 ff_w32(0x800bc0b8,0x8002f59c); /* 8003CBC0 */
 ff_w32(0x800bc0bc,0x8002f59c); /* 8003CBC4 */
 ff_w32(0x800bc0c0,0x8002f59c); /* 8003CBC8 */
 ff_w32(0x800bc0c4,0x8002f59c); /* 8003CBCC */
 ff_w32(0x800bc0c8,0x8002f59c); /* 8003CBD0 */
 ff_w32(0x800bc0e0,0x8002da3c); /* 8003CBDC */
 ff_w32(0x800bc070,0x8002da3c); /* 8003CBE0 */
 ff_w32(0x800bc074,0x8002dba0); /* 8003CBEC */
 ff_w32(0x800bc078,0x8002dba0); /* 8003CBF0 */
 ff_w32(0x800bc08c,0x8002ddb8); /* 8003CBFC */
 ff_w32(0x800bbfbc,0x8002da3c); /* 8003CC00 */
 ff_w32(0x800bc080,0x8002da3c); /* 8003CC04 */
 ff_w32(0x800bc084,0x8002e038); /* 8003CC10 */
 ff_w32(0x800bc0e8,0x8002e038); /* 8003CC14 */
 ff_w32(0x800bc0cc,0x8002e038); /* 8003CC18 */
 ff_w32(0x800bc0d0,0x8002e038); /* 8003CC1C */
 ff_w32(0x800bc0d4,0x8002e038); /* 8003CC20 */
 ff_w32(0x800bc0d8,0x8002e038); /* 8003CC24 */
 ff_w32(0x800bc0f0,0x8002da3c); /* 8003CC28 */
 ff_w32(0x800bc0f4,0x8002da3c); /* 8003CC2C */
 ff_w32(0x800bc0f8,0x8002dba0); /* 8003CC30 */
 ff_w32(0x800bc11c,0x8002ddb8); /* 8003CC34 */
 ff_w32(0x800bc104,0x8002df00); /* 8003CC40 */
 ff_w32(0x800bc108,0x8002dfa4); /* 8003CC4C */
 ff_w32(0x800bc128,0x8002da3c); /* 8003CC50 */
 ff_w32(0x800bc12c,0x8002da3c); /* 8003CC54 */
 ff_w32(0x800bc130,0x8002dba0); /* 8003CC58 */
 ff_w32(0x800bcab8,0x8002da3c); /* 8003CC5C */
 ff_w32(0x800bcabc,0x8002da3c); /* 8003CC60 */
 ff_w32(0x800bcac0,0x8002dba0); /* 8003CC64 */
 ff_w32(0x800bcac8,0x8002ddb8); /* 8003CC68 */
 ff_w32(0x800bc090,0x8002cde0); /* 8003CC74 */
 ff_w32(0x800bc0a0,0x8002cde0); /* 8003CC78 */
 ff_w32(0x800bc0a4,0x8002cde0); /* 8003CC7C */
 ff_w32(0x800bc0a8,0x8002cde0); /* 8003CC80 */
 ff_w32(0x800bc0ac,0x8002d7c8); /* 8003CC8C */
 ff_w32(0x800bc098,0x8002d8fc); /* 8003CC98 */
 ff_w32(0x800bc09c,0x8002d8fc); /* 8003CC9C */
 ff_w32(0x800bc980,0x8002d5d4); /* 8003CCA0 */
 ff_w32(0x800bc984,0x8002d5d4); /* 8003CCA4 */
 ff_w32(0x800bc988,0x8002d5d4); /* 8003CCA8 */
 ff_w32(0x800bc98c,0x8002d5d4); /* 8003CCAC */
 ff_w32(0x800bc990,0x8002d678); /* 8003CCB8 */
 ff_w32(0x800bc99c,0x8002d5d4); /* 8003CCBC */
 ff_w32(0x800bc9b4,0x8002d5d4); /* 8003CCC0 */
 ff_w32(0x800bc9b8,0x8002d678); /* 8003CCC4 */
 ff_w32(0x800bc168,0x8002ce6c); /* 8003CCD0 */
 ff_w32(0x800bc16c,0x8002cef8); /* 8003CCDC */
 ff_w32(0x800bc170,0x8002cef8); /* 8003CCE0 */
 ff_w32(0x800bc174,0x8002d218); /* 8003CCEC */
 ff_w32(0x800bc17c,0x8002d2a4); /* 8003CCF8 */
 ff_w32(0x800bc180,0x8002d2a4); /* 8003CCFC */
 ff_w32(0x800bc184,0x8002d2a4); /* 8003CD00 */
 ff_w32(0x800bc19c,0x800300bc); /* 8003CD0C */
 ff_w32(0x800bc9f8,0x8002d5d4); /* 8003CD10 */
 ff_w32(0x800bc9fc,0x8002d5d4); /* 8003CD14 */
 ff_w32(0x800bca00,0x8002d678); /* 8003CD18 */
 ff_w32(0x800bca14,0x8002c2e4); /* 8003CD24 */
 ff_w32(0x800bca18,0x8002c2e4); /* 8003CD28 */
 ff_w32(0x800bca1c,0x8002c2e4); /* 8003CD2C */
 ff_w32(0x800bca20,0x8002c2e4); /* 8003CD30 */
 ff_w32(0x800bca04,0x8002c454); /* 8003CD3C */
 ff_w32(0x800bca08,0x8002c454); /* 8003CD40 */
 ff_w32(0x800bca0c,0x8002c454); /* 8003CD44 */
 ff_w32(0x800bc1c0,0x8002c69c); /* 8003CD50 */
 ff_w32(0x800bcaac,0x8002e37c); /* 8003CD5C */
 ff_w32(0x800bca54,0x800469a4); /* 8003CD68 */
 ff_w32(0x800bca88,0x800469a4); /* 8003CD6C */
 ff_w32(0x800bca8c,0x800469a4); /* 8003CD70 */
 ff_w32(0x800bca90,0x800469a4); /* 8003CD74 */
 ff_w32(0x800bca58,0x80046e1c); /* 8003CD80 */
 ff_w32(0x800bc6dc,0x80046c08); /* 8003CD8C */
 ff_w32(0x800bc6e0,0x80046c08); /* 8003CD90 */
 ff_w32(0x800bc6e4,0x80046c08); /* 8003CD94 */
 ff_w32(0x800bcab0,0x8002d5d4); /* 8003CD98 */
 ff_w32(0x800bcab4,0x8002d678); /* 8003CD9C */
 ff_w32(0x800bcafc,0x8002d5d4); /* 8003CDA0 */
 ff_w32(0x800bc3ac,0x8002e444); /* 8003CDAC */
 ff_w32(0x800bc3b0,0x8002e444); /* 8003CDB0 */
 ff_w32(0x800bcb28,0x8002d5d4); /* 8003CDB4 */
 ff_w32(0x800bc424,0x8002d5d4); /* 8003CDB8 */
 ff_w32(0x800bc428,0x8002d5d4); /* 8003CDBC */
 ff_w32(0x800bc42c,0x8002d678); /* 8003CDC0 */
 ff_w32(0x800bc414,0x8002d5d4); /* 8003CDC4 */
 ff_w32(0x800bc418,0x8002d678); /* 8003CDC8 */
 ff_w32(0x800bc41c,0x8002d5d4); /* 8003CDCC */
 ff_w32(0x800bc420,0x8002d678); /* 8003CDD0 */
 ff_w32(0x800bc490,0x8002d5d4); /* 8003CDD4 */
 ff_w32(0x800bc464,0x8002e7c4); /* 8003CDE0 */
 ff_w32(0x800bc468,0x8002e7c4); /* 8003CDE4 */
 ff_w32(0x800bc4ec,0x8002d218); /* 8003CDE8 */
 ff_w32(0x800bc4f0,0x8002d2a4); /* 8003CDEC */
 ff_w32(0x800bc4f4,0x8002d2a4); /* 8003CDF0 */
 ff_w32(0x800bc4f8,0x8002d2a4); /* 8003CDF4 */
 ff_w32(0x800bc4fc,0x8002d2a4); /* 8003CDF8 */
 ff_w32(0x800bc500,0x8002d2a4); /* 8003CDFC */
 ff_w32(0x800bc504,0x8002d2a4); /* 8003CE00 */
 ff_w32(0x800bc508,0x8002d2a4); /* 8003CE04 */
 ff_w32(0x800bc50c,0x8002d2a4); /* 8003CE08 */
 ff_w32(0x800bc510,0x8002d2a4); /* 8003CE0C */
 ff_w32(0x800bc4cc,0x8002a168); /* 8003CE18 */
 ff_w32(0x800bc56c,0x8002d5d4); /* 8003CE1C */
 ff_w32(0x800bc564,0x8002d5d4); /* 8003CE20 */
 ff_w32(0x800bc55c,0x8002d5d4); /* 8003CE24 */
 ff_w32(0x800bc554,0x8002d5d4); /* 8003CE28 */
 ff_w32(0x800bc54c,0x8002d5d4); /* 8003CE2C */
 ff_w32(0x800bc544,0x8002d5d4); /* 8003CE30 */
 ff_w32(0x800bc534,0x8002d5d4); /* 8003CE34 */
 ff_w32(0x800bc570,0x8002d678); /* 8003CE38 */
 ff_w32(0x800bc568,0x8002d678); /* 8003CE3C */
 ff_w32(0x800bc560,0x8002d678); /* 8003CE40 */
 ff_w32(0x800bc558,0x8002d678); /* 8003CE44 */
 ff_w32(0x800bc550,0x8002d678); /* 8003CE48 */
 ff_w32(0x800bc548,0x8002d678); /* 8003CE4C */
 ff_w32(0x800bcbf0,0x8002e7f0); /* 8003CE58 */
 ff_w32(0x800bcbf8,0x8002e870); /* 8003CE64 */
 ff_w32(0x800bcc08,0x8002d5d4); /* 8003CE68 */
 ff_w32(0x800bcc10,0x8002d5d4); /* 8003CE6C */
 ff_w32(0x800bcc18,0x8002d5d4); /* 8003CE70 */
 ff_w32(0x800bcc20,0x8002d5d4); /* 8003CE74 */
 ff_w32(0x800bcc24,0x8002e8a4); /* 8003CE80 */
 ff_w32(0x800bcc1c,0x8002e8a4); /* 8003CE84 */
 ff_w32(0x800bcc14,0x8002e8a4); /* 8003CE88 */
 ff_w32(0x800bcc0c,0x8002e8a4); /* 8003CE8C */
 ff_w32(0x800bcbcc,0x8002d5d4); /* 8003CE90 */
 ff_w32(0x800bcbd0,0x8002d5d4); /* 8003CE94 */
 ff_w32(0x800bcbd4,0x8002d678); /* 8003CE98 */
 ff_w32(0x800bcbd8,0x8002d5d4); /* 8003CE9C */
 ff_w32(0x800bcbdc,0x8002d5d4); /* 8003CEA0 */
 ff_w32(0x800bcbe0,0x8002d678); /* 8003CEA4 */
 ff_w32(0x800bcbe4,0x8002d5d4); /* 8003CEA8 */
 ff_w32(0x800bcbe8,0x8002d5d4); /* 8003CEAC */
 ff_w32(0x800bcbec,0x8002d678); /* 8003CEB0 */
 ff_w32(0x800bcbc0,0x8002d5d4); /* 8003CEB4 */
 ff_w32(0x800bcbc4,0x8002d5d4); /* 8003CEB8 */
 ff_w32(0x800bcbc8,0x8002d678); /* 8003CEBC */
 ff_w32(0x800bcc5c,0x8002eb58); /* 8003CEC8 */
 ff_w32(0x800bc63c,0x8002eb6c); /* 8003CED4 */
 ff_w32(0x800bc5d8,0x800469a4); /* 8003CED8 */
 ff_w32(0x800bc5dc,0x8002d5d4); /* 8003CEDC */
 ff_w32(0x800bc5e0,0x8002e9fc); /* 8003CEE8 */
 ff_w32(0x800bc5e4,0x8002d5d4); /* 8003CEEC */
 ff_w32(0x800bc5e8,0x8002d678); /* 8003CEF0 */
 ff_w32(0x800bc5ec,0x8002d5d4); /* 8003CEF4 */
 ff_w32(0x800bc5f0,0x8002d678); /* 8003CEF8 */
 ff_w32(0x800bc5f4,0x8002d5d4); /* 8003CEFC */
 ff_w32(0x800bc5f8,0x8002d678); /* 8003CF00 */
 ff_w32(0x800bc5fc,0x8002d5d4); /* 8003CF04 */
 ff_w32(0x800bc600,0x8002d678); /* 8003CF08 */
 ff_w32(0x800bc604,0x8002d5d4); /* 8003CF0C */
 ff_w32(0x800bc608,0x8002d678); /* 8003CF10 */
 ff_w32(0x800bc60c,0x8002d5d4); /* 8003CF14 */
 ff_w32(0x800bc610,0x8002d678); /* 8003CF18 */
 ff_w32(0x800bc614,0x8002d5d4); /* 8003CF1C */
 ff_w32(0x800bc618,0x8002d678); /* 8003CF20 */
 ff_w32(0x800bc61c,0x8002d5d4); /* 8003CF24 */
 ff_w32(0x800bc620,0x8002d678); /* 8003CF28 */
 ff_w32(0x800bc624,0x8002d5d4); /* 8003CF2C */
 ff_w32(0x800bc628,0x8002d678); /* 8003CF30 */
 ff_w32(0x800bc62c,0x8002d5d4); /* 8003CF34 */
 ff_w32(0x800bc630,0x8002d678); /* 8003CF38 */
 ff_w32(0x800bc634,0x8002d5d4); /* 8003CF3C */
 ff_w32(0x800bc638,0x8002d678); /* 8003CF40 */
 ff_w32(0x800bc678,0x8002ee60); /* 8003CF4C */
 ff_w32(0x800bc6a8,0x8002efdc); /* 8003CF58 */
 ff_w32(0x800bc6ac,0x8002f2e0); /* 8003CF64 */
 ff_w32(0x800bc6b0,0x8002efdc); /* 8003CF68 */
 ff_w32(0x800bc6b4,0x8002f2e0); /* 8003CF6C */
 ff_w32(0x800bc1f0,0x80041b4c); /* 8003CF78 */
 ff_w32(0x800bffdc,0x8005dacc); /* 8003CF8C */
 ff_w32(0x800c0c88,0x8005c808); /* 8003CF98 */
 ff_w32(0x800c0c84,0x8005c808); /* 8003CF9C */
 ff_w32(0x800c0cc4,0x8005c808); /* 8003CFA0 */
 ff_w32(0x800c0c94,0x8005c808); /* 8003CFA4 */
 ff_w32(0x800c0800,0x8005c808); /* 8003CFA8 */
 ff_w32(0x800c07fc,0x8005c808); /* 8003CFAC */
 ff_w32(0x800c0c80,0x8005c808); /* 8003CFB0 */
 ff_w32(0x800c0c7c,0x8005c808); /* 8003CFB4 */
 ff_w32(0x800c0c78,0x8005c808); /* 8003CFB8 */
 ff_w32(0x800c07f8,0x8005cd1c); /* 8003CFC4 */
 ff_w32(0x800c0cc8,0x8005d150); /* 8003CFD0 */
 ff_w32(0x800c0ccc,0x8005d150); /* 8003CFD4 */
 ff_w32(0x800c0cd0,0x8005d150); /* 8003CFD8 */
 ff_w32(0x800c0cd4,0x8005d150); /* 8003CFDC */
 ff_w32(0x800c0cd8,0x8005d158); /* 8003CFE8 */
 ff_w32(0x800c0738,0x8005daac); /* 8003CFF4 */
 ff_w32(0x800c073c,0x8005daac); /* 8003CFF8 */
 ff_w32(0x800c00a4,0x8005d67c); /* 8003D004 */
 ff_w32(0x800c0014,0x8005d67c); /* 8003D008 */
 ff_w32(0x800c0a50,0x8005d67c); /* 8003D00C */
 ff_w32(0x800c098c,0x8002c55c); /* 8003D018 */
 ff_w32(0x800c0990,0x8002c55c); /* 8003D01C */
 ff_w32(0x800c0994,0x8002c55c); /* 8003D020 */
 ff_w32(0x800c06c4,0x8005dad4); /* 8003D02C */
 for(i=0;i<11;i++)ff_w32(0x800c0828+4*i,0x8005daac); /* 8003D04C */
 ff_w32(0x800c0630,0x8002aa6c); /* 8003D070 */
 ff_w32(0x800c0634,0x8002aa6c); /* 8003D074 */
 ff_w32(0x800c0638,0x8002aa6c); /* 8003D078 */
 ff_w32(0x800c063c,0x8002aa6c); /* 8003D07C */
 ff_w32(0x800be5e0,0x8002bec0); /* 8003D090 */
 ff_w32(0x800be5e4,0x8002bec0); /* 8003D094 */
 ff_w32(0x800be5f8,0x8002bec0); /* 8003D098 */
 ff_w32(0x800be5fc,0x8002bec0); /* 8003D09C */
 ff_w32(0x800be5f0,0x8002c060); /* 8003D0A8 */
 ff_w32(0x800be5e8,0x8002c154); /* 8003D0B4 */
 ff_w32(0x800be5ec,0x8002c1f4); /* 8003D0C0 */
 ff_w32(0x800bef3c,0x8002cba0); /* 8003D0CC */
 ff_w32(0x800bef40,0x8002cba0); /* 8003D0D0 */
 ff_w32(0x800bef44,0x8002cba0); /* 8003D0D4 */
 ff_w32(0x800bf3e4,0x8002bfa8); /* 8003D0E0 */
 ff_w32(0x800be5f4,0x8002da0c); /* 8003D0EC */
 ff_w32(0x800beefc,0x8002cba0); /* 8003D0F0 */
 ff_w32(0x800beedc,0x8002ace8); /* 8003D0FC */
 ff_w32(0x800beee0,0x8002ad7c); /* 8003D108 */
 ff_w32(0x800bf378,0x8002ae10); /* 8003D114 */
 ff_w32(0x800bf37c,0x8002ae64); /* 8003D120 */
 ff_w32(0x800bf380,0x8002ae64); /* 8003D124 */
 ff_w32(0x800bf384,0x80041894); /* 8003D130 */
 ff_w32(0x800bf39c,0x80041974); /* 8003D13C */
 ff_w32(0x800beee4,0x8002e11c); /* 8003D148 */
 ff_w32(0x800beee8,0x8002e11c); /* 8003D14C */
 ff_w32(0x800be700,0x8002ccbc); /* 8003D158 */
 ff_w32(0x800be704,0x8002ccbc); /* 8003D15C */
 ff_w32(0x800be708,0x8002ccbc); /* 8003D160 */
 ff_w32(0x800be70c,0x8002ccbc); /* 8003D164 */
 ff_w32(0x800bf3cc,0x8002aeb8); /* 8003D170 */
 ff_w32(0x800bf3d0,0x8002aee8); /* 8003D17C */
 ff_w32(0x800bf3d4,0x8002aee8); /* 8003D180 */
 ff_w32(0x800bf3d8,0x8002aee8); /* 8003D184 */
 ff_w32(0x800bf3dc,0x8002aee8); /* 8003D188 */
 ff_w32(0x800bef48,0x8002af18); /* 8003D194 */
 ff_w32(0x800bef4c,0x8002af48); /* 8003D1A0 */
 ff_w32(0x800bef50,0x8002af78); /* 8003D1AC */
 ff_w32(0x800bef58,0x8002b120); /* 8003D1B8 */
 ff_w32(0x800bef5c,0x8002b120); /* 8003D1BC */
 ff_w32(0x800bf3f8,0x8002f828); /* 8003D1C8 */
 ff_w32(0x800bf3fc,0x8002f828); /* 8003D1CC */
 ff_w32(0x800bf400,0x8002f828); /* 8003D1D0 */
 ff_w32(0x800bf404,0x8002f828); /* 8003D1D4 */
 ff_w32(0x800bf3e8,0x8002f700); /* 8003D1E0 */
 ff_w32(0x800bf3ec,0x8002f700); /* 8003D1E4 */
 ff_w32(0x800bf3f0,0x8002f700); /* 8003D1E8 */
 ff_w32(0x800bf3f4,0x8002f700); /* 8003D1EC */
 ff_w32(0x800bef54,0x8002b150); /* 8003D1F8 */
 ff_w32(0x800bf408,0x8002f950); /* 8003D204 */
 ff_w32(0x800bf40c,0x8002f950); /* 8003D208 */
 ff_w32(0x800bf410,0x8002f950); /* 8003D20C */
 ff_w32(0x800bf414,0x8002f950); /* 8003D210 */
 ff_w32(0x800beeec,0x8002ed7c); /* 8003D21C */
 ff_w32(0x80098270,0x8005c2f0); /* 8003D230 */
 ff_w32(0x80098274,0x8005c2f0); /* 8003D234 */
 ff_w32(0x80098288,0x8005c480); /* 8003D240 */
 ff_w32(0x8009828c,0x8005c598); /* 8003D24C */
 ff_w32(0x80098280,0x8005c654); /* 8003D258 */
 ff_w32(0x80099074,0x8002abe0); /* 8003D264 */
 ff_w32(0x80098278,0x8005c65c); /* 8003D270 */
 ff_w32(0x8009827c,0x8005c664); /* 8003D27C */
 ff_w32(0x80098bcc,0x8005c66c); /* 8003D288 */
 ff_w32(0x80098bd0,0x8005c66c); /* 8003D28C */
 ff_w32(0x80098bd4,0x8005c66c); /* 8003D290 */
 ff_w32(0x80098284,0x8005d608); /* 8003D29C */
 ff_w32(0x80098b74,0x8005bbac); /* 8003D2A8 */
 ff_w32(0x80098b78,0x8005bc64); /* 8003D2B4 */
 ff_w32(0x80098b7c,0x8005ba28); /* 8003D2C0 */
 ff_w32(0x80099008,0x8005c808); /* 8003D2CC */
 ff_w32(0x8009900c,0x8005c808); /* 8003D2D0 */
 ff_w32(0x80099010,0x8005c808); /* 8003D2D4 */
 ff_w32(0x80099014,0x8005c808); /* 8003D2D8 */
 ff_w32(0x8009902c,0x8005c808); /* 8003D2DC */
 ff_w32(0x8009905c,0x8005d160); /* 8003D2E8 */
 ff_w32(0x80099060,0x8005d1fc); /* 8003D2F4 */
 ff_w32(0x80099064,0x8005d1fc); /* 8003D2F8 */
 ff_w32(0x80099068,0x8005d1fc); /* 8003D2FC */
 ff_w32(0x8009906c,0x8005d1fc); /* 8003D300 */
 ff_w32(0x80098bd8,0x8005d340); /* 8003D30C */
 ff_w32(0x80098bdc,0x8005d3d8); /* 8003D318 */
 ff_w32(0x80098be0,0x8005d3d8); /* 8003D31C */
 ff_w32(0x80098be8,0x8005d488); /* 8003D328 */
 ff_w32(0x80098bec,0x8005d488); /* 8003D32C */
 ff_w32(0x80098b6c,0x8005c808); /* 8003D330 */
 ff_w32(0x80098b70,0x8005c808); /* 8003D334 */
 ff_w32(0x80099088,0x8005d2e4); /* 8003D340 */
 ff_w32(0x8009908c,0x8005d2e4); /* 8003D344 */
 ff_w32(0x80099090,0x8005d2e4); /* 8003D348 */
 ff_w32(0x80099094,0x8005d2e4); /* 8003D34C */
 ff_w32(0x80099098,0x8005d2e4); /* 8003D350 */
 ff_w32(0x8009909c,0x8005d2e4); /* 8003D354 */
 ff_w32(0x800990a0,0x8005d2e4); /* 8003D358 */
 ff_w32(0x800990a4,0x8005d2e4); /* 8003D35C */
 ff_w32(0x80098be4,0x8005d53c); /* 8003D36C */
 return 0x80098270;
}
uint32 FUN_80045BE4(void) {
 uint32 i;
 for(i=0;i<289;i++)ff_w32(0x800aee90+4*i,0x80052fe8); /* 80045C00 */
 ff_w32(0x800aefa4,0x80053278); /* 80045C24 */
 ff_w32(0x800af1dc,0x80053278); /* 80045C28 */
 ff_w32(0x800af1e0,0x80053278); /* 80045C2C */
 ff_w32(0x800af1e4,0x80053278); /* 80045C30 */
 ff_w32(0x800af1e8,0x80053278); /* 80045C34 */
 ff_w32(0x800af1ec,0x80053278); /* 80045C38 */
 ff_w32(0x800af1f0,0x80053278); /* 80045C3C */
 ff_w32(0x800af1f4,0x80053278); /* 80045C40 */
 ff_w32(0x800af1f8,0x80053278); /* 80045C44 */
 ff_w32(0x800af1fc,0x80053278); /* 80045C48 */
 ff_w32(0x800aefa4,0x80053278); /* 80045C4C */
 ff_w32(0x800aefa8,0x80053278); /* 80045C50 */
 ff_w32(0x800aefb4,0x80053278); /* 80045C54 */
 ff_w32(0x800aefc0,0x80053278); /* 80045C58 */
 ff_w32(0x800aefd0,0x80053278); /* 80045C5C */
 ff_w32(0x800aefcc,0x80053278); /* 80045C60 */
 ff_w32(0x800aefd4,0x80053278); /* 80045C64 */
 ff_w32(0x800aeffc,0x80053298); /* 80045C70 */
 ff_w32(0x800aefd8,0x80053558); /* 80045C7C */
 ff_w32(0x800aefdc,0x80053558); /* 80045C80 */
 ff_w32(0x800aefe4,0x80053558); /* 80045C84 */
 ff_w32(0x800af200,0x80053558); /* 80045C88 */
 ff_w32(0x800af204,0x80053558); /* 80045C8C */
 ff_w32(0x800af208,0x80053558); /* 80045C90 */
 ff_w32(0x800aefec,0x80053558); /* 80045C94 */
 ff_w32(0x800af018,0x80053558); /* 80045C98 */
 ff_w32(0x800af02c,0x80053558); /* 80045C9C */
 ff_w32(0x800af054,0x80053558); /* 80045CA0 */
 ff_w32(0x800af040,0x80053558); /* 80045CA4 */
 ff_w32(0x800af020,0x80053558); /* 80045CA8 */
 ff_w32(0x800af034,0x80053558); /* 80045CAC */
 ff_w32(0x800af05c,0x80053558); /* 80045CB0 */
 ff_w32(0x800af048,0x80053558); /* 80045CB4 */
 ff_w32(0x800af028,0x80053558); /* 80045CB8 */
 ff_w32(0x800af03c,0x80053558); /* 80045CBC */
 ff_w32(0x800af064,0x80053558); /* 80045CC0 */
 ff_w32(0x800af050,0x80053558); /* 80045CC4 */
 ff_w32(0x800af0a4,0x80053e1c); /* 80045CD0 */
 ff_w32(0x800af0a8,0x80053e1c); /* 80045CD4 */
 ff_w32(0x800af0ac,0x80053e1c); /* 80045CD8 */
 ff_w32(0x800af0b0,0x80053e1c); /* 80045CDC */
 ff_w32(0x800af0b4,0x80053e1c); /* 80045CE0 */
 ff_w32(0x800af0bc,0x80053e1c); /* 80045CE4 */
 ff_w32(0x800af0f0,0x80053e1c); /* 80045CE8 */
 ff_w32(0x800af0f4,0x80053e1c); /* 80045CEC */
 ff_w32(0x800af0fc,0x80053f4c); /* 80045CF8 */
 ff_w32(0x800af100,0x80053f4c); /* 80045CFC */
 ff_w32(0x800af104,0x80053f4c); /* 80045D00 */
 ff_w32(0x800af154,0x80054988); /* 80045D0C */
 ff_w32(0x800af10c,0x800555dc); /* 80045D18 */
 ff_w32(0x800af110,0x80054a40); /* 80045D24 */
 ff_w32(0x800af114,0x800555dc); /* 80045D28 */
 ff_w32(0x800af118,0x80054a40); /* 80045D2C */
 ff_w32(0x800af160,0x80053fc4); /* 80045D38 */
 ff_w32(0x800af164,0x80053fc4); /* 80045D3C */
 ff_w32(0x800af168,0x80053fc4); /* 80045D40 */
 ff_w32(0x800af16c,0x80053fc4); /* 80045D44 */
 ff_w32(0x800af170,0x80053fc4); /* 80045D48 */
 ff_w32(0x800af174,0x80053fc4); /* 80045D4C */
 ff_w32(0x800af178,0x80053fc4); /* 80045D50 */
 ff_w32(0x800af17c,0x80053fc4); /* 80045D54 */
 ff_w32(0x800af180,0x80053fc4); /* 80045D58 */
 ff_w32(0x800af184,0x80053fc4); /* 80045D5C */
 ff_w32(0x800af0c0,0x80053fc4); /* 80045D60 */
 ff_w32(0x800af0c4,0x80053fc4); /* 80045D64 */
 ff_w32(0x800af0c8,0x80053fc4); /* 80045D68 */
 ff_w32(0x800af0dc,0x80053fc4); /* 80045D6C */
 ff_w32(0x800af0e0,0x80053fc4); /* 80045D70 */
 ff_w32(0x800af0f8,0x80053fc4); /* 80045D74 */
 ff_w32(0x800af27c,0x80053fc4); /* 80045D78 */
 ff_w32(0x800af144,0x80053fc4); /* 80045D7C */
 ff_w32(0x800af2f4,0x80053fc4); /* 80045D80 */
 ff_w32(0x800af280,0x80054398); /* 80045D8C */
 ff_w32(0x800af284,0x80054398); /* 80045D90 */
 ff_w32(0x800af2c8,0x8005392c); /* 80045D9C */
 ff_w32(0x800af274,0x80055574); /* 80045DA8 */
 ff_w32(0x800af278,0x80055574); /* 80045DAC */
 ff_w32(0x800af0e4,0x80054474); /* 80045DB8 */
 ff_w32(0x800af0e8,0x80054474); /* 80045DBC */
 ff_w32(0x800af0ec,0x80054474); /* 80045DC0 */
 ff_w32(0x800af068,0x800539c0); /* 80045DCC */
 ff_w32(0x800af06c,0x80053a70); /* 80045DD8 */
 ff_w32(0x800af07c,0x80053b40); /* 80045DE4 */
 ff_w32(0x800af088,0x80053bf0); /* 80045DF0 */
 ff_w32(0x800af08c,0x80053ca0); /* 80045DFC */
 ff_w32(0x800af080,0x80053ca0); /* 80045E00 */
 ff_w32(0x800af074,0x80053cec); /* 80045E0C */
 ff_w32(0x800af078,0x80053dd0); /* 80045E18 */
 ff_w32(0x800af190,0x800544e4); /* 80045E24 */
 ff_w32(0x800af194,0x800544e4); /* 80045E28 */
 ff_w32(0x800af19c,0x800544e4); /* 80045E2C */
 ff_w32(0x800af1a0,0x800544e4); /* 80045E30 */
 ff_w32(0x800af128,0x80054598); /* 80045E3C */
 ff_w32(0x800aefb8,0x80054598); /* 80045E40 */
 ff_w32(0x800aeec4,0x80054598); /* 80045E44 */
 ff_w32(0x800aef30,0x80054598); /* 80045E48 */
 ff_w32(0x800af13c,0x80052f70); /* 80045E54 */
 ff_w32(0x800af14c,0x80052f70); /* 80045E58 */
 ff_w32(0x800af140,0x80052f70); /* 80045E5C */
 ff_w32(0x800aeeb8,0x800547b8); /* 80045E68 */
 ff_w32(0x800aeebc,0x80054840); /* 80045E74 */
 ff_w32(0x800aeec0,0x80054900); /* 80045E80 */
 ff_w32(0x800aee90,0x80054a40); /* 80045E84 */
 ff_w32(0x800aee94,0x80054a40); /* 80045E88 */
 ff_w32(0x800aee98,0x80054a40); /* 80045E8C */
 ff_w32(0x800aee9c,0x80054a40); /* 80045E90 */
 ff_w32(0x800aeea0,0x80054a40); /* 80045E94 */
 ff_w32(0x800aeea4,0x80054a40); /* 80045E98 */
 ff_w32(0x800af228,0x80054a40); /* 80045E9C */
 ff_w32(0x800af22c,0x80054a40); /* 80045EA0 */
 ff_w32(0x800af230,0x80054a40); /* 80045EA4 */
 ff_w32(0x800aeef8,0x80054a40); /* 80045EA8 */
 ff_w32(0x800aeefc,0x80054a40); /* 80045EAC */
 ff_w32(0x800aef00,0x80054a40); /* 80045EB0 */
 ff_w32(0x800aef1c,0x80054a40); /* 80045EB4 */
 ff_w32(0x800aef24,0x80054a40); /* 80045EB8 */
 ff_w32(0x800aef28,0x80054a40); /* 80045EBC */
 ff_w32(0x800af21c,0x80054a40); /* 80045EC0 */
 ff_w32(0x800af220,0x80054a40); /* 80045EC4 */
 ff_w32(0x800af224,0x80054a40); /* 80045EC8 */
 ff_w32(0x800aef10,0x80054a40); /* 80045ECC */
 ff_w32(0x800af20c,0x80054a40); /* 80045ED0 */
 ff_w32(0x800af210,0x80054a40); /* 80045ED4 */
 ff_w32(0x800af214,0x80054a40); /* 80045ED8 */
 ff_w32(0x800af218,0x80054a40); /* 80045EDC */
 ff_w32(0x800aef20,0x80054a40); /* 80045EE0 */
 ff_w32(0x800aef54,0x80054a40); /* 80045EE4 */
 ff_w32(0x800aef18,0x80054a40); /* 80045EE8 */
 ff_w32(0x800aef38,0x80054a40); /* 80045EEC */
 ff_w32(0x800aef3c,0x80054a40); /* 80045EF0 */
 ff_w32(0x800aef40,0x80054a40); /* 80045EF4 */
 ff_w32(0x800aef44,0x80054a40); /* 80045EF8 */
 ff_w32(0x800aef48,0x80054a40); /* 80045EFC */
 ff_w32(0x800aef4c,0x80054a40); /* 80045F00 */
 ff_w32(0x800aef50,0x80054a40); /* 80045F04 */
 ff_w32(0x800aef58,0x80054a40); /* 80045F08 */
 ff_w32(0x800aef5c,0x80054a40); /* 80045F0C */
 ff_w32(0x800aef60,0x80054a40); /* 80045F10 */
 ff_w32(0x800aef64,0x80054a40); /* 80045F14 */
 ff_w32(0x800aef74,0x80055100); /* 80045F20 */
 ff_w32(0x800aef80,0x80055100); /* 80045F24 */
 ff_w32(0x800aef7c,0x80055100); /* 80045F28 */
 ff_w32(0x800aef78,0x80055100); /* 80045F2C */
 ff_w32(0x800af1d0,0x800551f0); /* 80045F38 */
 ff_w32(0x800af1d8,0x800551f0); /* 80045F3C */
 ff_w32(0x800af1d4,0x800551f0); /* 80045F40 */
 ff_w32(0x800aef84,0x80055100); /* 80045F44 */
 ff_w32(0x800aef90,0x80055100); /* 80045F48 */
 ff_w32(0x800aef8c,0x80055100); /* 80045F4C */
 ff_w32(0x800aef88,0x80055100); /* 80045F50 */
 ff_w32(0x800af25c,0x8005531c); /* 80045F5C */
 ff_w32(0x800af260,0x8005531c); /* 80045F60 */
 ff_w32(0x800af264,0x8005531c); /* 80045F64 */
 ff_w32(0x800af268,0x8005531c); /* 80045F68 */
 ff_w32(0x800af26c,0x800553a4); /* 80045F74 */
 ff_w32(0x800af270,0x800553a4); /* 80045F78 */
 ff_w32(0x800af1c4,0x800553f0); /* 80045F84 */
 ff_w32(0x800af1c8,0x80055528); /* 80045F90 */
 ff_w32(0x800af1bc,0x800553f0); /* 80045F94 */
 ff_w32(0x800af1c0,0x80055528); /* 80045F98 */
 ff_w32(0x800af1cc,0x800553f0); /* 80045F9C */
 ff_w32(0x800af238,0x80055770); /* 80045FA8 */
 ff_w32(0x800af23c,0x80055770); /* 80045FAC */
 ff_w32(0x800af240,0x80055770); /* 80045FB0 */
 ff_w32(0x800af244,0x80055770); /* 80045FB4 */
 ff_w32(0x800af248,0x80055770); /* 80045FB8 */
 ff_w32(0x800af24c,0x80055770); /* 80045FBC */
 ff_w32(0x800af2c4,0x80055918); /* 80045FC8 */
 ff_w32(0x800af308,0x800565a4); /* 80045FD4 */
 ff_init_copy289(0x8009cd48,0x800aee90); /* 80045FE8 */
 ff_w32(0x8009cfe0,0x8004cb88); /* 80046030 */
 ff_w32(0x8009cfe4,0x8004cd5c); /* 8004603C */
 ff_w32(0x8009cfec,0x8004ce48); /* 80046048 */
 ff_init_copy289(0x80099f40,0x800aee90); /* 80046060 */
 ff_w32(0x8009a368,0x8004d2d4); /* 800460A8 */
 ff_w32(0x8009a36c,0x8004d20c); /* 800460B4 */
 ff_w32(0x8009a370,0x8004d288); /* 800460C0 */
 ff_w32(0x8009a338,0x8004d2d4); /* 800460C4 */
 ff_w32(0x8009a33c,0x8004d2d4); /* 800460C8 */
 ff_w32(0x8009a340,0x8004d2d4); /* 800460CC */
 ff_w32(0x8009a344,0x8004d2d4); /* 800460D0 */
 ff_w32(0x8009a348,0x8004d2d4); /* 800460D4 */
 ff_w32(0x8009a34c,0x8004d2d4); /* 800460D8 */
 ff_w32(0x80099fbc,0x8004d914); /* 800460E4 */
 for(i=0;i<289;i++)ff_w32(0x800b3e90+4*i,0x8004bf3c); /* 80046104 */
 ff_w32(0x800b3fd8,0x8004bfa0); /* 80046128 */
 ff_w32(0x800b3fec,0x8004bfa0); /* 8004612C */
 ff_w32(0x800b3fa4,0x8004c134); /* 80046138 */
 ff_w32(0x800b3e90,0x8004c180); /* 80046144 */
 for(i=0;i<289;i++)ff_w32(0x8009e3d0+4*i,0x8004e38c); /* 80046164 */
 ff_w32(0x8009e4b4,0x8004e484); /* 80046188 */
 ff_w32(0x8009e4c0,0x8004e484); /* 8004618C */
 ff_w32(0x8009e4bc,0x8004e484); /* 80046190 */
 ff_w32(0x8009e4b8,0x8004e484); /* 80046194 */
 ff_w32(0x8009e5e4,0x8004ec94); /* 800461A0 */
 ff_w32(0x8009e5e8,0x8004ec94); /* 800461A4 */
 ff_w32(0x8009e630,0x8004ec94); /* 800461A8 */
 ff_w32(0x8009e5a8,0x8004ef64); /* 800461B4 */
 ff_w32(0x8009e5ac,0x8004ef6c); /* 800461C0 */
 ff_w32(0x8009e3d0,0x8004efb8); /* 800461CC */
 ff_w32(0x8009e3d4,0x8004efb8); /* 800461D0 */
 ff_w32(0x8009e4e4,0x8004f000); /* 800461DC */
 ff_w32(0x8009e518,0x8004f084); /* 800461E8 */
 ff_w32(0x8009e808,0x8004e574); /* 800461F4 */
 ff_w32(0x8009e80c,0x8004e7e8); /* 80046200 */
 ff_w32(0x8009e810,0x8004ec48); /* 8004620C */
 ff_w32(0x8009e814,0x8004ece0); /* 80046218 */
 ff_w32(0x8009e818,0x8004ed54); /* 80046224 */
 ff_w32(0x8009e81c,0x8004edb8); /* 80046230 */
 ff_w32(0x8009e820,0x8004ef18); /* 80046240 */
 return 0x8004ef18;
}
uint32 FUN_8003D370(void) {
 uint32 i;
 for(i=0;i<921;i++)ff_w32(0x80097408+4*i,0x8002a0c4); /* 8003D3B0 */
 for(i=0;i<416;i++)ff_w32(0x800974e8+4*i,0x8002a0c4); /* 8003D3E0 */
 ff_w32(0x800bffdc,0x8005dacc); /* 8003D400 */
 ff_w32(0x80097e50,0x8005b7ac); /* 8003D414 */
 ff_w32(0x80097e54,0x8005b7ac); /* 8003D418 */
 ff_w32(0x80097618,0x800326dc); /* 8003D424 */
 ff_w32(0x8009761c,0x800326dc); /* 8003D428 */
 ff_w32(0x80097620,0x800326dc); /* 8003D42C */
 ff_w32(0x80097624,0x80032728); /* 8003D438 */
 ff_w32(0x80097dec,0x8005ab84); /* 8003D444 */
 ff_w32(0x800974e8,0x80029f48); /* 8003D450 */
 ff_w32(0x80097b48,0x8002a048); /* 8003D45C */
 ff_w32(0x80097540,0x8005ac08); /* 8003D468 */
 ff_w32(0x80097544,0x8005ac08); /* 8003D46C */
 ff_w32(0x80097548,0x8005ac08); /* 8003D470 */
 ff_w32(0x8009754c,0x8005ac08); /* 8003D474 */
 ff_w32(0x80097550,0x8005ac08); /* 8003D478 */
 ff_w32(0x80097554,0x8005ac08); /* 8003D47C */
 ff_w32(0x80097558,0x8005ac08); /* 8003D480 */
 ff_w32(0x8009755c,0x8005ad54); /* 8003D48C */
 ff_w32(0x80097560,0x8005ad54); /* 8003D490 */
 ff_w32(0x80097564,0x8005ad54); /* 8003D494 */
 ff_w32(0x80097568,0x8005ad54); /* 8003D498 */
 ff_w32(0x80097570,0x8005b350); /* 8003D4A4 */
 ff_w32(0x80097500,0x8005b350); /* 8003D4A8 */
 ff_w32(0x8009744c,0x8005b350); /* 8003D4AC */
 ff_w32(0x80097510,0x8005b350); /* 8003D4B0 */
 ff_w32(0x80097504,0x8005b350); /* 8003D4B4 */
 ff_w32(0x80097508,0x8005b404); /* 8003D4C0 */
 ff_w32(0x80097580,0x8005ae3c); /* 8003D4CC */
 ff_w32(0x80097584,0x8005ae3c); /* 8003D4D0 */
 ff_w32(0x80097588,0x8005b178); /* 8003D4DC */
 ff_w32(0x80097594,0x8005ad54); /* 8003D4E0 */
 ff_w32(0x80097598,0x8005ad54); /* 8003D4E4 */
 ff_w32(0x8009757c,0x8003262c); /* 8003D4F0 */
 ff_w32(0x800975b8,0x8005ae3c); /* 8003D4F4 */
 ff_w32(0x800975bc,0x8005ae3c); /* 8003D4F8 */
 ff_w32(0x800975c0,0x8005b178); /* 8003D4FC */
 ff_w32(0x800975e0,0x8002a97c); /* 8003D508 */
 ff_w32(0x800975d0,0x8002ab40); /* 8003D514 */
 ff_w32(0x800975d4,0x8002ab40); /* 8003D518 */
 ff_w32(0x800975d8,0x8002ab40); /* 8003D51C */
 ff_w32(0x800975dc,0x8002ab40); /* 8003D520 */
 ff_w32(0x80097b50,0x8005dacc); /* 8003D524 */
 ff_w32(0x80097e10,0x8002a0c4); /* 8003D530 */
 ff_w32(0x80097e14,0x8002a0c4); /* 8003D534 */
 ff_w32(0x80097e18,0x8002a0c4); /* 8003D538 */
 ff_w32(0x80097e1c,0x8002a0c4); /* 8003D53C */
 ff_w32(0x80097e2c,0x8002a0c4); /* 8003D540 */
 ff_w32(0x80097e44,0x8002a0c4); /* 8003D544 */
 ff_w32(0x80097e48,0x8002a0c4); /* 8003D548 */
 ff_w32(0x800975f4,0x80032680); /* 8003D554 */
 ff_w32(0x80097e70,0x80032680); /* 8003D558 */
 ff_w32(0x800975e4,0x8002a00c); /* 8003D564 */
 ff_w32(0x800975f0,0x8002a00c); /* 8003D568 */
 ff_w32(0x80097628,0x80032774); /* 8003D574 */
 ff_w32(0x80097e88,0x8002a0c4); /* 8003D578 */
 ff_w32(0x80097e8c,0x8002a0c4); /* 8003D57C */
 ff_w32(0x80097e90,0x8002a0c4); /* 8003D580 */
 ff_w32(0x80097634,0x8002c294); /* 8003D58C */
 ff_w32(0x80097638,0x8002c294); /* 8003D590 */
 ff_w32(0x8009763c,0x80032774); /* 8003D594 */
 ff_w32(0x80097ea0,0x800327b0); /* 8003D5A0 */
 ff_w32(0x80097650,0x8002cac4); /* 8003D5AC */
 ff_w32(0x80097748,0x80032818); /* 8003D5B8 */
 ff_w32(0x8009774c,0x80032818); /* 8003D5BC */
 ff_w32(0x80097750,0x80032818); /* 8003D5C0 */
 ff_w32(0x80097754,0x80032818); /* 8003D5C4 */
 ff_w32(0x80097760,0x80032818); /* 8003D5C8 */
 ff_w32(0x800977a4,0x80032818); /* 8003D5CC */
 ff_w32(0x800977c4,0x80032818); /* 8003D5D0 */
 ff_w32(0x800977ac,0x80032818); /* 8003D5D4 */
 ff_w32(0x80097794,0x80032818); /* 8003D5D8 */
 ff_w32(0x8009779c,0x80032818); /* 8003D5DC */
 ff_w32(0x800977b4,0x80032818); /* 8003D5E0 */
 ff_w32(0x800977bc,0x80032818); /* 8003D5E4 */
 ff_w32(0x800977cc,0x80032818); /* 8003D5E8 */
 ff_w32(0x8009777c,0x80032818); /* 8003D5EC */
 ff_w32(0x8009778c,0x80032818); /* 8003D5F0 */
 ff_w32(0x80097764,0x80032818); /* 8003D5F4 */
 ff_w32(0x8009776c,0x80032818); /* 8003D5F8 */
 ff_w32(0x80097784,0x80032818); /* 8003D5FC */
 ff_w32(0x80097f40,0x80032818); /* 8003D600 */
 ff_w32(0x80097f44,0x80032818); /* 8003D604 */
 ff_w32(0x80097ee0,0x80032818); /* 8003D608 */
 ff_w32(0x80097ee8,0x80046ef0); /* 8003D614 */
 ff_w32(0x80097eec,0x80032a48); /* 8003D620 */
 ff_w32(0x80097ef0,0x80032818); /* 8003D624 */
 ff_w32(0x80097ef4,0x80032818); /* 8003D628 */
 ff_w32(0x80097ef8,0x80032818); /* 8003D62C */
 ff_w32(0x80097efc,0x80032818); /* 8003D630 */
 ff_w32(0x80097f08,0x80032818); /* 8003D634 */
 ff_w32(0x80097f0c,0x80032818); /* 8003D638 */
 ff_w32(0x80097f10,0x80032818); /* 8003D63C */
 ff_w32(0x80097f14,0x80032818); /* 8003D640 */
 ff_w32(0x80097f24,0x80032818); /* 8003D644 */
 ff_w32(0x80097f28,0x80032818); /* 8003D648 */
 ff_w32(0x80097f2c,0x80032818); /* 8003D64C */
 ff_w32(0x80097f04,0x80032818); /* 8003D650 */
 ff_w32(0x80097f30,0x80032818); /* 8003D654 */
 ff_w32(0x80097f34,0x80032818); /* 8003D658 */
 ff_w32(0x80097f48,0x8005b4f8); /* 8003D664 */
 ff_w32(0x80097f4c,0x8005b4f8); /* 8003D668 */
 ff_w32(0x80097f50,0x8005b598); /* 8003D674 */
 ff_w32(0x800977e4,0x80032818); /* 8003D678 */
 ff_w32(0x800977e8,0x80032818); /* 8003D67C */
 ff_w32(0x80097b54,0x80032aa4); /* 8003D688 */
 ff_w32(0x80097b58,0x80032aa4); /* 8003D68C */
 ff_w32(0x80097b5c,0x80032b20); /* 8003D698 */
 ff_w32(0x80097b60,0x80032b20); /* 8003D69C */
 ff_w32(0x80097758,0x80032b98); /* 8003D6A8 */
 ff_w32(0x80097f3c,0x8002a0c4); /* 8003D6AC */
 ff_w32(0x80097828,0x8005b71c); /* 8003D6B8 */
 ff_w32(0x8009782c,0x8005b71c); /* 8003D6BC */
 ff_w32(0x80097830,0x8005b71c); /* 8003D6C0 */
 ff_w32(0x80097834,0x8005b71c); /* 8003D6C4 */
 ff_w32(0x80097f78,0x8005b71c); /* 8003D6C8 */
 ff_w32(0x80097f7c,0x8005b71c); /* 8003D6CC */
 ff_w32(0x80097f80,0x8005b71c); /* 8003D6D0 */
 ff_w32(0x80097844,0x8005b71c); /* 8003D6D4 */
 ff_w32(0x80097848,0x8005b71c); /* 8003D6D8 */
 ff_w32(0x8009784c,0x8005b71c); /* 8003D6DC */
 ff_w32(0x80097850,0x8005b71c); /* 8003D6E0 */
 ff_w32(0x80097854,0x8005b71c); /* 8003D6E4 */
 ff_w32(0x80097858,0x8005b71c); /* 8003D6E8 */
 ff_w32(0x8009785c,0x8005b71c); /* 8003D6EC */
 ff_w32(0x80097fd4,0x8005b71c); /* 8003D6F0 */
 ff_w32(0x80097860,0x8005b71c); /* 8003D6F4 */
 ff_w32(0x80097864,0x8005b71c); /* 8003D6F8 */
 ff_w32(0x80097868,0x8005b71c); /* 8003D6FC */
 ff_w32(0x80097870,0x8005b71c); /* 8003D700 */
 ff_w32(0x80097874,0x8005b71c); /* 8003D704 */
 ff_w32(0x80097878,0x8005b71c); /* 8003D708 */
 ff_w32(0x8009789c,0x8005b71c); /* 8003D70C */
 ff_w32(0x800978a0,0x8005b71c); /* 8003D710 */
 ff_w32(0x8009786c,0x80032d44); /* 8003D71C */
 ff_w32(0x800978c0,0x80032c4c); /* 8003D728 */
 ff_w32(0x800978c4,0x80032c4c); /* 8003D72C */
 ff_w32(0x800978c8,0x80032c4c); /* 8003D730 */
 ff_w32(0x800978d4,0x8005d7a4); /* 8003D73C */
 ff_w32(0x800978dc,0x8005d7a4); /* 8003D740 */
 ff_w32(0x800978e4,0x8005d7a4); /* 8003D744 */
 ff_w32(0x800978ec,0x8005d7a4); /* 8003D748 */
 ff_w32(0x8009795c,0x8002a640); /* 8003D754 */
 ff_w32(0x80098028,0x8002a7dc); /* 8003D760 */
 ff_w32(0x800979a8,0x8002a858); /* 8003D76C */
 ff_w32(0x80098048,0x8002a89c); /* 8003D778 */
 ff_w32(0x800979fc,0x8002a0c4); /* 8003D77C */
 ff_w32(0x800979f4,0x8002a0c4); /* 8003D780 */
 ff_w32(0x800979ec,0x8002a0c4); /* 8003D784 */
 ff_w32(0x800979e4,0x8002a0c4); /* 8003D788 */
 ff_w32(0x800979dc,0x8002a0c4); /* 8003D78C */
 ff_w32(0x800979d4,0x8002a0c4); /* 8003D790 */
 ff_w32(0x800979c4,0x8002a0c4); /* 8003D794 */
 ff_w32(0x80098058,0x80032d70); /* 8003D7A0 */
 ff_w32(0x80098054,0x80032d70); /* 8003D7A4 */
 ff_w32(0x80098050,0x80032d70); /* 8003D7A8 */
 ff_w32(0x8009807c,0x80032d70); /* 8003D7AC */
 ff_w32(0x80098078,0x80032d70); /* 8003D7B0 */
 ff_w32(0x80098074,0x80032d70); /* 8003D7B4 */
 ff_w32(0x80098070,0x80032d70); /* 8003D7B8 */
 ff_w32(0x8009806c,0x80032d70); /* 8003D7BC */
 ff_w32(0x80098068,0x80032d70); /* 8003D7C0 */
 ff_w32(0x80098064,0x80032d70); /* 8003D7C4 */
 ff_w32(0x80098060,0x80032d70); /* 8003D7C8 */
 ff_w32(0x8009805c,0x80032d70); /* 8003D7CC */
 ff_w32(0x800980b8,0x80032d70); /* 8003D7D0 */
 ff_w32(0x800980b0,0x80032d70); /* 8003D7D4 */
 ff_w32(0x800980a8,0x80032d70); /* 8003D7D8 */
 ff_w32(0x800980a0,0x80032d70); /* 8003D7DC */
 ff_w32(0x80098098,0x80032d70); /* 8003D7E0 */
 ff_w32(0x80098094,0x80032d70); /* 8003D7E4 */
 ff_w32(0x80098090,0x80032d70); /* 8003D7E8 */
 ff_w32(0x80098084,0x80032d70); /* 8003D7EC */
 ff_w32(0x80098080,0x80032d70); /* 8003D7F0 */
 ff_w32(0x80097a34,0x80032d70); /* 8003D7F4 */
 ff_w32(0x80097b64,0x8005dacc); /* 8003D7F8 */
 ff_w32(0x80097a38,0x80032774); /* 8003D7FC */
 ff_w32(0x80097a40,0x800326dc); /* 8003D800 */
 ff_w32(0x80097a48,0x8002a8fc); /* 8003D80C */
 ff_w32(0x800979bc,0x8005b71c); /* 8003D810 */
 ff_w32(0x80097a28,0x80032e20); /* 8003D81C */
 ff_w32(0x80097a58,0x800428ec); /* 8003D828 */
 ff_w32(0x80097a5c,0x8005b71c); /* 8003D82C */
 ff_w32(0x80097a54,0x8005b71c); /* 8003D830 */
 ff_w32(0x80097a50,0x8005b71c); /* 8003D834 */
 ff_w32(0x80097a4c,0x8005b71c); /* 8003D838 */
 ff_w32(0x80097a70,0x8005b71c); /* 8003D83C */
 ff_w32(0x80097a6c,0x8005b71c); /* 8003D840 */
 ff_w32(0x80097a68,0x8005b71c); /* 8003D844 */
 ff_w32(0x80097a64,0x8005b71c); /* 8003D848 */
 ff_w32(0x80097cd0,0x8005b71c); /* 8003D84C */
 ff_w32(0x80097aa8,0x8005b71c); /* 8003D850 */
 ff_w32(0x80097aa4,0x8005b71c); /* 8003D854 */
 ff_w32(0x80097aa0,0x8005b71c); /* 8003D858 */
 ff_w32(0x80097a9c,0x8005b71c); /* 8003D85C */
 ff_w32(0x80097a98,0x8005b71c); /* 8003D860 */
 ff_w32(0x80097a94,0x8005b71c); /* 8003D864 */
 ff_w32(0x80097a90,0x8005b71c); /* 8003D868 */
 ff_w32(0x80097a8c,0x8005b71c); /* 8003D86C */
 ff_w32(0x80097a88,0x8005b71c); /* 8003D870 */
 ff_w32(0x80097a84,0x8005b71c); /* 8003D874 */
 ff_w32(0x80097a80,0x8005b71c); /* 8003D878 */
 ff_w32(0x80097a7c,0x8005b71c); /* 8003D87C */
 ff_w32(0x80097a78,0x8005b71c); /* 8003D880 */
 ff_w32(0x80097a74,0x8005b71c); /* 8003D884 */
 ff_w32(0x80097ac8,0x8005b71c); /* 8003D888 */
 ff_w32(0x80097ac4,0x8005b71c); /* 8003D88C */
 ff_w32(0x80097ac0,0x8005b71c); /* 8003D890 */
 ff_w32(0x80097abc,0x8005b71c); /* 8003D894 */
 ff_w32(0x80097ab8,0x8005b71c); /* 8003D898 */
 ff_w32(0x80097ab4,0x8005b71c); /* 8003D89C */
 ff_w32(0x80097ab0,0x8005b71c); /* 8003D8A0 */
 ff_w32(0x80097aac,0x8005b71c); /* 8003D8A4 */
 ff_w32(0x80097acc,0x8005b844); /* 8003D8B0 */
 ff_w32(0x80097ad8,0x8005bd7c); /* 8003D8BC */
 ff_w32(0x80097af4,0x8005b71c); /* 8003D8C0 */
 ff_w32(0x80098140,0x8005bf7c); /* 8003D8CC */
 ff_w32(0x80098144,0x8005bf7c); /* 8003D8D0 */
 ff_w32(0x80098148,0x8005bf7c); /* 8003D8D4 */
 ff_w32(0x8009814c,0x8005bf7c); /* 8003D8D8 */
 ff_w32(0x80098150,0x8005bf7c); /* 8003D8DC */
 ff_w32(0x80098160,0x8005bff4); /* 8003D8E8 */
 ff_w32(0x80098164,0x8005bff4); /* 8003D8EC */
 ff_w32(0x80098168,0x8005bf7c); /* 8003D8F0 */
 ff_w32(0x8009816c,0x8005bf7c); /* 8003D8F4 */
 ff_w32(0x80098170,0x8005bf7c); /* 8003D8F8 */
 ff_w32(0x80098174,0x8005bf7c); /* 8003D8FC */
 ff_w32(0x80097b18,0x8005bf7c); /* 8003D900 */
 ff_w32(0x80097b1c,0x8005bf0c); /* 8003D90C */
 for(i=0;i<289;i++)ff_w32(0x800bb9b0+4*i,0x80032ed8); /* 8003D92C */
 ff_w32(0x800bbbfc,0x800332bc); /* 8003D950 */
 ff_w32(0x800bbabc,0x80033374); /* 8003D95C */
 ff_w32(0x800bbc74,0x80033c44); /* 8003D968 */
 ff_w32(0x800bbc80,0x800332bc); /* 8003D96C */
 ff_w32(0x800bbc84,0x80033488); /* 8003D978 */
 ff_w32(0x800bbc88,0x80033488); /* 8003D97C */
 ff_w32(0x800bbc8c,0x80033534); /* 8003D988 */
 ff_w32(0x800bbc90,0x800336cc); /* 8003D994 */
 ff_w32(0x800bbc94,0x8003375c); /* 8003D9A0 */
 ff_w32(0x800bbc98,0x800338e0); /* 8003D9AC */
 ff_w32(0x800bbc9c,0x800338e0); /* 8003D9B0 */
 ff_w32(0x800bbca0,0x800338e0); /* 8003D9B4 */
 ff_w32(0x800bbca4,0x80033534); /* 8003D9B8 */
 ff_w32(0x800bb9d4,0x80033068); /* 8003D9C4 */
 ff_w32(0x800bbcb0,0x80033d7c); /* 8003D9D0 */
 ff_w32(0x800bbcc0,0x80033dc8); /* 8003D9DC */
 ff_w32(0x800bbcb4,0x80033e18); /* 8003D9E8 */
 ff_w32(0x800bbcb8,0x80033e38); /* 8003D9F4 */
 ff_w32(0x800bba4c,0x8003314c); /* 8003DA00 */
 ff_w32(0x800bba34,0x8003314c); /* 8003DA04 */
 ff_w32(0x800bba3c,0x800330cc); /* 8003DA10 */
 ff_w32(0x800bba44,0x800330cc); /* 8003DA14 */
 ff_w32(0x800bba48,0x800330cc); /* 8003DA18 */
 ff_w32(0x800bba5c,0x800330cc); /* 8003DA1C */
 ff_w32(0x800bba60,0x800330cc); /* 8003DA20 */
 ff_w32(0x800bba54,0x80033e58); /* 8003DA2C */
 ff_w32(0x800bbbe0,0x80033a6c); /* 8003DA38 */
 ff_w32(0x800bbbe4,0x80033a6c); /* 8003DA3C */
 ff_w32(0x800bbbe8,0x80033a6c); /* 8003DA40 */
 ff_w32(0x800bbc8c,0x80033534); /* 8003DA44 */
 ff_w32(0x800bbbb8,0x80033f7c); /* 8003DA50 */
 ff_w32(0x800bbbbc,0x80033f7c); /* 8003DA54 */
 ff_w32(0x800bbbc0,0x800341a0); /* 8003DA60 */
 ff_w32(0x800bbc28,0x80033008); /* 8003DA6C */
 ff_w32(0x800bbab4,0x80033008); /* 8003DA70 */
 ff_w32(0x800bbab8,0x80033008); /* 8003DA74 */
 ff_w32(0x800bb9b0,0x80032f38); /* 8003DA80 */
 ff_w32(0x800bb9b4,0x80032f38); /* 8003DA84 */
 ff_w32(0x800bb9b8,0x80032f38); /* 8003DA88 */
 ff_w32(0x800bb9dc,0x80032f38); /* 8003DA8C */
 ff_w32(0x800bba10,0x80032f38); /* 8003DA90 */
 ff_w32(0x800bba14,0x80032f38); /* 8003DA94 */
 ff_w32(0x800bba18,0x80032f38); /* 8003DA98 */
 ff_w32(0x800bba1c,0x80032f38); /* 8003DA9C */
 ff_w32(0x800bba38,0x80032f38); /* 8003DAA0 */
 ff_w32(0x800bba18,0x80032f38); /* 8003DAA4 */
 ff_w32(0x800bba1c,0x80032f38); /* 8003DAA8 */
 ff_w32(0x800bba20,0x80032f38); /* 8003DAAC */
 ff_w32(0x800bbc2c,0x80032f38); /* 8003DAB0 */
 ff_w32(0x800bbc34,0x80032f38); /* 8003DAB4 */
 ff_w32(0x800bbdf4,0x80040fac); /* 8003DAC0 */
 ff_w32(0x800bbdf8,0x80040fac); /* 8003DAC4 */
 ff_w32(0x800bbdfc,0x80040fac); /* 8003DAC8 */
 ff_w32(0x800bbe00,0x80040fac); /* 8003DACC */
 ff_w16(0x8009ea90,0x4000); /* 8003DADC */
 ff_w16(0x8009ea94,0xfff6); /* 8003DAE4 */
 ff_w16(0x8009ea96,0x30); /* 8003DAEC */
 ff_w16(0x8009eaa8,0x4000); /* 8003DAF0 */
 ff_w16(0x8009eaac,0xfff6); /* 8003DAF4 */
 ff_w16(0x8009eaae,0x44); /* 8003DAFC */
 ff_w16(0x8009ea18,0xc000); /* 8003DB04 */
 ff_w16(0x8009ea0c,0xc000); /* 8003DB08 */
 ff_w16(0x8009ea1a,0x8); /* 8003DB10 */
 ff_w16(0x8009ea0e,0x8); /* 8003DB14 */
 ff_w16(0x8009ea1e,0xfff8); /* 8003DB1C */
 ff_w16(0x8009ea12,0xfff8); /* 8003DB20 */
 ff_w16(0x8009e8ac,0xc000); /* 8003DB24 */
 ff_w16(0x8009e894,0xc000); /* 8003DB28 */
 ff_w16(0x8009e87c,0xc000); /* 8003DB2C */
 ff_w16(0x8009e8a0,0xc000); /* 8003DB30 */
 ff_w16(0x8009e888,0xc000); /* 8003DB34 */
 ff_w16(0x8009e870,0xc000); /* 8003DB38 */
 ff_w16(0x8009e8b4,0xffd0); /* 8003DB40 */
 ff_w16(0x8009e89c,0xffd0); /* 8003DB44 */
 ff_w16(0x8009e884,0xffd0); /* 8003DB48 */
 ff_w16(0x8009e8a8,0xffd0); /* 8003DB4C */
 ff_w16(0x8009e890,0xffd0); /* 8003DB50 */
 ff_w16(0x8009e878,0xffd0); /* 8003DB54 */
 ff_w16(0x8009e8d2,0xc000); /* 8003DB58 */
 ff_w16(0x8009e8da,0x16); /* 8003DB60 */
 ff_w16(0x8009e8de,0xc000); /* 8003DB64 */
 ff_w16(0x8009e8e6,0x16); /* 8003DB68 */
 ff_w16(0x8009e8ea,0xc000); /* 8003DB6C */
 ff_w16(0x8009e8f2,0x16); /* 8003DB70 */
 ff_w16(0x8009e8f6,0xc000); /* 8003DB74 */
 ff_w16(0x8009e8fc,0xfff6); /* 8003DB78 */
 ff_w16(0x8009e8fe,0x10); /* 8003DB80 */
 ff_w16(0x8009e902,0x4000); /* 8003DB84 */
 ff_w16(0x8009e90a,0x14); /* 8003DB8C */
 ff_w16(0x8009e86e,0xffe0); /* 8003DB94 */
 ff_w16(0x8009ea06,0x20); /* 8003DB9C */
 ff_w16(0x8009e93a,0x20); /* 8003DBA0 */
 ff_w16(0x8009e916,0x20); /* 8003DBA4 */
 ff_w16(0x8009e946,0x20); /* 8003DBA8 */
 ff_w16(0x8009ea66,0x20); /* 8003DBAC */
 ff_w16(0x8009e8ce,0x20); /* 8003DBB0 */
 ff_w16(0x8009e8c2,0x20); /* 8003DBB4 */
 ff_w16(0x8009e99c,0x4000); /* 8003DBB8 */
 ff_w16(0x8009e990,0x4000); /* 8003DBBC */
 ff_w16(0x8009e984,0x4000); /* 8003DBC0 */
 ff_w16(0x8009e978,0x4000); /* 8003DBC4 */
 ff_w16(0x8009e96c,0x4000); /* 8003DBC8 */
 ff_w16(0x8009e9a2,0x4); /* 8003DBD0 */
 ff_w16(0x8009e996,0x4); /* 8003DBD4 */
 ff_w16(0x8009e98a,0x4); /* 8003DBD8 */
 ff_w16(0x8009e97e,0x4); /* 8003DBDC */
 ff_w16(0x8009e972,0x4); /* 8003DBE0 */
 ff_w16(0x8009e9a4,0x4); /* 8003DBE4 */
 ff_w16(0x8009e998,0x4); /* 8003DBE8 */
 ff_w16(0x8009e98c,0x4); /* 8003DBEC */
 ff_w16(0x8009e980,0x4); /* 8003DBF0 */
 ff_w16(0x8009e974,0x4); /* 8003DBF4 */
 ff_w16(0x8009e9a6,0x8); /* 8003DBF8 */
 ff_w16(0x8009e99a,0x8); /* 8003DBFC */
 ff_w16(0x8009e98e,0x8); /* 8003DC00 */
 ff_w16(0x8009e982,0x8); /* 8003DC04 */
 ff_w16(0x8009e976,0x8); /* 8003DC08 */
 ff_w16(0x8009e9b4,0x4000); /* 8003DC0C */
 ff_w16(0x8009e9ba,0x4); /* 8003DC10 */
 ff_w16(0x8009e9bc,0x4); /* 8003DC14 */
 ff_w16(0x8009e9be,0x8); /* 8003DC18 */
 ff_w16(0x8009e9b4,0x4000); /* 8003DC1C */
 ff_w16(0x8009e9ba,0x4); /* 8003DC20 */
 ff_w16(0x8009e9bc,0x4); /* 8003DC24 */
 ff_w16(0x8009e9be,0x8); /* 8003DC28 */
 ff_w16(0x8009e9a8,0x3400); /* 8003DC30 */
 ff_w16(0x8009e9aa,0x2000); /* 8003DC38 */
 ff_w16(0x8009e9ac,0xe000); /* 8003DC40 */
 ff_w16(0x8009e9b0,0xfff4); /* 8003DC48 */
 ff_w16(0x8009e9b2,0x7); /* 8003DC50 */
 ff_w16(0x8009e91a,0xc000); /* 8003DC54 */
 ff_w16(0x8009e91e,0x4); /* 8003DC58 */
 ff_w16(0x8009e920,0xfffc); /* 8003DC60 */
 ff_w16(0x8009e922,0x10); /* 8003DC64 */
 ff_w16(0x8009e9c0,0x4800); /* 8003DC6C */
 ff_w16(0x8009e9c4,0x400); /* 8003DC74 */
 ff_w16(0x8009e9c6,0x9); /* 8003DC7C */
 ff_w16(0x8009e9c8,0xfff4); /* 8003DC80 */
 ff_w16(0x8009e9ca,0xffe0); /* 8003DC84 */
 ff_w16(0x8009e9e4,0x4000); /* 8003DC88 */
 ff_w16(0x8009e9e8,0x400); /* 8003DC8C */
 ff_w16(0x8009e9ea,0x8); /* 8003DC90 */
 ff_w16(0x8009e9ec,0xffe5); /* 8003DC98 */
 ff_w16(0x8009e9ee,0x10); /* 8003DC9C */
 ff_w16(0x8009e9cc,0x4800); /* 8003DCA0 */
 ff_w16(0x8009e9d0,0x400); /* 8003DCA4 */
 ff_w16(0x8009e9d2,0x5); /* 8003DCAC */
 ff_w16(0x8009e9d4,0xffe8); /* 8003DCB4 */
 ff_w16(0x8009e9d6,0x8); /* 8003DCB8 */
 ff_w16(0x8009e9d8,0x1000); /* 8003DCC0 */
 ff_w16(0x8009e9da,0x8000); /* 8003DCC8 */
 ff_w16(0x8009e9dc,0x4900); /* 8003DCD0 */
 ff_w16(0x8009e9de,0xffe0); /* 8003DCD4 */
 ff_w16(0x8009e9e2,0xffc0); /* 8003DCDC */
 ff_w16(0x8009eab0,0x1000); /* 8003DCE0 */
 ff_w16(0x8009eab2,0x3e00); /* 8003DCE8 */
 ff_w16(0x8009eab4,0xd00); /* 8003DCF0 */
 ff_w16(0x8009eab6,0xfff8); /* 8003DCF4 */
 ff_w16(0x8009eab8,0x8); /* 8003DCF8 */
 ff_w16(0x8009eabe,0x4000); /* 8003DCFC */
 ff_w16(0x8009eac0,0x2d00); /* 8003DD04 */
 ff_w16(0x8009eac2,0xffea); /* 8003DD0C */
 ff_w16(0x8009eac4,0xe); /* 8003DD14 */
 ff_w16(0x8009eac6,0x9); /* 8003DD18 */
 ff_w16(0x8009ea2c,0xc000); /* 8003DD1C */
 ff_w16(0x8009ea2e,0x8400); /* 8003DD24 */
 ff_w16(0x8009ea30,0xb000); /* 8003DD2C */
 ff_w16(0x8009ea34,0xb); /* 8003DD34 */
 ff_w16(0x8009ea36,0x3f); /* 8003DD3C */
 ff_w16(0x8009eac8,0x1000); /* 8003DD40 */
 ff_w16(0x8009eace,0x32); /* 8003DD48 */
 ff_w16(0x8009ead0,0xfff7); /* 8003DD50 */
 ff_w16(0x8009ead2,0x1e); /* 8003DD58 */
 ff_w16(0x8009eaec,0x1000); /* 8003DD5C */
 ff_w16(0x8009eaf2,0x44); /* 8003DD60 */
 ff_w16(0x8009eaf4,0x0); /* 8003DD64 */
 ff_w16(0x8009eaf6,0x4); /* 8003DD68 */
 ff_w16(0x8009ead4,0x1000); /* 8003DD6C */
 ff_w16(0x8009eada,0x32); /* 8003DD70 */
 ff_w16(0x8009eadc,0xfff7); /* 8003DD74 */
 ff_w16(0x8009eade,0x1e); /* 8003DD78 */
 ff_w16(0x8009eae0,0x1000); /* 8003DD7C */
 ff_w16(0x8009eae6,0x32); /* 8003DD80 */
 ff_w16(0x8009eae8,0xfff7); /* 8003DD84 */
 ff_w16(0x8009eaea,0x1e); /* 8003DD88 */
 ff_w16(0x8009eb10,0x1800); /* 8003DD90 */
 ff_w16(0x8009eb04,0x1800); /* 8003DD94 */
 ff_w16(0x8009eb14,0xf800); /* 8003DD9C */
 ff_w16(0x8009eb08,0xf800); /* 8003DDA0 */
 ff_w16(0x8009eb16,0x2d); /* 8003DDA8 */
 ff_w16(0x8009eb0a,0x2d); /* 8003DDAC */
 ff_w16(0x8009eb1a,0x43); /* 8003DDB4 */
 ff_w16(0x8009eb0e,0x43); /* 8003DDB8 */
 ff_w16(0x8009e92e,0x20); /* 8003DDBC */
 ff_w16(0x8009ebf4,0x4800); /* 8003DDC0 */
 ff_w16(0x8009eb7c,0x4800); /* 8003DDC4 */
 ff_w16(0x8009eb88,0x4800); /* 8003DDC8 */
 ff_w16(0x8009ebfa,0x30); /* 8003DDCC */
 ff_w16(0x8009eb82,0x30); /* 8003DDD0 */
 ff_w16(0x8009eb8e,0x30); /* 8003DDD4 */
 ff_w16(0x8009ebfc,0xffc0); /* 8003DDD8 */
 ff_w16(0x8009eb84,0xffc0); /* 8003DDDC */
 ff_w16(0x8009eb90,0xffc0); /* 8003DDE0 */
 ff_w16(0x8009ebfe,0x10); /* 8003DDE4 */
 ff_w16(0x8009eb86,0x10); /* 8003DDE8 */
 ff_w16(0x8009eb92,0x10); /* 8003DDEC */
 ff_w16(0x8009e948,0x4000); /* 8003DDF0 */
 ff_w16(0x8009e94e,0x10); /* 8003DDF4 */
 ff_w16(0x8009e950,0xfffc); /* 8003DDF8 */
 ff_w16(0x8009e954,0x4000); /* 8003DDFC */
 ff_w16(0x8009e95a,0x10); /* 8003DE00 */
 ff_w16(0x8009e95c,0xfffc); /* 8003DE04 */
 ff_w16(0x8009eb58,0x8000); /* 8003DE08 */
 ff_w16(0x8009eb5a,0xf800); /* 8003DE0C */
 ff_w16(0x8009eb5c,0x0); /* 8003DE10 */
 ff_w16(0x8009eb5e,0x20); /* 8003DE14 */
 ff_w16(0x8009eb60,0x0); /* 8003DE18 */
 ff_w16(0x8009eb62,0x0); /* 8003DE1C */
 ff_w16(0x8009ec26,0xfe00); /* 8003DE24 */
 ff_w16(0x8009ec2a,0x2c); /* 8003DE2C */
 ff_w16(0x8009ec2c,0xfffa); /* 8003DE34 */
 ff_w16(0x8009ec2e,0x8); /* 8003DE38 */
 ff_w16(0x8009ec3e,0xf600); /* 8003DE40 */
 ff_w16(0x8009ec40,0xf000); /* 8003DE48 */
 ff_w16(0x8009ec42,0x2d); /* 8003DE4C */
 ff_w16(0x8009ec44,0xffdb); /* 8003DE54 */
 ff_w16(0x8009ec4a,0xf800); /* 8003DE58 */
 ff_w16(0x8009ec4e,0x30); /* 8003DE5C */
 ff_w16(0x8009ec50,0xffec); /* 8003DE64 */
 ff_w16(0x8009ebb8,0x4000); /* 8003DE68 */
 ff_w16(0x8009ebac,0x4000); /* 8003DE6C */
 ff_w16(0x8009eba0,0x4000); /* 8003DE70 */
 ff_w16(0x8009eb94,0x4000); /* 8003DE74 */
 ff_w16(0x8009ec60,0x4000); /* 8003DE78 */
 ff_w16(0x8009eaf8,0x4000); /* 8003DE7C */
 ff_w16(0x8009ebbe,0x30); /* 8003DE80 */
 ff_w16(0x8009ebb2,0x30); /* 8003DE84 */
 ff_w16(0x8009eba6,0x30); /* 8003DE88 */
 ff_w16(0x8009eb9a,0x30); /* 8003DE8C */
 ff_w16(0x8009ec66,0x30); /* 8003DE90 */
 ff_w16(0x8009eafe,0x30); /* 8003DE94 */
 ff_w16(0x8009ebc0,0xfff2); /* 8003DE9C */
 ff_w16(0x8009ebb4,0xfff2); /* 8003DEA0 */
 ff_w16(0x8009eba8,0xfff2); /* 8003DEA4 */
 ff_w16(0x8009eb9c,0xfff2); /* 8003DEA8 */
 ff_w16(0x8009ec68,0xfff2); /* 8003DEAC */
 ff_w16(0x8009eb00,0xfff2); /* 8003DEB0 */
 ff_w16(0x8009ebe0,0xf000); /* 8003DEB4 */
 ff_w16(0x8009ebd4,0xf000); /* 8003DEB8 */
 ff_w16(0x8009ebc8,0xf000); /* 8003DEBC */
 ff_w16(0x8009eb74,0xf000); /* 8003DEC0 */
 ff_w16(0x8009eb68,0xf000); /* 8003DEC4 */
 ff_w16(0x8009ec34,0xf000); /* 8003DEC8 */
 ff_w16(0x8009ebe2,0x34); /* 8003DED0 */
 ff_w16(0x8009ebd6,0x34); /* 8003DED4 */
 ff_w16(0x8009ebca,0x34); /* 8003DED8 */
 ff_w16(0x8009eb76,0x34); /* 8003DEDC */
 ff_w16(0x8009eb6a,0x34); /* 8003DEE0 */
 ff_w16(0x8009ec36,0x34); /* 8003DEE4 */
 ff_w16(0x8009ebe4,0xffd3); /* 8003DEEC */
 ff_w16(0x8009ebd8,0xffd3); /* 8003DEF0 */
 ff_w16(0x8009ebcc,0xffd3); /* 8003DEF4 */
 ff_w16(0x8009eb78,0xffd3); /* 8003DEF8 */
 ff_w16(0x8009eb6c,0xffd3); /* 8003DEFC */
 ff_w16(0x8009ec38,0xffd3); /* 8003DF00 */
 ff_w16(0x8009ebee,0x31); /* 8003DF08 */
 ff_w16(0x8009ebf0,0xffd4); /* 8003DF10 */
 ff_w16(0x8009ebf2,0x14); /* 8003DF14 */
 ff_w16(0x8009ec54,0xf000); /* 8003DF18 */
 ff_w16(0x8009ec56,0xc400); /* 8003DF20 */
 ff_w16(0x8009ec5c,0xff90); /* 8003DF28 */
 memset(ff_ptr(0x80094578,87),0,87); /* 8003DF38 */
 ff_w8(0x8009458c,0x0); /* 8003DF48 */
 ff_w8(0x8009458d,0x0); /* 8003DF4C */
 ff_w8(0x8009459c,0x63); /* 8003DF54 */
 ff_w8(0x8009459d,0x0); /* 8003DF58 */
 ff_w8(0x8009459e,0x63); /* 8003DF5C */
 ff_w8(0x80094599,0x4); /* 8003DF64 */
 ff_w8(0x80094598,0x4); /* 8003DF68 */
 ff_w8(0x80094597,0x4); /* 8003DF6C */
 ff_w8(0x80094596,0x4); /* 8003DF70 */
 ff_w8(0x8009459b,0x8); /* 8003DF78 */
 ff_w8(0x80094589,0x8); /* 8003DF7C */
 ff_w8(0x8009458a,0x8); /* 8003DF80 */
 ff_w8(0x80094587,0x8); /* 8003DF84 */
 ff_w8(0x80094586,0x8); /* 8003DF88 */
 ff_w8(0x80094585,0x8); /* 8003DF8C */
 ff_w8(0x80094584,0x8); /* 8003DF90 */
 ff_w8(0x80094583,0x8); /* 8003DF94 */
 ff_w8(0x80094582,0x8); /* 8003DF98 */
 ff_w8(0x8009457f,0x8); /* 8003DF9C */
 ff_w8(0x8009457e,0x8); /* 8003DFA0 */
 ff_w8(0x8009457d,0x8); /* 8003DFA4 */
 ff_w8(0x8009457c,0x8); /* 8003DFA8 */
 ff_w8(0x8009457b,0x8); /* 8003DFAC */
 ff_w8(0x8009457a,0x8); /* 8003DFB0 */
 ff_w8(0x8009458b,0x8); /* 8003DFB4 */
 ff_w8(0x80094581,0x8); /* 8003DFB8 */
 ff_w8(0x80094580,0x8); /* 8003DFBC */
 ff_w8(0x80094579,0x8); /* 8003DFC0 */
 ff_w8(0x800945a4,0x8); /* 8003DFC4 */
 ff_w8(0x80094588,0x5); /* 8003DFCC */
 ff_w8(0x80094593,0x5); /* 8003DFD0 */
 ff_w8(0x80094592,0x5); /* 8003DFD4 */
 ff_w8(0x80094591,0x5); /* 8003DFD8 */
 ff_w8(0x80094590,0x5); /* 8003DFDC */
 ff_w8(0x8009458f,0x5); /* 8003DFE0 */
 ff_w8(0x80094594,0x3); /* 8003DFE8 */
 ff_w8(0x80094595,0x1); /* 8003DFF0 */
 ff_w8(0x8009459f,0x4); /* 8003DFF4 */
 ff_w8(0x800945ce,0x2); /* 8003DFFC */
 ff_w8(0x800945cd,0x2); /* 8003E000 */
 ff_w8(0x800945cc,0x2); /* 8003E004 */
 ff_w8(0x800945cb,0x2); /* 8003E008 */
 ff_w8(0x800945ca,0x2); /* 8003E00C */
 ff_w8(0x800945c9,0x2); /* 8003E010 */
 ff_w8(0x800945c8,0x2); /* 8003E014 */
 ff_w8(0x800945c7,0x2); /* 8003E018 */
 ff_w8(0x800945c6,0x2); /* 8003E01C */
 ff_w8(0x800945c5,0x2); /* 8003E020 */
 ff_w8(0x800945c4,0x2); /* 8003E024 */
 ff_w8(0x800945c3,0x2); /* 8003E028 */
 ff_w8(0x800945c2,0x2); /* 8003E02C */
 ff_w8(0x800945c1,0x2); /* 8003E030 */
 ff_w8(0x800945c0,0x2); /* 8003E034 */
 ff_w8(0x800945bf,0x2); /* 8003E038 */
 ff_w8(0x800945be,0x2); /* 8003E03C */
 ff_w8(0x800945bd,0x2); /* 8003E040 */
 ff_w8(0x800945bc,0x2); /* 8003E044 */
 ff_w8(0x800945bb,0x2); /* 8003E048 */
 ff_w8(0x800945ba,0x2); /* 8003E04C */
 ff_w8(0x800945b9,0x2); /* 8003E050 */
 ff_w8(0x800945b8,0x2); /* 8003E054 */
 ff_w8(0x800945b7,0x2); /* 8003E058 */
 ff_w8(0x800945b6,0x2); /* 8003E05C */
 ff_w8(0x800945b5,0x2); /* 8003E060 */
 ff_w8(0x800945b4,0x2); /* 8003E064 */
 ff_w8(0x800945b3,0x2); /* 8003E068 */
 ff_w8(0x800945b2,0x2); /* 8003E06C */
 ff_w8(0x800945b1,0x2); /* 8003E070 */
 ff_w8(0x800945b0,0x2); /* 8003E074 */
 ff_w8(0x800945af,0x2); /* 8003E078 */
 ff_w8(0x800945ae,0x2); /* 8003E07C */
 ff_w8(0x800945ad,0x2); /* 8003E080 */
 ff_w8(0x800945ac,0x2); /* 8003E084 */
 ff_w8(0x800945ab,0x2); /* 8003E088 */
 ff_w8(0x800945aa,0x2); /* 8003E08C */
 ff_w8(0x800945a9,0x2); /* 8003E090 */
 ff_w8(0x800945a8,0x2); /* 8003E094 */
 ff_w8(0x800945a7,0x2); /* 8003E098 */
 ff_w8(0x800945a6,0x2); /* 8003E09C */
 ff_w32(0x80094040,0x8001f510); /* 8003E0B0 */
 ff_w32(0x80094048,0x8001f510); /* 8003E0B4 */
 ff_w32(0x8009404c,0x8001f510); /* 8003E0B8 */
 ff_w32(0x80094054,0x8001f518); /* 8003E0C4 */
 ff_w32(0x80094064,0x8001fa78); /* 8003E0D0 */
 ff_w32(0x80094068,0x8001ff08); /* 8003E0DC */
 ff_w32(0x8009406c,0x800205c4); /* 8003E0E8 */
 ff_w32(0x80094044,0x80021220); /* 8003E0F4 */
 ff_w32(0x80094078,0x800213f8); /* 8003E100 */
 ff_w32(0x80094070,0x80020c84); /* 8003E10C */
 ff_w32(0x80094074,0x80020a60); /* 8003E118 */
 ff_w32(0x8009407c,0x80021628); /* 8003E124 */
 ff_w32(0x80094080,0x80021910); /* 8003E130 */
 ff_w32(0x80094084,0x80021bf8); /* 8003E13C */
 ff_w32(0x80094088,0x8002225c); /* 8003E148 */
 ff_w32(0x8009408c,0x80022548); /* 8003E154 */
 ff_w32(0x80094058,0x8001f7ec); /* 8003E160 */
 ff_w32(0x8009405c,0x800229c0); /* 8003E16C */
 ff_w32(0x80094094,0x8001fda0); /* 8003E178 */
 ff_w32(0x80094060,0x80022b18); /* 8003E184 */
 ff_w32(0x80094098,0x80022c60); /* 8003E190 */
 ff_w32(0x8009409c,0x80022edc); /* 8003E19C */
 ff_w32(0x800940a0,0x80022ff0); /* 8003E1A8 */
 ff_w32(0x800940a4,0x8002322c); /* 8003E1B4 */
 ff_w8(0x80094488,0x0); /* 8003E1C0 */
 ff_w8(0x80094489,0x1); /* 8003E1C4 */
 ff_w8(0x8009448a,0x2); /* 8003E1C8 */
 ff_w8(0x8009448b,0x2); /* 8003E1CC */
 ff_w8(0x8009448c,0x2); /* 8003E1D0 */
 ff_w8(0x8009448d,0x2); /* 8003E1D4 */
 ff_w8(0x8009448e,0x0); /* 8003E1D8 */
 ff_w8(0x8009448f,0x0); /* 8003E1DC */
 ff_w8(0x80094490,0x0); /* 8003E1E0 */
 ff_w8(0x80094491,0x0); /* 8003E1E4 */
 ff_w8(0x80094492,0x0); /* 8003E1E8 */
 ff_w8(0x80094494,0x2); /* 8003E1EC */
 ff_w8(0x80094493,0x0); /* 8003E1F0 */
 ff_w8(0x80094495,0x0); /* 8003E1F4 */
 ff_w8(0x80094499,0x0); /* 8003E1F8 */
 ff_w8(0x800944a0,0x2); /* 8003E1FC */
 ff_w8(0x800944a1,0x0); /* 8003E200 */
 ff_w8(0x800944ac,0x2); /* 8003E204 */
 ff_w8(0x800944ae,0x2); /* 8003E208 */
 ff_w8(0x800944af,0x2); /* 8003E20C */
 ff_w8(0x800946d8,0x2); /* 8003E218 */
 ff_w8(0x800946d9,0xa); /* 8003E220 */
 ff_w8(0x800946da,0x0); /* 8003E224 */
 ff_w8(0x800946db,0x0); /* 8003E228 */
 ff_w8(0x800946dc,0x0); /* 8003E22C */
 ff_w8(0x800946dd,0x0); /* 8003E230 */
 ff_w8(0x800946de,0x0); /* 8003E234 */
 ff_w8(0x800946df,0x0); /* 8003E238 */
 ff_w8(0x800946e0,0x0); /* 8003E23C */
 ff_w8(0x800946e1,0x0); /* 8003E240 */
 ff_w8(0x800946e2,0x0); /* 8003E244 */
 ff_w8(0x800946e4,0x0); /* 8003E248 */
 ff_w8(0x800946e3,0x0); /* 8003E24C */
 ff_w8(0x800946e5,0x0); /* 8003E250 */
 ff_w8(0x800946e9,0x0); /* 8003E254 */
 ff_w8(0x800946f0,0x2); /* 8003E258 */
 ff_w8(0x800946f1,0x0); /* 8003E25C */
 ff_w8(0x800946fc,0x0); /* 8003E260 */
 ff_w8(0x800946fe,0x0); /* 8003E264 */
 ff_w8(0x800946ff,0x0); /* 8003E268 */
 ff_w32(0x8009cc98,0x0); /* 8003E274 */
 ff_w32(0x8009cc9c,0x0); /* 8003E278 */
 ff_w32(0x8009cca0,0x8003060c); /* 8003E284 */
 ff_w32(0x8009cca4,0x800306d4); /* 8003E290 */
 ff_w32(0x8009cca8,0x80030ad4); /* 8003E29C */
 ff_w32(0x8009ccac,0x80030b94); /* 8003E2A8 */
 ff_w32(0x8009ccb0,0x0); /* 8003E2AC */
 ff_w32(0x8009ccb4,0x80030d04); /* 8003E2B8 */
 ff_w32(0x8009ccb8,0x80030d60); /* 8003E2C4 */
 ff_w32(0x8009ccbc,0x80030c50); /* 8003E2D0 */
 ff_w32(0x8009ccc0,0x80030ca8); /* 8003E2DC */
 ff_w32(0x8009ccc8,0x800307ac); /* 8003E2E8 */
 ff_w32(0x8009ccc4,0x80030a14); /* 8003E2F4 */
 ff_w32(0x8009cccc,0x0); /* 8003E2F8 */
 ff_w32(0x8009ccdc,0x0); /* 8003E2FC */
 ff_w32(0x8009ccf8,0x0); /* 8003E300 */
 ff_w32(0x8009ccfc,0x80030dbc); /* 8003E30C */
 ff_w32(0x8009cd28,0x80047174); /* 8003E318 */
 ff_w32(0x8009cd30,0x80030874); /* 8003E324 */
 ff_w32(0x8009cd34,0x80030944); /* 8003E330 */
 ff_w16(0x800a7720,0xfe80); /* 8003E340 */
 ff_w16(0x800a7722,0xfec0); /* 8003E348 */
 ff_w16(0x800a771a,0xff40); /* 8003E350 */
 ff_w16(0x800a77be,0xff50); /* 8003E358 */
 ff_w16(0x800a7802,0xfe00); /* 8003E35C */
 ff_w16(0x800a7810,0xfe00); /* 8003E360 */
 ff_w16(0x800a7812,0xfe00); /* 8003E364 */
 ff_w16(0x800a7814,0xfe00); /* 8003E368 */
 ff_w16(0x800a7816,0xfe00); /* 8003E36C */
 ff_w16(0x800a74a8,0xfb80); /* 8003E374 */
 ff_w16(0x800a74ca,0xfe80); /* 8003E378 */
 ff_w16(0x800a74cc,0xfe80); /* 8003E37C */
 ff_w16(0x800a74d0,0xfe80); /* 8003E380 */
 ff_w16(0x800a74d4,0xfe80); /* 8003E384 */
 ff_w16(0x800a74d8,0xfe80); /* 8003E388 */
 ff_w16(0x800a74d2,0xff40); /* 8003E38C */
 ff_w16(0x800a74d6,0xff40); /* 8003E390 */
 ff_w16(0x800a78cc,0xfe00); /* 8003E394 */
 ff_w16(0x800a78e8,0xfe40); /* 8003E39C */
 ff_w16(0x800a78ea,0xff50); /* 8003E3A0 */
 ff_w16(0x800a78d0,0xffd0); /* 8003E3A4 */
 ff_w16(0x800a7902,0xfed8); /* 8003E3AC */
 ff_w16(0x80094378,0xf868); /* 8003E3BC */
 ff_w16(0x800943c0,0xf1f0); /* 8003E3CC */
 ff_w16(0x800942e8,0x200); /* 8003E3DC */
 ff_w32(0x800b3e00,0x8007af14); /* 8003E3F0 */
 ff_w32(0x800a7dc8,0x8007b3fc); /* 8003E404 */
 ff_w32(0x800a86d8,0x8005e148); /* 8003E418 */
 ff_w32(0x8009cc08,0x800345a0); /* 8003E42C */
 ff_w32(0x800ba180,0x80034530); /* 8003E440 */
 ff_w32(0x800adbe0,0x80027df8); /* 8003E454 */
 ff_w16(0x8009437a,0x40); /* 8003E45C */
 ff_w16(0x800943c2,0x500); /* 8003E464 */
 ff_w16(0x800942ea,0x0); /* 8003E468 */
 ff_w32(0x800b3e04,0x8007af84); /* 8003E474 */
 ff_w32(0x800a7dcc,0x8007b62c); /* 8003E480 */
 ff_w32(0x800a86dc,0x8005e190); /* 8003E48C */
 ff_w32(0x8009cc0c,0x80034c80); /* 8003E498 */
 ff_w32(0x800ba184,0x80034c20); /* 8003E4A4 */
 ff_w32(0x800adbe4,0x80027fa4); /* 8003E4B0 */
 ff_w16(0x8009437c,0x0); /* 8003E4B4 */
 ff_w16(0x800943c4,0x0); /* 8003E4B8 */
 ff_w16(0x800942ec,0x0); /* 8003E4BC */
 ff_w32(0x800b3e08,0x8007afac); /* 8003E4C8 */
 ff_w32(0x800a7dd0,0x8007b8c4); /* 8003E4D4 */
 ff_w32(0x800a86e0,0x8005e1d8); /* 8003E4E0 */
 ff_w32(0x8009cc10,0x80035024); /* 8003E4EC */
 ff_w32(0x800ba188,0x80034fac); /* 8003E4F8 */
 ff_w32(0x800adbe8,0x8002804c); /* 8003E504 */
 ff_w16(0x8009437e,0x4); /* 8003E508 */
 ff_w16(0x800943c6,0xfb80); /* 8003E50C */
 ff_w16(0x800942ee,0x400); /* 8003E510 */
 ff_w32(0x800b3e0c,0x8007afc0); /* 8003E51C */
 ff_w32(0x800a7dd4,0x8007bb98); /* 8003E528 */
 ff_w32(0x800a86e4,0x8005e220); /* 8003E534 */
 ff_w32(0x8009cc14,0x800351f4); /* 8003E540 */
 ff_w32(0x800ba18c,0x80035184); /* 8003E54C */
 ff_w32(0x800adbec,0x800280b0); /* 8003E558 */
 ff_w16(0x80094380,0x8); /* 8003E55C */
 ff_w16(0x800943c8,0x900); /* 8003E564 */
 ff_w16(0x800942f0,0x400); /* 8003E568 */
 ff_w32(0x800b3e10,0x8007afd8); /* 8003E574 */
 ff_w32(0x800a7dd8,0x8007bcfc); /* 8003E580 */
 ff_w32(0x800a86e8,0x8005e250); /* 8003E58C */
 ff_w32(0x8009cc18,0x8003538c); /* 8003E598 */
 ff_w32(0x800ba190,0x8003532c); /* 8003E5A4 */
 ff_w32(0x800adbf0,0x80028114); /* 8003E5B0 */
 ff_w16(0x80094382,0xfb00); /* 8003E5B8 */
 ff_w16(0x800943ca,0xe300); /* 8003E5C0 */
 ff_w16(0x800942f2,0x400); /* 8003E5C4 */
 ff_w32(0x800b3e14,0x8007afe8); /* 8003E5D0 */
 ff_w32(0x800a7ddc,0x8007be70); /* 8003E5DC */
 ff_w32(0x800a86ec,0x8005e298); /* 8003E5E8 */
 ff_w32(0x8009cc1c,0x800355c0); /* 8003E5F4 */
 ff_w32(0x800ba194,0x80035528); /* 8003E600 */
 ff_w32(0x800adbf4,0x8002837c); /* 8003E60C */
 ff_w16(0x80094384,0xfba0); /* 8003E614 */
 ff_w16(0x800943cc,0x120); /* 8003E61C */
 ff_w16(0x800942f4,0x400); /* 8003E620 */
 ff_w32(0x800b3e18,0x8007b028); /* 8003E62C */
 ff_w32(0x800a7de0,0x8007c32c); /* 8003E638 */
 ff_w32(0x800a86f0,0x8005e2e0); /* 8003E644 */
 ff_w32(0x8009cc20,0x80035984); /* 8003E650 */
 ff_w32(0x800ba198,0x8003593c); /* 8003E65C */
 ff_w32(0x800adbf8,0x8002848c); /* 8003E668 */
 ff_w16(0x80094386,0xf00); /* 8003E670 */
 ff_w16(0x800943ce,0xe600); /* 8003E678 */
 ff_w16(0x800942f6,0x400); /* 8003E67C */
 ff_w32(0x800b3e1c,0x8007b078); /* 8003E688 */
 ff_w32(0x800a7de4,0x8007c354); /* 8003E694 */
 ff_w32(0x800a86f4,0x8005e328); /* 8003E6A0 */
 ff_w32(0x8009cc24,0x80035d50); /* 8003E6AC */
 ff_w32(0x800ba19c,0x80035cd8); /* 8003E6B8 */
 ff_w32(0x800adbfc,0x800284d4); /* 8003E6C4 */
 ff_w16(0x80094388,0xfa00); /* 8003E6CC */
 ff_w16(0x800943d0,0xef80); /* 8003E6D4 */
 ff_w16(0x800942f8,0x400); /* 8003E6D8 */
 ff_w32(0x800b3e20,0x8007b0f4); /* 8003E6E4 */
 ff_w32(0x800a7de8,0x8007c724); /* 8003E6F0 */
 ff_w32(0x800a86f8,0x8005e370); /* 8003E6FC */
 ff_w32(0x8009cc28,0x80036564); /* 8003E708 */
 ff_w32(0x800ba1a0,0x800364ac); /* 8003E714 */
 ff_w32(0x800adc00,0x80028610); /* 8003E720 */
 ff_w16(0x8009438a,0xfe00); /* 8003E724 */
 ff_w16(0x800943d2,0xef80); /* 8003E728 */
 ff_w16(0x800942fa,0x400); /* 8003E72C */
 ff_w32(0x800b3e24,0x8007b130); /* 8003E738 */
 ff_w32(0x800a7dec,0x8007c9c4); /* 8003E744 */
 ff_w32(0x800a86fc,0x8005e3b8); /* 8003E750 */
 ff_w32(0x8009cc2c,0x800368bc); /* 8003E75C */
 ff_w32(0x800ba1a4,0x80036804); /* 8003E768 */
 ff_w32(0x800adc04,0x80028658); /* 8003E774 */
 ff_w16(0x8009438c,0x0); /* 8003E778 */
 ff_w16(0x800943d4,0xe250); /* 8003E780 */
 ff_w16(0x800942fc,0x400); /* 8003E784 */
 ff_w32(0x800b3e28,0x8007b16c); /* 8003E790 */
 ff_w32(0x800a7df0,0x8007ce64); /* 8003E79C */
 ff_w32(0x800a8700,0x8005e400); /* 8003E7A8 */
 ff_w32(0x8009cc30,0x80036bcc); /* 8003E7B4 */
 ff_w32(0x800ba1a8,0x80036b5c); /* 8003E7C0 */
 ff_w32(0x800adc08,0x800286a0); /* 8003E7CC */
 ff_w16(0x8009438e,0xfc00); /* 8003E7D4 */
 ff_w16(0x800943d6,0xe200); /* 8003E7DC */
 ff_w16(0x800942fe,0x400); /* 8003E7E0 */
 ff_w32(0x800b3e2c,0x8007b19c); /* 8003E7EC */
 ff_w32(0x800a7df4,0x8007d16c); /* 8003E7F8 */
 ff_w32(0x800a8704,0x8005e448); /* 8003E804 */
 ff_w32(0x8009cc34,0x80036f44); /* 8003E810 */
 ff_w32(0x800ba1ac,0x80036edc); /* 8003E81C */
 ff_w32(0x800adc0c,0x80028704); /* 8003E828 */
 ff_w16(0x80094390,0x1400); /* 8003E830 */
 ff_w16(0x800943d8,0xfb50); /* 8003E838 */
 ff_w16(0x80094300,0x400); /* 8003E83C */
 ff_w32(0x800b3e30,0x8007b1f0); /* 8003E848 */
 ff_w32(0x800a7df8,0x8007d3c0); /* 8003E854 */
 ff_w32(0x800a8708,0x8005e490); /* 8003E860 */
 ff_w32(0x8009cc38,0x800374d0); /* 8003E86C */
 ff_w32(0x800ba1b0,0x80037460); /* 8003E878 */
 ff_w32(0x800adc10,0x80028874); /* 8003E884 */
 ff_w16(0x80094392,0x1400); /* 8003E888 */
 ff_w16(0x800943da,0x700); /* 8003E890 */
 ff_w16(0x80094302,0x400); /* 8003E894 */
 ff_w32(0x800b3e34,0x8007b1f0); /* 8003E898 */
 ff_w32(0x800a7dfc,0x8007d6b0); /* 8003E8A4 */
 ff_w32(0x800a870c,0x8005e490); /* 8003E8A8 */
 ff_w32(0x8009cc3c,0x800374d0); /* 8003E8AC */
 ff_w32(0x800ba1b4,0x800378c4); /* 8003E8B8 */
 ff_w32(0x800adc14,0x80028874); /* 8003E8BC */
 ff_w16(0x80094394,0x0); /* 8003E8C0 */
 ff_w16(0x800943dc,0xe230); /* 8003E8C8 */
 ff_w16(0x80094304,0x400); /* 8003E8CC */
 ff_w32(0x800b3e38,0x8007b238); /* 8003E8D8 */
 ff_w32(0x800a7e00,0x8007d6dc); /* 8003E8E4 */
 ff_w32(0x800a8710,0x8005e4d8); /* 8003E8F0 */
 ff_w32(0x8009cc40,0x8003799c); /* 8003E8FC */
 ff_w32(0x800ba1b8,0x8003790c); /* 8003E908 */
 ff_w32(0x800adc18,0x800289c0); /* 8003E914 */
 ff_w16(0x80094396,0x0); /* 8003E918 */
 ff_w16(0x800943de,0xe200); /* 8003E91C */
 ff_w16(0x80094306,0x400); /* 8003E920 */
 ff_w32(0x800b3e3c,0x8007b258); /* 8003E92C */
 ff_w32(0x800a7e04,0x8007d9a0); /* 8003E938 */
 ff_w32(0x800a8714,0x8005e520); /* 8003E944 */
 ff_w32(0x8009cc44,0x80038068); /* 8003E950 */
 ff_w32(0x800ba1bc,0x80037b28); /* 8003E95C */
 ff_w32(0x800adc1c,0x80028a94); /* 8003E968 */
 ff_w16(0x80094398,0x0); /* 8003E96C */
 ff_w16(0x800943e0,0xf000); /* 8003E970 */
 ff_w16(0x80094308,0x400); /* 8003E974 */
 ff_w32(0x800b3e40,0x8007b2a8); /* 8003E980 */
 ff_w32(0x800a7e08,0x8007dd14); /* 8003E98C */
 ff_w32(0x800a8718,0x8005e568); /* 8003E998 */
 ff_w32(0x8009cc48,0x80038120); /* 8003E9A4 */
 ff_w32(0x800ba1c0,0x800380b8); /* 8003E9B0 */
 ff_w32(0x800adc20,0x80028ba0); /* 8003E9BC */
 ff_w16(0x8009439a,0x0); /* 8003E9C0 */
 ff_w16(0x800943e2,0xe800); /* 8003E9C8 */
 ff_w16(0x8009430a,0x400); /* 8003E9CC */
 ff_w32(0x800b3e44,0x8007b2e4); /* 8003E9D8 */
 ff_w32(0x800a7e0c,0x8007e080); /* 8003E9E4 */
 ff_w32(0x800a871c,0x8005e5b0); /* 8003E9F0 */
 ff_w32(0x8009cc4c,0x8003847c); /* 8003E9FC */
 ff_w32(0x800ba1c4,0x800383fc); /* 8003EA08 */
 ff_w32(0x800adc24,0x80028d08); /* 8003EA14 */
 ff_w16(0x8009439c,0x0); /* 8003EA18 */
 ff_w16(0x800943e4,0x800); /* 8003EA20 */
 ff_w16(0x8009430c,0x400); /* 8003EA24 */
 ff_w32(0x800b3e48,0x8007b314); /* 8003EA30 */
 ff_w32(0x800a7e10,0x8007e4c0); /* 8003EA3C */
 ff_w32(0x800a8720,0x8005e5f8); /* 8003EA48 */
 ff_w32(0x8009cc50,0x80038850); /* 8003EA54 */
 ff_w32(0x800ba1c8,0x800387c8); /* 8003EA60 */
 ff_w32(0x800adc28,0x80028f30); /* 8003EA6C */
 ff_w16(0x8009439e,0x0); /* 8003EA70 */
 ff_w16(0x800943e6,0xf000); /* 8003EA74 */
 ff_w16(0x8009430e,0x400); /* 8003EA78 */
 ff_w32(0x800b3e4c,0x8007b340); /* 8003EA84 */
 ff_w32(0x800a7e14,0x8007e6c0); /* 8003EA90 */
 ff_w32(0x800a8724,0x8005e640); /* 8003EA9C */
 ff_w32(0x8009cc54,0x80038b0c); /* 8003EAA8 */
 ff_w32(0x800ba1cc,0x80038aac); /* 8003EAB4 */
 ff_w32(0x800adc2c,0x8002903c); /* 8003EAC0 */
 ff_w16(0x800943a0,0x18c0); /* 8003EAC8 */
 ff_w16(0x800943e8,0xfa80); /* 8003EAD0 */
 ff_w16(0x80094310,0x400); /* 8003EAD4 */
 ff_w32(0x800b3e50,0x8007b258); /* 8003EAD8 */
 ff_w32(0x800a7e18,0x8007efcc); /* 8003EAE4 */
 ff_w32(0x800a8728,0x8005e688); /* 8003EAF0 */
 ff_w32(0x8009cc58,0x80038c70); /* 8003EAFC */
 ff_w32(0x800ba1d0,0x80038c28); /* 8003EB08 */
 ff_w32(0x800adc30,0x80029090); /* 8003EB14 */
 ff_w16(0x800943a2,0x0); /* 8003EB18 */
 ff_w16(0x800943ea,0xf000); /* 8003EB1C */
 ff_w16(0x80094312,0x400); /* 8003EB20 */
 ff_w32(0x800b3e54,0x8007b2a8); /* 8003EB24 */
 ff_w32(0x800a7e1c,0x8007eff0); /* 8003EB30 */
 ff_w32(0x800a872c,0x8005e6d0); /* 8003EB3C */
 ff_w32(0x8009cc5c,0x80038d08); /* 8003EB48 */
 ff_w32(0x800ba1d4,0x80038cc0); /* 8003EB54 */
 ff_w32(0x800adc34,0x80029110); /* 8003EB60 */
 ff_w16(0x800943a4,0x0); /* 8003EB64 */
 ff_w16(0x800943ec,0xf000); /* 8003EB68 */
 ff_w16(0x80094314,0x400); /* 8003EB6C */
 ff_w32(0x800b3e58,0x8007b358); /* 8003EB78 */
 ff_w32(0x800a7e20,0x8007e8b0); /* 8003EB84 */
 ff_w32(0x800a8730,0x8005e718); /* 8003EB90 */
 ff_w32(0x8009cc60,0x80038fe4); /* 8003EB9C */
 ff_w32(0x800ba1d8,0x80038f6c); /* 8003EBA8 */
 ff_w32(0x800adc38,0x80029180); /* 8003EBB4 */
 ff_w16(0x800943a6,0x440); /* 8003EBBC */
 ff_w16(0x800943ee,0xfc00); /* 8003EBC0 */
 ff_w16(0x80094316,0x400); /* 8003EBC4 */
 ff_w32(0x800b3e5c,0x8007b38c); /* 8003EBD0 */
 ff_w32(0x800a7e24,0x8007ecd0); /* 8003EBDC */
 ff_w32(0x800a8734,0x8005e748); /* 8003EBE8 */
 ff_w32(0x8009cc64,0x8003937c); /* 8003EBF4 */
 ff_w32(0x800ba1dc,0x80039314); /* 8003EC00 */
 ff_w32(0x800adc3c,0x800291e8); /* 8003EC0C */
 ff_w16(0x800943a8,0x0); /* 8003EC10 */
 ff_w16(0x800943f0,0x0); /* 8003EC14 */
 ff_w16(0x80094318,0x400); /* 8003EC18 */
 ff_w32(0x800b3e60,0x8007b39c); /* 8003EC24 */
 ff_w32(0x800a7e28,0x8007ef84); /* 8003EC30 */
 ff_w32(0x800a8738,0x8005e790); /* 8003EC3C */
 ff_w32(0x8009cc68,0x800394e8); /* 8003EC48 */
 ff_w32(0x800ba1e0,0x800394a0); /* 8003EC54 */
 ff_w32(0x800adc40,0x8002926c); /* 8003EC60 */
 ff_w32(0x800b3e6c,0x8007af14); /* 8003EC64 */
 ff_w32(0x800a8744,0x8005e148); /* 8003EC68 */
 ff_w32(0x800b3e70,0x8007b3ac); /* 8003EC74 */
 ff_w32(0x800a8748,0x8005e808); /* 8003EC80 */
 ff_w32(0x800b3e74,0x8007b2a8); /* 8003EC84 */
 ff_w32(0x800a874c,0x8005e568); /* 8003EC88 */
 ff_w32(0x800b3e78,0x8007b258); /* 8003EC8C */
 ff_w32(0x800a8750,0x8005e520); /* 8003EC90 */
 ff_w32(0x800b3e7c,0x8007b3cc); /* 8003EC9C */
 ff_w32(0x800a8754,0x8005e8e0); /* 8003ECA8 */
 ff_w32(0x800b3e80,0x8007b3cc); /* 8003ECAC */
 ff_w32(0x800a8758,0x8005e8e0); /* 8003ECB0 */
 ff_w32(0x800b3e84,0x8007b3d0); /* 8003ECBC */
 ff_w32(0x800a875c,0x8005e928); /* 8003ECC8 */
 ff_w32(0x800b3e88,0x8007b3d8); /* 8003ECD4 */
 ff_w32(0x800a8760,0x8005e970); /* 8003ECE0 */
 return 0x8005e970;
}
/* END constant initializers */

GDB_CALL sint32 FUN_8006A108(uint32 first,uint32 second,uint32 third) {
 FF_FUNCTION_MARKER(0x8006a108u,"GAME.EXE");
 sint32 result;
 FUN_800678E8();FUN_8005703C();
 do {
  ff_game_prepare_80058634_stage0();FUN_8005C2A0();
  FUN_80061B7C(first,32,64);FUN_80061B7C(second,32,80);FUN_80061B7C(third,32,96);
  FUN_80058C14();result=FUN_8005F5AC((sint32)ff_u32(0x80094288))&8;
 } while(!result);
 return result;
}
