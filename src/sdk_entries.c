#include "ff.h"
#include "platform_dummy.h"
#include "ff_audio.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ff_gpu.h"
GDB_CALL sint32 FUN_SLUS_8001287C(uint32 filename,FF_SLUS_SERVICE service) {
 FF_FUNCTION_MARKER(0x8001287cu,"SLUS_004.33");
 sint16 result;
 service(0x80012bbc,0,0,0,0,0);service(0x8001772c,0,0,0,0,0);
 service(0x80013808,0x80030410,34,0x80030438,34,0);service(0x800138a4,0,0,0,0,0);
 service(0x80014658,0,0,0,0,0);service(0x800148f0,0,0,0,0,0);FUN_SLUS_800113F0();service(0x80014a7c,1,0,0,0,0);
 ff_w32(0x8003032c,filename);ff_w16(0x80030330,(uint16)ff_s16(0x800302f0));ff_w16(0x80030332,1);ff_w16(0x80030348,127);
 FUN_SLUS_8001268C(0x8003032c);result=(sint16)FUN_SLUS_8001152C(0x8003032c);
 service(0x80014a7c,0,0,0,0,0);VSync(0);DrawSync(0);VSync(0);service(0x800138d4,0,0,0,0,0);
 service(0x80014658,3,0,0,0,0);service(0x80012cb0,0,0,0,0,0);service(0x800136ac,0,0,0,0,0);return result;
}
static void slus_boot_display_cleanup(FF_SLUS_SERVICE service) {
 service(0x80014a7c,0,0,0,0,0);VSync(0);DrawSync(0);VSync(0);service(0x800138d4,0,0,0,0,0);
 service(0x80014658,3,0,0,0,0);service(0x80012cb0,0,0,0,0,0);
}
GDB_CALL uint32 FUN_SLUS_80010EE4(FF_SLUS_SERVICE service) {
 FF_FUNCTION_MARKER(0x80010ee4u,"SLUS_004.33");
 uint8 saved[28];uint32 a,b,result;
 service(0x80012bbc,0,0,0,0,0);service(0x80014658,0,0,0,0,0);service(0x80014658,3,0,0,0,0);service(0x80012cb0,0,0,0,0,0);
 ff_w32(0x80030460,638);
 if(FUN_SLUS_8001287C(0x800302fc,service)==1){slus_boot_display_cleanup(service);ff_w32(0x80030460,483);FUN_SLUS_8001287C(0x80030308,service);}
 slus_boot_display_cleanup(service);service(0x80014658,0,0,0,0,0);
 /* Internal 10EF4 is inside this IDA function, not a separate TODO entry. */
 memcpy(saved,ff_ptr(0x801fe700,28),28);a=ff_u32(0x800302f4);b=ff_u32(0x800302f8);ff_w32(0x801fe700,a);ff_w32(0x801fe704,b);
 ff_gpu_set_def_disp_env(0x801fe708,0,0,320,240);ff_w16(0x801fe712,8);ff_w16(0x801fe716,240);
 DrawSync(0);VSync(0);service(0x800153e4,0x801fe708,0,0,0,0);DrawSync(0);VSync(0);
 ff_gpu_load_image(0x801fe700,0x80085920);DrawSync(0);VSync(0);service(0x80014a7c,1,0,0,0,0);DrawSync(0);
 memcpy(ff_ptr(0x801fe700,28),saved,28);
 /* Last cleanup has no SetDispMask call in the MIPS. */
 VSync(0);DrawSync(0);VSync(0);service(0x800138d4,0,0,0,0,0);service(0x80014658,3,0,0,0,0);service(0x80012cb0,0,0,0,0,0);
 service(0x800136ac,0,0,0,0,0);FUN_SLUS_8001370C();result=service(0x8001369c,0x80030318,0x801ffff0,0,0,0);return result;
}
GDB_CALL void FUN_8006CD64(uint32 entry_ra,FF_GAME_BOOT_SERVICE service) {
 FF_FUNCTION_MARKER(0x8006cd64u,"GAME.EXE");
 FF_GAME_BOOT_STATE state;uint32 address,top;
 for(address=0x80093f40;address<0x800c0d68;address+=4)ff_w32(address,0);
 top=ff_u32(0x8006ce14);state.sp=top|0x80000000u;state.fp=state.sp;state.gp=0x8008d48c;state.ra=entry_ra;
 state.heap_begin=0x800c0d6c;state.heap_bytes=top-ff_u32(0x8008d484)-0xc0d68;
 ff_w32(0x80093fb0,entry_ra);service(0x8006ce20,&state);state.ra=ff_u32(0x80093fb0);service(0x80015c6c,&state);
 /* Main has no normal return path in the audited entry. */
 ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
}
static uint32 slus_unpack_bit(uint32 *source,uint32 *bits) {
 uint32 value=*bits>>31;
 if(*bits==0x80000000u){uint32 byte=*(uint8*)ff_ptr((*source)++,1);value=byte>>7;*bits=(byte<<25)|0x1000000;}
 else *bits<<=1;
 return value;
}
GDB_CALL uint32 FUN_SLUS_80010000(uint32 source,uint32 destination) {
 FF_FUNCTION_MARKER(0x80010000u,"SLUS_004.33");
 uint32 start=destination,bits,length,high,distance,back,count,byte,first,second;int i;
 source+=18;bits=(((uint32)*(uint8*)ff_ptr(source++,1)<<25)|0x1000000)<<1;
 for(;;){
  if(!slus_unpack_bit(&source,&bits)){byte=*(uint8*)ff_ptr(source++,1);ff_w8(destination++,(uint8)byte);continue;}
  length=2;high=0;
  if(!slus_unpack_bit(&source,&bits)){
   length=4|slus_unpack_bit(&source,&bits);
   if(slus_unpack_bit(&source,&bits)){
    length=((length-1)<<1)|slus_unpack_bit(&source,&bits);
    if(length==9){
     for(i=0;i<4;++i)high=(high<<1)|slus_unpack_bit(&source,&bits);
     count=(high+3)*4;while(count){byte=ff_u32(source);source+=4;ff_w32(destination,byte);destination+=4;count-=4;}continue;
    }
   }
  }else{
   if(!slus_unpack_bit(&source,&bits))goto low_distance;
   length=3;
   if(slus_unpack_bit(&source,&bits)){
    byte=*(uint8*)ff_ptr(source++,1);
    if(!byte){if(slus_unpack_bit(&source,&bits))continue;break;}
    length=byte+8;
   }
  }
  if(slus_unpack_bit(&source,&bits)){
   high=slus_unpack_bit(&source,&bits);
   if(slus_unpack_bit(&source,&bits)){
    high=(high<<1)|slus_unpack_bit(&source,&bits)|4;
    if(!slus_unpack_bit(&source,&bits))high=(high<<1)|slus_unpack_bit(&source,&bits);
   }else if(!high)high=2|slus_unpack_bit(&source,&bits);
   high<<=8;
  }
low_distance:
  distance=high|*(uint8*)ff_ptr(source++,1);
  {sint64 difference=(sint64)(sint32)destination-(sint64)(sint32)distance;
   if(difference>2147483647LL||difference<(-2147483647LL-1))ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
   back=(uint32)difference-1;}
  if(length&1){byte=*(uint8*)ff_ptr(back++,1);ff_w8(destination++,(uint8)byte);}
  count=(length>>1)*2;
  if(!distance){byte=*(uint8*)ff_ptr(back,1);do{ff_w8(destination++,(uint8)byte);ff_w8(destination++,(uint8)byte);count-=2;}while(count);}
  else do{first=*(uint8*)ff_ptr(back,1);second=*(uint8*)ff_ptr(back+1,1);ff_w8(destination,(uint8)first);ff_w8(destination+1,(uint8)second);destination+=2;back+=2;count-=2;}while(count);
 }
 {sint64 size=(sint64)(sint32)destination-(sint64)(sint32)start;if(size>2147483647LL||size<(-2147483647LL-1))ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);return (uint32)size;}
}
GDB_CALL sint32 FUN_SLUS_8001152C(uint32 config) {
 FF_FUNCTION_MARKER(0x8001152cu,"SLUS_004.33");
 uint8 saved[40];uint16 tries=0; sint16 result=0;uint32 frame,button=0,p,source,mode,words,destination;
 memcpy(saved,ff_ptr(0x801fe680,40),40);ff_w16(0x801fe6a0,0);
 if(!ff_dummy_cd_search(ff_ptr(0x801fe680,24),(const char*)ff_ptr(ff_u32(config),1))){result=3;goto done;}
 FUN_SLUS_80011A00(config);
 if(!(sint16)FUN_SLUS_80011FE0(0x801fe680)){FUN_SLUS_80011EC8();result=3;goto done;}
 while(!(frame=FUN_SLUS_800120DC()))if(++tries==5){FUN_SLUS_80011EC8();result=3;goto done;}
 tries=0;FUN_SLUS_800122A0(frame);
 while(!ff_s16(0x80030388)&&!result){
  mode=(uint16)FUN_SLUS_8001263C();source=ff_u32(0x80030394+4u*(uint32)(sint32)ff_s16(0x8003038e));
  (void)mode;ff_dummy_str_decode(ff_ptr(source,1),0); /* WIP DecDCTin. */
  words=FUN_SLUS_800124E0();destination=ff_u32(0x8003039c+4u*(uint32)(sint32)ff_s16(0x80030390));
  (void)words;(void)destination;++ff_dummy_calls[2]; /* WIP DecDCTout; no decoded pixels. */
  frame=FUN_SLUS_800120DC();if(frame)tries=0;else if(++tries==5)result=3;
  FUN_SLUS_800122A0(frame);if(!button)button=FUN_SLUS_800112AC();
  if(!ff_s16(0x801fe6a0)&&button)ff_w16(0x801fe6a0,(uint16)ff_s16(0x80030368));
  if(ff_s16(0x801fe6a0))result=(sint16)FUN_SLUS_80012414(0x801fe6a0);
  FUN_SLUS_80012334();DrawSync(0);VSync(0);p=0x800303a4+20u*(uint32)(sint32)ff_s16(0x8003038c);ff_gpu_display_offset(ff_s16(p),ff_s16(p+2));
 }
 FUN_SLUS_80011EC8();if(ff_s16(0x80030388)||button==2||!button)result=1;
done: memcpy(ff_ptr(0x801fe680,40),saved,40);return result;
}
GDB_CALL uint32 FUN_SLUS_8001268C(uint32 config) {
 FF_FUNCTION_MARKER(0x8001268cu,"SLUS_004.33");
 uint8 saved[24];uint16 tries=0;uint32 frame,width;memcpy(saved,ff_ptr(0x801fe640,24),24);
 if(!ff_dummy_cd_search(ff_ptr(0x801fe640,24),(const char*)ff_ptr(ff_u32(config),1))){memcpy(ff_ptr(0x801fe640,24),saved,24);return 3;}
 FUN_SLUS_80011A00(config);
 if(!(sint16)FUN_SLUS_80011FE0(0x801fe640)){FUN_SLUS_80011EC8();memcpy(ff_ptr(0x801fe640,24),saved,24);return 3;}
 while(!(frame=FUN_SLUS_800120DC()))if(++tries==5){FUN_SLUS_80011EC8();memcpy(ff_ptr(0x801fe640,24),saved,24);return 3;}
 FUN_SLUS_80011EC8();width=ff_u32(0x80030400);
 ff_w16(config+8,(sint32)width<257?256:((sint32)width<321?320:640));
 ff_w16(config+14,(uint16)ff_s16(0x80030400));ff_w16(config+16,(uint16)ff_s16(0x80030404));
 ff_w16(config+10,(uint16)(((sint32)(uint16)ff_s16(config+8)-(uint16)ff_s16(config+14))/2));
 ff_w16(config+12,(uint16)((240-(sint32)(uint16)ff_s16(config+16))/2));
 memcpy(ff_ptr(0x801fe640,24),saved,24);return 3;
}
GDB_CALL uint32 FUN_SLUS_80011A00(uint32 config) {
 FF_FUNCTION_MARKER(0x80011a00u,"SLUS_004.33");
 uint16 x,y,width,height,screen;uint32 p,bytes,ring,i;
 FUN_SLUS_8001133C();x=(uint16)ff_s16(config+10);y=(uint16)ff_s16(config+12);width=(uint16)ff_s16(config+14);height=(uint16)ff_s16(config+16);screen=(uint16)ff_s16(config+8);
 ff_w16(0x80030358,(uint16)ff_s16(config+4));ff_w16(0x8003035a,screen);ff_w16(0x8003035c,x);ff_w16(0x8003035e,y);ff_w16(0x80030360,width);ff_w16(0x80030362,height);
 ff_w32(0x80030364,ff_u32(config+20));ff_w16(0x80030368,(uint16)ff_s16(config+28));ff_w32(0x8003036c,0);
 for(i=0;i<5;++i)ff_w16(0x80030388+2*i,0);
 ff_gpu_set_def_disp_env(0x800303a4,0,240,(sint16)screen,240);ff_gpu_set_def_disp_env(0x800303b8,0,0,(sint16)screen,240);
 ff_w16(0x800303b2,240);ff_w16(0x800303c6,240);ff_w8(0x800303c9,ff_s16(config+4)==1);ff_w8(0x800303b5,ff_s16(config+4)==1);
 if(ff_s16(config+6)||ff_s16(0x80030358))FUN_SLUS_80011F54();
 VSync(0);p=0x800303a4+20u*(uint32)(sint32)ff_s16(0x8003038c);ff_gpu_display_offset(ff_s16(p),ff_s16(p+2));
 ff_w16(0x80030370,(uint16)FUN_SLUS_800125AC(x));ff_w16(0x80030372,y);ff_w16(0x80030374,(uint16)FUN_SLUS_800125AC(width));ff_w16(0x80030376,height);
 ff_w16(0x80030378,(uint16)FUN_SLUS_800125AC(x));ff_w16(0x8003037a,(uint16)(y+240));ff_w16(0x8003037c,(uint16)FUN_SLUS_800125AC(width));ff_w16(0x8003037e,height);
 p=0x80030370+8u*(uint32)(sint32)ff_s16(0x8003038c);ff_w32(0x80030380,ff_u32(p));ff_w32(0x80030384,ff_u32(p+4));ff_w16(0x80030384,(uint16)FUN_SLUS_800125AC(16));
 bytes=ff_u32(config+24);if(!bytes)bytes=0x20400;
 ff_w32(0x80030394,FUN_SLUS_80011374(bytes));ff_w32(0x80030398,FUN_SLUS_80011374(bytes));
 bytes=(uint32)(sint32)(sint16)height*(ff_s16(0x80030358)?48u:32u);
 ff_w32(0x8003039c,FUN_SLUS_80011374(bytes));ff_w32(0x800303a0,FUN_SLUS_80011374(bytes));
 ff_dummy_str_decode(NULL,0); /* WIP MDEC reset and output interrupt registration. */
 ff_w32(0x8001c1ac,0xffffff);
 ring=FUN_SLUS_80011374(0x20000);ff_w32(0x80034270,ring);ff_w32(0x80034274,64);
 ff_w32(0x8003425c,0);ff_w32(0x80034254,0);ff_w32(0x80034250,0);ff_w32(0x80032a38,0);
 for(i=0;i<64;++i)ff_w32(ring+32*i,0);
 ff_w32(0x80032a10,0);ff_w16(0x800329fc,0);ff_w32(0x800329f8,0);
 ff_w32(0x80034268,1);ff_w32(0x80032a30,0);ff_w32(0x80034264,0xffffffffu);
 ff_w32(0x80034260,0);ff_w32(0x80032a08,0);ff_w32(0x80032a00,(uint16)ff_s16(0x80030358)&1);ff_w32(0x80032a34,0);ff_w32(0x80032a2c,0);ff_w32(0x80032a0c,0);
 ff_w16(0x800303cc,(uint16)((sint16)ff_spu_common_state[2]/258));ff_w16(0x800303ce,(uint16)((sint16)ff_spu_common_state[3]/258));
 ff_spu_cd_volume_host(ff_s16(0x80030368),ff_s16(0x80030368));return 0;
}
GDB_CALL sint32 FUN_SLUS_800113F0(void) {
 FF_FUNCTION_MARKER(0x800113f0u,"SLUS_004.33");
 PSX_RECT rectangle;int i;
 DrawSync(0);VSync(0);rectangle.w=512;rectangle.h=256;
 for(i=0;i<4;++i){rectangle.x=(sint16)((i&1)*512);rectangle.y=(sint16)((i>>1)*256);ClearImage(&rectangle,0,0,0);}
 DrawSync(0);return VSync(0);
}
GDB_CALL uint32 FUN_SLUS_80011894(void) {
 FF_FUNCTION_MARKER(0x80011894u,"SLUS_004.33");
 uint32 offset,words,destination;
 if(ff_s16(0x80030358)&&ff_u32(0x80032a10)){++ff_dummy_calls[2];/* WIP StCdInterrupt: no new sectors. */ff_w32(0x80032a10,0);}
 offset=(uint32)(sint32)ff_s16(0x80030390)<<2;
 ff_gpu_load_image(0x80030380,ff_u32(0x8003039c+offset));
 ff_w16(0x80030390,(uint16)ff_s16(0x80030390)^1);
 ff_w16(0x80030380,(uint16)((uint16)ff_s16(0x80030380)+(uint16)ff_s16(0x80030384)));
 offset=(uint32)(sint32)ff_s16(0x8003038c)<<3;
 if(ff_s16(0x80030380)>=ff_s16(0x80030370+offset)+ff_s16(0x80030374+offset)){ff_w16(0x8003038a,1);return 1;}
 words=FUN_SLUS_800124E0();destination=ff_u32(0x8003039c+((uint32)(sint32)ff_s16(0x80030390)<<2));
 (void)words;(void)destination;++ff_dummy_calls[2]; /* WIP DecDCTout: no fabricated decoded pixels or IRQ. */
 return ff_u32(0x8001f26c); /* SKIP1BEA4 returns its DMA CHCR pointer. */
}
GDB_CALL uint32 FUN_SLUS_80013AE0(uint32 entry_v0,FF_SDK_INVOKE invoke,void *arguments) {
 FF_FUNCTION_MARKER(0x80013ae0u,"SLUS_004.33");
 return invoke(ff_u32(0x800303dc),entry_v0,arguments);
}
GDB_CALL uint32 FUN_8006DF34(uint32 entry_v1,FF_SDK_INVOKE invoke,void *arguments) {
 FF_FUNCTION_MARKER(0x8006df34u,"GAME.EXE");
 uint32 target;
 if((entry_v1&0x1fffffffu)==0x1f800000){++ff_dummy_calls[0];return 0;} /* WIP physical card/SIO IRQ hook. */
 if(!(ff_u32(entry_v1+0x1074)&0x80))return 0;
 while(ff_u32(entry_v1+0x1044)&0x80){}
 target=ff_u32(0x80093fcc);return invoke(target,target,arguments);
}
GDB_CALL uint32 FUN_SLUS_800103F0(uint8 *development_profile_bank) {
 FF_FUNCTION_MARKER(0x800103f0u,"SLUS_004.33");
 uint32 tcb=ff_u32(0x110),pc=(ff_u32(tcb+0x88)|0xe0000000u)^0xe0000000u,address,value;
 pc+=(ff_u32(tcb+0x98)>>31)<<2;address=pc<0x200000?pc+0x200000:0x200000;
 /* The profiling build expects a writable development-RAM bank at2M.
  * Supply that bank explicitly; do not alias host code or infer ff_ram mirrors. */
 memcpy(&value,development_profile_bank+(address-0x200000),4);++value;memcpy(development_profile_bank+(address-0x200000),&value,4);return address;
}
GDB_CALL sint32 FUN_SLUS_80034278(void) {
 FF_FUNCTION_MARKER(0x80034278u,"SLUS_004.33");
 uint8 saved[3076];uint32 source=0x800b4530,destination=0x801fda04,group,row,column,v,p;
 memcpy(saved,ff_ptr(0x801fda00,3076),3076);ff_w32(0x800ac52c,0x800ac530);
 for(group=0;group<3;++group){for(row=0;row<8;++row){for(column=0;column<32;++column){
  v=*(uint8*)ff_ptr(source,1);source+=8;
  p=(v>>7)|((v&1)<<28)|((v&2)<<23)|((v&4)<<18)|((v&8)<<13)|((v&16)<<8)|((v&32)<<3)|((v&64)>>2);
  ff_w32(destination,p);destination+=4;
 }source-=255;}source+=248;}
 ff_gpu_load_image(0x800ac520,0x801fda04);DrawSync(0);ff_w16(0x800ac528,31);
 ff_w32(0x801fda00,0xffff0000);ff_gpu_load_rect(0x801fda00,640,257,16,1);ff_w16(0x800ac52a,(uint16)((257<<6)|40));
 DrawSync(0);memcpy(ff_ptr(0x801fda00,3076),saved,3076);return 0;
}
static uint32 sdk_clear_ot(uint32 table,uint32 count,uint32 terminal,int reverse) {
 uint32 p=table;
 if(reverse){while(count>1){p=table+4*(--count);ff_w32(p,(p-4)&0xffffff);}ff_w32(table,terminal&0xffffff);return table;}
 --count;while(count){--count;ff_w8(p+3,0);ff_w32(p,(ff_u32(p)&0xff000000u)|((p+4)&0xffffff));p+=4;}
 ff_w32(p,terminal&0xffffff);return p;
}
GDB_CALL uint32 FUN_8006F60C(uint32 table,uint32 count,uint32 entry_v0) {
 FF_FUNCTION_MARKER(0x8006f60cu,"GAME.EXE");
 if(entry_v0>=2)fprintf(stderr,"ClearOTag(%08x,%d)...\n",table,(sint32)count);
 return sdk_clear_ot(table,count,0x800873a8,0);
}
GDB_CALL uint32 FUN_8006F6C4(uint32 table,uint32 count,uint32 entry_v0) {
 FF_FUNCTION_MARKER(0x8006f6c4u,"GAME.EXE");
 if(entry_v0>=2)fprintf(stderr,"ClearOTagR(%08x,%d)...\n",table,(sint32)count);
 return sdk_clear_ot(table,count,0x800873a8,1);
}
GDB_CALL uint32 FUN_SLUS_80014F70(uint32 table,uint32 count,uint32 entry_v0) {
 FF_FUNCTION_MARKER(0x80014f70u,"SLUS_004.33");
 if(entry_v0>=2)fprintf(stderr,"ClearOTag(%08x,%d)...\n",table,(sint32)count);
 return sdk_clear_ot(table,count,0x8001ec28,0);
}
GDB_CALL uint32 FUN_SLUS_80015028(uint32 table,uint32 count,uint32 entry_v0) {
 FF_FUNCTION_MARKER(0x80015028u,"SLUS_004.33");
 if(entry_v0>=2)fprintf(stderr,"ClearOTagR(%08x,%d)...\n",table,(sint32)count);
 return sdk_clear_ot(table,count,0x8001ec28,1);
}
/* Native GPU command/queue boundaries reuse the synchronous rasterizer.
 * Preserve SDK byte shadows; custom replacement driver tables remain WIP. */
static void sdk_gpu_control(uint32 command,int slus) {
 uint32 address=ff_u32(slus?0x8001ec40:0x800873c0);
 if((address&0x1fffffffu)==0x1f801814){if((command>>24)==8)ff_gpu_set_reverse(command&128);}
 else ff_w32(address,command);
 ff_w8((slus?0x800304bc:0x80094ed4)+(command>>24),(uint8)command);
}
static uint32 sdk_graph_reverse(uint32 reverse,uint32 debug,int slus) {
 uint32 flags=slus?0x8001eb6c:0x800872ec,shadow=slus?0x800304bc:0x80094ed4,old=*(uint8*)ff_ptr(flags+3,1),command;
 if(debug>=2)fprintf(stderr,"SetGraphReverse(%d)...\n",(sint32)reverse);
 ff_w8(flags+3,(uint8)reverse);command=*(uint8*)ff_ptr(shadow+8,1);
 command|=*(uint8*)ff_ptr(flags+3,1)?0x08000080:0x08000000;sdk_gpu_control(command,slus);
 if(*(uint8*)ff_ptr(flags,1)==2)sdk_gpu_control(*(uint8*)ff_ptr(flags+3,1)?0x20000501:0x20000504,slus);
 return old;
}
GDB_CALL uint32 FUN_8006EE78(uint32 reverse,uint32 entry_v0) {
 FF_FUNCTION_MARKER(0x8006ee78u,"GAME.EXE");
 return sdk_graph_reverse(reverse,entry_v0,0);
}
GDB_CALL uint32 FUN_SLUS_800147E4(uint32 reverse,uint32 entry_v0) {
 FF_FUNCTION_MARKER(0x800147e4u,"SLUS_004.33");
 return sdk_graph_reverse(reverse,entry_v0,1);
}
static uint32 sdk_graph_queue(uint32 mode,uint32 debug,int slus) {
 uint32 flag=slus?0x8001eb6d:0x800872ed,old=*(uint8*)ff_ptr(flag,1),queue=slus?0x8001ec70:0x800873f0,callback=slus?0x8001eaf0:0x8008cafc;
 uint32 address,value,gpu=slus?0x8001ec40:0x800873c0;
 if(debug>=2)fprintf(stderr,"SetGrapQue(%d)...\n",(sint32)mode);
 if(mode!=*(uint8*)ff_ptr(flag,1)){
  address=ff_u32(slus?0x8001eab0:0x8008cabc);value=(address&0x1fffffffu)==0x1f801074?ff_sdk_counter_state[1]&65535:(uint16)ff_s16(address);
  ff_w32(queue+4,0);ff_w32(queue,0);ff_w32(queue+16,value);
  address=ff_u32(gpu+12);if((address&0x1fffffffu)!=0x1f8010a8)ff_w32(address,0x401);
  address=ff_u32(gpu+28);if((address&0x1fffffffu)!=0x1f8010f0)ff_w32(address,ff_u32(address)|0x800);
  address=ff_u32(gpu);if((address&0x1fffffffu)!=0x1f801814){ff_w32(address,0x02000000);ff_w32(ff_u32(gpu),0x01000000);}
  /* Reset(1) clears GPU DMA/command queue, not texture memory. */
  ff_w8(flag,(uint8)mode);
  if(ff_u32(callback)){ff_w32(callback,0);address=ff_u32(slus?0x8001eae4:0x8008caf0);
   if((address&0x1fffffffu)!=0x1f8010f4)ff_w32(address,((ff_u32(address)&0xffffff)|0x800000)&~0x40000u);}
 }
 return old;
}
GDB_CALL uint32 FUN_8006EFF8(uint32 mode,uint32 entry_v0) {
 FF_FUNCTION_MARKER(0x8006eff8u,"GAME.EXE");
 return sdk_graph_queue(mode,entry_v0,0);
}
GDB_CALL uint32 FUN_SLUS_8001495C(uint32 mode,uint32 entry_v0) {
 FF_FUNCTION_MARKER(0x8001495cu,"SLUS_004.33");
 return sdk_graph_queue(mode,entry_v0,1);
}
GDB_CALL sint32 FUN_SLUS_80015120(uint32 table,uint32 entry_v0) {
 FF_FUNCTION_MARKER(0x80015120u,"SLUS_004.33");
 if(entry_v0>=2)fprintf(stderr,"DrawOTag(%08x)...\n",table);
 return ff_gpu_ot(table)<0?-1:0; /* Immediate handler returns0, not packet count. */
}
GDB_CALL uint32 FUN_SLUS_80014A28(uint32 callback,uint32 entry_v0) {
 FF_FUNCTION_MARKER(0x80014a28u,"SLUS_004.33");
 uint32 old;if(entry_v0>=2)fprintf(stderr,"DrawSyncCallback(%08x)...\n",callback);
 old=ff_u32(0x8001eb78);ff_w32(0x8001eb78,callback);return old;
}
static uint32 sdk_table_call(uint32 entry_v0,uint32 offset,FF_SDK_INVOKE invoke,void *arguments) {
 uint32 target=ff_u32(entry_v0+offset);return invoke(target,target,arguments);
}
GDB_CALL uint32 FUN_80075CE0(uint32 entry_v0,FF_SDK_INVOKE invoke,void *arguments) {
 FF_FUNCTION_MARKER(0x80075ce0u,"GAME.EXE");
 return sdk_table_call(entry_v0,20,invoke,arguments);
}
GDB_CALL uint32 FUN_80075D10(uint32 entry_v0,FF_SDK_INVOKE invoke,void *arguments) {
 FF_FUNCTION_MARKER(0x80075d10u,"GAME.EXE");
 return sdk_table_call(entry_v0,16,invoke,arguments);
}
GDB_CALL uint32 FUN_80075D40(uint32 entry_v0,FF_SDK_INVOKE invoke,void *arguments) {
 FF_FUNCTION_MARKER(0x80075d40u,"GAME.EXE");
 return sdk_table_call(entry_v0,24,invoke,arguments);
}
GDB_CALL uint32 FUN_8006FEF0(uint32 entry_v0,FF_SDK_INVOKE invoke,void *arguments) {
 FF_FUNCTION_MARKER(0x8006fef0u,"GAME.EXE");
 return sdk_table_call(entry_v0,56,invoke,arguments)>>31;
}
GDB_CALL uint32 FUN_SLUS_80012C88(uint32 entry_v0,FF_SDK_INVOKE invoke,void *arguments) {
 FF_FUNCTION_MARKER(0x80012c88u,"SLUS_004.33");
 return sdk_table_call(entry_v0,20,invoke,arguments);
}
GDB_CALL uint32 FUN_SLUS_80012CE8(uint32 entry_v0,FF_SDK_INVOKE invoke,void *arguments) {
 FF_FUNCTION_MARKER(0x80012ce8u,"SLUS_004.33");
 return sdk_table_call(entry_v0,24,invoke,arguments);
}
GDB_CALL uint32 FUN_SLUS_800158CC(uint32 entry_v0,FF_SDK_INVOKE invoke,void *arguments) {
 FF_FUNCTION_MARKER(0x800158ccu,"SLUS_004.33");
 return sdk_table_call(entry_v0,56,invoke,arguments)>>31;
}
GDB_CALL uint32 FUN_SLUS_80013B74(uint32 entry_t1,uint32 entry_v0,FF_SDK_INVOKE invoke,void *arguments) {
 FF_FUNCTION_MARKER(0x80013b74u,"SLUS_004.33");
 return invoke(entry_t1,entry_v0,arguments);
}
GDB_CALL uint32 FUN_SLUS_80017DB0(uint32 mix) {
 FF_FUNCTION_MARKER(0x80017db0u,"SLUS_004.33");
 uint32 i; /* WIP CD mixer register bank2/bank3 writes, SKIP18EC8. */
 for(i=0;i<4;++i)ff_dummy_cd_mix_state[i]=*(uint8*)ff_ptr(mix+i,1);
 ++ff_dummy_calls[1];return 1;
}
static sint32 sdk_slus_cd_sector_dummy(uint32 destination,uint32 words,int asynchronous) {
 /* WIP SKIP194CC/195CC: CD DMA variants share no-media failure. The original
  * BCR is words|10000; no wait on unavailable hardware and no invented bytes. */
 (void)asynchronous;return ff_dummy_cd_read(0,ff_ptr(destination,0),(words&65535u)*4);
}
GDB_CALL uint32 FUN_SLUS_80017DD0(uint32 destination,uint32 words) {
 FF_FUNCTION_MARKER(0x80017dd0u,"SLUS_004.33");
 return (uint32)sdk_slus_cd_sector_dummy(destination,words,0)==0;
}
GDB_CALL uint32 FUN_SLUS_80017DF0(uint32 destination,uint32 words) {
 FF_FUNCTION_MARKER(0x80017df0u,"SLUS_004.33");
 return (uint32)sdk_slus_cd_sector_dummy(destination,words,1)==0;
}
/* Host root-counter boundary: scanlines at completed VSyncs. This is a
 * deterministic frame-clock substitute, not cycle-accurate profiling. Keep
 * interrupt mask and reset epoch separate: Start/StopRCnt only mask IRQs. */
uint32 ff_sdk_counter_state[3]; /* reset VSync epoch, I_MASK, event enabled */
static uint32 sdk_counter_read(void) {
 uint32 address=ff_u32(0x80087274)+16;
 if((address&0x1fffffffu)!=0x1f801110)return (uint16)ff_s16(address);
 return (uint16)(((uint32)VSync(-1)-ff_sdk_counter_state[0])*(ff_u32(0x8008cb18)?312u:263u));
}
static void sdk_counter_mask(int enable) {
 uint32 address=ff_u32(0x80087270)+4,mask=ff_u32(0x8008727c),value;
 if((address&0x1fffffffu)==0x1f801074){value=ff_sdk_counter_state[1];ff_sdk_counter_state[1]=enable?value|mask:value&~mask;}
 else {value=ff_u32(address);ff_w32(address,enable?value|mask:value&~mask);}
}
GDB_CALL sint32 FUN_8006CAE0(void) {
 FF_FUNCTION_MARKER(0x8006cae0u,"GAME.EXE");
 uint32 address=ff_u32(0x80087274)+16;
 if((address&0x1fffffffu)==0x1f801110)ff_sdk_counter_state[0]=(uint32)VSync(-1);else ff_w16(address,0);
 sdk_counter_mask(1);return 1;
}
GDB_CALL sint32 FUN_8006CB10(void) {
 FF_FUNCTION_MARKER(0x8006cb10u,"GAME.EXE");
 sint32 value;sdk_counter_mask(0);value=(sint32)sdk_counter_read()>>((uint32)(sint32)ff_s16(0x80093fa4)&31);
 ff_w16(0x80093fa8,(uint16)value);return (sint16)value;
}
GDB_CALL sint32 FUN_8006CB60(void) {
 FF_FUNCTION_MARKER(0x8006cb60u,"GAME.EXE");
 sint32 value=(sint32)sdk_counter_read()>>((uint32)(sint32)ff_s16(0x80093fa4)&31);
 ff_w16(0x80093fa8,(uint16)value);return (sint16)value;
}
GDB_CALL sint32 FUN_8006C748(void) {
 FF_FUNCTION_MARKER(0x8006c748u,"GAME.EXE");
 sint32 value=(sint32)sdk_counter_read()>>((uint32)(sint32)ff_s16(0x80093fa4)&31);
 ff_w16(0x80093fac,(uint16)value);return value;
}
GDB_CALL sint32 FUN_8006C784(uint32 mode) {
 FF_FUNCTION_MARKER(0x8006c784u,"GAME.EXE");
 uint32 table=0x80087264+(mode<<2),i=0,p,q,x,address,j; sint32 spacing;
 ff_w16(0x80093fa6,*(uint8*)ff_ptr(table+3,1));ff_w16(0x80093fa4,*(uint8*)ff_ptr(table+2,1));spacing=ff_s16(table);
 FUN_8006CE80();
 /* Native timer event, separate from the dummy memory-card event namespace. */
 ff_w32(0x80093fa0,0xf1000100);ff_sdk_counter_state[2]=1;
 address=ff_u32(0x80087274)+16;
 if((address&0x1fffffffu)==0x1f801110)ff_sdk_counter_state[0]=(uint32)VSync(-1);
 else {ff_w16(address+4,0);ff_w16(address+8,64000);ff_w16(address+4,0x158);ff_w16(address,0);}
 ff_w32(0x800872f8,0x8006c748);FUN_8006CE90();
 if(ff_s16(0x80093fa6)>0)do {
  p=0x80094d20+20*(i&255);q=0x80094dd4+20*(i&255);x=(i&255)*(uint32)spacing;++i;
  ff_w8(p+3,4);ff_w8(p+7,0x20);ff_w8(p+4,50);ff_w8(p+5,50);ff_w16(p+10,20);ff_w16(p+14,20);ff_w16(p+18,30);ff_w8(p+6,150);
  ff_w16(p+8,(uint16)(x+21));ff_w16(p+12,(uint16)(x+29));ff_w16(p+16,(uint16)(x+25));
  ff_w8(q+3,4);ff_w8(q+7,0x20);ff_w8(q+4,50);ff_w8(q+5,50);ff_w8(q+6,150);ff_w16(q+18,45);
  ff_w16(q+8,(uint16)(x+21));ff_w16(q+10,55);ff_w16(q+12,(uint16)(x+29));ff_w16(q+14,55);ff_w16(q+16,(uint16)(x+25));
 }while((sint32)(i&255)<ff_s16(0x80093fa6));
 for(i=0;i<2;++i){p=0x80094cd8+36*i;ff_w8(p+3,8);ff_w8(p+7,0x38);
  for(j=0;j<4;++j){q=p+8*j;ff_w8(q+4,j&1?200:0);ff_w8(q+5,j&1?0:200);ff_w8(q+6,0);
   ff_w16(q+8,j&1?100:25);ff_w16(q+10,(uint16)(25+15*i+(j>=2?10:0)));}}
 return 50;
}
GDB_CALL sint32 FUN_8006CD04(void) {
 FF_FUNCTION_MARKER(0x8006cd04u,"GAME.EXE");
 VSync(0);FUN_8006C784(0);FUN_8006CAE0();VSync(0);return (sint16)FUN_8006CB10()<287?1:2;
}
/* PCdrv BREAK services own a separate handle namespace; guest handles must
 * never close a CRT descriptor belonging to the port. Files use the bin cwd. */
static FILE *sdk_pc_files[16];
static void sdk_pc_cleanup(void) {
 uint32 i;for(i=0;i<16;++i)if(sdk_pc_files[i]){fclose(sdk_pc_files[i]);sdk_pc_files[i]=NULL;}
}
sint32 ff_pc_create(uint32 name) {
 char path[128];uint32 i,n;static int registered;
 for(n=0;n<sizeof(path)-1;++n){path[n]=(char)*(uint8*)ff_ptr(name+n,1);if(!path[n])break;
  if(path[n]=='/'||path[n]=='\\'||path[n]==':')return -1;}
 if(n==sizeof(path)-1||!n||!strcmp(path,".")||!strcmp(path,".."))return -1;
 for(i=0;i<16;++i)if(!sdk_pc_files[i])break;if(i==16)return -1;
 if(!registered){if(atexit(sdk_pc_cleanup))return -1;registered=1;}
 sdk_pc_files[i]=fopen(path,"wb");return sdk_pc_files[i]?(sint32)i+1:-1;
}
GDB_CALL sint32 FUN_80079FC4(uint32 handle) {
 FF_FUNCTION_MARKER(0x80079fc4u,"GAME.EXE");
 FILE *file;int result;
 if(!handle||handle>16||!sdk_pc_files[handle-1])return -1;
 file=sdk_pc_files[handle-1];sdk_pc_files[handle-1]=NULL;result=fclose(file);return result? -1:0;
}
GDB_CALL sint32 FUN_8007A0B4(uint32 unused,uint32 handle,uint32 bytes,uint32 source) {
 FF_FUNCTION_MARKER(0x8007a0b4u,"GAME.EXE");
 size_t written;FILE *file;(void)unused;
 if(!handle||handle>16||!sdk_pc_files[handle-1])return -1;
 file=sdk_pc_files[handle-1];written=fwrite(ff_ptr(source,bytes),1,bytes,file);
 return ferror(file)?-1:(sint32)written;
}
GDB_CALL uint32 FUN_80079FF4(uint32 handle,uint32 source,uint32 bytes) {
 FF_FUNCTION_MARKER(0x80079ff4u,"GAME.EXE");
 uint32 total=0,chunk; sint32 written;
 while(bytes){chunk=bytes>0x8000?0x8000:bytes;written=FUN_8007A0B4(0,handle,chunk,source);
  total+=(uint32)written;if(written==-1)return 0xffffffffu;
  source+=(uint32)written;bytes-=(uint32)written;if(written<(sint32)chunk)break;
 }return total;
}
GDB_CALL void FUN_SLUS_8001AACC(void) {
 FF_FUNCTION_MARKER(0x8001aaccu,"SLUS_004.33");
 uint32 address;
 FUN_SLUS_8001370C();
 ++ff_dummy_calls[2]; /* WIP SKIP1BA28/17E10: detach CD DMA callback3. */
 if(ff_u32(0x8001ed48)==1)ff_w32(0x80032a44,0);else ff_w32(0x8001ed28,0);
 address=ff_u32(0x8001f088);
 if((address&0x1fffffffu)>=0x1f801800&&(address&0x1fffffffu)<=0x1f801803)++ff_dummy_calls[1];else ff_w8(address,0);
 address=ff_u32(0x8001f094);
 if((address&0x1fffffffu)>=0x1f801800&&(address&0x1fffffffu)<=0x1f801803)++ff_dummy_calls[1];else ff_w8(address,0);
 FUN_SLUS_8001371C();
}
GDB_CALL void FUN_SLUS_80011EC8(void) {
 FF_FUNCTION_MARKER(0x80011ec8u,"SLUS_004.33");
 FUN_SLUS_8001AACC();ff_dummy_cd_control(9,NULL,NULL);ff_dummy_str_decode(NULL,0); /* WIP DecDCTReset(0). */
 if(ff_s16(0x80030358))FUN_SLUS_80011F54();
 ff_spu_cd_volume_host(ff_s16(0x800303cc),ff_s16(0x800303ce));
}
GDB_CALL uint32 FUN_SLUS_80011FE0(uint32 location) {
 FF_FUNCTION_MARKER(0x80011fe0u,"SLUS_004.33");
 uint16 failed=0,accepted=0;
 for(;;){
  while(!ff_dummy_cd_control(21,(const uint8*)ff_ptr(location,4),NULL))if(++failed==5)return 0;
  failed=0;if(++accepted==6)return 0;
  /* WIP StCdInterrupt(448): no sector stream is produced by the CD dummy. */
  ++ff_dummy_calls[2];
 }
}
/* SKIP1ADBC ring consumer: no fabricated frames; existing RAM entries remain
 * readable, including the original wrap sentinel and ownership transition. */
static uint32 sdk_slus_next_stream(uint32 *stream,uint32 *header) {
 uint32 p=ff_u32(0x80034270)+(ff_u32(0x8003425c)<<5);
 if((uint16)ff_s16(p)==1){ff_w32(0x8003425c,0);if(ff_u32(0x80034264))ff_w16(p,0);p=ff_u32(0x80034270)+(ff_u32(0x8003425c)<<5);}
 if((uint16)ff_s16(p)!=2)return 1;
 ff_w16(p,4);*stream=ff_u32(0x80034270)+(ff_u32(0x80034274)<<5)+2016u*ff_u32(0x8003425c);*header=p;return 0;
}
GDB_CALL uint32 FUN_SLUS_800120DC(void) {
 FF_FUNCTION_MARKER(0x800120dcu,"SLUS_004.33");
 uint32 remaining=0x40000,stream,header;
 while(sdk_slus_next_stream(&stream,&header))if(!--remaining)return 0;
 ff_w32(0x8003036c,ff_u32(0x8003036c)+1u);
 if(ff_u32(0x8003036c)==1){ff_w32(0x80030400,(uint16)ff_s16(header+16));ff_w32(0x80030404,(uint16)ff_s16(header+18));}
 if(ff_u32(0x80030364)<ff_u32(header+8)||ff_u32(header+8)<ff_u32(0x8003036c)||
    (uint16)ff_s16(header+16)!=(uint16)ff_s16(0x80030360)||(uint16)ff_s16(header+18)!=(uint16)ff_s16(0x80030362))ff_w16(0x80030388,1);
 return stream;
}
static uint32 sdk_slus_release_stream(uint32 stream) {
 uint32 base=ff_u32(0x80034270),delta=stream-(base+(ff_u32(0x80034274)<<5)),q,p,i=0;
 sint32 shifted=(sint32)delta>>2,high=(sint32)(((sint64)shifted*(sint32)0x82082083u)>>32),count;
 q=(uint32)((sint32)((uint32)high+(uint32)shifted)>>8)-(uint32)((sint32)delta>>31);p=base+(q<<5);
 count=ff_s16(p+6);if(ff_s16(p)!=4)return 1;
 while((sint32)i<count){p=ff_u32(0x80034270)+((q+i)<<5);++i;ff_w16(p,0);}
 ff_w32(0x8003425c,q+i);return 0;
}
GDB_CALL uint32 FUN_SLUS_800122A0(uint32 stream) {
 FF_FUNCTION_MARKER(0x800122a0u,"SLUS_004.33");
 uint32 destination;
 if(!stream)return 0;ff_w16(0x8003038e,(uint16)ff_s16(0x8003038e)^1);
 destination=ff_u32(0x80030394+4u*(uint32)(sint32)ff_s16(0x8003038e));
 (void)destination;ff_dummy_str_decode(ff_ptr(stream,1),0); /* WIP VLC decode; preserve the original buffer selection. */
 return sdk_slus_release_stream(stream);
}
GDB_CALL uint32 FUN_8007676C(void) {
 FF_FUNCTION_MARKER(0x8007676cu,"GAME.EXE");
 return ff_spu_reset_startup(); /* 7678C mode0, existing native SPU reset. */
}
GDB_CALL void FUN_8006E0D4(uint32 guest_return_address) {
 FF_FUNCTION_MARKER(0x8006e0d4u,"GAME.EXE");
 ff_w32(0x80093fd8,guest_return_address);FUN_8006CE80();
 /* WIP card BIOS patch: B0:56 resolves a BIOS handler and replaces its
  * words70/74/78, then flushes I-cache. Native no-media handling requires
  * neither executable RAM patches nor a host instruction-cache flush. */
 ++ff_dummy_calls[0];FUN_8006CE90();
}
GDB_CALL void FUN_8006DED4(void) {
 FF_FUNCTION_MARKER(0x8006ded4u,"GAME.EXE");
 FUN_8006DF24();
 /* WIP SKIP6E044 card interrupt patch boundary. */
 FUN_8006CE80();++ff_dummy_calls[0];FUN_8006CE90();
 FUN_8006E0D4(0x8006def4);
}
GDB_CALL void FUN_80057380(void) {
 FF_FUNCTION_MARKER(0x80057380u,"GAME.EXE");
 FUN_8006CE70();FUN_8006DED4();
}
GDB_CALL uint32 FUN_80078D08(uint32 source,uint32 bytes) {
 FF_FUNCTION_MARKER(0x80078d08u,"GAME.EXE");
 /* Native synchronous SpuWrite includes this entry's unsigned cap and
  * conditional completion-flag clear. */
 return ff_spu_transfer_write(source,bytes);
}
GDB_CALL sint32 FUN_SLUS_80012414(uint32 volume) {
 FF_FUNCTION_MARKER(0x80012414u,"SLUS_004.33");
 sint16 next=(sint16)((uint16)ff_s16(volume)-8);ff_w16(volume,(uint16)next);
 if(next<0)ff_w16(volume,0);
 ff_spu_cd_volume_host(ff_s16(volume),ff_s16(volume));
 return ff_s16(volume)?0:2;
}
/* WIP BIOS critical-section boundary. Host game callbacks execute serially;
 * this tracks guest enable state without disabling Windows interrupts. */
uint32 ff_sdk_interrupts_enabled=1;
GDB_CALL void FUN_8006CE80(void) {
 FF_FUNCTION_MARKER(0x8006ce80u,"GAME.EXE");
 ff_sdk_interrupts_enabled=0;
}
GDB_CALL void FUN_8006CE90(void) {
 FF_FUNCTION_MARKER(0x8006ce90u,"GAME.EXE");
 ff_sdk_interrupts_enabled=1;
}
GDB_CALL void FUN_SLUS_8001370C(void) {
 FF_FUNCTION_MARKER(0x8001370cu,"SLUS_004.33");
 ff_sdk_interrupts_enabled=0;
}
GDB_CALL void FUN_SLUS_8001371C(void) {
 FF_FUNCTION_MARKER(0x8001371cu,"SLUS_004.33");
 ff_sdk_interrupts_enabled=1;
}
GDB_CALL void FUN_8006DE48(uint32 mode) {
 FF_FUNCTION_MARKER(0x8006de48u,"GAME.EXE");
 ++ff_dummy_calls[0]; /* WIP ChangeClearPAD(0). */
 FUN_8006CE80();FUN_8006DF04(mode);
 /* WIP SKIP DF8C/E044 patch PSX BIOS card interrupt handlers. Native
  * no-media events replace those handlers, without executable RAM patches. */
 FUN_8006CE80();++ff_dummy_calls[0];FUN_8006CE80();++ff_dummy_calls[0];FUN_8006CE90();
}
GDB_CALL void FUN_8006DE9C(void) {
 FF_FUNCTION_MARKER(0x8006de9cu,"GAME.EXE");
 FUN_8006CE80();FUN_8006DF14();++ff_dummy_calls[0];FUN_8006CE90();
}
GDB_CALL sint32 FUN_8006B050(void) {
 FF_FUNCTION_MARKER(0x8006b050u,"GAME.EXE");
 static const uint32 specs[4]={4,0x8000,0x100,0x2000};uint32 i;sint32 result=0;
 /* WIP native event namespace is recreated on card subsystem init. */
 ff_dummy_card_events_reset();
 for(i=0;i<8;++i)ff_w32(0x80093cb0+4*i,ff_dummy_card_open_event(i<4?0xf4000001u:0xf0000011u,specs[i&3],0x2000,0));
 FUN_8006DE48(0);FUN_8006DE9C();++ff_dummy_calls[0]; /* WIP _bu_init. */
 ++ff_dummy_calls[0]; /* WIP ChangeClearPAD(0). */
 for(i=0;i<8;++i)result=ff_dummy_card_enable_event(ff_u32(0x80093cb0+4*i));return result;
}
static uint32 sdk_signed_arithmetic(sint64 value);
static uint32 sdk_add_primitive(uint32 bucket,uint32 packet) {
 uint32 tag=ff_u32(packet),head=ff_u32(bucket);ff_w32(packet,(tag&0xff000000u)|(head&0xffffff));
 head=(ff_u32(bucket)&0xff000000u)|(packet&0xffffff);ff_w32(bucket,head);return head;
}
static uint32 sdk_counter_overlay(uint32 bucket,uint32 entry_v0,sint32 entry_v1,uint32 base,uint32 sprites) {
 uint32 index=0;ff_w16(base+16,(uint16)(entry_v0+25));ff_w16(base+32,(uint16)(entry_v0+25));
 if(entry_v1>0)do{sdk_add_primitive(bucket,sprites+20*(index&255));++index;}while((sint32)(index&255)<ff_s16(0x80093fa6));
 return sdk_add_primitive(bucket,base);
}
GDB_CALL uint32 FUN_8006CBB4(uint32 bucket,uint32 entry_v0,sint32 entry_v1) {
 FF_FUNCTION_MARKER(0x8006cbb4u,"GAME.EXE");
 /* MIPS retains V0/V1 on entry; IDA's one-argument prototype loses them. */
 return sdk_counter_overlay(bucket,entry_v0,entry_v1,0x80094cd8,0x80094d20);
}
GDB_CALL uint32 FUN_8006CC64(uint32 bucket,uint32 entry_v0,sint32 entry_v1) {
 FF_FUNCTION_MARKER(0x8006cc64u,"GAME.EXE");
 return sdk_counter_overlay(bucket,entry_v0,entry_v1,0x80094cfc,0x80094dd4);
}
GDB_CALL uint32 FUN_SLUS_80012334(void) {
 FF_FUNCTION_MARKER(0x80012334u,"SLUS_004.33");
 uint32 countdown=0x800000,offset;
 do{if(ff_s16(0x8003038a))break;--countdown;}while(countdown);
 ff_w16(0x8003038a,0);ff_w16(0x8003038c,(uint16)ff_s16(0x8003038c)^1);
 offset=(uint32)(sint32)ff_s16(0x8003038c)<<3;ff_w16(0x80030380,(uint16)ff_s16(0x80030370+offset));
 offset=(uint32)(sint32)ff_s16(0x8003038c)<<3;ff_w16(0x80030382,(uint16)ff_s16(0x80030372+offset));return offset;
}
GDB_CALL uint32 FUN_SLUS_800124E0(void) {
 FF_FUNCTION_MARKER(0x800124e0u,"SLUS_004.33");
 sint32 lines=(uint16)ff_s16(0x80030362),groups=(lines-1)/16+1;
 return (uint32)(groups*(ff_s16(0x80030358)?192:128));
}
GDB_CALL uint32 FUN_SLUS_800111CC(void) {
 FF_FUNCTION_MARKER(0x800111ccu,"SLUS_004.33");
 uint32 result=0;
 if((*(uint8*)ff_ptr(0x80030439,1)>>4)==4)result=(uint32)(uint16)~ff_s16(0x8003043a)<<16;
 if((*(uint8*)ff_ptr(0x80030411,1)>>4)==4)result|=(uint16)~ff_s16(0x80030412);
 return result;
}
GDB_CALL sint32 FUN_SLUS_80011F54(void) {
 FF_FUNCTION_MARKER(0x80011f54u,"SLUS_004.33");
 PSX_RECT rectangle;rectangle.x=0;rectangle.y=0;
 rectangle.w=(sint16)FUN_SLUS_800125AC((uint16)ff_s16(0x8003035a));rectangle.h=240;
 ClearImage(&rectangle,0,0,0);rectangle.y=240;return ClearImage(&rectangle,0,0,0);
}
GDB_CALL uint32 FUN_SLUS_800104B8(uint32 destination,uint32 source,uint32 words) {
 FF_FUNCTION_MARKER(0x800104b8u,"SLUS_004.33");
 uint32 groups=words>>2,tail=words&3,a,b,c,d;
 if((sint32)words<=0)return groups;
 while(groups){a=ff_u32(source);b=ff_u32(source+4);c=ff_u32(source+8);d=ff_u32(source+12);source+=16;--groups;
  ff_w32(destination,a);ff_w32(destination+4,b);ff_w32(destination+8,c);ff_w32(destination+12,d);destination+=16;}
 while(tail){a=ff_u32(source);source+=4;--tail;ff_w32(destination,a);destination+=4;}return groups;
}
GDB_CALL void FUN_SLUS_8001043C(uint32 destination,uint32 value,uint32 count) {
 FF_FUNCTION_MARKER(0x8001043cu,"SLUS_004.33");
 uint32 n;if(!count)return;value|=value<<8;
 if((sint32)count>=4){
  value|=value<<16;n=destination&3;
  if(n){n=4-n;count-=n;do{ff_w8(destination,(uint8)value);--n;destination=sdk_signed_arithmetic((sint64)(sint32)destination+1);}while(n);}
  n=(uint32)((sint32)count>>2);count&=3;
  while(n){ff_w32(destination,value);--n;destination=sdk_signed_arithmetic((sint64)(sint32)destination+4);}
 }
 while(count){ff_w8(destination,(uint8)value);count=sdk_signed_arithmetic((sint64)(sint32)count-1);destination=sdk_signed_arithmetic((sint64)(sint32)destination+1);}
}
GDB_CALL uint32 FUN_SLUS_80011374(uint32 bytes) {
 FF_FUNCTION_MARKER(0x80011374u,"SLUS_004.33");
 uint32 result=ff_u32(0x80030350);if(bytes&3)bytes=(bytes+4)&0xfffffffcu;
 ff_w32(0x80030350,ff_u32(0x80030350)+bytes);return result;
}
GDB_CALL uint32 FUN_SLUS_800125AC(uint32 value) {
 FF_FUNCTION_MARKER(0x800125acu,"SLUS_004.33");
 value&=65535;return ff_s16(0x80030358)?((3*value/2)&65535):value;
}
GDB_CALL uint32 FUN_SLUS_800112AC(void) {
 FF_FUNCTION_MARKER(0x800112acu,"SLUS_004.33");
 ff_w32(0x8003045c,FUN_SLUS_800111CC());ff_w32(0x80030464,ff_u32(0x8003045c)>>16);
 if(ff_u32(0x8003045c)&8)return 1;if(ff_u32(0x8003045c)&0x4000)return 2;return 0;
}
GDB_CALL void FUN_8006DF04(uint32 mode) {
 FF_FUNCTION_MARKER(0x8006df04u,"GAME.EXE");
 /* WIP BIOS B0:4A InitCARD: no physical card controller on the host. */
 (void)mode;++ff_dummy_calls[0];
}
GDB_CALL void FUN_8006DF14(void) {
 FF_FUNCTION_MARKER(0x8006df14u,"GAME.EXE");
 /* WIP BIOS B0:4B StartCARD; requests use the no-media event backend. */
 ++ff_dummy_calls[0];
}
GDB_CALL void FUN_8006DF24(void) {
 FF_FUNCTION_MARKER(0x8006df24u,"GAME.EXE");
 /* WIP BIOS B0:4C StopCARD; no hardware activity to stop. */
 ++ff_dummy_calls[0];
}
GDB_CALL sint32 FUN_8006D230(uint32 left,uint32 right,uint32 count) {
 FF_FUNCTION_MARKER(0x8006d230u,"GAME.EXE");
 for(;;){
  uint32 a=*(uint8*)ff_ptr(left,1),b=*(uint8*)ff_ptr(right,1);++left;
  if(a!=b)return (sint32)*(uint8*)ff_ptr(left-1,1)-(sint32)*(uint8*)ff_ptr(right,1);
  --count;++right;if((sint32)count<=0)return 0;
 }
}
GDB_CALL void FUN_SLUS_80010EDC(void) {
 FF_FUNCTION_MARKER(0x80010edcu,"SLUS_004.33");
 /* Exact JR RA/NOP peer of GAME11FEC; do not emit a GAME marker. */
}
GDB_CALL void FUN_SLUS_8001AE80(uint32 a,uint32 b,uint32 c) {
 FF_FUNCTION_MARKER(0x8001ae80u,"SLUS_004.33");
 ff_w32(0x80034268,a);ff_w32(0x80032a30,b);ff_w32(0x80034264,c);
}
GDB_CALL uint32 FUN_SLUS_80010520(uint32 destination,uint32 source,sint32 count) {
 FF_FUNCTION_MARKER(0x80010520u,"SLUS_004.33");
 uint32 byte=*(uint8*)ff_ptr(source,1); /* Initial BLEZ delay slot reads even for count<=0. */
 if(count<=0)return byte;
 for(;;){++source;--count;ff_w8(destination++,(uint8)byte);if(count<=0)return byte;byte=*(uint8*)ff_ptr(source,1);}
}
GDB_CALL uint32 FUN_SLUS_8001133C(void) {
 FF_FUNCTION_MARKER(0x8001133cu,"SLUS_004.33");
 ff_w32(0x80030350,0x000a4838);return 0x000a4838;
}
GDB_CALL sint32 FUN_SLUS_8001263C(void) {
 FF_FUNCTION_MARKER(0x8001263cu,"SLUS_004.33");
 return ff_s16(0x80030358)?3:2;
}
GDB_CALL uint32 FUN_SLUS_80034388(uint32 text,uint32 x,uint32 y) {
 FF_FUNCTION_MARKER(0x80034388u,"SLUS_004.33");
 uint32 p=ff_u32(0x800ac52c);uint8 byte;
 ff_w32(p,0x74ffffff);ff_w32(p+4,(x&65535)|(y<<16));p+=8;
 do{byte=*(uint8*)ff_ptr(text++,1);ff_w8(p++,byte);}while(byte);
 ff_w32(0x800ac52c,(p+3)&0xfffffffcu);return 0;
}
static void sdk_sector_to_bcd(sint32 sector,uint32 destination) {
 sint32 total=(sint32)((uint32)sector+150u),seconds,minutes,frames;
 seconds=total/75;frames=total-seconds*75;minutes=seconds/60;seconds-=minutes*60;
 ff_w8(destination+2,(uint8)(((uint32)(frames/10)<<4)+(uint32)(frames%10)));
 ff_w8(destination+1,(uint8)(((uint32)(seconds/10)<<4)+(uint32)(seconds%10)));
 ff_w8(destination,(uint8)(((uint32)(minutes/10)<<4)+(uint32)(minutes%10)));
}
GDB_CALL sint32 FUN_SLUS_8001ABE8(uint32 destination,uint32 entry_v0) {
 FF_FUNCTION_MARKER(0x8001abe8u,"SLUS_004.33");
 if(entry_v0)return -1;
 sdk_sector_to_bcd((sint32)((uint32)FUN_SLUS_80017F58(0x800329e8)+1),destination);
 return (sint32)ff_u32(0x800329ec);
}
static uint32 sdk_signed_arithmetic(sint64 value) {
 if(value>2147483647LL||value<(-2147483647LL-1))ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
 return (uint32)value;
}
static uint32 sdk_mips_multiply(ff_mips_1288c_registers *r,uint32 a,uint32 b) {
 uint64 product=(uint64)((sint64)(sint32)a*(sint32)b);
 r->hi=(uint32)(product>>32);r->lo=(uint32)product;return r->lo;
}
GDB_CALL void FUN_8001288C(ff_mips_1288c_registers *r) {
 FF_FUNCTION_MARKER(0x8001288cu,"GAME.EXE");
 /* Register ABI: IDA's empty void body loses all six inputs and four outputs. */
 r->a0=sdk_mips_multiply(r,r->t6,r->t4);r->a1=sdk_mips_multiply(r,r->v1,r->t5);
 r->t2=(uint32)((sint32)sdk_signed_arithmetic((sint64)(sint32)r->a0+(sint32)r->a1)>>12);
 r->a0=sdk_mips_multiply(r,r->v1,r->t4);r->a1=sdk_mips_multiply(r,r->t6,r->t5);
 r->t3=(uint32)((sint32)sdk_signed_arithmetic((sint64)(sint32)r->a0-(sint32)r->a1)>>12);
 r->t2=sdk_signed_arithmetic(-(sint64)(sint32)r->t2);
 r->t2=sdk_signed_arithmetic((sint64)(sint32)r->t2+(sint32)r->t0);
 r->t3=sdk_signed_arithmetic((sint64)(sint32)r->t3+(sint32)r->t1);
}
/* WIP BIOS file boundary: an absent memory card has no files. The path is
 * still constructed by the translated caller; no successful reads are invented. */
static sint32 sdk_card_open_dummy(uint32 path,uint32 mode) {
 (void)path;(void)mode;++ff_dummy_calls[0];return -1;
}
static sint32 sdk_card_close_dummy(sint32 fd) {(void)fd;++ff_dummy_calls[0];return 0;}
static void sdk_card_path(uint32 slot,uint32 name) {
 sprintf(ff_ptr(0x80093f60,1),"bu%2.2x:%s",slot&255,(char*)ff_ptr(name,1));
}
static sint32 sdk_card_write_dummy(sint32 fd,uint32 source,uint32 bytes) {
 (void)fd;(void)source;(void)bytes;++ff_dummy_calls[0];return -1; /* WIP absent card. */
}
GDB_CALL sint32 FUN_8006B22C(uint32 slot,uint32 name,uint32 title,uint32 data) {
 FF_FUNCTION_MARKER(0x8006b22cu,"GAME.EXE");
 uint32 words[4],source,destination;int i,j,frame;sint32 fd,written;
 if(FUN_8006B1C8(slot,name)){sdk_card_path(slot,name);if(!FUN_8006CF10(0x80093f60))return -2;}
 ff_w8(0x80094ad8,'S');ff_w8(0x80094ad9,'C');ff_w8(0x80094ada,17);ff_w8(0x80094adb,1);FUN_8006BF14(0x80094adc,title);
 /* Palette copied as3+3+2 words; each load group precedes its stores. */
 for(i=0;i<8;){int n=i<6?3:2;for(j=0;j<n;++j)words[j]=ff_u32(0x80080224+4u*(i+j));for(j=0;j<n;++j)ff_w32(0x80094b38+4u*(i+j),words[j]);i+=n;}
 for(frame=0;frame<3;++frame)for(i=0;i<128;i+=16){
  source=0x80080250+i;destination=0x80094b58+128*frame+i;
  for(j=0;j<4;++j)words[j]=ff_u32(source+4*j);for(j=0;j<4;++j)ff_w32(destination+4*j,words[j]);
 }
 for(i=0;i<512;i+=16){for(j=0;j<4;++j)words[j]=ff_u32(0x80094ad8+i+4*j);for(j=0;j<4;++j)ff_w32(data+i+4*j,words[j]);}
 sdk_card_path(slot,name);fd=sdk_card_open_dummy(0x80093f60,((uint32)(sint32)*(sint8*)ff_ptr(0x80094adb,1)<<16)|0x200);
 if(fd==-1)return -1;sdk_card_close_dummy(fd);fd=sdk_card_open_dummy(0x80093f60,2);if(fd==-1)return -2;
 if(ff_u32(0x80092484))FUN_80068788();written=sdk_card_write_dummy(fd,data,8192);sdk_card_close_dummy(fd);return written==8192?1:-3;
}
GDB_CALL uint32 FUN_8006CEF0(uint32 path,uint32 entry) {
 FF_FUNCTION_MARKER(0x8006cef0u,"GAME.EXE");
 /* BIOS B0:42 firstfile, WIP empty card directory. */
 (void)path;(void)entry;++ff_dummy_calls[0];return 0;
}
GDB_CALL sint32 FUN_8006CF10(uint32 path) {
 FF_FUNCTION_MARKER(0x8006cf10u,"GAME.EXE");
 /* BIOS B0:45 erase, WIP absent card. */
 (void)path;++ff_dummy_calls[0];return 0;
}
GDB_CALL sint32 FUN_8006B1C8(uint32 slot,uint32 name) {
 FF_FUNCTION_MARKER(0x8006b1c8u,"GAME.EXE");
 sint32 fd;sdk_card_path(slot,name);fd=sdk_card_open_dummy(0x80093f60,1);
 if(fd==-1)return 0;sdk_card_close_dummy(fd);return 1;
}
GDB_CALL sint32 FUN_8006B6CC(uint32 slot) {
 FF_FUNCTION_MARKER(0x8006b6ccu,"GAME.EXE");
 sprintf(ff_ptr(0x80093f60,1),"bu%2.2x:",slot&255);
 /* WIP B0:41 format: absent card, no storage mutation. */
 ++ff_dummy_calls[0];return 0;
}
GDB_CALL sint32 FUN_8006B710(uint32 slot,uint32 name) {
 FF_FUNCTION_MARKER(0x8006b710u,"GAME.EXE");
 if(!FUN_8006B1C8(slot,name))return 1;
 sdk_card_path(slot,name);return FUN_8006CF10(0x80093f60);
}
GDB_CALL sint32 FUN_8006B77C(uint32 slot,uint32 name,uint32 destination) {
 FF_FUNCTION_MARKER(0x8006b77cu,"GAME.EXE");
 sint32 fd,count;sdk_card_path(slot,name);fd=sdk_card_open_dummy(0x80093f60,1);
 if(fd==-1)return 0;
 count=ff_dummy_memory_card_read(fd,ff_ptr(destination,8192),8192);
 sdk_card_close_dummy(fd);return count==8192;
}
GDB_CALL sint32 FUN_8006B9D0(void) {
 FF_FUNCTION_MARKER(0x8006b9d0u,"GAME.EXE");
 uint8 a,b,c,d;ff_w8(0x80093cf0,255);
 if(!FUN_8006B77C(0,0x80093c04,0x000e2d40)){ff_w8(0x80093cf0,254);return -2;}
 a=*(uint8*)ff_ptr(0x000e2f40,1);b=*(uint8*)ff_ptr(0x000e2f41,1);
 c=*(uint8*)ff_ptr(0x000e2f42,1);d=*(uint8*)ff_ptr(0x000e2f43,1);
 ff_w8(0x80093cf0,a);ff_w8(0x80093cf1,b);ff_w8(0x80093cf2,c);ff_w8(0x80093cf3,d);return a;
}
static uint32 sdk_card_next_file_dummy(uint32 entry) {(void)entry;++ff_dummy_calls[0];return 0;}
GDB_CALL sint32 FUN_8006BF74(uint32 slot,uint32 entry) {
 FF_FUNCTION_MARKER(0x8006bf74u,"GAME.EXE");
 uint8 saved[128];uint32 count=0,path=0x801fc600;char *buffer=ff_ptr(path,128);
 memcpy(saved,buffer,128);sprintf(buffer,"bu%2.2x:",slot);strcat(buffer,ff_ptr(0x80093cf4,1));
 if(FUN_8006CEF0(path,entry)==entry){
  count=1;do{entry+=40;if(sdk_card_next_file_dummy(entry)!=entry)break;++count;}while(1);
 }
 memcpy(buffer,saved,128);return (sint32)count;
}
GDB_CALL sint32 FUN_8006BFFC(uint32 slot,uint32 names,uint32 entries) {
 FF_FUNCTION_MARKER(0x8006bffcu,"GAME.EXE");
 sint32 count,fd,bytes;uint32 i,blocks;uint8 saved[264];uint32 temp=0x801fc680;
 ff_w32(0x80093ca0,15);count=FUN_8006BF74(slot,entries);if(count<=0)return count;
 memcpy(saved,ff_ptr(temp,264),264);
 for(i=0;(sint32)i<count;++i,entries+=40,names+=64){
  sdk_card_path(slot,entries);blocks=ff_u32(entries+24)+1;
  blocks=(uint32)((sint32)blocks/8192);ff_w32(0x80093ca0,ff_u32(0x80093ca0)-blocks);
  fd=sdk_card_open_dummy(0x80093f60,1);
  if(fd==-1){memcpy(ff_ptr(temp,264),saved,264);return 0;}
  bytes=ff_dummy_memory_card_read(fd,ff_ptr(0x80094ad8,512),512);
  /* The original failure edge does not close an already-open descriptor. */
  if(bytes!=512){memcpy(ff_ptr(temp,264),saved,264);return 0;}
  if(*(uint8*)ff_ptr(0x80094adc,1)<128)strcpy(ff_ptr(temp,264),ff_ptr(0x80094adc,1));
  else FUN_8006C6E8(temp,0x80094adc);
  strcpy(ff_ptr(names,1),ff_ptr(temp,1));sdk_card_close_dummy(fd);
 }
 memcpy(ff_ptr(temp,264),saved,264);return count;
}
GDB_CALL uint32 FUN_8006BDF4(uint32 character) {
 FF_FUNCTION_MARKER(0x8006bdf4u,"GAME.EXE");
 uint32 ch=character&255,group=0,punctuation=0,value;
 if(((ch-32)&255)<16)punctuation=1;
 else if(((ch-48)&255)<10)group=0;
 else if(((ch-58)&255)<7)punctuation=11;
 else if(((ch-65)&255)<26)group=1;
 else if(((ch-91)&255)<6)punctuation=37;
 else if(((ch-97)&255)<26)group=2;
 else if(((ch-123)&255)<4)punctuation=63;
 else return 0;
 if(punctuation)value=(uint16)ff_s16(0x800801cc+2u*(ch-punctuation-31u));
 else value=(uint16)ff_s16(0x80093ca4+4*group)+ch-(uint32)(uint16)ff_s16(0x80093ca6+4*group);
 return ((value&255)<<8)|((value>>8)&255);
}
GDB_CALL sint32 FUN_8006BF14(uint32 destination,uint32 source) {
 FF_FUNCTION_MARKER(0x8006bf14u,"GAME.EXE");
 while(*(sint8*)ff_ptr(source,1)){
  uint32 ch=*(uint8*)ff_ptr(source++,1);ff_w16(destination,(uint16)FUN_8006BDF4(ch));destination+=2;
 }
 ff_w16(destination,0);return 0;
}
GDB_CALL sint32 FUN_8006C178(uint32 character) {
 FF_FUNCTION_MARKER(0x8006c178u,"GAME.EXE");
 uint32 ch=character&65535,high=ch>>8;
 /* Original decision tree omits 5282 ('3'); do not fill that hole. */
 if((ch&255)==0x82){
  if(high>=0x4f&&high<=0x58&&high!=0x52)return (sint32)(high-0x4f+'0');
  if(high>=0x60&&high<=0x79)return (sint32)(high-0x60+'A');
  if(high>=0x81&&high<=0x9a)return (sint32)(high-0x81+'a');
 }
 if(ch==0x8144)return '.';if(ch==0x817c)return '-';return ' ';
}
GDB_CALL void FUN_8006C6E8(uint32 destination,uint32 source) {
 FF_FUNCTION_MARKER(0x8006c6e8u,"GAME.EXE");
 uint32 ch;while((ch=(uint16)ff_s16(source))!=0){
  source+=2;ff_w8(destination++,(uint8)FUN_8006C178(ch));
 }ff_w8(destination,0);
}
GDB_CALL sint32 FUN_8006BA58(uint32 destination) {
 FF_FUNCTION_MARKER(0x8006ba58u,"GAME.EXE");
 sint32 stage=*(sint8*)ff_ptr(0x80093cf0,1);const char *format,*first,*second,*level;char *out=ff_ptr(destination,1);
 if(stage==-1||stage==-2){format=ff_ptr(ff_u32(stage==-1?0x80092464:0x80092468),1);return sprintf(out,format);}
 level=ff_ptr(ff_u32(0x8007f79c+4u*(uint32)stage),1);
 first=ff_ptr(ff_u32(0x800b6e90+4u*(uint32)(sint32)*(sint8*)ff_ptr(0x80093cf2,1)),1);
 if(*(sint8*)ff_ptr(0x80093cf1,1)==1)return sprintf(out,ff_ptr(ff_u32(0x8009246c),1),first,level);
 second=ff_ptr(ff_u32(0x800b6e90+4u*(uint32)(sint32)*(sint8*)ff_ptr(0x80093cf3,1)),1);
 /* The level string is the fifth argument, stored at SP+10, omitted by IDA. */
 return sprintf(out,ff_ptr(ff_u32(0x80092470),1),first,second,level);
}
/* Identical instruction words in GAME and SLUS; private arithmetic avoids
 * logging a call into the other image when this implementation is shared. */
static sint32 sdk_bcd_sector(uint32 position) {
 uint32 minute=*(uint8*)ff_ptr(position,1),second=*(uint8*)ff_ptr(position+1,1),frame=*(uint8*)ff_ptr(position+2,1);
 return (sint32)(75u*(60u*(10u*(minute>>4)+(minute&15))+10u*(second>>4)+(second&15))+10u*(frame>>4)+(frame&15)-150u);
}
GDB_CALL sint32 FUN_800731E8(uint32 position) {
 FF_FUNCTION_MARKER(0x800731e8u,"GAME.EXE");
 return sdk_bcd_sector(position);
}
GDB_CALL sint32 FUN_SLUS_80017F58(uint32 position) {
 FF_FUNCTION_MARKER(0x80017f58u,"SLUS_004.33");
 return sdk_bcd_sector(position);
}
/* WIP TestEvent: consume synthetic no-media events from the dummy backend. */
static sint32 sdk_card_test_event_dummy(uint32 handle) {
 return ff_dummy_card_test_event(handle);
}
static sint32 sdk_card_test_group(uint32 table) {
 uint32 i;sint32 result=0;
 for(i=0;i<4;++i)result=sdk_card_test_event_dummy(ff_u32(table+4*i));
 return result;
}
GDB_CALL sint32 FUN_8006B894(void) {
 FF_FUNCTION_MARKER(0x8006b894u,"GAME.EXE");
 return sdk_card_test_group(0x80093cb0);
}
GDB_CALL sint32 FUN_8006B950(void) {
 FF_FUNCTION_MARKER(0x8006b950u,"GAME.EXE");
 return sdk_card_test_group(0x80093cc0);
}
static sint32 sdk_card_wait_group(uint32 table) {
 uint32 i;for(;;)for(i=0;i<4;++i)if(sdk_card_test_event_dummy(ff_u32(table+4*i))==1)return (sint32)i;
}
GDB_CALL sint32 FUN_8006B820(void) {
 FF_FUNCTION_MARKER(0x8006b820u,"GAME.EXE");
 return sdk_card_wait_group(0x80093cb0);
}
GDB_CALL sint32 FUN_8006B8DC(void) {
 FF_FUNCTION_MARKER(0x8006b8dcu,"GAME.EXE");
 return sdk_card_wait_group(0x80093cc0);
}
static void sdk_card_completion(uint32 result) {
 ff_w32(0x80093cec,result);ff_w32(0x80093ce8,3);
}
GDB_CALL sint32 FUN_8006BB44(void) {
 FF_FUNCTION_MARKER(0x8006bb44u,"GAME.EXE");
 uint32 state=ff_u32(0x80093ce8);sint32 result;
 if(!state){FUN_8006B894();ff_dummy_card_request_no_media(0);ff_w32(0x80093ce8,1);return 1;}
 if(state==1){
  if(sdk_card_test_event_dummy(ff_u32(0x80093cb0))==1){
   FUN_8006B894();ff_dummy_card_request_no_media(0);ff_w32(0x80093ce8,4);return 4;
  }
  if(sdk_card_test_event_dummy(ff_u32(0x80093cb4))==1){sdk_card_completion(3);return 3;}
  if(sdk_card_test_event_dummy(ff_u32(0x80093cb8))==1){sdk_card_completion(0);return 3;}
  result=sdk_card_test_event_dummy(ff_u32(0x80093cbc));
  if(result==1){
   FUN_8006B950();ff_dummy_card_request_no_media(1);FUN_8006B8DC();
   FUN_8006B894();ff_dummy_card_request_no_media(0);ff_w32(0x80093ce8,7);return 7;
  }return result;
 }
 if(state==3){ff_w32(0x80093ce8,0);return 4;}
 if(state!=4&&state!=7)return 7;
 if(sdk_card_test_event_dummy(ff_u32(0x80093cb0))==1){
  sdk_card_completion(1);
  if(state==7){ff_w8(0x80093cf0,255);return -1;}
  if(*(sint8*)ff_ptr(0x80093cf0,1)==-1)return FUN_8006B9D0();return -1;
 }
 result=sdk_card_test_event_dummy(ff_u32(0x80093cb4));
 if(result==1){sdk_card_completion(3);return result;}
 result=sdk_card_test_event_dummy(ff_u32(0x80093cb8));
 if(result==1){sdk_card_completion(0);return result;}
 result=sdk_card_test_event_dummy(ff_u32(0x80093cbc));
 if(result==1){sdk_card_completion(2);if(state==7)return FUN_8006B9D0();}
 return 2;
}
GDB_CALL sint32 FUN_8006B998(void) {
 FF_FUNCTION_MARKER(0x8006b998u,"GAME.EXE");
 do{FUN_80061120();FUN_8006BB44();}while(ff_u32(0x80093ce8));return 0;
}
GDB_CALL uint32 FUN_8006BDBC(uint32 output) {
 FF_FUNCTION_MARKER(0x8006bdbcu,"GAME.EXE");
 FUN_8006BB44();FUN_8006B998();return FUN_8006BD9C(output);
}

GDB_CALL uint32 FUN_8007675C(void) {
 FF_FUNCTION_MARKER(0x8007675cu,"GAME.EXE");
 return ff_u32(0x8008cb18);
}
GDB_CALL uint32 FUN_SLUS_80013F54(void) {
 FF_FUNCTION_MARKER(0x80013f54u,"SLUS_004.33");
 return ff_u32(0x8001eb18);
}
GDB_CALL void FUN_80072694(sint32 x,sint32 y) {
 FF_FUNCTION_MARKER(0x80072694u,"GAME.EXE");
 /* SLL 16 discards the high half before the signed OFX/OFY write. */
 SetGeomOffset((sint16)x,(sint16)y);
}
GDB_CALL void FUN_800726B4(uint32 h) {
 FF_FUNCTION_MARKER(0x800726b4u,"GAME.EXE");
 psx_gte_write_h((uint16)h);
}
GDB_CALL sint32 FUN_80064638(uint32 packet,uint32 unused,uint32 stacked) {
 FF_FUNCTION_MARKER(0x80064638u,"GAME.EXE");
 /* The color-split callee 64620 is exactly JR RA / NOP. V0 survives
  * as the signed color halfword shifted right one. */
 (void)unused;(void)stacked;return ff_s16(packet+16)>>1;
}
/* WIP CD boundary: acknowledge commands without media playback. */
static void sdk_cd_sync_dummy(uint8 *status) {
 ++ff_dummy_calls[1];status[0]=0x80;
}
static sint32 sdk_cd_status_dummy(uint8 *status) {
 return ff_dummy_cd_control(1,NULL,status);
}
GDB_CALL sint32 FUN_80067758(void) {
 FF_FUNCTION_MARKER(0x80067758u,"GAME.EXE");
 uint32 result=ff_u32(0x80093bb4);uint8 status[8];
 if(!result){sdk_cd_sync_dummy(status);ff_dummy_cd_pause();result=1;ff_w32(0x80093bb4,result);}return (sint32)result;
}
GDB_CALL sint32 FUN_800677A0(void) {
 FF_FUNCTION_MARKER(0x800677a0u,"GAME.EXE");
 uint32 result=ff_u32(0x80093bb4);uint8 status[16];
 if(result){
  sdk_cd_sync_dummy(status);ff_dummy_cd_control(3,NULL,NULL);
  do {while(!sdk_cd_status_dummy(status)){}result=status[0]&0x40u;}while(!(status[0]&0x80u)||result);
  ff_w32(0x80093bb4,0);
 }return (sint32)result;
}
GDB_CALL sint32 FUN_800678E8(void) {
 FF_FUNCTION_MARKER(0x800678e8u,"GAME.EXE");
 return FUN_80067758();
}
GDB_CALL sint32 FUN_80067908(void) {
 FF_FUNCTION_MARKER(0x80067908u,"GAME.EXE");
 return FUN_800677A0();
}
GDB_CALL sint32 FUN_80067820(void) {
 FF_FUNCTION_MARKER(0x80067820u,"GAME.EXE");
 uint32 track=ff_u32(0x80093bac),result;uint8 status[8];sint32 current;
 if(track==0xffffffff)return -1;
 ff_dummy_cd_control(1,NULL,status);result=status[0]&0x40u;
 if(result)return (sint32)result;
 /* WIP GetlocP: a ready virtual drive stays on the requested track. No CD audio. */
 ++ff_dummy_calls[1];status[0]=(uint8)(((track/10u)<<4)|(track%10u));
 current=(status[0]&15)+10*((sint32)((uint32)status[0]<<24)>>28);
 track=ff_u32(0x80093bac);result=track-1;
 if((uint32)current!=result&&(uint32)current!=track){
  uint32 asynchronous=ff_u32(0x80093bb0);
  if(asynchronous)return FUN_80067670((sint32)asynchronous,(sint32)track);
  return FUN_80067724();
 }
 return (sint32)result;
}
