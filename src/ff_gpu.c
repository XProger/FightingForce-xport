#include <stdlib.h>
/* Raster core adapted from AgentArmstrong game_platform.c:770..1221.
 * Provenance: status/menu/wrapper-provenance.json. Host renderer, WIP fidelity. */
#include "ff_gpu.h"
#include "ff.h"
#include <string.h>
#include <stdio.h>
#define FW 320
#define FH 240
#define VRAM_W 1024
#define VRAM_H 512
typedef struct { sint32 x,y,u,v; } RV;
typedef struct {
 sint32 minx,maxx,miny,maxy;
 sint64 e0,e1,e2,e0dx,e1dx,e2dx,e0dy,e1dy,e2dy;
 sint64 u,v,r,g,b,udx,vdx,rdx,gdx,bdx,udy,vdy,rdy,gdy,bdy;
} RasterScan;
static uint32 fb[FW*FH];
static uint16 vram[VRAM_W*VRAM_H],active_tpage;
static uint32 texture_window;
static uint32 display_reverse;
/* Diagnostic policy only: keep GPU commands and VRAM transfers, omit host pixels.
 * VRAM/pixel verification must explicitly set FF_VERIFY_VRAM=1 or FF_RASTERIZE=1. */
static sint32 raster_enabled(void) {
 static sint32 force=-1;
 if(!psx_is_headless())return 1;
 if(force<0) {
  const char *r=getenv("FF_RASTERIZE"),*v=getenv("FF_VERIFY_VRAM");
  force=(r&&!strcmp(r,"1"))||(v&&!strcmp(v,"1"));
  printf("headless_rasterization %s\n",force?"enabled":"disabled");
 }
 return force;
}
void ff_gpu_set_reverse(uint32 enabled){display_reverse=enabled!=0;}
static sint32 raster_clip_x0,raster_clip_y0,raster_clip_x1=FW,raster_clip_y1=FH,raster_offset_x,raster_offset_y;
static sint16 read_s16_le_at(const uint8 *p,sint32 o) { return (sint16)((uint16)p[o]|(uint16)p[o+1]<<8); }
static void pxc(sint32 x,sint32 y,uint32 color) {
 if(x>=raster_clip_x0&&x<raster_clip_x1&&y>=raster_clip_y0&&y<raster_clip_y1&&(uint32)x<FW&&(uint32)y<FH)fb[y*FW+x]=color;
}
static uint32 rgb555(uint16 c)
{
    uint32 r = (c & 31) << 3, g = ((c >> 5) & 31) << 3, b = ((c >> 10) & 31) << 3;
    return (r | (r >> 5)) << 16 | (g | (g >> 5)) << 8 | (b | (b >> 5));
}

static uint16 texel_indexed(sint32 u, sint32 v, uint16 tpage, uint16 clut, sint32 *transparent)
{
    sint32 tp = (tpage >> 7) & 3, tx = (tpage & 15) * 64, ty = ((tpage >> 4) & 1) * 256, cx = (clut & 63) * 16, cy = (clut >> 6) & 0x1ff, index;
    uint16 w, color;
    u = ((u & ~(((sint32)texture_window & 31) << 3)) | ((((sint32)texture_window >> 10) & 31) & ((sint32)texture_window & 31)) << 3) & 255;
    v = ((v & ~((((sint32)texture_window >> 5) & 31) << 3)) | ((((sint32)texture_window >> 15) & 31) & (((sint32)texture_window >> 5) & 31)) << 3) & 255;
    *transparent = 1;
    if ((uint32)(ty + v) >= VRAM_H || (uint32)cy >= VRAM_H)
        return 0;
    if (tp == 0)
    {
        w = vram[(ty + v) * VRAM_W + tx + (u >> 2)];
        index = (w >> ((u & 3) * 4)) & 15;
    }
    else if (tp == 1)
    {
        w = vram[(ty + v) * VRAM_W + tx + (u >> 1)];
        index = (w >> ((u & 1) * 8)) & 255;
    }
    else
    {
        if ((uint32)(tx + u) >= VRAM_W)
            return 0;
        color = vram[(ty + v) * VRAM_W + tx + u];
        *transparent = color == 0;
        return color;
    }
    if ((uint32)(cx + index) >= VRAM_W)
        return 0;
    color = vram[cy * VRAM_W + cx + index];
    *transparent = color == 0;
    return color;
}

static uint16 texel(sint32 u, sint32 v, uint16 tpage, uint16 clut)
{
    sint32 transparent;
    return texel_indexed(u, v, tpage, clut, &transparent);
}

static uint32 modulate(uint16 c, sint32 r, sint32 g, sint32 b, sint32 raw)
{
    uint32 q = rgb555(c), tr = (q >> 16) & 255, tg = (q >> 8) & 255, tb = q & 255;
    if (raw)
        return q;
    tr = tr * r / 128;
    tg = tg * g / 128;
    tb = tb * b / 128;
    if (tr > 255)
        tr = 255;
    if (tg > 255)
        tg = 255;
    if (tb > 255)
        tb = 255;
    return tr << 16 | tg << 8 | tb;
}

static uint32 semi_blend(uint32 back, uint32 front, sint32 abr)
{
    sint32 br = (back >> 19) & 31, bg = (back >> 11) & 31, bb = (back >> 3) & 31, fr = (front >> 19) & 31, fg = (front >> 11) & 31, fbv = (front >> 3) & 31, r, g, b;
    if (abr == 0)
    {
        r = (br + fr) >> 1;
        g = (bg + fg) >> 1;
        b = (bb + fbv) >> 1;
    }
    else if (abr == 1)
    {
        r = br + fr;
        g = bg + fg;
        b = bb + fbv;
    }
    else if (abr == 2)
    {
        r = br - fr;
        g = bg - fg;
        b = bb - fbv;
    }
    else
    {
        r = br + (fr >> 2);
        g = bg + (fg >> 2);
        b = bb + (fbv >> 2);
    }
    if (r < 0)
        r = 0;
    if (g < 0)
        g = 0;
    if (b < 0)
        b = 0;
    if (r > 31)
        r = 31;
    if (g > 31)
        g = 31;
    if (b > 31)
        b = 31;
    r = (r << 3) | (r >> 2);
    g = (g << 3) | (g >> 2);
    b = (b << 3) | (b >> 2);
    return (uint32)(r << 16 | g << 8 | b);
}

static uint32 packet_rgb(uint8 *p, sint32 o)
{
    return (uint32)p[o] << 16 | (uint32)p[o + 1] << 8 | p[o + 2];
}

static void raster_bounds(RV a, RV b, RV c, sint32 *minx, sint32 *maxx, sint32 *miny, sint32 *maxy)
{
    *minx = a.x;
    *maxx = a.x;
    *miny = a.y;
    *maxy = a.y;
    if (b.x < *minx)
        *minx = b.x;
    if (c.x < *minx)
        *minx = c.x;
    if (b.x > *maxx)
        *maxx = b.x;
    if (c.x > *maxx)
        *maxx = c.x;
    if (b.y < *miny)
        *miny = b.y;
    if (c.y < *miny)
        *miny = c.y;
    if (b.y > *maxy)
        *maxy = b.y;
    if (c.y > *maxy)
        *maxy = c.y;
    if (*minx < raster_clip_x0)
        *minx = raster_clip_x0;
    if (*maxx >= raster_clip_x1)
        *maxx = raster_clip_x1 - 1;
    if (*miny < raster_clip_y0)
        *miny = raster_clip_y0;
    if (*maxy >= raster_clip_y1)
        *maxy = raster_clip_y1 - 1;
    if (*minx < 0)
        *minx = 0;
    if (*miny < 0)
        *miny = 0;
    if (*maxx >= FW)
        *maxx = FW - 1;
    if (*maxy >= FH)
        *maxy = FH - 1;
}

static sint64 edge2(const RV *a, const RV *b, sint32 sx, sint32 sy)
{
    return (sint64)(sx - a->x * 2) * (b->y - a->y) - (sint64)(sy - a->y * 2) * (b->x - a->x);
}

static sint64 fixed_value(sint32 aa, sint32 bb, sint32 cc, sint64 e0, sint64 e1, sint64 e2, sint64 denominator)
{
    return ((sint64)aa * e0 + (sint64)bb * e1 + (sint64)cc * e2) * 65536 / denominator;
}

static sint32 setup_scan(RV a, RV b, RV c, uint32 ca, uint32 cb, uint32 cc, RasterScan *s)
{
    sint64 area, denominator, sign;
    sint32 sx, sy;
    raster_bounds(a, b, c, &s->minx, &s->maxx, &s->miny, &s->maxy);
    if (s->minx > s->maxx || s->miny > s->maxy)
        return 0;
    area = (sint64)(c.x - a.x) * (b.y - a.y) - (sint64)(c.y - a.y) * (b.x - a.x);
    if (area == 0)
        return 0;
    sign = area < 0 ? -1 : 1;
    denominator = area * 2 * sign;
    sx = s->minx * 2 + 1;
    sy = s->miny * 2 + 1;
    s->e0 = edge2(&b, &c, sx, sy) * sign;
    s->e1 = edge2(&c, &a, sx, sy) * sign;
    s->e2 = edge2(&a, &b, sx, sy) * sign;
    s->e0dx = (sint64)2 * (c.y - b.y) * sign;
    s->e1dx = (sint64)2 * (a.y - c.y) * sign;
    s->e2dx = (sint64)2 * (b.y - a.y) * sign;
    s->e0dy = (sint64)-2 * (c.x - b.x) * sign;
    s->e1dy = (sint64)-2 * (a.x - c.x) * sign;
    s->e2dy = (sint64)-2 * (b.x - a.x) * sign;
    s->u = fixed_value(a.u, b.u, c.u, s->e0, s->e1, s->e2, denominator);
    s->v = fixed_value(a.v, b.v, c.v, s->e0, s->e1, s->e2, denominator);
    s->r = fixed_value((ca >> 16) & 255, (cb >> 16) & 255, (cc >> 16) & 255, s->e0, s->e1, s->e2, denominator);
    s->g = fixed_value((ca >> 8) & 255, (cb >> 8) & 255, (cc >> 8) & 255, s->e0, s->e1, s->e2, denominator);
    s->b = fixed_value(ca & 255, cb & 255, cc & 255, s->e0, s->e1, s->e2, denominator);
    s->udx = fixed_value(a.u, b.u, c.u, s->e0dx, s->e1dx, s->e2dx, denominator);
    s->vdx = fixed_value(a.v, b.v, c.v, s->e0dx, s->e1dx, s->e2dx, denominator);
    s->rdx = fixed_value((ca >> 16) & 255, (cb >> 16) & 255, (cc >> 16) & 255, s->e0dx, s->e1dx, s->e2dx, denominator);
    s->gdx = fixed_value((ca >> 8) & 255, (cb >> 8) & 255, (cc >> 8) & 255, s->e0dx, s->e1dx, s->e2dx, denominator);
    s->bdx = fixed_value(ca & 255, cb & 255, cc & 255, s->e0dx, s->e1dx, s->e2dx, denominator);
    s->udy = fixed_value(a.u, b.u, c.u, s->e0dy, s->e1dy, s->e2dy, denominator);
    s->vdy = fixed_value(a.v, b.v, c.v, s->e0dy, s->e1dy, s->e2dy, denominator);
    s->rdy = fixed_value((ca >> 16) & 255, (cb >> 16) & 255, (cc >> 16) & 255, s->e0dy, s->e1dy, s->e2dy, denominator);
    s->gdy = fixed_value((ca >> 8) & 255, (cb >> 8) & 255, (cc >> 8) & 255, s->e0dy, s->e1dy, s->e2dy, denominator);
    s->bdy = fixed_value(ca & 255, cb & 255, cc & 255, s->e0dy, s->e1dy, s->e2dy, denominator);
    return 1;
}

static void textured_triangle(RV a, RV b, RV c, uint16 tp, uint16 cl, uint32 ca, uint32 cb, uint32 cc, sint32 raw, sint32 semi)
{
    RasterScan s;
    sint32 x, y;
    if (!setup_scan(a, b, c, ca, cb, cc, &s))
        return;
    for (y = s.miny; y <= s.maxy; y++)
    {
        sint64 e0 = s.e0, e1 = s.e1, e2 = s.e2, u = s.u, v = s.v, rr = s.r, gg = s.g, bb = s.b;
        sint32 entered = 0;
        for (x = s.minx; x <= s.maxx; x++)
        {
            if (e0 >= 0 && e1 >= 0 && e2 >= 0)
            {
                sint32 transparent, r, g, bl;
                uint16 t;
                uint32 color;
                entered = 1;
                t = texel_indexed((sint32)(u >> 16), (sint32)(v >> 16), tp, cl, &transparent);
                if (!transparent)
                {
                    r = raw ? 128 : (sint32)(rr >> 16);
                    g = raw ? 128 : (sint32)(gg >> 16);
                    bl = raw ? 128 : (sint32)(bb >> 16);
                    color = modulate(t, r, g, bl, 0);
                    if (semi && (t & 0x8000))
                        color = semi_blend(fb[y * FW + x], color, (tp >> 5) & 3);
                    fb[y * FW + x] = color;
                }
            }
            else if (entered)
                break;
            e0 += s.e0dx;
            e1 += s.e1dx;
            e2 += s.e2dx;
            u += s.udx;
            v += s.vdx;
            rr += s.rdx;
            gg += s.gdx;
            bb += s.bdx;
        }
        s.e0 += s.e0dy;
        s.e1 += s.e1dy;
        s.e2 += s.e2dy;
        s.u += s.udy;
        s.v += s.vdy;
        s.r += s.rdy;
        s.g += s.gdy;
        s.b += s.bdy;
    }
}

static void colored_triangle(RV a, RV b, RV c, uint32 ca, uint32 cb, uint32 cc, sint32 semi)
{
    RasterScan s;
    sint32 x, y;
    if (!setup_scan(a, b, c, ca, cb, cc, &s))
        return;
    for (y = s.miny; y <= s.maxy; y++)
    {
        sint64 e0 = s.e0, e1 = s.e1, e2 = s.e2, rr = s.r, gg = s.g, bb = s.b;
        sint32 entered = 0;
        for (x = s.minx; x <= s.maxx; x++)
        {
            if (e0 >= 0 && e1 >= 0 && e2 >= 0)
            {
                uint32 color;
                sint32 r = (sint32)(rr >> 16), g = (sint32)(gg >> 16), bl = (sint32)(bb >> 16);
                entered = 1;
                color = (uint32)r << 16 | (uint32)g << 8 | (uint32)bl;
                if (semi)
                    color = semi_blend(fb[y * FW + x], color, (active_tpage >> 5) & 3);
                fb[y * FW + x] = color;
            }
            else if (entered)
                break;
            e0 += s.e0dx;
            e1 += s.e1dx;
            e2 += s.e2dx;
            rr += s.rdx;
            gg += s.gdx;
            bb += s.bdx;
        }
        s.e0 += s.e0dy;
        s.e1 += s.e1dy;
        s.e2 += s.e2dy;
        s.r += s.rdy;
        s.g += s.gdy;
        s.b += s.bdy;
    }
}

static RV rv(uint8 *p, sint32 xy, sint32 uv)
{
    RV v;
    v.x = read_s16_le_at(p, xy) + raster_offset_x;
    v.y = read_s16_le_at(p, xy + 2) + raster_offset_y;
    v.u = p[uv];
    v.v = p[uv + 1];
    return v;
}

static void magenta_bbox(uint8 *p, sint32 n, const sint32 *ofs)
{
    sint32 i, minx = 32767, miny = 32767, maxx = -32768, maxy = -32768, x, y;
    for (i = 0; i < n; i++)
    {
        x = read_s16_le_at(p, ofs[i]);
        y = read_s16_le_at(p, ofs[i] + 2);
        if (x < minx)
            minx = x;
        if (x > maxx)
            maxx = x;
        if (y < miny)
            miny = y;
        if (y > maxy)
            maxy = y;
    }
    if (minx < 0)
        minx = 0;
    if (miny < 0)
        miny = 0;
    if (maxx >= FW)
        maxx = FW - 1;
    if (maxy >= FH)
        maxy = FH - 1;
    for (y = miny; y <= maxy; y++)
        for (x = minx; x <= maxx; x++)
            pxc(x, y, 0xff00ff);
}

static void draw_prim(void *raw)
{
    uint8 *p = (uint8 *)raw;
    sint32 code = p[7] & 0xfc;
    uint32 *w = (uint32 *)p;
    uint32 ca, cb, cc, cd;
    if ((w[1] & 0xff000000) == 0xe1000000)
    {
        active_tpage = (uint16)(w[1] & 0x9ff);
        if ((w[2] & 0xff000000) == 0xe2000000)
            texture_window = w[2] & 0xfffff;
        return;
    }
    if ((w[1] & 0xff000000) == 0xe2000000)
    {
        texture_window = w[1] & 0xfffff;
        return;
    }
    if(!raster_enabled())return;
    ca = packet_rgb(p, 4);
    /* Host approximation of flat two-point PSX lines. Visual fidelity is
     * secondary; retain clipping, draw offset and semitransparency. */
    if (code == 0x40)
    {
        RV a = rv(p, 8, 0), b = rv(p, 12, 0);
        sint32 dx = abs(b.x-a.x), dy = -abs(b.y-a.y);
        sint32 sx = a.x < b.x ? 1 : -1, sy = a.y < b.y ? 1 : -1;
        sint32 err = dx+dy, e;
        for (;;) {
            if ((uint32)a.x < FW && (uint32)a.y < FH) {
                uint32 color = ca;
                if (p[7]&2)color=semi_blend(fb[a.y*FW+a.x],color,(active_tpage>>5)&3);
                pxc(a.x,a.y,color);
            }
            if(a.x==b.x && a.y==b.y)break;
            e=2*err;
            if(e>=dy){err+=dy;a.x+=sx;}
            if(e<=dx){err+=dx;a.y+=sy;}
        }
        return;
    }
    if (code == 0x20)
    {
        RV a = rv(p, 8, 0), b = rv(p, 12, 0), c = rv(p, 16, 0);
        colored_triangle(a, b, c, ca, ca, ca, (p[7] & 2) != 0);
        return;
    }
    if (code == 0x28)
    {
        RV a = rv(p, 8, 0), b = rv(p, 12, 0), c = rv(p, 16, 0), d = rv(p, 20, 0);
        colored_triangle(a, b, c, ca, ca, ca, (p[7] & 2) != 0);
        colored_triangle(b, c, d, ca, ca, ca, (p[7] & 2) != 0);
        return;
    }
    if (code == 0x30)
    {
        RV a = rv(p, 8, 0), b = rv(p, 16, 0), c = rv(p, 24, 0);
        cb = packet_rgb(p, 12);
        cc = packet_rgb(p, 20);
        colored_triangle(a, b, c, ca, cb, cc, (p[7] & 2) != 0);
        return;
    }
    if (code == 0x38)
    {
        RV a = rv(p, 8, 0), b = rv(p, 16, 0), c = rv(p, 24, 0), d = rv(p, 32, 0);
        cb = packet_rgb(p, 12);
        cc = packet_rgb(p, 20);
        cd = packet_rgb(p, 28);
        colored_triangle(a, b, c, ca, cb, cc, (p[7] & 2) != 0);
        colored_triangle(b, c, d, cb, cc, cd, (p[7] & 2) != 0);
        return;
    }
    if (code == 0x24)
    {
        RV a = rv(p, 8, 12), b = rv(p, 16, 20), c = rv(p, 24, 28);
        textured_triangle(a, b, c, *(uint16 *)(p + 22), *(uint16 *)(p + 14), ca, ca, ca, p[7] & 1, (p[7] & 2) != 0);
        return;
    }
    if (code == 0x2c)
    {
        RV a = rv(p, 8, 12), b = rv(p, 16, 20), c = rv(p, 24, 28), d = rv(p, 32, 36);
        uint16 tp = *(uint16 *)(p + 22), cl = *(uint16 *)(p + 14);
        textured_triangle(a, b, c, tp, cl, ca, ca, ca, p[7] & 1, (p[7] & 2) != 0);
        textured_triangle(b, c, d, tp, cl, ca, ca, ca, p[7] & 1, (p[7] & 2) != 0);
        return;
    }
    if (code == 0x34)
    {
        RV a = rv(p, 8, 12), b = rv(p, 20, 24), c = rv(p, 32, 36);
        uint16 tp = *(uint16 *)(p + 26), cl = *(uint16 *)(p + 14);
        cb = packet_rgb(p, 16);
        cc = packet_rgb(p, 28);
        textured_triangle(a, b, c, tp, cl, ca, cb, cc, p[7] & 1, (p[7] & 2) != 0);
        return;
    }
    if (code == 0x3c)
    {
        RV a = rv(p, 8, 12), b = rv(p, 20, 24), c = rv(p, 32, 36), d = rv(p, 44, 48);
        uint16 tp = *(uint16 *)(p + 26), cl = *(uint16 *)(p + 14);
        cb = packet_rgb(p, 16);
        cc = packet_rgb(p, 28);
        cd = packet_rgb(p, 40);
        textured_triangle(a, b, c, tp, cl, ca, cb, cc, p[7] & 1, (p[7] & 2) != 0);
        textured_triangle(b, c, d, tp, cl, cb, cc, cd, p[7] & 1, (p[7] & 2) != 0);
        return;
    }
    if (code == 0x64 || code == 0x74 || code == 0x7c)
    {
        sint32 x, y, x0 = read_s16_le_at(p, 8) + raster_offset_x, y0 = read_s16_le_at(p, 10) + raster_offset_y, ww = code == 0x64 ? read_s16_le_at(p, 16) : (code == 0x74 ? 8 : 16), hh = code == 0x64 ? read_s16_le_at(p, 18) : (code == 0x74 ? 8 : 16);
        for (y = 0; y < hh; y++)
            for (x = 0; x < ww; x++)
            {
                sint32 transparent;
                uint16 t = texel_indexed(p[12] + x, p[13] + y, active_tpage, *(uint16 *)(p + 14), &transparent);
                if (!transparent && x0 + x >= raster_clip_x0 && x0 + x < raster_clip_x1 && y0 + y >= raster_clip_y0 && y0 + y < raster_clip_y1 && (uint32)(x0 + x) < FW && (uint32)(y0 + y) < FH)
                {
                    uint32 color = modulate(t, p[4], p[5], p[6], p[7] & 1);
                    if ((p[7] & 2) && (t & 0x8000))
                        color = semi_blend(fb[(y0 + y) * FW + x0 + x], color, (active_tpage >> 5) & 3);
                    fb[(y0 + y) * FW + x0 + x] = color;
                }
            }
        return;
    }
    if (code == 0x60 || code == 0x68 || code == 0x70 || code == 0x78)
    {
        sint32 x, y, x0 = read_s16_le_at(p, 8) + raster_offset_x, y0 = read_s16_le_at(p, 10) + raster_offset_y, ww = code == 0x60 ? read_s16_le_at(p, 12) : (code == 0x68 ? 1 : code == 0x70 ? 8 : 16), hh = code == 0x60 ? read_s16_le_at(p, 14) : (code == 0x68 ? 1 : code == 0x70 ? 8 : 16);
        for (y = 0; y < hh; y++)
            for (x = 0; x < ww; x++)
            {
                uint32 color = ca;
                if ((uint32)(x0+x)>=FW || (uint32)(y0+y)>=FH) continue;
                if (p[7] & 2)
                    color = semi_blend(fb[(y0 + y) * FW + x0 + x], color, (active_tpage >> 5) & 3);
                pxc(x0 + x, y0 + y, color);
            }
        return;
    }
    pxc(read_s16_le_at(p, 8) + raster_offset_x, read_s16_le_at(p, 10) + raster_offset_y, 0xff00ff);
}
void ff_gpu_begin(void) { if(raster_enabled())memset(fb,0,sizeof(fb)); }
/* Host PsyQ DrawOTagEnv adaptation, not a replacement decompilation of the SDK.
 * The caller supplies the VRAM origin of the draw surface (not the displayed
 * surface). This preserves relative clip/offset changes in the 320x240 window.
 * Dithering, draw-to-display inhibition and GPU mask bits remain WIP. */
void ff_gpu_draw_env(uint32 env,sint32 surface_x,sint32 surface_y) {
 const uint8 *e=(const uint8*)ff_ptr(env,28);
 sint32 x,y;
 raster_clip_x0=ff_s16(env)-surface_x;
 raster_clip_y0=ff_s16(env+2)-surface_y;
 raster_clip_x1=raster_clip_x0+ff_s16(env+4);
 raster_clip_y1=raster_clip_y0+ff_s16(env+6);
 raster_offset_x=ff_s16(env+8)-surface_x;
 raster_offset_y=ff_s16(env+10)-surface_y;
 active_tpage=(uint16)ff_s16(env+20);
 /* Texture-window packing follows the actual 80070590..800705F0 MIPS. */
 texture_window=(((uint32)(-ff_s16(env+16))&255u)>>3)
  | ((((uint32)(-ff_s16(env+18))&255u)>>3)<<5)
  | ((uint32)(e[12]>>3)<<10)
  | ((uint32)(e[14]>>3)<<15);
 if(e[24]&&raster_enabled()) {
  uint32 color=(uint32)e[25]<<16|(uint32)e[26]<<8|e[27];
  for(y=0;y<FH;y++)for(x=0;x<FW;x++)pxc(x,y,color);
 }
}
/* 703A0/7046C indexed twins differ only in the command opcode. */
static uint8 gpu_env_u8(uint32 address) { return *(uint8*)ff_ptr(address,1); }
static sint32 gpu_env_clamp(sint32 value,sint32 limit) {
 if(value<0)return 0;
 return value>limit-1?limit-1:value;
}
static uint32 gpu_env_clip(uint32 command,sint32 x,sint32 y,uint32 wide) {
 uint32 mask=wide?4095u:1023u,shift=wide?12u:10u;
 x=gpu_env_clamp((sint16)x,ff_s16(0x800872f0));
 y=gpu_env_clamp((sint16)y,ff_s16(0x800872f2));
 return command|((uint32)x&mask)|(((uint32)y&mask)<<shift);
}
/* 700A0 packet writes and 6F828 cache copy, audited from actual MIPS.
 * DMA/interrupt queue bookkeeping remains a separate host SDK adaptation. */
void ff_gpu_put_draw_env(uint32 env,sint32 surface_x,sint32 surface_y) {
 uint32 packet=env+28,wide=((uint32)gpu_env_u8(0x800872ec)-1u)<2u;
 uint32 mask=wide?4095u:2047u,shift=wide?12u:11u,mode,count=6;
 sint32 x,y,w,h;
 ff_w32(packet+4,gpu_env_clip(0xe3000000u,ff_s16(env),ff_s16(env+2),wide));
 ff_w32(packet+8,gpu_env_clip(0xe4000000u,(sint16)((uint16)ff_s16(env)+(uint16)ff_s16(env+4)-1),
  (sint16)((uint16)ff_s16(env+2)+(uint16)ff_s16(env+6)-1),wide));
 ff_w32(packet+12,0xe5000000u|((uint32)ff_s16(env+8)&mask)|(((uint32)ff_s16(env+10)&mask)<<shift));
 mode=0xe1000000u|((uint16)ff_s16(env+20)&(wide?0x27ffu:0x9ffu));
 if(gpu_env_u8(env+22))mode|=wide?0x800u:0x200u;
 if(gpu_env_u8(env+23))mode|=wide?0x1000u:0x400u;
 ff_w32(packet+16,mode);
 ff_w32(packet+20,0xe2000000u|(((uint32)-ff_s16(env+16)&255u)>>3)
  |((((uint32)-ff_s16(env+18)&255u)>>3)<<5)
  |((uint32)(gpu_env_u8(env+12)>>3)<<10)|((uint32)(gpu_env_u8(env+14)>>3)<<15));
 ff_w32(packet+24,0xe6000000u);
 if(gpu_env_u8(env+24)) {
  x=ff_s16(env);y=ff_s16(env+2);
  w=(sint16)gpu_env_clamp(ff_s16(env+4),ff_s16(0x800872f0));
  h=(sint16)gpu_env_clamp(ff_s16(env+6),ff_s16(0x800872f2));
  mode=0x02000000u;
  if(((uint32)x&63u)||((uint32)w&63u)) {
   mode=0x60000000u;x=(sint16)(x-ff_s16(env+8));y=(sint16)(y-ff_s16(env+10));
  }
  ff_w32(packet+28,mode|gpu_env_u8(env+25)|((uint32)gpu_env_u8(env+26)<<8)|((uint32)gpu_env_u8(env+27)<<16));
  ff_w32(packet+32,(uint16)x|((uint32)(uint16)y<<16));
  ff_w32(packet+36,(uint16)w|((uint32)(uint16)h<<16));count=9;
 }
 ff_w8(packet+3,(uint8)count);ff_w32(packet,ff_u32(packet)|0xffffffu);
 memcpy(ff_ptr(0x800872fc,92),ff_ptr(env,92),92);
 ff_gpu_draw_env(env,surface_x,surface_y);
}
void ff_gpu_packet(void *packet) { draw_prim(packet); }
/* Host DMA adapter: consume PSX address tags, never native pointer tags. */
sint32 ff_gpu_ot(uint32 head) {
 uint32 count=0;
 while((head&0xffffffu)!=0xffffffu) {
  uint32 address=head&0xffffffu,tag;
  if(address>=0x200000u || (address&3u) || count++>=65536)return -1;
  tag=ff_u32(address);
  /* Preserve the original terminal packet in RAM. The host still omits this
   * exact degenerate black-line sentinel, as it did before original OT setup
   * was translated. Other flat line packets use the host rasterizer. */
  if(address==0x10018u && tag==0x03ffffffu && ff_u32(address+4)==0x40000000u && !ff_u32(address+8) && !ff_u32(address+12)){head=tag;continue;}
  if(tag>>24)draw_prim(ff_ptr(address,4u+4u*(tag>>24)));
  head=tag;
 }
 return (sint32)count;
}
sint32 ff_gpu_present(void) {
 static uint32 reversed[FW*FH];sint32 x,y;
 if(!raster_enabled())return 1;
 if(!display_reverse)return psx_window_present(fb,FW,FH,"Fighting Force");
 for(y=0;y<FH;++y)for(x=0;x<FW;++x)reversed[y*FW+x]=fb[y*FW+FW-1-x];
 return psx_window_present(reversed,FW,FH,"Fighting Force");
}
/* Host presentation adaptation: emulate display-origin adjustment as a clipped
 * pixel translation. PSX scanout timing/overscan is not reproduced here. */
void ff_gpu_display_offset(sint32 x,sint32 y) {
 static uint32 copy[FW*FH];sint32 row,col;
 if(!raster_enabled())return;
 if(!x&&!y)return;memcpy(copy,fb,sizeof(fb));memset(fb,0,sizeof(fb));
 for(row=0;row<FH;row++)for(col=0;col<FW;col++) {
  sint32 sx=col-x,sy=row-y;
  if((uint32)sx<FW&&(uint32)sy<FH)fb[row*FW+col]=copy[sy*FW+sx];
 }
}
void ff_gpu_init_empty(void) {
 display_reverse=0;
 memset(vram,0,sizeof(vram));memset(fb,0,sizeof(fb));active_tpage=0;texture_window=0;
 raster_clip_x0=raster_clip_y0=raster_offset_x=raster_offset_y=0;raster_clip_x1=FW;raster_clip_y1=FH;
}
sint32 ff_gpu_load_vram(const char *path) {
 FILE *f=fopen(path,"rb"); size_t n;
 if(!f)return 0; n=fread(vram,1,sizeof(vram),f);fclose(f);return n==sizeof(vram);
}
sint32 ff_gpu_save_frame(const char *path) {
 FILE *f;size_t n;
 if(!raster_enabled())return 0; /* No valid pixels to export in fast mode. */
 f=fopen(path,"wb");if(!f)return 0;n=fwrite(fb,1,sizeof(fb),f);fclose(f);return n==sizeof(fb);
}

/* Host LoadImage adapter for the menu's in-bounds RAM rectangle. Upload remains
 * before scene generation, preserving the prior texture-buffer contents. */
sint32 ff_gpu_load_rect(uint32 source,sint32 x,sint32 y,sint32 w,sint32 h) {
 sint32 row;
 if(x<0||y<0||w<0||h<0||x+w>VRAM_W||y+h>VRAM_H)return 0;
 for(row=0;row<h;row++)memcpy(vram+(y+row)*VRAM_W+x,ff_ptr(source+(uint32)(row*w*2),(size_t)w*2),(size_t)w*2);
 return 1;
}
sint32 ff_gpu_load_image(uint32 rect,uint32 source) {
 return ff_gpu_load_rect(source,ff_s16(rect),ff_s16(rect+2),ff_s16(rect+4),ff_s16(rect+6));
}
/* PsyQ StoreImage boundary reused from AgentArmstrong's software VRAM.
 * Framebuffer/VRAM coherence remains a renderer limitation, independent of
 * the screenshot caller's audited packing and transfer sequence. */
sint32 ff_gpu_store_image(uint32 rect,uint32 destination) {
 sint32 x=ff_s16(rect),y=ff_s16(rect+2),w=ff_s16(rect+4),h=ff_s16(rect+6),row;
 if(x<0||y<0||w<0||h<0||x+w>VRAM_W||y+h>VRAM_H)return -1;
 for(row=0;row<h;++row)memcpy(ff_ptr(destination+(uint32)(row*w*2),(size_t)w*2),vram+(y+row)*VRAM_W+x,(size_t)w*2);
 return 0;
}
/* Host SDK adapter: 8006E240 constructs a 256x1 rectangle for LoadImage.
 * Return is host upload success, not the SDK CLUT identifier. */
sint32 ff_gpu_load_clut(uint32 source,sint32 x,sint32 y) {
 return ff_gpu_load_rect(source,(sint16)x,(sint16)y,256,1);
}

/* Original startup GP0 fill: black rectangle(0,0,640,256). */
/* 6ED14 mode0 CPU state. Host software GPU selects original type0;
 * BIOS callbacks, DMA addresses and hardware reset are handled by the host. */
void ff_gpu_reset_graph_state(void) {
 memset(ff_ptr(0x800872ec,128),0,128);ff_w8(0x800872ed,1);
 ff_w16(0x800872f0,(uint16)ff_s16(0x8008736c));ff_w16(0x800872f2,(uint16)ff_s16(0x80087380));
 memset(ff_ptr(0x800872fc,92),255,92);memset(ff_ptr(0x80087358,20),255,20);
}
/* SDK adapters from 6E310/6E3E0. DRAWENV packet storage at +28 is
 * deliberately untouched. The two GetGraphType calls have no side effects. */
uint32 ff_gpu_set_def_draw_env(uint32 env,sint32 x,sint32 y,sint32 w,sint32 h) {
 uint32 i;
 ff_w16(env,(uint16)x);ff_w16(env+2,(uint16)y);ff_w16(env+4,(uint16)w);ff_w16(env+6,(uint16)h);
 for(i=12;i<=18;i+=2)ff_w16(env+i,0);
 ff_w8(env+25,0);ff_w8(env+26,0);ff_w8(env+27,0);ff_w8(env+22,1);
 ff_w8(env+23,(uint8)(h<(ff_u32(0x8008cb18)?289:257)));
 ff_w16(env+8,(uint16)x);ff_w16(env+10,(uint16)y);ff_w16(env+20,10);ff_w8(env+24,0);
 return env;
}
uint32 ff_gpu_set_def_disp_env(uint32 env,sint32 x,sint32 y,sint32 w,sint32 h) {
 ff_w16(env,(uint16)x);ff_w16(env+2,(uint16)y);ff_w16(env+4,(uint16)w);
 memset(ff_ptr(env+8,12),0,12);ff_w16(env+6,(uint16)h);return env;
}
void ff_gpu_clear_menu_surfaces(void) {
 uint32 y;for(y=0;y<256;y++)memset(vram+y*VRAM_W,0,640*sizeof(uint16));
}

#include "diagnostic_state.h"
int ff_gpu_state_io(FILE *f,int load) { return FF_STATE(f,fb,load)&&FF_STATE(f,vram,load)&&FF_STATE(f,active_tpage,load)&&FF_STATE(f,texture_window,load)&&FF_STATE(f,display_reverse,load)&&FF_STATE(f,raster_clip_x0,load)&&FF_STATE(f,raster_clip_y0,load)&&FF_STATE(f,raster_clip_x1,load)&&FF_STATE(f,raster_clip_y1,load)&&FF_STATE(f,raster_offset_x,load)&&FF_STATE(f,raster_offset_y,load); }
