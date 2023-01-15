#include "Polygon2D.hpp"
#include <cmath>

#include "debug_functions.hpp"

Polygon2D::Polygon2D(){
    // nothing
    // is_convexは、点の追加時や図形定義時点で代入する。
}

// 凸形状かを判定する際に使用するサブ関数
// 点の順序はindex0->index1->index2の順
// 渡されるindexのチェックは省くので、呼び出し側が注意
void Polygon2D::check_convex( const uint16_t index0, const uint16_t index1, const uint16_t index2 ){
    Point2D v0 = this->vertices[index1] - this->vertices[index0];
    Point2D v1 = this->vertices[index2] - this->vertices[index1];
    coordinate_sq_t outer_product = v0.x * v1.y - v0.y * v1.x;
    if( outer_product > 0 && this->sign_of_outer_product != 1){
        this->sign_of_outer_product = 0;
        this->is_convex = false;
    }else if( outer_product < 0 && this->sign_of_outer_product != -1){
        this->sign_of_outer_product = 0;
        this->is_convex = false;
    }
}

// 初期化
// 
void Polygon2D::clear(){
    this->is_convex = false;
    this->vertices.clear();
}

// 点の追加。bounding boxと、is_convexの更新
void Polygon2D::add_Point2D( const float x, const float y ){
    this->add_Point2D(Point2D(x,y));
}

void Polygon2D::add_Point2D( const Point2D p ){

    uint16_t n_points = this->vertices.size();

    // bounding box
    if( n_points == 0 ){
        this->minX = p.x;
        this->maxX = p.x;
        this->minY = p.y;
        this->maxY = p.y;
    }else{
        if( this->minX > p.x ){
            this->minX = p.x;
        }else if( this->maxX < p.x ){
            this->maxX = p.x;        
        }
        if( this->minY > p.y ){
            this->minY = p.y;
        }else if( this->maxY < p.y ){
            this->maxY = p.y;
        }
    }
    // 追加
    this->vertices.push_back(p);
    // 追加後の数
    n_points = this->vertices.size();
    // 凸判定
    // 3点目が追加された時：
    //  外積を計算してその符号から、右回りか左回りかを判定。外積が0だと直線
    //  三角形は、直線に並んでいなければ常に凸なので、is_convexはtrue
    //  直線に並んでいる時は凸の可能性は残されているが、計算量観点から凸ではないとする。
    // 4点目以降が追加された時：
    //  すでに凸でない部分があれば、判定が覆ることがないためスキップする。
    //  N点目が追加された時、以下の3つで外積の継続性を判定
    //  {N-2, N-1, N}, {N-1, N, 0}, {N-1, 0, 1}
    if( n_points == 3 ){
        Point2D v0 = this->vertices[1] - this->vertices[0];
        Point2D v1 = this->vertices[2] - this->vertices[1];
        coordinate_sq_t outer_product = v0.x * v1.y - v0.y * v1.x;
        if( outer_product > 0 ){
            this->sign_of_outer_product = 1;
            this->is_convex = true;
        }else if( outer_product < 0 ){
            this->sign_of_outer_product = -1;
            this->is_convex = true;
        }else{
            this->sign_of_outer_product = 0;
            this->is_convex = false;
        }
    }else if( n_points > 3 ){
        if( this->is_convex ){
            check_convex( n_points-3, n_points-2, n_points-1 );
        }
        if( this->is_convex ){
            check_convex( n_points-2, n_points-1, 0 );
        }
        if( this->is_convex ){
            check_convex( n_points-1, 0, 1 );
        }
    }
}

// ほぼ円(正24角形)
void Polygon2D::circle24( Point2D center, float raduis ){
    clear();
    add_Point2D(Point2D( 1.000f,  0.000f));
    add_Point2D(Point2D( 0.966f,  0.259f));
    add_Point2D(Point2D( 0.866f,  0.500f));
    add_Point2D(Point2D( 0.707f,  0.707f));
    add_Point2D(Point2D( 0.500f,  0.866f));
    add_Point2D(Point2D( 0.259f,  0.966f));
    add_Point2D(Point2D( 0.000f,  1.000f));
    add_Point2D(Point2D(-0.259f,  0.966f));
    add_Point2D(Point2D(-0.500f,  0.866f));
    add_Point2D(Point2D(-0.707f,  0.707f));
    add_Point2D(Point2D(-0.866f,  0.500f));
    add_Point2D(Point2D(-0.966f,  0.259f));
    add_Point2D(Point2D(-1.000f,  0.000f));
    add_Point2D(Point2D(-0.966f, -0.259f));
    add_Point2D(Point2D(-0.866f, -0.500f));
    add_Point2D(Point2D(-0.707f, -0.707f));
    add_Point2D(Point2D(-0.500f, -0.866f));
    add_Point2D(Point2D(-0.259f, -0.966f));
    add_Point2D(Point2D(-0.000f, -1.000f));
    add_Point2D(Point2D( 0.259f, -0.966f));
    add_Point2D(Point2D( 0.500f, -0.866f));
    add_Point2D(Point2D( 0.707f, -0.707f));
    add_Point2D(Point2D( 0.866f, -0.500f));
    add_Point2D(Point2D( 0.966f, -0.259f));
    *this *= (raduis * internal_scale);
    *this += center;
}


// 指定したindexのPoint2Dオブジェクトを返す。
// indexが負なら先頭を、indexがオブジェクト数を超えていたら最後のオブジェクトを返す
Point2D Polygon2D::get_Point2D(const uint16_t index) const{
    if( index < 0 ) return vertices[0];
    if( index >= this->vertices.size() ) return vertices[this->vertices.size() - 1];
    return vertices[index];
}

// 点(x,y)から右に伸ばした半直線が、線分[p0,p1)と交差するかどうかを判定
// 交差する時はそのx座標をx_cross_pointに代入。
// 交差しない時は未定
// 計算量削減のため、p0.yあるいはp1.yがyと同じ場合は、y座標を少しずらす
bool Polygon2D::is_crossing( const coordinate_t x, const coordinate_t y, const Point2D &p0, const Point2D &p1, coordinate_t *x_cross_point ) const{

    // わかりやすいケースから早期リターン
    // case 0
    // xが、p0.xとp1.xよりも大きければそもそも右側にないので交差しない
    if(( p0.x < x ) && ( p1.x < x )){
        return false;
    }

    // 水平線は、この後の計算量削減処理によって絶対交差しない
    if( p0.y == p1.y ){
        return false;
    }

    // 計算量削減のため、p0.yやp1.yがyと一致するときは、一律少しずらす。
    float y0 = p0.y;
    float y1 = p1.y;
#ifdef USE_SINGLE_PRECISION_FLOATING_COORDINATES
    if( y0 == y ) y0 += 0.005; 
    if( y1 == y ) y1 += 0.005; 
#else
    if( y0 == y ) y0 += 1; 
    if( y1 == y ) y1 += 1; 
#endif
    // case 1
    // y座標が2点の間になければ交差しない
    if( ( y0 - y ) * ( y1 - y) > 0 ){
        return false;
    }

    // 交点算出
    coordinate_t x_cross_point_temp = ((p1.x - p0.x) * (y - y0)) / (y1 - y0) + p0.x; 
    if( x_cross_point != NULL ){
        *x_cross_point = x_cross_point_temp;
    }
    // 交点のx座標が2点の間にあるか？
    // 交点が右側にあるか?
    if( x <= x_cross_point_temp ){
        return true;
    }else{
        return false;
    }
}

// 点x, yがポリゴンの中かエッジ上にある場合にtrue, ない時はfalse
// 呼び出し側の計算量削減のため、最も左にある交差点のx座標を代入する
bool Polygon2D::is_the_point_inside(const coordinate_t x, const coordinate_t y, coordinate_t &first_crossing_point_X ) const{
    bool ret_val = false;
    Point2D p0 = this->vertices[0];
    int np = this->vertices.size();
    coordinate_t crossing_point_X;
    for(int n = 1; n < np; n++){
        Point2D p1 = this->vertices[n];
        if( is_crossing( x, y, p0, p1, &crossing_point_X ) ){
            ret_val = !ret_val;
            if( first_crossing_point_X > crossing_point_X ) first_crossing_point_X = crossing_point_X;
        }
        p0 = p1;
    }
    {
        Point2D p1 = this->vertices[0];
        if( is_crossing( x, y, p0, p1, &crossing_point_X ) ){
            ret_val = !ret_val;
            if( first_crossing_point_X > crossing_point_X ) first_crossing_point_X = crossing_point_X;
        }
    }

    return ret_val;
}

/*
float Polygon2D::compute_covered_area(const pixel_index_t ix, const pixel_index_t iy ) const{
    int count = 0;
    const int resol = 5;
    const float n_inv = 0.04f;
    for(int j = 0; j < resol; j++){
        float y = iy - 0.4f + 0.2f * j;
        float first_cross_point_X = ix + 1;
        bool first_judge = true;
        float former_result = false;
        for( int i = 0; i < resol; i++){
            float x = ix - 0.4f + 0.2f * i;
            // 最初の交点が調べたい点よりも右側の場合は、判定が変わらずし直す必要がないので、チェック
            if( x <= first_cross_point_X && first_judge == false ){
                // 再判定必要なし。
                if( former_result ){
                    count++;
                }
            }else{
                // 再判定必要あり
                former_result = is_the_point_inside(x,y, first_cross_point_X);
                first_judge = false;
                if( former_result ){
                    count++;
                }
            }
        }
    }
    return count * n_inv;
}
*/

void Polygon2D::compute_covered_areas(const pixel_index_t iy, const pixel_index_t start_x, const pixel_index_t end_x, uint8_t *areas ) const{

    // zero clear the areas
    for( pixel_index_t ix = start_x; ix <= end_x; ix++ ){
        areas[(ix-start_x)] = 0U;        
    }


    for(int j = 0; j < n_divides; j++){
        coordinate_t y = iy * internal_scale + init_pos + delta_pos * j;
        coordinate_t first_cross_point_X = end_x * internal_scale + 1;
        bool first_judge = true;
        bool former_result = false;

        for( pixel_index_t ix = start_x; ix <= end_x; ix++ ){
            for( int i = 0; i < n_divides; i++){
                coordinate_t x = ix * internal_scale +  init_pos + delta_pos * i;
                // 交点が左側にある時は調べ直す必要があるので、交点をリセットしてフラグを立てる。
                if( x > first_cross_point_X ){
                    first_cross_point_X = end_x * internal_scale + 1;
                    first_judge = true;
                }
                // 最初の交点が調べたい点よりも右側の場合は、判定が変わらずし直す必要がないので、チェック
                if( x <= first_cross_point_X && first_judge == false ){
                //if( x <= first_cross_point_X ){
                    // 再判定必要なし。
                    if( former_result ){
                        areas[ix-start_x]++;
                    }
                }else{
                    // 再判定必要あり
                    former_result = is_the_point_inside(x,y, first_cross_point_X);
                    first_judge = false;
                    if( former_result ){
                        areas[ix-start_x]++;
                    }
                }
            }
        }
    }
    return;
}


// 余白のないbbox, 画面外も返す
void Polygon2D::get_bounding_box( pixel_index_t &isx, pixel_index_t &isy, pixel_index_t &iex, pixel_index_t &iey) const{
#ifdef USE_SINGLE_PRECISION_FLOATING_COORDINATES
    isx = floor(minX+0.5f);
    isy = floor(minY+0.5f);
    iex = ceil(maxX-0.5f);
    iey = ceil(maxY-0.5f);
#else
    isx = (minX+half_internal_scale)/internal_scale;
    isy = (minY+half_internal_scale)/internal_scale;
    iex = (maxX+half_internal_scale-1)/internal_scale;
    iey = (maxY+half_internal_scale-1)/internal_scale;
#endif
}

// for fill_polygon
// 行の中で、外->混合->包含<-->混合->外と変化する。
// 最初に変化する座標 sx_mixと、最後に変化するsx_outを計算
void Polygon2D::get_sx_mix_and_out( const pixel_index_t iy, pixel_index_t &sx_mix, pixel_index_t &sx_out ) const{

    // y-0.5fと、y+0.5fの２つで調べる
    // floatで座標を求めておき、floorとceil
    int np = this->vertices.size();
    coordinate_t y = iy * internal_scale;
    // 初期化
    coordinate_t sx_mix_temp = this->maxX - internal_scale;
    coordinate_t sx_out_temp = this->minX + internal_scale;
    Point2D p0 = this->vertices[0];
    for( int n = 1; n < np ; n++ ){
        Point2D p1 = this->vertices[n];
        coordinate_t x_crossing_point;
        if( is_crossing( this->minX-internal_scale, y-half_internal_scale, p0, p1, &x_crossing_point ) ){
            if( sx_mix_temp > x_crossing_point ) sx_mix_temp = x_crossing_point;
            if( sx_out_temp < x_crossing_point ) sx_out_temp = x_crossing_point;
        }
        if( is_crossing( this->minX-internal_scale, y+half_internal_scale, p0, p1, &x_crossing_point ) ){
            if( sx_mix_temp > x_crossing_point ) sx_mix_temp = x_crossing_point;
            if( sx_out_temp < x_crossing_point ) sx_out_temp = x_crossing_point;
        }
        p0 = p1;
    }
    {
        Point2D p1 = this->vertices[0];
        coordinate_t x_crossing_point;
        if( is_crossing( this->minX-internal_scale, y-half_internal_scale, p0, p1, &x_crossing_point ) ){
            if( sx_mix_temp > x_crossing_point ) sx_mix_temp = x_crossing_point;
            if( sx_out_temp < x_crossing_point ) sx_out_temp = x_crossing_point;
        }
        if( is_crossing( this->minX-internal_scale, y+half_internal_scale, p0, p1, &x_crossing_point ) ){
            if( sx_mix_temp > x_crossing_point ) sx_mix_temp = x_crossing_point;
            if( sx_out_temp < x_crossing_point ) sx_out_temp = x_crossing_point;
        }
    }

#ifdef USE_SINGLE_PRECISION_FLOATING_COORDINATES
    sx_mix = floor( sx_mix_temp +0.5 );
    sx_out = ceil( sx_out_temp +0.5 );
#else
    sx_mix = (sx_mix_temp + half_internal_scale)/internal_scale;
    sx_out = (sx_out_temp + half_internal_scale-1)/internal_scale + 1;
#endif


}

// for fill_convex_polygon
// 行の中で、外->混合->包含->混合->外と変化する。
// 場合によっては、外->混合->外と変化する。
// 変化する座標 sx_mix_0, sx_inc, sx_min_1, sx_out1を計算
void Polygon2D::get_start_x_of_the_areas( const pixel_index_t iy, pixel_index_t &sx_mix_0, pixel_index_t &sx_inc, pixel_index_t &sx_mix_1, pixel_index_t &sx_out_1 ) const{
    
    // y-0.5fと、y+0.5fの２つで調べる
    // floatで座標を求めておき、floorとceil
    int np = this->vertices.size();
    coordinate_t y = iy * internal_scale;
    // 初期化
    coordinate_t sx_mix0_temp = this->maxX + internal_scale; // for y - 0.5
    coordinate_t sx_out0_temp = this->minX - internal_scale; // for y - 0.5
    coordinate_t sx_mix1_temp = this->maxX + internal_scale; // for y + 0.5
    coordinate_t sx_out1_temp = this->minX - internal_scale; // for y + 0.5
    Point2D p0 = this->vertices[0];
    for( int n = 1; n < np ; n++ ){
        Point2D p1 = this->vertices[n]; // あえて n%npにしない。
        coordinate_t x_crossing_point;
        if( is_crossing( this->minX-internal_scale, y - half_internal_scale, p0, p1, &x_crossing_point ) ){
            if( sx_mix0_temp > x_crossing_point ) sx_mix0_temp = x_crossing_point;
            if( sx_out0_temp < x_crossing_point ) sx_out0_temp = x_crossing_point;
        }
        if( is_crossing( this->minX-internal_scale, y + half_internal_scale, p0, p1, &x_crossing_point ) ){
            if( sx_mix1_temp > x_crossing_point ) sx_mix1_temp = x_crossing_point;
            if( sx_out1_temp < x_crossing_point ) sx_out1_temp = x_crossing_point;
        }
        p0 = p1;
    }
    {
        Point2D p1 = this->vertices[0];
        coordinate_t x_crossing_point;
        if( is_crossing( this->minX-internal_scale, y - half_internal_scale, p0, p1, &x_crossing_point ) ){
            if( sx_mix0_temp > x_crossing_point ) sx_mix0_temp = x_crossing_point;
            if( sx_out0_temp < x_crossing_point ) sx_out0_temp = x_crossing_point;
        }
        if( is_crossing( this->minX-internal_scale, y + half_internal_scale, p0, p1, &x_crossing_point ) ){
            if( sx_mix1_temp > x_crossing_point ) sx_mix1_temp = x_crossing_point;
            if( sx_out1_temp < x_crossing_point ) sx_out1_temp = x_crossing_point;
        }

    }
//std::cout << "DEBUG::y-m0-o0-m1-o1:" << y << " " << sx_mix0_temp << " " << sx_out0_temp << " " << sx_mix1_temp << " " << sx_out1_temp << std::endl;
    if( sx_mix0_temp > sx_out0_temp && sx_mix1_temp > sx_out1_temp ){
        // Intra line polygon
#ifdef USE_SINGLE_PRECISION_FLOATING_COORDINATES
        sx_mix_0 = floor(this->minX);
        sx_inc = ceil(this->maxX);
        sx_mix_1 = sx_inc;
        sx_out_1 = sx_inc;
#else
        sx_mix_0 = (this->minX )/internal_scale;
        sx_inc = (this->maxX + internal_scale - 1 )/internal_scale;
        sx_mix_1 = sx_inc;
        sx_out_1 = sx_inc;
#endif        
        return;
    }
    if( sx_mix0_temp > sx_out0_temp){
        // out->mix->out
#ifdef USE_SINGLE_PRECISION_FLOATING_COORDINATES
        sx_mix_0 = floor(sx_mix1_temp+0.5f);
        sx_inc = sx_mix_0;
        sx_mix_1 = sx_mix_0;
        sx_out_1 = ceil(sx_out1_temp+0.5f);
#else
        sx_mix_0 = (sx_mix1_temp+half_internal_scale)/internal_scale;
        sx_inc = sx_mix_0;
        sx_mix_1 = sx_mix_0;
        sx_out_1 = (sx_out1_temp+half_internal_scale-1)/internal_scale + 1;
#endif
        return;
    }
    if( sx_mix1_temp > sx_out1_temp){
        // out->mix->out
#ifdef USE_SINGLE_PRECISION_FLOATING_COORDINATES
        sx_mix_0 = floor(sx_mix0_temp+0.5f);
        sx_inc = sx_mix_0;
        sx_mix_1 = sx_mix_0;
        sx_out_1 = ceil(sx_out0_temp+0.5f);
#else
        sx_mix_0 = (sx_mix0_temp+half_internal_scale)/internal_scale;
        sx_inc = sx_mix_0;
        sx_mix_1 = sx_mix_0;
        sx_out_1 = (sx_out0_temp+half_internal_scale-1)/internal_scale + 1;
#endif
        return;
    }
    if( sx_mix0_temp < sx_mix1_temp ){
        if( sx_out0_temp < sx_mix1_temp ){
            // m0-o0-m1-o1
            // all mix
#ifdef USE_SINGLE_PRECISION_FLOATING_COORDINATES
            sx_mix_0 = floor(sx_mix0_temp+0.5f);
            sx_inc   = ceil(sx_out1_temp+0.5f);
            sx_mix_1 = sx_inc;
            sx_out_1 = sx_inc;
#else
            sx_mix_0 = (sx_mix0_temp+half_internal_scale)/internal_scale;
            sx_inc   = (sx_out1_temp+half_internal_scale-1)/internal_scale + 1;
            sx_mix_1 = sx_inc;
            sx_out_1 = sx_inc;
#endif
        }else{
            if( sx_out0_temp < sx_out1_temp ){
                // m0-m1-o0-o1
#ifdef USE_SINGLE_PRECISION_FLOATING_COORDINATES
                sx_mix_0 = floor(sx_mix0_temp+0.5f);
                sx_inc   = ceil(sx_mix1_temp+0.5f);
                sx_mix_1 = floor(sx_out0_temp+0.5f);
                sx_out_1 = ceil(sx_out1_temp+0.5f);
#else
                sx_mix_0 = (sx_mix0_temp+half_internal_scale)/internal_scale;
                sx_inc   = (sx_mix1_temp+half_internal_scale-1)/internal_scale + 1;
                sx_mix_1 = (sx_out0_temp+half_internal_scale)/internal_scale;
                sx_out_1 = (sx_out1_temp+half_internal_scale-1)/internal_scale + 1;
#endif
                if( sx_inc > sx_mix_1) sx_inc = sx_mix_1;
            }else{
                // m0-m1-o1-o0
#ifdef USE_SINGLE_PRECISION_FLOATING_COORDINATES
                sx_mix_0 = floor(sx_mix0_temp+0.5f);
                sx_inc   = ceil(sx_mix1_temp+0.5f);
                sx_mix_1 = floor(sx_out1_temp+0.5f);
                sx_out_1 = ceil(sx_out0_temp+0.5f);
#else
                sx_mix_0 = (sx_mix0_temp+half_internal_scale)/internal_scale;
                sx_inc   = (sx_mix1_temp+half_internal_scale-1)/internal_scale + 1;
                sx_mix_1 = (sx_out1_temp+half_internal_scale)/internal_scale;
                sx_out_1 = (sx_out0_temp+half_internal_scale-1)/internal_scale + 1;
#endif
                if( sx_inc > sx_mix_1) sx_inc = sx_mix_1;
            }
        }
    }else{
        if( sx_out1_temp < sx_mix0_temp ){
            // m1-o1-m0-o0
            // all mix
#ifdef USE_SINGLE_PRECISION_FLOATING_COORDINATES
            sx_mix_0 = floor(sx_mix1_temp+0.5f);
            sx_inc   = ceil(sx_out0_temp+0.5f);
            sx_mix_1 = sx_inc;
            sx_out_1 = sx_inc;
#else
            sx_mix_0 = (sx_mix1_temp+half_internal_scale)/internal_scale;
            sx_inc   = (sx_out0_temp+half_internal_scale-1)/internal_scale + 1;
            sx_mix_1 = sx_inc;
            sx_out_1 = sx_inc;
#endif
        }else{
            if( sx_out0_temp < sx_out1_temp ){
                // m1-m0-o0-o1
#ifdef USE_SINGLE_PRECISION_FLOATING_COORDINATES
                sx_mix_0 = floor(sx_mix1_temp+0.5f);
                sx_inc   = ceil(sx_mix0_temp+0.5f);
                sx_mix_1 = floor(sx_out0_temp+0.5f);
                sx_out_1 = ceil(sx_out1_temp+0.5f);
#else
                sx_mix_0 = (sx_mix1_temp+half_internal_scale)/internal_scale;
                sx_inc   = (sx_mix0_temp+half_internal_scale-1)/internal_scale + 1;
                sx_mix_1 = (sx_out0_temp+half_internal_scale)/internal_scale;
                sx_out_1 = (sx_out1_temp+half_internal_scale-1)/internal_scale + 1;
#endif
                if( sx_inc > sx_mix_1) sx_inc = sx_mix_1;
            }else{
                // m1-m0-o1-o0
#ifdef USE_SINGLE_PRECISION_FLOATING_COORDINATES
                sx_mix_0 = floor(sx_mix1_temp+0.5f);
                sx_inc   = ceil(sx_mix0_temp+0.5f);
                sx_mix_1 = floor(sx_out1_temp+0.5f);
                sx_out_1 = ceil(sx_out0_temp+0.5f);
#else
                sx_mix_0 = (sx_mix1_temp+half_internal_scale)/internal_scale;
                sx_inc   = (sx_mix0_temp+half_internal_scale-1)/internal_scale + 1;
                sx_mix_1 = (sx_out1_temp+half_internal_scale)/internal_scale;
                sx_out_1 = (sx_out0_temp+half_internal_scale-1)/internal_scale + 1;
#endif
                if( sx_inc > sx_mix_1) sx_inc = sx_mix_1;
            }
        }
    }
    // 上下ラインの中にある小さなポリゴン--初期値のまま)
    if( sx_mix0_temp == this->maxX + 1 
     && sx_out0_temp == this->minX - 1 // for y - 0.5
     && sx_mix1_temp == this->maxX + 1 // for y + 0.5
     && sx_out1_temp == this->minX - 1 // for y + 0.5
    ){

    }
}

void Polygon2D::print() const{
#ifndef ESP32    
    for(int i = 0; i < this->vertices.size(); i++ ){
        Point2D p = vertices[i];
        p.print();
    }
    if( is_convex ){
        debug_println("  convex");
    }else{
        debug_println("  not convex");
    }
#endif     
}


// methods
Polygon2D & Polygon2D::operator = (const Polygon2D p){
    // vertices
    this->vertices.resize(p.vertices.size());
    copy(p.vertices.begin(), p.vertices.end(), this->vertices.begin());
    this->minX = p.minX;
    this->maxX = p.maxX;
    this->minY = p.minY;
    this->maxY = p.maxY;
    this->is_convex = p.is_convex;
    this->sign_of_outer_product = p.sign_of_outer_product;
    return *this;
}
Polygon2D & Polygon2D::operator += (const Point2D p){
    int n = this->vertices.size();
    for( int i = 0; i < n; i++ ){
        (this->vertices[i]) += p;
    }
    this->minX += p.x;
    this->maxX += p.x;
    this->minY += p.y;
    this->maxY += p.y;
    return *this;
}
Polygon2D & Polygon2D::operator -= (const Point2D p){
    int n = this->vertices.size();
    for( int i = 0; i < n; i++ ){
        (this->vertices[i]) -= p;
    }
    this->minX -= p.x;
    this->maxX -= p.x;
    this->minY -= p.y;
    this->maxY -= p.y;
    return *this;
}
Polygon2D & Polygon2D::operator *= (const float f){
    int n = this->vertices.size();
    for( int i = 0; i < n; i++ ){
        this->vertices[i] *= f;
    }
    this->minX = this->minX * f;
    this->maxX = this->maxX * f;
    this->minY = this->minY * f;
    this->maxY = this->maxY * f;
    return *this;
}
Polygon2D & Polygon2D::operator /= (const float f){
    int n = this->vertices.size();
    //float cx = (this->minX+this->maxX)/2.0;
    //float cy = (this->minY+this->maxY)/2.0;
    //Point2D center(cx,cy);
    for( int i = 0; i < n; i++ ){
        //this->vertices[i] -= center;
        this->vertices[i] /= f;
        //this->vertices[i] += center;
    }
    float cx = 0.0f;
    float cy = 0.0f;
    float f_inv = 1.0f/f;
    this->minX = ( this->minX - cx ) * f_inv + cx;
    this->maxX = ( this->maxX - cx ) * f_inv + cx;
    this->minY = ( this->minY - cy ) * f_inv + cy;
    this->maxY = ( this->maxY - cy ) * f_inv + cy;
    return *this;
}

Polygon2D Polygon2D::operator + (const Point2D p) const{
    Polygon2D ret_polygon = *this;
    ret_polygon += p;
    return ret_polygon;
}
Polygon2D Polygon2D::operator - (const Point2D p) const{
    Polygon2D ret_polygon = *this;
    ret_polygon -= p;
    return ret_polygon;
}
Polygon2D Polygon2D::operator * (const float f) const{
    Polygon2D ret_polygon = *this;
    ret_polygon *= f;
    return ret_polygon;
}
Polygon2D Polygon2D::operator / (const float f) const{
    Polygon2D ret_polygon = *this;
    ret_polygon /= f;
    return ret_polygon;
}

Polygon2D Polygon2D::rotate( const float deg ) const{
    Polygon2D ret_polygon = *this;
    ret_polygon.rotate_equal(deg);
    return ret_polygon;
}

Polygon2D Polygon2D::rotate( const float deg, Point2D center ) const{
    Polygon2D ret_polygon = *this;
    ret_polygon -= center;
    ret_polygon.rotate_equal(deg);
    ret_polygon += center;
    return ret_polygon;
}

Polygon2D & Polygon2D::rotate_equal( const float deg ){

    int n = this->vertices.size();
    float rad = deg * 3.1415926535f / 180.0f;
    float cos_t = cos(rad);
    float sin_t = sin(rad);
    for( int i = 0; i < n; i++ ){
        float tx = cos_t * this->vertices[i].x - sin_t * this->vertices[i].y;
        float ty = sin_t * this->vertices[i].x + cos_t * this->vertices[i].y;
        this->vertices[i].x = tx;
        this->vertices[i].y = ty;
        if( i == 0 ){
            this->minX = this->vertices[i].x;
            this->maxX = this->vertices[i].x;
            this->minY = this->vertices[i].y;
            this->maxY = this->vertices[i].y;            
        }else{
            if( this->minX > this->vertices[i].x ) this->minX = this->vertices[i].x;
            if( this->maxX < this->vertices[i].x ) this->maxX = this->vertices[i].x;
            if( this->minY > this->vertices[i].y ) this->minY = this->vertices[i].y;
            if( this->maxY < this->vertices[i].y ) this->maxY = this->vertices[i].y;
        }
    }

    return *this;
} 
Polygon2D & Polygon2D::rotate_equal( const float deg, Point2D center ){
    *this -= center;
    this->rotate_equal(deg);
    *this += center;
    return *this;
}


Polygon2D & Polygon2D::rotate_equal( const float cos_t, const float sin_t ){

    int n = this->vertices.size();
    for( int i = 0; i < n; i++ ){
        float tx = cos_t * this->vertices[i].x - sin_t * this->vertices[i].y;
        float ty = sin_t * this->vertices[i].x + cos_t * this->vertices[i].y;
        this->vertices[i].x = tx;
        this->vertices[i].y = ty;
        if( i == 0 ){
            this->minX = this->vertices[i].x;
            this->maxX = this->vertices[i].x;
            this->minY = this->vertices[i].y;
            this->maxY = this->vertices[i].y;            
        }else{
            if( this->minX > this->vertices[i].x ) this->minX = this->vertices[i].x;
            if( this->maxX < this->vertices[i].x ) this->maxX = this->vertices[i].x;
            if( this->minY > this->vertices[i].y ) this->minY = this->vertices[i].y;
            if( this->maxY < this->vertices[i].y ) this->maxY = this->vertices[i].y;
        }
    }

    return *this;
}

// ポリゴンの結合
// 凸判定のため、1要素ずつ追加
void Polygon2D::concat( const Polygon2D p ){
    int np = p.vertices.size();
    for( int n = 0; n < np; n++ ){
        this->add_Point2D(p.get_Point2D(n));        
    }
}

// ポリゴンの結合
// 凸判定のため、1要素ずつ追加
void Polygon2D::concat_inversely( const Polygon2D p ){
    int np = p.vertices.size();
    for( int n = np-1; n >=0; n-- ){
        this->add_Point2D(p.get_Point2D(n));        
    }
}