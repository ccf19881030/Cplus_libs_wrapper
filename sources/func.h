#pragma once

namespace Math
{
	namespace Byte
	{
		// bytes转换成BCD码
		static Uint8Array bytes2bcd(const Uint8Array& data)
		{
			Uint8Array out;
			// 比如 20 18 转换成 0x20 0x18
			for (size_t i = 0; i < data.size(); i++)
			{
				const uint8 d = data[i];
				out.push_back(d / 10 * 16 + d % 10);
			}
			return out;
		}

		// BCD码转换成bytes
		static Uint8Array bcd2bytes(const Uint8Array& data)
		{
			Uint8Array out;
			// 比如 0x20 0x18 转换成 20 18
			for (size_t i = 0; i < data.size(); i++)
			{
				const uint8 d = data[i];
				out.push_back(d / 16 * 10 + d % 16);
			}
			return out;
		}

		// BCD码转换成bytes
		static Uint8Array bcd2bytes(const uint8* buff, uint32 buffsize)
		{
			Uint8Array out(buff, buff + buffsize);
			return bcd2bytes(out);
		}
	}
	
	// 时间
	namespace Date
	{
		// 将unix时间戳转换成本地时间, fmt格式, %02d
		static std::string unix2str(const time_t unix, const char* fmt) 
		{
			tm tt;
			localtime_s(&tt, &unix);
			char date[64];
			sprintf_s(date, fmt, tt.tm_year + 1900, tt.tm_mon + 1, tt.tm_mday, tt.tm_hour, tt.tm_min, tt.tm_sec);
			return date;
		}
		// 获取当前时间字符串，默认格式 2018-10-01 01:10:20
		static std::string getnow(const char* fmt = nullptr)
		{
			const char* fmtt = (fmt == nullptr) ? "%04d-%02d-%02d %02d:%02d:%02d" : fmt;
			return unix2str(time(0), fmtt);
		}
		// 获取本地时间tm
		static tm gettm(const time_t unix = 0)
		{
			time_t t = unix == 0 ? time(0) : unix;
			tm tt;
			localtime_s(&tt, &t);
			return tt;
		}
		static time_t gettime(int year, int month, int day, int hour = 0, int minute = 0, int second = 0)
		{
			tm tt = { 0 };
			tt.tm_year = year - 1900;
			tt.tm_mon = month - 1;
			tt.tm_mday = day;
			tt.tm_hour = hour;
			tt.tm_min = minute;
			tt.tm_sec = second;
			return mktime(&tt);
		}
	}

	//  定时器
	class Timer
	{
		// 毫秒时钟
		typedef std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> millisecClock_type;
	public:
		static time_t now_s() {
			return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		}
		static time_t now_ms() {
			return std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count();
			// return system_clock::to_time_t(time_point_cast<milliseconds>(system_clock::now()));
		}
		// 条件休眠,单位秒
		static inline void sleep_if(volatile bool& condition, int32 sec) {
			auto t0 = now_s();
			while (condition && (now_s() - t0) < sec)
			{
				msleep(10);
			}
		}
		// 条件休眠,单位毫秒
		static inline void usleep_if(volatile bool& condition, int32 millsec) {
			auto t0 = now_ms();
			while (condition && (now_ms() - t0) < millsec)
			{
				msleep(10);
			}
		}
	public:
		typedef std::function<void()> timer_handler;
		void start(timer_handler func, int timeout_ms)
		{
			cancel();
			continue_ = true;
			auto t1 = Timer::now_ms();
			thread_ = std::thread([this, func, t1, timeout_ms]() {
				while (continue_ && (Timer::now_ms() - t1 < timeout_ms))
				{
					msleep(10);
				}
				func();
			});
		}
		void cancel()
		{
			continue_ = false;
			if (thread_.joinable()) {
				thread_.join();
			}
		}

	private:
		std::thread		thread_;
		volatile bool	continue_ = false;
	};
}