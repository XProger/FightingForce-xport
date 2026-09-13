#include "ff.h"
GDB_CALL uint32 FUN_8001A068(uint32 table,sint32 animation) {
 return ff_u32(table+4u*(uint32)animation);
}
static sint32 held_animation(sint32 animation,sint32 idle,sint32 walk,sint32 run,sint32 turn) {
 if(animation==69)return idle;if(animation==82)return walk;
 if(animation==87)return run;if(animation==92)return turn;return animation;
}
/* 237CC: left-hand remapping precedes right-hand remapping. Large right-hand
 * props suppress states87/35 and return35 without starting an animation. */
GDB_CALL sint32 FUN_800237CC(uint32 actor,uint32 table,sint32 animation) {
 sint32 slot;uint32 type;
 ff_w16(actor+64,(uint16)FUN_8002371C(actor));
 slot=*(sint8*)ff_ptr(actor+125,1);
 if(slot!=-1) {
  type=ff_u32(0x8009a3c8+36u*(uint32)slot);
  if(type==515)animation=held_animation(animation,73,83,88,95);
  else if(type==504)animation=held_animation(animation,78,86,90,97);
 }
 slot=*(sint8*)ff_ptr(actor+126,1);
 if(slot!=-1) {
  type=ff_u32(0x8009a3c8+36u*(uint32)slot);
  if(type==515)animation=held_animation(animation,73,83,88,95);
  else if(type-499u<5u||type==505)animation=held_animation(animation,72,82,87,94);
  else if(type==477||type==491||type==493||type-484u<2u||type-494u<2u) {
   if(animation==69&&FUN_8001A068(table,71))animation=71;
  } else if(type==509||type-506u<2u)animation=held_animation(animation,76,84,89,96);
  else if((sint32)type>=522) {
   animation=held_animation(animation,70,85,87,93);if(animation==87||animation==35)return 35;
  }
 }
 return FUN_80019F00(actor,table,animation);
}
/* 80011A04: null stream still reads word 4 in the branch delay slot.
 * Return is undefined on that path and is not consumed by animation startup. */
GDB_CALL void FUN_80011A04(uint32 actor) {
 uint32 stream=ff_u32(actor),header=ff_u32(stream+4),data;
 if(!stream)return;
 ff_w32(actor+28,header>>16);data=stream+(header&65535);
 ff_w32(actor+16,ff_u32(data));(void)ff_u32(data+4);ff_w32(actor+24,ff_u32(data+8));
}
/* 8001AE7C: conditional vertical scaling for animation 269. Return remains
 * four times the old negative value on unsupported stages, without a store. */
GDB_CALL sint32 FUN_8001AE7C(uint32 actor) {
 uint32 stage,v,result;
 if(ff_u32(actor+12)!=269||(sint32)ff_u32(actor+16)>=0)return 269;
 stage=ff_u32(0x80093d58);v=ff_u32(actor+16);
 if(!stage)result=v<<1;
 else if(stage==15)result=(v+(uint32)((sint32)v>>1))<<1;
 else {result=v<<2;if(stage-17u>=2&&stage!=23)return (sint32)result;}
 ff_w32(actor+16,result);return (sint32)result;
}

#include <string.h>
/* 80052758: shift the 16-byte action history, then search longest suffix first.
 * Equality against the old prediction uses the full action argument. */
GDB_CALL sint32 FUN_80052758(sint32 index,sint32 action) {
 uint32 base=0x80094640+((uint32)index<<4);int end,start,length;uint8 value;
 memmove(ff_ptr(base,15),ff_ptr(base+1,15),15);*(uint8*)ff_ptr(base+15,1)=(uint8)action;
 *(uint8*)ff_ptr(0x80094414+(uint32)index,1)=(*(uint8*)ff_ptr(0x80094374+(uint32)index,1)==(uint32)action);
 for(end=12,length=4;end<16;end++,length--)for(start=end-1;start>=0;start--) {
  if(!memcmp(ff_ptr(base+(uint32)start,(size_t)length),ff_ptr(base+(uint32)end,(size_t)length),(size_t)length)) {
   value=*(uint8*)ff_ptr(base+(uint32)(start+length),1);*(uint8*)ff_ptr(0x80094374+(uint32)index,1)=value;return value;
  }
 }
 return 0;
}
/* 80019F00: select animation stream and initialize actor state. The multiplied
 * actor-index calculation is retained, including its 32-bit truncation. */
GDB_CALL sint32 FUN_80019F00(uint32 actor,uint32 table,sint32 animation) {
 uint32 old=ff_u32(actor+12),index,flags;
 if(old==281&&ff_u32(actor+4)!=0xffffffff)return -1;
 if((old==205||old==203)&&(sint32)ff_u32(actor+44)<0&&old!=(uint32)animation)ff_w32(0x8008e4ec,1);
 ff_w32(actor+12,(uint32)animation);old=ff_u32(table+((uint32)animation<<2));
 ff_w32(actor+4,0);ff_w16(actor+82,0);ff_w32(actor+32,0);ff_w32(actor+28,0);ff_w16(actor+84,0);ff_w32(actor,old);
 index=(uint32)((sint32)((actor-0x800b4318u)*0xc10c9715u)>>2);
 flags=(uint16)ff_s16(0x800b39a0+((uint32)animation<<1));
 if(index<2&&((flags>>3)&15))FUN_80052758((sint32)index,(sint32)((flags>>3)&15));
 flags=(uint16)ff_s16(0x800b39a0+((uint32)animation<<1));
 if(!(flags&2)&&ff_s16(actor+52)!=17){FUN_80011A04(actor);return FUN_8001AE7C(actor);}
 return 17;
}

#include <stdlib.h>
static uint32 ff_anim_byte(uint32 a){return *(uint8*)ff_ptr(a,1);}
static uint32 ff_anim_word_bytes(uint32 a){return ff_anim_byte(a)|(ff_anim_byte(a+1)<<8)|(ff_anim_byte(a+2)<<16)|(ff_anim_byte(a+3)<<24);}
static uint32 ff_anim_bit(uint32 flags,uint32 *bit){uint32 b=*bit;*bit=b+1;return (ff_anim_byte(flags+(b>>3))>>(b&7))&1;}
/* 80011A50: first frame is absolute; later frames contain sparse signed deltas.
 * Null-stream return is undefined and ignored by actor update. */
GDB_CALL void FUN_80011A50(uint32 model,uint32 actor,uint32 pose) {
 uint32 stream=ff_u32(actor),frame=ff_u32(actor+4),count,header,data,flags,values,bit=0,i;
 if(!stream)return;count=ff_u32(model+20);
 if(!count||count>0x2aaaaaaau||frame==0x7fffffffu)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);count*=3;
 ff_w32(actor+4,ff_u32(stream)==frame+1?0xffffffff:frame+1);
 header=ff_u32(stream+4+(frame<<2));ff_w32(actor+28,header>>16);data=stream+4+(header&65535);
 for(i=0;i<3;i++)ff_w32(actor+16+4*i,ff_anim_word_bytes(data+4*i));
 flags=data+13;values=flags+ff_anim_byte(data+12);pose+=216;
 for(i=0;i<count;i++,pose+=2) {
  uint32 changed=ff_anim_bit(flags,&bit);
  if(!frame) {
   uint32 value=0;if(changed){value=ff_anim_byte(values)|(ff_anim_byte(values+1)<<8);values+=2;}ff_w16(pose,(uint16)value);
  } else if(changed) {
   uint32 wide=ff_anim_bit(flags,&bit);sint32 delta=(sint8)ff_anim_byte(values++);
   if(wide){delta=(uint8)delta+256*(sint32)(sint8)ff_anim_byte(values++);}
   ff_w16(pose,(uint16)(((uint32)(sint32)ff_s16(pose)+(uint32)(delta*2))&0xffe));
  }
 }
}

/* 8002371C: map animation variants to the state used by actor movement. */
GDB_CALL sint32 FUN_8002371C(uint32 actor) {
 uint32 n=ff_u32(actor+12);
 if(n==198||n==70||n-71u<2||n==73||n==76||n==78)n=69;
 if(n-96u<2||n-93u<2||n==95)n=92;
 if(n-83u<2||n-85u<2)n=82;
 if(n-88u<2||n==90)n=87;return (sint32)n;
}
/* 800164CC: four signed Newton iterations, including the unusual OR seed.
 * This is not a correctly rounded generic square root. */
GDB_CALL sint32 FUN_800164CC(sint32 value) {
 uint32 mask=0x80000000;int bit=31,i;sint32 guess,q;
 if(!value)return 0;
 while(!(mask&(uint32)value)){mask=(uint32)((sint32)mask>>1);bit--;}
 bit>>=1;guess=bit|(1<<bit);
 for(i=0;i<4;i++) {
  if(!guess||(guess==-1&&(uint32)value==0x80000000u))ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
  q=value/guess;guess=(sint32)((uint32)guess+(uint32)q)>>1;
 }
 return guess;
}
GDB_CALL sint32 FUN_80016478(sint32 x0,sint32 z0,sint32 x1,sint32 z1) {
 uint32 x=(uint32)x1-(uint32)x0,z=(uint32)z1-(uint32)z0;
 if((sint32)x<0)x=0u-x;if((sint32)z<0)z=0u-z;
 return FUN_800164CC((sint32)(x*x+z*z));
}
/* 8001627C: quadrant-table direction approximation and approximate distance.
 * All shifts/products retain the original 32-bit wrap before signed division. */
GDB_CALL sint32 FUN_8001627C(sint32 x0,sint32 z0,sint32 x1,sint32 z1) {
 uint32 x=(uint32)x1-(uint32)x0,z=(uint32)z1-(uint32)z0,q=0,n,d,v;int major;
 if((sint32)z<0){z=0u-z;q=1;}
 if((sint32)x<0){x=0u-x;q+=2;}
 major=(sint32)z<(sint32)x;
 ff_w32(0x800941a0,major?x+(uint32)((sint32)z>>1):z+(uint32)((sint32)x>>1));
 n=(major?z:x)<<13;d=major?x:(z?z:1);
 if(!d||(d==0xffffffffu&&n==0x80000000u))ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
 v=(uint32)((sint32)n/(sint32)d);if(major)v^=0x3fff;
 v=(v+ff_u32(0x8007aedc+4*q))^ff_u32(0x8007aecc+4*q);
 return ((sint32)v>>5)&0x7ff;
}
/* 800163F4: standalone distance approximation; unlike 8001627C, no global store. */
GDB_CALL sint32 FUN_800163F4(sint32 x0,sint32 z0,sint32 x1,sint32 z1) {
 uint32 x=(uint32)x1-(uint32)x0,z=(uint32)z1-(uint32)z0;
 if((sint32)x<0)x=0u-x;if((sint32)z<0)z=0u-z;
 return (sint32)((sint32)z<(sint32)x?x+(uint32)((sint32)z>>1):z+(uint32)((sint32)x>>1));
}
/* 8001A3F8: collision event gate; preserve actor in original A3 at dispatch. */
GDB_CALL sint32 FUN_8001A3F8(uint32 obstacle,uint32 actor) {
 uint32 flags=(uint16)ff_s16(actor+82),animation=ff_u32(actor+12),object,position[3],target;
 if(flags&0x4000)return 0x4000;
 if(animation-180u>=10&&animation-147u>=2&&animation!=154)return 154;
 object=ff_u32(obstacle+20);if(!object)return (sint32)0x80010000u;
 position[0]=ff_u32(object+4);position[1]=0x80010000u;position[2]=ff_u32(object+12);
 ff_w16(actor+82,(uint16)(flags|0x4000));
 target=ff_u32(0x80094040+4u*(uint32)(sint32)ff_s16(obstacle+12));
 return ff_collision_dispatch(target,obstacle,object,position,actor);
}
static sint32 collision_product(sint32 a,sint32 b){return (sint32)((uint32)a*(uint32)b);}
/* 80012B5C: bit-pair root with an unconditional final OR 1 in the branch
 * delay slot. Last remainder adjustment cannot affect the returned word. */
GDB_CALL sint32 FUN_80012B5C(uint32 value) {
 uint32 root=0,remainder=0,trial;int shift;
 for(shift=30;shift>0;shift-=2) {
  remainder=(remainder<<2)|((value>>shift)&3);trial=(root<<2)|1;root<<=1;
  if(remainder>=trial){root|=1;remainder-=trial;}
 }
 return (sint32)((root<<1)|1);
}
/* PsyQ rand adapter (GAME:8006D348, SDK SKIP): use the emulated seed rather
 * than the host CRT's different global state and implementation. */
GDB_CALL uint32 ff_rand_8006D348(void) {
 uint32 seed=ff_u32(0x80094e88)*0x41c64e6du+12345;
 ff_w32(0x80094e88,seed);return (seed>>16)&0x7fff;
}
static sint32 collision_circle(uint32 obstacle,uint32 x,uint32 z,sint32 *sn,sint32 *cs) {
 uint32 offset=(uint32)FUN_8001627C((sint32)x>>16,(sint32)z>>16,ff_s16(obstacle+4),ff_s16(obstacle+6))*2;
 *sn=ff_s16(0x800802d0+offset);*cs=ff_s16(0x800806d0+offset);
 return FUN_800163F4((sint32)(x+((uint32)*sn<<8))>>16,(sint32)(z-((uint32)*cs<<8))>>16,ff_s16(obstacle+4),ff_s16(obstacle+6));
}
/* 8001A4D0: ordered obstacle correction and second-pass rejection. The clear
 * flag persists across records, as do the cached rectangle candidate integers. */
GDB_CALL sint32 FUN_8001A4D0(uint32 actor,uint32 xa,uint32 za) {
 uint32 x,z,dx,dz,o; sint32 xi,zi,oldx,oldz,i,clear=1,changed=0;
 if(ff_s16(actor+80))return 1;
 x=ff_u32(xa);z=ff_u32(za);xi=(sint32)x>>16;zi=(sint32)z>>16;
 oldx=ff_s16(actor+42);oldz=ff_s16(actor+50);
 dx=x-(uint32)oldx;dz=z-(uint32)oldx; /* Original uses old X for both differences. */
 for(i=0,o=0x8009ec70;i<(sint32)ff_u32(0x8009479c);i++,o+=24) {
  sint32 type=ff_s16(o+10),sn,cs;
  if(ff_s16(o+12)<2)continue;
  if(type==1) {
   uint32 object=ff_u32(o+20),offset=((uint16)ff_s16(object+16)>>5)*2;
   sint32 cx=ff_s16(object+6),cz=ff_s16(object+14),lx,lz,angle,k;
   uint32 rx=(uint32)((sint32)x>>16)-(uint32)cx,rz=(uint32)((sint32)z>>16)-(uint32)cz;
   cs=ff_s16(0x800806d0+offset);sn=ff_s16(0x800802d0+offset);
   lx=(sint32)(rx*(uint32)cs+rz*(uint32)sn)>>15;lz=(sint32)(rz*(uint32)cs-rx*(uint32)sn)>>15;
   angle=(sint16)FUN_8001627C(lx,lz,0,0);
   for(k=0;k<2048;k+=256) {
    sint32 sx=ff_s16(0x800802d0+2*k)>>9,sz=ff_s16(0x800806d0+2*k)>>9;
    sint32 hx=ff_s16(o),hz=ff_s16(o+2),px=(sint32)((uint32)lx+(uint32)sx),pz=(sint32)((uint32)lz+(uint32)sz),corner;
    if(px<=-hx||px>=hx||pz<=-hz||pz>=hz)continue;
    clear=0;changed=1;corner=(sint16)FUN_8001627C(0,0,hx,-hz);
    if(angle<corner||2048-corner<angle)lz=hz-sz;
    else if(angle<1024-corner)lx=-hx-sx;
    else if(angle<1024+corner)lz=-hz-sz;
    else lx=hx-sx;
    offset=(uint32)((-ff_s16(object+16))>>4)&0xffe;
    cs=ff_s16(0x800806d0+offset);sn=ff_s16(0x800802d0+offset);
    x=((uint32)cx+(uint32)((sint32)((uint32)lx*(uint32)cs+(uint32)lz*(uint32)sn)>>15))<<16;
    z=((uint32)cz+(uint32)((sint32)((uint32)lz*(uint32)cs-(uint32)lx*(uint32)sn)>>15))<<16;
    break;
   }
  }else if(type) {
   if(collision_circle(o,x,z,&sn,&cs)<ff_s16(o)) {
    uint32 scale=(((uint32)(sint32)ff_s16(o)-ff_u32(0x800941a0))<<1)+256;
    clear=0;changed=1;x-=(uint32)sn*scale;z+=(uint32)cs*scale;
   }
  }else {
   sint32 left=ff_s16(o)-32,right=ff_s16(o+4)+32,bottom=ff_s16(o+2)-32,top=ff_s16(o+6)+32,side=0;
   if(xi<left||xi>right||zi<bottom||zi>top)continue;
   clear=0;changed=1;
   if(oldx<left)side=1;else if(oldx>right)side=2;
   if(oldz<bottom)side|=4;else if(oldz>top)side|=8;
   if(side==5)side=collision_product(bottom-oldz,(sint32)dx)<collision_product((sint32)dz,left-oldx)?4:1;
   else if(side==6)side=collision_product(bottom-oldz,(sint32)dx)<collision_product((sint32)dz,right-oldx)?2:4;
   else if(side==9)side=collision_product(top-oldz,(sint32)dx)<collision_product((sint32)dz,left-oldx)?1:8;
   else if(side==10)side=collision_product(top-oldz,(sint32)dx)<collision_product((sint32)dz,right-oldx)?8:2;
   if(side==1)xi=left-1;else if(side==2)xi=right+1;else if(side==4)zi=bottom-1;else if(side==8)zi=top+1;
   x=(uint32)xi<<16;z=(uint32)zi<<16;
  }
  if(!clear)FUN_8001A3F8(o,actor);
 }
 if(changed)for(i=0,o=0x8009ec70;i<(sint32)ff_u32(0x8009479c);i++,o+=24) {
  sint32 type=ff_s16(o+10),reject=0,sn,cs;
  if(ff_s16(o+12)<2||type==1)continue;
  if(!type) {
   xi=(sint32)x>>16;zi=(sint32)z>>16;
   reject=xi>=ff_s16(o)-32&&xi<=ff_s16(o+4)+32&&zi>=ff_s16(o+2)-32&&zi<=ff_s16(o+6)+32;
  }else if(type==2)reject=collision_circle(o,x,z,&sn,&cs)<=ff_s16(o);
  if(reject){ff_w32(xa,ff_u32(actor+40));ff_w32(za,ff_u32(actor+48));return 0;}
 }
 ff_w32(xa,x);ff_w32(za,z);return clear;
}
static int collision_excluded_animation(uint32 n) {
 return n-177u<13||n-10u<4||n-14u<5||n-19u<5||n-252u<2||n-155u<2||n-140u<2||n==142||n==148||n-145u<2||n==139||n==171||n==74||n==166||n==77||n==169||n==174||n==168||n==172||n==167||n==173||n==170||n==65||n-3u<2||n==205||n==203||n==281||n==269;
}
GDB_CALL void ff_collision_candidate(uint32 actor,uint32 *x,uint32 *z) {
 uint32 savedx=ff_u32(0x801fc080),savedz=ff_u32(0x801fc084);
 ff_w32(0x801fc080,*x);ff_w32(0x801fc084,*z);FUN_8001A4D0(actor,0x801fc080,0x801fc084);
 *x=ff_u32(0x801fc080);*z=ff_u32(0x801fc084);ff_w32(0x801fc080,savedx);ff_w32(0x801fc084,savedz);
}
/* 8001B2C4: ordered pair separation. The global call counter is not decremented
 * on return. Candidate actor stores occur only in the second-distance branch. */
GDB_CALL sint32 FUN_8001B2C4(sint32 index) {
 uint32 actor=0x800b4318+244u*(uint32)index,count,animation,other; sint32 gate,frame,react,i;
 gate=ff_s16(actor+80);if(gate)return gate;
 count=ff_u32(0x80093d54)+1;ff_w32(0x80093d54,count);if((sint32)count>=6)return -1;
 frame=(sint32)ff_u32(actor+4);animation=ff_u32(actor+12);
 react=frame!=-1&&((animation==180&&frame<20)||(animation==147&&frame<12)||animation==148);
 for(i=0,other=0x800b4318;i<8;i++,other+=244) {
  sint32 type,radius,r,distance;uint32 n,dx,dz,x,z,table; sint32 ix,iz;
  if(ff_s16(actor+88)==i||i==index)continue;
  type=ff_s16(other+52);if(type==-1||type==50||(sint8)*(uint8*)ff_ptr(other+119,1)>0)continue;
  n=ff_u32(other+12);if(collision_excluded_animation(n))continue;
  radius=ff_s16(0x80094120+2u*(uint32)type);r=ff_s16(0x80094120+2u*(uint32)(sint32)ff_s16(actor+52));if(radius<r)radius=r;
  dx=ff_u32(other+40)-ff_u32(actor+40);dz=ff_u32(other+48)-ff_u32(actor+48);ix=(sint32)dx>>16;iz=(sint32)dz>>16;
  distance=FUN_80012B5C((uint32)ix*(uint32)ix+(uint32)iz*(uint32)iz);if(!distance)distance=1;
  if(distance>=radius)continue;
  /* The real bit-pair helper returns 1..65535, so signed division cannot trap. */
  if(ff_s16(other+80)){x=ff_u32(other+40);z=ff_u32(other+48);}
  else {x=ff_u32(actor+40)+(uint32)radius*(uint32)((sint32)dx/distance);z=ff_u32(actor+48)+(uint32)radius*(uint32)((sint32)dz/distance);ff_collision_candidate(other,&x,&z);}
  type=ff_s16(other+52);
  if(type!=17&&type!=51) {
   table=ff_u32(0x800b3be8+4u*(uint32)type);
   if(n-50u>=2&&n-52u>=2&&ff_u32(table+588)&&i>=2&&ff_s16(other+88)==-1&&react&&ff_u32(other+12)-133u>=4)
    FUN_80019F00(other,table,(sint32)(ff_rand_8006D348()&3)+133);
   dx=ff_u32(actor+40)-x;dz=ff_u32(actor+48)-z;ix=(sint32)dx>>16;iz=(sint32)dz>>16;
   distance=FUN_80012B5C((uint32)ix*(uint32)ix+(uint32)iz*(uint32)iz);if(!distance)distance=1;
   if(distance<radius) {
    ff_w32(other+40,x);ff_w32(other+48,z);
    x+=(uint32)radius*(uint32)((sint32)dx/distance);z+=(uint32)radius*(uint32)((sint32)dz/distance);
    ff_collision_candidate(actor,&x,&z);ff_w32(actor+40,x);ff_w32(actor+48,z);
   }
  }
  FUN_8001B2C4(i);
 }
 return 0;
}
GDB_CALL sint32 FUN_80023C80(sint32 index) {
 uint32 actor=0x800b4318+244u*(uint32)index,angle; sint32 frame=(sint32)ff_u32(actor+4),length=ff_s16(actor+96),phase,distance;
 if(frame<0||frame>=length)return 0;
 phase=1040*frame/length;if(phase>1024)phase=1024;
 distance=collision_product(ff_s16(actor+94),ff_s16(0x800806d0+2u*(uint32)phase))>>15;
 angle=2u*(uint32)(sint32)ff_s16(actor+56);
 ff_w32(actor+40,((uint32)(sint32)ff_s16(actor+90)<<16)-(uint32)ff_s16(0x800802d0+angle)*(uint32)distance);
 ff_w32(actor+48,((uint32)(sint32)ff_s16(actor+92)<<16)+(uint32)ff_s16(0x800806d0+angle)*(uint32)distance);return 1;
}
/* 8001AEF4: velocity comes from the old pose, even though decoding advances
 * the stream and replaces velocity fields before movement is applied. */
GDB_CALL sint32 FUN_8001AEF4(sint32 index) {
 uint32 actor=0x800b4318+244u*(uint32)index,stage=ff_u32(0x80093d58),side,forward,state,x,z,angle,other;
 sint32 previous_distance,limit,sn,cs,distance;uint32 dx,dz;
 if(stage==4||stage==18) {
  limit=stage==4?47185920:78643200;
  if((sint32)ff_u32(actor+40)<-limit)ff_w32(actor+40,(uint32)-limit);
  if((sint32)ff_u32(actor+40)>limit)ff_w32(actor+40,(uint32)limit);
 }
 previous_distance=FUN_80016478(ff_s16(0x800b4342),ff_s16(0x800b434a),ff_s16(0x800b4436),ff_s16(0x800b443e));
 side=ff_u32(actor+20);forward=ff_u32(actor+24);
 FUN_80011A50(ff_u32(0x80081720+4u*(uint32)(sint32)ff_s16(actor+52)),actor,0x800ba210+504u*(uint32)index);
 state=(uint32)FUN_8002371C(actor);
 if((sint8)*(uint8*)ff_ptr(actor+120,1)!=-1&&state!=139&&state!=172&&state!=77)return 77;
 if((uint16)ff_s16(0x800b39a0+2*state)&1) {
  ff_w32(actor+32,ff_u32(actor+32)+forward);FUN_8001A4D0(actor,actor+40,actor+48);
  ff_w32(0x80093d54,0);return FUN_8001B2C4(index);
 }
 angle=2u*(uint32)(sint32)ff_s16(actor+56);sn=ff_s16(0x800802d0+angle);cs=ff_s16(0x800806d0+angle);
 x=ff_u32(actor+40)+(uint32)sn*(uint32)((sint32)forward>>15)+(uint32)cs*(uint32)((sint32)side>>15);
 z=ff_u32(actor+48)-(uint32)cs*(uint32)((sint32)forward>>15)+(uint32)sn*(uint32)((sint32)side>>15);
 FUN_8001AE7C(actor);
 if(ff_u32(actor+12)==65&&FUN_80023C80(index)){x=ff_u32(actor+40);z=ff_u32(actor+48);}
 if(!ff_s16(actor+80)&&ff_s16(actor+52)!=51&&ff_u32(actor+12)!=130)ff_collision_candidate(actor,&x,&z);
 if((sint32)ff_u32(0x800940b0)>=2&&index<2) {
  other=0x800b4318+244u*(1u-(uint32)index);
  distance=FUN_80016478((sint32)x>>16,(sint32)z>>16,ff_s16(other+42),ff_s16(other+50));
  dx=x-ff_u32(other+40);dz=z-ff_u32(other+48);
  if(distance<previous_distance||ff_s16(other+66)<=0){dx=0;dz=0;}
  if(dx+0x3ffffffu>0x7fffffeu||dz+0x3ffffffu>0x7fffffeu)goto movement_done;
 }
 ff_w32(actor+40,x);ff_w32(actor+48,z);
movement_done:
 if((sint8)*(uint8*)ff_ptr(actor+120,1)!=-1)return -1;
 ff_w32(0x80093d54,0);return FUN_8001B2C4(index);
}
GDB_CALL sint32 FUN_80050CC8(void) {
 sint32 i;uint32 actor;
 for(i=0,actor=0x800b4318;i<4;i++,actor+=244) {
  if(!ff_u32(actor))ff_w32(actor+4,0xffffffffu);
  if(ff_u32(actor+4)==0xffffffffu)FUN_80019F00(actor,ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52)),69);
  if(ff_u32(actor+4)!=0xffffffffu)FUN_8001AEF4(i);
 }
 return 0;
}
