#include "analytics.hpp"

#include <stdarg.h>
#include <curl/curl.h>

namespace bengine {
	namespace analytics {
		static CURLM* curl_multi_handle = nullptr;

		// caches clientID and trackingID after calling Init() (http://www.google-analytics.com/collect?v=1&tid=%s&cid=%s)
		static char analytics_url[2048] = { '\0' };

		// utility function, used to replace spaces with pluses for URLs
		static void replace_str_char(char *s, const int len, const char what, const char with)
		{
			for (int i = 0; i < len; ++i) {
				if (s[i] == what)
					s[i] = with;
			}
		}

		// utility function, used to send the HTTP get
		static bool execute_curl_url(const char* url, ...)
		{
			if (!curl_multi_handle)
				return false;

			va_list argptr;
			va_start(argptr, url);
			char strAddr[2048] = { '\0' };
			int slen = vsprintf(strAddr, url, argptr);
			va_end(argptr);

			replace_str_char(strAddr, slen, ' ', '+');

			CURL* pCurlHandle = curl_easy_init();
			curl_easy_setopt(pCurlHandle, CURLOPT_URL, strAddr);
			curl_easy_setopt(pCurlHandle, CURLOPT_FOLLOWLOCATION, 1L);
			curl_easy_setopt(pCurlHandle, CURLOPT_TIMEOUT, 20);
			CURLMcode result = curl_multi_add_handle(curl_multi_handle, pCurlHandle);
			return (result == CURLM_OK);
		}

		bool init(const char* trackingId, const char* uniqueClientId) noexcept
		{
			curl_global_init(CURL_GLOBAL_ALL);
			curl_multi_handle = curl_multi_init();
			if (!curl_multi_handle)
				return false;

			sprintf(analytics_url, "http://www.google-analytics.com/collect?v=1&tid=%s&cid=%s", trackingId, uniqueClientId);
			return true;
		}

		void shutdown() noexcept
		{
			if (!curl_multi_handle)
				return;

			update_tick(); // one last update to remove handles from stack if they're ready
			curl_multi_cleanup(curl_multi_handle);
			curl_multi_handle = NULL;
		}

		void on_event(const char* category, const char* action, const char* label, const unsigned int value) noexcept
		{
			execute_curl_url("%s&t=event&ec=%s&ea=%s&el=%s&ev=%u&z=%d", analytics_url, category, action, label, value, rand());
		}

		void on_event(const char* category, const char* action, const char* label) noexcept
		{
			execute_curl_url("%s&t=event&ec=%s&ea=%s&el=%s&z=%d", analytics_url, category, action, label, rand());
		}

		void on_event(const char* category, const char* action) noexcept
		{
			execute_curl_url("%s&t=event&ec=%s&ea=%s&z=%d", analytics_url, category, action, rand());
		}

		void update_tick() noexcept
		{
			if (!curl_multi_handle)
				return;

			int stillRunning = 0;
			curl_multi_perform(curl_multi_handle, &stillRunning);

			CURLMsg* pMsg = NULL;
			do {
				int msgsInQueue = 0;
				pMsg = curl_multi_info_read(curl_multi_handle, &msgsInQueue);
				if (pMsg && (pMsg->msg == CURLMSG_DONE)) {
					long response_code;
					curl_easy_getinfo(pMsg->easy_handle, CURLINFO_RESPONSE_CODE, &response_code);
					if (response_code != 200) {
						const char *urlp;
						curl_easy_getinfo(pMsg->easy_handle, CURLINFO_EFFECTIVE_URL, &urlp);

						char strerr[2048];
						sprintf(strerr, "[Error] TAnalytics_Update() failed for URL '%s' with error %ld\n", urlp ? urlp : "?", response_code);
						//assert(response_code == 200 && strerr);
					}
					curl_multi_remove_handle(curl_multi_handle, pMsg->easy_handle);
					curl_easy_cleanup(pMsg->easy_handle);
				}
			} while (pMsg);
		}
	}
}
