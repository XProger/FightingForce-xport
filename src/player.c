#include "ff.h"
GDB_CALL sint32 FUN_8001A268(sint32 index) {
 FF_FUNCTION_MARKER(0x8001a268u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,state=ff_u32(actor+12),distance,offset,x,z,other,dx,dz;
 if(state==87)distance=352;else if(state==82||state==69||state-198u<3u)distance=280;else return 0;
 offset=2u*(uint32)(sint32)ff_s16(actor+56);x=ff_u32(actor+40)+(uint32)(sint32)ff_s16(0x800802d0+offset)*distance;z=ff_u32(actor+48)-(uint32)(sint32)ff_s16(0x800806d0+offset)*distance;
 if(ff_u32(0x800940b0)==2){
  other=0x800b4318+244u*(1u-(uint32)index);dx=x-ff_u32(other+40);dz=z-ff_u32(other+48);
  if(ff_s16(actor+66)<=0||ff_s16(other+66)<=0){dx=0;dz=0;}
  if(dx+0x04000000u>0x08000000u||dz+0x04000000u>0x08000000u)return 1;
 }
 return FUN_80019718((sint32)x,(sint32)z);
}
#include "wip.h"
#include <stdio.h>
#include <stdlib.h>
GDB_CALL uint32 FUN_800234E8(uint32 unused,uint32 matrix,uint32 position) {
 FF_FUNCTION_MARKER(0x800234e8u,"GAME.EXE");
 uint32 *point=(uint32*)ff_ptr(position,12);uint32 result;(void)unused;
 FUN_80010040();FUN_80011F74(matrix);
 FUN_800614F4((sint32)point[0],(sint32)point[1],(sint32)point[2],point);
 FUN_80011D20();FUN_8001033C(-ff_s16(0x8008d490));FUN_800101CC(-ff_s16(0x8008d492));
 FUN_800614F4((sint32)point[0],(sint32)point[1],(sint32)point[2],point);FUN_80010028();
 ff_w32(position,ff_u32(position)+ff_u32(0x8008d4a4));ff_w32(position+4,ff_u32(position+4)+ff_u32(0x8008d4a8));
 result=ff_u32(position+8)+ff_u32(0x8008d4ac);ff_w32(position+8,result);return result;
}

#define player_wip_zero(pc) do { if(ff_wip_visit((pc),"player",__FUNCTION__,__FILE__,__LINE__,1,"return_zero_from_function"))return 0;player_wip_abort(pc); } while(0)
#define player_wip_void(pc) do { if(ff_wip_visit((pc),"player",__FUNCTION__,__FILE__,__LINE__,1,"return_from_function"))return;player_wip_abort(pc); } while(0)
#define player_wip_fatal(pc) do { ff_wip_visit((pc),"player",__FUNCTION__,__FILE__,__LINE__,0,"abort");player_wip_abort(pc); } while(0)
static void player_wip_abort(uint32 pc) {
 if(getenv("FF_CAPTURE_WIP")) {
  FILE *f=fopen("../status/gameplay/player-abort.ram","wb");if(f){fwrite(ff_ptr(0x80000000,0x200000),0x200000,1,f);fclose(f);}
  f=fopen("../status/gameplay/player-abort.scratch","wb");if(f){fwrite(ff_ptr(0x1f800000,1024),1024,1,f);fclose(f);}
 }
 fprintf(stderr,"WIP player24008 branch %08X\n",pc);abort();
}
static sint32 player_byte(uint32 address){return *(sint8*)ff_ptr(address,1);}
/* 1CF08: item endpoint in world space, using pose matrix and camera inverse. */
GDB_CALL uint32 FUN_8001CF08(sint32 index,sint32 half,uint32 output[3]) {
 uint32 type=ff_u32(0x8009a3c8+36u*(uint32)player_byte(0x800b4396+244u*(uint32)index));
 sint32 distance=0;int i;
 FUN_80010040();FUN_80011F74(0x800ba234+504u*(uint32)index);FUN_8001033C(6144);
 if(type==477||type==491||type-493u<2u||type==492||type-484u<2u||type==519||type==495||type==520)distance=6291456;
 if(type==486||type==488)distance=3145728;
 if(type==487)distance=6291456;if(type==489)distance=9437184;
 if(type-478u<2u||type-480u<2u||type-482u<2u)distance=3145728;
 if(half)distance>>=1;
 FUN_800614F4(0,0,distance,output);FUN_80011D20();
 FUN_8001033C(-ff_s16(0x8008d490));FUN_800101CC(-ff_s16(0x8008d492));
 FUN_800614F4((sint32)output[0],(sint32)output[1],(sint32)output[2],output);FUN_80010028();
 for(i=0;i<3;i++)output[i]+=ff_u32(0x8008d4a4+4u*i);return output[2];
}
/* 57CE4: append paired trail endpoints, preserving the fourth halfword of each
 * vertex. MIPS uses SRL for scaled X/Z, SRA for scaled Y and lifetime. */
GDB_CALL uint32 FUN_80057CE4(sint32 index,sint32 attachment) {
 uint32 actor=0x800b4318+244u*(uint32)index,point[3],dest,angle;
 sint32 mode=ff_s16(actor+100),x,y,z,sine,cosine,height,age;uint16 v[6];int i,j;
 if(mode==1||mode==3)FUN_8001B9F4(index,attachment,point);else FUN_8001CF08(index,0,point);
 x=(sint32)(point[0]-ff_u32(actor+40))>>16;y=(sint32)(point[1]-ff_u32(actor+44))>>16;z=(sint32)(point[2]-ff_u32(actor+48))>>16;
 if(mode==1) {
  angle=((uint32)FUN_8001614C(0,0,x,z)>>4)&4094u;sine=ff_s16(0x800802d0+angle);cosine=ff_s16(0x800806d0+angle);
  x+=sine>>9;z-=cosine>>9;
  v[0]=(uint16)x;v[1]=(uint16)y;v[2]=(uint16)z;
  v[3]=(uint16)(x-((3*sine)>>10));v[4]=(uint16)y;v[5]=(uint16)(z+((3*cosine)>>10));
 } else {
  v[0]=(uint16)x;v[1]=(uint16)y;v[2]=(uint16)z;height=ff_s16(0x800990da+4u*(uint32)(sint32)ff_s16(actor+52));
  v[3]=(uint16)(((uint32)x*5u)>>3);v[4]=(uint16)(((sint32)(((uint32)y-(uint32)height)*5u)>>3)+height);v[5]=(uint16)(((uint32)z*5u)>>3);
 }
 dest=ff_u32(ff_u32(0x800824ac+4u*((uint32)index&1u)));
 if(ff_s16(actor+98)==1) {
  for(i=0;i<9;i++)for(j=0;j<6;j++)ff_w16(dest+16u*i+(j<3?2u*j:8u+2u*(j-3)),v[j]);
 } else {
  for(i=0;i<16;i++)for(j=0;j<3;j++)ff_w16(dest+8u*i+2u*j,(uint16)ff_s16(dest+8u*i+16u+2u*j));
  for(j=0;j<6;j++)ff_w16(dest+128u+(j<3?2u*j:8u+2u*(j-3)),v[j]);
 }
 age=ff_s16(actor+98)>>2;if(age>=8){ff_w16(actor+100,0);ff_w16(actor+98,0);}
 return FUN_80011F50(0x800b5158+16u*(uint32)index,0x800a1870+32u*(uint32)age+16u*((uint16)ff_s16(actor+52)&1u),16);
}
/* 21220: damage/destruction callback; original167C4 has five arguments. */
GDB_CALL sint32 FUN_80021220(uint32 obstacle,uint32 object,const uint32 *position,uint32 actor){
 FF_FUNCTION_MARKER(0x80021220u,"GAME.EXE");
 uint32 x,y,z,item; sint32 row,col;
 FUN_8001697C(865,position[0],position[1],position[2]);
 ff_w16(obstacle+16,(uint16)((uint16)ff_s16(obstacle+16)-1u));
 if(ff_s16(obstacle+16)>=0)return 1;
 ff_w16(actor+138,(uint16)((uint16)ff_s16(actor+138)+1u));FUN_80056670(113,255,6144);
 x=ff_u32(object+4);y=ff_u32(object+8);z=ff_u32(object+12);ff_w32(object,ff_u32(object)+1u);
 item=0x8009a3c8+36u*(uint32)FUN_800167C4(492,x,y,z,0);
 ff_w16(item+24,64);ff_w16(item+32,0);
 for(row=-1;row<=1;row++)for(col=-1;col<=1;col++)FUN_80031660(x+(uint32)col*0x180000u,y+(uint32)row*0x200000u,z-0x100000u,0);
 return 1;
}
/* 1F7EC: region type6 breakable, with eight impact and eight debris calls. */
GDB_CALL sint32 FUN_8001F7EC(uint32 obstacle,uint32 object,const uint32 *position,uint32 actor){
 FF_FUNCTION_MARKER(0x8001f7ecu,"GAME.EXE");
 uint32 angle=actor?(uint32)(sint32)ff_s16(actor+56):0,kind=ff_u32(object);
 uint32 x=ff_u32(object+4),y=ff_u32(object+8),z=ff_u32(object+12);sint32 i,drop=-1;
 FUN_8001697C(865,position[0],position[1],position[2]);FUN_80056670(112,255,6144);
 for(i=0;i<8;i++)FUN_80031894(position[0],position[1],position[2],angle);
 ff_w16(obstacle+16,(uint16)((uint16)ff_s16(obstacle+16)-1u));
 if(ff_s16(obstacle+16)>=0){ff_w16(object+22,16);return 1;}
 ff_w16(obstacle+12,0);ff_w32(object,55);ff_w16(actor+138,(uint16)((uint16)ff_s16(actor+138)+1u));
 FUN_80056670(19,255,6144);FUN_80031C50(893,x,0xffe00000u,z);
 ff_w32(0x800941a8,16);ff_w32(0x80094190,4);
 for(i=0;i<8;i++)FUN_80031CD8(902u+(ff_rand_8006D348()&3u),x,y,z);
 if(kind==608)drop=(sint32)ff_rand_8006D348()%3+587;
 else if(kind==609)drop=499;else if(kind==610)drop=504;else if(kind==611)drop=496;else if(kind==612)drop=497;else if(kind==613)drop=505;
 if(drop!=-1)FUN_800167C4(drop,x,0,z,0);return 1;
}
/* 213F8: type14 multi-stage damage; signed health selects two destruction paths. */
GDB_CALL sint32 FUN_800213F8(uint32 obstacle,uint32 object,const uint32 *position,uint32 actor){
 FF_FUNCTION_MARKER(0x800213f8u,"GAME.EXE");
 uint32 x,z,item; sint32 health;
 ff_w32(0x800941cc,obstacle);ff_w16(object+22,8);FUN_80056670(117,255,6144);
 if(ff_u32(0x80093d60)!=1){
  ff_w16(obstacle+16,4);FUN_8001697C(866,position[0],position[1],position[2]);return 1;
 }
 if(ff_s16(obstacle+16)<0){
  ff_w16(obstacle+12,0);x=ff_u32(object+4);z=ff_u32(object+12);ff_w32(object,71);
  FUN_80056670(19,255,6144);FUN_80031C50(892,x,position[1],z);
  FUN_80031C50(603,ff_u32(object+4),0,ff_u32(object+12));
  ff_w32(0x800941a8,16);ff_w32(0x80094190,8);
  item=0x8009a3c8+36u*(uint32)FUN_80016654(72,x,0,z);
  ff_w16(item+24,(uint16)((ff_rand_8006D348()&63u)+448u));ff_w16(item+16,3072);
  item=0x8009a3c8+36u*(uint32)FUN_80016654(73,x,0,z);
  ff_w16(item+24,(uint16)((ff_rand_8006D348()&63u)+512u));ff_w16(item+16,(uint16)-3072);
  *(uint8*)ff_ptr(0x80093dd9,1)=0;return 1;
 }
 FUN_8001697C(865,position[0],position[1],position[2]);health=ff_s16(obstacle+16);
 ff_w32(object,health>=3?70u:health>=2?74u:health>0?75u:76u);
 ff_w16(obstacle+16,(uint16)((uint16)ff_s16(obstacle+16)-1u));
 if(ff_s16(obstacle+16)<0){
  ff_w16(obstacle+16,0);ff_w16(obstacle+12,0);*(uint8*)ff_ptr(0x80093dd9,1)=0;
  ff_w16(actor+138,(uint16)((uint16)ff_s16(actor+138)+1u));
  ff_w32(object,77);ff_w16(object+22,0);ff_w16(object+26,0);
 }
 return 1;
}
/* 1FDA0: destruction precedes impact particles; fourth position word selects sound. */
GDB_CALL sint32 FUN_8001FDA0(uint32 obstacle,uint32 object,const uint32 *position,uint32 actor){
 FF_FUNCTION_MARKER(0x8001fda0u,"GAME.EXE");
 uint32 angle=actor?(uint32)(sint32)ff_s16(actor+56):0,v;int i;
 FUN_8001697C(865,position[0],position[1],position[2]);
 ff_w16(obstacle+16,(uint16)((uint16)ff_s16(obstacle+16)-1u));
 if(ff_s16(obstacle+16)<0){
  ff_w16(actor+138,(uint16)((uint16)ff_s16(actor+138)+1u));ff_w16(obstacle+12,0);ff_w32(object,55);
  FUN_80056670(19,255,6144);FUN_80031C50(893,ff_u32(object+4),0xffe00000u,ff_u32(object+12));
  ff_w32(0x800941a8,16);ff_w32(0x80094190,4);
  for(i=0;i<8;i++){v=(ff_rand_8006D348()&3u)+902u;FUN_80031CD8(v,ff_u32(object+4),ff_u32(object+8),ff_u32(object+12));}
  *(uint8*)ff_ptr(0x80093dd9,1)=0;
 }
 for(i=0;i<8;i++)FUN_80031894(position[0],position[1],position[2],angle);
 if(position[3])FUN_80056670(23,192,6144);else FUN_80056670(33,255,6144);return 2;
}
GDB_CALL sint32 FUN_80022548(uint32 obstacle,uint32 object,const uint32 *position,uint32 actor){
 FF_FUNCTION_MARKER(0x80022548u,"GAME.EXE");
 uint32 angle=actor?(uint32)(sint32)ff_s16(actor+56):0,x,z,y,v;int i;(void)obstacle;
 if(ff_u32(object)==648)return 2;
 FUN_8001697C(865,position[0],position[1],position[2]);
 for(i=0;i<8;i++)FUN_80031894(position[0],position[1],position[2],angle);
 if(position[3])FUN_80056670(23,192,6144);else FUN_80056670(33,255,6144);
 x=ff_u32(object+4);z=ff_u32(object+12);ff_w16(actor+138,(uint16)((uint16)ff_s16(actor+138)+1u));
 FUN_80056670(113,255,6144);FUN_80031C50(894,x,0xff800000u,z);
 for(i=0;i<16;i++){y=(0xffffffc0u-(ff_rand_8006D348()&127u))<<16;v=ff_rand_8006D348()&2047u;FUN_80031894(x,y,z,v);}
 ff_w32(0x800941a8,8);ff_w32(0x80094190,4);ff_w32(object,ff_u32(object)+1u);return 2;
}
/* 1FF08: negative entry health explodes immediately; a decrement below zero
 * instead advances the damage stage. The two paths must remain distinct. */
GDB_CALL sint32 FUN_8001FF08(uint32 obstacle,uint32 object,const uint32 *position,uint32 actor){
 FF_FUNCTION_MARKER(0x8001ff08u,"GAME.EXE");
 uint32 angle=actor?(uint32)(sint32)ff_s16(actor+56):0,kind,x,z,p,v,a,b,d,stage,rx,rz,point[3];
 uint32 *mutable_position=(uint32*)position;int i;
 if(ff_s16(obstacle+16)<0){
  if(ff_s16(object+30)){FUN_800567F8(ff_s16(object+30)-1);ff_w16(object+30,0);}
  ff_w16(obstacle+12,3);ff_w32(object,100);ff_w16(object+32,(uint16)-128);ff_w16(object+22,1);
  object+=36;x=ff_u32(object+4);z=ff_u32(object+12);ff_w32(object,861);
  ff_w16(actor+138,(uint16)((uint16)ff_s16(actor+138)+1u));FUN_80056670(19,255,6144);
  FUN_80031C50(892,x,position[1],z);FUN_80031C50(603,x,0,z);
  for(i=0;i<16;i++){v=ff_rand_8006D348();FUN_80031CD8((v&3u)+902u,ff_u32(object+4),ff_u32(object+8),ff_u32(object+12));}
  ff_w32(0x800941a8,16);ff_w32(0x80094190,8);return 2;
 }
 FUN_8001697C(865,position[0],position[1],position[2]);
 for(i=0;i<8;i++)FUN_80031894(position[0],position[1],position[2],angle);
 kind=ff_u32(object);
 if(position[3])FUN_80056670(23,192,6144);else FUN_80056670(33,255,6144);
 if(kind==96||kind==110){ff_w32(0x8009411c,4);ff_w16(obstacle+16,1);}
 else{
  stage=ff_u32(0x80093d58);
  if(stage==0||stage==7){
   if(!ff_s16(object+30))ff_w16(object+30,(uint16)((uint32)FUN_80056670(107,255,6144)+1u));
   ff_w16(object+32,300);
  }
 }
 a=384;b=768;if(kind==95||kind==109){a=768;b=1536;}
 if(!ff_u32(0x8009411c))ff_w16(object+26,(uint16)a);
 d=ff_u32(0x8009411c);if(d==1){ff_w16(object+22,(uint16)b);d=ff_u32(0x8009411c);}
 if(d==2)ff_w16(object+26,(uint16)(0u-a));
 if(ff_u32(0x8009411c)==3)ff_w16(object+22,(uint16)(0u-b));
 ff_w16(obstacle+16,(uint16)((uint16)ff_s16(obstacle+16)-1u));
 if(ff_s16(obstacle+16)>=0)return 2;
 ff_w32(object,ff_u32(object)+1u);ff_w16(obstacle+16,(uint16)((uint16)ff_s16(obstacle+16)+4u));
 kind=ff_u32(object);angle=(uint32)(sint32)ff_s16(object+16);
 if(kind==95||kind==109){
  FUN_80056670(114,255,6144);x=ff_u32(object+4);z=ff_u32(object+12);
  FUN_80010040();FUN_80011D20();FUN_800101CC(32768);FUN_8001033C((sint32)angle);
  for(i=0;i<40;i++){
   v=ff_rand_8006D348()&255u;rx=(v+(v>>3)-144u)<<16;
   rz=((ff_rand_8006D348()&127u)+(i<24?0xfffffec4u:216u))<<16;
   FUN_800614F4((sint32)rx,0,(sint32)rz,mutable_position);
   FUN_80031B20(870,x+position[0],0xff600000u,z+position[2],0,0,0);
  }
  FUN_80010028();kind=ff_u32(object);
 }
 if(kind==96||kind==110){
  ff_w16(actor+138,(uint16)((uint16)ff_s16(actor+138)+1u));FUN_80056670(115,255,6144);
  x=ff_u32(object+4);z=ff_u32(object+12);
  FUN_80010040();FUN_80011D20();FUN_800101CC(32768);FUN_8001033C((sint32)angle);
  FUN_800614F4(0,0,-17301504,point);FUN_800168BC(534,x+point[0],0,z+point[2],angle);
  for(i=0;i<4;i++){
   FUN_800614F4(i<2?9437184:-9437184,0,(i&1)?-16515072:14155776,point);
   p=0x8009a3c8+36u*(uint32)FUN_800168BC(105,x+point[0],0,z+point[2],angle+(i<2?0xffffc000u:0x4000u));
   /* Original loads stage before the last piece's halfword store. */
   if(i==3)stage=ff_u32(0x80093d58);
   ff_w16(p+18,0x4000);
  }
  if(stage==0||stage==10){
   kind=504;
   if(stage==0){if(ff_u32(object)==110)kind=506;}
   else if((sint32)ff_u32(object+12)<0)kind=503;
   FUN_800614F4(0,0,0x1000000,point);
   p=0x8009a3c8+36u*(uint32)FUN_800167C4(kind,x+point[0],0xffb00000u,z+point[2],angle);
   ff_w16(p+24,70);ff_w16(p+32,96);ff_w16(p+34,1);
  }
  ff_w32(object+36,55);FUN_80010028();
 }
 return 2;
}
/* 205C4: staged object rotation and destruction. Original721 writes the
 * caller's position vector through the matrix transform, despite dispatcher const ABI. */
GDB_CALL sint32 FUN_800205C4(uint32 obstacle,uint32 object,const uint32 *position,uint32 actor){
 FF_FUNCTION_MARKER(0x800205c4u,"GAME.EXE");
 uint32 angle=actor?(uint32)(sint32)ff_s16(actor+56):0,kind,a,b,d,v,x,z,p,point[3],rx,rz;
 uint32 *mutable_position=(uint32*)position;int i;
 FUN_8001697C(865,position[0],position[1],position[2]);
 for(i=0;i<8;i++)FUN_80031894(position[0],position[1],position[2],angle);
 kind=ff_u32(object);
 if(position[3])FUN_80056670(23,192,6144);else FUN_80056670(33,255,6144);
 a=384;b=768;
 if(kind==722){ff_w32(0x8009411c,4);ff_w16(obstacle+16,1);}
 if(kind==721){a=768;b=1536;}
 if(!ff_u32(0x8009411c))ff_w16(object+26,(uint16)a);
 d=ff_u32(0x8009411c);
 if(d==1){ff_w16(object+22,(uint16)b);d=ff_u32(0x8009411c);}
 if(d==2)ff_w16(object+26,(uint16)(0u-a));
 if(ff_u32(0x8009411c)==3)ff_w16(object+22,(uint16)(0u-b));
 ff_w16(obstacle+16,(uint16)((uint16)ff_s16(obstacle+16)-1u));
 if(ff_s16(obstacle+16)>=0)return 2;
 v=ff_u32(0x800944b8);if(v){FUN_800567F8((sint32)(v-1u));ff_w32(0x800944b8,0);}
 ff_w32(object,ff_u32(object)+1u);ff_w16(obstacle+16,(uint16)((uint16)ff_s16(obstacle+16)+4u));
 kind=ff_u32(object);angle=(uint32)(sint32)ff_s16(object+16);
 if(kind==721){
  FUN_80056670(114,255,6144);x=ff_u32(object+4);z=ff_u32(object+12);
  FUN_80010040();FUN_80011D20();FUN_800101CC(32768);FUN_8001033C((sint32)angle);
  for(i=0;i<40;i++){
   v=ff_rand_8006D348()&255u;rx=(v+(v>>3)-144u)<<16;
   rz=((ff_rand_8006D348()&127u)+(i<24?0xfffffec4u:216u))<<16;
   FUN_800614F4((sint32)rx,0,(sint32)rz,mutable_position);
   FUN_80031B20(870,x+position[0],0xff600000u,z+position[2],0,0,0);
  }
  FUN_80010028();
 }
 if(ff_u32(object)==722){
  ff_w16(actor+138,(uint16)((uint16)ff_s16(actor+138)+1u));FUN_80056670(115,255,6144);
  object+=36;x=ff_u32(object+4);z=ff_u32(object+12);ff_w32(object,55);
  FUN_80010040();FUN_80011D20();FUN_800101CC(32768);FUN_8001033C((sint32)angle);
  for(i=0;i<4;i++){
   FUN_800614F4(i<2?9437184:-9437184,0,(i&1)?-15466496:15204352,point);
   p=0x8009a3c8+36u*(uint32)FUN_800168BC(724,x+point[0],0,z+point[2],angle+(i<2?0xffffc000u:0x4000u));
   ff_w16(p+18,0x4000);
  }
  FUN_80010028();
 }
 return 2;
}
/* 21BF8: staged breakable; old649 deliberately falls through into new646. */
GDB_CALL sint32 FUN_80021BF8(uint32 obstacle,uint32 object,const uint32 *position,uint32 actor){
 FF_FUNCTION_MARKER(0x80021bf8u,"GAME.EXE");
 uint32 angle=actor?(uint32)(sint32)ff_s16(actor+56):0,old=ff_u32(object);
 uint32 kind,x,z,dx,dz,y,v,p,h,slot,heading;int i;
 if(old==646||old==674)return 2;
 ff_w16(object+22,16);FUN_8001697C(865,position[0],position[1],position[2]);
 for(i=0;i<8;i++)FUN_80031894(position[0],position[1],position[2],angle);
 if(position[3])FUN_80056670(23,192,6144);else FUN_80056670(33,255,6144);
 FUN_80056670(133,96,6144);
 ff_w16(obstacle+16,(uint16)((uint16)ff_s16(obstacle+16)-1u));
 if(ff_s16(obstacle+16)>=0)return 2;
 v=ff_rand_8006D348();FUN_80056670(114,160,(v&511u)|0x1a00u);
 ff_w32(object,ff_u32(object)+1u);h=(uint16)ff_s16(obstacle+16);
 ff_w16(obstacle+16,(uint16)(h+2u));if(old==672)ff_w16(obstacle+16,(uint16)(h+4u));
 if(old==649){
  ff_w16(actor+138,(uint16)((uint16)ff_s16(actor+138)+1u));ff_w32(object,646);
  for(i=0;i<4;i++){
   v=ff_rand_8006D348();FUN_80031DA8((v&3u)+906u,ff_u32(object+4),0xffc00000u,ff_u32(object+12),(i&1)?640:384);
  }
  v=(ff_rand_8006D348()&8191u)+45056u;
  p=0x8009a3c8+36u*(uint32)FUN_800167C4(527,ff_u32(object+4),0xff900000u,ff_u32(object+12),v);
  ff_w16(p+24,(uint16)((ff_rand_8006D348()&15u)+64u));
  ff_w16(p+32,(uint16)((ff_rand_8006D348()&15u)+96u));
  ff_w16(object+26,(uint16)((ff_rand_8006D348()&1023u)+768u));ff_w16(p+34,1);
 }
 kind=ff_u32(object);x=ff_u32(object+4);z=ff_u32(object+12);
 if(kind==646||kind==674){
  ff_w16(actor+138,(uint16)((uint16)ff_s16(actor+138)+1u));ff_w16(object+22,0);ff_w16(object+26,240);
  for(i=0;i<4;i++){
   v=ff_rand_8006D348();FUN_80031DA8((v&3u)+906u,ff_u32(object+4),0xffc00000u,ff_u32(object+12),(i&1)?640:384);
  }
  for(i=0;i<32;i++){
   dx=((ff_rand_8006D348()&127u)-64u)<<16;dz=((ff_rand_8006D348()&127u)-64u)<<16;
   y=(0xffffffa0u-(ff_rand_8006D348()&127u))<<16;
   FUN_80031B20(870,x+dx,y,z+dz,0,0,0);
  }
  v=(ff_rand_8006D348()&8191u)+45056u;
  p=0x8009a3c8+36u*(uint32)FUN_800167C4(491,ff_u32(object+4),0xffb00000u,ff_u32(object+12),v);
  ff_w16(p+24,(uint16)((ff_rand_8006D348()&15u)+70u));
  ff_w16(p+32,(uint16)((ff_rand_8006D348()&15u)+96u));ff_w16(p+34,1);return 2;
 }
 if(kind==645){
  for(i=0;i<4;i++){
   v=ff_rand_8006D348();FUN_80031DA8((v&3u)+906u,ff_u32(object+4),0xff800000u,ff_u32(object+12),(i&1)?640:384);
  }
 }else if(kind-643u<2u){
  do{
   p=0x8009a3c8+36u*(uint32)FUN_80016654(512,x,0xffb00000u,z);
   ff_w16(p+24,(uint16)((ff_rand_8006D348()&15u)+48u));v=ff_rand_8006D348();slot=ff_u32(0x8008e4f4);
   kind--;ff_w16(p+32,(uint16)((v&15u)+64u));heading=(uint16)ff_s16(0x8008e4f8+2u*slot);
   ff_w16(p+34,1);ff_w32(0x8008e4f4,(slot+1u)&3u);ff_w16(p+16,(uint16)heading);
  }while((sint32)kind>=643);
 }
 for(i=0;i<16;i++){
  dz=((ff_rand_8006D348()&127u)-64u)<<16;
  y=(0xffffff60u-((ff_rand_8006D348()&127u)-64u))<<16;
  FUN_80031B20(870,x-0x600000u,y,z+dz,0,0,0);
  dx=((ff_rand_8006D348()&127u)-64u)<<16;
  y=(0xffffff60u-((ff_rand_8006D348()&127u)-64u))<<16;
  FUN_80031B20(870,x+dx,y,z+0x600000u,0,0,0);
 }
 return 2;
}
/* 21910: debris direction depends on signed X; coordinates reload after every call. */
GDB_CALL sint32 FUN_80021910(uint32 obstacle,uint32 object,const uint32 *position,uint32 actor){
 FF_FUNCTION_MARKER(0x80021910u,"GAME.EXE");
 uint32 angle,kind,x,y,z,v;int i,row,col;
 static const uint32 kinds[4]={130,129,129,131};
 static const uint32 offsets[4]={0xfee00000u,0xffa00000u,0x00600000u,0x01200000u};
 ff_w16(object+22,8);FUN_8001697C(865,position[0],position[1],position[2]);
 FUN_80056670(117,255,6144);FUN_80056670(133,160,6144);
 ff_w16(obstacle+16,(uint16)((uint16)ff_s16(obstacle+16)-1u));
 if(ff_s16(obstacle+16)>=0)return 2;
 ff_w16(obstacle+16,0);ff_w16(obstacle+12,0);ff_w32(object,128);
 ff_w16(object+22,0);ff_w16(object+26,0);
 ff_w16(actor+138,(uint16)((uint16)ff_s16(actor+138)+1u));
 FUN_80056670(116,255,6144);FUN_80056670(19,96,6144);
 x=ff_u32(object+4);ff_w32(0x800941a8,8);ff_w32(0x80094190,4);
 angle=(sint32)x<0?1536u:512u;
 for(i=0;i<12;i++){
  if(i==8)angle^=1024u;
  kind=(ff_rand_8006D348()&3u)+906u;
  y=(0u-(ff_rand_8006D348()&127u))<<16;v=ff_rand_8006D348();
  z=ff_u32(object+12);x=ff_u32(object+4);
  FUN_80031DA8(kind,x,y,z+(((v&511u)-256u)<<16),angle);
 }
 for(row=0;row<2;row++)for(col=0;col<4;col++){
  y=ff_u32(object+8);z=ff_u32(object+12);x=ff_u32(object+4);
  FUN_800168BC(kinds[col],x,y+(row?0xfeec0000u:0xffa40000u),z+offsets[col],0);
 }
 return 2;
}
/* 21628: stage gate precedes damage; preserve captured coordinates across sound calls. */
GDB_CALL sint32 FUN_80021628(uint32 obstacle,uint32 object,const uint32 *position,uint32 actor){
 FF_FUNCTION_MARKER(0x80021628u,"GAME.EXE");
 uint32 stage=ff_u32(0x80093d60),x,z,kind,px,py,counter;int i,row,col;
 static const uint32 offsets[4]={0xfed40000u,0xff9c0000u,0x00640000u,0x012c0000u};
 ff_w16(object+22,8);
 if(stage!=1){ff_w16(obstacle+16,4);FUN_8001697C(866,position[0],position[1],position[2]);return 1;}
 FUN_8001697C(865,position[0],position[1],position[2]);
 FUN_80056670(117,255,6144);FUN_80056670(133,160,6144);
 ff_w16(obstacle+16,(uint16)((uint16)ff_s16(obstacle+16)-1u));
 if(ff_s16(obstacle+16)>=0)return 1;
 ff_w16(obstacle+16,0);ff_w16(obstacle+12,0);x=ff_u32(object+4);
 ff_w32(object,55);ff_w16(object+22,0);ff_w16(object+26,0);
 counter=(uint16)ff_s16(actor+138);z=ff_u32(object+12);ff_w16(actor+138,(uint16)(counter+1u));
 FUN_80056670(116,255,6144);FUN_80056670(19,96,6144);
 ff_w32(0x800941a8,8);ff_w32(0x80094190,4);
 for(i=0;i<12;i++){
  kind=(ff_rand_8006D348()&3u)+906u;
  px=x+(((ff_rand_8006D348()&511u)-256u)<<16);
  py=(0u-(ff_rand_8006D348()&127u))<<16;
  FUN_80031DA8(kind,px,py,z,i<8?0:1024);
 }
 for(row=0;row<2;row++)for(col=0;col<4;col++)
  FUN_800168BC(126u-(uint32)row,x+offsets[col],row?0xfeec0000u:0xffa40000u,z,0);
 return 1;
}
/* 226C4: twelve fragments in two angle groups, then twelve fixed panels.
 * IDA omits the fifth 31DA8 argument: eight use 0, four use 1024. */
GDB_CALL sint32 FUN_800226C4(uint32 obstacle,uint32 object,const uint32 *position,uint32 actor){
 FF_FUNCTION_MARKER(0x800226c4u,"GAME.EXE");
 uint32 x,z,kind,px,py;int i,row,col;
 static const uint32 offsets[4]={0xfe000000u,0xff400000u,0x00c00000u,0x02000000u};
 ff_w16(object+22,8);FUN_8001697C(865,position[0],position[1],position[2]);
 FUN_80056670(133,160,6144);
 ff_w16(obstacle+16,(uint16)((uint16)ff_s16(obstacle+16)-1u));
 if(ff_s16(obstacle+16)>=0)return 1;
 ff_w16(actor+138,(uint16)((uint16)ff_s16(actor+138)+1u));
 FUN_80056670(116,255,6144);FUN_80056670(19,96,6144);
 ff_w16(obstacle+16,0);ff_w16(obstacle+12,0);
 x=ff_u32(object+4);z=ff_u32(object+12);ff_w32(object,55);
 ff_w32(0x800941a8,8);ff_w16(object+22,0);ff_w16(object+26,0);ff_w32(0x80094190,4);
 for(i=0;i<12;i++){
  kind=(ff_rand_8006D348()&3u)+906u;
  px=x+(((ff_rand_8006D348()&511u)-256u)<<16);
  py=(0u-(ff_rand_8006D348()&127u))<<16;
  FUN_80031DA8(kind,px,py,z,i<8?0:1024);
 }
 for(row=0;row<3;row++)for(col=0;col<4;col++)
  FUN_800168BC(758u+4u*(uint32)row+(uint32)col,x,0xfe000000u+0x01000000u*(uint32)row,z+offsets[col],0);
 return 1;
}
GDB_CALL sint32 FUN_80022B18(uint32 obstacle,uint32 object,const uint32 *position,uint32 actor){
 FF_FUNCTION_MARKER(0x80022b18u,"GAME.EXE");
 uint32 p;int i,count=0;(void)obstacle;(void)actor;
 FUN_8001697C(865,position[0],position[1],position[2]);
 for(i=0;i<8;i++)FUN_80031894(position[0],position[1],position[2],512);
 if(position[3])FUN_80056670(23,192,6144);else FUN_80056670(33,255,6144);
 for(i=0,p=0x8009a3c8;i<64;i++,p+=36)if(ff_u32(p)==514)count++;
 if(count<4){
  p=0x8009a3c8+36u*(uint32)FUN_80016654(514,ff_u32(object+4),0xff800000u,ff_u32(object+12)-0x200000u);
  ff_w16(p+24,(uint16)((ff_rand_8006D348()&15u)+48u));ff_w16(p+32,(uint16)((ff_rand_8006D348()&15u)+64u));
  ff_w16(p+16,(uint16)((ff_rand_8006D348()&4095u)-18432u));ff_w16(p+34,1);
 }
 return 2;
}
/* 22EDC: clamp AFTER the original signed16 wrap of four times angle delta. */
GDB_CALL sint32 FUN_80022EDC(uint32 obstacle,uint32 object,const uint32 *position,uint32 actor){
 FF_FUNCTION_MARKER(0x80022edcu,"GAME.EXE");
 uint32 angle,v;sint32 delta;(void)obstacle;if(!actor)return 0;
 FUN_8001697C(865,position[0],position[1],position[2]);
 if(position[3])FUN_80056670(23,192,6144);else FUN_80056670(33,255,6144);
 FUN_80056670(232,255,6144);ff_w16(object+24,40);
 ff_w16(object+28,(uint16)((sint32)((actor-0x800b4318u)*0xc10c9715u)>>2));
 angle=FUN_8001627C(ff_s16(actor+42),ff_s16(actor+50),ff_s16(object+6),ff_s16(object+14));ff_w16(object+32,(uint16)angle);
 v=(angle-(uint16)ff_s16(actor+56))<<2;delta=(sint16)v;ff_w16(object+22,(uint16)v);
 if(delta>512)ff_w16(object+22,512);else if(delta< -512)ff_w16(object+22,(uint16)-512);return 0;
}
/* 229C0: four RNG draws per fragment; 31B20 receives seven actual arguments. */
GDB_CALL sint32 FUN_800229C0(uint32 obstacle,uint32 object,const uint32 *position,uint32 actor){
 FF_FUNCTION_MARKER(0x800229c0u,"GAME.EXE");
 uint32 width,x,z,y,v,age;int i;(void)position;
 ff_w32(object,ff_u32(object)+1u);ff_w16(obstacle+12,3);
 ff_w16(actor+138,(uint16)((uint16)ff_s16(actor+138)+1u));FUN_80056670(113,255,6144);ff_w32(0x80094190,4);
 for(i=0;i<32;i++){
  if((sint32)ff_u32(object)<225){
   width=(uint32)((sint32)ff_s16(obstacle+6)-ff_s16(obstacle+2));x=0;z=width*(ff_rand_8006D348()<<1)-(width<<15);
  }else{
   width=(uint32)((sint32)ff_s16(obstacle+4)-ff_s16(obstacle));z=0;x=width*(ff_rand_8006D348()<<1)-(width<<15);
  }
  v=ff_rand_8006D348();y=(0u-((v&255u)+(ff_rand_8006D348()&63u)))<<16;age=ff_rand_8006D348()&3u;
  FUN_80031B20(871,ff_u32(object+4)+x,y,ff_u32(object+12)+z,(sint32)age,0,0);
 }
 return 1;
}
/* 22FF0: quadrant-dependent impulse and release of the linked object slot. */
GDB_CALL sint32 FUN_80022FF0(uint32 obstacle,uint32 object,const uint32 *position,uint32 actor){
 FF_FUNCTION_MARKER(0x80022ff0u,"GAME.EXE");
 uint32 angle,p;(void)obstacle;if(!actor)return 0;
 FUN_8001697C(865,position[0],position[1],position[2]);FUN_80056670(133,1286,6400);
 if(position[3])FUN_80056670(23,192,6144);else FUN_80056670(33,255,6144);
 angle=((uint32)FUN_8001627C(ff_s16(object+6),ff_s16(object+14),ff_s16(actor+42),ff_s16(actor+50))+256u)&2047u;
 ff_w16(object+24,96);
 if(angle<512){ff_w16(object+26,1);ff_w16(object+22,1023);}
 else if(angle<1024){ff_w16(object+22,1023);ff_w16(object+26,0);}
 else if(angle<1536){ff_w16(object+26,1);ff_w16(object+22,2047);}
 else {ff_w16(object+22,2047);ff_w16(object+26,0);}
 ff_w16(object+28,8192);
 if(ff_s16(object+30)!=-1){
  p=36u*(uint32)(sint32)ff_s16(object+30);ff_w16(0x800b8a0a+p,1);
  p=36u*(uint32)(sint32)ff_s16(object+30);ff_w16(0x800b8a08+p,12);
  p=36u*(uint32)(sint32)ff_s16(object+30);ff_w16(0x800b8a10+p,(uint16)-8);
  p=36u*(uint32)(sint32)ff_s16(object+30);ff_w16(0x800b8a00+p,(uint16)((angle-1280u)<<5));
  p=36u*(uint32)(sint32)ff_s16(object+30);ff_w16(0x800b8a0c+p,(uint16)((sint32)((actor-0x800b4318u)*0xc10c9715u)>>2));
  p=36u*(uint32)(sint32)ff_s16(object+30);ff_w16(0x800b8a0e+p,0);
  p=36u*(uint32)(sint32)ff_s16(object+30);ff_w16(0x800b8a12+p,1);ff_w16(object+30,65535);
 }
 return 0;
}
/* 2322C: rotating drop table, reloaded after each RNG/callee as in MIPS. */
GDB_CALL sint32 FUN_8002322C(uint32 obstacle,uint32 object,const uint32 *position,uint32 actor){
 FF_FUNCTION_MARKER(0x8002322cu,"GAME.EXE");
 uint32 x,z,v,kind,p;int i,count;(void)actor;
 FUN_8001697C(865,position[0],position[1],position[2]);
 if(position[3])FUN_80056670(23,192,6144);else FUN_80056670(33,255,6144);
 x=ff_u32(object+4);z=ff_u32(object+12);ff_w16(obstacle+16,(uint16)((uint16)ff_s16(obstacle+16)-1u));
 if(ff_s16(obstacle+16)>=0)return 2;
 ff_w16(obstacle+16,0);ff_w16(obstacle+12,0);ff_w32(object,55);ff_w16(object+22,0);ff_w16(object+26,0);
 FUN_80056670(19,255,6144);FUN_80031C50(893,x,0xffc00000u,z);
 for(i=0;i<16;i++){v=(ff_rand_8006D348()&3u)+902u;FUN_80031CD8(v,ff_u32(object+4),ff_u32(object+8),ff_u32(object+12));}
 kind=ff_u32(0x8007aeec+4u*ff_u32(0x8008e500));ff_w32(0x800941a8,16);ff_w32(0x80094190,4);count=kind==496?3:1;
 for(i=0;i<count;i++){
  v=(ff_rand_8006D348()&8191u)+45056u;kind=ff_u32(0x8007aeec+4u*ff_u32(0x8008e500));
  p=0x8009a3c8+36u*(uint32)FUN_800167C4((sint32)kind,x+(i==1?0x140000u:0),0xffc00000u,z+(i==2?0x140000u:0),v);ff_w16(p+34,1);
 }
 v=ff_u32(0x8008e500)+1u;ff_w32(0x8008e500,v);if(v==6)ff_w32(0x8008e500,0);return 2;
}
/* 20A60: health is inspected, not decremented; aimed debris uses signed yaw>>5. */
GDB_CALL sint32 FUN_80020A60(uint32 obstacle,uint32 object,const uint32 *position,uint32 actor){
 FF_FUNCTION_MARKER(0x80020a60u,"GAME.EXE");
 uint32 angle=actor?(uint32)(sint32)ff_s16(actor+56):0,kind=ff_u32(object),v;int i;
 if(ff_s16(obstacle+16)>=0){
  FUN_8001697C(866,position[0],position[1],position[2]);
  for(i=0;i<8;i++)FUN_80031894(position[0],position[1],position[2],angle);
  if(position[3])FUN_80056670(23,192,6144);else FUN_80056670(33,255,6144);return 2;
 }
 ff_w16(obstacle+12,3);
 if(kind==78)ff_w32(object,85);if(kind==79)ff_w32(object,86);if(kind==80)ff_w32(object,87);
 if(kind>=81&&kind<=84)ff_w32(object,88);if(kind==776)ff_w32(object,86);if(kind==341)return 2;
 ff_w16(object+32,(uint16)-128);ff_w16(object+22,1);ff_w16(actor+138,(uint16)((uint16)ff_s16(actor+138)+1u));
 FUN_80056670(19,255,6144);FUN_80031C50(892,ff_u32(object+4),position[1],ff_u32(object+12));
 FUN_80031C50(603,ff_u32(object+4),0,ff_u32(object+12));
 for(i=0;i<16;i++){
  v=(ff_rand_8006D348()&3u)+902u;angle=(uint32)(sint32)(ff_s16(object+16)>>5);
  FUN_80031DA8(v,ff_u32(object+4),ff_u32(object+8),ff_u32(object+12),angle);
 }
 ff_w32(0x800941a8,16);ff_w32(0x80094190,8);return 2;
}
GDB_CALL sint32 FUN_80022C60(uint32 obstacle,uint32 object,const uint32 *position,uint32 actor){
 FF_FUNCTION_MARKER(0x80022c60u,"GAME.EXE");
 uint32 p,kind,y,z; sint32 count;int i,row,col;
 ff_w16(object+22,8);FUN_8001697C(865,position[0],position[1],position[2]);FUN_80056670(117,255,6144);
 ff_w16(obstacle+16,(uint16)((uint16)ff_s16(obstacle+16)-1u));if(ff_s16(obstacle+16)>=0)return 2;
 count=(sint32)ff_u32(0x8009403c);ff_w16(actor+138,(uint16)((uint16)ff_s16(actor+138)+1u));
 ff_w16(obstacle+16,0);ff_w16(obstacle+12,0);ff_w32(object,55);ff_w16(object+22,0);ff_w16(object+26,0);
 for(i=0,p=0x800b89f0;i<count;i++,p+=36)if(ff_u32(p)==348){ff_w32(p,349);break;}
 FUN_80056670(19,255,6144);ff_w32(0x800941a8,8);ff_w32(0x80094190,4);
 for(i=0;i<12;i++){
  kind=(ff_rand_8006D348()&3u)+906u;y=(0u-(ff_rand_8006D348()&127u))<<16;z=((ff_rand_8006D348()&511u)-256u)<<16;
  FUN_80031DA8(kind,ff_u32(object+4),y,ff_u32(object+12)+z,512);
 }
 for(row=0;row<3;row++)for(col=0;col<3;col++){
  y=0xfe700000u+(uint32)row*0xa00000u;z=0x1000000u-(uint32)col*0x1000000u;
  FUN_800168BC(350u+3u*(uint32)row+(uint32)col,ff_u32(object+4),y,ff_u32(object+12)+z,0);
 }
 return 2;
}
GDB_CALL sint32 FUN_8002225C(uint32 obstacle,uint32 object,const uint32 *position,uint32 actor){
 FF_FUNCTION_MARKER(0x8002225cu,"GAME.EXE");
 uint32 angle=actor?(uint32)(sint32)ff_s16(actor+56):0,kind=ff_u32(object),x,z,v,p;int i;
 ff_w16(object+22,16);if(kind==656)return 2;
 FUN_8001697C(865,position[0],position[1],position[2]);
 for(i=0;i<8;i++)FUN_80031894(position[0],position[1],position[2],angle);
 if(position[3])FUN_80056670(23,192,6144);else FUN_80056670(33,255,6144);
 x=ff_u32(object+4);z=ff_u32(object+12);ff_w16(obstacle+16,(uint16)((uint16)ff_s16(obstacle+16)-1u));
 if(ff_s16(obstacle+16)>=0)return 2;
 ff_w16(actor+138,(uint16)((uint16)ff_s16(actor+138)+1u));FUN_80056670(19,255,6144);FUN_80031C50(893,x,0xffc00000u,z);
 for(i=0;i<16;i++){v=(ff_rand_8006D348()&3u)+902u;FUN_80031CD8(v,ff_u32(object+4),ff_u32(object+8),ff_u32(object+12));}
 kind=ff_u32(object);v=ff_u32(object+4);ff_w32(0x800941a8,16);ff_w32(0x80094190,4);ff_w32(object,kind+1u);
 if((sint32)v<0){
  angle=(ff_rand_8006D348()&8191u)+12288u;p=0x8009a3c8+36u*(uint32)FUN_800167C4(499,x,0xffc00000u,z,angle);
  ff_w16(p+24,(uint16)((ff_rand_8006D348()&15u)+50u));ff_w16(p+32,(uint16)((ff_rand_8006D348()&15u)+60u));ff_w16(p+34,1);
  angle=(ff_rand_8006D348()&8191u)+12288u;p=0x8009a3c8+36u*(uint32)FUN_800167C4(496,x,0xffb00000u,z,angle);
 }else{
  kind=(uint32)((sint32)ff_rand_8006D348()%3+587);angle=(ff_rand_8006D348()&4095u)+47104u;
  p=0x8009a3c8+36u*(uint32)FUN_800167C4((sint32)kind,x,0xffb00000u,z,angle);
 }
 ff_w16(p+24,(uint16)((ff_rand_8006D348()&15u)+50u));ff_w16(p+32,(uint16)((ff_rand_8006D348()&15u)+60u));ff_w16(p+34,1);return 2;
}
/* 1FA78: staged model damage and kind-specific item drops. */
GDB_CALL sint32 FUN_8001FA78(uint32 obstacle,uint32 object,const uint32 *position,uint32 actor){
 FF_FUNCTION_MARKER(0x8001fa78u,"GAME.EXE");
 uint32 angle=actor?(uint32)(sint32)ff_s16(actor+56):0,kind,p,v;int i;
 FUN_8001697C(865,position[0],position[1],position[2]);
 for(i=0;i<8;i++)FUN_80031894(position[0],position[1],position[2],angle);
 FUN_80056670(112,255,6144);ff_w16(object+22,16);ff_w16(obstacle+16,(uint16)((uint16)ff_s16(obstacle+16)-1u));
 if(ff_s16(obstacle+16)>=0)return 1;
 kind=ff_u32(object);ff_w32(object,kind+1u);
 if(kind==789||kind==792||kind==795||kind==786){ff_w16(obstacle+16,4);FUN_80056670(114,255,6144);return 1;}
 ff_w16(obstacle+12,3);ff_w16(actor+138,(uint16)((uint16)ff_s16(actor+138)+1u));FUN_80056670(19,255,6144);
 FUN_80031C50(893,ff_u32(object+4),0xffc00000u,ff_u32(object+12));ff_w32(0x800941a8,16);ff_w32(0x80094190,4);
 for(i=0;i<8;i++){v=(ff_rand_8006D348()&3u)+902u;FUN_80031CD8(v,ff_u32(object+4),ff_u32(object+8),ff_u32(object+12));}
 ff_w16(object+26,240);ff_w16(object+22,0);
 if(kind==718)for(i=0;i<4;i++){
  angle=ff_rand_8006D348()<<1;p=0x8009a3c8+36u*(uint32)FUN_800167C4(591,ff_u32(object+4),0xff800000u,ff_u32(object+12),angle);
  ff_w16(p+24,(uint16)((ff_rand_8006D348()&15u)+50u));ff_w16(p+32,(uint16)((ff_rand_8006D348()&15u)+60u));ff_w16(p+34,1);
 }
 if(kind==828||kind==830){
  angle=(sint32)ff_u32(object+4)>0?0xffffc000u:0x4000u;
  p=0x8009a3c8+36u*(uint32)FUN_800167C4(505,ff_u32(object+4),0xff000000u,ff_u32(object+12),angle);ff_w16(p+24,32);ff_w16(p+34,1);
 }
 if(kind==784){
  p=0x8009a3c8+36u*(uint32)FUN_800167C4(515,0xf6400000u,0xff100000u,0xf0c00000u,0x4000u);
  object+=36;ff_w16(p+24,64);ff_w16(p+32,64);ff_w16(p+34,1);ff_w32(object,55);
 }
 if(kind==781&&ff_u32(0x800940ac)==object&&ff_s16(object+26)){ff_w16(object+26,0);*(uint8*)ff_ptr(0x80093dd9,1)=0;}return 1;
}
/* 199AC: first containing region, including rotated bounds and local-coordinate globals. */
GDB_CALL sint32 FUN_800199AC(uint32 world_x,uint32 world_z){
 FF_FUNCTION_MARKER(0x800199acu,"GAME.EXE");
 sint32 x=(sint32)world_x>>16,z=(sint32)world_z>>16,i,shape,lx,lz;
 uint32 p=0x8009ec70,object,angle,dx,dz,sine,cosine;
 for(i=0;i<(sint32)ff_u32(0x8009479c);i++,p+=24){
  if(ff_s16(p+12)<2)continue;shape=ff_s16(p+10);
  if(!shape){if(ff_s16(p)<x&&x<ff_s16(p+4)&&ff_s16(p+2)<z&&z<ff_s16(p+6))return i;}
  else if(shape==1){
   object=ff_u32(p+20);angle=2u*((uint16)ff_s16(object+16)>>5);
   dx=(uint32)x-(uint32)(sint32)ff_s16(object+6);dz=(uint32)z-(uint32)(sint32)ff_s16(object+14);
   sine=(uint32)(sint32)ff_s16(0x800802d0+angle);cosine=(uint32)(sint32)ff_s16(0x800806d0+angle);
   lx=(sint32)(dx*cosine+dz*sine)>>15;lz=(sint32)(dz*cosine-dx*sine)>>15;
   ff_w32(0x80094290,(uint32)lx);ff_w32(0x80094294,(uint32)lz);
   if(-ff_s16(p)<lx&&lx<ff_s16(p)&&-ff_s16(p+2)<lz&&lz<ff_s16(p+2))return i;
  }else if((sint32)FUN_800163F4(x,z,ff_s16(p+4),ff_s16(p+6))<ff_s16(p))return i;
 }
 return -1;
}
GDB_CALL sint32 FUN_80019BA8(uint32 world_x,uint32 world_z,uint32 object){
 FF_FUNCTION_MARKER(0x80019ba8u,"GAME.EXE");
 uint32 actor=0x800b4318;int i;
 for(i=0;i<8;i++,actor+=244){
  if(FUN_80048CFC(ff_u32(actor+12)))continue;
  if(ff_s16(object+28)==i||(sint32)(ff_u32(actor+44)+ff_u32(actor+16))< -16777216)continue;
  if(player_byte(actor+120)!=-1||player_byte(actor+122)!=-1||ff_s16(actor+66)<=0)continue;
  if((sint32)FUN_800163F4((sint32)world_x>>16,(sint32)world_z>>16,ff_s16(actor+42),ff_s16(actor+50))<64)return i;
 }
 return -1;
}
GDB_CALL sint32 FUN_800235C4(uint32 actor,uint32 other){
 FF_FUNCTION_MARKER(0x800235c4u,"GAME.EXE");
 uint32 dx=(uint32)((sint32)ff_s16(other+42)-ff_s16(actor+42)),dz=(uint32)((sint32)ff_s16(other+50)-ff_s16(actor+50));
 uint32 angle=2u*(uint32)(sint32)ff_s16(actor+56),sine=(uint32)(sint32)ff_s16(0x800802d0+angle),cosine=(uint32)(sint32)ff_s16(0x800806d0+angle);
 sint32 x=(sint32)(dx*cosine+dz*sine)>>15,z=(sint32)(dz*cosine-dx*sine)>>15;
 ff_w16(actor+90,(uint16)x);ff_w16(actor+92,(uint16)z);return x;
}
GDB_CALL uint32 FUN_80023664(uint32 actor,uint32 other){
 FF_FUNCTION_MARKER(0x80023664u,"GAME.EXE");
 uint32 x=(uint32)(sint32)ff_s16(actor+90),z=(uint32)(sint32)ff_s16(actor+92),angle=2u*((0u-(uint32)(sint32)ff_s16(actor+56))&2047u);
 uint32 sine=(uint32)(sint32)ff_s16(0x800802d0+angle),cosine=(uint32)(sint32)ff_s16(0x800806d0+angle),dx,dz,v;
 dx=(x*cosine+z*sine)<<1;dz=(z*cosine-x*sine)<<1;
 ff_w32(other+40,dx+ff_u32(actor+40));ff_w32(other+48,dz+ff_u32(actor+48));
 v=(uint16)ff_s16(actor+56)^1024u;ff_w16(other+56,(uint16)v);return v;
}
/* 322C8: special item hits stop after one actor; other kinds can hit all eight. */
/* 31F64: radial damage, excluding owner and actors at the exact origin.
 * Independent MIPS audit; shares mechanics with322C8 but has different gates. */
GDB_CALL sint32 FUN_80031F64(uint32 world_x,uint32 world_z,uint32 object){
 FF_FUNCTION_MARKER(0x80031f64u,"GAME.EXE");
 uint32 y=ff_u32(object+8),actor=0x800b4318,kind,animation;int i,j;sint32 radius,type,x=(sint32)world_x>>16,z=(sint32)world_z>>16;
 for(i=0;i<8;i++,actor+=244){
  if(ff_s16(object+28)==i)continue;
  if(ff_s16(actor+80))continue;type=ff_s16(actor+52);if(type==-1||type==17||type==51)continue;
  if(ff_u32(actor+40)==world_x&&ff_u32(actor+48)==world_z)continue;
  if(ff_u32(actor+12)==205||ff_u32(actor+12)==203||ff_s16(actor+66)<=0)continue;
  kind=ff_u32(object);radius=(kind==515||kind==574)?256:512;
  if((sint32)FUN_800163F4(x,z,ff_s16(actor+42),ff_s16(actor+50))>=radius)continue;
  kind=ff_u32(object);
  if(kind==572)FUN_8001F230(ff_s16(object+28),i,-160);
  else if(kind==574)FUN_8001F230(ff_s16(object+28),i,-120);
  else FUN_8001F468(ff_s16(object+28),i,ff_u32(object));
  FUN_8001B850(ff_s16(object+28),i);kind=ff_u32(object);
  if(kind-499u<5u){
   if(ff_u32(actor+12)!=147){
    ff_w16(actor+56,(uint16)(((uint16)ff_s16(object+16)>>5)^1024u));FUN_8001697C(865,world_x,y,world_z);
    for(j=0;j<4;j++)FUN_8003124C(i,world_x,y,world_z,(uint32)(sint32)ff_s16(object+16));
    for(j=0;j<12;j++)FUN_80031164(i,world_x,y,world_z,(uint32)(sint32)ff_s16(object+16));
    FUN_800293D8(i);
    if(ff_s16(actor+74)<49&&ff_s16(actor+66)>0)animation=(ff_rand_8006D348()&1u)+249u;
    else animation=251;
    FUN_80019F00(actor,ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52)),animation);
   }
   return FUN_8001CE14(i);
  }
  ff_w16(actor+56,(uint16)((uint32)FUN_8001627C(x,z,ff_s16(actor+42),ff_s16(actor+50))^1024u));
  FUN_80017E30(i);FUN_80019F00(actor,ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52)),147);FUN_8001CE14(i);
 }
 return 0;
}
GDB_CALL sint32 FUN_800322C8(uint32 world_x,uint32 world_z,uint32 object){
 FF_FUNCTION_MARKER(0x800322c8u,"GAME.EXE");
 uint32 y=ff_u32(object+8),actor=0x800b4318,kind,animation;int i,j;sint32 type,x=(sint32)world_x>>16,z=(sint32)world_z>>16;
 for(i=0;i<8;i++,actor+=244){
  if(ff_s16(actor+80))continue;type=ff_s16(actor+52);if(type==-1||type==17||type==51)continue;
  if(ff_u32(actor+12)==205||ff_u32(actor+12)==203||ff_s16(actor+66)<=0)continue;
  if((sint32)FUN_800163F4(x,z,ff_s16(actor+42),ff_s16(actor+50))>=64)continue;
  FUN_80017C78(i);kind=ff_u32(object);
  if(kind-499u<5u)FUN_8001F230(ff_s16(object+28),i,-60);
  else if(kind-504u<2u||kind==515)FUN_8001F230(ff_s16(object+28),i,-100);
  else FUN_8001F468(ff_s16(object+28),i,ff_u32(object));
  FUN_8001B850(ff_s16(object+28),i);kind=ff_u32(object);
  if(kind-499u<5u||kind-504u<2u||kind==515){
   if(ff_u32(actor+12)!=251){
    ff_w16(actor+56,(uint16)(((uint16)ff_s16(object+16)>>5)^1024u));FUN_8001697C(865,world_x,y,world_z);
    for(j=0;j<4;j++)FUN_8003124C(i,world_x,y,world_z,(uint32)(sint32)ff_s16(object+16));
    for(j=0;j<12;j++)FUN_80031164(i,world_x,y,world_z,(uint32)(sint32)ff_s16(object+16));
    FUN_800293D8(i);ff_w16(actor+74,(uint16)((uint16)ff_s16(actor+74)+32u));if(ff_u32(object)==504)ff_w16(actor+74,127);
    if(ff_s16(actor+74)<49&&ff_s16(actor+66)>0)animation=(ff_rand_8006D348()&1u)+249u;
    else {FUN_80017E30(i);animation=251;}
    FUN_80019F00(actor,ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52)),animation);
   }
   return FUN_8001CE14(i);
  }
  ff_w16(actor+56,(uint16)((uint32)FUN_8001627C(x,z,ff_s16(actor+42),ff_s16(actor+50))^1024u));
  FUN_80017E30(i);FUN_80019F00(actor,ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52)),147);FUN_8001CE14(i);
 }
 return 0;
}
/* 1F518: model-specific destruction and replacement of a collision region. */
GDB_CALL sint32 FUN_8001F518(uint32 obstacle,uint32 object,const uint32 *position,uint32 actor){
 FF_FUNCTION_MARKER(0x8001f518u,"GAME.EXE");
 uint32 kind,v,height,angle;int i;
 ff_w16(object+22,16);kind=ff_u32(object);FUN_8001697C(865,position[0],position[1],position[2]);FUN_80056670(133,160,6144);
 ff_w16(obstacle+16,(uint16)((uint16)ff_s16(obstacle+16)-1u));if(ff_s16(obstacle+16)>=0)return 1;
 ff_w32(object,ff_u32(object)+1u);
 if(kind==295||kind==297){
  ff_w16(obstacle+12,3);ff_w16(actor+138,(uint16)((uint16)ff_s16(actor+138)+1u));FUN_80056670(19,255,6144);
  FUN_80031C50(893,ff_u32(object+4),0xffc00000u,ff_u32(object+12));ff_w32(0x800941a8,16);ff_w32(0x80094190,4);
  for(i=0;i<8;i++){v=(ff_rand_8006D348()&3u)+902u;FUN_80031CD8(v,ff_u32(object+4),0xffc00000u,ff_u32(object+12));}
 }
 if(kind-299u<2u){
  ff_w32(object,301);ff_w16(actor+138,(uint16)((uint16)ff_s16(actor+138)+1u));FUN_80056670(19,192,6144);
  FUN_80031C50(894,ff_u32(object+4),0xff800000u,ff_u32(object+12));ff_w32(actor+40,ff_u32(actor+40)+0x200000u);
  v=(uint32)(sint32)ff_s16(object+6)+32u;ff_w16(obstacle+4,(uint16)v);ff_w16(obstacle,(uint16)v);v=(uint16)ff_s16(object+14);
  ff_w32(0x800941a8,8);ff_w32(0x80094190,4);ff_w16(obstacle+6,(uint16)v);ff_w16(obstacle+2,(uint16)v);
  FUN_8003B77C(obstacle,301);ff_w32(0x8009479c,ff_u32(0x8009479c)-1u);
 }
 if(kind==804||kind==806||kind==808||kind==810||kind==409){
  ff_w16(obstacle+12,3);ff_w16(actor+138,(uint16)((uint16)ff_s16(actor+138)+1u));FUN_80056670(19,192,6144);
  height=(kind==808||kind==810||kind==409)?0xfffffee0u:0xffffff60u;
  FUN_80031C50(894,ff_u32(object+4),height<<16,ff_u32(object+12));angle=(sint32)ff_u32(object+4)>0?512u:1536u;
  for(i=0;i<8;i++){v=(ff_rand_8006D348()&3u)+902u;FUN_80031DA8(v,ff_u32(object+4),height<<15,ff_u32(object+12),angle);}
  ff_w32(0x800941a8,8);ff_w32(0x80094190,4);
 }
 return 1;
}
/* 20C84: compound object damage. The kind63 path overwrites the input XYZ. */
GDB_CALL sint32 FUN_80020C84(uint32 obstacle,uint32 object,const uint32 *position,uint32 actor){
 FF_FUNCTION_MARKER(0x80020c84u,"GAME.EXE");
 uint32 angle=actor?(uint32)(sint32)ff_s16(actor+56):0,kind,v,a,b,x,z,y,age,p,point[3];
 uint32 *scratch=(uint32*)position;sint32 spread;int i;
 if(ff_s16(obstacle+16)<0){
  ff_w16(actor+138,(uint16)((uint16)ff_s16(actor+138)+1u));ff_w16(obstacle+12,3);ff_w32(object,67);ff_w16(object+32,(uint16)-128);ff_w16(object+22,1);
  object+=36;v=(uint32)(sint32)ff_s16(object+30);ff_w32(object,861);FUN_800567F8((sint32)v);FUN_80056670(19,255,6144);
  FUN_80031C50(892,ff_u32(object+4),position[1],ff_u32(object+12));FUN_80031C50(603,ff_u32(object+4),0,ff_u32(object+12));
  for(i=0;i<16;i++){v=(ff_rand_8006D348()&3u)+902u;FUN_80031CD8(v,ff_u32(object+4),ff_u32(object+8),ff_u32(object+12));}
  ff_w32(0x800941a8,16);ff_w32(0x80094190,8);return 2;
 }
 FUN_8001697C(865,position[0],position[1],position[2]);for(i=0;i<8;i++)FUN_80031894(position[0],position[1],position[2],angle);
 kind=ff_u32(object);if(position[3])FUN_80056670(23,192,6144);else FUN_80056670(33,255,6144);
 if(kind==64){ff_w32(0x8009411c,4);ff_w16(obstacle+16,1);}a=kind==63?768u:384u;b=kind==63?1536u:768u;
 if(!ff_u32(0x8009411c))ff_w16(object+26,(uint16)a);
 if(ff_u32(0x8009411c)==1)ff_w16(object+22,(uint16)b);
 if(ff_u32(0x8009411c)==2)ff_w16(object+26,(uint16)(0u-a));
 if(ff_u32(0x8009411c)==3)ff_w16(object+22,(uint16)(0u-b));
 ff_w16(obstacle+16,(uint16)((uint16)ff_s16(obstacle+16)-1u));if(ff_s16(obstacle+16)>=0)return 2;
 kind=ff_u32(object);ff_w32(object,kind==62||kind==66?63:64);ff_w16(obstacle+16,(uint16)((uint16)ff_s16(obstacle+16)+4u));
 kind=ff_u32(object);angle=(uint32)(sint32)ff_s16(object+16);
 if(kind==63){
  FUN_80056670(114,255,6144);x=ff_u32(object+4);z=ff_u32(object+12);
  FUN_80010040();FUN_80011D20();FUN_800101CC(32768);FUN_8001033C((sint32)angle);
  for(i=0;i<24;i++){
   spread=(sint32)(ff_rand_8006D348()&511u)-256;spread+=spread>>2;v=(0xffffff60u-(ff_rand_8006D348()&31u))<<16;
   FUN_800614F4((sint32)v,0,(sint32)((uint32)spread<<16),scratch);
   y=(0xffffff70u-(ff_rand_8006D348()&127u))<<16;age=ff_rand_8006D348()&3u;FUN_80031B20(871,x+position[0],y,z+position[2],(sint32)age,0,0);
   spread=(sint32)(ff_rand_8006D348()&511u)-256;v=((ff_rand_8006D348()&31u)+160u)<<16;
   FUN_800614F4((sint32)v,0,(sint32)((uint32)spread<<16),scratch);
   y=(0xffffff70u-(ff_rand_8006D348()&127u))<<16;age=ff_rand_8006D348()&3u;FUN_80031B20(871,x+position[0],y,z+position[2],(sint32)age,0,0);
  }
  FUN_80010028();kind=ff_u32(object);
 }
 if(kind==64){
  object+=36;ff_w16(actor+138,(uint16)((uint16)ff_s16(actor+138)+1u));FUN_80056670(115,255,6144);
  x=ff_u32(object+4);z=ff_u32(object+12);ff_w32(object,55);FUN_80010040();FUN_80011D20();FUN_800101CC(32768);FUN_8001033C((sint32)angle);
  for(i=0;i<4;i++){
   FUN_800614F4((sint32)(i<2?0x00ac0000u:0xff540000u),0,(sint32)((i&1)?0xfef00000u:0x01200000u),point);
   p=0x8009a3c8+36u*(uint32)FUN_800168BC(69,x+point[0],0,z+point[2],angle+(i<2?0xffffc000u:0x4000u));ff_w16(p+18,0x4000);
  }
  FUN_80010028();
 }
 return 2;
}
/* 23A48: choose a living state252/253 NPC by wrapped angular distance. */
GDB_CALL sint32 FUN_80023A48(sint32 index){
 FF_FUNCTION_MARKER(0x80023a48u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,other,table,delta;
 sint32 type=ff_s16(actor+52),best=-1,best_angle=8192,distance,mode=8;int i;
 table=ff_u32(0x800b3be8+4u*(uint32)type);
 for(i=2;i<8;i++){
  other=0x800b4318+244u*(uint32)i;if(ff_s16(other+66)<=0||ff_u32(other+12)-252u>=2u)continue;
  delta=(uint32)FUN_8001614C(ff_s16(actor+42),ff_s16(actor+50),ff_s16(other+42),ff_s16(other+50));
  delta-=((uint32)(uint16)ff_s16(actor+56)<<5);if((sint32)ff_u32(0x800941a0)>=513)continue;
  if(delta&0x8000u)delta=0u-delta;distance=(sint16)delta;
  if(distance<best_angle){best_angle=distance;best=i;}
 }
 if(best==-1)return -16;
 other=0x800b4318+244u*(uint32)best;
 ff_w16(actor+56,(uint16)FUN_8001627C(ff_s16(actor+42),ff_s16(actor+50),ff_s16(other+42),ff_s16(other+50)));
 ff_w16(actor+90,(uint16)(((sint32)ff_s16(other+42)+ff_s16(actor+42))>>1));
 ff_w16(actor+92,(uint16)(((sint32)ff_s16(other+50)+ff_s16(actor+50))>>1));
 ff_w16(actor+94,(uint16)FUN_80016478(ff_s16(actor+42),ff_s16(actor+50),ff_s16(other+42),ff_s16(other+50)));
 if(type==0)mode=9;else if(type==1)mode=15;else if(type==2)mode=13;else if(type==3)mode=14;
 ff_w16(actor+96,(uint16)mode);return FUN_800237CC(actor,table,65);
}
/* 1F004: retained_word models entry-SP-12, not an original register argument.
 * Original initializes only XYZ at SP+16..24; callbacks can read SP+28. */
GDB_CALL sint32 FUN_8001F004(sint32 index,uint32 projectile,uint32 retained_word){
 FF_FUNCTION_MARKER(0x8001f004u,"GAME.EXE");
 uint32 obstacle=0x8009ec70+24u*(uint32)index,kind=ff_u32(projectile),object=ff_u32(obstacle+20),point[4];
 if(kind==17)return 51;if(kind==51)return (sint32)(kind-499u);
 if(kind-499u>=5u&&kind!=504)ff_w16(obstacle+16,0xffff);
 point[0]=ff_u32(projectile+4);point[1]=ff_u32(projectile+8);point[2]=ff_u32(projectile+12);point[3]=retained_word;
 return ff_collision_dispatch(ff_u32(0x80094040+4u*(uint32)(sint32)ff_s16(obstacle+12)),obstacle,object,point,0);
}
/* 2FD90: 64 ray steps; entry-SP-12 is saved S5 from199AC or S3 from31C50. */
GDB_CALL sint32 FUN_8002FD90(uint32 projectile,sint32 index){
 FF_FUNCTION_MARKER(0x8002fd90u,"GAME.EXE");
 uint32 angle=(uint16)ff_s16(projectile+16)>>5;
 uint32 dx=(uint32)(sint32)ff_s16(0x800802d0+2u*angle)<<7;
 uint32 dz=(uint32)(sint32)ff_s16(0x800806d0+2u*angle)<<7;
 uint32 x=ff_u32(projectile+4),z=ff_u32(projectile+12),kind,retained;int i;sint32 hit;
 for(i=0;i<64;i++){
  hit=FUN_800199AC((sint32)x,(sint32)z);
  if(hit>=0){
   kind=ff_u32(projectile);retained=dz;
   if(kind-499u>=5u&&kind-504u>=2u){
    FUN_80031C50(894,ff_u32(projectile+4),ff_u32(projectile+8),ff_u32(projectile+12));retained=projectile;
   }
   FUN_8001F004(hit,projectile,retained);return 1;
  }
  hit=FUN_80019BA8((sint32)x,(sint32)z,projectile);
  if(hit>=0&&hit!=index){
   kind=ff_u32(projectile);
   if(kind-499u>=5u&&kind-504u>=2u)FUN_80031C50(894,x,ff_u32(projectile+8),z);
   FUN_800322C8(x,z,projectile);return 1;
  }
  x+=dx;z-=dz;ff_w32(projectile+4,x);ff_w32(projectile+12,z);
 }
 return 0;
}
/* Local projectile adapter: disjoint from existing native RAM temporaries.
 * Only original initialized fields are written; ray callees read kind/XYZ/yaw/owner.
 * Save/restore all bytes so this native stack substitute leaves no RAM residue. */
GDB_CALL sint32 FUN_80030310(sint32 index){
 FF_FUNCTION_MARKER(0x80030310u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,held,point[3],temp=0x801fc300,saved[9];int i;
 sint32 slot=player_byte(actor+126);
 if(slot==-1){slot=player_byte(actor+125);if(slot==-1)return 0;}
 held=0x8009a3c8+36u*(uint32)slot;
 FUN_80056670(19,160,7168);FUN_8001B9F4(index,ff_s16(actor+52)==2?2:3,point);
 for(i=0;i<9;i++)saved[i]=ff_u32(temp+4u*i);
 ff_w32(temp,ff_u32(held));ff_w16(temp+28,(uint16)index);
 ff_w32(temp+4,ff_u32(actor+40));ff_w32(temp+12,ff_u32(actor+48));ff_w32(temp+8,point[1]);
 ff_w16(temp+16,(uint16)((uint32)(uint16)ff_s16(actor+56)<<5));
 FUN_8002FD90(temp,index);
 for(i=0;i<9;i++)ff_w32(temp+4u*i,saved[i]);return 0;
}
GDB_CALL sint32 FUN_80030440(sint32 index){
 FF_FUNCTION_MARKER(0x80030440u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,held,kind,point[3],temp=0x801fc300,saved[9];int i;
 sint32 slot=player_byte(actor+126);if(slot==-1)slot=player_byte(actor+125);
 held=0x8009a3c8+36u*(uint32)slot;
 if(ff_u32(held)==504)FUN_80056670(27,255,5376);else FUN_80056670(24,255,6144);
 FUN_80056670(19,128,8192);FUN_8001B9F4(index,3,point);
 for(i=0;i<9;i++)saved[i]=ff_u32(temp+4u*i);
 ff_w16(temp+28,(uint16)index);kind=ff_u32(held);
 ff_w32(temp,kind-499u<5u||kind-504u<2u?ff_u32(held):499u);
 ff_w32(temp+4,ff_u32(actor+40));ff_w32(temp+12,ff_u32(actor+48));ff_w32(temp+8,point[1]);
 ff_w16(temp+16,(uint16)((uint32)(uint16)ff_s16(actor+56)<<5));
 if(!FUN_8002FD90(temp,index)){
  ff_w16(temp+16,(uint16)((uint16)ff_s16(temp+16)-384u));
  ff_w32(temp+4,ff_u32(actor+40));ff_w32(temp+12,ff_u32(actor+48));
  if(!FUN_8002FD90(temp,index)){
   ff_w16(temp+16,(uint16)((uint16)ff_s16(temp+16)+768u));
   ff_w32(temp+4,ff_u32(actor+40));ff_w32(temp+12,ff_u32(actor+48));FUN_8002FD90(temp,index);
  }
 }
 for(i=0;i<9;i++)ff_w32(temp+4u*i,saved[i]);return 0;
}
/* Native address dispatch for table94040; unknown real callees remain WIP. */
GDB_CALL sint32 ff_collision_dispatch(uint32 target,uint32 obstacle,uint32 object,const uint32 *position,uint32 actor){
 if(ff_services.collision_event)return ff_services.collision_event(target,obstacle,object,position,actor);
 if(target==0x8001f510)return FUN_8001F510();
 if(target==0x80021220)return FUN_80021220(obstacle,object,position,actor);
 if(target==0x8001f7ec)return FUN_8001F7EC(obstacle,object,position,actor);
 if(target==0x800213f8)return FUN_800213F8(obstacle,object,position,actor);
 if(target==0x8001fda0)return FUN_8001FDA0(obstacle,object,position,actor);
 if(target==0x80022548)return FUN_80022548(obstacle,object,position,actor);
 if(target==0x800226c4)return FUN_800226C4(obstacle,object,position,actor);
 if(target==0x80021628)return FUN_80021628(obstacle,object,position,actor);
 if(target==0x80021910)return FUN_80021910(obstacle,object,position,actor);
 if(target==0x80021bf8)return FUN_80021BF8(obstacle,object,position,actor);
 if(target==0x800205c4)return FUN_800205C4(obstacle,object,position,actor);
 if(target==0x8001ff08)return FUN_8001FF08(obstacle,object,position,actor);
 if(target==0x80022b18)return FUN_80022B18(obstacle,object,position,actor);
 if(target==0x80022edc)return FUN_80022EDC(obstacle,object,position,actor);
 if(target==0x800229c0)return FUN_800229C0(obstacle,object,position,actor);
 if(target==0x80022ff0)return FUN_80022FF0(obstacle,object,position,actor);
 if(target==0x8002322c)return FUN_8002322C(obstacle,object,position,actor);
 if(target==0x80020a60)return FUN_80020A60(obstacle,object,position,actor);
 if(target==0x80022c60)return FUN_80022C60(obstacle,object,position,actor);
 if(target==0x8002225c)return FUN_8002225C(obstacle,object,position,actor);
 if(target==0x8001fa78)return FUN_8001FA78(obstacle,object,position,actor);
 if(target==0x8001f518)return FUN_8001F518(obstacle,object,position,actor);
 if(target==0x80020c84)return FUN_80020C84(obstacle,object,position,actor);
 player_wip_zero(target);return 0;
}
/* 1BBE8: strict contact bounds and first matching stage callback. */
GDB_CALL sint32 FUN_8001BBE8(uint32 position[4],sint32 index) {
 uint32 actor=0x800b4318+244u*(uint32)index,p,object,angle; sint32 i,type,shape,x=(sint32)position[0]>>16,z=(sint32)position[2]>>16;
 if((uint16)ff_s16(actor+82)&0x8000u)return 0;
 for(i=0;i<(sint32)ff_u32(0x8009479c);i++){
  p=0x8009ec70+24u*(uint32)i;type=ff_s16(p+12);if((type<3&&type!=1)||ff_s16(p+16)<0)continue;
  shape=ff_s16(p+10);
  if(!shape){if(!(ff_s16(p)<x&&x<ff_s16(p+4)&&ff_s16(p+2)<z&&z<ff_s16(p+6)))continue;}
  else if(shape==1){
   uint32 dx,dz,sine,cosine; sint32 local_x,local_z;
   object=ff_u32(p+20);angle=(uint16)ff_s16(object+16)>>5;
   dx=(uint32)x-(uint32)(sint32)ff_s16(object+6);dz=(uint32)z-(uint32)(sint32)ff_s16(object+14);
   sine=(uint32)(sint32)ff_s16(0x800802d0+2u*angle);cosine=(uint32)(sint32)ff_s16(0x800806d0+2u*angle);
   local_x=(sint32)(dx*cosine+dz*sine)>>15;local_z=(sint32)(dz*cosine-dx*sine)>>15;
   if(local_x<=-ff_s16(p)||local_x>=ff_s16(p)||local_z<=-ff_s16(p+2)||local_z>=ff_s16(p+2))continue;
   {
    sint32 a=(sint16)FUN_8001627C(local_x,local_z,0,0);
    sint32 b=(sint16)FUN_8001627C(0,0,ff_s16(p),-ff_s16(p+2)),side;
    if(a<b||a>2048-b)side=0;else if(a<1024-b)side=1;else side=a<b+1024?2:3;
    ff_w32(0x8009411c,(uint32)side);
   }
  }else if(FUN_800163F4(x,z,ff_s16(p+4),ff_s16(p+6))>=ff_s16(p))continue;
  object=ff_u32(p+20);ff_w16(actor+82,(uint16)((uint16)ff_s16(actor+82)|0x8000u));
  return ff_collision_dispatch(ff_u32(0x80094040+4u*(uint32)(sint32)ff_s16(p+12)),p,object,position,actor);
 }
 return 0;
}
/* 1D10C: weapon endpoint against the stage, then NPC slots2..7.
 * Positive NPC contact continues at1D2D4 and remains an explicit WIP. */
/* 1F468: weapon damage selector; range guards and skipped-call return are MIPS V0. */
GDB_CALL uint32 FUN_8001F468(sint32 attacker,sint32 victim,uint32 kind) {
 sint32 damage;
 if(kind==572||kind==574||kind==604)damage=64;
 else if(kind==695||kind-708u<2u||kind==710||kind==408)damage=32;
 else {
  if(kind-476u>=88u||(uint32)attacker>=9u||(uint32)victim>=9u)return 0;
  damage=ff_s16(0x800846ba+4u*kind);
 }
 if(damage<0)damage=0;
 return FUN_8001F230(attacker,victim,damage);
}
GDB_CALL sint32 FUN_8001D10C(sint32 index,sint32 half) {
 uint32 actor=0x800b4318+244u*(uint32)index,point[4],item,kind,x,y,z;
 sint32 result,i,type,distance;uint32 other;
 item=0x8009a3c8+36u*(uint32)player_byte(actor+126);
 kind=ff_u32(item);FUN_8001CF08(index,half,point);x=point[0];y=point[1];z=point[2];point[3]=1;result=FUN_8001BBE8(point,index);
 if(result) {
  ff_w16(item+22,(uint16)(ff_s16(item+22)-1));
  if(ff_s16(item+22)<=0){FUN_80017E30(index);ff_w16(item+22,65535);}
  return result;
 }
 for(i=2;i<8;i++) {
  if(i==index)continue;
  other=0x800b4318+244u*(uint32)i;type=ff_s16(other+52);
  if(type==-1||ff_s16(other+80)||type==17||type==51)continue;
  if(((sint32)ff_s16(actor+82)>>i)&1)continue;
  if(FUN_80048CFC(ff_u32(other+12)))continue;
  if((sint32)ff_u32(other+44)<0)continue;
  distance=FUN_800163F4((sint32)point[0]>>16,(sint32)point[2]>>16,ff_s16(other+42),ff_s16(other+50));
  ff_w32(0x800941a0,(uint32)distance);
  if(distance<ff_s16(0x80094120+2u*(uint32)(sint32)ff_s16(other+52))) {
   uint32 table=ff_u32(0x800b3be8+4u*(uint32)type),angle; sint16 direction;int j;
   sint16 bearing=(sint16)FUN_8001614C(ff_s16(other+42),ff_s16(other+50),ff_s16(actor+42),ff_s16(actor+50));
   angle=(uint32)(sint32)ff_s16(other+56);
   direction=(sint16)((angle<<5)-(uint32)(sint32)bearing-0x4000u);
   FUN_8001697C(865,x,y,z);FUN_800293D8(i);FUN_80017C78(i);FUN_80056670(23,255,6144);
   ff_w16(item+22,(uint16)(ff_s16(item+22)-1));angle^=0x400u;
   if(ff_s16(item+22)<=0){FUN_80017E30(index);ff_w16(item+22,65535);}
   if(kind==478||kind==480||kind==482)player_wip_zero(0x8001d388);
   ff_w8(other+129,(uint8)index);
   for(j=0;j<8;j++)FUN_8003124C(i,x,y,z,angle);
   for(j=0;j<32;j++)FUN_80031164(i,x,y,z,angle);
   FUN_8001F468(index,i,kind);
   if(ff_s16(other+66)<=0){FUN_8001B8DC(i);FUN_80019F00(other,table,147);ff_w16(other+66,0);}
   else {
    ff_w16(actor+82,(uint16)((uint16)ff_s16(actor+82)|(1u<<i)));
    if(ff_s16(other+74)>=49){FUN_80019F00(other,table,direction<0?147:154);ff_w16(other+74,0);FUN_80017E30(i);}
    else FUN_80019F00(other,table,direction<0?133:((ff_rand_8006D348()&1u)|152u));
   }
   FUN_8001B850(index,i);FUN_8001CE14(i);return 3;
  }
 }
 return 0;
}
/* 1BFA0: projectile contact scan; positive actor contact remains1C130 WIP. */
GDB_CALL sint32 FUN_8001BFA0(sint32 owner,uint32 point[4],uint32 kind) {
 sint32 i,type;uint32 other,state,x=point[0],y=point[1],z=point[2];
 point[3]=0;
 if(FUN_8001BBE8(point,owner))return 2;
 for(i=0;i<8;i++) {
  if(ff_u32(0x80093d5c)==2&&i>=2)return 0;
  if(i==owner)continue;
  other=0x800b4318+244u*(uint32)i;type=ff_s16(other+52);
  if(type==-1||ff_s16(other+80)||type==17||type==51)continue;
  if((sint32)(ff_u32(other+44)+ff_u32(other+16))<-16777216)continue;
  if(FUN_80048CFC(ff_u32(other+12)))continue;
  FUN_8001614C((sint32)x>>16,(sint32)z>>16,ff_s16(other+42),ff_s16(other+50));
  if(ff_s16(0x80094120+2u*(uint32)(sint32)ff_s16(other+52))<(sint32)ff_u32(0x800941a0))continue;
  state=ff_u32(other+12);
  if(state==253||state==156||state==252||state==155||state==147||state==154||state==191||state-149u<2u||state==148)continue;
  {
   uint32 angle=(uint32)(sint32)ff_s16(other+56)<<5,table;int j;
   FUN_80056670(76,255,6144);FUN_800293D8(i);FUN_80017C78(i);
   ff_w8(other+129,(uint8)owner);FUN_8001697C(865,x,y,z);
   for(j=0;j<4;j++)FUN_8003124C(i,x,y,z,angle);
   for(j=0;j<12;j++)FUN_80031164(i,x,y,z,angle);
   FUN_8001F468(owner,i,kind);
   if(ff_s16(other+66)<=0) {
    FUN_8001B8DC(i);table=ff_u32(0x800b3be8+4u*(uint32)type);
    FUN_80019F00(other,table,147);FUN_8001CE14(i);ff_w16(other+66,0);
   } else {
    if((sint32)kind>=522||kind==695||kind-708u<2u||kind==710||kind==408)player_wip_zero(0x8001c250);
    table=ff_u32(0x800b3be8+4u*(uint32)type);FUN_80019F00(other,table,133);
   }
   FUN_8001CE14(i);if((sint32)ff_u32(0x80093d5c)<2)FUN_8001B850(owner,i);
   return 1;
  }
 }
 return 0;
}
/* 1CE14: detach paired actors of types44..46 before animation281. */
GDB_CALL sint32 FUN_8001CE14(sint32 index){
 uint32 actor=0x800b4318+244u*(uint32)index; sint32 type=ff_s16(actor+52),partner;
 if((uint32)(uint16)type-44u>=2u&&type!=46)return 0;
 partner=ff_s16(actor+88);if(partner==-1)return 0;
 ff_w16(actor+88,65535);ff_w16(0x800b4370+244u*(uint32)partner,65535);
 FUN_80019F00(actor,ff_u32(0x800b3be8+4u*(uint32)type),281);return 1;
}
/* 1B8DC: HP is cleared even when death has already been accounted for.
 * LB attribution is compared unsigned with2; absent partner returns -16. */
GDB_CALL sint32 FUN_8001B8DC(sint32 index){
 uint32 actor=0x800b4318+244u*(uint32)index,score; sint32 result=player_byte(actor+119),credit,partner;
 ff_w16(actor+66,0);if(result)return result;
 FUN_80029530(index);FUN_80017E30(index);ff_w8(actor+126,255);ff_w8(actor+125,255);
 if(index<2)return 1;
 ff_w32(0x80093dbc,ff_u32(0x80093dbc)-1u);credit=player_byte(actor+129);
 if((uint32)credit<2u){score=0x80094030+4u*(uint32)credit;ff_w32(score,ff_u32(score)+ff_u32(0x800a8600+4u*(uint32)(sint32)ff_s16(actor+52)));FUN_8005E9E8(credit);}
 partner=player_byte(actor+120);ff_w8(actor+120,255);ff_w8(actor+119,(uint8)(*(uint8*)ff_ptr(actor+119,1)+1u));
 if(partner==-1)return -16;
 ff_w8(0x800b4392+244u*(uint32)partner,255);return (sint32)(244u*(uint32)partner);
}
GDB_CALL sint32 FUN_8001B850(sint32 index,sint32 victim){
 uint32 actor=0x800b4318+244u*(uint32)index;
 if(victim<2&&ff_u32(0x80093d5c)!=2)return 2;
 if(ff_s16(actor+66)<=0)FUN_8001B8DC(index);
 ff_w8(actor+116,(uint8)victim);ff_w8(actor+118,60);return 60;
}
/* 1CB08..1CD90: ordinary hit reaction, including attack-specific displacement. */
static void player_contact_reaction(uint32 victim,uint32 table,uint32 actor,sint32 direction) {
 uint32 attack=ff_u32(actor+12),yaw; sint32 animation=133,scale=0;
 if(direction>=0){FUN_80019F00(victim,table,152);return;}
 switch(attack) {
 case 11:
  animation=139;
  if(table==0x80082a08||table==0x80082e8c||table==0x80083794||table==0x800849a4)scale=24;
  else scale=table==0x80084520?64:-64;
  break;
 case 14:animation=145;break;
 case 15:animation=146;break;
 case 20:animation=143;break;
 case 21:animation=144;break;
 case 40:animation=139;scale=-112;break;
 default:break; /* attack2 and the default both select133. */
 }
 FUN_80019F00(victim,table,animation);
 if(scale) {
  yaw=(uint32)(sint32)ff_s16(victim+56);
  ff_w32(victim+40,ff_u32(victim+40)+(uint32)((sint32)ff_s16(0x800802d0+2u*yaw)*scale));
  ff_w32(victim+48,ff_u32(victim+48)-(uint32)((sint32)ff_s16(0x800806d0+2u*yaw)*scale));
 }
}
/* 1C864..1C8D8: extra paired finisher effects before the ordinary HP reaction. */
static void player_finisher_contact(uint32 victim,sint32 index,uint32 x,uint32 y,uint32 z,uint32 angle) {
 sint32 j;
 for(j=0;j<12;j++){FUN_8003124C(index,x,y,z,angle);FUN_80031164(index,x,y,z,angle);}
 ff_w32(0x80094190,4);ff_w32(0x800941a8,8);ff_w16(victim+74,127);
 FUN_80056670(78,255,5632);
}
/* 1C35C: contact, defense, damage and reaction. */
GDB_CALL sint32 FUN_8001C35C(sint32 index,uint32 position[4]) {
 uint32 hit_x=position[0],hit_y=position[1],hit_z=position[2];
 uint32 actor=0x800b4318+244u*(uint32)index,victim,table,angle; sint32 direction,blocked,j,i,type,state,result,x=(sint32)hit_x>>16,z=(sint32)hit_z>>16;
 position[3]=0;result=FUN_8001BBE8(position,index);if(result)return result;
 for(i=0;i<8;i++){
  if((!ff_u32(0x80093d44)&&i<2)||i==index)continue;
  victim=0x800b4318+244u*(uint32)i;type=ff_s16(victim+52);
  if(type==-1||ff_s16(victim+66)<=0||type==17||type==51||(sint32)ff_u32(victim+44)<0||ff_s16(victim+80)||((ff_s16(actor+82)>>i)&1)||FUN_80048CFC(ff_u32(victim+12)))continue;
  FUN_8001614C(x,z,ff_s16(victim+42),ff_s16(victim+50));
  if(ff_s16(0x80094120+2u*(uint32)type)<(sint32)ff_u32(0x800941a0))continue;
  table=ff_u32(0x800b3be8+4u*(uint32)type);
  angle=(uint32)(sint32)(sint16)FUN_8001614C(ff_s16(victim+42),ff_s16(victim+50),ff_s16(actor+42),ff_s16(actor+50));
  direction=(sint16)(((uint32)(uint16)ff_s16(victim+56)<<5)-angle-0x4000u);
  state=i<2?(sint32)ff_u32(victim+12):FUN_8004C9B0(victim);
  if((uint32)state-147u<2u||state==154||state==191)continue;
  ff_w16(actor+82,(uint16)(ff_s16(actor+82)|(1u<<i)));ff_w8(victim+129,(uint8)index);blocked=0;
  if(state==149||state==151){FUN_8001697C(866,hit_x,hit_y,hit_z);blocked=16;}
  if((uint32)state-230u<2u||state==232){FUN_8001697C(866,hit_x,hit_y,hit_z);blocked=16;}
  if(ff_u32(victim+12)==67&&(state==208||(uint32)state-209u<2u)){FUN_8001697C(866,hit_x,hit_y,hit_z);blocked=16;}
  if(state==57||(uint32)state-59u<2u){
   FUN_8001697C(866,hit_x,hit_y,hit_z);blocked=16;
   if(state!=59||ff_u32(victim+4)==0xffffffffu)FUN_80019F00(victim,table,59);
  }
  if(blocked)FUN_8001F230(index,i,blocked);
  else{
   FUN_800293D8(i);FUN_80017C78(i);
   ff_w32(victim+176,ff_u32(0x80093dd0)+*(uint8*)ff_ptr(ff_u32(victim+180)+8u*ff_u32(victim+184)+1,1));
   FUN_8001697C(865,hit_x,hit_y,hit_z);
   for(j=0;j<4;j++)FUN_8003124C(i,hit_x,hit_y,hit_z,angle);
   for(j=0;j<12;j++)FUN_80031164(i,hit_x,hit_y,hit_z,angle);
   FUN_8001F41C((uint32)index,(uint32)i,ff_u32(actor+12));
  }
  /* 1C78C..1C850: special-attack hit count, bonus label and score. */
  if(i>=2&&ff_u32(actor+12)==33) {
   uint32 score=0x80094030+4u*(uint32)index;
   ff_w8(actor+127,(uint8)(*(uint8*)ff_ptr(actor+127,1)+1u));
   if(player_byte(actor+127)==2) {
    FUN_8001697C(862,ff_u32(victim+40),0xff400000u,ff_u32(victim+48));
    ff_w32(score,ff_u32(score)+50u);FUN_8005E9E8(index);
   }
   if(player_byte(actor+127)==3) {
    FUN_8001697C(863,ff_u32(victim+40),0xff400000u,ff_u32(victim+48));
    ff_w32(score,ff_u32(score)+100u);FUN_8005E9E8(index);
   }
   if(player_byte(actor+127)==4) {
    FUN_8001697C(864,ff_u32(victim+40),0xff400000u,ff_u32(victim+48));
    ff_w32(score,ff_u32(score)+250u);FUN_8005E9E8(index);
   }
  }
  if(ff_u32(actor+12)==34)player_finisher_contact(victim,i,hit_x,hit_y,hit_z,angle);
  if(ff_s16(victim+66)<=0){
   FUN_8001B8DC(i);
   FUN_80019F00(victim,table,state==253||state==156?156:state==252||state==155?155:direction<0?147:154);
   ff_w16(victim+66,0);FUN_8001CE14(i);
  }else if(!blocked){
   ff_w16(actor+82,(uint16)(ff_s16(actor+82)|(1u<<i)));
   if(state==253||state==156){ff_w16(victim+74,0);FUN_80019F00(victim,table,156);}
   else if(state==252||state==155){ff_w16(victim+74,0);FUN_80019F00(victim,table,155);}
   else if((uint32)state-141u<2u)ff_w16(victim+74,0);
   else if(ff_s16(victim+74)<49&&(uint32)ff_u32(actor+12)-66u>=2u){
    if(player_byte(victim+120)==-1&&ff_s16(victim+66)<32&&ff_s16(victim+74)>=2&&FUN_8001A068(table,234)&&FUN_8001A068(table,235)){
     FUN_80017E30(i);FUN_80019F00(victim,table,direction<0?234:235);ff_w32(0x800940a8,ff_u32(0x80093dd0)+60u);
    }else player_contact_reaction(victim,table,actor,direction);
   }else if(player_byte(victim+120)==-1){FUN_80017E30(i);ff_w16(victim+74,0);FUN_80019F00(victim,table,direction<0?147:154);}
   else player_contact_reaction(victim,table,actor,direction);
   FUN_8001CE14(i);
  }
  if((sint32)ff_u32(0x80093d5c)<2)FUN_8001B850(index,i);
  return 1;
 }
 return 0;
}
GDB_CALL sint32 FUN_8001CED0(sint32 index,sint32 attachment){
 uint32 position[4];FUN_8001B9F4(index,attachment,position);return FUN_8001C35C(index,position);
}
GDB_CALL sint32 FUN_80057CA4(sint32 resource) {
 uint32 p=ff_u32(ff_u32(0x80081720+4u*(uint32)resource)),i;
 for(i=0;i<18;i++,p+=8){ff_w16(p,0);ff_w16(p+2,0);ff_w16(p+4,0);}return 0;
}
/* 32F38: attack animation callback, sample selection or attachment reset. */
GDB_CALL sint32 FUN_80032F38(sint32 index) {
 uint32 actor=0x800b4318+244u*(uint32)index;sint32 type=ff_s16(actor+52);
 if(ff_u32(actor+28)&0x8000u){
  if(index>=2)return 1;
  ff_w16(actor+98,1);ff_w16(actor+100,2);return FUN_80057CA4(index+867);
 }
 if(type==0||type==2)return FUN_80056670(48,80,6144);
 if(type==1||type==3)return FUN_80056670(70,160,6144);
 if(type==49)return FUN_80056670(169,160,6144);
 return 49;
}
/* 330CC: same attachment reset as32F38, fixed sample22 on the other event. */
GDB_CALL sint32 FUN_800330CC(sint32 index) {
 uint32 actor=0x800b4318+244u*(uint32)index;
 if(ff_u32(actor+28)&0x8000u) {
  if(index>=2)return 1;
  ff_w16(actor+98,1);ff_w16(actor+100,2);return FUN_80057CA4(index+867);
 }
 return FUN_80056670(22,255,6144);
}
/* 2976C: character voice sample and one RNG draw for pitch. */
GDB_CALL sint32 FUN_8002976C(sint32 index) {
 sint32 type=ff_s16(0x800b434c+244u*(uint32)index),sample=38;
 switch(type) {
  case 1:case 12:case 29:case 30:case 38:sample=62;break;
  case 2:case 14:case 32:case 49:sample=79;break;
  case 3:sample=98;break;case 48:sample=189;break;
 }
 return FUN_80056670(sample,255,6144-(sint32)(ff_rand_8006D348()&255u));
}
/* 29854/32EE0: landing voice, then ground effect607. */
GDB_CALL sint32 FUN_80029854(sint32 index) {
 sint32 type=ff_s16(0x800b434c+244u*(uint32)index),sample=39;
 switch(type) {
  case 1:case 3:case 12:case 29:case 30:case 38:sample=63;break;
  case 2:case 14:sample=80;break;case 48:sample=197;break;
  case 32:sample=165;break;case 49:sample=173;break;
 }
 return FUN_80056670(sample,255,6144-(sint32)(ff_rand_8006D348()&255u));
}
/* 33374: running attack sound and ground effect; exactly two offset RNG draws. */
GDB_CALL uint32 FUN_80033374(sint32 index){
 uint32 actor=0x800b4318+244u*(uint32)index,x,z;
 if(!ff_s16(actor+84)) {
  if(ff_s16(actor+52)==2){ff_w16(actor+136,(uint16)(FUN_80056670(91,80,6144)+1));FUN_80056670(92,255,6144);}
  else FUN_80056670(56,255,6144);
 }
 if(ff_u32(actor+28)&2048u){FUN_800567F8(ff_s16(actor+136)-1);ff_w16(actor+136,0);FUN_80056670(92,255,6144);}
 x=(( (ff_rand_8006D348()&31u)-16u)<<16)+ff_u32(actor+40);
 z=(( (ff_rand_8006D348()&31u)-16u)<<16)+ff_u32(actor+48);
 return FUN_80031B20(871,x,0,z,0,0,0);
}
GDB_CALL uint32 FUN_80032EE0(sint32 index) {
 uint32 actor=0x800b4318+244u*(uint32)index;
 FUN_80029854(index);return FUN_80031C50(607,ff_u32(actor+40),0,ff_u32(actor+48));
}
GDB_CALL sint32 FUN_80033008(sint32 index) {
 if(ff_u32(0x800b4334+244u*(uint32)index)&0x800u)return (sint32)FUN_80032EE0(index);
 return FUN_8002976C(index);
}
/* 1F0B4: attachment proximity to environment objects584/585. Live no-contact
 * route verified; positive object/effect route needs a separate fixture. */
GDB_CALL sint32 FUN_8001F0B4(sint32 index,sint32 attachment) {
 uint32 point[3],object=0x800b89f0,kind,item;sint32 i=0,count,x,z,slot;
 FUN_8001B9F4(index,attachment,point);
 x=(sint16)(point[0]>>16);z=(sint16)(point[2]>>16);
 count=(sint32)ff_u32(0x8009403c);if(count<=0)return count;
 do {
  kind=ff_u32(object);
  if(kind-584u<2u&&FUN_800163F4(x,z,ff_s16(object+6),ff_s16(object+14))<192) {
   if(ff_u32(object)-584u<2u) {
    FUN_80056670(133,255,6144);
    FUN_80031C50(894,ff_u32(object+4),ff_u32(object+8),ff_u32(object+12));
    ff_w32(0x800941a8,8);
    slot=FUN_800167C4(527,ff_u32(object+4),ff_u32(object+8),ff_u32(object+12),0);
    item=0x8009a3c8+36u*(uint32)slot;ff_w16(item+24,64);ff_w16(item+32,32);
    ff_w16(object+16,ff_u32(object)==584?0xc000:0x8000);
    ff_w16(item+16,(uint16)((uint16)ff_s16(item+16)-4096u+(ff_rand_8006D348()&8191u)));
   }
   ff_w32(object,55);return 55;
  }
  ++i;object+=36;
 }while(i<(sint32)ff_u32(0x8009403c));
 return 0;
}
/* 3314C: combo voice/landing, attachment reset and Smasher ground burst. */
GDB_CALL sint32 FUN_8003314C(sint32 index) {
 uint32 actor=0x800b4318+244u*(uint32)index,state;
 if(ff_u32(actor+28)&0x8000u) {
  if(index>=2)return 0;
  ff_w16(actor+98,1);FUN_80057CA4(index+867);state=ff_u32(actor+12);
  if(state==33&&ff_s16(actor+52)!=2){ff_w16(actor+100,1);return 39;}
  if(state==39&&ff_s16(actor+52)==3){ff_w16(actor+100,3);return 3;}
  ff_w16(actor+100,2);return 2;
 }
 if(ff_u32(actor+28)&0x800u)return (sint32)FUN_80032EE0(index);
 if(ff_u32(actor+12)!=33)return FUN_8002976C(index);
 if(ff_s16(actor+52)==2) {
  uint32 effect=FUN_80031C50(604,ff_u32(actor+40),0,ff_u32(actor+48));
  ff_w16(effect+28,(uint16)index);FUN_80056670(19,128,6144);ff_w32(0x800941a8,16);
  return FUN_80031F64(ff_u32(actor+40),ff_u32(actor+48),effect);
 }
 return 2;
}
/* 23FD0: preserve19718's region lookup side effects even on a zero result. */
GDB_CALL sint32 FUN_80023FD0(uint32 x,uint32 z) {
 if(!FUN_80019718(x,z))return 0;
 return ff_s16(ff_u32(0x80093d04)+18);
}
/* Identical effects at24F38..24F78 and26578..265AC, in MIPS call order. */
static void player_special_attack(uint32 actor,uint32 table,sint32 index) {
 sint32 health=ff_s16(actor+66);if(health<33)return;
 ff_w16(actor+66,(uint16)(health-32));ff_w8(actor+127,0);
 FUN_80056670(46,255,6144);FUN_800237CC(actor,table,33);
 FUN_8005FB00(index);FUN_80017E30(index);
}
/* 24E24..24FEC: Circle action, including original combination priority. */
static void player_action1(uint32 actor,uint32 table,sint32 index,sint32 state) {
 sint32 item;uint32 actions=0x80094100+6u*(uint32)index;
 if(state!=69&&state!=82&&state!=87&&state!=87)return;
 item=player_byte(actor+126);
 if(item!=-1&&(sint32)ff_u32(0x8009a3c8+36u*(uint32)item)>=522) {
  FUN_8005FB00(index);FUN_800237CC(actor,table,161);return;
 }
 item=player_byte(actor+125);
 if(item!=-1&&(sint32)ff_u32(0x8009a3c8+36u*(uint32)item)>=522) {
  FUN_8005FB00(index);FUN_80017E30(index);return;
 }
 if(player_byte(actions)) {
  player_special_attack(actor,table,index);return;
 }
 if(player_byte(actions+2)) {
  if(!FUN_8001A068(table,39))return;
  FUN_800237CC(actor,table,39);
 } else if(state!=87&&ff_u32(actor+12)!=87) {
  ff_w32(actor+36,0);FUN_800237CC(actor,table,158);
 }
 FUN_8005FB00(index);
}
/* 25014..2519C: Square action; simultaneous Cross selects animation28. */
static void player_action2(uint32 actor,uint32 table,sint32 index,sint32 state) {
 uint32 angle,target_state=0; sint32 animation=26;
 if(state==69||state==82) {
  if(player_byte(0x80094100+6u*(uint32)index))animation=28;
  else {
   if(ff_u32(0x80094118)==1)target_state=ff_u32(0x800b4324+244u*ff_u32(0x80094010));
   angle=2u*(uint32)(sint32)ff_s16(actor+56);
   if(FUN_80023FD0(ff_u32(actor+40)+((uint32)(sint32)ff_s16(0x800802d0+angle)<<9),
     ff_u32(actor+48)-((uint32)(sint32)ff_s16(0x800806d0+angle)<<9)))animation=28;
   else if(ff_u32(0x80094118)==1&&(uint16)((uint16)ff_s16(0x8009483c)+16383u)<32767u
    &&(target_state-252u<2u||target_state-237u<2u||target_state==239))animation=28;
  }
  FUN_800237CC(actor,table,animation);FUN_8005FB00(index);
 } else if(state==26&&(ff_u32(actor+28)&512u)) {
  FUN_800237CC(actor,table,27);FUN_8005FB00(index);
 }
}
static void player_action0(uint32 actor,uint32 table,sint32 index,sint32 state) {
 uint32 angle,target_state=0; sint32 animation;
 /* 251C4..251E0: running Cross chooses67, without clearing buffered input. */
 if(state==87){FUN_800237CC(actor,table,67);return;}
 if(state==69||state==82) {
  FUN_80017FE0(index,96);
  if((uint16)ff_s16(actor+102)&15) {
   if(FUN_8001A068(table,130)){FUN_80017E30(index);FUN_800237CC(actor,table,130);}
   return;
  }
  if((ff_u32(actor+124)&0x00ffff00u)==0x00ffff00u&&ff_u32(0x80094550+4u*(uint32)index)!=0xffffffffu){
   uint32 selected=ff_u32(0x80094550+4u*(uint32)index);
   uint32 kind=ff_u32(0x8009a3c8+36u*selected);
   ff_w8(actor+131,(uint8)selected); /* 25298 branch delay executes for every kind. */
   if(kind-586u<12u)return;
   animation=(sint32)kind>=522?121:kind-506u<2u?123:kind==515?126:kind==504?128:118;
   FUN_800237CC(actor,table,animation);FUN_8005FB00(index);return;
  }
  if(player_byte(actor+126)!=-1) {
   /* 2530C..25504: signed halfword kind selects the held-item Cross action. */
   uint32 item=0x8009a3c8+36u*(uint32)player_byte(actor+126);
   sint32 kind=ff_s16(item);
   if(kind>=522)animation=161;
   else if((uint16)(kind-496)<2u) {
    FUN_800237CC(actor,table,159);
    if(!ff_s16(item+22)){ff_w16(item+22,270);FUN_80031A28(index,577);}
    FUN_8005FB00(index);return;
   }
   else if(kind==513)animation=159;
   else if(kind==515||(uint16)(kind-499)<5u) {
    if(ff_s16(item+22)>0){
     ff_w16(item+22,(uint16)((uint16)ff_s16(item+22)-1u));
     if(kind==515){FUN_800237CC(actor,table,193);FUN_80030310(index);}
     else{FUN_80030440(index);FUN_800237CC(actor,table,192);}
    }else FUN_80056670(26,255,6144);
    FUN_8005FB00(index);return;
   }
   else if(kind==505) {
    if(ff_s16(item+22)>0){
     ff_w16(item+22,(uint16)((uint16)ff_s16(item+22)-1u));
     FUN_800301CC(index);FUN_800237CC(actor,table,192);ff_w16(actor+84,1);
    }else FUN_80056670(26,255,6144);
    FUN_8005FB00(index);return;
   }
   else if((uint16)(kind-486)<3u||kind==479||kind==481||kind==483)animation=43;
   else if((uint16)(kind-506)<2u) {
    if(ff_s16(item+22)>0){
     ff_w16(item+22,(uint16)((uint16)ff_s16(item+22)-1u));
     FUN_800237CC(actor,table,195);FUN_800301CC(index);
    }else FUN_80056670(26,255,6144);
    FUN_8005FB00(index);return;
   }
   else animation=35;
   FUN_800237CC(actor,table,animation);FUN_8005FB00(index);return;
  }
  if(player_byte(actor+125)!=-1){
   uint32 item=0x8009a3c8+36u*(uint32)player_byte(actor+125);sint32 kind=ff_s16(item);
   if(kind==515||kind==504){
    if(ff_s16(item+22)>0){
     ff_w16(item+22,(uint16)((uint16)ff_s16(item+22)-1u));
     if(kind==515){FUN_800237CC(actor,table,193);FUN_80030310(index);}
     else{FUN_80030440(index);FUN_800237CC(actor,table,196);}
    }else FUN_80056670(26,255,6144);
   }else FUN_800237CC(actor,table,35);
   FUN_8005FB00(index);return;
  }
  if(ff_u32(0x80094118)==1)target_state=ff_u32(0x800b4324+244u*ff_u32(0x80094010));
  angle=2u*(uint32)(sint32)ff_s16(actor+56);
  animation=0;
  if(FUN_80023FD0(ff_u32(actor+40)+((uint32)(sint32)ff_s16(0x800802d0+angle)<<9),
    ff_u32(actor+48)-((uint32)(sint32)ff_s16(0x800806d0+angle)<<9)))animation=28;
  else if(ff_u32(0x80094118)==1&&(uint16)((uint16)ff_s16(0x8009483c)+16383u)<32767u
   &&(target_state-252u<2u||target_state-237u<2u||target_state==239))animation=28;
  FUN_800237CC(actor,table,animation);FUN_8005FB00(index);return;
 }
 if(state==0||state==1) {
  if(ff_u32(actor+28)&512){FUN_800237CC(actor,table,state+1);FUN_8005FB00(index);}
  return;
 }
 /* 256F4..25808: only0/1 and held-item states35/37/43 can chain.
  * All remaining states (including hurt133) preserve buffered input. */
 if((state==35||state==37||state==43)&&(ff_u32(actor+28)&512u)&&player_byte(actor+126)!=-1){
  FUN_800237CC(actor,table,state==35?37:state==37?38:44);FUN_8005FB00(index);
 }
 return;
}
/* 262B4..262FC: one RNG draw per footstep, followed by the real sound allocator. */
GDB_CALL void ff_player_footstep_800262B4(sint32 type) {
 static const sint32 bases[4]={52,72,87,72};
 if((uint32)type>=4)player_wip_fatal(0x800262b4);
 FUN_80056670(bases[type]+(sint32)(ff_rand_8006D348()&3),255,6144);
}
/* 1A07C: predict whether this player's step increases separation beyond the
 * two-player boundary. The initial distance call also occurs in one-player mode.
 * Unlike1AEF4's movement bound, this uses inclusive +/-0x4000000. */
GDB_CALL sint32 FUN_8001A07C(sint32 index) {
 uint32 actor=0x800b4318+244u*(uint32)index,other,step,angle,x,z,dx,dz;
 sint32 previous,state,distance;
 previous=FUN_80016478(ff_s16(0x800b4342),ff_s16(0x800b434a),ff_s16(0x800b4436),ff_s16(0x800b443e));
 state=FUN_8002371C(actor);
 if(state==87)step=352;
 else if(state==82||state==69)step=280;
 else if(state==158)step=ff_u32(actor+36);
 else return 0;
 if(ff_u32(0x800940b0)!=2)return 0;
 angle=2u*(uint32)(sint32)ff_s16(actor+56);
 other=0x800b4318+244u*(1u-(uint32)index);
 x=ff_u32(actor+40)+(uint32)(sint32)ff_s16(0x800802d0+angle)*step;
 z=ff_u32(actor+48)-(uint32)(sint32)ff_s16(0x800806d0+angle)*step;
 dx=x-ff_u32(other+40);dz=z-ff_u32(other+48);
 distance=FUN_80016478(ff_s16(other+42),ff_s16(other+50),(sint32)x>>16,(sint32)z>>16);
 if(ff_s16(actor+66)<=0||distance<previous||ff_s16(other+66)<=0){dx=0;dz=0;}
 return dx+0x4000000u>0x8000000u||dz+0x4000000u>0x8000000u;
}
/* Digital-pad block 247A4..24994, split out for original-MIPS differential.
 * Returns local B8 (facing) in bit0 and local C0 (backward) in bit1.
 * Includes analog24720 and digital247A4; movement dispatch remains at24AD8. */
static uint32 player_direction_context(uint32 actor,sint32 state,uint32 *buttons,uint32 locked,uint32 retained[3]) {
 uint32 direction=*buttons&240u,desired=(uint16)ff_s16(actor+62),flags=0,offset,raw;
 sint32 delta;
 if(player_byte(0x80094114)==22) {
  if(!ff_u32(0x800945d4)&&!ff_u32(0x800945f8)){*buttons&=0xffffff0fu;direction=0;}
  else {*buttons|=240u;direction=240u;}
 }
 if(direction) {
  if(player_byte(0x80094114)==22)
   desired=(uint32)FUN_8001614C(0,0,(sint32)ff_u32(0x800945d4),(sint32)(0u-ff_u32(0x800945f8)));
  else switch(direction) {
   case 16:desired=(uint32)-32768;break;case 48:desired=24576;break;
   case 32:desired=16384;break;case 96:desired=8192;break;
   case 64:desired=0;break;case 192:desired=(uint32)-8192;break;
   case 128:desired=(uint32)-16384;break;case 144:desired=(uint32)-24576;break;
  }
  desired+=(uint16)ff_s16(0x80093d34);
  offset=((uint32)(uint16)ff_s16(actor+56)<<5)-desired;
  if((uint16)(offset+8191u)<16383u)flags|=1;
  if((uint16)(offset-24577u)<16383u)flags|=2;
  raw=ff_u32(actor+12);
  if(raw==147||raw-149u<2u||raw==166||(state==158&&(sint32)ff_u32(actor+4)<8))
   desired=(uint16)ff_s16(actor+62);
 }
 /* 49864 saves these caller S0/S1/S2 at entry-SP-40/-36/-32. */
 if(retained){retained[0]=desired;retained[1]=actor;retained[2]=direction;}
 if(locked){*buttons&=0xffffff0fu;return flags;}
 if((flags&2)&&state==69)return flags;
 ff_w16(actor+62,(uint16)desired);
 delta=(sint32)((desired-((uint32)(uint16)ff_s16(actor+56)<<5))<<16)>>23;
 if(delta>128)delta=128;
 if(delta< -128)delta=-128;
 /* s2 is a nonnegative digital mask: zero delta with held direction adds1. */
 if(!delta&&direction)delta=1;
 if(retained)retained[0]=(uint32)delta;
 if(direction)ff_w16(actor+56,(uint16)(((uint32)(uint16)ff_s16(actor+56)+(uint32)delta)&2047));
 return flags;
}
GDB_CALL uint32 ff_player_direction_800247A4(uint32 actor,sint32 state,uint32 *buttons,uint32 locked) {
 return player_direction_context(actor,state,buttons,locked,NULL);
}
/* 26108..262A4: movement requested by the animation event2000. */
GDB_CALL void ff_player_motion_80026108(uint32 actor,sint32 index,uint32 buttons) {
 uint32 desired=(uint16)ff_s16(actor+62),direction=buttons&240u,speed,offset,x,z;
 if(player_byte(0x80094114)==22) {
  desired=(uint32)FUN_8001614C(0,0,(sint32)ff_u32(0x800945d4),(sint32)(0u-ff_u32(0x800945f8)))+(uint16)ff_s16(0x80093d34);
  direction=1;speed=(uint32)(FUN_800163F4(0,0,(sint32)ff_u32(0x800945d4),(sint32)ff_u32(0x800945f8))>>1);
 } else {
  speed=ff_u32(actor+36);
  if(direction&&(sint32)speed<64){speed+=8;ff_w32(actor+36,speed);}
  switch(direction) {
   case 16:desired=0xffff8000u;break;case 48:desired=24576;break;
   case 32:desired=16384;break;case 96:desired=8192;break;
   case 64:desired=0;break;case 192:desired=0xffffe000u;break;
   case 128:desired=0xffffc000u;break;case 144:desired=0xffffa000u;break;
  }
  desired+=(uint16)ff_s16(0x80093d34);
 }
 if(!direction)return;
 offset=(desired>>4)&4094u;
 x=ff_u32(actor+40)+(uint32)(sint32)ff_s16(0x800802d0+offset)*speed;
 z=ff_u32(actor+48)-(uint32)(sint32)ff_s16(0x800806d0+offset)*speed;
 ff_collision_candidate(actor,&x,&z);
 if(!FUN_8001A07C(index)){ff_w32(actor+40,x);ff_w32(actor+48,z);}
}
static void player_target_turn(uint32 actor,uint32 flag,uint32 angle_address) {
 uint32 angle=(uint16)ff_s16(angle_address);
 if(ff_u32(flag)&&(uint16)(angle+8191u)<16383u)
  ff_w16(actor+56,(uint16)(((uint32)(uint16)ff_s16(actor+56)+(uint32)((sint32)(angle<<16)>>24))&2047));
}
/* 24008 live unarmed idle and133/134 hit-reaction control paths.
 * Reaction path follows the same no-input dispatch until animation completion;
 * 2758C then branches to27CB0 and returns to state69 via237CC.
 * Evidence: player-hit186-trace.json records all97 reached transfers.
 * Conditions outside the translated
 * path stop at a named original PC; this is not complete player logic.
 * Evidence: player-first-path.lst, actual MIPS and first-call differential. */
/* 23DC0: acquire a facing, eligible NPC for the grapple animation pair. */
GDB_CALL sint32 FUN_80023DC0(sint32 index) {
 FF_FUNCTION_MARKER(0x80023dc0u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,other,table;
 sint32 type=ff_s16(actor+52),state,result,sample=-1;
 table=ff_u32(0x800b3be8+4u*(uint32)type);
 if((uint16)((uint16)ff_s16(0x8009483c)+0x2000u)>=0x4001u)return 0;
 other=0x800b4318+244u*ff_u32(0x80094010);
 if(ff_s16(other+52)==50||ff_s16(other+88)!=-1)return -1;
 state=FUN_8004C9B0(other);
 result=ff_s16(other+66);if(result<=0)return result;
 result=player_byte(other+120);if(result!=-1)return result;
 if((uint32)state!=ff_u32(other+200)&&(uint32)state-57u>=8u&&state!=236&&state!=82&&state!=103&&state!=113&&state!=108)return 108;
 FUN_80017E30((sint32)ff_u32(0x80094010));FUN_80017E30(index);
 ff_w8(actor+121,0);ff_w8(other+120,(uint8)index);ff_w8(other+121,0);
 ff_w8(actor+122,(uint8)ff_u32(0x80094010));ff_w8(other+123,50);
 if(type==0)sample=47;else if(type==1)sample=67;else if(type==2)sample=83;else if(type==3)sample=103;
 if(sample>=0)FUN_80056670(sample,255,6144);
 FUN_800237CC(actor,table,166);
 return FUN_800237CC(other,ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(other+52)),171);
}
/* 27A8C..27B14: retained S6, not a fresh partner lookup after callbacks. */
static void player_grapple_pair_end(uint32 actor,uint32 table,uint32 other,sint32 state) {
 if(state==167) {
  FUN_800237CC(other,ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(other+52)),77);
  FUN_800237CC(actor,table,169);
  FUN_800235C4(actor,other);
 } else {
  FUN_800237CC(actor,table,169);
  FUN_800237CC(other,ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(other+52)),77);
 }
}
/* 26DF8..27240: paired attack completion, including signed wrapped combo counts.
 * Called only for states3/4/34/41,10..17,19..23,40. Callee order and link reloads matter. */
static void player_grapple_combo_end(uint32 actor,uint32 table,sint32 index,sint32 state) {
 sint32 partner=player_byte(actor+122);uint32 other,count;
 uint32 actions=0x80094100+6u*(uint32)index;
 if(state==3||state==4||state==34||state==41){FUN_800237CC(actor,table,69);return;}
 if(state==14||state==15) {
  if(player_byte(actions)&&partner!=-1&&ff_s16(0x800b4318+244u*(uint32)partner+66)>0) {
   if(state==14){FUN_8005FB00(index);FUN_800237CC(actor,table,15);return;}
   count=(uint16)((uint16)ff_s16(actor+86)+1u);ff_w16(actor+86,(uint16)count);
   if((sint16)count<2){FUN_8005FB00(index);FUN_800237CC(actor,table,14);return;}
  }
  FUN_800237CC(actor,table,state==14?16:17);return;
 }
 if(state==16||state==17) {
  if(partner!=-1) {
   other=0x800b4318+244u*(uint32)partner;ff_w8(other+120,255);ff_w8(other+121,0);
   FUN_800237CC(other,ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(other+52)),69);
  }
  ff_w8(actor+122,255);FUN_800237CC(actor,table,69);return;
 }
 if(partner==-1){FUN_800237CC(actor,table,69);return;}
 if(state==10||state==19){FUN_800237CC(actor,table,state==10?11:20);return;}
 if(state==11||state==20) {
  if(player_byte(actions)){FUN_800237CC(actor,table,state==11?12:21);FUN_8005FB00(index);}
  else FUN_800237CC(actor,table,state==11?13:22);
  return;
 }
 if(state==12) {
  count=(uint16)((uint16)ff_s16(actor+86)+1u);ff_w16(actor+86,(uint16)count);
  if((sint16)count<3){FUN_800237CC(actor,table,11);return;}
  other=0x800b4318+244u*(uint32)player_byte(actor+122);
  FUN_800237CC(other,ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(other+52)),171);
  FUN_800237CC(actor,table,34);ff_w8(actor+122,255);ff_w8(other+120,255);
  ff_w32(other+176,ff_u32(0x80093dd0)+15u);return;
 }
 if(state==21) {
  if(!player_byte(actions)){FUN_800237CC(actor,table,23);return;}
  count=(uint16)((uint16)ff_s16(actor+86)+1u);ff_w16(actor+86,(uint16)count);
  if((sint16)count<2)FUN_800237CC(actor,table,20);
  else {
   FUN_800237CC(actor,table,23);
   other=0x800b4318+244u*(uint32)player_byte(actor+122);
   ff_w8(other+120,255);ff_w8(actor+122,255);
  }
  FUN_8005FB00(index);return;
 }
 /* 13/40/22/23 return to the hold while still linked. */
 FUN_800237CC(actor,table,74);
}

/* 26634..26754: state169 paired hold; Square is reloaded after Cross clears input. */
static void player_grapple169(uint32 actor,uint32 table,sint32 index,sint32 type) {
 sint32 partner=player_byte(actor+122);
 uint32 other,actions=0x80094100+6u*(uint32)index;
 if(partner==-1){FUN_800237CC(actor,table,69);return;}
 other=0x800b4318+244u*(uint32)partner;
 FUN_80023664(actor,other);
 if(player_byte(actions)) {
  FUN_8005FB00(index);
  if(type==2) {
   FUN_800237CC(actor,table,178);ff_w8(actor+122,255);
   FUN_800237CC(other,ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(other+52)),184);
   ff_w8(other+120,255);ff_w8(other+129,(uint8)index);
  }
 }
 if(player_byte(actions+2)) {
  FUN_8005FB00(index);
  if(type==2) {
   FUN_800237CC(actor,table,179);ff_w8(actor+122,255);
   FUN_800237CC(other,ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(other+52)),189);
   ff_w8(other+120,255);ff_w8(other+129,(uint8)index);
  }
 }
}
/* 26764..26D50: state74 grapple controls; input priority3,1,0,2. */
static void player_grapple74(uint32 actor,uint32 table,sint32 index,sint32 type,uint32 directions) {
 sint32 partner=player_byte(actor+122),mine=0,theirs=0;
 uint32 other,actions=0x80094100+6u*(uint32)index;
 if(partner==-1){FUN_800237CC(actor,table,69);return;}
 other=0x800b4318+244u*(uint32)partner;
 if(directions&2u) {
  FUN_800237CC(actor,table,69);ff_w8(other+120,255);
  FUN_800237CC(other,ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(other+52)),69);
  ff_w8(actor+122,255);return;
 }
 if(player_byte(actions+3)) {
  FUN_8005FB00(index);
  if(type==2) {
   ff_w8(other+121,1);
   FUN_800237CC(other,ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(other+52)),172);
   FUN_800237CC(actor,table,167);return;
  }
  if(type==1){theirs=(directions&1u)?188:186;mine=(directions&1u)?179:178;}
  else if(type==3){theirs=(directions&1u)?142:185;mine=(directions&1u)?179:178;}
  else if(type==0) {
   if(!(directions&1u)) {
    FUN_800237CC(other,ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(other+52)),171);
    FUN_800237CC(actor,table,34);ff_w8(actor+122,255);ff_w8(other+120,255);
    ff_w32(other+176,ff_u32(0x80093dd0)+15u);return;
   }
   theirs=187;mine=179;
  } else return;
 } else if(player_byte(actions+1)) {
  FUN_8005FB00(index);ff_w8(actor+122,255);
  theirs=type==2?181:type==1?182:type==3?183:180;
  FUN_800237CC(other,ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(other+52)),theirs);
  if(type==3){ff_w8(actor+116,(uint8)partner);ff_w8(actor+118,0);}
  FUN_800237CC(actor,table,177);ff_w8(other+120,255);ff_w8(other+129,(uint8)index);return;
 } else if(player_byte(actions)) {
  FUN_8005FB00(index);ff_w16(actor+86,0); /* branch delay for every type */
  if(type==0||type==1){FUN_800237CC(actor,table,type==0?10:19);return;}
  if(type==2) {
   FUN_800237CC(other,ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(other+52)),141);
   FUN_800237CC(actor,table,3);ff_w8(other+129,(uint8)index);ff_w8(other+120,255);
   ff_w8(actor+122,255);ff_w8(actor+116,(uint8)partner);ff_w8(actor+118,0);return;
  }
  if(type==3) {
   FUN_800237CC(other,ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(other+52)),174);
   FUN_800237CC(actor,table,168);ff_w8(actor+121,2);
  }
  return;
 } else if(player_byte(actions+2)) {
  FUN_8005FB00(index);
  if(type==0){FUN_800237CC(actor,table,40);return;}
  if(type==1||type==3){theirs=140;mine=41;}
  else if(type==2){theirs=142;mine=4;}else return;
 } else return;
 FUN_800237CC(other,ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(other+52)),theirs);
 FUN_800237CC(actor,table,mine);ff_w8(other+129,(uint8)index);ff_w8(other+120,255);ff_w8(actor+122,255);
}
/* Extracted 24408..2452C block; item kind 587..596 was checked by caller. */
static void player_collect_80024408(uint32 actor,uint32 item,sint32 index,uint32 kind) {
 sint32 amount=0;
 if(ff_s16(actor+66)<=0)return;
 if(kind==587)amount=160;else if(kind==588)amount=128;else if(kind==589)amount=96;
 else if(kind==590||kind==591)amount=64;
 if(amount)ff_w16(actor+66,(uint16)((uint16)ff_s16(actor+66)+amount));
 if(kind-593u<4u){
  uint32 score=0x80094030+4u*(uint32)index;
  ff_w32(score,ff_u32(score)+500u*(kind-592u));FUN_8005E9E8(index);
 }
 ff_w32(item,0xffffffffu);FUN_80056670(46,255,7168);
 if(ff_s16(actor+66)>=ff_s16(actor+72))ff_w16(actor+66,(uint16)(ff_s16(actor+72)-4));
}
/* 25A8C..25E58: hit sounds depend on S0 (saved state), not a reloaded pose. */
static void player_contact_sound(sint32 type,sint32 state,sint32 attachment) {
 sint32 sample=-1;
 if(attachment==2){
  if(type==0){if(state==1)sample=34;else if(state==11)sample=44;else if(state==24)sample=33;else if(state==25)sample=35;}
  else if(type==1){if(state==1)sample=58;else if(state==11||state==24)sample=57;else if(state==25)sample=59;}
  else if(type==2){if(state==1||state==66)sample=77;else if(state==11||state==24)sample=76;else if(state==25)sample=78;}
  else if(type==3){if(state==1||state==15)sample=94;else if(state==11||state==24)sample=93;else if(state==25)sample=95;}
 }else{
  if(type==0){if(state==0)sample=33;else if(state==2)sample=35;else if(state==25)sample=34;}
  else if(type==1){if(state==0||state==20)sample=57;else if(state==2)sample=59;else if(state==25||state==21)sample=58;}
  else if(type==2){if(state==0)sample=76;else if(state==2)sample=78;else if(state==25||state==66)sample=77;}
  else if(type==3){if(state==0||state==14)sample=93;else if(state==2)sample=95;else if(state==25)sample=94;}
 }
 if(sample>=0)FUN_80056670(sample,255,6144);
}
/* 245DC..246FC: the original lock predicate, including the state169 override. */
static uint32 player_direction_locked(uint32 actor,sint32 state) {
 uint32 n=(uint32)state;
 if(state==169)return 0;
 return state==147||n-149u<2u||n-177u<2u||state==179||state==118
  ||state==121||n-123u<2u||state==126||n-66u<2u||state==65||state==92
  ||state==34||state==41||n-3u<2u||state==33||n-170u<2u||state==173
  ||n-180u<2u||n-182u<2u||n-184u<2u||state==186||n-130u<2u
  ||player_byte(actor+122)!=-1||ff_s16(actor+66)<=0;
}
/* 2588C..25A68: events and state are captured before any contact callback. */
static void player_early_contacts(uint32 actor,sint32 index,sint32 type,sint32 state,uint32 events) {
 sint32 sample;
 if(events&0x800u)ff_w16(actor+82,0);
 if(events&1u)if(FUN_8001CED0(index,1)==1)
  if((uint32)type<4u)FUN_80056670(43,255,6144);
 if(events&32u)if(FUN_8001CED0(index,6)==1) {
  sample=-1;
  if(state==24){if(type==0)sample=33;else if(type==1)sample=57;else if(type==2)sample=76;else if(type==3)sample=93;}
  else if(type==2&&state==66)sample=77;
  if(sample>=0)FUN_80056670(sample,255,6144);
 }
 if(events&64u)if(FUN_8001CED0(index,7)==1) {
  sample=-1;
  if(state==25){if(type==0)sample=34;else if(type==1)sample=58;else if(type==2)sample=77;else if(type==3)sample=94;}
  else if(type==2&&state==66)sample=77;
  else if(type==3&&state==2)sample=95;
  if(sample>=0)FUN_80056670(sample,255,6144);
 }
}
GDB_CALL sint32 FUN_80024008(void) {
 sint32 index=0,count=(sint32)ff_u32(0x800940b0),type,partner,state,timer,pose_step,contact_state;
 uint32 actor=0x800b4318,table,buttons,actions,direction_flags,contact_events,direction_locked;
 uint32 pickup_words[3],pickup_context,pickup_state;
 uint32 retained_partner_actor;
 if(count<=0)return count;
 do {
  type=ff_s16(actor+52);table=ff_u32(0x800b3be8+4u*(uint32)type);partner=player_byte(actor+122);
  if(type==2&&ff_u32(actor+12)!=67&&ff_s16(actor+136)>0){FUN_800567F8(ff_s16(actor+136)-1);ff_w16(actor+136,0);}
  if(ff_s16(actor+52)==-1)goto next_actor;
  timer=ff_s16(actor+80);if(timer)ff_w16(actor+80,(uint16)(timer-1));
  if((sint32)ff_u32(0x800940b0)>=2) {
   timer=player_byte(actor+119);
   if(timer){timer=(sint8)(timer+1);ff_w8(actor+119,(uint8)timer);if(timer>=63){ff_w8(actor+119,62);ff_w32(actor,0);}goto next_actor;}
   if(ff_s16(actor+66)<=0){timer=player_byte(actor+122);if(timer!=-1){ff_w8(0x800b4390+244u*(uint32)timer,255);ff_w8(actor+122,255);}ff_w8(actor+119,1);goto next_actor;}
  }
  if(ff_u32(actor+232)&1){
   FUN_80040CF4(index);
   if((sint32)ff_u32(actor+176)<(sint32)ff_u32(0x80093dd0))ff_w32(actor+232,ff_u32(actor+232)&0xfffffffeu);
  }
  /* 241C8..242E4: held actor handles escape input, then finishes this actor. */
  if(player_byte(actor+120)!=-1){
   FUN_80017E30(index);ff_w16(actor+98,0);
   if(player_byte(actor+121)==1)goto next_actor;
   FUN_8004ADA8(player_byte(actor+120),index);
   if(ff_u32(actor+4)==0xffffffffu)FUN_800237CC(actor,ff_u32(0x800b3be8+4u*(uint32)type),171);
   buttons=FUN_8005F558(ff_u32(0x80094288+4u*(uint32)index));
   FUN_8005F8F0((uint32)index,~ff_u32(0x80093ff8+4u*(uint32)index)&buttons);
   FUN_8005FA58((uint32)index,buttons);
   if(player_byte(0x80094100+6u*(uint32)index)){
    timer=player_byte(actor+120);
    if(timer!=-1){ff_w8(actor+120,255);ff_w8(0x800b4392+244u*(uint32)timer,255);}
   }
   goto next_actor;
  }
  partner=player_byte(actor+122);
  /* 242E8..24340 initializes saved S6, including the slot7 fallback. */
  partner=player_byte(actor+122);
  retained_partner_actor=partner==-1?0x800b49c4:0x800b4318+244u*(uint32)partner;
  if(partner!=-1&&ff_s16(retained_partner_actor+66)<=0){
   ff_w8(actor+122,255);FUN_800237CC(actor,table,69);
  }
  FUN_80017FE0(index,96);
  {
   sint32 selected=(sint32)ff_u32(0x80094550+4u*(uint32)index);
   if(selected>=0){
    uint32 kind=ff_u32(0x8009a3c8+36u*(uint32)selected);
    /* 2438C/24400: ordinary equipment stays selected for the action dispatch. */
    if(kind==598)player_wip_zero(0x80024394);
    if(kind-587u<10u)player_collect_80024408(actor,0x8009a3c8+36u*(uint32)selected,index,kind);
   }
  }
  buttons=FUN_8005F558(ff_u32(0x80094288+4u*(uint32)index));
  if((sint32)ff_u32(0x80093e04)>0||(ff_u32(0x80093d5c)==1&&(sint32)ff_u32(0x80093dd0)<60)
   ||(ff_u32(0x80093d58)==4&&(sint32)ff_u32(0x80093dd0)<660)) {
   buttons=0;ff_w32(0x800945f8,0);ff_w32(0x800945d4,0);
  }
  state=FUN_8002371C(actor);
  direction_locked=player_direction_locked(actor,state);
  direction_flags=player_direction_context(actor,state,&buttons,direction_locked,pickup_words);
  FUN_8005F8F0((uint32)index,~ff_u32(0x80093ff8+4u*(uint32)index)&buttons);
  FUN_8005FA58((uint32)index,buttons);actions=0x80094100+6u*(uint32)index;
  if(ff_u32(0x800941b8)){ff_w8(actions+3,0);ff_w8(actions+2,0);ff_w8(actions,0);}
  pickup_state=ff_u32(actor+12);
  pickup_context=pickup_state-130u<2u&&direction_locked&&partner==-1
   &&!ff_s16(actor+98)&&player_byte(actor+125)==-1&&player_byte(actor+126)==-1;
  FUN_80049864(index);
  if(partner!=-1||direction_locked){ff_w32(0x80094110,0);ff_w32(0x80094118,0);}
  player_target_turn(actor,0x80094118,0x8009483c);player_target_turn(actor,0x80094110,0x8009410c);
  if((buttons&240)&&FUN_8002371C(actor)==69) {
   if(player_byte(actions+5)) {
    ff_w32(actor+12,87);
    if(!FUN_8001A07C(index)) {
     ff_w32(actor+12,(uint32)state);FUN_800237CC(actor,table,87);
     if(FUN_8002371C(actor)!=87)FUN_800237CC(actor,table,82);
    }
   } else {
   ff_w32(actor+12,82);
   if(direction_flags&2)FUN_800237CC(actor,table,92);
   else if(!FUN_8001A07C(index)) {
    FUN_800237CC(actor,table,82);
    /* Original24BDC loop deliberately advances the same pose twelve times. */
    if(type==0||type==2)for(pose_step=0;pose_step<12;pose_step++)
     FUN_80011A50(ff_u32(0x80081720+4u*(uint32)type),actor,0x800ba210+504u*(uint32)index);
   }
   }
  }
  state=FUN_8002371C(actor);
  if(ff_s16(actor+98)) {
   sint32 attachment;
   if(type==0)attachment=4;else if(type==1)attachment=5;
   else if(type==3)attachment=ff_u32(actor+12)==39?4:5;
   else if(type==2)attachment=3;else {player_wip_zero(0x80024c58);attachment=0;}
   FUN_80057CE4(index,attachment);
   ff_w16(actor+98,(uint16)((uint16)ff_s16(actor+98)+(uint16)ff_s16(0x8007af04+4u*(uint32)(sint32)ff_s16(actor+100))));
  }
  /* 24CB0..24CE8: held right item only enters this dispatch on Triangle/Square. */
  if(partner==-1&&player_byte(actor+126)!=-1&&(player_byte(actions+3)||player_byte(actions+2))) {
   if(FUN_8002371C(actor)==69) {
    sint32 animation=ff_u32(actor+12)==70?161:159;
    FUN_8005FB00(index);FUN_800237CC(actor,table,animation);
   }
  }
  /* 24D48..24DFC: left item uses a separate drop/put-away animation. */
  if(partner==-1&&player_byte(actor+125)!=-1&&(player_byte(actions+3)||player_byte(actions+2))){
   if(FUN_8002371C(actor)==69){
    if(ff_u32(actor+12)==78){FUN_8005FB00(index);FUN_800237CC(actor,table,163);}
    else{FUN_8005FB00(index);FUN_80017E30(index);FUN_800237CC(actor,table,69);}
   }
  }
  if(player_byte(actions+1))player_action1(actor,table,index,state);
  if(player_byte(actions+2))player_action2(actor,table,index,state);
  if(player_byte(actions))player_action0(actor,table,index,state);
  /* 25828..2587C: Triangle idle24, then buffered event200 chain25. */
  if(player_byte(actions+3)) {
   if(state==69){FUN_8005FB00(index);FUN_800237CC(actor,table,24);}
   else if(state==24&&(ff_u32(actor+28)&512u)){FUN_8005FB00(index);FUN_800237CC(actor,table,25);}
  }
  contact_state=(sint32)ff_u32(actor+12);contact_events=ff_u32(actor+28); /* 25880/25884: saved across calls. */
  player_early_contacts(actor,index,type,contact_state,contact_events);
  if(contact_events&2u)if(FUN_8001CED0(index,2)==1)player_contact_sound(type,contact_state,2);
  if(contact_events&4u)if(FUN_8001CED0(index,3)==1)player_contact_sound(type,contact_state,3);
  /* 25E60: attachment4 contact event precedes animation sound callbacks. */
  if(contact_events&8u)if(FUN_8001CED0(index,4)==1) {
   sint32 sample=-1;
   if(type==0)sample=ff_u32(actor+12)==26?36:37;
   else if(type==1||type==3)sample=60;
   else if(type==2)sample=36;
   if(sample>=0)FUN_80056670(sample,255,6144);
  }
  /* 25ECC..25F24: attachment5 follows attachment4; sound only on contact. */
  if(contact_events&16u)if(FUN_8001CED0(index,5)==1) {
   sint32 sample=-1;
   if(type==0||type==2)sample=37;
   else if(type==1||type==3)sample=61;
   if(sample>=0)FUN_80056670(sample,255,6144);
  }
  /* 25F28..25FEC: attachment8 follows attachment5. State40 contact sound. */
  if(contact_events&0x80u)if(FUN_8001CED0(index,8)==1)
   if((uint32)type<4u&&ff_u32(actor+12)==40)FUN_80056670(45,255,6144);
  /* 25FEC..25FFC: attachment9 after attachment8, without a contact sound. */
  if(contact_events&0x100u)FUN_8001CED0(index,9);
  /* 26000..260B8: try full reach, then half reach only after a miss. */
  if(contact_events&0x400u) {
   sint32 contact=FUN_8001D10C(index,0);
   if(contact==3)FUN_80056670(33+(sint32)ff_rand_8006D348()%3,255,6144);
   if(!contact&&FUN_8001D10C(index,1)==3)FUN_80056670(33+(sint32)ff_rand_8006D348()%3,255,6144);
  }
  if(contact_events&0x1000u){
   uint32 callback=ff_u32(0x800bb9b0+4u*ff_u32(actor+12));
   /* Conditional path proved in33f7c-event-path.json: no intervening stack
    * writer after49864. Keep other contexts on the explicit WIP path. */
   if(callback==0x80033f7c&&pickup_context&&ff_u32(actor+12)==pickup_state
    &&contact_events==0x1800u&&!ff_s16(actor+98)
    &&player_byte(actor+125)==-1&&player_byte(actor+126)==-1)
    FUN_80033F7C(index,pickup_words);
   else ff_actor_event_call(callback,index);
   ff_w16(actor+84,(uint16)(ff_s16(actor+84)+1));
  }
  if(contact_events&0x2000u)ff_player_motion_80026108(actor,index,buttons);
  if(contact_events&0x4000)ff_player_footstep_800262B4(type);
  state=FUN_8002371C(actor);
  if(state==82) {
   if(!(buttons&240))FUN_800237CC(actor,table,69);
   if(FUN_8001A07C(index))FUN_800237CC(actor,table,69);
   if(player_byte(actions+5))FUN_800237CC(actor,table,87);
   if(player_byte(actions+3)&&!(direction_flags&2)&&ff_u32(0x80094118)==1&&(sint32)ff_u32(0x8009418c)<192){
    FUN_80023DC0(index);FUN_8005FB00(index); /* 26410 -> 265EC, even if acquisition failed. */
   }
  } else if(state==87) {
   if(!(buttons&240))FUN_800237CC(actor,table,69);
   if(FUN_8001A07C(index))FUN_800237CC(actor,table,69);
   if(player_byte(actions+2)&&ff_s16(actor+64)==87){FUN_800237CC(actor,table,66);FUN_8005FB00(index);}
  } else if(state==39) {
   /* 2649C..264D4: event2000 invokes environmental contact. */
   if(ff_u32(actor+28)&0x2000u)FUN_8001F0B4(index,type==2?8:4);
  } else if(state==158) {
   /* 264E0..26524: acquisition is followed by input consumption even on failure. */
   if((ff_u32(actor+28)&512u)&&player_byte(actions+3)){FUN_80023A48(index);FUN_8005FB00(index);}
   if((sint32)ff_u32(actor+4)<8&&ff_u32(actor+4)!=0xffffffffu) {
    if(player_byte(actions))player_special_attack(actor,table,index);
    else if(player_byte(actions+2)) {
     /* 265D8..265F8: Circle-to-Square transition, actual Alana tick173. */
     ff_w8(actor+127,0);FUN_800237CC(actor,table,39);FUN_8005FB00(index);
    }
   }
  } else if(state==13) {
   if(player_byte(actor+122)==-1)FUN_800237CC(actor,table,69);
  } else if((uint32)state-14u<2u) {
   if(player_byte(actor+122)==-1)FUN_800237CC(actor,table,18);
  } else if(state==74||state==169) {
   /* 2665C/267B0 refresh S6 only when a partner is present. */
   sint32 linked=player_byte(actor+122);
   if(linked!=-1)retained_partner_actor=0x800b4318+244u*(uint32)linked;
   if(state==74)player_grapple74(actor,table,index,type,direction_flags);
   else player_grapple169(actor,table,index,type);
  }
  /* Other states fall through 2675C ->26D54 in the original, including123/251. */
  if(ff_u32(actor+4)==0xffffffffu) {
   FUN_80017FE0(index,128);state=FUN_8002371C(actor);
   if(state==82) {
    /* 274C4..27534: restart walk while held; sprint may fall back to walk. */
    if(buttons&240) {
     if(player_byte(actions+5)) {
      FUN_800237CC(actor,table,87);
      if(FUN_8002371C(actor)!=87)FUN_800237CC(actor,table,82);
     } else FUN_800237CC(actor,table,82);
    } else FUN_800237CC(actor,table,69);
   } else if(state==87) {
    FUN_800237CC(actor,table,(buttons&240)?(player_byte(actions+5)?87:82):69);
   } else if((uint32)(state-10)<8u||(uint32)(state-19)<5u||state==40||state==3||state==4||state==34||state==41) {
    player_grapple_combo_end(actor,table,index,state);
   } else if(state==167||state==169) {
    player_grapple_pair_end(actor,table,retained_partner_actor,state);
   } else if(state==166||state==74) {
    /* 27A64..27A80: enter/restart the grapple hold, not idle69. */
    FUN_800237CC(actor,table,74);
   } else if(state==168) {
    /* 26DE0..26DF4: Alana paired action continues at14. */
    FUN_800237CC(actor,table,14);
   } else if(state==177||state==178||state==179) {
    /* 26DAC..26DDC and27B30..27B54: type-specific facing corrections. */
    if(state==177&&type!=1&&(uint32)(type-2)>=2u) {
     uint16 facing=(uint16)ff_s16(actor+56)^1024u;
     ff_w16(actor+56,facing);ff_w16(actor+62,(uint16)((uint32)facing<<5));
    } else if(state==179&&type==1)ff_w16(actor+56,(uint16)(((uint16)ff_s16(actor+56)-512u)&2047u));
    FUN_800237CC(actor,table,69);
   } else if(state==159) {
    /* 27304..27378,27478..274C0: release right item from attachment3. */
    sint32 selected;uint32 item,point[4];
    FUN_800237CC(actor,table,160);
    selected=player_byte(actor+126);ff_w8(actor+126,255);
    item=0x8009a3c8+36u*(uint32)selected;
    ff_w16(item+24,128);ff_w16(item+32,64);ff_w16(item+26,6528);
    ff_w16(item+16,(uint16)((uint32)(uint16)ff_s16(actor+56)<<5));
    FUN_8001B9F4(index,3,point);
    ff_w32(item+4,point[0]);ff_w32(item+8,point[1]);ff_w32(item+12,point[2]);
    ff_w16(item+28,(uint16)index);
   } else if(state==118) {
    /* 276CC..27798: claim the selected item after the pickup animation. */
    sint32 selected=player_byte(actor+131);
    uint32 item=0x8009a3c8+36u*(uint32)selected,kind;
    if(ff_u32(item+8)==1)FUN_800237CC(actor,table,119);
    else {
     sint32 animation;
     kind=ff_u32(item);
     if(kind==515){ff_w8(actor+(type==2?126u:125u),(uint8)selected);animation=126;}
     else {ff_w8(actor+126,(uint8)selected);animation=kind-499u<5u?125:119;}
     FUN_800237CC(actor,table,animation);
     ff_w32(item+8,1);ff_w32(0x80094550+4u*(uint32)index,0xffffffffu);
    }
   } else if(state==123) {
    /* 277AC..2781C: reserve the right-hand item, then enter state124. */
    sint32 selected=player_byte(actor+131);
    uint32 item=0x8009a3c8+36u*(uint32)selected;
    if(ff_u32(item+8)==1)FUN_800237CC(actor,table,119);
    else {
     ff_w8(actor+126,(uint8)selected);ff_w32(item+8,1);
     ff_w32(0x80094550+4u*(uint32)index,0xffffffffu);
     FUN_800237CC(actor,table,124);
    }
   } else if(state==121) {
    /* 27964..279FC: reserve right-hand item; convert522/524 once. */
    sint32 selected=player_byte(actor+131);
    uint32 item=0x8009a3c8+36u*(uint32)selected,kind;
    if(ff_u32(item+8)==1)FUN_800237CC(actor,table,119);
    else {
     ff_w8(actor+126,(uint8)selected);kind=ff_u32(item);
     if(kind==522){ff_w32(item,523);kind=ff_u32(item);}
     if(kind==524)ff_w32(item,525);
     ff_w32(item+8,1);ff_w32(0x80094550+4u*(uint32)index,0xffffffffu);
     FUN_800237CC(actor,table,122);
    }
   } else if(state==128) {
    /* 27A08..27A60: reserve left-hand item; selection is not cleared here. */
    sint32 selected=player_byte(actor+131);
    uint32 item=0x8009a3c8+36u*(uint32)selected;
    if(ff_u32(item+8)==1)FUN_800237CC(actor,table,119);
    else {
     ff_w8(actor+125,(uint8)selected);ff_w32(item+8,1);
     FUN_800237CC(actor,table,129);
    }
   } else if(state==147||state==154||state==251||state==180) {
    /* 275C0..276C0: recover after landing, or begin the continue countdown. */
    ff_w16(actor+74,0);
    if(ff_s16(actor+66)>0) {
     ff_w32(actor+176,ff_u32(0x80093dd0)+45u);
     FUN_800237CC(actor,table,state==154?150:149);
    } else if(ff_u32(0x800940b0)==1) {
     if(!ff_u32(0x80093dd4)){ff_w32(0x80093dd4,1);ff_w32(0x80093d4c,0);}
    } else if(ff_s16(0x800b435a)<=0&&ff_s16(0x800b444e)<=0&&!ff_u32(0x80093dd4)) {
     ff_w32(0x80093dd4,1);ff_w32(0x80093d4c,0);
    }
   } else if(state==119) {
    /* 27820..278C4: finish pickup; kind512 consumes a healing item. */
    sint32 selected;
    FUN_800237CC(actor,table,69);
    selected=player_byte(actor+126);
    if(selected!=-1) {
     uint32 item=0x8009a3c8+36u*(uint32)selected;
     if(ff_u32(item)==512) {
      FUN_80056670(122,255,6144);
      ff_w16(item+30,0);
      FUN_800237CC(actor,table,132);
      ff_w16(actor+66,(uint16)(ff_s16(actor+66)+32));
      if(ff_s16(actor+66)>=ff_s16(actor+72))ff_w16(actor+66,(uint16)(ff_s16(actor+72)-4));
     }
    }
   } else {
    /* 26D80 ->26DC4: states25/92 finish with a half-turn for every type. */
    if(state==25||state==92||state==150){uint16 facing=(uint16)ff_s16(actor+56)^1024u;ff_w16(actor+56,facing);ff_w16(actor+62,(uint16)((uint32)facing<<5));}
    /* State125 follows the MIPS default at27CB0: idle69, no half-turn. */
    else if(state!=122&&state!=124&&state!=125&&state!=129&&state!=192&&state!=195&&state!=149&&state!=0&&state!=1&&state!=2&&state!=24&&state!=26&&state!=27&&state!=28&&state!=33&&state!=35&&state!=39&&state!=66&&state!=67&&state!=158&&state!=160&&state!=69&&(uint32)(state-133)>=2&&(uint32)(state-152)>=2)player_wip_zero(0x80026d78);
    FUN_800237CC(actor,table,69);
   }
  }
  timer=player_byte(actor+128);if(timer>0)ff_w8(actor+128,(uint8)(timer-1));
  timer=player_byte(actor+118);if(timer>0)ff_w8(actor+118,(uint8)(timer-1));
  timer=(sint32)ff_u32(actor+36);if(timer>0)ff_w32(actor+36,(uint32)(timer-1));
  ff_w32(0x80093ff8+4u*(uint32)index,buttons);
next_actor:
  ++index;actor+=244;
 } while(index<(sint32)ff_u32(0x800940b0));
 return 0;
}
