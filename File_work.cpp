#include "head.hpp"
#include "Figure_Factory.hpp"

bool ParametrsFromFile(const std::string& FileName, Parametrs& param) {
    std::ifstream inp(FileName);
    
    if(!inp){
        std::cout << "Error! Cannot open file: " << FileName << "\n";
        return 0;
    }
    
    while(!inp.eof()){
        std::string I; inp >> I;

        if(I == "cam") {
            double x, y, z;
            inp >> x >> y >> z;
            param.cam = Vec3d(x, y, z);
        }
        else if(I == "width") {
            inp >> param.width;
        }
        else if(I == "height") {
            inp >> param.height;
        }
        else if(I == "light") {
            double x, y, z;
            inp >> x >> y >> z;
            param.light = Vec3d(x, y ,z);
        }
        else if(I == "normal") {
            double x, y, z;
            inp >> x >> y >> z;
            param.normal = Vec3d(x, y ,z);
        }
        else if(I == "up") {
            double x, y, z;
            inp >> x >> y >> z;
            param.up = Vec3d(x, y ,z);
        }
        else if(I == "screen") {
            inp >> param.screen;
        }
        else if(I == "limit") {
            inp >> param.limit;
        }
        else if(I == "alpha") {
            inp >> param.alpha;
        }
        else{
            std::cout << "Error! Unknown parametr: " << I << "\n";
            inp.close();
            return 0;
        }
    }

    inp.close();

    return 1;
}

bool FiguresFromFile(const std::string& FileName, std::vector<Figure*>& Figures) {
    std::ifstream inp(FileName);
    if(!inp){
        std::cout << "Error! Cannot open file: " << FileName << "\n";
        return 0;
    }

    
    std::map<std::string, Creator*> Factory;
    Factory["sphere"] = new CreateSphere;
    Factory["box"]    = new CreateBox;
    Factory["tetra"]  = new CreateTetrahedron;
    

    while(!inp.eof()) {
        std::string I; inp >> I;

        
        auto ConcreteCreator = Factory.find(I);
        if(ConcreteCreator == Factory.end()){
            std::cout << "Error! Wrong number type: " << I << "\n";
            return 0;
        }
        

        if(I == "sphere") {
            double x, y, z, R;
            inp >> x >> y >> z >> R;
            
            double r, g, b;
            inp >> r >> g >> b;
            Material material(Vec3d(r/255.0, g/255.0, b/255.0));
            
            //Figures.push_back(new Sphere(Vec3d(x, y, z), R));
            Figures.push_back(ConcreteCreator->second->factoryMethod(Vec3d(x, y, z), R));
            Figures[Figures.size()-1]->UpdateMaterial(material);
        }
        else if(I == "box") {
            double x1, y1, z1, x2, y2, z2;
            inp >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;

            double r, g, b;
            inp >> r >> g >> b;
            Material material(Vec3d(r/255.0, g/255.0, b/255.0));

            //Figures.push_back(new Box(Vec3d(x1, y1, z1), Vec3d(x2, y2, z2)));
            Figures.push_back(ConcreteCreator->second->factoryMethod(Vec3d(x1, y1, z1), Vec3d(x2, y2, z2)));
            Figures[Figures.size()-1]->UpdateMaterial(material);
        }
        else if(I == "tetra") {
            Vec3d points[4];
            for(int i = 0; i < 4; ++i) {
                inp >> points[i][0] >> points[i][1] >> points[i][2];
            }

            double r, g, b;
            inp >> r >> g >> b;
            Material material(Vec3d(r/255.0, g/255.0, b/255.0));

            //Figures.push_back(new Tetrahedron(points[0], points[1], points[2], points[3]));
            Figures.push_back(ConcreteCreator->second->factoryMethod(points[0], points[1], points[2], points[3]));
            Figures[Figures.size()-1]->UpdateMaterial(material);
        }
    }
    inp.close();

    for(auto& fd: Factory) {
        delete fd.second;
    }

    return 1;
}