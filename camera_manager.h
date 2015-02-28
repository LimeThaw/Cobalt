//Singleton-class managing all camera objects

#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include <iostream>
#include <vector>

#include "camera.h"

class camera_manager{
    public:
        static camera_manager& get_instance();
        unsigned int create_camera(glm::vec3 new_place, glm::vec3 new_point);
        void set_active_camera(unsigned int cameraID);
        void update_camera();
        ~camera_manager();

    private:
        camera_manager();
        unsigned int active_camera;
        std::vector<camera*> cameras;
};

unsigned int create_camera(glm::vec3 new_place, glm::vec3 new_point);
void set_active_camera(unsigned int cameraID);
void update_camera();

#endif // CAMERA_MANAGER_H
