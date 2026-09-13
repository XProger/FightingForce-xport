#include "ff.h"
GDB_CALL sint32 FUN_80061800(uint32 text,uint32 buffer) {
 FF_FUNCTION_MARKER(0x80061800u,"GAME.EXE");
 uint32 p=buffer,word=buffer,line_width=0,word_width=0,i;uint8 ch;
 for(i=0;i<40;i++)ff_w8(0x800940d8+i,0);
 /* strcpy boundary: preserve the original forward byte copy, including NUL. */
 i=0;do{ch=*(uint8*)ff_ptr(text+i,1);ff_w8(buffer+i,ch);++i;}while(ch);
 ff_w32(0x80094330,0);ff_w32(0x800940d8,buffer);
 for(;;){
  sint32 letter=*(sint8*)ff_ptr(p,1);
  if(letter==32||!letter){
   if((sint32)(line_width+word_width)>=273){
    line_width=word_width;word_width=0;i=ff_u32(0x80094330)+1;
    ff_w32(0x80094330,i);
    if(ff_u32(0x800940d4+4*i)!=word)ff_w32(0x800940d8+4*i,word);
    ff_w8(word-1,0);
    if(!*(sint8*)ff_ptr(p,1))break;
    /* The overflowing separator goes through the glyph path as in 618E8. */
   }else{
    line_width+=10+word_width;word_width=0;
    if(!*(sint8*)ff_ptr(p,1))break;
    word=++p;continue;
   }
  }
  letter=*(sint8*)ff_ptr(p++,1);
  word_width+=(uint32)(sint32)*(sint8*)ff_ptr(0x800800d0+(uint32)FUN_80018670(letter),1);
 }
 ff_w32(0x80094330,0);
 for(;;){
  i=ff_u32(0x80094330);
  if(!ff_u32(0x800940d8+4*i))return (sint32)(i+1);
  ff_w32(0x80094330,++i);if((sint32)i>=10)return 0;
 }
}
GDB_CALL sint32 FUN_8006159C(uint32 text,sint32 x,sint32 y,sint32 phase) {
 FF_FUNCTION_MARKER(0x8006159cu,"GAME.EXE");
 sint32 position[4],letter,sprite,offset,step,width;position[1]=y;position[2]=128;
 while(*(uint8*)ff_ptr(text,1)){letter=*(sint8*)ff_ptr(text++,1);
  if(letter==32)x=(sint32)((uint32)x+6u);
  else{letter-=65;sprite=letter+ff_s16(0x8009acc8+2u*(uint32)(sint32)ff_s16(0x800af4a8));
   if(phase<0)offset=400;else{step=phase>16?16:phase;offset=(32767-ff_s16(0x800802d0+((uint32)step<<6)))>>5;}
   position[0]=(sint32)((uint32)x+(uint32)offset);position[3]=3*offset+64;if(offset!=400)FUN_80012594(position,sprite);
   width=*(sint8*)ff_ptr(0x800800d0+(uint32)letter,1);x=(sint32)((uint32)x+(uint32)(width+(width>>3)));
  }phase=(sint32)((uint32)phase-3u);
 }return 0;
}
GDB_CALL sint32 FUN_800616DC(uint32 text,sint32 x,sint32 y,sint32 phase) {
 FF_FUNCTION_MARKER(0x800616dcu,"GAME.EXE");
 sint32 position[4],letter,sprite,offset,step;position[1]=y;position[2]=128;
 while(*(uint8*)ff_ptr(text,1)){letter=(sint32)*(sint8*)ff_ptr(text++,1)-65;
  sprite=letter+ff_s16(0x8009ad40+2u*(uint32)(sint32)ff_s16(0x800af4a8));
  if(phase<0)offset=400;else{step=phase>16?16:phase;offset=(32767-ff_s16(0x800802d0+((uint32)step<<6)))>>5;}
  position[0]=(sint32)((uint32)x+(uint32)offset);position[3]=3*offset+64;if(offset!=400)FUN_80012594(position,sprite);
  x=(sint32)((uint32)x+(uint32)(sint32)ff_s16(0x8008013c+2u*(uint32)letter));phase=(sint32)((uint32)phase-4u);
 }return 0;
}
GDB_CALL sint32 FUN_800619E8(sint32 y) {
 FF_FUNCTION_MARKER(0x800619e8u,"GAME.EXE");
 sint32 count=(sint32)ff_u32(0x80094330);uint32 i=0;if(count<=0)return count;
 if(y==-1){do{FUN_80061ADC(ff_u32(0x800940d8+4u*i),(sint32)(16u*i+120u-8u*(uint32)count));
  ++i;count=(sint32)ff_u32(0x80094330);}while((sint32)i<count);return (sint32)(4u*i);}
 do{FUN_80061ADC(ff_u32(0x800940d8+4u*i),(sint32)((uint32)y+10u*i));++i;}while((sint32)i<(sint32)ff_u32(0x80094330));return 0;
}
GDB_CALL sint32 FUN_80061CB8(uint32 text,sint32 x,sint32 y) {
 FF_FUNCTION_MARKER(0x80061cb8u,"GAME.EXE");
 uint32 width=(uint32)FUN_8005EFB8(text);return FUN_80061B7C(text,(sint32)((uint32)x+10u-width),y);
}
GDB_CALL uint32 FUN_8005EEDC(void) {
 FF_FUNCTION_MARKER(0x8005eedcu,"GAME.EXE");
 sint32 level;ff_w16(0x80093e14,0);ff_w16(0x80093e12,0);ff_w16(0x80093e10,0);
 for(level=0;level<=256;level+=32)FUN_8005EE58(level);return 0;
}
GDB_CALL uint32 FUN_8005EF30(void) {
 FF_FUNCTION_MARKER(0x8005ef30u,"GAME.EXE");
 sint32 level;uint32 result;
 for(level=256;level>0;level-=32)result=FUN_8005EE58(level);
 ff_w16(0x80093e14,0);ff_w16(0x80093e12,0);ff_w16(0x80093e10,0);return result;
}
/* Original stack-local XYZ crosses the host ABI as a native pointer,
 * just like the existing 12594 sprite path; no emulated RAM scratch alias. */
GDB_CALL uint32 FUN_8005EF80(sint32 x,sint32 y,uint32 texture) {
 FF_FUNCTION_MARKER(0x8005ef80u,"GAME.EXE");
 sint32 position[3]={x,y,256};return FUN_800124A0(position,texture);
}
/* 6494C: sky gradient and three horizontally wrapping sprites. MIPS keeps
 * coordinates in 32 bits until SH; the stage-4 horizon offset is +132. */
GDB_CALL uint32 FUN_8006494C(void) {
 uint32 horizon=ff_u32(0x8008d4d0),bottom,a,color,x,v,i;uint8 *p;
 if(ff_u32(0x80093d58)==4)horizon+=132u;
 bottom=horizon-128u;
 if((sint32)bottom>0) {
  color=ff_u32(0x8008d4c8);a=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,a+36);
  p=ff_ptr(a,36);setPolyG4(p);
  for(i=0;i<4;i++) {
   p[4+8*i]=i<2?0:(uint8)(color<<3);
   p[5+8*i]=i<2?0:(uint8)(((sint32)color>>2)&~7);
   p[6+8*i]=i<2?0:(uint8)(((sint32)color>>7)&~7);
   ff_w16(a+8+8*i,(uint16)((i&1)?320:0));
   ff_w16(a+10+8*i,(uint16)(i<2?horizon-288u:bottom));
  }
  FUN_80011E40(ff_u32(0x8008d4b4),a);
 }
 x=(uint32)((sint32)ff_s16(0x8008d490)>>5)+160u;v=(x&256u)>>1;x&=255u;
 for(i=0;i<3;i++) {
  a=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,a+20);p=ff_ptr(a,20);
  setSprt(p);ff_w16(a+14,0x7c00);setShadeTex(p,1);
  ff_w16(a+8,(uint16)(x+256u*i-256u));ff_w16(a+10,(uint16)bottom);
  p[12]=0;p[13]=(uint8)(i==1?128u-v:v);
  ff_w16(a+16,256);ff_w16(a+18,128);FUN_80011E40(ff_u32(0x8008d4b4),a);
 }
 a=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,a+12);p=ff_ptr(a,12);setDrawMode(p,1,0,142,NULL);
 if((uint32)(*(uint8*)ff_ptr(0x800872ec,1)-1)<2)ff_w32(a+4,0xe100108e);
 return FUN_80011E40(ff_u32(0x8008d4b4),a);
}
/* 5E9E8: five signed decimal quotients, blank leading zeros, literal units0. */
sint32 FUN_8005E9E8(sint32 player) {
 static const sint32 divisors[5]={100000,10000,1000,100,10};
 uint32 i,address,started=0; sint32 value,q;uint8 digit;
 if((uint32)player>=2)return 0;
 address=0x80094738+8u*(uint32)player;value=(sint32)ff_u32(0x80094030+4u*(uint32)player);
 for(i=0;i<5;i++) {
  q=value/divisors[i];digit=(uint8)((uint32)q+48);
  if(!started&&digit==48)digit=32;else started=1;
  *(uint8*)ff_ptr(address+i,1)=digit;
  value=(sint32)((uint32)value-(uint32)q*(uint32)divisors[i]);
 }
 *(uint8*)ff_ptr(address+5,1)=48;*(uint8*)ff_ptr(address+6,1)=0;return 48;
}
/* 8006686C: menu backdrop packets. Original signed shifts and low-halfword
 * coordinate stores; AA PsyQ initializers reuse existing normalized twins. */
GDB_CALL uint32 FUN_8006686C(void) {
 sint32 x=(sint32)((uint32)((sint32)ff_u32(0x8008d4a4)>>8)-(((uint32)(sint32)ff_s16(0x8008d490)<<10)+0x200000u))>>16;
 uint32 horizon=ff_u32(0x8008d4d0)-256u,a,color;uint8 *p;
 if((sint32)horizon>0) {
  color=ff_u32(0x8008d4c8);a=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,a+24);p=ff_ptr(a,24);setPolyF4(p);
  p[4]=(uint8)(color<<3);p[5]=(uint8)(((sint32)color>>2)&~7);p[6]=(uint8)(((sint32)color>>7)&~7);
  ff_w16(a+8,0);ff_w16(a+10,0);ff_w16(a+12,320);ff_w16(a+14,0);ff_w16(a+16,0);ff_w16(a+18,(uint16)horizon);ff_w16(a+20,320);ff_w16(a+22,(uint16)horizon);
  FUN_80011E40(ff_u32(0x8008d4b4),a);
 }
 a=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,a+20);p=ff_ptr(a,20);setSprt(p);ff_w16(a+14,0x7c00);setShadeTex(p,1);
 ff_w16(a+8,(uint16)(0u-(uint32)x));ff_w16(a+10,(uint16)horizon);p[12]=p[13]=0;ff_w16(a+16,256);ff_w16(a+18,256);FUN_80011E40(ff_u32(0x8008d4b4),a);
 a=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,a+12);p=ff_ptr(a,12);setDrawMode(p,1,0,142,NULL);
 /* Original SDK supports alternate GPU revisions with the DFE bit at 12. */
 if((uint32)(*(uint8*)ff_ptr(0x800872ec,1)-1)<2)ff_w32(a+4,0xe100108e);
 return FUN_80011E40(ff_u32(0x8008d4b4),a);
}
/* 80062AB4: volume bar. SDK normalized twins reuse AA polygon initializers.
 * Allocation padding is deliberately preserved, including the unused color bytes. */
GDB_CALL uint32 FUN_80062AB4(sint32 x,sint32 y,sint32 value) {
 sint32 width=value>>2;uint32 a,ux=(uint32)x,uy=(uint32)y;uint8 *p;int i;
 if(width>0) {
  a=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,a+36);p=ff_ptr(a,36);setPolyG4(p);
  for(i=0;i<4;i++){p[4+8*i]=p[5+8*i]=i<2?0:255;p[6+8*i]=i<2?255:0;
   ff_w16(a+8+8u*i,(uint16)(ux+1+(i<2?0:(uint32)width)));ff_w16(a+10+8u*i,(uint16)(uy+(i&1?5:1)));}
  FUN_80011E40(ff_u32(0x8008d4b4)+8188,a);
 }
 a=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,a+24);p=ff_ptr(a,24);setPolyF4(p);p[4]=p[5]=p[6]=0;
 for(i=0;i<4;i++){ff_w16(a+8+4u*i,(uint16)(ux+(i<2?66:0)));ff_w16(a+10+4u*i,(uint16)(uy+(i&1?6:0)));}
 return FUN_80011E40(ff_u32(0x8008d4b4)+8188,a);
}
#include <stdlib.h>
/* Original GAME.EXE:80018670..8001886C. IDA draft audited against all MIPS branches.
 * The unsigned lowercase-range comparison precedes signed special-character cases. */
GDB_CALL sint32 FUN_80018670(sint32 value) {
 if((uint32)value-97u<26u)value=(sint32)((uint32)value-32u);
 switch(value) {
 case 91:return 26;case 93:return 27;case 123:return 28;case 125:return 29;
 case 63:return 30;case 46:return 31;case 40:return 32;case 41:return 33;
 case -93:return 34;case 36:return 35;case 37:return 36;case 94:return 37;
 case -60:return 48;case -42:return 49;case -36:return 50;
 case 45:return 51;case 44:return 52;case 39:return 53;case 33:return 54;
 case 95:return 55;case 60:return 56;case 35:return 57;case 61:return 58;
 default:return (sint32)((uint32)value-(value>=65?65u:10u));
 }
}
/* Original GAME.EXE:8005EFB8..8005F050. Widths are LB (signed), not LBU.
 * Text bytes are sign-extended before conversion. Return includes the +10 margin. */
GDB_CALL sint32 FUN_8005EFB8(uint32 text) {
 uint32 width=0;
 while(*(uint8*)ff_ptr(text,1)) {
  sint32 ch=*(sint8*)ff_ptr(text++,1);
  if(ch==32)width+=6;
  else { sint32 index=FUN_80018670(ch);width+=(sint32)*(sint8*)ff_ptr(0x800800d0u+(uint32)index,1); }
 }
 return (sint32)(width+10u);
}

/* Original GAME.EXE:80012390..800124A0. The two packets are inserted LIFO:
 * draw-mode packet first in the final chain, then the textured sprite. */
GDB_CALL uint32 FUN_80012390(const sint32 *position,sint32 sprite) {
 uint32 packet=ff_u32(0x8008d4b0),descriptor=0x800a8768u+(uint32)sprite*12u;
 uint32 uv_clut=ff_u32(descriptor),mode=ff_u32(descriptor+4),end=ff_u32(descriptor+8);
 uint32 semitrans=uv_clut&1u,uv=uv_clut-semitrans;
 uint32 delta=((end>>16)-uv)&65535u;
 uint32 table=ff_u32(0x8008d4d8),ot=ff_u32(0x8008d4b4);
 uint32 bucket=ot+(sint32)ff_s16(table+(uint32)(position[2]>>2)*2u);
 ff_w32(packet+12,uv);ff_w32(packet+4,(semitrans<<25)|0x65000000u);
 ff_w16(packet+8,(uint16)position[0]);ff_w16(packet+10,(uint16)position[1]);
 ff_w16(packet+16,(uint16)((delta&255u)+1));ff_w16(packet+18,(uint16)((delta>>8)+1));
 ff_w32(packet,ff_u32(bucket)|0x04000000u);ff_w32(bucket,packet&0xffffffu);
 packet+=20;
 ff_w32(packet+4,(mode>>16)+0xe1000000u+1024u);ff_w32(packet+8,0);
 ff_w32(packet,ff_u32(bucket)|0x02000000u);ff_w32(bucket,packet&0xffffffu);
 ff_w32(0x8008d4b0,packet+12);
 return 0x8008d4b0;
}

/* 12594: perspective-scaled FT4. MIPS DIV has no compiler break guards. */
static sint32 ff_sprite_div(sint32 n,sint32 d) {
 if(!d)return n<0?1:-1;if(n==(sint32)0x80000000u&&d==-1)return n;return n/d;
}
GDB_CALL uint32 FUN_80012594(const sint32 *position,sint32 sprite) {
 uint32 p=ff_u32(0x8008d4b0),a=0x800a8768u+12u*(uint32)sprite;
 uint32 uv=ff_u32(a),mode=ff_u32(a+4),end=ff_u32(a+8),semi=uv&1,delta,w,h,bucket;
 sint32 dx,dy;uint32 x=(uint32)(sint32)(sint16)position[0],y=(uint32)(sint32)(sint16)position[1];
 uv-=semi;delta=(((end>>16)-uv)&65535)+257;w=delta&255;h=delta>>8;
 ff_w32(p+12,uv);ff_w32(p+20,mode);ff_w16(p+28,(uint16)end);ff_w16(p+36,(uint16)(end>>16));
 ff_w32(p+4,(semi<<25)|ff_u32(0x8008d4bc));
 dx=ff_sprite_div((sint32)(w*(uint32)position[3]),position[2]);
 dy=w==h?dx:ff_sprite_div((sint32)(h*(uint32)position[3]),position[2]);
 ff_w16(p+8,(uint16)(x-(uint32)dx));ff_w16(p+24,(uint16)(x-(uint32)dx));
 ff_w16(p+10,(uint16)(y-(uint32)dy));ff_w16(p+18,(uint16)(y-(uint32)dy));
 ff_w16(p+16,(uint16)(x+(uint32)dx-1));ff_w16(p+32,(uint16)(x+(uint32)dx-1));
 ff_w16(p+26,(uint16)(y+(uint32)dy-1));ff_w16(p+34,(uint16)(y+(uint32)dy-1));
 bucket=ff_u32(0x8008d4b4)+(uint32)(sint32)ff_s16(ff_u32(0x8008d4d8)+2u*(uint32)(position[2]>>2));
 ff_w32(p,ff_u32(bucket)|0x09000000u);ff_w32(bucket,p&0xffffff);
 ff_w32(0x8008d4bc,0x2d000000);ff_w32(0x8008d4b0,p+40);return 0x8008d4b0;
}
/* Original GAME.EXE:80061B7C..80061C68. Signed glyph widths and font table LH. */
GDB_CALL sint32 FUN_80061B7C(uint32 text,sint32 x,sint32 y) {
 sint32 position[3]={x,y,0};
 while(*(uint8*)ff_ptr(text,1)) {
  sint32 ch=*(sint8*)ff_ptr(text++,1);
  if(ch==32)position[0]=(sint32)((uint32)position[0]+6u);
  else {
   sint32 glyph=FUN_80018670(ch),font=ff_s16(0x800af4a8);
   FUN_80012390(position,(sint32)((uint32)glyph+(sint32)ff_s16(0x8009acc8u+(uint32)font*2u)));
   position[0]=(sint32)((uint32)position[0]+(sint32)*(sint8*)ff_ptr(0x800800d0u+(uint32)glyph,1));
  }
 }
 return (sint32)((uint32)position[0]+10u);
}
/* Original GAME.EXE:80061ADC..80061B28. SRA width/2, not C signed division. */
GDB_CALL sint32 FUN_80061ADC(uint32 text,sint32 y) {
 sint32 width=FUN_8005EFB8(text);
 return FUN_80061B7C(text,(sint32)(160u-(uint32)(width>>1)),y);
}
/* GAME.EXE:80061C68. Logo font bank, fixed depth 16. */
GDB_CALL uint32 FUN_80061C68(sint32 x,sint32 y) {
 sint32 position[3]={x,y,16};
 return FUN_80012390(position,ff_s16(0x8009acc8u+(uint32)ff_s16(0x800af4b4)*2u));
}

/* GAME.EXE:80012A00. Descriptor end coordinates are inclusive sprite limits;
 * the polygon path deliberately does not add the sprite path's +1. */
GDB_CALL void FUN_80012A00(uint32 packet,sint32 sprite,sint32 x,sint32 y) {
 uint32 descriptor=0x800a8768u+(uint32)sprite*12u;
 uint32 uv=ff_u32(descriptor),mode=ff_u32(descriptor+4),end=ff_u32(descriptor+8);
 uint32 delta=((end>>16)-uv)&65535u;
 uint16 right=(uint16)((uint32)x+(delta&255)),bottom=(uint16)((uint32)y+(delta>>8));
 ff_w32(packet+12,uv);ff_w32(packet+24,mode);ff_w16(packet+36,(uint16)end);ff_w16(packet+48,(uint16)(end>>16));
 ff_w16(packet+8,(uint16)x);ff_w16(packet+10,(uint16)y);
 ff_w16(packet+20,right);ff_w16(packet+22,(uint16)y);
 ff_w16(packet+32,(uint16)x);ff_w16(packet+34,bottom);
 ff_w16(packet+44,right);ff_w16(packet+46,bottom);
}
/* GAME.EXE:80011E40. Preserve the original full pointer store to the OT;
 * GPU traversal masks to 24 bits. A host pointer must never enter this chain. */
GDB_CALL uint32 FUN_80011E40(uint32 bucket,uint32 packet) {
 uint32 previous=ff_u32(bucket),tag=previous|(ff_u32(packet)&0xff000000u);
 ff_w32(bucket,packet);ff_w32(packet,tag);return previous;
}
/* GAME.EXE:80062C0C. SB then sign extension of color deltas is intentional.
 * SDK 8006E7D4 is the existing PsyQ setPolyGT4 macro (12, 0x3c);
 * its normalized twins differ in their packet length and command constants. */
GDB_CALL sint32 FUN_80062C0C(uint32 text,sint32 x,sint32 y,uint32 colors) {
 sint32 width=FUN_8005EFB8(text);uint32 advance=0;uint8 *c=(uint8*)ff_ptr(colors,12);
 sint8 delta[6];uint8 previous[6];int i;
 if(!width)return 0;
 for(i=0;i<6;i++){int k=i<3?i:i+3;delta[i]=(sint8)(c[k+3]-c[k]);previous[i]=c[k];}
 while(*(uint8*)ff_ptr(text,1)) {
  sint32 ch=*(sint8*)ff_ptr(text++,1);
  if(ch==32)advance+=6;
  else {
   uint8 next[6];sint32 glyph=FUN_80018670(ch);uint32 packet=ff_u32(0x8008d4b0);uint8 *p=(uint8*)ff_ptr(packet,52);
   ff_w32(0x8008d4b0,packet+52);setPolyGT4(p);
   for(i=0;i<6;i++) {
    sint32 product=(sint32)((uint32)(sint32)delta[i]*advance);int k=i<3?i:i+3;
    /* Original BREAK 6 on signed division overflow. */
    if(width==-1 && (uint32)product==0x80000000u)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
    next[i]=(uint8)((uint32)c[k]+(uint32)(product/width));
   }
   for(i=0;i<3;i++) {
    p[4+i]=(uint8)(previous[i]*8u-128u);p[16+i]=(uint8)(next[i]*8u-128u);
    p[28+i]=(uint8)(next[i+3]*8u-128u);p[40+i]=(uint8)(previous[i+3]*8u-128u);
   }
   FUN_80012A00(packet,(sint32)((uint32)glyph+(sint32)ff_s16(0x8009acc8u+(uint32)ff_s16(0x800af4a8)*2u)),(sint32)((uint32)x+advance),y);
   advance+=(sint32)*(sint8*)ff_ptr(0x800800d0u+(uint32)glyph,1);
   FUN_80011E40(ff_u32(0x8008d4b4)+8188,packet);
   for(i=0;i<6;i++)previous[i]=next[i];
  }
 }
 return (sint32)(advance+(uint32)x+170u);
}

/* 800128F4: variable-height FT4 from a 12-byte texture descriptor. */
GDB_CALL uint32 FUN_800128F4(uint32 position,sint32 sprite) {
 uint32 p=ff_u32(0x8008d4b0),d=0x800a8768u+(uint32)sprite*12u;
 uint32 uv=ff_u32(d),mode=ff_u32(d+4),end=ff_u32(d+8),bit=uv&1u,depth,bucket;
 sint32 x=ff_s16(position),y=ff_s16(position+4);sint64 diff,bottom;uint32 right;
 uv-=bit;diff=(sint64)(end>>16)-(sint32)uv;if(diff<-2147483647LL-1||diff>2147483647LL)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
 ff_w32(p+12,uv);ff_w32(p+20,mode);ff_w16(p+28,(uint16)end);ff_w16(p+36,(uint16)(end>>16));
 ff_w32(p+4,ff_u32(0x8008d4bc)|(bit<<25));depth=ff_u32(position+8);
 ff_w16(p+8,(uint16)x);ff_w16(p+24,(uint16)x);ff_w16(p+10,(uint16)y);ff_w16(p+18,(uint16)y);
 right=(uint32)x+((uint32)diff&255)+1;bottom=(sint64)(sint32)ff_u32(position+12)+y;
 if(bottom<-2147483647LL-1||bottom>2147483647LL)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
 ff_w16(p+16,(uint16)right);ff_w16(p+32,(uint16)right);ff_w16(p+26,(uint16)bottom);ff_w16(p+34,(uint16)bottom);
 bucket=ff_u32(0x8008d4b4)+(sint32)ff_s16(ff_u32(0x8008d4d8)+((uint32)((sint32)depth>>2)<<1));
 ff_w32(p,ff_u32(bucket)|0x09000000);ff_w32(bucket,p&0xffffff);ff_w32(0x8008d4bc,0x2d000000);ff_w32(0x8008d4b0,p+40);return 0x8008d4b0;
}

/* 18DCC scans CRLF text in place; NUL does not advance the global cursor. */
uint32 FUN_80018DCC(void) {
 uint32 p=ff_u32(0x80093cfc),next=p+1;
 while(*(uint8*)ff_ptr(p,1)) {
  ff_w32(0x80093cfc,next);
  if(*(uint8*)ff_ptr(p,1)==13) {
   ff_w8(p,0);p=ff_u32(0x80093cfc);next=p+1;ff_w32(0x80093cfc,next);ff_w8(p,0);return next;
  }
  p=ff_u32(0x80093cfc);next=p+1;
 }
 return next;
}
uint32 FUN_80018E50(void) {
 static const uint32 head[]={0x8009243c,0x80092440,0x80092424,0x80092428,0x8009242c,0x80092430,0x80092434,0x80092438,0x80092444,0x80092448,0x8009244c,0x80092464,0x80092468,0x8009246c,0x80092470,0x80092450,0x80092454,0x80092458,0x8009245c,0x80092460,0x80092474,0x80092478,0x8009247c,0x80092480,0x80092484,0x80092488,0x8009248c,0x80092490,0x80092494,0x80092498,0x8009249c,0x800924a0,0x800924a4,0x800924a8,0x800924ac,0x800924b0,0x800924b4,0x800924b8,0x800924bc,0x800924c0,0x800924c4,0x800924c8,0x800924cc,0x800924d0,0x800924d4,0x800924d8,0x800924dc,0x800924e0,0x800924e4,0x800924e8,0x800924ec,0x800924f0,0x800924f4,0x800924f8,0x800924fc};
 static const uint32 tail[]={0x80092500,0,0x80092504,0,0x80092508,0,0x8009250c,0,0x80092510,0,0x80092514,0,0x80094600,0x80094610,0,0x80094614,0,0x80094618,0,0x80094604,0x80094618,0,0x80094608,0x80094620,0x80092528,0,0x80092518,0,0x8009251c,0,0x80092520,0,0x80092524,0,0x8009252c,0,0x80092530,0,0x80092534,0,0x80092538,0,0x8009253c,0,0x80094458,0,0x8009445c,0,0x80094460,0,0x80094464,0,0x80094468,0,0x8009446c,0};
 uint32 i;
 FUN_80061448(ff_u32(0x80093d70)+208,ff_u32(0x80093cfc));
 for(i=0;i<15;i++){ff_w32(0x80094678+4*i,ff_u32(0x80093cfc));FUN_80018DCC();}
 for(i=0;i<sizeof(head)/sizeof(head[0]);i++){ff_w32(head[i],ff_u32(0x80093cfc));FUN_80018DCC();}
 for(i=0;i<3;i++){ff_w32(0x800947e0+4*i,ff_u32(0x80093cfc));FUN_80018DCC();}
 for(i=0;i<sizeof(tail)/sizeof(tail[0]);i++)if(tail[i])ff_w32(tail[i],ff_u32(0x80093cfc));else FUN_80018DCC();
 ff_w32(0x80093cfc,(ff_u32(0x80093cfc)+3)&0xfffffffcu);return 0xfffffffcu;
}

/* 63100: left health bar. MIPS signed HP shifts; byte stores preserve
 * untouched packet padding. 6E7C0/6E798 reuse PsyQ G4/F4 constructors. */
GDB_CALL uint32 FUN_80063100(uint32 xy,uint32 actor) {
 sint32 maximum=ff_s16(actor+72)>>2,old=ff_s16(actor+70)>>2;
 sint32 shown=ff_s16(actor+68)>>2,hp=ff_s16(actor+66)>>2;
 uint32 d=0x800a8768u+12u*(uint32)(sint32)ff_s16(0x8009ad3eu+2u*(uint32)(sint32)ff_s16(0x800af4a8));
 uint32 x=xy&65535,y=(uint32)((sint32)xy>>16),p,bucket,previous,i;
 int wide=maximum>128,special;uint8 *bytes;sint32 delta,fade;
 if(wide){maximum>>=1;old>>=1;shown>>=1;hp>>=1;}
 p=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,p+36);bytes=ff_ptr(p,36);setPolyG4(bytes);
 special=ff_s16(actor+52)==53;
 for(i=0;i<4;i++) {
  bytes[4+8*i]=i<2?(wide?0:128):255;
  bytes[5+8*i]=i<2?(wide?128:0):(special?128:255);
  bytes[6+8*i]=i<2?(special?128:0):(special?255:128);
  ff_w16(p+8+8*i,(uint16)(x+(i<2?0:(uint32)shown)));
  ff_w16(p+10+8*i,(uint16)(y+2+((i&1)?4:0)));
 }
 FUN_80011E40(ff_u32(0x8008d4b4)+8188,p);
 delta=old-hp;if(delta<0)delta=-delta;
 if(delta>0) {
  fade=255*(shown-hp);if(fade<0)fade=-fade;fade/=delta;
  p=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,p+24);bytes=ff_ptr(p,24);setPolyF4(bytes);
  bytes[4]=(uint8)fade;bytes[5]=bytes[6]=0;
  for(i=0;i<4;i++) {
   ff_w16(p+8+4*i,(uint16)(x+(uint32)(i<2?shown:old)));
   ff_w16(p+10+4*i,(uint16)(y+2+((i&1)?4:0)));
  }
  FUN_80011E40(ff_u32(0x8008d4b4)+8188,p);
 }
 p=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,p+40);ff_w32(p+4,0x2d808080);
 ff_w32(p+12,ff_u32(d)&~1u);ff_w32(p+20,ff_u32(d+4));ff_w16(p+28,ff_s16(d+8));ff_w16(p+36,ff_s16(d+10));
 for(i=0;i<4;i++) {
  ff_w16(p+8+8*i,(uint16)(x+((i&1)?(uint32)maximum:0)));
  ff_w16(p+10+8*i,(uint16)(y+(i<2?0:11)));
 }
 /* 11E28: full PSX pointer in bucket, OR preceding link with DMA length. */
 bucket=ff_u32(0x8008d4b4)+8188;previous=ff_u32(bucket);ff_w32(bucket,p);ff_w32(p,previous|0x09000000);return previous;
}

/* 61D0C: score digits use signed kerning indexed by the next low nibble.
 * Unlike 61B7C, the original returns zero, including an empty string. */
GDB_CALL uint32 FUN_80061D0C(uint32 text,sint32 x,sint32 y) {
 sint32 position[3]={x,y,0};
 while(*(uint8*)ff_ptr(text,1)) {
  sint32 digit=*(sint8*)ff_ptr(text++,1);
  if(digit==32)position[0]=(sint32)((uint32)position[0]+6u);
  else {
   digit-=48;
   FUN_80012390(position,digit+ff_s16(0x8009ad14u+2u*(uint32)(sint32)ff_s16(0x800af4a8)));
   position[0]=(sint32)((uint32)position[0]+(sint32)*(sint8*)ff_ptr(0x80080140u+10u*(uint32)digit+(*(uint8*)ff_ptr(text,1)&15),1));
  }
 }
 return 0;
}
/* 61E0C is a two-byte decimal formatter, not general sprintf. Negative
 * arguments follow the one-character branch; preserve full 32-bit return. */
GDB_CALL uint32 FUN_80061E0C(sint32 value,uint32 output) {
 uint32 result=(uint32)value+48u;
 if(value<10){ff_w8(output,(uint8)result);ff_w8(output+1,0);}
 else {ff_w8(output,(uint8)(value/10+48));result=(uint32)(value%10)+48;ff_w8(output+1,(uint8)result);ff_w8(output+2,0);}
 return result;
}

/* 6351C: right bar, same fill arithmetic as 63100, reversed X. Border
 * is a run of sprites, not the left bar FT4; preserve last-call return. */
GDB_CALL uint32 FUN_8006351C(uint32 xy,uint32 actor) {
 sint32 maximum=ff_s16(actor+72)>>2,old=ff_s16(actor+70)>>2;
 sint32 shown=ff_s16(actor+68)>>2,hp=ff_s16(actor+66)>>2;
 uint32 x=xy&65535,y=(uint32)((sint32)xy>>16),p,result,i;
 sint32 position[3]={(sint32)x,(sint32)y,0};
 int wide=maximum>128,special;uint8 *bytes;sint32 delta,fade;
 if(wide){maximum>>=1;old>>=1;shown>>=1;hp>>=1;}
 p=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,p+36);bytes=ff_ptr(p,36);setPolyG4(bytes);
 special=ff_s16(actor+52)==53;
 for(i=0;i<4;i++) {
  bytes[4+8*i]=i<2?(wide?0:128):255;
  bytes[5+8*i]=i<2?(wide?128:0):(special?128:255);
  bytes[6+8*i]=i<2?(special?128:0):(special?255:128);
  ff_w16(p+8+8*i,(uint16)(x-(i<2?0:(uint32)shown)));
  ff_w16(p+10+8*i,(uint16)(y+2+((i&1)?4:0)));
 }
 result=FUN_80011E40(ff_u32(0x8008d4b4)+8188,p);
 delta=old-hp;if(delta<0)delta=-delta;
 if(delta>0) {
  fade=255*(shown-hp);if(fade<0)fade=-fade;fade/=delta;
  p=ff_u32(0x8008d4b0);ff_w32(0x8008d4b0,p+24);bytes=ff_ptr(p,24);setPolyF4(bytes);
  bytes[4]=(uint8)fade;bytes[5]=bytes[6]=0;
  for(i=0;i<4;i++) {
   ff_w16(p+8+4*i,(uint16)(x-(uint32)(i<2?shown:old)));
   ff_w16(p+10+4*i,(uint16)(y+2+((i&1)?4:0)));
  }
  result=FUN_80011E40(ff_u32(0x8008d4b4)+8188,p);
 }
 for(i=(uint32)(maximum>>3);(sint32)i>0;i--) {
  position[0]=(sint32)((uint32)position[0]-8u);
  result=FUN_80012390(position,ff_s16(0x8009ad3eu+2u*(uint32)(sint32)ff_s16(0x800af4a8)));
 }
 return result;
}

/* 61B28: centered text, arithmetic width/2, fixed palette93BE4. */
GDB_CALL sint32 FUN_80061B28(uint32 text,sint32 y) {
 FF_FUNCTION_MARKER(0x80061b28u,"GAME.EXE");
 sint32 width=FUN_8005EFB8(text);
 return FUN_80062C0C(text,(sint32)(160u-(uint32)(width>>1)),y,0x80093be4);
}
