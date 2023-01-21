#ifndef __CANVAS_HPP__
#define __CANVAS_HPP__

/*==============================================================//
class Canvas
    A super template class of image data with 2D polygon drawing functions.
    It is assumed that this class is used for powerful microcomputers 
    like ESP32.
    The color space is RGB but the data format should be defined by
    the child class. 
    The drawing functions are written to support alpha channel and 
    antialiasing.
    alpha = 0 is opaque and alpha = 128 is transparent 

    - Coordinates
    The coordinates (x,y) of the center of the first pixel is (0,0).
    
    画像クラスと描画関数を備えたテンプレートクラス。
    ESP32などのマイコン向けに開発。
    描画関数は半透明カラーとアンチエイリアスをサポート。
    色はRGBを想定しているが、フォーマット(RGB565やRGB888など)を指定するために、
    このクラスを継承して使う。
    
//==============================================================*/
#include <string>
#include "Color.hpp"
#include "Polygon2D.hpp"
#include "ColoredPolygon.hpp"
#include "VectorPicture.hpp"

#include <iostream>
#include <fstream>
#include <cmath>

template <
    unsigned int WIDTH, 
    unsigned int HEIGHT, 
    unsigned int BYTES_PER_PIXEL,
    class Color
> 
class Canvas{

    //================
    // Variables / 変数
    //================
    protected:
    // Size of the image / 画像サイズ
    static const int width = WIDTH;
    static const int height = HEIGHT; 
    static const int bytes_per_pixel = BYTES_PER_PIXEL;
    static const int n_pixels = WIDTH * HEIGHT;
    static const int n_data = WIDTH * HEIGHT * BYTES_PER_PIXEL;
    
    // Pixle values / 画素値
    uint8_t data[ n_data ];

    // The state of this canvas. / データの読み書き可能状態
    // All drawing functions check this state before drawing. / 描画関数はこの情報を確認してから描画する
    public:
    enum Canvas_RW_STATE{
        READABLE, // READ OK, WRITE NG
        WRITABLE, // READ NG, WRITE OK 
    };
    protected:
    Canvas_RW_STATE rw_state;

    private:
    // A line buffer for drawing function.
    uint8_t line_buffer[ width * bytes_per_pixel ];


    //================
    // constructor / コンストラクタ
    //================
    public:
    Canvas();    
    Canvas( const Canvas<WIDTH, HEIGHT, BYTES_PER_PIXEL, Color>& src ){
        for( int n = 0; n < n_data; n++ ){
            this->data[n] = src.data[n];
        }
    };   
    

    //================
    // Functions / 関数
    //================
    // State Control
    public:
    inline void set_readable(){this->rw_state = READABLE;}
    inline bool is_readable() const {return this->rw_state == READABLE;}
    inline void set_writable(){this->rw_state = WRITABLE;}
    inline bool is_writable() const {return this->rw_state == WRITABLE;}

    // data access
    // These two function should be defined in derived class
    protected:
    virtual inline void get_Color( uint8_t *p_data, Color &color ) const = 0;
    virtual inline void set_Color( uint8_t *p_data, Color &color ) = 0;

    // data pointer
    public:
    // get the pointer of the first pixel 
    inline uint8_t* get_pointer_to_data() {return data;}
    // get the pointer of the pixel at (x,y). If the position is out of image, the position is shifted to inside of the image.
    uint8_t* get_pointer_to_data( int x, int y ) ;
    protected:
    // get the pointer of the pixel at (x,y). No range check
    inline uint8_t* get_pointer_to_data_unsafe( int x, int y )  { return (&(data[(y*width+x)*bytes_per_pixel])); }


    //---------------------
    // Drawing functions
    //---------------------
    public:
    // Set all pixel values to val
    void clear( uint8_t val = 0U );

    // Draw filled polygon, no edge / ポリゴンを塗りつぶす。ポリゴンは自動で閉じる。
    void fill_polygon( Polygon2D &polygon, Color &color, const uint8_t alpha = 0U);

    // Draw a segment, from p0 to p1 
    void draw_line( const Point2D p0, const Point2D p1, const float weight, Color &color, const uint8_t alpha = 0U);

    // Fill the pixel including the point p0
    void draw_dot( Point2D p0, Color &color, const uint8_t alpha = 0U);

    // Draw edges of polygon. The polygon is automatically closed.
    // If weight is larger than 2.0f, the draw_polygon_HQ(..) is recommended.
    // ポリゴンの辺を描画する。閉じる。高速簡易実装のため、線が太い場合はdraw_polygon_HQの方がおすすめ
    void draw_polygon( Polygon2D &polygon, const float weight, Color &color, const uint8_t alpha = 0U);

    // Draw edges of polygon. The polygon is automatically closed.
    // Because this function internaly create a new polygon representing the edges, it is relatively slow.
    // If edges are crossing, the crossing area may not be filled. 
    // ポリゴンの辺を描画する。閉じる。低速だが高品質。ただし、辺が交差したところは塗られない仕様
    void draw_polygon_HQ( Polygon2D &polygon, const float weight, Color &color, const uint8_t alpha = 0U);

    // 
    private:
    enum POLYGON_CLOSING_MODE{
        OPEN,
        CLOSE
    };
    public:
    // Draw edges of polygon.
    // If weight is larger than 2.0f, the draw_polygon_HQ(..) is recommended.
    void draw_segments( Polygon2D &polygon, const float weight, Color &color, const uint8_t alpha = 0U, const POLYGON_CLOSING_MODE oc = OPEN );

    // Draw edges of polygon.
    // Because this function internaly create a new polygon representing the edges, it is relatively slow.
    // ポリゴンの辺を描く。低速高品質版。
    void draw_segments_HQ( Polygon2D &polygon, const float weight, Color &color, const uint8_t alpha = 0U, const POLYGON_CLOSING_MODE oc = OPEN );
    
    
    inline void fill_polygon( ColoredPolygon2D &polygon ){
        fill_polygon( polygon.polygon, polygon.face_color, polygon.alpha );
    }
    inline void draw_polygon( ColoredPolygon2D &polygon, const float weight){
        draw_polygon( polygon.polygon, weight, polygon.face_color, polygon.alpha );        
    }
    // void draw_circle( const float cx, const float cy, const float radius, const float weight, Color &color, const uint8_t alpha = 0U );


    private:
    // These functions are private.
    void fill_convex_polygon( Polygon2D &convex_polygon, Color &color, const uint8_t alpha );
    void fill_not_convex_polygon( Polygon2D &polygon, Color &color, const uint8_t alpha );
    inline void alpha_blend( const Color color_org, const Color color_cur, const uint8_t alpha, Color &new_color ) const{
        for( int c = 0; c < Color::n_color; c++ ){
            new_color.color[c] = ( ( alpha * ( static_cast<color_alpha_blend_t>(color_org.color[c]) - static_cast<color_alpha_blend_t>(color_cur.color[c]) )) >> 7 ) + color_cur.color[c];
//            //antialias off
        //    if( alpha <= 64 ){
        //        new_color.color[c] = color_cur.color[c];
        //    }else{
        //        new_color.color[c] = color_org.color[c];
        //    }
        }
        return; 
    }

    // 
    public:
    bool saveBMP(std::string file_name) ;
    private:
    virtual void Color_to_RGB888( Color &color,  uint8_t &r8, uint8_t &g8, uint8_t &b8 ) const = 0;

    // [min,max]に制限
    static inline void clip_min_max( pixel_index_t &target, pixel_index_t min, pixel_index_t max ){
        if( target < min ){
            target = min;
        }else if( target > max ){
            target = max;
        }
    }

    
};


// Constructor
// Set writable / 書き込み可能状態で初期化
// All values are set to zero. / 画素値は全て0
template <unsigned int WIDTH, unsigned int HEIGHT, unsigned int BYTES_PER_PIXEL, class Color> 
Canvas<WIDTH, HEIGHT, BYTES_PER_PIXEL, Color> :: Canvas(){
    rw_state = WRITABLE;
    for( int n = 0; n < n_data; n++ ){
        data[n] = 0;
    }
}

// Drawing functions

// Set all pixel values to val
template <unsigned int WIDTH, unsigned int HEIGHT, unsigned int BYTES_PER_PIXEL, class Color> 
void Canvas<WIDTH, HEIGHT, BYTES_PER_PIXEL, Color> ::clear( uint8_t val ){
    for( int n = 0; n < n_data; n++ ){
        data[n] = val;
    }
}

// Get the pointer to the pixel value at (x,y).
// If x and/or y are out of range, they are cliped.
template <unsigned int WIDTH, unsigned int HEIGHT, unsigned int BYTES_PER_PIXEL, class Color> 
uint8_t* Canvas<WIDTH, HEIGHT, BYTES_PER_PIXEL, Color> ::get_pointer_to_data( int x, int y ) {
    if( x < 0 ) x = 0;
    if( x >= width ) x = width - 1;
    if( y < 0 ) y = 0;
    if( y >= height ) y = height - 1;
    return &(data[ ( y * height + x ) * bytes_per_pixel ]);    
}


// Draw filled polygon by the color, r, g, b, and alpha.
// If the number of points of the polygon is less than two, this function do nothing.
// 多角形を指定の色(RGBA)で塗りつぶす. 点の数が2以下の場合は何もしない。
template <unsigned int WIDTH, unsigned int HEIGHT, unsigned int BYTES_PER_PIXEL, class Color> 
void Canvas<WIDTH, HEIGHT, BYTES_PER_PIXEL, Color> ::fill_polygon( Polygon2D &polygon, Color &color, const uint8_t alpha){
    if( polygon.size() >= 3 ){
        if( polygon.is_convex_polygon() ){
            // Fast drawing for convex / 凸形状限定高速描画
            fill_convex_polygon( polygon, color, alpha );
        }else{
            // Drawing for non convex polygon / 凸以外
            fill_not_convex_polygon( polygon, color, alpha );
        }
    }
}

// this function is private and should be called by fill_polygon();
template <unsigned int WIDTH, unsigned int HEIGHT, unsigned int BYTES_PER_PIXEL, class Color> 
void Canvas<WIDTH, HEIGHT, BYTES_PER_PIXEL, Color> ::fill_not_convex_polygon( Polygon2D &polygon, Color &color, const uint8_t alpha){
    // get minimum rectangle
    pixel_index_t isx, isy, iex, iey;
    polygon.get_bounding_box(isx, isy, iex, iey);
    if(isy < 0) isy = 0;
    if(iey >= height) iey = height - 1;    

    // pixel loop
    pixel_index_t pixel_index = 0;
    for( pixel_index_t iy = isy; iy <= iey; iy++ ){
        // 高速化のため、行の中に完全に包含されるポリゴンは無視する。
        // 行の中で、外->混合->包含<-->混合<-->外と変化する。
        // 最初に混合変化する座標 sx_mix, 最後に外に出るsx_outを計算
        pixel_index_t sx_mix, sx_out;
        polygon.get_sx_mix_and_out( iy, sx_mix, sx_out );
        // 座標を画面内に制限
        clip_min_max( sx_mix, 0, width-1);
        clip_min_max( sx_out, 0, width-1 );
        // 描画
        Color org_color;
        Color new_color;
        uint8_t *ppixel = get_pointer_to_data_unsafe(sx_mix, iy);
        // 先に占有率を計算
        polygon.compute_covered_areas( iy, sx_mix, sx_out, line_buffer );
        for( int ix = sx_mix; ix <= sx_out; ix++ ){
            uint8_t total_alpha = 128 - ( 128 - alpha ) * line_buffer[ix-sx_mix] / Polygon2D::n_subpixels;
            //fill_pixel( ppixel, r, g, b, total_alpha );
            get_Color( ppixel, org_color );
            alpha_blend( org_color, color, total_alpha, new_color );
            set_Color( ppixel, new_color );
            ppixel += bytes_per_pixel;
        }
    }
}

// this function is private and should be called by fill_polygon();
// 凸多角形に限定して高速に描画する関数
// 凸多角形でない場合は、意図した動作をしない
template <unsigned int WIDTH, unsigned int HEIGHT, unsigned int BYTES_PER_PIXEL, class Color> 
void Canvas<WIDTH, HEIGHT, BYTES_PER_PIXEL, Color> ::fill_convex_polygon( Polygon2D &convex_polygon, Color &color, const uint8_t alpha ){
    // get minimum rectangle
    pixel_index_t isx, isy, iex, iey;
    convex_polygon.get_bounding_box(isx, isy, iex, iey);
    if(isy < 0) isy = 0;
    if(iey >= this->height) iey = this->height - 1;    
    
    // row loop
    for( pixel_index_t iy = isy; iy <= iey; iy++ ){
        // 行の中で、外->混合->包含->混合->外と変化する。
        // 変化する座標 sx_mix_0, sx_inc, sx_min_1, sx_out1を計算
        pixel_index_t sx_mix_0, sx_inc, sx_mix_1, sx_out1;
        convex_polygon.get_start_x_of_the_areas( iy, sx_mix_0, sx_inc, sx_mix_1, sx_out1 );
        clip_min_max( sx_mix_0, 0, width-1 );
        clip_min_max( sx_inc, 0, width-1 );
        clip_min_max( sx_mix_1, 0, width-1 );
        clip_min_max( sx_out1, 0, width-1 );
        // 左側混合領域 (面積判定と描画)
        uint8_t *ppixel  = this->get_pointer_to_data_unsafe( sx_mix_0, iy );
        //&(this->data[ ( iy * width + sx_mix_0 ) * bytes_per_pixel ]); 

        // 高速化のため、ポリゴンが画素を覆っている面積を1行分計算してから色を処理する。
        convex_polygon.compute_covered_areas( iy, sx_mix_0, sx_inc-1, line_buffer );
        Color org_color;
        Color new_color;
        for( pixel_index_t ix = sx_mix_0; ix < sx_inc; ix++ ){
            uint8_t total_alpha = 128 - ( 128 - alpha ) * line_buffer[ix-sx_mix_0] / Polygon2D::n_subpixels;
            get_Color( ppixel, org_color );
            alpha_blend( org_color, color, total_alpha, new_color );
            set_Color( ppixel, new_color );
            ppixel += bytes_per_pixel;
        }
        // 包含領域 (塗りつぶし)
        for( pixel_index_t ix = sx_inc; ix < sx_mix_1; ix++ ){
            get_Color( ppixel, org_color );
            alpha_blend( org_color, color, alpha, new_color );
            set_Color( ppixel, new_color );
            ppixel += bytes_per_pixel;
        }
        // 右混合領域 (面積判定と描画)
        // 高速化のため、ポリゴンが画素を覆っている面積を1行分計算してから色を処理する。
        convex_polygon.compute_covered_areas( iy, sx_mix_1, sx_out1, line_buffer );
        for( pixel_index_t ix = sx_mix_1; ix <= sx_out1; ix++ ){
            uint8_t total_alpha = 128 - ( 128 - alpha ) * line_buffer[ix-sx_mix_1] / Polygon2D::n_subpixels;
            get_Color( ppixel, org_color );
            alpha_blend( org_color, color, total_alpha, new_color );
            set_Color( ppixel, new_color );
            ppixel += bytes_per_pixel;
        }
    }
}

// fill the pixel including the point p0. / 点p0が含まれる画素を塗りつぶす。
template <unsigned int WIDTH, unsigned int HEIGHT, unsigned int BYTES_PER_PIXEL, class Color> 
void Canvas<WIDTH, HEIGHT, BYTES_PER_PIXEL, Color> ::draw_dot( Point2D p0, Color &color, const uint8_t alpha){
    pixel_index_t iy = (p0.y+half_internal_scale)/internal_scale;
    pixel_index_t ix = (p0.x+half_internal_scale)/internal_scale;
    if( 0 <= ix && ix < width && 0<= iy && iy < height ){
        Color org_color;
        Color new_color;
        uint8_t* ppixel = &data[iy*width+ix];
        get_Color( ppixel, org_color );
        alpha_blend( org_color, color, alpha, new_color );
        set_Color( ppixel, new_color );
    }
}

template <unsigned int WIDTH, unsigned int HEIGHT, unsigned int BYTES_PER_PIXEL, class Color> 
void Canvas<WIDTH, HEIGHT, BYTES_PER_PIXEL, Color> ::draw_line( const Point2D p0, const Point2D p1, const float weight, Color &color, const uint8_t alpha){
    // 長方形polygon作成
    Polygon2D line_segment;
    line_segment.line_segment( p0, p1, weight );
    fill_convex_polygon(line_segment, color, alpha);
}

template <unsigned int WIDTH, unsigned int HEIGHT, unsigned int BYTES_PER_PIXEL, class Color> 
void Canvas<WIDTH, HEIGHT, BYTES_PER_PIXEL, Color> ::draw_polygon( Polygon2D &polygon, const float weight, Color &color, const uint8_t alpha){
    draw_segments( polygon, weight, color, alpha, CLOSE );
}

// 塗りつぶしなしポリゴン。閉じる。低速高品質版
// 高品質版:線の幅を考慮して、交点をきちんと算出する。
template <unsigned int WIDTH, unsigned int HEIGHT, unsigned int BYTES_PER_PIXEL, class Color> 
void Canvas<WIDTH, HEIGHT, BYTES_PER_PIXEL, Color> ::draw_polygon_HQ( Polygon2D &polygon, const float weight, Color &color, const uint8_t alpha ){
    draw_segments_HQ( polygon, weight, color, alpha, CLOSE );
}


template <unsigned int WIDTH, unsigned int HEIGHT, unsigned int BYTES_PER_PIXEL, class Color> 
void Canvas<WIDTH, HEIGHT, BYTES_PER_PIXEL, Color> ::draw_segments( Polygon2D &polygon, const float weight, Color &color, const uint8_t alpha, const POLYGON_CLOSING_MODE oc ){
    // 各辺をdrawLineという簡易実装
    int np = polygon.size();
    Point2D p0 = polygon.get_Point2D(0);
    for( int n = 1; n < np; n++ ){
        Point2D p1 = polygon.get_Point2D(n);
        draw_line( p0, p1, weight, color, alpha );
        p0 = p1;
    }
    if( oc == CLOSE ){
        Point2D p1 = polygon.get_Point2D(0);
        draw_line( p0, p1, weight, color, alpha );
    }
}


template <unsigned int WIDTH, unsigned int HEIGHT, unsigned int BYTES_PER_PIXEL, class Color> 
void Canvas<WIDTH, HEIGHT, BYTES_PER_PIXEL, Color> ::draw_segments_HQ( Polygon2D &polygon, const float weight, Color &color, const uint8_t alpha, const POLYGON_CLOSING_MODE oc ){
    int np = polygon.size();
    if( np <= 1 ){
        // do nothing
    }else if( np == 2 ){
        // draw a segment
        draw_line(polygon.get_Point2D(0), polygon.get_Point2D(1), weight, color, alpha );
    }else{
        // 線分(p0,p1)と、(p1,p2)の交点を求める
        Point2D p0, p1, p2, p0i, p0o, p1i, p1o;
        Polygon2D leftside_points;
        Polygon2D rightside_points;
        float length_internal = weight * internal_scale * 0.5f;
        if( oc == CLOSE ){
            p0 = polygon.get_Point2D(np-1); // previous vertex
            p1 = polygon.get_Point2D(0);    // current vertex
            p2 = polygon.get_Point2D(1);    // next vertex
            for( int n = 1; n <= np+1; n++ ){
                // 交点算出
                coordinate_t x01 = p0.x - p1.x; // internal scale
                coordinate_t y01 = p0.y - p1.y;
                coordinate_t x12 = p1.x - p2.x;
                coordinate_t y12 = p1.y - p2.y;
                //float r01 = sqrt( x01 * x01 + y01 * y01 );
                //float r12 = sqrt( x12 * x12 + y12 * y12 );
                //float length = r01 * r12 / abs( x01 * y12 - y01 * x12 ) * weight * internal_scale * 0.5f;
                //Point2D p1_temp = (Point2D( x01, y01 ) / r01 - Point2D( x12, y12 ) / r12 ) * length;
                float length_factor = length_internal / fabs( x01 * y12 - y01 * x12 );
                float r01 = sqrt( x01 * x01 + y01 * y01 ) * length_factor;
                float r12 = sqrt( x12 * x12 + y12 * y12 ) * length_factor;
                Point2D p1_temp = Point2D( x01 * r12, y01 * r12, true )  - Point2D( x12 * r01, y12 * r01, true );
                // 左右判定
                if( -x01 * p1_temp.y + y01 * p1_temp.x >= 0 ){
                    rightside_points.add_Point2D(p1 + p1_temp);
                    //rightside_points.add_Point2D(p1 );
                    leftside_points.add_Point2D(p1 - p1_temp);
                }else{
                    rightside_points.add_Point2D(p1 - p1_temp);
                    //rightside_points.add_Point2D(p1 );
                    leftside_points.add_Point2D(p1 + p1_temp);
                }
                p0 = p1;
                p1 = p2;
                p2 = polygon.get_Point2D((n+1)%np);
            }
            rightside_points.concat(leftside_points);
            fill_not_convex_polygon( rightside_points, color, alpha );
        }else{ // OPEN
            Point2D p0, p1, p2, p0i, p0o, p1i, p1o;
            Polygon2D edge;

            p0 = polygon.get_Point2D(0); // previous vertex
            p1 = polygon.get_Point2D(1);    // current vertex
            p2 = polygon.get_Point2D(2);    // next vertex

            Point2D n01 = p1 - p0;
            n01.normalize();
            Point2D v(-n01.y, n01.x);
            v = v * (weight*internal_scale/2);
            if( v.x * n01.y - v.y * n01.x <= 0 ){
                rightside_points.add_Point2D(p0 + v);
                leftside_points.add_Point2D(p0 - v);
            }else{
                rightside_points.add_Point2D(p0 - v);
                leftside_points.add_Point2D(p0 + v);
            }

            for( int n = 2; n < np; n++ ){
                // 交点算出
                coordinate_t x01 = p0.x - p1.x;
                coordinate_t y01 = p0.y - p1.y;
                coordinate_t x12 = p1.x - p2.x;
                coordinate_t y12 = p1.y - p2.y;
                float length_factor = length_internal / fabs( x01 * y12 - y01 * x12 );
                float r01 = sqrt( x01 * x01 + y01 * y01 ) * length_factor;
                float r12 = sqrt( x12 * x12 + y12 * y12 ) * length_factor;
                Point2D p1_temp = Point2D( x01 * r12, y01 * r12, true )  - Point2D( x12 * r01, y12 * r01, true );
                // 左右判定
                if( -x01 * p1_temp.y + y01 * p1_temp.x >= 0 ){
                    rightside_points.add_Point2D(p1 + p1_temp);
                    leftside_points.add_Point2D(p1 - p1_temp);
                }else{
                    rightside_points.add_Point2D(p1 - p1_temp);
                    leftside_points.add_Point2D(p1 + p1_temp);
                }
                p0 = p1;
                p1 = p2;
                p2 = polygon.get_Point2D((n+1)%np);
            }
            n01 = p1 - p0;
            n01.normalize();
            v = Point2D(-n01.y, n01.x, true);
            v = v * (weight*internal_scale/2);
            if( v.x * n01.y - v.y * n01.x <= 0 ){
                rightside_points.add_Point2D(p1 + v);
                leftside_points.add_Point2D(p1 - v);
            }else{
                rightside_points.add_Point2D(p1 - v);
                leftside_points.add_Point2D(p1 + v);
            }
            rightside_points.concat_inversely(leftside_points);
            fill_not_convex_polygon( rightside_points, color, alpha );

        }
    }
}
/*
void draw_circle( const float cx, const float cy, const float radius, const float weight, Color &color, const uint8_t alpha = 0U ){

}
*/


template <unsigned int WIDTH, unsigned int HEIGHT, unsigned int BYTES_PER_PIXEL, class Color> 
bool Canvas<WIDTH, HEIGHT, BYTES_PER_PIXEL, Color> ::saveBMP(std::string file_name){

#ifndef ESP32
    std::ofstream fout( file_name, std::ios::binary );
    if(!fout){return false;}
    // BITMAPFILEHEADER 14 bytes
    char bfType[2] = {'B','M'};
    fout.write(bfType,2);
    unsigned int file_size = width * height * 3 + 54;
    fout.write(reinterpret_cast<char *>(&file_size),4); // bfSize
    unsigned short s_zero = 0;
    fout.write(reinterpret_cast<char *>(&s_zero),2); // bfReserved1
    fout.write(reinterpret_cast<char *>(&s_zero),2); // bfReserved2
    unsigned int bfOffBits = 54;
    fout.write(reinterpret_cast<char *>(&bfOffBits),4); // bfOffBits
    // BITMAPINFOHEADER 40 bytes
    unsigned int bcSize = 40;
    fout.write(reinterpret_cast<char *>(&bcSize),4); // bcSize
    unsigned int bcWidth = width;
    int bcHeight = -height;
    fout.write(reinterpret_cast<char *>(&bcWidth),4); // bcWidth
    fout.write(reinterpret_cast<char *>(&bcHeight),4); // bcHeight
    unsigned short bcPlanes = 1;
    fout.write(reinterpret_cast<char *>(&bcPlanes),2); // bcPlanes
    unsigned short bcBitCount = 24;
    fout.write(reinterpret_cast<char *>(&bcBitCount),2); // bcBitCount
    unsigned int biCompression = 0;
    fout.write(reinterpret_cast<char *>(&biCompression),4); // biCompression
    unsigned int biSizeImage = 0;
    fout.write(reinterpret_cast<char *>(&biSizeImage),4); // biSizeImage
    unsigned int biPixPerMeter = 4767;
    fout.write(reinterpret_cast<char *>(&biPixPerMeter),4); // biXPixPerMeter
    fout.write(reinterpret_cast<char *>(&biPixPerMeter),4); // biYPixPerMeter
    unsigned int i_zero = 0;
    fout.write(reinterpret_cast<char *>(&i_zero),4); // biCompression
    fout.write(reinterpret_cast<char *>(&i_zero),4); // biCompression

    // DATA
    Color color;     // original data
    uint8_t r8, g8, b8;  // converted data
    for( int h = 0; h < height; h++ ){
        uint8_t* p_data = get_pointer_to_data_unsafe( 0, h );
        for( int w = 0; w < width; w++ ){
            get_Color( p_data, color );
            Color_to_RGB888( color, r8, g8, b8 );
            fout.write(reinterpret_cast<char *>(&b8),1); // 
            fout.write(reinterpret_cast<char *>(&g8),1); // 
            fout.write(reinterpret_cast<char *>(&r8),1); // 
            p_data += bytes_per_pixel;
        }
        // padding 
        int n_padding = (( 4 - (3 * width) & 0x3 ) & 0x3);
        char dummy = 0;
        for( int n = 0; n < n_padding; n++ ){
            fout.write( &dummy, 1 ); // 
        }
    }    
    fout.close();

#endif
    return true;
}

#endif