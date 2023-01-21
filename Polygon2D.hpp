#ifndef __POLYGON2D_HPP__
#define __POLYGON2D_HPP__
/*==============================================================//
class Polygon2D 
    2D polygon data class / ベクトル描画向け  2次元ポリゴンのデータクラス 
//==============================================================*/
#include "resolution.hpp"
#include "Point2D.hpp"
#include <vector>

class Polygon2D{

    //================
    // data
    //================
    private:
    std::vector<Point2D> vertices;
    // bounding box
    coordinate_t minX;
    coordinate_t maxX;
    coordinate_t minY;
    coordinate_t maxY;
    // convex or not
    bool is_convex; 
    int sign_of_outer_product;    // used to determine convex or not

    //================
    // constructor / コンストラクタ
    //================
    public:
    Polygon2D();

    //================
    // constructor / コンストラクタ
    //================
    // methods
    void clear(); // 初期化

    //================
    // Functions / 関数
    //================
    private:
    // 凸形状かを判定する際に使用するサブ関数
    // 点の順序はindex0->index1->index2の順
    // 渡されるindexのチェックは省くので、呼び出し側が注意
    void check_convex( const uint16_t index0, const uint16_t index1, const uint16_t index2 );

    // 点(x,y)から右に伸ばした半直線が、線分[p0,p1)と交差するかどうかを判定
    // 交差する時、x_cross_pointのポインタが渡されていればそのx座標をx_cross_pointに代入。NULLなら何もしない
    // 計算量削減のため、p0.yやp1.yがyと一致した場合は、0.005程度p0やp1をシフトする。
    bool is_crossing( const coordinate_t x, const coordinate_t y, const Point2D &p0, const Point2D &p1, coordinate_t *x_cross_point ) const;

    //================
    // Public Functions / 関数
    //================
    public:
    // Functions to define the polygon
    // 点の追加。Polygonの定義は必ずこれで行う。追加時に凸判定も行う
    void add_Point2D( const Point2D p );
    void add_Point2D( const float x, const float y ); // 中で(Point2Dで)internal scale倍

    // Basic Shapes
    void rectangle( Point2D p0, Point2D p1 );  //
    void line_segment( Point2D p0, Point2D p1, float weight );
    // ほぼ円(正24角形)
    void circle24( Point2D center, float raduis ); // radiusは中でinternal scale倍
    
    // Derive a new shape based on this
    //Polygon2D frame( float weight );
    
    
    // indexの点を返す。
    Point2D get_Point2D(const uint16_t index) const;
    inline uint16_t size(){return this->vertices.size();} 
    //
    bool is_convex_polygon() const{return this->is_convex;}
    bool is_the_point_inside(const coordinate_t x, const coordinate_t y, coordinate_t &first_crossing_point_X ) const;
    
    // 画素の中でポリゴンに含まれている面積を返す。5x5に分割して返す近似。
    float compute_covered_area(const pixel_index_t ix, pixel_index_t iy) const;
    static const uint8_t n_divides = COODINATES_RESOLUTION;
    static const uint8_t n_subpixels = n_divides * n_divides;
    void compute_covered_areas(const pixel_index_t iy, const pixel_index_t start_x, const pixel_index_t end_x, uint8_t *areas ) const;
    // バウンディングボックス。余白なし
    void get_bounding_box( pixel_index_t &isx, pixel_index_t &isy, pixel_index_t &iex, pixel_index_t &iey) const;
    void get_sx_mix_and_out( const pixel_index_t iy, pixel_index_t &sx_mix, pixel_index_t &sx_out ) const; // for fill_polygon
    void get_start_x_of_the_areas( const pixel_index_t iy, pixel_index_t &sx_mix_0, pixel_index_t &sx_inc, pixel_index_t &sx_mix_1, pixel_index_t &sx_out1 ) const; // for fill_convex_polygon

    // operators
    public:
    Polygon2D & operator = (const Polygon2D p);
    Polygon2D & operator += (const Point2D p);
    Polygon2D & operator -= (const Point2D p);
    Polygon2D & operator *= (const float f);
    Polygon2D & operator /= (const float f);
    Polygon2D operator + (const Point2D p) const;
    Polygon2D operator - (const Point2D p) const;
    Polygon2D operator * (const float f) const;
    Polygon2D operator / (const float f) const;

    Polygon2D rotate( const float deg ) const; 
    Polygon2D rotate( const float deg, Point2D center ) const; 
    Polygon2D & rotate_equal( const float deg ); 
    Polygon2D & rotate_equal( const float deg, Point2D center ); 
    Polygon2D & rotate_equal( const float cos_theta, const float sin_theta ); 

    void concat( const Polygon2D p );
    void concat_inversely( const Polygon2D p );
    
    void print() const;


    private:
#ifdef USE_SINGLE_PRECISION_FLOATING_COORDINATES
    static constexpr coordinate_t init_pos = -0.5f + 0.5f / (COODINATES_RESOLUTION);
    static constexpr coordinate_t delta_pos = 1.0f / (COODINATES_RESOLUTION);
#else
    static const coordinate_t init_pos = (-internal_scale + internal_scale / (COODINATES_RESOLUTION))/2;
    static const coordinate_t delta_pos = internal_scale / (COODINATES_RESOLUTION);
#endif    
};
// __POLYGON2D_HPP__
#endif