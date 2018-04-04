#include "ssao.hpp"
#include <random>
#include "../global_assets/shader_storage.hpp"
#include "../bengine/gl_include.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shaders/lightstage_shader.hpp"

namespace render {
	bool ssao_setup = false;
	unsigned int noise_tex = 0;

	static float lerp(const float a, const float b, const float f)
	{
		return a + f * (b - a);
	}

	static std::vector<glm::vec3> ssao_kernel;

	void setup_ssao() {
		std::uniform_real_distribution<float> random_floats(0.0, 1.0); // random floats between 0.0 - 1.0
		std::default_random_engine generator;
		for (unsigned int i = 0; i < 64; ++i)
		{
			glm::vec3 sample(
				random_floats(generator) * 2.0 - 1.0,
				random_floats(generator) * 2.0 - 1.0,
				random_floats(generator)
			);
			sample = glm::normalize(sample);
			sample *= random_floats(generator);
			auto scale = static_cast<float>(i) / 64.0f;
			scale = lerp(0.1f, 1.0f, scale * scale);
			sample *= scale;
			ssao_kernel.push_back(sample);
		}
	}

	void send_samples_to_shader() {
		for (unsigned int i = 0; i < 64; ++i)
			assets::lightstage_shader->set_vec3("samples[" + std::to_string(i) + "]", ssao_kernel[i]);
	}
}