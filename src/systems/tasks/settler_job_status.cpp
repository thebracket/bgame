#include "settler_job_status.hpp"
#include "../../messages/log_message.hpp"
#include "../../components/logger.hpp"

namespace tasks {

void change_job_status(settler_ai_t &ai, name_t &name, const std::string new_status) {
	ai.job_status = new_status;
	//std::cout << name.first_name << " is now: " << new_status << "\n";
	emit_deferred(log_message{LOG{}.settler_name_t(name)->text(" is now ")->text(new_status)->chars});
}

}