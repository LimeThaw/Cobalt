#include "camera_manager.h"

camera_manager& camera_manager::get_instance(){
    static camera_manager instance;
    return instance;
}

unsigned int camera_manager::create_camera(glm::vec3 new_place, glm::vec3 new_point){
    camera* new_camera = new camera;
    new_camera->setup(new_place, new_point);
    cameras.push_back(new_camera);
    return cameras.size()-1;
}

void camera_manager::set_active_camera(unsigned int cameraID){
    if(cameraID < cameras.size() && cameras[cameraID] != NULL){
        active_camera = cameraID;
    }else{
        std::cerr << "! Tried to set non-existent camera active\n";
    }
}

void camera_manager::update_camera(){
    if(cameras[active_camera] != NULL){
        cameras[active_camera]->update();
    }else{
        std::cerr << "! Tried to use non-existent camera\n";
    }
}

camera_manager::~camera_manager(){
    for(int i=0; i<cameras.size(); i++){
        delete cameras[i];
    }
    cameras.clear();
}

camera_manager::camera_manager(){
    //ctor
}

unsigned int create_camera(glm::vec3 new_place, glm::vec3 new_point){
    camera_manager::get_instance().create_camera(new_place, new_point);
}

void set_active_camera(unsigned int cameraID){
    camera_manager::get_instance().set_active_camera(cameraID);
}

void update_camera(){
    camera_manager::get_instance().update_camera();
}
