// generated by Fast Light User Interface Designer (fluid) version 1.0010

#include "Fl_SevenSeg.H"

// Andy Preston changed
// Fl_SevenSeg::Fl_SevenSeg(int x,int y,int w,int h):Fl_Widget(x,y,w,h) {
// to
Fl_SevenSeg::Fl_SevenSeg(int x, int y, int w, int h) :
Fl_Widget (x, y, w, h),
decpt (off) {

  color2(FL_BLACK);
	color(FL_RED);
	digit = 8;
	segwidth = (w-8)/13;
	segwidth = segwidth<(h-8)/13 ?(h-8)/13: segwidth;
        if (segwidth<1) segwidth =1;
}

Fl_SevenSeg::~Fl_SevenSeg(void) {
}

// Andy Preston changed
//void Fl_SevenSeg::value(int v) {
//  if (v <0) digit = -1;
//	else
//	if (v>9) digit = 9;
//	else
//	digit = v;
//	if (active()) redraw();
//}
// to
void Fl_SevenSeg::value (int v) {
     if (v>9) digit = 9;
     else digit = v;
     if (active()) redraw();
}

void Fl_SevenSeg::value(char c) {
  int v = c - '0';
	value(v);
}

const int Fl_SevenSeg::value(void) {
  return digit;
}

// Andy Preston changed
// void Fl_SevenSeg::dp(int onoff) {
//  decpt =(onoff!=0);
// }
// to
void Fl_SevenSeg::dp (dp_type state) {
  decpt = state;
}

// Andy Preston changed
// const int Fl_SevenSeg::dp(void) {
// to
const dp_type Fl_SevenSeg::dp(void) {
  return decpt;
}

const int Fl_SevenSeg::bar_width(void) {
  return segwidth;
}

void Fl_SevenSeg::bar_width(int w) {
  segwidth = w > 2 ? w:2;
	if (active()) redraw();
}
// PRIVATE METHODS

void Fl_SevenSeg::draw() {
  uchar r,g,b;
int	val;

	draw_box();

        // andy preston
        fl_color (color2());
	fl_rectf (x(), y(), w(), h());

        // Andy Preston Changed
        // int xx = x()+4;
	// int ww = w()-8;
	// int yy = y()+4;
	// int hh = h()-8;
        // to
        int xx = x()+2;
	int ww = w()-4;
	int yy = y()+2;
	int hh = h()-4;

        // Andy Preston removed
	// fl_clip(xx,yy,ww,hh);
	// fl_color(color2());
	// fl_rectf(xx,yy,ww,hh);

	// Andy Preston changed
        // if (digit == -1) return;
	if (digit < -1) return;

        Fl::get_color(color(),r,g,b);
	if (!active())
	{
		val = 64+(int)r/4; r = val > 255 ? 255:val;
		val = 64+(int)g/4; g = val > 255 ? 255:val;
		val = 64+(int)b/4; b = val > 255 ? 255:val;
	}
	fl_color(r,g,b);

        fl_line_style(FL_SOLID|FL_CAP_ROUND,segwidth);

	switch(digit)
	{
                // Andy Preston
                case -1:
			draw_seg_g (xx, yy, ww, hh);
			break;


		case 0:
			draw_seg_a(xx,yy,ww,hh);
			draw_seg_b(xx,yy,ww,hh);
			draw_seg_c(xx,yy,ww,hh);
			draw_seg_d(xx,yy,ww,hh);
			draw_seg_e(xx,yy,ww,hh);
			draw_seg_f(xx,yy,ww,hh);
			break;
		case 1:
			draw_seg_b(xx,yy,ww,hh);
			draw_seg_c(xx,yy,ww,hh);
			break;
		case 2:
			draw_seg_a(xx,yy,ww,hh);
			draw_seg_b(xx,yy,ww,hh);
			draw_seg_g(xx,yy,ww,hh);
			draw_seg_e(xx,yy,ww,hh);
			draw_seg_d(xx,yy,ww,hh);
			break;
		case 3:
			draw_seg_a(xx,yy,ww,hh);
			draw_seg_b(xx,yy,ww,hh);
			draw_seg_c(xx,yy,ww,hh);
			draw_seg_d(xx,yy,ww,hh);
			draw_seg_g(xx,yy,ww,hh);
			break;
		case 4:
			draw_seg_b(xx,yy,ww,hh);
			draw_seg_c(xx,yy,ww,hh);
			draw_seg_f(xx,yy,ww,hh);
			draw_seg_g(xx,yy,ww,hh);
			break;
		case 5:
			draw_seg_a(xx,yy,ww,hh);
			draw_seg_c(xx,yy,ww,hh);
			draw_seg_d(xx,yy,ww,hh);
			draw_seg_f(xx,yy,ww,hh);
			draw_seg_g(xx,yy,ww,hh);
			break;
		case 6:
			draw_seg_a(xx,yy,ww,hh);
			draw_seg_c(xx,yy,ww,hh);
			draw_seg_d(xx,yy,ww,hh);
			draw_seg_e(xx,yy,ww,hh);
			draw_seg_f(xx,yy,ww,hh);
			draw_seg_g(xx,yy,ww,hh);
			break;
		case 7:
			draw_seg_a(xx,yy,ww,hh);
			draw_seg_b(xx,yy,ww,hh);
			draw_seg_c(xx,yy,ww,hh);
			break;
		case 8:
			draw_seg_a(xx,yy,ww,hh);
			draw_seg_b(xx,yy,ww,hh);
			draw_seg_c(xx,yy,ww,hh);
			draw_seg_d(xx,yy,ww,hh);
			draw_seg_e(xx,yy,ww,hh);
			draw_seg_f(xx,yy,ww,hh);
			draw_seg_g(xx,yy,ww,hh);
			break;
		case 9:
			draw_seg_a(xx,yy,ww,hh);
			draw_seg_b(xx,yy,ww,hh);
			draw_seg_c(xx,yy,ww,hh);
			draw_seg_d(xx,yy,ww,hh);
			draw_seg_g(xx,yy,ww,hh);
			draw_seg_f(xx,yy,ww,hh);
			break;
	}

        // Andy Preston changed (naughty, naughty, who missed out the dp drawing code then?)
        // fl_line_style(FL_SOLID,1);
        // if (decpt) { }
        // to
        if (decpt==point) draw_seg_dp (xx, yy, ww, hh);
        else if (decpt==colon) draw_seg_col (xx, yy, ww, hh);
        fl_line_style(FL_SOLID,1);

        //fl_pop_clip();
}


// Andy Preston
void Fl_SevenSeg::draw_seg_dp (int xx, int yy, int ww, int hh) {
     int x1 = xx - segwidth / 2;
     int y1 = yy + hh - segwidth;
     int x2 = x1 + segwidth / 3;
     fl_line (x1, y1, x2, y1);
}

// Andy Preston
void Fl_SevenSeg::draw_seg_col (int xx, int yy, int ww, int hh) {
     int x1 = xx - segwidth/2;
     int x2 = x1 + segwidth/3;
     int y1 = yy + 1 + hh/4;
     int y2 = yy - 1 + (hh/4) * 3;
     fl_line (x1, y1, x2, y1);
     fl_line (x1, y2, x2, y2);
}

void Fl_SevenSeg::draw_seg_a(int xx,int yy,int ww,int hh) {
  int x1 = xx+2*segwidth+1;
	int y1 = yy+segwidth;
	int x2 = xx+ww-2*segwidth-1;
	fl_line(x1,y1,x2,y1);
}

void Fl_SevenSeg::draw_seg_b(int xx,int yy,int ww,int hh) {
  int x1 = xx+ww-segwidth;
	int y1 = yy+(3*segwidth)/2+1;
	int y2 = y1+hh/2-2*segwidth-1;
	fl_line(x1,y1,x1,y2);
}

void Fl_SevenSeg::draw_seg_c(int xx,int yy,int ww,int hh) {
  int x1 = xx+ww-segwidth;
	int y1 = yy+(hh+segwidth)/2+1;
	int y2 = y1+hh/2-2*segwidth-1;
	fl_line(x1,y1,x1,y2);
}

void Fl_SevenSeg::draw_seg_d(int xx,int yy,int ww,int hh) {
  int x1 = xx+2*segwidth+1;
	int y1 = yy+hh-segwidth;
	int x2 = xx+ww-2*segwidth-1;
	fl_line(x1,y1,x2,y1);
}

void Fl_SevenSeg::draw_seg_e(int xx,int yy,int ww,int hh) {
  int x1 = xx+segwidth;
	int y1 = yy+(hh+segwidth)/2+1;
	int y2 = y1+hh/2-2*segwidth-1;
	fl_line(x1,y1,x1,y2);
}

void Fl_SevenSeg::draw_seg_f(int xx,int yy,int ww,int hh) {
  int x1 = xx+segwidth;
	int y1 = yy+(3*segwidth)/2+1;
	int y2 = y1+hh/2-2*segwidth-1;
	fl_line(x1,y1,x1,y2);
}

void Fl_SevenSeg::draw_seg_g(int xx,int yy,int ww,int hh) {
  int x1 = xx+2*segwidth+1;
	int y1 = yy+(hh-segwidth/2)/2;
	int x2 = xx+ww-2*segwidth-1;
	fl_line(x1,y1,x2,y1);
}
