#include "ff.h"
GDB_CALL uint32 FUN_80057788(uint32 first,uint32 second) {
 FF_FUNCTION_MARKER(0x80057788u,"GAME.EXE");
 uint32 start[3],end[3],step[3],current[3],point[3],saved[16],packet=0x801fd220;
 sint32 count,index,depth,old_depth,width,new_width;int i,j;
 static const uint16 styles[4]={1373,827,1647,553};
 for(i=0;i<3;++i){start[i]=ff_u32(first+4*i);end[i]=ff_u32(second+4*i);current[i]=start[i];}
 count=FUN_800163F4((sint32)start[0]>>16,(sint32)start[2]>>16,(sint32)end[0]>>16,(sint32)end[2]>>16)>>8;if(count<3)count=3;
 for(i=0;i<3;++i)step[i]=(uint32)((sint32)(end[i]-start[i])/count)<<1;
 for(i=0;i<16;++i)saved[i]=ff_u32(0x801fd200+4*i);
 FUN_80010040();FUN_800100C4((sint32)(start[0]-ff_u32(0x8008d4a4)),(sint32)(start[1]-ff_u32(0x8008d4a8)),(sint32)(start[2]-ff_u32(0x8008d4ac)));FUN_800120BC(0,0,0,0x801fd200);FUN_80010028();
 for(index=1;index<=count;++index){
  for(i=0;i<3;++i)current[i]+=step[i];
  if(index==count){for(i=0;i<3;++i)point[i]=end[i];}
  else for(i=0;i<3;++i)point[i]=current[i]+(((ff_rand_8006D348()&127)-64u)<<17);
  FUN_80010040();FUN_800100C4((sint32)(point[0]-ff_u32(0x8008d4a4)),(sint32)(point[1]-ff_u32(0x8008d4a8)),(sint32)(point[2]-ff_u32(0x8008d4ac)));FUN_800120BC(0,0,0,0x801fd210);FUN_80010028();
  if((sint32)ff_u32(0x801fd20c)<4&&(sint32)ff_u32(0x801fd21c)<4){
   old_depth=(sint32)ff_u32(0x801fd208);depth=(sint32)ff_u32(0x801fd218);
   if(!old_depth||!depth)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__); /* Original57A2C/57A60 BREAK. */
   width=65536/old_depth;new_width=65536/depth;
   ff_w16(packet+4,(uint16)ff_s16(0x801fd200));ff_w16(packet+6,(uint16)((uint16)ff_s16(0x801fd204)-(uint32)width));
   ff_w16(packet+8,(uint16)ff_s16(0x801fd210));ff_w16(packet+10,(uint16)((uint16)ff_s16(0x801fd214)-(uint32)new_width));
   ff_w16(packet+12,(uint16)ff_s16(0x801fd210));ff_w16(packet+14,(uint16)((uint16)ff_s16(0x801fd214)+(uint32)new_width));
   ff_w16(packet,(uint16)ff_s16(0x801fd200));ff_w16(packet+2,(uint16)((uint16)ff_s16(0x801fd204)+(uint32)width));
   if(index!=1&&index!=count)ff_rand_8006D348(); /* Unused result is still an RNG advance. */
   ff_w16(packet+16,831);depth=(sint32)ff_u32(0x801fd218)>>2;
   ff_w16(packet,(uint16)ff_s16(0x801fd210));ff_w16(packet+2,(uint16)ff_s16(0x801fd214));
   for(j=0;j<4;++j){
    uint32 target=packet+(j&1?0:4),base=packet+(j&1?4:0);sint32 delta;
    delta=FUN_80057700(depth);ff_w16(target,(uint16)((uint16)ff_s16(base)+(uint32)delta));
    delta=FUN_80057700(depth);ff_w16(target+2,(uint16)((uint16)ff_s16(base+2)+(uint32)delta));
    ff_w16(packet+16,styles[j]);FUN_80064638(packet,(uint32)depth,129);
   }
  }
  for(i=0;i<4;++i)ff_w32(0x801fd200+4*i,ff_u32(0x801fd210+4*i));
 }
 for(i=0;i<16;++i)ff_w32(0x801fd200+4*i,saved[i]);return 1;
}
GDB_CALL uint32 FUN_80046EF0(uint32 object) {
 FF_FUNCTION_MARKER(0x80046ef0u,"GAME.EXE");
 uint32 kind=ff_u32(object),bias,state;int visible=0;
 if(ff_s16(object+16))FUN_8001033C(ff_s16(object+16));if(ff_s16(object+20))FUN_800104A0(ff_s16(object+20));if(ff_s16(object+18))FUN_800101CC(ff_s16(object+18));
 bias=kind-207u<4u||kind==214||kind==211?512:0;state=ff_u32(0x80093d60);
 if(!state&&(sint32)ff_u32(object+12)<=(sint32)0xf2900000u)visible=1;
 if(!visible){
  state=ff_u32(0x80093d60);
  if((state-1u<2u||(!state&&ff_u32(0x80094674)))&&(sint32)ff_u32(object+12)>(sint32)0xf2900000u&&(sint32)ff_u32(object+4)>0x06000000)visible=1;
 }
 if(!visible){
  state=ff_u32(0x80093d60);if(((state==2&&ff_u32(0x80094674))||state-3u<2u||state==5)&&ff_u32(object+4)+0x0dffffffu<=0x13fffffeu)visible=1;
 }
 if(!visible){state=ff_u32(0x80093d60);if(((state==4&&ff_u32(0x80094674))||state-5u<2u)&&(sint32)ff_u32(object+4)<(sint32)0xf2000000u)visible=1;}
 if(visible)return ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,bias);return 0;
}
GDB_CALL uint32 FUN_8002A640(uint32 object) {
 FF_FUNCTION_MARKER(0x8002a640u,"GAME.EXE");
 sint32 amplitude,scale;uint32 offset;
 if(ff_u32(0x80093d58)==18&&(sint32)ff_u32(object+4)<(sint32)0xe5300000u)return 1;
 FUN_80010040();FUN_800100C4(0,0x02000000,0);FUN_8001033C(ff_s16(object+16));FUN_800104A0(-ff_s16(object+20));FUN_800101CC(ff_s16(object+18));
 amplitude=(sint32)(0u-ff_u32(object+8))>>18;if(amplitude>64)amplitude=64;offset=(ff_u32(0x80093dd0)<<4)&0xff0u;
 FUN_800100C4(0,(sint32)((uint32)(sint32)ff_s16(0x800802d0+offset)*(uint32)amplitude),0);ff_static_model_80010B18(ff_u32(0x80081c74),0,0);
 FUN_800100C4(0,0x00100000,0);FUN_8001033C((sint32)((uint32)(uint16)ff_s16(0x80093dd0)<<12));ff_static_model_80010B18(ff_u32(0x80081ff0),0,0);FUN_80010028();
 if(ff_u32(0x80093d58)==18)return 18;FUN_80010040();FUN_8001033C(ff_s16(object+16));FUN_800100C4(0,(sint32)ff_u32(object+8),(sint32)0xfec00000u);
 scale=((sint32)ff_u32(object+8)>>14)+16384;FUN_80010F94(scale,scale,scale);ff_static_model_80010B18(ff_u32(0x80082428),0,0);return FUN_80010028();
}
GDB_CALL uint32 FUN_8002C55C(uint32 object) {
 FF_FUNCTION_MARKER(0x8002c55cu,"GAME.EXE");
 uint32 z,color;sint32 shade;FUN_80010040();z=ff_u32(object+12)-((uint32)(sint32)ff_s16(0x800806d0+(((uint32)(sint32)ff_s16(object+26)<<3)&0xff8u))<<9);
 FUN_800100C4((sint32)(ff_u32(object+4)-ff_u32(0x8008d4a4)),(sint32)(ff_u32(object+8)-ff_u32(0x8008d4a8)),(sint32)(z-ff_u32(0x8008d4ac)));
 FUN_800101CC(32768);if(ff_s16(object+18))FUN_800101CC(ff_s16(object+18));if(ff_s16(object+20))FUN_800104A0(ff_s16(object+20));
 shade=((sint32)z>>19)-450;if(shade<0)shade=0;else shade>>=2;if(shade>128)shade=128;color=128u-(uint32)shade;
 ff_w32(0x8008d4bc,0x2c000000u+color*0x10101u);ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,0);return FUN_80010028();
}
GDB_CALL uint32 FUN_800454E0(uint32 object) {
 FF_FUNCTION_MARKER(0x800454e0u,"GAME.EXE");
 FUN_8001033C(ff_s16(object+16));if(ff_s16(object+20))FUN_800104A0(ff_s16(object+20));if(ff_s16(object+18))FUN_800101CC(ff_s16(object+18));
 FUN_800100C4((sint32)0xfb000000u,0,0);FUN_80011594(ff_u32(0x80081720+4u*ff_u32(object)),0,0);
 FUN_800100C4(0x05000000,0,0);FUN_80011594(ff_u32(0x80081720+4u*ff_u32(object)),0,0);
 FUN_800100C4(0x05000000,0,0);return FUN_80011594(ff_u32(0x80081720+4u*ff_u32(object)),0,0);
}
GDB_CALL uint32 FUN_8005B404(uint32 object) {
 FF_FUNCTION_MARKER(0x8005b404u,"GAME.EXE");
 sint32 phase=ff_s16(object+28),height;if(phase>=3)phase=2;
 height=ff_s16(0x800802d0+((uint32)phase<<9))>>9;ff_w16(object+26,0);ff_w16(object+22,0);
 if(ff_s16(object+16))FUN_8001033C(ff_s16(object+16));ff_static_model_80010B18(ff_u32(0x80081830),0,8192);
 FUN_800100C4(0,(sint32)((64u-(uint32)height)<<16),0);
 if(ff_s16(object+20))FUN_800104A0(ff_s16(object+20));if(ff_s16(object+18))FUN_800101CC(ff_s16(object+18));
 return ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,0);
}
GDB_CALL uint32 FUN_80032C4C(uint32 object) {
 FF_FUNCTION_MARKER(0x80032c4cu,"GAME.EXE");
 FUN_80010040();if((sint32)ff_u32(object+4)>0x13000000)FUN_8001033C(32768);
 FUN_80011594(ff_u32(0x80081720+4u*ff_u32(object)),0,0);
 FUN_800100C4(0,0,(sint32)0xf1a00000u);FUN_80011594(ff_u32(0x80081bdc),0,0);
 FUN_800100C4(0,0,0x07300000);FUN_80011594(ff_u32(0x80081be0),0,0);
 FUN_800100C4(0,0,0x0e600000);FUN_80011594(ff_u32(0x80081bdc),0,0);
 FUN_800100C4(0,0,0x07300000);FUN_80011594(ff_u32(0x80081be0),0,0);return FUN_80010028();
}
GDB_CALL uint32 FUN_8005B598(uint32 object) {
 FF_FUNCTION_MARKER(0x8005b598u,"GAME.EXE");
 sint32 phase=ff_s16(object+28),height;if(phase>=3)phase=2;
 height=(5*ff_s16(0x800802d0+((uint32)phase<<9)))>>12;ff_w16(object+26,0);ff_w16(object+22,0);
 if(ff_s16(object+16))FUN_8001033C(ff_s16(object+16));FUN_800100C4(0,(sint32)((40u-(uint32)height)<<16),0);
 if(ff_s16(object+20))FUN_800104A0(ff_s16(object+20));if(ff_s16(object+18))FUN_800101CC(ff_s16(object+18));
 return ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,0);
}
GDB_CALL uint32 FUN_8005AD54(uint32 object) {
 FF_FUNCTION_MARKER(0x8005ad54u,"GAME.EXE");
 uint32 kind=ff_u32(object);if(ff_s16(object+16))FUN_8001033C(ff_s16(object+16));
 FUN_80010040();FUN_800100C4(0,(sint32)ff_u32(object+8),0);
 ff_static_model_80010B18(ff_u32(0x80081720+4u*(kind==87?89u:106u)),0,8192);FUN_80010028();
 if(ff_s16(object+20))FUN_800104A0(ff_s16(object+20));if(ff_s16(object+18))FUN_800101CC(ff_s16(object+18));
 return ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,0);
}
GDB_CALL uint32 FUN_8002CAC4(uint32 object) {
 FF_FUNCTION_MARKER(0x8002cac4u,"GAME.EXE");
 FUN_80010040();FUN_8001033C((sint32)((uint32)(sint32)ff_s16(object+16)<<5));
 FUN_800104A0(-ff_s16(object+20));FUN_800101CC(ff_s16(object+18));
 ff_static_model_80010B18(ff_u32(0x80081968),0,0);
 FUN_80010040();FUN_800101CC((sint32)((uint32)(sint32)ff_s16(object+18)<<3));
 ff_static_model_80010B18(ff_u32(0x8008196c),0,0);FUN_80010028();
 if((sint32)ff_u32(0x80093dd0)>=631){FUN_800100C4(0,(sint32)0xfe9a0000u,0x1000000);FUN_8001033C(16384);ff_static_model_80010B18(ff_u32(0x80081970),0,0);}
 return FUN_80010028();
}
static void object_rotations(uint32 object);
GDB_CALL uint32 FUN_80032818(uint32 object) {
 FF_FUNCTION_MARKER(0x80032818u,"GAME.EXE");
 uint32 kind=ff_u32(object),state,mode; sint32 x,z,bias;int draw=0;
 object_rotations(object);bias=(kind-207u<4u||kind==214||kind==211)?512:0;
 state=ff_u32(0x80093d60);mode=ff_u32(0x80094674);x=(sint32)ff_u32(object+4);z=(sint32)ff_u32(object+12);
 if(!state&&z<=(sint32)0xf2900000u)draw=1;
 else if(((!state&&mode)||state-1u<3u)&&z>(sint32)0xf2900000u&&x>0x06000000)draw=1;
 else if(((state==2&&mode)||state-3u<3u)&&(uint32)x+0x0dffffffu<=0x13fffffeu)draw=1;
 else if(((state==4&&mode)||state-5u<2u)&&x<(sint32)0xf2000000u)draw=1;
 if(draw)return ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,bias);return 0;
}
GDB_CALL uint32 FUN_80032A48(uint32 object) {
 FF_FUNCTION_MARKER(0x80032a48u,"GAME.EXE");
 sint16 angle=(sint16)FUN_8001614C(ff_s16(object+6),ff_s16(object+14),ff_s16(0x800b4342),ff_s16(0x800b434a));
 ff_w16(object+16,(uint16)angle);FUN_8001033C(angle);return FUN_80032818(object);
}
GDB_CALL uint32 FUN_80032D70(uint32 object) {
 FF_FUNCTION_MARKER(0x80032d70u,"GAME.EXE");
 if((sint32)ff_u32(0x80094674)<=0&&!FUN_800473B0((sint32)0xee000000u))return 0;
 object_rotations(object);return ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,0);
}
GDB_CALL sint32 FUN_80032E20(uint32 object) {
 FF_FUNCTION_MARKER(0x80032e20u,"GAME.EXE");
 sint32 result;if(ff_s16(object+16))FUN_8001033C(ff_s16(object+16));
 FUN_80011594(ff_u32(0x80081720+4u*ff_u32(object)),0,0);
 FUN_800100C4(0,(sint32)((uint32)(sint32)ff_s16(object+26)<<16),0x00d00000);FUN_80011594(ff_u32(0x80081d44),0,0);
 result=ff_s16(0x800802d0+((ff_u32(0x80093dd0)<<5)&0x7e0u))>>7;ff_w16(object+26,(uint16)result);return result;
}
static uint32 object_faded_model(uint32 object) {
 object_rotations(object);return FUN_80011594(ff_u32(0x80081720+4u*ff_u32(object)),0,0);
}
GDB_CALL uint32 FUN_8005B71C(uint32 object) {
 FF_FUNCTION_MARKER(0x8005b71cu,"GAME.EXE");
 return object_faded_model(object);
}
GDB_CALL uint32 FUN_8005C0E8(uint32 object) {
 FF_FUNCTION_MARKER(0x8005c0e8u,"GAME.EXE");
 FUN_80012190();return object_faded_model(object);
}
GDB_CALL uint32 FUN_800428EC(uint32 object) {
 FF_FUNCTION_MARKER(0x800428ecu,"GAME.EXE");
 uint32 state=ff_u32(0x80093d60);if((sint32)state<=0)return state;return object_faded_model(object);
}
GDB_CALL uint32 FUN_80042848(uint32 object) {
 FF_FUNCTION_MARKER(0x80042848u,"GAME.EXE");
 if(ff_u32(0x80093d60)>=2u)return 0;return object_faded_model(object);
}
GDB_CALL uint32 FUN_80032D44(void) {
 FF_FUNCTION_MARKER(0x80032d44u,"GAME.EXE");
 return FUN_80011594(ff_u32(0x80081b84),0,1024);
}
static sint32 ff_project_local(sint32 x,sint32 y,sint32 z,sint32 dest[4]);
GDB_CALL uint32 FUN_8005B844(uint32 object) {
 FF_FUNCTION_MARKER(0x8005b844u,"GAME.EXE");
 sint32 brightness=(ff_s16(0x800802d0+((ff_u32(0x80093dd0)<<5)&0x7e0u))>>9)+65,point[4],distance,amount,scale;uint32 cutoff;
 ff_project_local(0,0,0,point);cutoff=ff_u32(0x80011590);distance=(sint32)((uint32)point[2]-cutoff);if(distance>0)return cutoff;
 distance=(sint32)((uint32)distance+2048u);if(distance<0)distance=0;brightness=(sint32)((128u-(uint32)(distance>>4))*(uint32)brightness)>>7;
 amount=(sint32)ff_u32(0x80094338);
 if(amount>0){
  scale=(sint32)((uint32)amount<<14)/624;FUN_80010040();FUN_80010F94(16384,scale,16384);
  ff_w32(0x8008d4bc,0x2c000000u+(uint32)brightness*0x10101u);ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,-64);FUN_80010028();
 }else scale=(sint32)((0u-ff_u32(0x80094338))<<14)/624;
 FUN_800100C4(0,0x02700000,0);FUN_80010F94(16384,(sint32)(16384u-(uint32)scale),16384);
 ff_w32(0x8008d4bc,0x2c000000u+((uint32)brightness<<16)+(uint32)(brightness>>1)*257u);
 return ff_static_model_80010B18(ff_u32(0x80081de8),0,-64);
}
GDB_CALL uint32 FUN_8005BA28(uint32 object) {
 FF_FUNCTION_MARKER(0x8005ba28u,"GAME.EXE");
 sint32 point[4],distance;uint32 phase,x,z,cutoff,color;
 FUN_80010040();phase=(uint32)(sint32)ff_s16(object+26)+ff_u32(0x80093dd0);
 x=((uint32)(sint32)ff_s16(0x800802d0+((phase<<6)&0xfc0u))<<5)+ff_u32(object+4);
 z=((uint32)(sint32)ff_s16(0x800802d0+((phase<<5)&0xfe0u))<<5)+ff_u32(object+12);
 FUN_800100C4((sint32)(x-ff_u32(0x8008d4a4)),(sint32)(ff_u32(object+8)-ff_u32(0x8008d4a8)),(sint32)(z-ff_u32(0x8008d4ac)));
 ff_project_local(0,0,0,point);FUN_80010028();if(point[3])return (uint32)point[3];
 cutoff=ff_u32(0x80011590);distance=(sint32)((uint32)point[2]-cutoff);if(distance>0)return cutoff;
 distance=(sint32)((uint32)distance+2048u);if(distance<0)distance=0;
 color=(16u-(uint32)(distance>>7))*1057u;
 if(point[2]>2560)return FUN_800121A8(point[0],point[1],point[2],color);
 if(point[2]<1536)return FUN_800122E0(point[0],point[1],point[2],color);
 return FUN_80012240(point[0],point[1],point[2],color);
}
GDB_CALL uint32 FUN_8005CEE4(uint32 object) {
 FF_FUNCTION_MARKER(0x8005cee4u,"GAME.EXE");
 sint32 a[4],b[4];FUN_80010040();FUN_800100C4((sint32)(ff_u32(object+4)-ff_u32(0x8008d4a4)),(sint32)(ff_u32(object+8)-ff_u32(0x8008d4a8)),(sint32)(ff_u32(object+12)-ff_u32(0x8008d4ac)));
 ff_project_local(0,0,0,a);if(!a[3]){ff_project_local(ff_s16(object+16),ff_s16(object+18),ff_s16(object+20),b);if(!b[3]){
  ff_w32(0x8008d4bc,0x2c303040);a[3]=128;b[3]=256;FUN_8006A1B4(b,a,ff_s16(0x8009adce+2u*(uint32)(sint32)ff_s16(object+22)));
  ff_w32(0x8008d4bc,0x2c181820);a[3]=176;b[3]=336;FUN_8006A1B4(b,a,ff_s16(0x8009adce+2u*(uint32)(sint32)ff_s16(object+22)));
 }}return FUN_80010028();
}
GDB_CALL uint32 FUN_8005D024(uint32 object) {
 FF_FUNCTION_MARKER(0x8005d024u,"GAME.EXE");
 sint32 a[4],b[4];uint32 color;FUN_80010040();FUN_800100C4((sint32)(ff_u32(object+4)-ff_u32(0x8008d4a4)),(sint32)(ff_u32(object+8)-ff_u32(0x8008d4a8)),(sint32)(ff_u32(object+12)-ff_u32(0x8008d4ac)));
 ff_project_local(0,0,0,a);ff_project_local(ff_s16(object+16),ff_s16(object+18),ff_s16(object+20),b);
 a[2]=(sint32)((uint32)a[2]-32u);b[2]=(sint32)((uint32)b[2]-32u);color=ff_u32(object+28);ff_w32(0x8008d4bc,color);
 ff_w32(0x8008d4c0,(uint32)((sint32)color>>24));ff_w32(0x8008d4bc,(color&0xffffffu)|0x2c000000u);
 a[3]=ff_s16(object+24);b[3]=ff_s16(object+26);FUN_8006A1B4(a,b,ff_s16(0x8009acc8+2u*(uint32)(sint32)ff_s16(object+22)));return FUN_80010028();
}
/* PsyQ 72480/724F0 host boundary: SQR without SF, even LZCR and
 * reciprocal-sqrt table, then GPF without SF. COP2 register side effects WIP. */
static void ff_normalize_screen(sint32 x,sint32 y,sint16 output[2]) {
 sint32 sx=(sint16)x,sy=(sint16)y,sum,scaled,shift;uint32 bits,lz=0,index,factor;
 int64_t total=(int64_t)sx*sx+(int64_t)sy*sy;
 if(total>2147483647)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
 sum=(sint32)total;bits=(uint32)sum;if(sum<0)bits=~bits;
 if(!bits)lz=32;else{while(!(bits&0x80000000u)){lz++;bits<<=1;}}
 lz&=~1u;shift=(31-(sint32)lz)>>1;
 scaled=lz>=24?(sint32)((uint32)sum<<((lz-24)&31u)):sum>>((24-lz)&31u);
 index=((uint32)scaled-64u)*2;factor=(uint16)ff_s16(0x800874b4+index);
 output[0]=(sint16)((sint32)((uint32)sx*factor)>>((uint32)shift&31u));
 output[1]=(sint16)((sint32)((uint32)sy*factor)>>((uint32)shift&31u));
}
GDB_CALL uint32 FUN_8006A1B4(const sint32 *a,const sint32 *b,sint32 sprite) {
 FF_FUNCTION_MARKER(0x8006a1b4u,"GAME.EXE");
 uint32 packet=ff_u32(0x8008d4b0),texture=0x800a8768+12u*(uint32)sprite,depth=(uint32)((sint32)((uint32)a[2]+(uint32)b[2])>>3),color;
 sint16 normal[2];sint32 numerator,width,dx,dy,ot;uint32 pass;const sint32 *point;
 color=ff_u32(0x8008d4bc);if(ff_u32(texture)&1u)color|=0x02000000u;
 ff_w32(packet+4,color);ff_w32(packet+12,ff_u32(texture)&0xfffffffeu);ff_w32(0x8008d4bc,0x2d000000);
 if(depth>=2048)return 0;
 ff_normalize_screen((sint32)((uint32)b[0]-(uint32)a[0]),(sint32)((uint32)b[1]-(uint32)a[1]),normal);
 ff_w32(0x8008d4b0,ff_u32(0x8008d4b0)+40);ff_w32(packet+20,ff_u32(texture+4));ff_w16(packet+28,(uint16)ff_s16(texture+8));ff_w16(packet+36,(uint16)ff_s16(texture+10));
 if(ff_u32(0x8008d4c0)){ff_w16(packet+22,(uint16)(((uint32)(uint16)ff_s16(packet+22)&0xff9fu)|(((uint32)(uint16)ff_s16(0x8008d4c0)-1u)<<5)));ff_w32(packet+4,ff_u32(packet+4)|0x02000000u);}
 for(pass=0;pass<2;pass++){uint32 uv=texture+(pass?8u:0u),p=packet+(pass?24u:8u);point=pass?b:a;
  width=((sint32)*(sint8*)ff_ptr(uv+(pass?2u:4u),1)-(sint32)*(sint8*)ff_ptr(uv,1))>>1;
  numerator=(sint32)((uint32)width*(uint32)point[3]);if(!point[2]||(point[2]==-1&&(uint32)numerator==0x80000000u))ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
  width=numerator/point[2];dx=(sint32)((uint32)width*(uint32)(sint32)normal[1])>>12;dy=(sint32)((uint32)width*(uint32)-(sint32)normal[0])>>12;
  ff_w16(p,(uint16)((uint32)point[0]-(uint32)dx));ff_w16(p+2,(uint16)((uint32)point[1]-(uint32)dy));ff_w16(p+8,(uint16)((uint32)point[0]+(uint32)dx));ff_w16(p+10,(uint16)((uint32)point[1]+(uint32)dy));
 }
 ot=ff_s16(ff_u32(0x8008d4d8)+2*depth)>>2;return FUN_80011E28(ff_u32(0x8008d4b4)+4u*(uint32)ot,packet,0x09000000);
}
GDB_CALL uint32 FUN_8005CDBC(uint32 object) {
 FF_FUNCTION_MARKER(0x8005cdbcu,"GAME.EXE");
 sint32 a[4],b[4];FUN_80010040();FUN_800100C4((sint32)(ff_u32(object+4)-ff_u32(0x8008d4a4)),(sint32)(ff_u32(object+8)-ff_u32(0x8008d4a8)),(sint32)(ff_u32(object+12)-ff_u32(0x8008d4ac)));
 ff_project_local(0,0,0,a);ff_project_local(ff_s16(object+16),ff_s16(object+18),ff_s16(object+20),b);
 ff_w32(0x8008d4bc,0x2c303040);a[3]=128;b[3]=256;FUN_8006A1B4(b,a,ff_s16(0x8009ad9c+2u*(uint32)(sint32)ff_s16(object+22)));
 ff_w32(0x8008d4bc,0x2c181820);a[3]=176;b[3]=336;FUN_8006A1B4(b,a,ff_s16(0x8009ad9c+2u*(uint32)(sint32)ff_s16(object+22)));return FUN_80010028();
}
GDB_CALL uint32 FUN_8005DAD4(uint32 object) {
 FF_FUNCTION_MARKER(0x8005dad4u,"GAME.EXE");
 sint32 position[4];FUN_80010040();FUN_8005C740(object);ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,0);
 if(ff_s16(object+22)>0){ff_project_local(0,64,0,position);
  if(!position[3]){position[3]=ff_u32(0x80093dd0)&1u?448:256;ff_w32(0x8008d4bc,0x2cffffffu);FUN_80012594(position,ff_s16(0x8009ad4c));}
  if(ff_s16(object+22)>0)FUN_8002BDD0(object);
 }return FUN_80010028();
}
GDB_CALL uint32 FUN_8005D1FC(uint32 object) {
 FF_FUNCTION_MARKER(0x8005d1fcu,"GAME.EXE");
 sint32 position[4];uint32 kind;FUN_80010040();FUN_8005C740(object);ff_project_local(0,0,0,position);
 if(!position[3]){kind=ff_u32(object);
  if(kind==892)position[3]=2048;else if(kind==893)position[3]=1024;else if(kind==894)position[3]=256;else if(kind==895)position[3]=128;
  ff_w32(0x8008d4bc,0x2c404040u);FUN_80012594(position,ff_s16(0x8009ad5c+2u*(uint32)(ff_s16(object+22)>>1)));
 }return FUN_80010028();
}
GDB_CALL uint32 FUN_8005C598(uint32 object) {
 FF_FUNCTION_MARKER(0x8005c598u,"GAME.EXE");
 sint32 position[4];FUN_80010040();
 FUN_800100C4((sint32)(ff_u32(object+4)-ff_u32(0x8008d4a4)),(sint32)(ff_u32(object+8)-ff_u32(0x8008d4a8)),(sint32)(ff_u32(object+12)-ff_u32(0x8008d4ac)));
 ff_project_local(0,0,0,position);
 if(!position[3])FUN_800121A8(position[0],position[1],position[2],(uint32)(sint32)ff_s16(0x800800b0+2u*(uint32)(ff_s16(object+26)>>2)));
 return FUN_80010028();
}
GDB_CALL uint32 FUN_8005D53C(uint32 object) {
 FF_FUNCTION_MARKER(0x8005d53cu,"GAME.EXE");
 sint32 position[4];uint32 color;FUN_80010040();FUN_8005C740(object);ff_project_local(0,0,0,position);
 if(!position[3]){
  position[3]=256;ff_w32(0x8008d4bc,0x2c404040u);color=64u-((uint32)(sint32)ff_s16(object+22)<<2);
  ff_w32(0x8008d4bc,0x2c000000u+(color<<16)+(color<<8)+color);
  position[3]=(sint32)(((uint32)(sint32)ff_s16(object+22)<<4)+256u);FUN_80012594(position,ff_s16(0x8009ad5a));
 }return FUN_80010028();
}
GDB_CALL uint32 FUN_8005B4F8(uint32 object) {
 FF_FUNCTION_MARKER(0x8005b4f8u,"GAME.EXE");
 if(ff_s16(object+16))FUN_8001033C(ff_s16(object+16));FUN_800100C4(0,0x280000,0);
 if(ff_s16(object+20))FUN_800104A0(ff_s16(object+20));if(ff_s16(object+18))FUN_800101CC(ff_s16(object+18));
 return ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,0);
}
GDB_CALL uint32 FUN_8005B350(uint32 object) {
 FF_FUNCTION_MARKER(0x8005b350u,"GAME.EXE");
 if(ff_s16(object+16))FUN_8001033C(ff_s16(object+16));
 ff_static_model_80010B18(ff_u32(0x80081830),0,8192);FUN_800100C4(0,0x400000,0);
 if(ff_s16(object+20))FUN_800104A0(ff_s16(object+20));if(ff_s16(object+18))FUN_800101CC(ff_s16(object+18));
 return ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,0);
}
GDB_CALL uint32 FUN_80044CD4(uint32 object) {
 FF_FUNCTION_MARKER(0x80044cd4u,"GAME.EXE");
 FUN_80010040();FUN_8001033C(ff_s16(object+16));
 if(ff_s16(object+20))FUN_800104A0(ff_s16(object+20));if(ff_s16(object+18))FUN_800101CC(ff_s16(object+18));
 ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,0);
 FUN_8001033C((sint32)((uint32)(uint16)ff_s16(0x80093dd0)<<12));ff_static_model_80010B18(ff_u32(0x80081ff0),0,0);return FUN_80010028();
}
GDB_CALL uint32 FUN_8005CD1C(uint32 object) {
 FF_FUNCTION_MARKER(0x8005cd1cu,"GAME.EXE");
 sint32 position[4];FUN_80010040();FUN_8005C740(object);ff_project_local(0,0,0,position);
 if(!position[3]){position[3]=ff_u32(0x80093dd0)&1u?704:512;ff_w32(0x8008d4bc,0x2cffffffu);FUN_80012594(position,ff_s16(0x8009ad4c));}
 return FUN_80010028();
}
GDB_CALL uint32 FUN_8005BBAC(uint32 object) {
 FF_FUNCTION_MARKER(0x8005bbacu,"GAME.EXE");
 sint32 position[4];FUN_80010040();
 FUN_800100C4((sint32)(ff_u32(object+4)-ff_u32(0x8008d4a4)),(sint32)(ff_u32(object+8)-ff_u32(0x8008d4a8)),(sint32)(ff_u32(object+12)-ff_u32(0x8008d4ac)));
 ff_project_local(0,0,0,position);
 if(!position[3]){FUN_800121A8(position[0],position[1],position[2],0x4210);FUN_800121A8((sint32)((uint32)position[0]+1u),position[1],position[2],0x4210);}
 return FUN_80010028();
}
/* 11594: distance-faded quads and lines; GTE projection uses the port boundary. */
GDB_CALL uint32 FUN_80011590(uint32 model,uint32 unused,sint32 bias) {
 FF_FUNCTION_MARKER(0x80011590u,"GAME.EXE");
 /* No resolved call targets11590; the word is normally read as cutoff data.
  * If entered as code, SLL V0,ZERO,16 falls through11594. V0 is overwritten
  * at11598 before any read, so the existing11594 body is the exact successor. */
 return FUN_80011594(model,unused,bias);
}
GDB_CALL uint32 FUN_80011594(uint32 model,uint32 unused,sint32 bias) {
 FF_FUNCTION_MARKER(0x80011594u,"GAME.EXE");
 sint32 count=(sint32)ff_u32(model+4),position[4],cutoff;
 uint32 vertices=ff_u32(model),xy,depths,total,n,face,left,p,ot,depth_table;
 (void)unused;
 FUN_80010170();
 xy=count<193?0x1f800000:0x1fe800;
 depths=count<127?0x1f800200:count<193?0x1fe400:count<385?0x1f800000:0x1fe400;
 total=count<=6?6:((uint32)count+2u)/3u*3u;
 for(n=0;n<total;n++){
  uint32 v=vertices+8u*n;
  ff_project_local(ff_s16(v),ff_s16(v+2),ff_s16(v+4),position);
  ff_w32(xy+4u*n,((uint32)position[0]&65535u)|((uint32)position[1]<<16));
  ff_w16(depths+2u*n,(uint16)position[2]);
 }
 /* 11590 is also read as the 0x1400 distance cutoff; see boundary audit. */
 face=ff_u32(model+8);left=ff_u32(model+12);cutoff=(sint32)ff_u32(0x80011590);
 depth_table=ff_u32(0x8008d4d8);ot=ff_u32(0x8008d4b4);p=ff_u32(0x8008d4b0);
 do {
  uint32 packed=ff_u32(face),second=ff_u32(face+4),i0=packed&65535u,i1=packed>>16;
  sint32 d0=ff_s16(depths+(i0>>1)),depth;--left;
  if((sint32)second<0){
   depth=(d0+ff_s16(depths+(i1>>1)))>>3;
   if(depth>0&&depth<=8188){
    uint32 command=ff_u32(face+8),s0=ff_u32(xy+i0),s1=ff_u32(xy+i1);
    uint32 bucket=ot+(uint32)(sint32)ff_s16(depth_table+2u*(uint32)depth);
    ff_w32(p+4,command);ff_w32(p+8,s0);ff_w32(p+12,s1);
    ff_w32(p,ff_u32(bucket)|0x03000000u);ff_w32(bucket,p);p+=16;
   }
  } else if(d0>0){
   uint32 i2=second&65535u,i3=second>>16,color=0x2d000000u;
   sint32 d1=ff_s16(depths+(i2>>1));
   if(d1>0){
    sint32 delta;depth=(sint32)((uint32)(d0+d1)+(uint32)bias)>>1;delta=(sint32)((uint32)depth-(uint32)cutoff);
    if(delta<=0){
     sint32 fade=(sint32)((uint32)delta+2048u);
     if(fade>=0){uint32 shade=128u-(uint32)(fade>>4);color=0x2c000000u|shade|(shade<<8)|(shade<<16);}
     depth>>=2;
     if(depth<=8192){
      uint32 s0=ff_u32(xy+i0),s1=ff_u32(xy+i1),s2=ff_u32(xy+i2),s3=ff_u32(xy+i3);
      if(((s0&65535u)<320||(s1&65535u)<320||(s2&65535u)<320||(s3&65535u)<320)&&
         ((sint32)s0>0||(sint32)s1>0||(sint32)s2>0||(sint32)s3>=0)){
       uint32 offset=(uint32)(sint32)ff_s16(depth_table+2u*(uint32)depth),flags=ff_u32(face+8);
       sint32 area=(sint32)((sint64)(sint16)s0*((sint16)(s1>>16)-(sint16)(s2>>16))+
         (sint64)(sint16)s1*((sint16)(s2>>16)-(sint16)(s0>>16))+
         (sint64)(sint16)s2*((sint16)(s0>>16)-(sint16)(s1>>16)));
       if((flags&1u)||area>=0){
        uint32 texture=0x800a8768+(flags>>16),uv,mode,end,semi,bucket=ot+offset;
        if(flags&2u){ff_w32(p+8,s1);ff_w32(p+16,s0);ff_w32(p+24,s2);ff_w32(p+32,s3);}
        else{ff_w32(p+8,s0);ff_w32(p+16,s1);ff_w32(p+24,s3);ff_w32(p+32,s2);}
        uv=ff_u32(texture);mode=ff_u32(texture+4);end=ff_u32(texture+8);semi=uv&1u;
        ff_w32(p+12,uv-semi);ff_w32(p+20,mode);ff_w16(p+28,(uint16)end);ff_w16(p+36,(uint16)(end>>16));
        ff_w32(p+4,(semi<<25)|color);ff_w32(p,ff_u32(bucket)|0x09000000u);ff_w32(bucket,p&0xffffffu);p+=40;
       }
      }
     }
    }
   }
  }
  face+=12;
 }while(left);
 ff_w32(0x8008d4bc,0x2d000000u);ff_w32(0x8008d4b0,p);return 0x8008d4b0;
}
GDB_CALL uint32 FUN_800450F8(uint32 object) {
 FF_FUNCTION_MARKER(0x800450f8u,"GAME.EXE");
 sint32 position[4],z;uint32 i;
 FUN_80010040();z=ff_s16(object+14);
 FUN_800100C4((sint32)(0u-ff_u32(0x8008d4a4)),(sint32)(0u-ff_u32(0x8008d4a8)),(sint32)(0u-ff_u32(0x8008d4ac)));
 ff_project_local(ff_s16(object+6),ff_s16(object+10),z,position);
 for(i=0;i<32;i++)FUN_80044D88();return FUN_80010028();
}
GDB_CALL uint32 FUN_8004277C(uint32 object) {
 FF_FUNCTION_MARKER(0x8004277cu,"GAME.EXE");
 if((sint32)ff_u32(object+8)<=(sint32)0xe0000000u)return 0;
 FUN_80010040();FUN_8001033C(ff_s16(object+16));FUN_800104A0(ff_s16(object+20));FUN_800101CC(ff_s16(object+18));
 ff_static_model_80010B18(ff_u32(0x800821dc),0,0);
 FUN_8001033C((sint32)((uint32)(uint16)ff_s16(0x80093dd0)<<12));
 ff_static_model_80010B18(ff_u32(0x80081ff0),0,0);return FUN_80010028();
}
GDB_CALL uint32 FUN_8005B684(uint32 object) {
 FF_FUNCTION_MARKER(0x8005b684u,"GAME.EXE");
 sint32 position[4];uint32 model,color;ff_project_local(0,0,0,position);model=ff_u32(object);
 if((uint32)position[2]-1u>=0x2fffu)return 0;
 color=128u-(uint32)(position[2]>>7);ff_w32(0x8008d4bc,0x2c000000u+(color<<16)+(color<<8)+color);
 return ff_static_model_80010B18(ff_u32(0x80081720+4u*model),0,0);
}
GDB_CALL uint32 FUN_8005B7AC(uint32 object) {
 FF_FUNCTION_MARKER(0x8005b7acu,"GAME.EXE");
 sint32 position[4],shade;uint32 color;ff_project_local(0,0,0,position);shade=(position[2]>>2)-32;
 if(shade<=0)return 1;
 if(shade<128){color=(uint32)shade;ff_w32(0x8008d4bc,0x2c000000u+(color<<16)+(color<<8)+color);}
 return ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,0);
}
GDB_CALL uint32 FUN_8005D160(uint32 object) {
 FF_FUNCTION_MARKER(0x8005d160u,"GAME.EXE");
 sint32 position[4];FUN_80010040();FUN_8005C740(object);ff_project_local(0,0,0,position);
 if(!position[3]){position[3]=4096;ff_w32(0x8008d4bc,0x2c404040);
  FUN_80012594(position,ff_s16(0x8009ad5c+2u*(uint32)(ff_s16(object+22)>>2)));
 }
 return FUN_80010028();
}
GDB_CALL uint32 FUN_8005D608(uint32 object) {
 FF_FUNCTION_MARKER(0x8005d608u,"GAME.EXE");
 sint32 position[4];FUN_80010040();FUN_8005C740(object);ff_project_local(0,0,0,position);
 if(!position[3]){position[3]=512;FUN_80012594(position,ff_s16(0x8009ad4c));}
 return FUN_80010028();
}
GDB_CALL uint32 FUN_8005D7A4(uint32 object) {
 FF_FUNCTION_MARKER(0x8005d7a4u,"GAME.EXE");
 if((uint32)FUN_80012190()-257u>=0xeffu)return 0;
 ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,0);
 return ff_static_model_80010B18(ff_u32(0x80081724+4u*ff_u32(object)),0,0);
}
GDB_CALL uint32 FUN_8005BD7C(uint32 object) {
 FF_FUNCTION_MARKER(0x8005bd7cu,"GAME.EXE");
 sint32 phase=(sint32)ff_u32(0x80093da8)>>4;uint32 color,height,i;
 if(phase<433){phase-=416;if(phase<0)phase=0;color=4u*(uint32)phase+64u;ff_w32(0x8008d4bc,0x2c000000u+(color<<16)+(color<<8)+color);}
 ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,2048);
 height=0u-ff_u32(0x80093da8);
 for(i=0;i<4;i++){FUN_8005BD04(440,height);height+=1472u;}
 FUN_8005BD04(439,height);
 for(i=0;i<8;i++){height+=3456u;FUN_8005BD04(437u+(i&1u),height);}
 height+=3456u;FUN_8005BD04(441,height);return FUN_8005BD04(442,height+3456u);
}
GDB_CALL void FUN_80047108(uint32 object) {
 FF_FUNCTION_MARKER(0x80047108u,"GAME.EXE");
 object_rotations(object);
}
static void object_rotations(uint32 object) {
 sint32 angle=ff_s16(object+16);if(angle)FUN_8001033C(angle);
 angle=ff_s16(object+20);if(angle)FUN_800104A0(angle);
 angle=ff_s16(object+18);if(angle)FUN_800101CC(angle);
}
GDB_CALL uint32 FUN_8005AB18(uint32 object) {
 FF_FUNCTION_MARKER(0x8005ab18u,"GAME.EXE");
 if(!FUN_80011E5C())return 0;
 if(ff_s16(object+16))FUN_8001033C(ff_s16(object+16));
 return ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,0);
}
GDB_CALL uint32 FUN_8005BD04(uint32 model,uint32 height) {
 FF_FUNCTION_MARKER(0x8005bd04u,"GAME.EXE");
 if(height+0x17ffu>=0x1fffu)return 0;
 FUN_80010040();FUN_800100C4(0,(sint32)(height<<16),0);
 ff_static_model_80010B18(ff_u32(0x80081720+4u*model),0,2048);return FUN_80010028();
}
GDB_CALL uint32 FUN_8002A858(void) {
 FF_FUNCTION_MARKER(0x8002a858u,"GAME.EXE");
 if((sint32)ff_u32(0x80094004)>0x0fffffff)return 1;
 return ff_static_model_80010B18(ff_u32(0x80081cc0),0,0);
}
GDB_CALL uint32 FUN_8002A8FC(uint32 object) {
 FF_FUNCTION_MARKER(0x8002a8fcu,"GAME.EXE");
 uint32 angle;FUN_800100C4(0,0x02800000,0);
 angle=ff_u32(0x80093dd0);if((sint32)ff_u32(object+4)>0)angle=0u-angle;
 FUN_800104A0((sint32)(angle<<12));return ff_static_model_80010B18(ff_u32(0x80081d60),0,0);
}
static uint32 rising_particle(uint32 object,sint32 end_age) {
 uint16 age,velocity;uint32 delta;
 if(!ff_s16(object+22))ff_w16(object+26,(uint16)(ff_rand_8006D348()&3u));
 age=(uint16)((uint16)ff_s16(object+22)+1u);ff_w16(object+22,age);
 if((sint16)age==end_age)ff_w32(object,0xffffffffu);
 velocity=(uint16)((uint16)ff_s16(object+26)+1u);ff_w16(object+26,velocity);
 delta=(uint32)((sint32)((uint32)velocity<<16)>>1);ff_w32(object+8,ff_u32(object+8)-delta);return delta;
}
GDB_CALL uint32 FUN_8002ACE8(uint32 object) {
 FF_FUNCTION_MARKER(0x8002ace8u,"GAME.EXE");
 return rising_particle(object,16);
}
GDB_CALL uint32 FUN_8002AD7C(uint32 object) {
 FF_FUNCTION_MARKER(0x8002ad7cu,"GAME.EXE");
 return rising_particle(object,64);
}
GDB_CALL uint32 FUN_8002B150(uint32 object) {
 FF_FUNCTION_MARKER(0x8002b150u,"GAME.EXE");
 return rising_particle(object,16);
}
GDB_CALL uint32 FUN_80032680(uint32 object) {
 FF_FUNCTION_MARKER(0x80032680u,"GAME.EXE");
 if((uint16)((uint16)ff_s16(0x8008d490)+16384u)<=32768u)return 0;
 return ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,0);
}
GDB_CALL uint32 FUN_800326DC(uint32 object) {
 FF_FUNCTION_MARKER(0x800326dcu,"GAME.EXE");
 uint32 kind=ff_u32(object);return ff_static_model_80010B18(ff_u32(0x80081720+4u*kind),0,kind-133u<2u?1024:512);
}
GDB_CALL uint32 FUN_80032728(uint32 object) {
 FF_FUNCTION_MARKER(0x80032728u,"GAME.EXE");
 if(!ff_u32(0x80094674))return 0;
 return ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,512);
}
GDB_CALL uint32 FUN_800327B0(void) {
 FF_FUNCTION_MARKER(0x800327b0u,"GAME.EXE");
 sint32 phase=(sint32)(ff_u32(0x80093dd0)-270u);if(phase<0)phase=0;else if(phase>128)phase=128;
 FUN_80010F94(16384,16384,phase<<7);return ff_static_model_80010B18(ff_u32(0x800821b8),0,0);
}
GDB_CALL uint32 FUN_80032AA4(uint32 object) {
 FF_FUNCTION_MARKER(0x80032aa4u,"GAME.EXE");
 uint32 offset=ff_u32(0x80094674)<<3;if(ff_u32(object)==467)offset=0u-offset;
 FUN_800100C4((sint32)(offset<<16),0,0);return ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,256);
}
GDB_CALL uint32 FUN_80032B20(uint32 object) {
 FF_FUNCTION_MARKER(0x80032b20u,"GAME.EXE");
 uint32 offset=ff_u32(0x80094674)<<3;if(ff_u32(object)==470)offset=0u-offset;
 FUN_800100C4(0,0,(sint32)(offset<<16));return ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,256);
}
GDB_CALL uint32 FUN_80033068(sint32 index) {
 FF_FUNCTION_MARKER(0x80033068u,"GAME.EXE");
 if(ff_u32(0x800b4334+244u*(uint32)index)&0x800u)return FUN_80032EE0(index);
 return (uint32)FUN_80056670(187,255,6144);
}
GDB_CALL sint32 FUN_80033DC8(sint32 index) {
 FF_FUNCTION_MARKER(0x80033dc8u,"GAME.EXE");
 sint32 value=ff_s16(0x800b436c+244u*(uint32)index);return value?value:FUN_80056670(28,255,6144);
}
GDB_CALL uint32 FUN_8002A00C(uint32 object) {
 FF_FUNCTION_MARKER(0x8002a00cu,"GAME.EXE");
 return ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,256);
}
GDB_CALL uint32 FUN_80032774(uint32 object) {
 FF_FUNCTION_MARKER(0x80032774u,"GAME.EXE");
 return ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,512);
}
GDB_CALL uint32 FUN_8002A7DC(uint32 object) {
 FF_FUNCTION_MARKER(0x8002a7dcu,"GAME.EXE");
 FUN_8001033C(ff_s16(object+16));ff_static_model_80010B18(ff_u32(0x80082340),0,0);
 FUN_800100C4(0,0x016a0000,-10485760);FUN_8001033C(ff_s16(object+26));
 return ff_static_model_80010B18(ff_u32(0x80082344),0,0);
}
GDB_CALL uint32 FUN_8002A89C(void) {
 FF_FUNCTION_MARKER(0x8002a89cu,"GAME.EXE");
 ff_static_model_80010B18(ff_u32(0x80082360),0,0);FUN_800100C4(0x04e00000,0x00f00000,0);
 FUN_8001033C(16384);return ff_static_model_80010B18(ff_u32(0x80081f2c),0,0);
}
GDB_CALL uint32 FUN_8002AA6C(uint32 object) {
 FF_FUNCTION_MARKER(0x8002aa6cu,"GAME.EXE");
 FUN_80010040();
 FUN_800100C4((sint32)(ff_u32(object+4)-ff_u32(0x8008d4a4)),
  (sint32)(ff_u32(object+8)+ff_u32(0x800990d8+4u*ff_u32(object))-ff_u32(0x8008d4a8)),
  (sint32)(ff_u32(object+12)-ff_u32(0x8008d4ac)));
 FUN_800101CC(32768);FUN_8001033C(ff_s16(object+16));FUN_800104A0(-ff_s16(object+20));FUN_800101CC(ff_s16(object+18));
 ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,0);return FUN_80010028();
}
GDB_CALL uint32 FUN_8002ABE0(uint32 object) {
 FF_FUNCTION_MARKER(0x8002abe0u,"GAME.EXE");
 FUN_80010040();
 FUN_800100C4((sint32)(ff_u32(object+4)-ff_u32(0x8008d4a4)),(sint32)(ff_u32(object+8)-ff_u32(0x8008d4a8)),(sint32)(ff_u32(object+12)-ff_u32(0x8008d4ac)));
 FUN_800104A0(ff_s16(object+20));FUN_8001033C(ff_s16(object+18));FUN_800101CC(ff_s16(object+16));
 ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,0);return FUN_80010028();
}
GDB_CALL uint32 FUN_8002C294(uint32 object) {
 FF_FUNCTION_MARKER(0x8002c294u,"GAME.EXE");
 uint32 kind=ff_u32(object);if(ff_u32(0x80093d60)==2)++kind;
 return ff_static_model_80010B18(ff_u32(0x80081720+4u*kind),0,0);
}
GDB_CALL uint32 FUN_80032B98(uint32 object) {
 FF_FUNCTION_MARKER(0x80032b98u,"GAME.EXE");
 uint32 offset=ff_u32(0x80094794);FUN_80010040();offset<<=19;FUN_800100C4(0,0,(sint32)offset);
 ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,256);FUN_80010028();
 FUN_8001033C(32768);FUN_800100C4(0,0,(sint32)offset);
 return ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,256);
}
GDB_CALL uint32 FUN_8003EE7C(uint32 object) {
 FF_FUNCTION_MARKER(0x8003ee7cu,"GAME.EXE");
 FUN_8001033C(49152);return ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,0);
}
GDB_CALL uint32 FUN_8005BC64(uint32 object) {
 FF_FUNCTION_MARKER(0x8005bc64u,"GAME.EXE");
 FUN_80010040();
 FUN_800100C4((sint32)(ff_u32(object+4)-ff_u32(0x8008d4a4)),(sint32)(ff_u32(object+8)-ff_u32(0x8008d4a8)),(sint32)(ff_u32(object+12)-ff_u32(0x8008d4ac)));
 FUN_8001033C((sint32)(16384u-((uint32)(sint32)ff_s16(object+16)<<5)));
 ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,0);return FUN_80010028();
}
GDB_CALL uint32 FUN_8005BF0C(uint32 object) {
 FF_FUNCTION_MARKER(0x8005bf0cu,"GAME.EXE");
 FUN_80010040();FUN_8001033C(ff_s16(object+16));ff_w32(0x8008d4bc,0x2c404040);
 ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,0);return FUN_80010028();
}
GDB_CALL uint32 FUN_8005BF7C(uint32 object) {
 FF_FUNCTION_MARKER(0x8005bf7cu,"GAME.EXE");
 FUN_80010040();FUN_8001033C(ff_s16(object+16));FUN_800101CC(ff_s16(object+18));FUN_800104A0(-ff_s16(object+20));
 ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,0);return FUN_80010028();
}
GDB_CALL uint32 FUN_8005BFF4(uint32 object) {
 FF_FUNCTION_MARKER(0x8005bff4u,"GAME.EXE");
 FUN_80010040();FUN_8001033C(ff_s16(object+16));FUN_800101CC(ff_s16(object+18));FUN_800104A0(-ff_s16(object+20));
 ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,-256);
 FUN_800100C4(-3145728,2621440,0);FUN_8001033C((sint32)((uint32)(uint16)ff_s16(0x80093dd0)<<12));
 ff_static_model_80010B18(ff_u32(0x80081ff0),0,-256);FUN_80010028();
 FUN_80010040();FUN_800100C4(0,(sint32)ff_u32(object+8),0);FUN_8001033C(ff_s16(object+16));
 ff_static_model_80010B18(ff_u32(0x80082428),0,-2048);return FUN_80010028();
}
GDB_CALL uint32 FUN_8005D2E4(uint32 object) {
 FF_FUNCTION_MARKER(0x8005d2e4u,"GAME.EXE");
 FUN_80010040();FUN_8005C740(object);
 ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,0);return FUN_80010028();
}
GDB_CALL uint32 FUN_8005D340(uint32 object) {
 FF_FUNCTION_MARKER(0x8005d340u,"GAME.EXE");
 uint32 scale,color;FUN_80010040();FUN_8005C740(object);
 scale=(uint32)(sint32)ff_s16(object+22)<<12;FUN_80010F94((sint32)scale,(sint32)scale,(sint32)scale);
 color=64u-2u*(uint32)(sint32)ff_s16(object+22);ff_w32(0x8008d4bc,0x2c000000u+(color<<16)+(color<<8)+color);
 ff_static_model_80010B18(ff_u32(0x80082088),0,0);return FUN_80010028();
}
GDB_CALL sint32 FUN_80012190(void) {
 FF_FUNCTION_MARKER(0x80012190u,"GAME.EXE");
 return ff_s16(ff_u32(0x8008d494)+30);
}
/* 124A0: FT4 sprite packet. UV extent subtraction is 16-bit, OT offset signed. */
GDB_CALL uint32 FUN_800124A0(const sint32 *position,uint32 texture) {
 FF_FUNCTION_MARKER(0x800124a0u,"GAME.EXE");
 uint32 packet=ff_u32(0x8008d4b0),uv=0x800a8768+12u*texture;
 sint32 x=(sint16)position[0],y=(sint16)position[1];
 uint32 first=ff_u32(uv),second=ff_u32(uv+4),last=ff_u32(uv+8);
 uint32 mode=first&1u,base=first-mode,extent=((last>>16)-base)&65535u;
 uint32 right=(extent&255u)+(uint32)x,bottom=(extent>>8)+(uint32)y,ot,depth,lookup,link;
 ff_w32(packet+12,base);ff_w32(packet+20,second);
 ff_w16(packet+28,(uint16)last);ff_w16(packet+36,(uint16)(last>>16));
 ff_w32(packet+4,0x2d000000u|(mode<<25));
 ff_w16(packet+8,(uint16)x);ff_w16(packet+24,(uint16)x);
 ff_w16(packet+10,(uint16)y);ff_w16(packet+18,(uint16)y);
 ff_w16(packet+16,(uint16)right);ff_w16(packet+32,(uint16)right);
 ff_w16(packet+26,(uint16)bottom);ff_w16(packet+34,(uint16)bottom);
 ot=ff_u32(0x8008d4b4);depth=(uint32)(position[2]>>2);
 lookup=ff_u32(0x8008d4d8);link=ot+(uint32)(sint32)ff_s16(lookup+2u*depth);
 ff_w32(packet,ff_u32(link)|0x09000000u);ff_w32(link,packet&0xffffffu);
 ff_w32(0x8008d4b0,packet+40);return 0x8008d4b0;
}
/* Exact original leaf, including its return delay slot; see leaf-batch.json. */
GDB_CALL uint32 FUN_80011E28(uint32 head,uint32 packet,uint32 flags) {
 FF_FUNCTION_MARKER(0x80011e28u,"GAME.EXE");
 uint32 old=ff_u32(head);ff_w32(head,packet);ff_w32(packet,old|flags);return old;
}
/* Exact original leaf, including its return delay slot; see leaf-batch.json. */
GDB_CALL uint32 FUN_8001F510(void) {
 FF_FUNCTION_MARKER(0x8001f510u,"GAME.EXE");
 return 0;
}
/* Exact original leaf, including its return delay slot; see leaf-batch.json. */
GDB_CALL uint32 FUN_8002A154(void) {
 FF_FUNCTION_MARKER(0x8002a154u,"GAME.EXE");
 ff_w16(ff_u32(0x800947c8)+28,1);return 1;
}
/* Exact original leaf, including its return delay slot; see leaf-batch.json. */
GDB_CALL void FUN_8002A974(void) {
 FF_FUNCTION_MARKER(0x8002a974u,"GAME.EXE");
 
}
/* Exact original leaf, including its return delay slot; see leaf-batch.json. */
GDB_CALL uint32 FUN_8002C1DC(uint32 object) {
 FF_FUNCTION_MARKER(0x8002c1dcu,"GAME.EXE");
 ff_w32(object,0xffffffffu);return 0xffffffffu;
}
/* Exact original leaf, including its return delay slot; see leaf-batch.json. */
GDB_CALL uint32 FUN_8002C27C(uint32 object) {
 FF_FUNCTION_MARKER(0x8002c27cu,"GAME.EXE");
 ff_w32(object,0xffffffffu);return 0xffffffffu;
}
/* Exact original leaf, including its return delay slot; see leaf-batch.json. */
GDB_CALL uint32 FUN_8002EB58(uint32 object) {
 FF_FUNCTION_MARKER(0x8002eb58u,"GAME.EXE");
 uint32 value=(uint16)ff_s16(object+16)+1004u;ff_w16(object+16,(uint16)value);return value;
}
/* Exact original leaf, including its return delay slot; see leaf-batch.json. */
GDB_CALL void FUN_80032ED8(void) {
 FF_FUNCTION_MARKER(0x80032ed8u,"GAME.EXE");
 
}
/* Exact original leaf, including its return delay slot; see leaf-batch.json. */
GDB_CALL void FUN_8003B774(void) {
 FF_FUNCTION_MARKER(0x8003b774u,"GAME.EXE");
 
}
/* Exact original leaf, including its return delay slot; see leaf-batch.json. */
GDB_CALL uint32 FUN_800415F4(uint32 object) {
 FF_FUNCTION_MARKER(0x800415f4u,"GAME.EXE");
 ff_w32(object,0xffffffffu);return 0xffffffffu;
}
/* Exact original leaf, including its return delay slot; see leaf-batch.json. */
GDB_CALL uint32 FUN_80041600(uint32 object) {
 FF_FUNCTION_MARKER(0x80041600u,"GAME.EXE");
 ff_w32(object,0xffffffffu);return 0xffffffffu;
}
/* Exact original leaf, including its return delay slot; see leaf-batch.json. */
GDB_CALL void FUN_80041B4C(void) {
 FF_FUNCTION_MARKER(0x80041b4cu,"GAME.EXE");
 
}
/* Exact original leaf, including its return delay slot; see leaf-batch.json. */
GDB_CALL void FUN_8004483C(void) {
 FF_FUNCTION_MARKER(0x8004483cu,"GAME.EXE");
 
}
/* Exact original leaf, including its return delay slot; see leaf-batch.json. */
GDB_CALL void FUN_80044D88(void) {
 FF_FUNCTION_MARKER(0x80044d88u,"GAME.EXE");
 
}
/* Exact original leaf, including its return delay slot; see leaf-batch.json. */
GDB_CALL void FUN_800455DC(void) {
 FF_FUNCTION_MARKER(0x800455dcu,"GAME.EXE");
 
}
/* Exact original leaf, including its return delay slot; see leaf-batch.json. */
GDB_CALL void FUN_8004EF64(void) {
 FF_FUNCTION_MARKER(0x8004ef64u,"GAME.EXE");
 
}
/* Exact original leaf, including its return delay slot; see leaf-batch.json. */
GDB_CALL void FUN_800582DC(void) {
 FF_FUNCTION_MARKER(0x800582dcu,"GAME.EXE");
 
}
/* Exact original leaf, including its return delay slot; see leaf-batch.json. */
GDB_CALL void FUN_8005862C(void) {
 FF_FUNCTION_MARKER(0x8005862cu,"GAME.EXE");
 
}
/* Exact original leaf, including its return delay slot; see leaf-batch.json. */
GDB_CALL void FUN_8005C654(void) {
 FF_FUNCTION_MARKER(0x8005c654u,"GAME.EXE");
 
}
/* Exact original leaf, including its return delay slot; see leaf-batch.json. */
GDB_CALL void FUN_8005C65C(void) {
 FF_FUNCTION_MARKER(0x8005c65cu,"GAME.EXE");
 
}
/* Exact original leaf, including its return delay slot; see leaf-batch.json. */
GDB_CALL void FUN_8005C664(void) {
 FF_FUNCTION_MARKER(0x8005c664u,"GAME.EXE");
 
}
/* Exact original leaf, including its return delay slot; see leaf-batch.json. */
GDB_CALL void FUN_8005D150(void) {
 FF_FUNCTION_MARKER(0x8005d150u,"GAME.EXE");
 
}
/* Exact original leaf, including its return delay slot; see leaf-batch.json. */
GDB_CALL void FUN_8005D158(void) {
 FF_FUNCTION_MARKER(0x8005d158u,"GAME.EXE");
 
}
/* Exact original leaf, including its return delay slot; see leaf-batch.json. */
GDB_CALL void FUN_8005DACC(void) {
 FF_FUNCTION_MARKER(0x8005daccu,"GAME.EXE");
 
}
/* Exact original leaf, including its return delay slot; see leaf-batch.json. */
GDB_CALL void FUN_8005E138(void) {
 FF_FUNCTION_MARKER(0x8005e138u,"GAME.EXE");
 
}
/* Exact original leaf, including its return delay slot; see leaf-batch.json. */
GDB_CALL uint32 FUN_8005F050(uint32 value) {
 FF_FUNCTION_MARKER(0x8005f050u,"GAME.EXE");
 return value;
}
/* Exact original leaf, including its return delay slot; see leaf-batch.json. */
GDB_CALL uint32 FUN_8005F234(uint32 value) {
 FF_FUNCTION_MARKER(0x8005f234u,"GAME.EXE");
 return value;
}
/* Exact original leaf, including its return delay slot; see leaf-batch.json. */
GDB_CALL void FUN_80064620(void) {
 FF_FUNCTION_MARKER(0x80064620u,"GAME.EXE");
 
}
/* Exact original leaf, including its return delay slot; see leaf-batch.json. */
GDB_CALL uint32 FUN_8006467C(void) {
 FF_FUNCTION_MARKER(0x8006467cu,"GAME.EXE");
 return 0;
}
/* Exact original leaf, including its return delay slot; see leaf-batch.json. */
GDB_CALL void FUN_80066ABC(void) {
 FF_FUNCTION_MARKER(0x80066abcu,"GAME.EXE");
 
}
/* Exact original leaf, including its return delay slot; see leaf-batch.json. */
GDB_CALL void FUN_8006A0F8(void) {
 FF_FUNCTION_MARKER(0x8006a0f8u,"GAME.EXE");
 
}
#include <stdlib.h>
#include <string.h>
/* Positive finite software-float paths used only by6A50C. These preserve
 * the original SDK rounding; they are not general IEEE754 replacements. */
static uint32 ff_table_float_div(uint32 a,uint32 b) {
 sint32 exponent;uint32 numerator,denominator,bit,quotient=0;
 if((a|b)&0x80000000u||!b)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);if(!a)return 0;
 exponent=(sint32)((a>>23)&255)-(sint32)((b>>23)&255)+126;
 numerator=(a&0x7fffff)|0x800000;denominator=(b&0x7fffff)|0x800000;
 if(numerator<denominator){numerator<<=1;--exponent;}
 for(bit=0x1000000;bit;bit>>=1) {
  if(numerator>=denominator){quotient|=bit;numerator-=denominator;}
  numerator<<=1;
 }
 if(exponent<0||exponent>=254)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
 return ((uint32)(exponent+1)<<23)|(((quotient+1)>>1)&0xff7fffff);
}
static uint32 ff_table_float_add(uint32 a,uint32 b) {
 uint32 ea=(a>>23)&255,eb=(b>>23)&255,x,y,sum;
 if((a|b)&0x80000000u)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);if(!a)return b;if(!b)return a;
 if(eb+25<ea)return a;if(ea+25<eb)return b;
 x=((a&0x7fffff)|0x800000)<<6;y=((b&0x7fffff)|0x800000)<<6;
 if(eb<ea)y>>=ea-eb;else{x>>=eb-ea;ea=eb;}
 sum=x+y;while(!(sum&0xe0000000)){sum<<=1;--ea;}
 if(sum&0x40000000){sum>>=1;++ea;}
 sum+=(sum&0x40)?32:31;
 if(sum&0x40000000){sum>>=1;++ea;}if(ea>=255)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
 return (ea<<23)|((sum>>6)&0xff7fffff);
}
static uint32 ff_table_int_float(uint32 value) {
 uint32 exponent=127,top=value;if(!value)return 0;if(value>8191)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
 while(top>>1){top>>=1;++exponent;}
 return (exponent<<23)|((value<<(150-exponent))&0x7fffff);
}
static uint32 ff_table_float_int(uint32 value) {
 uint32 exponent=(value>>23)&255;
 if(!value||exponent<126)return 0;if(exponent>157||value&0x80000000u)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
 return (((value&0x7fffff)|0x800000)<<7)>>(157-exponent);
}
uint32 FUN_8006A50C(void) {
 uint32 i,state=0x3f800000;
 for(i=0;i<8192;i++) {
  ff_w16(ff_u32(0x8008d4d8)+2*i,(uint16)(ff_table_float_int(ff_table_float_div(0x44ffe000,state))<<2));
  state=ff_table_float_add(state,ff_table_float_div(ff_table_int_float(i),0x46000000));
 }
 ff_w16(ff_u32(0x8008d4d8),8188);return 8188;
}
/* 80011D20: eight word stores include matrix padding and translations. */
GDB_CALL sint32 FUN_80011D20(void) {
 uint32 a=ff_u32(0x8008d494);int i;
 for(i=0;i<8;i++)ff_w32(a+4u*i,(i==0||i==2||i==4)?4096:0);return 4096;
}
/* Shared rotation-column MVMVA output semantics for pitch, yaw and roll. */
static void ff_matrix_scratch_columns(uint32 a,MATRIX *matrix) {
 SVECTOR v;VECTOR q;sint16 out[9];int col,row;
 for(col=0;col<3;col++) {
  v.vx=ff_s16(0x1f8003e0+2u*col);v.vy=ff_s16(0x1f8003e6+2u*col);v.vz=ff_s16(0x1f8003ec+2u*col);v.pad=0;ApplyMatrix(matrix,&v,&q);
  for(row=0;row<3;row++){sint32 n=row==0?q.vx:row==1?q.vy:q.vz;if(n<-32768)n=-32768;if(n>32767)n=32767;out[3*row+col]=(sint16)n;}
 }
 for(row=0;row<9;row++)ff_w16(a+2u*row,(uint16)out[row]);ff_w32(a+16,(uint32)(sint32)out[8]);
}
/* 800101CC: rotate current matrix by table pitch, MVMVA rotation-only columns.
 * AA ApplyMatrix supplies fixed-point multiplication; GTE IR saturation follows.
 * No native consumer relies on this routine's unspecified zero-angle return. */
GDB_CALL void ff_pitch_index_800101E0(uint32 index) {
 uint32 a,pair;MATRIX m;
 pair=ff_u32(0x80087640+index);a=ff_u32(0x8008d494);memcpy(&m,ff_ptr(a,32),32);
 ff_w32(0x1f8003e0,4096);ff_w32(0x1f8003e4,0);ff_w16(0x1f8003e8,(uint16)(pair>>16));ff_w16(0x1f8003ea,(uint16)pair);
 ff_w16(0x1f8003ec,0);ff_w16(0x1f8003ee,(uint16)(0u-(pair&65535)));ff_w32(0x1f8003f0,pair>>16);
 ff_matrix_scratch_columns(a,&m);
}
GDB_CALL void FUN_800101CC(sint32 angle) {
 uint32 index;if((uint32)angle==0x80000000u)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);index=((0u-(uint32)angle)>>2)&0x3ffc;
 if(index)ff_pitch_index_800101E0(index);
}

/* GTE reciprocal approximation. The 257-entry UNR lookup is generated by its
 * integer formula; clamp the final entry rather than wrapping it to 255. */
static uint32 ff_gte_divide(uint32 h,uint32 z) {
 uint32 i,q; sint32 u,d,recip;
 if(h>=2u*z)return 0x1ffff;
 while(z<0x8000){z<<=1;h<<=1;}
 i=((z&0x7fff)+0x40)>>7;
 u=(sint32)(((0x40000u/(i+0x100u))+1u)/2u)-0x101;
 if(u<0)u=0;if(u>255)u=255;u+=0x101;
 d=((sint32)z*(-u)+0x80)>>8;recip=(u*(0x20000+d)+0x80)>>8;
 q=(uint32)(((uint64)h*(uint32)recip+0x8000)>>16);return q>0x1ffff?0x1ffff:q;
}
static sint32 ff_geom_clamp(sint32 n,sint32 lo,sint32 hi){return n<lo?lo:n>hi?hi:n;}
/* 800120BC reads V0's result from the RTPT FIFO (SXY0/SZ1).
 * Only those memory outputs are exposed here; other COP2 side effects are WIP.
 * FF uses integer screen offsets; host geometry setup supplies those and H. */
static sint32 ff_project_local(sint32 x,sint32 y,sint32 z,sint32 dest[4]) {
 MATRIX m;SVECTOR v;VECTOR q;uint32 a=ff_u32(0x8008d494),depth,ratio; sint32 ox,oy,sx,sy,result;
 memcpy(&m,ff_ptr(a,32),32);v.vx=(sint16)x;v.vy=(sint16)y;v.vz=(sint16)z;v.pad=0;
 ApplyMatrix(&m,&v,&q);q.vx+=m.t[0]>>16;q.vy+=m.t[1]>>16;q.vz+=m.t[2]>>16;
 depth=(uint32)ff_geom_clamp(q.vz,0,65535);ratio=ff_gte_divide((uint16)ReadGeomScreen(),depth);
 ReadGeomOffset(&ox,&oy);
 sx=ff_geom_clamp(ox+(sint32)(((sint64)ff_geom_clamp(q.vx,-32768,32767)*ratio)>>16),-1024,1023);
 sy=ff_geom_clamp(oy+(sint32)(((sint64)ff_geom_clamp(q.vy,-32768,32767)*ratio)>>16),-1024,1023);
 result=depth<40?4:((uint16)sx<320&&(uint16)sy<256?0:1);
 dest[0]=sx;dest[1]=sy;dest[2]=(sint16)depth;dest[3]=result;return result;
}
GDB_CALL uint32 FUN_800112A4(uint32 unused,uint32 column,uint32 row) {
 FF_FUNCTION_MARKER(0x800112a4u,"GAME.EXE");
 uint32 r,n,packed,xy=0x001fe800,depths=0x1f800000,p,ot,head,offset=0,cell,texture,sx[4],uv[3],color;
 sint32 point[4],z=-2048,cutoff,depth,delta,fade;(void)unused;
 column<<=1;row<<=7;FUN_80010170();
 /* RTPT: eighteen vertices per row with a seventeen-vertex stride.
  * Packed VXY increments carry into Y after X crosses FFFF. */
 for(r=0;r<17;++r,z+=256){
  for(n=0,packed=0xf800;n<18;++n,packed+=256){
   ff_project_local((sint16)packed,(sint16)(packed>>16),z,point);
   ff_w32(xy,((uint32)point[0]&65535)|((uint32)point[1]<<16));ff_w16(depths,(uint16)point[2]);xy+=4;depths+=2;
  }xy-=4;depths-=2;
 }
 p=ff_u32(0x8008d4b0);ot=ff_u32(0x8008d4b4)+8;head=ff_u32(ot);cutoff=(sint32)ff_u32(0x80011590);
 for(r=0;r<16;++r){
  for(n=0;n<16;++n){
   column&=0x7e;row&=0x1f80;cell=(uint16)ff_s16(0x8009f870+column+row);
   if(cell!=0xfffc&&ff_s16(0x1f800000+(offset>>1))>0){
    depth=ff_s16(0x1f800024+(offset>>1));delta=(sint32)((uint32)depth-(uint32)cutoff);
    if(depth>0&&delta<=0){
     color=0x2d000000;fade=(sint32)((uint32)delta+2048u);
     if(fade>=0){uint32 shade=128u-(uint32)(fade>>4);color=0x2c000000|shade|(shade<<8)|(shade<<16);}
     sx[0]=ff_u32(0x001fe800+offset);sx[1]=ff_u32(0x001fe804+offset);
     sx[2]=ff_u32(0x001fe844+offset);sx[3]=ff_u32(0x001fe848+offset);
     if((sx[0]&65535)<320||(sx[1]&65535)<320||(sx[2]&65535)<320||(sx[3]&65535)<320){
      ff_w32(p+8,sx[0]);ff_w32(p+16,sx[1]);ff_w32(p+24,sx[2]);ff_w32(p+32,sx[3]);
      texture=0x800a8768+4*cell;uv[0]=ff_u32(texture);uv[1]=ff_u32(texture+4);uv[2]=ff_u32(texture+8);
      ff_w32(p+12,uv[0]);ff_w32(p+20,uv[1]);ff_w16(p+28,(uint16)uv[2]);ff_w16(p+36,(uint16)(uv[2]>>16));
      ff_w32(p+4,color);ff_w32(p,head|0x09000000);head=p;p+=40;
     }
    }
   }
   column-=2;offset+=4;
  }column+=32;row-=128;offset+=4;
 }
 ff_w32(ot,head);ff_w32(0x8008d4bc,0x2d000000);ff_w32(0x8008d4b0,p);return 0x8008d4b0;
}
void ff_sky_occluder_80065B3C(void) {
 static const uint32 xs[4]={0xfe000000u,0x02000000,0xfa000000u,0x06000000};
 sint32 points[4][4];uint32 i,p;
 FUN_80011D20();FUN_800101CC(ff_s16(0x8008d492));FUN_8001033C(ff_s16(0x8008d490));
 for(i=0;i<4;++i){FUN_80010040();FUN_800100C4((sint32)(xs[i]-ff_u32(0x8008d4a4)),
  (sint32)(0u-ff_u32(0x8008d4a8)),(sint32)(i<2?0x07000000u-ff_u32(0x8008d4ac):0x0fff0000u));
  ff_project_local(0,0,0,points[i]);FUN_80010028();}
 p=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,p+24);ff_w8(p+3,5);ff_w32(p+4,0x28000000);
 for(i=0;i<4;++i){ff_w16(p+8+4*i,(uint16)points[i][0]);ff_w16(p+10+4*i,(uint16)((uint32)points[i][1]-(i<2?1:0)));}
 FUN_80011E40(ff_u32(0x8008d4b4),p);
}
GDB_CALL uint32 FUN_80064D34(uint32 left,uint32 right) {
 FF_FUNCTION_MARKER(0x80064d34u,"GAME.EXE");
 sint32 points[4][4];uint32 i,p;
 FUN_80011D20();FUN_800101CC(ff_s16(0x8008d492));FUN_8001033C(ff_s16(0x8008d490));
 for(i=0;i<4;++i){
  FUN_80010040();FUN_800100C4((sint32)(((i&1)?right:left)-ff_u32(0x8008d4a4)),
   (sint32)(0u-ff_u32(0x8008d4a8)),i<2?0x7fff0000:0x0fff0000);
  ff_project_local(0,0,0,points[i]);FUN_80010028();
 }
 p=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,p+24);ff_w8(p+3,5);ff_w32(p+4,0x28000000);
 for(i=0;i<4;++i){ff_w16(p+8+4*i,(uint16)points[i][0]);ff_w16(p+10+4*i,(uint16)points[i][1]);}
 return FUN_80011E40(ff_u32(0x8008d4b4),p);
}
GDB_CALL sint32 FUN_800120BC(sint32 x,sint32 y,sint32 z,uint32 dest) {
 sint32 position[4],result=ff_project_local(x,y,z,position);
 ff_w32(dest,(uint32)position[0]);ff_w32(dest+4,(uint32)position[1]);ff_w32(dest+8,(uint32)position[2]);ff_w32(dest+12,(uint32)position[3]);return result;
}

/* Partial 80058634, exact range 80058844..8005887C. Host-private temporary
 * substitutes the original caller's stack output. Remaining scene work is WIP. */
GDB_CALL void ff_menu_horizon_80058844(void) {
 FUN_80011D20();FUN_800101CC(ff_s16(0x8008d492));
 FUN_800120BC(0,0,32767,0x801fc020);ff_w32(0x8008d4d0,ff_u32(0x801fc024));
}

/* 80011D50: forward OT, 2048 entries; raw pointer stores retain alias bits.
 * Valid RAM ranges cannot trigger the original signed ADDI overflow. */
GDB_CALL sint32 FUN_80011D50(uint32 table) {
 uint32 i;ff_ptr(table,8192);
 for(i=0;i<2047;i++)ff_w32(table+4u*i,table+4u*(i+1));
 ff_w32(table+8188,0x10018);return 0x10018;
}
/* 80011D9C: skip empty links, retaining the predecessor node. In the final
 * empty run, the store uses the last node address, not the terminal's address.
 * Well-formed DMA lists are required, as in the original. */
GDB_CALL sint32 FUN_80011D9C(void) {
 uint32 current=ff_u32(0x8008d4b4),previous,tag,next,node;
 for(;;) {
  previous=current;tag=ff_u32(current);next=tag&0xffffff;
  if(next==0x10018)return 0x10018;
  current=next;if(tag&0xff000000)continue;
  for(;;) {
   node=current;tag=ff_u32(current);next=tag&0xffffff;
   if(next==0x10018){ff_w32(previous,current);return 0x10018;}
   current=next;if(tag&0xff000000)break;
  }
  ff_w32(previous,(ff_u32(previous)&0xff000000)|node);
 }
}

static uint32 ff_matrix_pointer_add(uint32 a,sint32 delta) {
 sint64 n=(sint64)(sint32)a+delta;if(n<-2147483647LL-1||n>2147483647LL)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);return (uint32)n;
}
static void ff_matrix_load_gte(MATRIX *source) {
 MATRIX m=*source;m.t[0]>>=16;m.t[1]>>=16;m.t[2]>>=16;SetRotMatrix(&m);SetTransMatrix(&m);
}
/* 80011F74: load all eight words before stores, then reload COP2 controls. */
GDB_CALL void FUN_80011F74(uint32 source) {
 MATRIX m;memcpy(&m,ff_ptr(source,32),32);
 memcpy(ff_ptr(ff_u32(0x8008d494),32),&m,32);ff_matrix_load_gte(&m);
}
/* 80010170: reload geometry registers from the current matrix, without moving
 * its pointer or modifying the matrix. Translation fractions are discarded. */
GDB_CALL void FUN_80010170(void) {
 MATRIX m;memcpy(&m,ff_ptr(ff_u32(0x8008d494),32),32);ff_matrix_load_gte(&m);
}
/* 80010028 only advances the pointer: it does not reload GTE registers. */
GDB_CALL uint32 FUN_80010028(void) {
 uint32 a=ff_matrix_pointer_add(ff_u32(0x8008d494),32);ff_w32(0x8008d494,a);return a;
}
/* 80010040 snapshots all eight source words before writing the lower slot. */
GDB_CALL uint32 FUN_80010040(void) {
 uint32 a=ff_u32(0x8008d494),b;MATRIX m;memcpy(&m,ff_ptr(a,32),32);
 b=ff_matrix_pointer_add(a,-32);memcpy(ff_ptr(b,32),&m,32);ff_matrix_load_gte(&m);ff_w32(0x8008d494,b);return b;
}
/* 800100C4: MVMVA results use MAC1..3, not saturated IR1..3. Translation
 * fractions are discarded before the operation; stores wrap after SLL 16. */
GDB_CALL uint32 FUN_800100C4(sint32 x,sint32 y,sint32 z) {
 uint32 a=ff_u32(0x8008d494);MATRIX m;SVECTOR v;VECTOR q;
 memcpy(&m,ff_ptr(a,32),32);ff_matrix_load_gte(&m);
 v.vx=(sint16)(x>>16);v.vy=(sint16)(y>>16);v.vz=(sint16)(z>>16);v.pad=0;ApplyMatrix(&m,&v,&q);
 ff_w32(a+20,((uint32)q.vx+(uint32)(m.t[0]>>16))<<16);
 ff_w32(a+24,((uint32)q.vy+(uint32)(m.t[1]>>16))<<16);
 ff_w32(a+28,((uint32)q.vz+(uint32)(m.t[2]>>16))<<16);return a;
}

/* 8001033C: same column multiply as pitch, with a different scratch matrix. */
GDB_CALL void ff_yaw_index_80010350(uint32 index) {
 uint32 pair,a;MATRIX m;
 pair=ff_u32(0x80087640+index);a=ff_u32(0x8008d494);
 ff_w32(0x1f8003e0,pair>>16);ff_w32(0x1f8003e4,(0u-pair)&65535);
 ff_w32(0x1f8003e8,4096);ff_w32(0x1f8003ec,pair&65535);ff_w32(0x1f8003f0,pair>>16);
 memcpy(&m,ff_ptr(a,32),32);ff_matrix_scratch_columns(a,&m);
}
GDB_CALL void FUN_8001033C(sint32 angle) {
 uint32 index;if((uint32)angle==0x80000000u)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);index=((0u-(uint32)angle)>>2)&0x3ffc;
 if(index)ff_yaw_index_80010350(index);
}
/* 800104B4 is also called directly by the model stream with a table offset. */
GDB_CALL void ff_roll_index_800104B4(uint32 index) {
 uint32 pair=ff_u32(0x80087640+index),a=ff_u32(0x8008d494);MATRIX m;
 ff_w16(0x1f8003e0,(uint16)(pair>>16));ff_w16(0x1f8003e2,(uint16)(0u-(pair&65535)));
 ff_w16(0x1f8003e4,0);ff_w16(0x1f8003e6,(uint16)pair);
 ff_w32(0x1f8003e8,pair>>16);ff_w32(0x1f8003ec,0);ff_w32(0x1f8003f0,4096);
 memcpy(&m,ff_ptr(a,32),32);ff_matrix_scratch_columns(a,&m);
}
GDB_CALL void FUN_800104A0(sint32 angle) {
 uint32 index;if((uint32)angle==0x80000000u)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
 index=((0u-(uint32)angle)>>2)&0x3ffc;if(index)ff_roll_index_800104B4(index);
}
static void ff_model_rotations(uint32 roll,uint32 yaw,uint32 pitch) {
 if(roll)ff_roll_index_800104B4(roll<<2);
 if(yaw){if(yaw==0x80000000u)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);ff_yaw_index_80010350(((0u-yaw)&0xfff)<<2);}
 if(pitch){if(pitch==0x80000000u)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);ff_pitch_index_800101E0(((0u-pitch)&0xfff)<<2);}
}
/* Partial 8001060C: transform phase through 80010AB8. The final matrix pop
 * and polygon phase remain caller work; no model is drawn by this entry yet. */
GDB_CALL void ff_model_transform_8001060C(uint32 model,uint32 pose) {
 uint32 stream=model+ff_u32(model+24),counts=model+32,vertices=ff_u32(model),index=0,command;
 uint32 saved[4];sint32 angle;memcpy(saved,ff_ptr(0x801fc090,16),16);
 FUN_80010040();
 angle=ff_s16(pose+220);if(angle)ff_pitch_index_800101E0(((uint32)angle&0xfff)<<2);
 angle=ff_s16(pose+218);if(angle)ff_yaw_index_80010350(((uint32)angle&0xfff)<<2);
 angle=ff_s16(pose+216);if(angle)ff_roll_index_800104B4(((uint32)angle&0xfff)<<2);
 for(;;) {
  uint32 flag,bone,count,matrix,out=0,k;
  command=ff_u32(stream);stream+=4;if(!command)break;
  if(command==1){FUN_80010040();continue;}
  if(command==2){FUN_80010028();continue;}
  flag=ff_u32(stream);
  FUN_800100C4((sint32)ff_u32(stream+4),(sint32)ff_u32(stream+8),(sint32)ff_u32(stream+12));stream+=16;
  ff_model_rotations(ff_u32(stream),ff_u32(stream+4),ff_u32(stream+8));stream+=12;
  bone=pose+6*ff_u32(stream);stream+=4;
  ff_model_rotations((uint32)(sint32)ff_s16(bone+216),(uint32)(sint32)ff_s16(bone+218),(uint32)(sint32)ff_s16(bone+220));
  if(flag==1){ff_yaw_index_80010350(ff_u32(pose)<<2);out=204;}
  else if(flag==2||flag==3||flag==10||flag==11) {
   out=flag==2?4:flag==3?36:flag==10?68:100;
   memcpy(ff_ptr(pose+out,32),ff_ptr(ff_u32(0x8008d494),32),32);out=0;
  }else if(flag>=4&&flag<=9)out=132+12*(flag-4);
  matrix=ff_u32(0x8008d494);if(out)memcpy(ff_ptr(pose+out,12),ff_ptr(matrix+20,12),12);
  count=ff_u32(counts);counts+=4;
  if(count) {
   FUN_80010170();
   for(k=0;k<count;k+=3) {
    uint32 j;
    for(j=0;j<3;j++) {
     uint32 n=index+k+j,v=vertices+8*n;
     FUN_800120BC(ff_s16(v),ff_s16(v+2),ff_s16(v+4),0x801fc090);
     ff_w32(0x1fe800+4*n,(ff_u32(0x801fc090)&65535)|(ff_u32(0x801fc094)<<16));
     ff_w16(0x1fe400+2*n,(uint16)ff_u32(0x801fc098));
    }
   }
  }
  index+=count;
  ff_model_rotations(ff_u32(stream),ff_u32(stream+4),ff_u32(stream+8));stream+=12;
 }
 memcpy(ff_ptr(0x801fc090,16),saved,16);
}
static sint32 ff_model_nclip(uint32 a,uint32 b,uint32 c) {
 sint32 x0=(sint16)a,y0=(sint16)(a>>16),x1=(sint16)b,y1=(sint16)(b>>16),x2=(sint16)c,y2=(sint16)(c>>16);
 return (sint32)((uint32)x0*(uint32)(y1-y2)+(uint32)x1*(uint32)(y2-y0)+(uint32)x2*(uint32)(y0-y1));
}
/* Shared polygon phase. Signed packed XY tests and line bucket alias behavior
 * deliberately follow MIPS rather than a generic polygon clipping policy. */
GDB_CALL uint32 ff_model_packets_80010CA0(uint32 count,uint32 records,uint32 xy,uint32 depths,sint32 bias) {
 uint32 packet=ff_u32(0x8008d4b0),ot=ff_u32(0x8008d4b4),table=ff_u32(0x8008d4d8),color=ff_u32(0x8008d4bc),blend=ff_u32(0x8008d4c0);
 if(!count)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
 do {
  uint32 first=ff_u32(records),second=ff_u32(records+4),i0=first&65535,i1=first>>16,i2=second&65535,i3=second>>16;
  sint32 depth;uint32 a,b,c,d,bucket,texture,uv0,uv1,uv2,flags;
  if((sint32)second<0) {
   /* 80010F24's bias addition is overwritten by the next instruction. */
   depth=(ff_s16(depths+(i0>>1))+ff_s16(depths+(i1>>1)))>>3;
   if(depth<=0||depth>8188)goto next_record;
   bucket=ot+(uint32)(sint32)ff_s16(table+2u*(uint32)depth);
   ff_w32(packet+4,ff_u32(records+8));ff_w32(packet+8,ff_u32(xy+i0));ff_w32(packet+12,ff_u32(xy+i1));
   ff_w32(packet,ff_u32(bucket)|0x03000000);ff_w32(bucket,packet);packet+=16;
  }else {
   sint32 z0=ff_s16(depths+(i0>>1)),z2=ff_s16(depths+(i2>>1));
   if(z0<=0||z2<=0)goto next_record;
   depth=(sint32)((uint32)(z0+z2)+(uint32)bias)>>3;if(depth>8192)goto next_record;
   a=ff_u32(xy+i0);b=ff_u32(xy+i1);c=ff_u32(xy+i2);d=ff_u32(xy+i3);
   if((a&65535)>=320&&(b&65535)>=320&&(c&65535)>=320&&(d&65535)>=320)goto next_record;
   if((sint32)a<=0&&(sint32)b<=0&&(sint32)c<=0&&(sint32)d<0)goto next_record;
   bucket=ot+(uint32)(sint32)ff_s16(table+2u*(uint32)depth);flags=ff_u32(records+8);
   if(!(flags&1)&&ff_model_nclip(a,b,c)<0)goto next_record;
   if(flags&2){ff_w32(packet+8,b);ff_w32(packet+16,a);ff_w32(packet+24,c);ff_w32(packet+32,d);}
   else {ff_w32(packet+8,a);ff_w32(packet+16,b);ff_w32(packet+24,d);ff_w32(packet+32,c);}
   texture=0x800a8768+(flags>>16);uv0=ff_u32(texture);uv1=ff_u32(texture+4);uv2=ff_u32(texture+8);
   if(blend)uv1=(uv1&0xff9fffff)|((blend-1)<<21);
   ff_w32(packet+12,uv0-(uv0&1));ff_w32(packet+20,uv1);ff_w16(packet+28,(uint16)uv2);ff_w16(packet+36,(uint16)(uv2>>16));
   ff_w32(packet+4,color|((uv0&1)<<25));ff_w32(packet,ff_u32(bucket)|0x09000000);ff_w32(bucket,packet&0xffffff);packet+=40;
  }
 next_record:records+=12;
 }while(--count);
 ff_w32(0x8008d4bc,0x2d000000);ff_w32(0x8008d4c0,0);ff_w32(0x8008d4b0,packet);return 0x8008d4b0;
}
GDB_CALL uint32 FUN_8001060C(uint32 model,uint32 pose) {
 ff_model_transform_8001060C(model,pose);FUN_80010028();
 return ff_model_packets_80010CA0(ff_u32(model+12),ff_u32(model+8),0x1fe800,0x1fe400,0);
}
/* Static entry shares packets but selects projection buffers by vertex count.
 * The pipelined original always projects at least two triples. */
GDB_CALL uint32 ff_static_model_80010B18(uint32 model,uint32 unused,sint32 bias) {
 sint32 count=(sint32)ff_u32(model+4);uint32 xy,depths,vertices=ff_u32(model),n,total,saved[4];
 (void)unused;
 xy=count<193?0x1f800000:0x1fe800;
 depths=count<127?0x1f800200:count<193?0x1fe400:count<385?0x1f800000:0x1fe400;
 total=count<=6?6:((uint32)count+2)/3*3;
 memcpy(saved,ff_ptr(0x801fc090,16),16);FUN_80010170();
 for(n=0;n<total;n++) {
  uint32 v=vertices+8*n;
  FUN_800120BC(ff_s16(v),ff_s16(v+2),ff_s16(v+4),0x801fc090);
  ff_w32(xy+4*n,(ff_u32(0x801fc090)&65535)|(ff_u32(0x801fc094)<<16));
  ff_w16(depths+2*n,(uint16)ff_u32(0x801fc098));
 }
 memcpy(ff_ptr(0x801fc090,16),saved,16);
 return ff_model_packets_80010CA0(ff_u32(model+12),ff_u32(model+8),xy,depths,bias);
}
GDB_CALL uint32 FUN_80067B8C(uint32 model,uint32 unused,sint32 bias) {
 return ff_static_model_80010B18(model,unused,bias);
}
/* 80011C60: reset matrix stack to scratchpad, then pitch followed by yaw.
 * The original uses unsigned halfword angle loads. Return is not consumed. */
GDB_CALL void FUN_80011C60(void) {
 ff_w32(0x8008d494,0x1f8003c0);FUN_80011D20();
 FUN_800101CC((uint16)ff_s16(0x8008d492));FUN_8001033C((uint16)ff_s16(0x8008d490));
}

/* 8001100C terrain grid: 18 projections per row, with the last point
 * overwritten by the following row (17-point stride). Packed X increments
 * carry into Y at zero: retain that original one-unit Y difference. */
GDB_CALL uint32 FUN_8001100C(sint32 unused,sint32 tile_x,sint32 tile_z) {
 uint32 row,col,n,packed,xy,p=ff_u32(0x8008d4b0),ot=ff_u32(0x8008d4b4)+4;
 uint32 head=ff_u32(ot),color=ff_u32(0x8008d4bc),sx=(uint32)tile_x<<1,sz=(uint32)tile_z<<7;
 (void)unused;
 for(row=0;row<17;row++)for(col=0;col<18;col++) {
  packed=0xf800+col*256;n=row*17+col;
  FUN_800120BC((sint16)packed,(sint16)(packed>>16),-2048+(sint32)row*256,0x801fc040);
  xy=(ff_u32(0x801fc040)&65535)|(ff_u32(0x801fc044)<<16);
  ff_w32(0x1fe800+4*n,xy);ff_w16(0x1f800000+2*n,(uint16)ff_u32(0x801fc048));
 }
 for(row=0;row<16;row++) {
  for(col=0;col<16;col++) {
   sint32 tile;uint32 v0,v1,v2,v3,t,uv;
   sx&=0x7e;sz&=0x1f80;tile=ff_s16(0x8009f870+sx+sz);n=row*17+col;
   if(tile!=-4&&ff_s16(0x1f800000+2*n)>0&&ff_s16(0x1f800024+2*n)>0) {
    v0=ff_u32(0x1fe800+4*n);v1=ff_u32(0x1fe804+4*n);v2=ff_u32(0x1fe844+4*n);v3=ff_u32(0x1fe848+4*n);
    ff_w32(p+8,v0); /* branch delay slot also executes for horizontal rejection */
    if((uint16)v0<320||(uint16)v1<320||(uint16)v2<320||(uint16)v3<320) {
     ff_w32(p+16,v1);ff_w32(p+24,v2);ff_w32(p+32,v3);t=0x800a8768+((uint32)tile<<2);
     ff_w32(p+12,ff_u32(t));ff_w32(p+20,ff_u32(t+4));uv=ff_u32(t+8);
     ff_w16(p+28,(uint16)uv);ff_w16(p+36,(uint16)(uv>>16));ff_w32(p+4,color);ff_w32(p,head|0x09000000);head=p;p+=40;
    }
   }
   sx-=2;
  }
  sx+=32;sz-=128;
 }
 ff_w32(ot,head);ff_w32(0x8008d4bc,0x2d000000);ff_w32(0x8008d4b0,p);return 0x8008d4b0;
}

/* 80064150: update ten alternating 320-byte scanlines. MULT uses low 32 bits
 * before SAR, and division by 160 applies after wrapped SLL 14. */
static sint32 ff_bg_product(sint32 a,sint32 b) {return (sint32)((uint32)a*(uint32)b)>>8;}
GDB_CALL sint32 FUN_80064150(void) {
 uint32 parity=ff_u32(0x8008d4c4)&1,p=0x800a1990+320*parity;
 sint32 base_x=~(sint32)ff_s16(0x8008d4a6),base_z=~(sint32)ff_s16(0x8008d4ae);
 sint32 angle=ff_s16(0x8008d490)>>5,sn=ff_s16(0x800802d0+2*((uint32)angle&2047)),cs=ff_s16(0x800802d0+2*((uint32)(angle+512)&2047));
 uint32 sn160=(uint32)sn*160,cs160=(uint32)cs*160,sn256=(uint32)sn<<8,cs256=(uint32)cs<<8;
 sint32 ax=(sint32)(0u-cs160-sn256)>>15,az=(sint32)(cs256-sn160)>>15;
 sint32 bx=(sint32)(cs160-sn256)>>15,bz=(sint32)(sn160+cs256)>>15;
 sint32 h=(sint32)((uint32)(sint32)ff_s16(0x8008d4aa)<<8),row,col;
 for(row=1;row<20;row+=2) {
  sint32 q=h/(row+(sint32)parity),x=ff_bg_product(q,ax),z=ff_bg_product(q,az);
  uint32 dx=(uint32)((sint32)(((uint32)ff_bg_product(q,bx)-(uint32)x)<<14)/160)<<1;
  uint32 dz=(uint32)((sint32)(((uint32)ff_bg_product(q,bz)-(uint32)z)<<14)/160)<<1;
  uint32 u=((uint32)x+(uint32)base_x)<<16,v=((uint32)z+(uint32)base_z)<<16;
  for(col=0;col<320;col++) {
   uint32 index=((uint32)((sint32)u>>23)&127)+((uint32)((sint32)v>>16)&0x3f80);
   *(uint8*)ff_ptr(p++,1)=*(uint8*)ff_ptr(0x800a3290+index,1);u+=dx;v+=dz;
  }
  p+=320;
 }
 return 0;
}

/* 80063EEC: four terrain grids and optional distant texture/quads. */
GDB_CALL sint32 FUN_80063EEC(void) {
 uint32 cx=ff_u32(0x8008d4a4),cz=ff_u32(0x8008d4ac),x=cx&0xffffff,z=cz&0xffffff;
 uint32 tx=(uint32)((sint32)~cx>>24),tz=(uint32)((sint32)~cz>>24);
 uint32 direction=(uint32)((ff_s16(0x8008d490)+2048)>>12)&15;int i;
 if(ff_s16(0x8008d492)<8192) {
  uint32 dx=ff_u32(0x80093a90+4*direction),dz=ff_u32(0x80093ad0+4*direction);
  x+=dx<<23;z+=dz<<23;tx+=(uint32)((sint32)dx>>1);tz+=(uint32)((sint32)dz>>1);
 }
 FUN_80011C60();
 for(i=0;i<4;i++) {
  FUN_80010040();FUN_800100C4((sint32)((i&1?0xf8010000u:0x07ff0000u)-x),(sint32)(0u-ff_u32(0x8008d4a8)),(sint32)((i&2?0xf8010000u:0x07ff0000u)-z));
  FUN_8001100C(0,(sint32)(tx+(i&1?16:0)),(sint32)(tz+(i&2?16:0)));FUN_80010028();
 }
 if(ff_u32(0x80093d58)==0xffffffff)return -1;
 FUN_80064150();ff_w32(0x801fc060,0);ff_w32(0x801fc064,ff_u32(0x8008d4d0));ff_w32(0x801fc068,32767);ff_w32(0x801fc06c,21);
 FUN_800128F4(0x801fc060,ff_s16(0x800af4b2)+256);ff_w32(0x801fc060,256);
 return (sint32)FUN_800128F4(0x801fc060,ff_s16(0x800af4b2)+257);
}

/* 80058E24: four menu actors, their shadows and selected-player rings. */
GDB_CALL sint32 FUN_80058E24(uint32 selected,sint32 players) {
 uint32 i,a;int slot,j,changed; sint32 red,value;
 if(ff_s16(0x8008d492)>=4097)return 0;
 FUN_80011D20();FUN_800101CC(ff_s16(0x8008d492));FUN_8001033C(ff_s16(0x8008d490));
 ff_w32(0x80093580,(ff_u32(0x80093580)+64)&2047);
 for(i=0;i<4;i++) {
  uint8 *colors; a=0x800b4318+244*i;colors=(uint8*)ff_ptr(a+156,4);changed=0;
  slot=players>0&&i==ff_u32(selected)?0:players>=2&&i==ff_u32(selected+4)?1:-1;
  red=(sint8)colors[1];
  if(slot>=0) {
   if(!(ff_u32(0x8009272c)&(1u<<slot))) {
    value=ff_s16(0x800802d0+2*ff_u32(0x80093580))>>10;changed=1;
    colors[1]=colors[2]=colors[3]=(uint8)value;
   } else if(red<32) {
    value=(sint8)(red+4);if(value>32)value=32;
    colors[1]=colors[2]=colors[3]=(uint8)value;changed=1;
   }
  } else if(ff_u32(0x8009278c)) {
   uint32 fade=ff_u32(0x8009278c)<<2;
   value=(sint32)fade>=129?-128:(sint32)(0u-fade);
   colors[1]=colors[2]=colors[3]=(uint8)value;changed=1;
  } else {
   if(red>0){for(j=1;j<=3;j++)colors[j]--;changed=1;}
   if((sint8)colors[1]<0){for(j=1;j<=3;j++)colors[j]++;changed=1;}
  }
  if(changed)colors[0]|=1;
  FUN_80010040();
  FUN_800100C4((sint32)(ff_u32(a+40)-ff_u32(0x8008d4a4)),(sint32)(ff_u32(a+44)+ff_u32(0x800990d8+4*(sint32)ff_s16(a+52))-ff_u32(0x8008d4a8)),(sint32)(ff_u32(a+48)-ff_u32(0x8008d4ac)));
  FUN_8001033C((sint16)(0u-32u*(uint32)(sint32)ff_s16(a+56))+32768);
  FUN_800100C4(0,(sint32)ff_u32(a+16),(sint32)ff_u32(a+32));
  ff_w32(0x8008d4bc,0x2c000000u+(uint32)((sint8)colors[1]+128)+((uint32)((sint8)colors[2]+128)<<8)+((uint32)((sint8)colors[3]+128)<<16));ff_w32(0x8008d4c0,0);
  FUN_8001060C(ff_u32(0x80081720+4*(sint32)ff_s16(a+52)),0x800ba210+504*i);FUN_80010028();
  FUN_80010040();FUN_800100C4((sint32)(ff_u32(a+40)-ff_u32(0x8008d4a4)),(sint32)(0u-ff_u32(0x8008d4a8)),(sint32)(ff_u32(a+48)-ff_u32(0x8008d4ac)));
  FUN_800101CC(32768);FUN_8001033C((sint16)(32u*(uint32)(sint32)ff_s16(a+56)));FUN_80067B8C(ff_u32(0x80082490),0,0);FUN_80010028();
  ff_w32(0x8009357c,(ff_u32(0x8009357c)+8)&2047);
  if(slot>=0) {
   FUN_80010040();FUN_800100C4((sint32)(ff_u32(a+40)-ff_u32(0x8008d4a4)),(sint32)(0u-ff_u32(0x8008d4a8)),(sint32)(ff_u32(a+48)-ff_u32(0x8008d4ac)));
   FUN_800101CC(32768);FUN_8001033C((sint16)(32u*ff_u32(0x8009357c)));FUN_80067B8C(ff_u32(slot==0?0x800824fc:0x80082500),0,0);FUN_80010028();
  }
 }
 return 0;
}

/* Shared rotation sequence with 8005B71C; that twin uses a different renderer. */
static void ff_object_rotations(uint32 object) {
 sint32 angle=ff_s16(object+16);if(angle)FUN_8001033C(angle);
 angle=ff_s16(object+20);if(angle)FUN_800104A0(angle);
 angle=ff_s16(object+18);if(angle)FUN_800101CC(angle);
}
GDB_CALL uint32 FUN_8002A0C4(uint32 object) {
 ff_object_rotations(object);
 return ff_static_model_80010B18(ff_u32(0x80081720+4*ff_u32(object)),0,0);
}
GDB_CALL uint32 FUN_80029F48(void) {
 uint32 model,angle=(uint16)ff_s16(0x8008d490);
 if((sint32)ff_u32(0x80093d60)<3||((angle+12288u)&65535u)>=24577u)
  ff_static_model_80010B18(ff_u32(0x8008180c),0,0);
 if((sint32)ff_u32(0x80093d60)<3)model=56;
 else model=(((uint32)(uint16)ff_s16(0x8008d490)+16384u)&65535u)>32768u?58:57;
 return ff_static_model_80010B18(ff_u32(0x80081720+4u*model),0,0);
}
GDB_CALL uint32 FUN_8003262C(uint32 object) {
 FUN_8001033C(ff_s16(object+16));return ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,128);
}
GDB_CALL uint32 FUN_8002A048(uint32 object) {
 if((((uint32)(uint16)ff_s16(0x8008d490)+16383u)&65535u)>=32767u)return 0;
 if(ff_s16(object+16))FUN_8001033C(ff_s16(object+16));
 return ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,0);
}
GDB_CALL uint32 FUN_8002AB40(uint32 object) {
 uint32 phase;
 if((sint32)ff_u32(0x80093dd0)<2)ff_w16(object+26,(uint16)ff_rand_8006D348());
 phase=(((uint32)(sint32)ff_s16(object+26)+ff_u32(0x80093dd0))<<5)&0xfe0u;
 FUN_800100C4(0,0,8*(sint32)ff_s16(0x800802d0+phase));
 return ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,0);
}
/* 11E5C MVMVA of zero vector: MAC1/3 equal the signed translations loaded
 * from matrix+20/+28 after SRA16; no perspective divide is performed. */
GDB_CALL sint32 FUN_80011E5C(void) {
 uint32 matrix=ff_u32(0x8008d494);sint32 x=(sint32)ff_u32(matrix+20)>>16,z=(sint32)ff_u32(matrix+28)>>16;
 if(z<80)return 0;if(x<=0)x=-x;return z-x-(z<800?z>>2:z>>3)>=0;
}
GDB_CALL uint32 FUN_8005AB84(uint32 object) {
 sint16 angle;if(!FUN_80011E5C())return 0;
 angle=(sint16)FUN_8001614C(ff_s16(object+6),ff_s16(object+14),ff_s16(0x800b4342),ff_s16(0x800b434a));
 ff_w16(object+16,(uint16)angle);FUN_8001033C(angle);
 return ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,0);
}
GDB_CALL uint32 FUN_8002A97C(uint32 object) {
 FUN_80010040();FUN_800100C4(0,0x800000,0);FUN_8001033C(ff_s16(object+16));
 FUN_800104A0(-ff_s16(object+20));FUN_800101CC(ff_s16(object+18));
 ff_static_model_80010B18(ff_u32(0x800818f8),0,0);FUN_800100C4(0,0x400000,0);
 FUN_8001033C((sint32)((uint32)(uint16)ff_u32(0x80093dd0)<<12));
 ff_static_model_80010B18(ff_u32(0x80081ff0),0,0);FUN_80010028();FUN_80010040();
 FUN_800100C4(0,(sint32)ff_u32(object+8),0);FUN_8001033C(ff_s16(object+16));
 ff_static_model_80010B18(ff_u32(0x80082428),0,0);return FUN_80010028();
}
/* 5AE3C: model transforms and six/two flashing sprites, actual MIPS order. */
GDB_CALL uint32 ff_car_render_8005AE3C(uint32 object) {
 static const sint32 lights[6][3]={{80,36,-376},{-80,36,-376},{128,36,384},{-128,36,384},{140,68,-176},{-140,68,-176}};
 uint32 original_kind=ff_u32(object),phase,shade,color,temp=0x801fc0c0,result; sint32 position[4];int i,j;
 if(ff_s16(object+16))FUN_8001033C(ff_s16(object+16));
 ff_static_model_80010B18(ff_u32(0x800818c8),0,8192);FUN_800100C4(0,2621440,0);
 if(ff_s16(object+20))FUN_800104A0(ff_s16(object+20));
 if(ff_s16(object+18))FUN_800101CC(ff_s16(object+18));
 ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,0);
 if(!ff_s16(object+30))return 0;
 phase=ff_u32(0x80093dd0)&15u;shade=phase<4?128u:phase<12?128u-16u*(phase-4u):0;
 color=0x2c000000u+(shade<<16)+(shade<<8)+shade;result=0;
 for(i=original_kind==94?0:4;i<6;i++) {
  FUN_800120BC(lights[i][0],lights[i][1],lights[i][2],temp);result=ff_u32(temp+12);
  if(!result) {
   for(j=0;j<3;j++)position[j]=(sint32)ff_u32(temp+4u*j);
   position[2]=(sint32)((uint32)position[2]-(i<4?32u:48u));position[3]=640;
   ff_w32(0x8008d4bc,color);result=FUN_80012594(position,ff_s16(0x8009ad4c));
  }
 }
 return result;
}

/* 800451A0: compound scene model, spinning component and ground shadow. */
GDB_CALL uint32 FUN_800451A0(uint32 object) {
 if((sint32)ff_u32(object+8)<=-536870912)return 0;
 FUN_80010040();FUN_8001033C(ff_s16(object+16));
 if(ff_s16(object+20))FUN_800104A0(ff_s16(object+20));
 FUN_800101CC(ff_s16(object+18)+4096);
 ff_static_model_80010B18(ff_u32(0x80082424),0,0);
 FUN_8001033C((sint32)((uint32)(uint16)ff_u32(0x80093dd0)<<12));
 ff_static_model_80010B18(ff_u32(0x80081ff0),0,0);FUN_80010028();
 FUN_80010040();FUN_800100C4(0,(sint32)ff_u32(object+8),0);
 FUN_8001033C(ff_s16(object+16));FUN_80067B8C(ff_u32(0x80082428),0,0);
 return FUN_80010028();
}

/* 800452AC: static object with three phase-offset effect attachments. */
GDB_CALL uint32 FUN_800452AC(uint32 object) {
 uint32 phase,i,p;
 FUN_80010040();FUN_8001033C(ff_s16(object+16));
 if(ff_s16(object+20))FUN_800104A0(ff_s16(object+20));
 if(ff_s16(object+18))FUN_800101CC(4096);
 ff_static_model_80010B18(ff_u32(0x80081720+4*ff_u32(object)),0,0);
 phase=ff_u32(0x80093dd0)&31;if(ff_u32(object)==836)phase=(phase+19)&31;
 FUN_800100C4(-4194304,16252928,0);
 for(i=0;i<3;i++) {
  if(i)FUN_800100C4(4194304,0,0);
  p=(phase+8*i)&31;
  if(p<6)ff_static_model_80010B18(ff_u32(p<2?0x80082440:p<4?0x8008243c:0x80082438),0,-256);
 }
 FUN_80010028();FUN_80010040();FUN_800100C4(0,(sint32)ff_u32(object+8),0);
 FUN_8001033C(ff_s16(object+16));FUN_80067B8C(ff_u32(0x80082434),0,0);
 return FUN_80010028();
}

GDB_CALL uint32 FUN_80059394(void) {
 uint32 shade=64u-16u*(ff_u32(0x80093dd0)&1u);
 ff_w32(0x8008d4bc,0x2c000000u+(shade<<16)+(shade<<8)+shade);
 return ff_static_model_80010B18(ff_u32(0x8008257c),0,4096);
}
/* 5AC08 compound roadside model, depth shade and alternating shadow. */
GDB_CALL uint32 FUN_8005AC08(uint32 object) {
 uint32 type=ff_u32(object),position[3];sint32 depth,shade;
 if(!FUN_80011E5C())return 0;
 FUN_800614F4(0,0,0,position);FUN_8001033C(ff_s16(object+16));
 depth=(sint16)(position[2]>>16);
 if(depth>8192) {
  shade=(depth-8192)>>7;if(shade>96)shade=96;shade=128-shade;
  ff_w32(0x8008d4bc,0x2c000000u+((uint32)shade<<16)+((uint32)shade<<8)+(uint32)shade);
 }
 ff_static_model_80010B18(ff_u32(0x80081720+4u*type),0,0);
 ff_static_model_80010B18(ff_u32(type==80?0x80081884:0x800818c8),0,8192);
 FUN_8001033C(-16384);FUN_800100C4(-25165824,0,0);
 if(type==78)FUN_800100C4(-8388608,0,0);
 if(type==80)FUN_800100C4(-16777216,0,0);
 return FUN_80059394();
}
/* 8005C17C: object render dispatch. Unported targets are explicit WIP failures. */
GDB_CALL sint32 FUN_8005C17C(void) {
 sint32 i,count;uint32 object=0x800b89f0,type,target;
 FUN_80011C60();count=(sint32)ff_u32(0x8009403c);if(count<=0)return count;
 for(i=0;i<(sint32)ff_u32(0x8009403c);i++,object+=36) {
  type=ff_u32(object);if(type==55)continue;
  if(!ff_u32(0x80081720+4*type))ff_wip_stop(__FUNCTION__,__FILE__,__LINE__); /* original fatal loop */
  FUN_80010040();FUN_800100C4((sint32)(ff_u32(object+4)-ff_u32(0x8008d4a4)),(sint32)(ff_u32(object+8)-ff_u32(0x8008d4a8)),(sint32)(ff_u32(object+12)-ff_u32(0x8008d4ac)));
  FUN_800101CC(32768);target=ff_u32(0x80097408+4*type);
  if(target==0x8002a0c4)FUN_8002A0C4(object);
  else if(target==0x80029f48)FUN_80029F48();
  else if(target==0x8003262c)FUN_8003262C(object);
  else if(target==0x8002a048)FUN_8002A048(object);
  else if(target==0x8002ab40)FUN_8002AB40(object);
  else if(target==0x8005ab84)FUN_8005AB84(object);
  else if(target==0x8005ac08)FUN_8005AC08(object);
  else if(target==0x8002a97c)FUN_8002A97C(object);
  else if(target==0x8005b178)FUN_8005B178(object);
  else if(target==0x8005b350)FUN_8005B350(object);
  else if(target==0x8005ae3c)ff_car_render_8005AE3C(object);
  else if(target==0x800451a0)FUN_800451A0(object);
  else if(target==0x800452ac)FUN_800452AC(object);
  else if(!ff_wip_visit(target,"render",__FUNCTION__,__FILE__,__LINE__,1,"skip_unknown_callback"))abort(); /* Diagnostic skip only. */
  FUN_80010028();
 }
 return 0;
}

/* 2B2E4 stationary branch and lifetime tail. Incoming a1 is overwritten by
 * LH object+24 in actual MIPS; IDA's single-argument signature is valid here. */
GDB_CALL sint32 FUN_8002B2E4(uint32 object) {
 uint32 type=ff_u32(object);sint32 life;
 if(ff_s16(object+24)>0) {
  uint32 angle=(uint16)ff_s16(object+16)>>5,x=ff_u32(object+4),z=ff_u32(object+12),point[4];
  sint32 sine=ff_s16(0x800802d0+2u*angle),cosine=ff_s16(0x800806d0+2u*angle);
  sint32 dx=(sint32)((uint32)(sint32)ff_s16(object+24)*(uint32)sine)>>1;
  sint32 dz=(sint32)((uint32)(sint32)ff_s16(object+24)*(uint32)cosine)>>1;
  uint32 reach_x=(uint32)sine<<7,reach_z=(uint32)cosine<<7;int step;
  if(((uint16)ff_s16(object+34)&1)&&!FUN_80019718(x,z))ff_w16(object+34,(uint16)(ff_s16(object+34)-1));
  for(step=0;step<2;step++) {
   if(!((uint16)ff_s16(object+34)&1)) {
    if(ff_s16(object+24)>=13&&type-586u>=12u) {
     point[0]=x+reach_x;point[1]=ff_u32(object+8);point[2]=z-reach_z;point[3]=1;
     {
      sint32 hit=FUN_8001BFA0(ff_s16(object+28),point,ff_u32(object));
      if(hit) {
       if(type==526)FUN_80056670(30,255,6144);
       if(type==478||type==480||type==482){if(ff_wip_visit(0x8002b478,"object",__FUNCTION__,__FILE__,__LINE__,1,"return_zero_from_function"))return 0;abort();} /* WIP breakable material effects */
       if(type-499u>=5u&&type!=504&&type!=506&&type!=515&&type-496u>=2u&&type!=505) {
        ff_w16(object+22,(uint16)(ff_s16(object+22)-1));
        if(ff_s16(object+22)<=0)ff_w16(object+22,65535);
       }
       if(hit==2){dx=dz=0;ff_w16(object+24,1);}
       else ff_w16(object+24,(uint16)(ff_s16(object+24)>>1));
      }
     }
    }
    if(!((uint16)ff_s16(object+34)&1)) {
     if(FUN_80019718(x+reach_x,z-reach_z)&1){dx=dz=0;ff_w16(object+24,1);}
     if(!((uint16)ff_s16(object+34)&1)&&(FUN_80019718(x+2u*(uint32)dx,z-2u*(uint32)dz)&1)){dx=dz=0;ff_w16(object+24,1);}
    }
   }
   x+=(uint32)dx;z-=(uint32)dz;
  }
  ff_w32(object+4,x);ff_w32(object+12,z);
  ff_w16(object+32,(uint16)(ff_s16(object+32)-16));
  ff_w32(object+8,ff_u32(object+8)-(uint32)((sint32)((uint32)(uint16)ff_s16(object+32)<<16)>>1));
  ff_w16(object+18,(uint16)(ff_s16(object+18)+(ff_s16(object+26)>>1)));
  if((sint32)ff_u32(object+8)>393216) {
   /* 2B714..2B8CC: material sound and diminishing volume on each bounce. */
   sint32 volume=255>>((uint16)ff_s16(object+30)&31u),sample=29,pitch=6144;
   int stop_spin=0;
   ff_w16(object+30,(uint16)(ff_s16(object+30)+1));
   if(type==526){sample=31;stop_spin=1;}
   else if(type==525||type==506||type==515||type==540||type==532||type==524){sample=219;pitch=4096;}
   else if(type-543u<2u||type-553u<2u||type-555u<2u||type==535||type-522u<2u)sample=221;
   else if(type==484||type==492||type==495)sample=218;
   else if(type-499u<5u||type==505){sample=220;pitch=7168;}
   else if(type==497)sample=52;
   else if(type==504)sample=220;
   else if(type==477||type==485||type==487||type==489||type-493u<2u||type==491||type==511||type==520)sample=219;
   else if(type-473u<3u||type-478u<6u){sample=222;stop_spin=1;}
   else if((sint32)type>=522){sample=31;stop_spin=1;}
   if(!stop_spin||ff_s16(object+26))FUN_80056670(sample,volume,pitch);
   if(stop_spin){ff_w16(object+26,0);ff_w16(object+32,0);}
  }
  if((sint32)ff_u32(object+8)>0) {
   ff_w16(object+34,(uint16)ff_s16(object+34)&65534u);ff_w16(object+18,0);ff_w32(object+8,0);
   ff_w16(object+32,(uint16)(-ff_s16(object+32)>>1));
  }
  ff_w16(object+24,(uint16)(ff_s16(object+24)-3));if(ff_s16(object+24)<0)ff_w16(object+24,0);
  if((ff_u32(object+8)==1||(sint32)ff_u32(object+8)<0)&&!ff_s16(object+24))ff_w16(object+24,3);
 } else if(ff_u32(object+8)!=1) {
  ff_w16(object+30,0);ff_w16(object+18,0);ff_w32(object+8,0);
  if(!ff_s16(object+22)&&(type==506||type==515||type-499u<5u||type-504u<2u))ff_w16(object+22,65535);
 }
 life=ff_s16(object+22);
 if(ff_u32(object)-587u<11u&&life>=0){ff_w16(object+22,(uint16)(life-1));life=ff_s16(object+22);}
 if(life>=0)return life;
 ff_w16(object+22,(uint16)(life-1));if(ff_s16(object+22)<-60)ff_w32(object,0xffffffffu);
 return -1;
}
GDB_CALL sint32 FUN_8002BA48(uint32 object) { return FUN_8002B2E4(object); }
/* 2BA68 supplies a1=0 instead of1024;2B2E4 overwrites it before any use. */
GDB_CALL sint32 FUN_8002BA68(uint32 object) { return FUN_8002B2E4(object); }
/* 121A8/12240/122E0: shared RGB555 and depth-table OT insertion.
 * Actual MIPS retains complete pointer/tag words, including alias bits. */
static uint32 ff_particle_packet(sint32 x,sint32 y,sint32 z,uint32 color,uint32 shape) {
 uint32 p=ff_u32(0x8008d4b0),xy=((uint32)x&65535)|((uint32)y<<16),bucket;
 ff_w32(p+4,(shape==2?0x28000000u:0x40000000u)|((color&31)<<3)|((color&0x3e0)<<6)|((color&0x7c00)<<9));
 ff_w32(p+8,xy);
 if(shape==2){xy+=2;ff_w32(p+12,xy);xy+=0x20000;ff_w32(p+20,xy);ff_w32(p+16,xy-2);}
 else ff_w32(p+12,xy+(shape?0x10000u:0));
 bucket=ff_u32(0x8008d4b4)+(uint32)(sint32)ff_s16(ff_u32(0x8008d4d8)+2u*(uint32)(z>>2));
 ff_w32(p,ff_u32(bucket)|(shape==2?0x05000000u:0x03000000u));ff_w32(bucket,p);
 ff_w32(0x8008d4b0,p+(shape==2?24:16));return 0x8008d4b0;
}
GDB_CALL uint32 FUN_800121A8(sint32 x,sint32 y,sint32 z,uint32 color){return ff_particle_packet(x,y,z,color,0);}
GDB_CALL uint32 FUN_80012240(sint32 x,sint32 y,sint32 z,uint32 color){return ff_particle_packet(x,y,z,color,1);}
GDB_CALL uint32 FUN_800122E0(sint32 x,sint32 y,sint32 z,uint32 color){return ff_particle_packet(x,y,z,color,2);}
/* 5C2F0: original stack output uses host-private RAM temporary here. */
GDB_CALL uint32 FUN_8005C2F0(uint32 object) {
 uint32 temp=0x801fc0a0,table,color;sint32 age,x,y,z;
 FUN_80010040();FUN_800100C4((sint32)(ff_u32(object+4)-ff_u32(0x8008d4a4)),(sint32)(ff_u32(object+8)-ff_u32(0x8008d4a8)),(sint32)(ff_u32(object+12)-ff_u32(0x8008d4ac)));
 FUN_800120BC(0,0,0,temp);
 if(!ff_u32(temp+12)) {
  age=ff_s16(object+26);table=ff_u32(object)==1?0x80080050:ff_u32(0x80093d70)==2?0x80080070:0x80080030;
  color=(uint32)(sint32)ff_s16(table+2u*(uint32)(age>>2));x=(sint32)ff_u32(temp);y=(sint32)ff_u32(temp+4);z=(sint32)ff_u32(temp+8);
  if(age>=17)FUN_800121A8(x,y,z,color);else if(age>=9)FUN_80012240(x,y,z,color);else FUN_800122E0(x,y,z,color);
 }
 return FUN_80010028();
}
/* 5C480: fixed palette80080090, signed age/4 and size thresholds9/17. */
GDB_CALL uint32 FUN_8005C480(uint32 object) {
 FF_FUNCTION_MARKER(0x8005c480u,"GAME.EXE");
 uint32 temp=0x801fc0d0,table,color;sint32 age,x,y,z;
 FUN_80010040();FUN_800100C4((sint32)(ff_u32(object+4)-ff_u32(0x8008d4a4)),(sint32)(ff_u32(object+8)-ff_u32(0x8008d4a8)),(sint32)(ff_u32(object+12)-ff_u32(0x8008d4ac)));
 FUN_800120BC(0,0,0,temp);
 if(!ff_u32(temp+12)) {
  age=ff_s16(object+26);table=0x80080090;
  color=(uint32)(sint32)ff_s16(table+2u*(uint32)(age>>2));x=(sint32)ff_u32(temp);y=(sint32)ff_u32(temp+4);z=(sint32)ff_u32(temp+8);
  if(age>=17)FUN_800121A8(x,y,z,color);else if(age>=9)FUN_80012240(x,y,z,color);else FUN_800122E0(x,y,z,color);
 }
 return FUN_80010028();
}
/* 2BEC0: signed table products, gravity, ground removal and saturating age.
 * The return still increments when age >=63 and its store is skipped. */
GDB_CALL sint32 FUN_8002BEC0(uint32 object) {
 sint32 speed=ff_s16(object+24),angle=ff_s16(object+16),vertical,age;uint32 y;
 ff_w32(object+4,ff_u32(object+4)+(uint32)((speed*ff_s16(0x800802d0+2u*(uint32)angle))>>1));
 ff_w32(object+12,ff_u32(object+12)-(uint32)((speed*ff_s16(0x800806d0+2u*(uint32)angle))>>1));
 vertical=ff_s16(object+32);if(vertical>=-255)ff_w16(object+32,(uint16)(vertical-2));
 y=ff_u32(object+8)-((uint32)(sint32)ff_s16(object+32)<<15);ff_w32(object+8,y);
 if((sint32)y>0)ff_w32(object,0xffffffff);
 age=ff_s16(object+26);if(age<63)ff_w16(object+26,(uint16)(age+1));return age+1;
}
/* 2B28C: remove the impact sprite at age6, then increment its unsigned age. */
GDB_CALL uint32 FUN_8002B28C(uint32 object) {
 uint32 result;if(ff_s16(object+22)==6)ff_w32(object,0xffffffff);
 result=(uint32)(uint16)ff_s16(object+22)+1;ff_w16(object+22,(uint16)result);return result;
}
/* Native target mapping; additional game callbacks remain explicit WIP. */
/* 2AF78: four ordered environment probes; expiry does not skip this update. */
GDB_CALL sint32 FUN_8002AF78(uint32 object) {
 FF_FUNCTION_MARKER(0x8002af78u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)(sint32)ff_s16(object+28),point[4],angle,dx,dz;
 sint16 age=(sint16)((uint16)ff_s16(object+22)+1u);sint32 result;int i;
 ff_w16(object+22,(uint16)age);if(age>=16)ff_w32(object,0xffffffffu);
 angle=((uint32)(uint16)ff_s16(object+22)<<8)&0xf00u;
 dx=((uint32)(sint32)ff_s16(0x800802d0+angle)*(uint32)(sint32)ff_s16(object+22))<<7;
 dz=((uint32)(sint32)ff_s16(0x800806d0+angle)*(uint32)(sint32)ff_s16(object+22))<<7;
 point[1]=0;point[3]=0;
 for(i=0;i<4;i++) {
  point[0]=(i&1)?ff_u32(actor+40)-dx:ff_u32(actor+40)+dx;
  point[2]=(i==0||i==3)?ff_u32(actor+48)-dz:ff_u32(actor+48)+dz;
  result=FUN_8001BBE8(point,ff_s16(object+28));if(result)return result;
 }
 return result;
}
/* 2B120: signed halfword lifetime; v0 is -1 on both branches. */
GDB_CALL uint32 FUN_8002B120(uint32 object) {
 sint16 age=(sint16)((uint16)ff_s16(object+22)+1u);ff_w16(object+22,(uint16)age);
 if(age>=8)ff_w32(object,0xffffffffu);return 0xffffffffu;
}
/* 5D3D8: signed age scaling and unclamped wrapping grayscale, MIPS exact. */
GDB_CALL uint32 FUN_8005D3D8(uint32 object) {
 FF_FUNCTION_MARKER(0x8005d3d8u,"GAME.EXE");
 uint32 shade,kind; sint32 scale;
 FUN_80010040();FUN_8005C740(object);
 scale=(sint32)((uint32)(sint32)ff_s16(object+22)<<12);
 FUN_80010F94(scale,scale,scale);
 shade=64u-((uint32)(sint32)ff_s16(object+22)<<2);
 ff_w32(0x8008d4bc,0x2c000000u+(shade<<16)+(shade<<8)+shade);
 kind=ff_u32(object);
 ff_static_model_80010B18(ff_u32(0x80081720+4u*kind),0,kind==604?0:8192);
 return FUN_80010028();
}
/* 5D488: expanding ground effect; scale and grayscale follow actual MIPS. */
GDB_CALL uint32 FUN_8005D488(uint32 object) {
 uint32 shade; sint32 scale;
 FUN_80010040();FUN_8005C740(object);
 scale=(sint32)((uint32)(sint32)ff_s16(object+22)<<13);
 if(ff_u32(object)==607)scale>>=1;
 FUN_80010F94(scale,scale,scale);
 shade=64u-((uint32)(sint32)ff_s16(object+22)<<3);
 ff_w32(0x8008d4bc,0x2c000000u+(shade<<16)+(shade<<8)+shade);
 ff_static_model_80010B18(ff_u32(0x80081720+4u*ff_u32(object)),0,0);
 return FUN_80010028();
}
/* 2AE64: dust age28 expiry; signed acceleration from wrapped halfword.
 * Indexed twin2AE10 has a different lifetime threshold; keep it distinct. */
GDB_CALL uint32 FUN_8002AE64(uint32 object) {
 uint16 age=(uint16)((uint16)ff_s16(object+22)+1u);
 uint16 speed=(uint16)((uint16)ff_s16(object+26)+1u);
 uint32 delta=(uint32)((sint32)((uint32)speed<<16)>>1);
 ff_w16(object+22,age);if((sint16)age>=28)ff_w32(object,0xffffffffu);
 ff_w16(object+26,speed);ff_w32(object+8,ff_u32(object+8)-delta);return delta;
}
/* 2AE10: indexed twin2AE64; only signed expiry threshold differs (14/28). */
GDB_CALL uint32 FUN_8002AE10(uint32 object) {
 FF_FUNCTION_MARKER(0x8002ae10u,"GAME.EXE");
 uint16 age=(uint16)((uint16)ff_s16(object+22)+1u),speed;
 uint32 delta;
 ff_w16(object+22,age);if((sint16)age>=14)ff_w32(object,0xffffffffu);
 speed=(uint16)((uint16)ff_s16(object+26)+1u);
 delta=(uint32)((sint32)((uint32)speed<<16)>>1);
 ff_w16(object+26,speed);ff_w32(object+8,ff_u32(object+8)-delta);return delta;
}
/* 11FEC: original JR RA/NOP; no result or side effects. */
GDB_CALL void FUN_80011FEC(void) {
 FF_FUNCTION_MARKER(0x80011fecu,"GAME.EXE");}
GDB_CALL uint32 ff_object_call(uint32 target,uint32 object) {
 if(target==0x800420f8) {
  if((sint32)ff_u32(0x80093d60)>=3&&ff_s16(object+26)==2&&!(ff_u32(0x80093dd0)&1u)) {
   if(!ff_wip_visit(target,"retained-stack",__FUNCTION__,__FILE__,__LINE__,1,"skip helicopter impact update: caller stack projectile unavailable"))abort();
   return 0;
  }
  /* Outside that branch the original never reads the local projectile. */
  return FUN_800420F8(object,0);
 }

 if(target==0x8002d7c8)return FUN_8002D7C8(object);
 if(target==0x8002dba0)return FUN_8002DBA0(object);
 if(target==0x8002da3c)return FUN_8002DA3C(object);
 if(target==0x8002cde0)return FUN_8002CDE0(object);
 if(target==0x8002d5d4)return FUN_8002D5D4(object);
 if(target==0x8002f59c)return FUN_8002F59C(object);
 if(target==0x80043978)return FUN_80043978(object);
 if(target==0x8002a168)return FUN_8002A168(object);
 if(target==0x8003fdcc)return FUN_8003FDCC(object);
 if(target==0x8003f9c0)return FUN_8003F9C0(object);
 if(target==0x80044844)return FUN_80044844(object);
 if(target==0x80042ba0)return FUN_80042BA0(object);
 if(target==0x80044334)return FUN_80044334(object);
 switch(target) {
 case 0x8002c060:return FUN_8002C060(object);
 case 0x8002a858:return FUN_8002A858();
 case 0x8002a8fc:return FUN_8002A8FC(object);
 case 0x8002ace8:return FUN_8002ACE8(object);
 case 0x8002ad7c:return FUN_8002AD7C(object);
 case 0x8002b150:return FUN_8002B150(object);
 case 0x80032680:return FUN_80032680(object);
 case 0x800326dc:return FUN_800326DC(object);
 case 0x80032728:return FUN_80032728(object);
 case 0x800327b0:return FUN_800327B0();
 case 0x80032aa4:return FUN_80032AA4(object);
 case 0x80032b20:return FUN_80032B20(object);
 case 0x8002a00c:return FUN_8002A00C(object);
 case 0x8002a7dc:return FUN_8002A7DC(object);
 case 0x8002a89c:return FUN_8002A89C();
 case 0x8002aa6c:return FUN_8002AA6C(object);
 case 0x8002abe0:return FUN_8002ABE0(object);
 case 0x8002c294:return FUN_8002C294(object);
 case 0x80032774:return FUN_80032774(object);
 case 0x80032b98:return FUN_80032B98(object);
 case 0x8003ee7c:return FUN_8003EE7C(object);
 case 0x8005bc64:return FUN_8005BC64(object);
 case 0x8005bf0c:return FUN_8005BF0C(object);
 case 0x8005bf7c:return FUN_8005BF7C(object);
 case 0x80032818:return FUN_80032818(object);
 case 0x80032a48:return FUN_80032A48(object);
 case 0x8005c598:return FUN_8005C598(object);
 case 0x8005d53c:return FUN_8005D53C(object);
 case 0x8002e37c:return (uint32)FUN_8002E37C(object);
 case 0x80032d70:return FUN_80032D70(object);
 case 0x80032e20:return (uint32)FUN_80032E20(object);
 case 0x8002bfa8:return FUN_8002BFA8(object);
 case 0x8005b4f8:return FUN_8005B4F8(object);
 case 0x8005b350:return FUN_8005B350(object);
 case 0x80044cd4:return FUN_80044CD4(object);
 case 0x8005cd1c:return FUN_8005CD1C(object);
 case 0x8005bbac:return FUN_8005BBAC(object);
 case 0x8002ed7c:return FUN_8002ED7C(object);
case 0x8005b71c:return FUN_8005B71C(object);
case 0x8005c0e8:return FUN_8005C0E8(object);
case 0x800428ec:return FUN_800428EC(object);
case 0x80042848:return FUN_80042848(object);
case 0x80032d44:return FUN_80032D44();
 case 0x800450f8:return FUN_800450F8(object);
 case 0x8004277c:return FUN_8004277C(object);
 case 0x8005b684:return FUN_8005B684(object);
 case 0x8005b7ac:return FUN_8005B7AC(object);
 case 0x8005d160:return FUN_8005D160(object);
 case 0x8005d608:return FUN_8005D608(object);
 case 0x8005d7a4:return FUN_8005D7A4(object);
 case 0x8005bd7c:return FUN_8005BD7C(object);
 case 0x8005ab18:return FUN_8005AB18(object);
 case 0x80041d60:return FUN_80041D60(object);
 case 0x80043680:return FUN_80043680(object);
 case 0x8003f088:return FUN_8003F088(object);
 case 0x800403dc:return FUN_800403DC(object);
 case 0x8002efdc:return FUN_8002EFDC(object);
 case 0x8002fa90:return FUN_8002FA90(object);
 case 0x8002f2e0:return FUN_8002F2E0(object);
 case 0x8002bc1c:return FUN_8002BC1C(object);
 case 0x800469a4:return FUN_800469A4(object);
 case 0x8002e11c:return FUN_8002E11C(object);
 case 0x8004322c:return FUN_8004322C(object);
 case 0x80046c08:return FUN_80046C08(object);
 case 0x80046ef0:return FUN_80046EF0(object);
 case 0x8002eb6c:return FUN_8002EB6C(object);
 case 0x8005b844:return FUN_8005B844(object);
 case 0x8002a640:return FUN_8002A640(object);
 case 0x80042f80:return FUN_80042F80(object);
 case 0x800429f0:return FUN_800429F0(object);
 case 0x8002ff30:return FUN_8002FF30(object);
 case 0x8002ee60:return FUN_8002EE60(object);
 case 0x8005ba28:return FUN_8005BA28(object);
 case 0x8002c2e4:return FUN_8002C2E4(object);
 case 0x800419d4:return FUN_800419D4(object);
 case 0x8002e9fc:return FUN_8002E9FC(object);
 case 0x8002d678:return FUN_8002D678(object);
 case 0x8002e8a4:return FUN_8002E8A4(object);
 case 0x8005cee4:return FUN_8005CEE4(object);
 case 0x8002f950:return FUN_8002F950(object);
 case 0x8002c55c:return FUN_8002C55C(object);
 case 0x8005d024:return FUN_8005D024(object);
 case 0x8003f6ec:return FUN_8003F6EC(object);
 case 0x8005cdbc:return FUN_8005CDBC(object);
 case 0x8004471c:return FUN_8004471C(object);
 case 0x8002d8fc:return FUN_8002D8FC(object);
 case 0x800300bc:return FUN_800300BC(object);
 case 0x800416e0:return FUN_800416E0(object);
 case 0x800454e0:return FUN_800454E0(object);
 case 0x80043128:return FUN_80043128(object);
 case 0x8002c454:return FUN_8002C454(object);
 case 0x8005b404:return FUN_8005B404(object);
 case 0x80032c4c:return FUN_80032C4C(object);
 case 0x8005b598:return FUN_8005B598(object);
 case 0x8005dad4:return FUN_8005DAD4(object);
 case 0x80041e6c:return FUN_80041E6C(object);
 case 0x8005ad54:return FUN_8005AD54(object);
 case 0x8005d1fc:return FUN_8005D1FC(object);
 case 0x80046e1c:FUN_80046E1C(object);return 0; /* Callback's return is unused; original leaves v0 undefined on early exit. */
 case 0x80041894:return FUN_80041894(object);
 case 0x8002cac4:return FUN_8002CAC4(object);
 case 0x80041dd8:return FUN_80041DD8(object);
 case 0x8004160c:return FUN_8004160C(object);
 case 0x80047108:FUN_80047108(object);return 0;
 case 0x8005bff4:return FUN_8005BFF4(object);
 case 0x8005d2e4:return FUN_8005D2E4(object);
 case 0x8005d340:return FUN_8005D340(object);
 case 0x8002c154:return FUN_8002C154(object);
 case 0x8002c1f4:return FUN_8002C1F4(object);
 case 0x8002ccbc:return FUN_8002CCBC(object);
 case 0x8002ac8c:return FUN_8002AC8C(object);
 case 0x8002b1e4:return FUN_8002B1E4(object);
 case 0x8002d218:return FUN_8002D218(object);
 case 0x8002e444:return FUN_8002E444(object);
 case 0x8002c69c:return FUN_8002C69C(object);
 case 0x80044d90:return FUN_80044D90(object);
 case 0x8002cef8:return (uint32)FUN_8002CEF8(object);
 case 0x8002d2a4:return FUN_8002D2A4(object);
 case 0x8002df00:return FUN_8002DF00(object);
 case 0x8002dfa4:return FUN_8002DFA4(object);
 case 0x8003f388:return FUN_8003F388(object);
 case 0x8003f410:return FUN_8003F410(object);
 case 0x800417ec:return FUN_800417EC(object);
 case 0x8002e7c4:return FUN_8002E7C4(object);
 case 0x8002aeb8:return FUN_8002AEB8(object);
 case 0x8002aee8:return FUN_8002AEE8(object);
 case 0x8002af18:return FUN_8002AF18(object);
 case 0x8002af48:return FUN_8002AF48(object);
 case 0x8002b2b4:return FUN_8002B2B4(object);
 case 0x80041974:return FUN_80041974(object);
 case 0x800419a4:return FUN_800419A4(object);
 case 0x8002da0c:return FUN_8002DA0C(object);
 case 0x8002e7f0:return FUN_8002E7F0(object);
 case 0x8002e870:return FUN_8002E870(object);
 case 0x8004348c:return FUN_8004348C(object);
 case 0x8002cba0:return FUN_8002CBA0(object);
 case 0x8002ce6c:return FUN_8002CE6C(object);
 case 0x8002e038:return FUN_8002E038(object);
 case 0x8002c1dc:return FUN_8002C1DC(object);
 case 0x8002c27c:return FUN_8002C27C(object);
 case 0x800415f4:return FUN_800415F4(object);
 case 0x80041600:return FUN_80041600(object);
 case 0x8002eb58:return FUN_8002EB58(object);
 case 0x8002a974:FUN_8002A974();return 0;
 case 0x80041b4c:FUN_80041B4C();return 0;
 case 0x8004483c:FUN_8004483C();return 0;
 case 0x800455dc:FUN_800455DC();return 0;
 case 0x8005c654:FUN_8005C654();return 0;
 case 0x8005c65c:FUN_8005C65C();return 0;
 case 0x8005c664:FUN_8005C664();return 0;
 case 0x8005d150:FUN_8005D150();return 0;
 case 0x8005d158:FUN_8005D158();return 0;
 case 0x8005dacc:FUN_8005DACC();return 0;
 case 0x8005d67c:return FUN_8005D67C(object);
 case 0x8002ddb8:return FUN_8002DDB8(object);
 case 0x8002af78:return (uint32)FUN_8002AF78(object);
 case 0x8002ae10:return FUN_8002AE10(object);
 case 0x8002ae64:return FUN_8002AE64(object);
 case 0x8002b120:return FUN_8002B120(object);
 case 0x8005d3d8:return FUN_8005D3D8(object);
 case 0x8005d488:return FUN_8005D488(object);
 case 0x8005c480:return FUN_8005C480(object);
 case 0x8005c2f0:return FUN_8005C2F0(object);
 case 0x8005c808:return FUN_8005C808(object);
 case 0x8002bec0:return (uint32)FUN_8002BEC0(object);
 case 0x8002f828:return FUN_8002F828(object);
 case 0x8002f700:return FUN_8002F700(object);
 case 0x8002b28c:return FUN_8002B28C(object);
 case 0x8002ba48:return (uint32)FUN_8002BA48(object);
 case 0x8002ba68:return (uint32)FUN_8002BA68(object);
 case 0x8005da8c:case 0x8005daac:return FUN_8005D820(object);
 case 0x8002a0c4:return FUN_8002A0C4(object);
 case 0x800451a0:return FUN_800451A0(object);
 case 0x800452ac:return FUN_800452AC(object);
 case 0x80043520:return (uint32)FUN_80043520(object);
 default:if(ff_wip_visit(target,"object_callback",__FUNCTION__,__FILE__,__LINE__,1,"skip_unknown_callback_return_zero"))return 0;abort();
 }
}
/* 8001EDB8: all128 effect slots are visited; absent callbacks are skipped. */
GDB_CALL sint32 FUN_8001EDB8(void) {
 uint32 i,object,type,target;
 for(i=0,object=0x8009d1d0;i<128;i++,object+=36) {
  type=ff_u32(object);if(type==0xffffffff)continue;
  target=ff_u32(0x800be5e0+4*type);if(target)ff_object_call(target,object);
 }
 return 0;
}

/* 8005C66C: default effect model with height offset and yaw/pitch/roll order. */
GDB_CALL uint32 FUN_8005C66C(uint32 object) {
 FUN_80010040();
 FUN_800100C4((sint32)(ff_u32(object+4)-ff_u32(0x8008d4a4)),(sint32)(ff_u32(object+8)+ff_u32(0x800990d8+4*ff_u32(object))-ff_u32(0x8008d4a8)),(sint32)(ff_u32(object+12)-ff_u32(0x8008d4ac)));
 FUN_800101CC(32768);FUN_8001033C(ff_s16(object+16));FUN_800101CC(ff_s16(object+18));FUN_800104A0(ff_s16(object+20));
 ff_static_model_80010B18(ff_u32(0x80081720+4*ff_u32(object)),0,0);return FUN_80010028();
}
/* 8001EF68: render128 effect slots, falling back when table entry is null. */
GDB_CALL sint32 FUN_8001EF68(void) {
 uint32 i,object,type,target;
 for(i=0,object=0x8009d1d0;i<128;i++,object+=36) {
  type=ff_u32(object);if(type==0xffffffff)continue;
  target=ff_u32(0x80098270+4*type);
  if(target)ff_object_call(target,object);else FUN_8005C66C(object);
 }
 return 0;
}

/* 8001ED1C:64 dynamic object updates. Default physics remains WIP. */
GDB_CALL sint32 FUN_8001ED1C(void) {
 uint32 i,object,type,target;
 for(i=0,object=0x8009a3c8;i<64;i++,object+=36) {
  type=ff_u32(object);if(type==0xffffffff)continue;
  target=ff_u32(0x800bbf78+4*type);
  ff_object_call(target?target:0x8002ba48,object);
 }
 return 0;
}

/* 8001EE44: hide dynamic records referenced by any actor attachment. */
GDB_CALL sint32 FUN_8001EE44(void) {
 uint8 attached[64]={0};uint32 i,j,object,type,target;sint32 index;
 for(i=0;i<8;i++)for(j=0;j<2;j++) {
  index=*(sint8*)ff_ptr(0x800b4395+244*i+j,1);
  if(index!=-1){if((uint32)index>=64)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);attached[index]=1;}
 }
 for(i=0,object=0x8009a3c8;i<64;i++,object+=36) {
  type=ff_u32(object);if(type==0xffffffff||attached[i])continue;
  target=ff_u32(0x800bff00+4*type);ff_object_call(target?target:0x8005da8c,object);
 }
 return 0;
}

/* 80064684: original double-buffer OT and packet arena selection. */
GDB_CALL uint32 FUN_80064684(void) {
 uint32 parity=ff_u32(0x8008d4c4)&1;
 ff_w32(0x8008d4b8,parity?0xe5140:0xe7140);
 ff_w32(0x8008d4b4,parity?0xe7140:0xe5140);
 ff_w32(0x8008d4b0,parity?0xff8a0:0xe9140);return parity;
}

/* 614F4: original RotTrans(MVMVA), MAC outputs shifted to16.16. */
GDB_CALL uint32 FUN_800614F4(sint32 x,sint32 y,sint32 z,uint32 output[3]) {
 SVECTOR v;VECTOR q;sint32 flags;FUN_80010170();
 v.vx=(sint16)(x>>16);v.vy=(sint16)(y>>16);v.vz=(sint16)(z>>16);v.pad=0;
 RotTrans(&v,&q,&flags);output[0]=(uint32)q.vx<<16;output[1]=(uint32)q.vy<<16;output[2]=(uint32)q.vz<<16;return output[2];
}
/* 1B9F4 valid attachment selectors. Original unspecified selectors read
 * uninitialized stack; reject them rather than invent attachment geometry. */
GDB_CALL uint32 FUN_8001B9F4(sint32 index,sint32 attachment,uint32 output[3]) {
 uint32 pose=0x800ba210+504u*(uint32)index,offset,point[3];int i;
 FUN_80010040();FUN_80011D20();FUN_8001033C(-ff_s16(0x8008d490));FUN_800101CC(-ff_s16(0x8008d492));
 if(attachment==2){FUN_800100C4(0,-786432,0);offset=24;}
 else if(attachment==3||attachment==11){FUN_800100C4(attachment==11?-2097152:0,-786432,0);offset=56;}
 else if(attachment>=4&&attachment<=9)offset=132+12u*(uint32)(attachment-4);
 else if(attachment==1)offset=204;else {ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);return 0;}
 FUN_800614F4((sint32)ff_u32(pose+offset),(sint32)ff_u32(pose+offset+4),(sint32)ff_u32(pose+offset+8),point);
 for(i=0;i<3;i++)output[i]=ff_u32(0x8008d4a4+4u*i)+point[i];return FUN_80010028();
}
