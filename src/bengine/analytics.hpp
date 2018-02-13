#pragma once

/*
Inspired by https://github.com/Pintea/tiniest-analytics
Original authors:
Mihai Dragomir, email:dmc@pixelshard.com
Mihai Gosa, email:pintea@inthekillhouse.com  twitter: @gosamihai

MIT licensed, and modified considerably.
*/

namespace bengine {
	namespace analytics {
		// 'uniqueClientId' should be unique for each user, e.g. the Steam UID or a randomly generated 32/64bit value
		// e.g. Init("UA-12345678-1", steamClientId)
		bool init(const char* trackingId, const char* uniqueClientId) noexcept;

		// call when exiting app
		void shutdown() noexcept;

		// call at most once per frame
		void update_tick() noexcept;

		// read this https://support.google.com/analytics/answer/1033068?hl=en
		// e.g. Event("GameStart", "linux")
		// e.g. Event("WeaponClicked", "RocketLauncher")
		// e.g. Event("MapStarted", "SinglePlayer", "MapName.map")
		void on_event(const char* category, const char* action) noexcept;
		void on_event(const char* category, const char* action, const char* label) noexcept;
		void on_event(const char* category, const char* action, const char* label, const unsigned int value) noexcept;
	}
}