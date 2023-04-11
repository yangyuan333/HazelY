#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>
#define M_PI 3.14159

std::ostream& operator<<(std::ostream & os, glm::mat4 data) {
	for (size_t i = 0; i < 4; ++i) {
		os << std::endl;
		for (size_t j = 0; j < 4; ++j) {
			os << data[j][i] << " ";
		}
	}
	return os;
}

int main() {

	glm::quat quat(glm::vec3{ M_PI / 2,M_PI / 2,0 });
	glm::mat4 rotationMatrix = glm::mat4_cast(quat);
	
	std::cout << rotationMatrix;

	glm::mat4 p{ 1.0f };
	std::cout << p << std::endl;

}