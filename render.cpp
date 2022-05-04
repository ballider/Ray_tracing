#include "head.hpp"
#include <cstring>

#include <chrono>

#define PI 3.14159265358979323846

const Vec3d BACKGROUND_COLOR(80/255.0, 125/255.0, 42/255.0);

// пересёк ли луч хоть одну фигуру. Получаем точку пересечения(point), вектор нормали в точке(N), данные о поверхности(material)
// выбираем ближающую фигуру
bool nearest_intersect(const Ray& ray, const std::vector<Figure*>& Figures, Vec3d& point, Vec3d& N, Material& material) {
    double nearest_dist = std::numeric_limits<double>::max();

    bool intersect = 0; // пересёк ли хоть одну фигуру

    for(size_t i = 0; i < Figures.size(); ++i) {
        double CurrentDist = Figures[i]->ray_intersect(ray);
        if(CurrentDist == -1) continue;
        
        intersect = 1;

        if(CurrentDist < nearest_dist) {
            nearest_dist = CurrentDist;
            // запоминаем данные
            point = ray.orig + ray.dir*CurrentDist;
            N = (point - Figures[i]->center()).normalize();
            material = Figures[i]->material();
        }
    }

    return intersect;
}

// выбираем цвет точки на холсте куда смотрит луч
Vec3d cast_ray(const Ray& ray, const std::vector<Figure*>& Figures, const Light& light) {
    Vec3d point, N;
    Material material;
    // point - точка пересечения луча из камеры с объектом
    // N - вектро номали в этой точки от поверхности фигуры
    // material - материал фигуры(цвет, коэф отражения и т.д)

    // если не пересекае никакую тх фигур
    if (!nearest_intersect(ray, Figures, point, N, material)) {
        return BACKGROUND_COLOR;
    }


    // по модели Фонга добавляем цвет(ambident, diffuse, specular)
    Vec3d I(0, 0 ,0);

    double lambda = 0.4;    // коэффицент для лучшей отрисрвки

    
    Vec3d light_dir = (light._position - point).normalize();

    // ambident
    // можно не учитывать, так как цвет фигуры без света это чётный (0, 0, 0) и это не повлечёт изменений
        
    // diffuse
    I = I + material._color*light._diffuse* std::max(0.0, dot(light_dir, N));

    // specular
    Vec3d intersect_to_camera = (ray.orig-point).normalize();
    Vec3d H = (light_dir + intersect_to_camera).normalize();
    I = I + Vec3d(1, 1, 1)*pow(dot(N, H), material._shininess/4)*lambda;

    return I;
}

void render(const std::vector<Figure*>& Figures, Parametrs& param) {
    std::vector<std::vector<Vec3d>> canvas(param.height, std::vector<Vec3d>(param.width, BACKGROUND_COLOR));

    size_t m = param.height;    // кол-во пикселей по вертикали
    size_t k = param.width;     // кол-во пикселей по горизонтали

    Vec3d t = param.normal.normalize();
    Vec3d b = cross(param.up, t).normalize();
    Vec3d v = cross(t, b);

    double d = param.screen;
    double fov = param.alpha/180 * PI;

    double gx = d*tan(fov/2.);
    double gy = gx*(static_cast<double>(m)/static_cast<double>(k));

    auto start_for = std::chrono::steady_clock::now();
    #pragma omp parallel for
    for(size_t j = 0; j < param.height; ++j) {
        for(size_t i = 0; i < param.width; ++i) {
            
            double _dir[3];
            for(int l = 0; l < 3; ++l) {
                _dir[l] = t[l]*d - gx*b[l] - gy*v[l] + (i*2*gx)*b[l]/(k-1) + ((m-j)*2*gy)*v[l]/(m-1);
            }
            Vec3d dir = Vec3d(_dir[0], _dir[1], _dir[2]).normalize();

            Ray ray(param.cam, dir);

            canvas[j][i] = cast_ray(ray, Figures, param.light);
        }
    }
    auto end_for = std::chrono::steady_clock::now();

    std::cout << "time: " << std::chrono::duration<double, std::milli>(end_for - start_for).count() << " ms" << "\n";

    //std::ofstream ofs;
    //ofs.open("./out.ppm");
    //ofs << "P6\n" << param.width << " " << param.height << "\n255\n";
    for(size_t i = 0; i < param.height; ++i) {
        for(size_t j = 0; j < param.width; ++j) {
            for(size_t l = 0; l < 3; ++l) {
                canvas[i][j][l] = std::min(1.0, canvas[i][j][l]);
                canvas[i][j][l] *= 255.0;
                
            }
        }
    }
    //ofs.close();


    //-----------------|
    // Create .BMP     |
    //-----------------|

    int w = param.width;
    int h = param.height;

    unsigned char* img = NULL;
    int filesize = 54 + 3*w*h;

    img = (unsigned char*)malloc(3*w*h);
    memset(img, 0, 3*w*h);

    for(int i = 0; i < h; ++i) {
        for(int j = 0; j < w; ++j) {
            unsigned char r = static_cast<unsigned char>(canvas[i][j][0]);
            unsigned char g = static_cast<unsigned char>(canvas[i][j][1]);
            unsigned char b = static_cast<unsigned char>(canvas[i][j][2]);
            img[(j+i*w)*3+2] = r;
            img[(j+i*w)*3+1] = g;
            img[(j+i*w)*3+0] = b;
        }
    }

    unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
    unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
    unsigned char bmppad[3] = {0,0,0};

    bmpfileheader[ 2] = (unsigned char)(filesize    );
    bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
    bmpfileheader[ 4] = (unsigned char)(filesize>>16);
    bmpfileheader[ 5] = (unsigned char)(filesize>>24);

    bmpinfoheader[ 4] = (unsigned char)(       w    );
    bmpinfoheader[ 5] = (unsigned char)(       w>> 8);
    bmpinfoheader[ 6] = (unsigned char)(       w>>16);
    bmpinfoheader[ 7] = (unsigned char)(       w>>24);
    bmpinfoheader[ 8] = (unsigned char)(       h    );
    bmpinfoheader[ 9] = (unsigned char)(       h>> 8);
    bmpinfoheader[10] = (unsigned char)(       h>>16);
    bmpinfoheader[11] = (unsigned char)(       h>>24);

    FILE *f = fopen("Result.bmp","wb");
    fwrite(bmpfileheader, 1, 14, f);
    fwrite(bmpinfoheader, 1, 40, f);
    for(int i = 0; i < h; ++i) {
        fwrite(img  +(w*(h-1-i)*3), 3, w, f);
        fwrite(bmppad, 1, (4-(w*3)%4)%4, f);
    }

    free(img);
    fclose(f);

}
