#include "std_scene.h"
using namespace cs;

std_scene::std_scene() : scene() {
	render_pass = std::make_shared<std_render_pass>();
	skybox_texture_path = "";
}

void std_scene::set_camera(std::shared_ptr<camera> new_camera) {
	main_camera = new_camera;
}

void std_scene::add_point_light(std::shared_ptr<point_light> new_light) {
	p_lights.push_back(new_light);
}

void std_scene::add_directional_light(std::shared_ptr<directional_light> new_light) {
	d_lights.push_back(new_light);
}

void std_scene::set_skybox(std::string path) {
	if(skybox == NULL) {
		skybox = std::make_shared<mesh>("engine/library/skybox.obj", "");
		skybox_shader = std::make_shared<shader>("./engine/library/shader/skybox_shader.vertex", "engine/library/shader/skybox_shader.fragment");
	}
	skybox_texture_path = path;
	auto skybox_mat = std::make_shared<material>();
	skybox_mat->add_texture("color_map", std::make_shared<texture2d>(
            texture_cache::get_instance().get_texture_from_filename(path)));
    skybox->set_material(skybox_mat);
}


void std_scene::save(const char* file_path, bool pretty) {
	json scene;
	
	// Add meshes from scene graph
	for(auto node : enumerate_nodes()) {
		auto name = node->get_name().c_str();
		
		// Is the node a mesh?
		if(mesh *m = dynamic_cast<mesh*>(node)) {
			auto item = scene["meshes"][name];
	 		item["path"] = m->get_path();
	 		item["node_matrix"] = serialize(m->get_isolated_matrix());
	 		if(m->get_parent() != nullptr && m->get_parent() != get_parent_node())
 				item["parent"] = m->get_parent()->get_name();
 			scene["meshes"][name] = item;
 			
 			// Add material if standard and not already present
 			auto mat = m->get_material();
 			if(mat != nullptr && mat->is_standard()) {
 				scene["meshes"][name]["material"] = mat->get_name().c_str();
 				auto name = mat->get_name().c_str();
 				if(scene["materials"][name] == NULL) {
 					auto item = scene["materials"][name];
 					item["uniforms"] = mat->get_uniforms_json();
 					item["shadow_caster"] = mat->is_shadow_caster();
 					for(auto t : mat->get_textures()) {
 						auto name = t.second->get_name().c_str();
 						item["textures"][t.first.c_str()] = name;
 						if(scene["textures"][name] == NULL) {
 							scene["textures"][name]["path"] = t.second->get_path().c_str();
 						}
 					}
 					scene["materials"][name] = item;
 				}
 			}
 				
 		// Or is it a camera?
	 	} else if(camera *c = dynamic_cast<camera*>(node)) {
			auto item = scene["cameras"][name];
	 		item["node_matrix"] = serialize(c->get_isolated_matrix());
	 		item["projection_matrix"] = serialize(c->get_projection());
	 		if(c->get_parent() != nullptr && c->get_parent() != get_parent_node())
 				item["parent"] = c->get_parent()->get_name();
 			scene["cameras"][name] = item;
 				
 		// Or is it a light?
	 	} else if(point_light *p = dynamic_cast<point_light*>(node)) {
	 		auto item = scene["point_lights"][name];
			item["position"] = serialize(p->get_position());
			item["color"] = serialize(p->get_color());
			item["intensity"] = p->get_intensity();
			item["radius"] = p->get_radius();
	 		if(p->get_parent() != nullptr && p->get_parent() != get_parent_node())
	 			item["parent"] = p->get_parent()->get_name();
	 		scene["point_lights"][name] = item;
	 			
	 	// No? The it is a group node.
	 	} else if(node != get_parent_node()){
	 		scene["groups"][name]["node_matrix"] = serialize(node->get_isolated_matrix());
	 		if(node->get_parent() != nullptr)
 				scene["groups"][name]["parent"] = node->get_parent()->get_name();
	 	}
	}
	
	// Add point lights from list if not in scene graph
	for(auto light : p_lights) {
		auto name = light->get_name().c_str();
		auto item = scene["point_lights"][name];
		if(item == NULL) {
			item["position"] = serialize(light->get_position());
			item["color"] = serialize(light->get_color());
			item["intensity"] = light->get_intensity();
			item["radius"] = light->get_radius();
			if(light->get_parent() != nullptr && light->get_parent() != get_parent_node())
				item["parent"] = light->get_parent()->get_name();
			scene["point_lights"][name] = item;
		}
	}
	
	// Add directional lights
	for(auto light : d_lights) {
		auto name = light->get_name();
		auto item = scene["dir_lights"][name];
		item["direction"] = serialize(light->get_direction());
		item["color"] = serialize(light->get_color());
		item["intensity"] = light->get_intensity();
		scene["dir_lights"][name] = item;
	}
	
	// Add main camera
	if(main_camera) {
		auto name = main_camera->get_name().c_str();
		scene["main_camera"] = name;
		if(scene["cameras"][name] == NULL) {
	 		scene["cameras"][name]["node_matrix"] = serialize(main_camera->get_isolated_matrix());
	 		scene["cameras"][name]["projection_matrix"] = serialize(main_camera->get_projection());
			if(main_camera->get_parent() != nullptr && main_camera->get_parent() != get_parent_node())
				scene["point_lights"][name]["parent"] = main_camera->get_parent()->get_name();
		}
	}
	
	// Add skybox texture path
	if(skybox_texture_path != "") {
		scene["skybox"] = skybox_texture_path.c_str();
	}
	
	// Serialize and write to file
	auto text = pretty ? scene.dump(4) : scene.dump();
	write_to_file(file_path, text.c_str());
	printf("- Wrote scene to file %s\n", file_path);
}



void std_scene::load(const char* file_path) {
	json scene = json::parse(read_from_file(file_path).c_str());
	
	// Dictionaries holding all loaded textures and materials
	map<string, shared_ptr<texture>> texs;
	map<string, shared_ptr<material>> mats;
	
	// Loading all textures for the scene
	for(json::iterator it = scene["textures"].begin(); it != scene["textures"].end(); ++it) {
		texs.insert(pair<string, shared_ptr<texture>>(it.key(), texture2d::load_file((*it)["path"].get<string>().c_str(), it.key())));
		
		// DEBUG INFO
		printf("  + Loaded texture %s from path %s\n", it.key().c_str(), (*it)["path"].get<string>().c_str());
	}
	
	// Loading all materials for the scene
	for(json::iterator it = scene["materials"].begin(); it != scene["materials"].end(); ++it) {
		auto mat = std::make_shared<material>(it.key());
		mat->set_shadow_cast((*it)["shadow_caster"]);
		
		// Add textures to material
		for(json::iterator jt = (*it)["textures"].begin(); jt != (*it)["textures"].end(); ++jt) {
			mat->add_texture(jt.key(), texs.find(*jt)->second);
			// DEBUG_INFO
			printf("  + Added texture %s as %s to material %s\n", (*jt).get<string>().c_str(), jt.key().c_str(), it.key().c_str());
		}
		
		// Add uniforms to material
		for(json::iterator jt = (*it)["uniforms"].begin(); jt != (*it)["uniforms"].end(); ++jt) {
			string type = (*jt)[0].get<string>();
			if(type == "float") {
				mat->set_uniform(jt.key(), make_shared<float_uniform>((*jt)[1].get<float>()));
			} else if(type == "vec3") {
				mat->set_uniform(jt.key(), make_shared<vec3_uniform>(deserialize_vec3((*jt)[1].get<string>())));
			} else {
				printf("! Found uniform %s of unknown type %s\n", jt.key().c_str(), type.c_str());
			}
		}
	}
	
	// Remember the parenthood relations - Name of the parent first,
	// pointer to the child second
	map<string, shared_ptr<node>> parents;
	// Also remember all nodes for setting parent
	vector<shared_ptr<node>> nodes;
	
	// Loading abstract nodes as groups
	for(json::iterator it = scene["groups"].begin(); it != scene["groups"].end(); ++it) {
		auto temp = make_shared<node>();
		temp->set_name(it.key());
		temp->set_node_matrix(deserialize_mat4((*it)["node_matrix"].get<string>()));
		if((*it)["parent"] != NULL)
			parents.insert(pair<string, shared_ptr<node>>((*it)["parent"].get<string>(), temp));
		nodes.push_back(temp);
		append_node(temp.get());
	}
	
	// Loading cameras in scene
	for(json::iterator it = scene["cameras"].begin(); it != scene["cameras"].end(); ++it) {
		auto temp = make_shared<camera>();
		temp->set_name(it.key());
		temp->set_node_matrix(deserialize_mat4((*it)["node_matrix"].get<string>()));
		temp->set_projection(deserialize_mat4((*it)["projection_matrix"].get<string>()));
		append_node(temp.get());
	}
	
	// Set main camera
	if(scene["main_camera"] == NULL) {
		printf("! Loaded scene does not define a main camera\n");
	} else {
		string mc = scene["main_camera"].get<string>();
		if(camera* c = dynamic_cast<camera*>(find_node(mc))) {
			auto cam = make_shared<camera>();
			*cam = *c;
			set_camera(cam);
		} else {
			printf("! Loaded scene scpecified undefined camera as main camera\n");
		}
	}
	
	// Loading all meshes in scene
	for(json::iterator it = scene["meshes"].begin(); it != scene["meshes"].end(); ++it) {
		auto temp = make_shared<mesh>();
		temp->set_name(it.key());
		temp->set_node_matrix(deserialize_mat4((*it)["node_matrix"].get<string>()));
		if((*it)["path"] != NULL) {
			temp->load_model((*it)["path"]);
		}
		// Set parent node if present
		if((*it)["parent"] != NULL)
			parents.insert(pair<string, shared_ptr<node>>((*it)["parent"].get<string>(), temp));
		// Set material if found
		if((*it)["material"] != NULL && mats.find((*it)["material"].get<string>()) != mats.end())
			temp->set_material(mats.find((*it)["material"].get<string>())->second);
		nodes.push_back(temp);
		append_node(temp.get());
		
		// DEBUG INFO
		printf("  + Loaded mesh %s from path %s\n", it.key().c_str(), (*it)["path"].get<string>().c_str());
	}
	
	// Now create scene graph structure by setting parenthood relationships
	map<string, shared_ptr<node>>::iterator it;
	for(auto n : nodes) {
		if((it = parents.find(n->get_name())) != parents.end()) {
			(*it).second->append_node(n.get());
		} else {
			printf("! Could not find parent of node %s\n", n->get_name().c_str());
		}
	}
}

void std_scene::render() {
	if(main_camera) {
		if(skybox) {
			skybox_shader->use();
			GLint active_shader_id;
			glGetIntegerv(GL_CURRENT_PROGRAM, &active_shader_id);
			
			GLuint model_id = glGetUniformLocation(active_shader_id, "model");
			glUniformMatrix4fv(model_id, 1, GL_FALSE, &skybox->get_node_matrix()[0][0]);
			GLuint view_id = glGetUniformLocation(active_shader_id, "view");
			glUniformMatrix4fv(view_id, 1, GL_FALSE, &main_camera->get_view()[0][0]);
			GLuint projection_id = glGetUniformLocation(active_shader_id, "projection");
			glUniformMatrix4fv(projection_id, 1, GL_FALSE, &main_camera->get_projection()[0][0]);
			
			glDepthMask(false);
			skybox->render();
			glDepthMask(true);
		}
		render_pass -> render(*this, *main_camera, d_lights, p_lights);
	} else {
		std::cerr << "! Tried to render scene without active camera\n";
	}
}
